/****************************************************************************
*
* Copyright (c) 2016 Wi-Fi Alliance
*
* Permission to use, copy, modify, and/or distribute this software for any
* purpose with or without fee is hereby granted, provided that the above
* copyright notice and this permission notice appear in all copies.
*
* THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
* WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
* MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
* SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER
* RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT,
* NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE
* USE OR PERFORMANCE OF THIS SOFTWARE.
*
*****************************************************************************/

/*
 * File: wfa_dut.c - The main program for DUT agent.
 *       This is the top level of traffic control. It initializes a local TCP
 *       socket for command and control link and waits for a connect request
 *       from a Control Agent. Once the the connection is established, it
 *       will process the commands from the Control Agent. For details, please
 *       reference the architecture documents.
 *
 */

#include <pthread.h>
#include <signal.h>
#include <time.h>
#include <getopt.h>
#include <inttypes.h>
#include <stdbool.h>


#include "wfa_portall.h"
#include "wfa_debug.h"
#include "wfa_main.h"
#include "wfa_types.h"
#include "wfa_dut.h"
#include "wfa_sock.h"
#include "wfa_tlv.h"
#include "wfa_tg.h"
#include "wfa_miscs.h"
#include "wfa_agt.h"
#include "wfa_rsp.h"
#include "wfa_wmmps.h"
#include "wfa_nw_al.h"

/* Global flags for synchronizing the TG functions */
int        gtimeOut = 0;        /* timeout value for select call in usec */

#ifdef WFA_WMM_PS_EXT
extern bool gtgWmmPS;
extern unsigned long psTxMsg[512];
extern unsigned long psRxMsg[512];
extern wfaWmmPS_t wmmps_info;
extern int  psSockfd;
extern struct apts_msg* apts_msgs;

#endif /* WFA_WMM_PS_EXT */

extern     int adj_latency;           /* adjust sleep time due to latency */
char       gnetIf[WFA_BUFF_32];        /* specify the interface to use */

extern char*   trafficBuf, *respBuf;

/* stream table */
extern tgStream_t gStreams[];         /* streams' buffers             */


/* the WMM traffic streams socket fds - Socket Handler table */
extern int tgSockfds[];

extern     xcCommandFuncPtr gWfaCmdFuncTbl[]; /* command process functions */
extern     char gCmdStr[];
extern     tgStream_t* findStreamProfile(int);
extern     int clock_drift_ps;

dutCmdResponse_t gGenericResp;

/* Debug message flags */
unsigned short wfa_defined_debug = WFA_DEBUG_ERR | WFA_DEBUG_WARNING | WFA_DEBUG_INFO;
unsigned short dfd_lvl = WFA_DEBUG_DEFAULT | WFA_DEBUG_ERR | WFA_DEBUG_INFO;

/*
 * Thread Synchronize flags
 */
tgWMM_t wmm_thr[WFA_THREADS_NUM];

extern double gtgPktRTDelay;

#define DEBUG 0

extern int wfa_estimate_timer_latency();



void help_usage(char* str)
{
    printf("Usage:  %s <command interface> <type> <Local Control Port><baud rate> \n", str);
    printf("  Options:                                                              \n"
           "     -I   --iface      interface either Ethernet or serial device name \n"
           "     -T   --type       type interface like serial(1) TCP(2) UDP(3)      \n"
           "     -P   --port       port address in case of TCP                      \n"
           "     -b   --baud       baud rate in case of serial                      \n"
           "     -h   --help       display usage                                    \n");
}

static const struct option longIPOptions[] = {
    { "iface", required_argument, NULL, 'I' },
    { "type",  required_argument, NULL, 'T' },
    { "port",  required_argument, NULL, 'P' },
    { "baud",  required_argument, NULL, 'b' },
    { "help",  no_argument, NULL, 'h' },
    { NULL,    no_argument, NULL, 0 }
};
static const char* optionString = "I:T:P:b:h?";


int
main(int argc, char** argv)
{
    t_ifaceHandle  dutHandle;
    int         typeOfConn = 0;
    int         baudrate = 0;
    int         retStatus;
    int         opt;
    int         optionIndex = 0;
    int         nbytes = 0;
    int         cmdLen = 0;
    int         isExit = 1;
    int         respLen;
    uint16_t        locPortNo = 0;      /* local control port number                  */
    char*        xcCmdBuf = NULL;
    char*        parmsVal = NULL;
    struct timeval* toutvalp = NULL; /* Timeout for select()           */
    uint16_t      xcCmdTag;

    tgThrData_t tdata[WFA_THREADS_NUM];
    int i = 0;
    pthread_attr_t ptAttr;
    int ptPolicy;
    struct sched_param ptSchedParam;


    openlog("wfa_dut", LOG_CONS | LOG_NDELAY, 0);
    printf("Starting WFA DUT.\n");

    while((opt = getopt_long(argc, argv, optionString, longIPOptions, &optionIndex)) != -1) {
        switch(opt) {
            case 'I':
                printf("option -I with value %s \n", optarg);
                strncpy(gnetIf, optarg, 31);
                break;

            case 'T':
                /*none: 0,  serial: 1, tcp: 2, udp: 3*/
                typeOfConn = atoi(optarg);
                break;

            case 'P':
                /* local server port*/
                locPortNo = atoi(optarg);
                break;

            case 'b':
                baudrate = atoi(optarg);
                break;

            case 'h':
                help_usage(argv[0]);
                exit(1);
                break;

            case ':':
                printf("option  --%s must have value \n", longIPOptions[optionIndex].name);
                exit(1);
                break;

            case '?':
                /* getopt_long() set a variable, just keep going */
                break;
        }
    }

    if(CONN_TYPE_SERIAL == typeOfConn) {
        if(baudrate == 0) {
            printf("wrong baud rate \n");
            exit(1);
        }
    }
    else if((CONN_TYPE_TCP == typeOfConn) || CONN_TYPE_UDP == typeOfConn) {
        if(locPortNo == 0) {
            printf("wrong local port for IP connection\n");
            exit(1);
        }
    }
    else {
        printf("type (-T) should be with correct value %d\n", typeOfConn);
        help_usage(argv[0]);
        exit(1);
    }

    /* allocate the traffic stream table */
    wfa_dut_init(&trafficBuf, &respBuf, &parmsVal, &xcCmdBuf, &toutvalp);

    if(CONN_TYPE_TCP == typeOfConn) {
        dutHandle.if_attr.ipConn.port = locPortNo;
        retStatus = wfaOpenInterFace(&dutHandle, gnetIf, CONN_TYPE_TCP, CONNECTION_SERVER);

        if(retStatus == WFA_ERROR) {
            exit(1);
        }
    }
    else if(CONN_TYPE_SERIAL == typeOfConn) {
        dutHandle.if_attr.serial.baudrate = baudrate;
        wfaOpenInterFace(&dutHandle, gnetIf, CONN_TYPE_SERIAL, CONNECTION_CLIENT);
    }


    pthread_attr_init(&ptAttr);

    ptSchedParam.sched_priority = 10;
    pthread_attr_setschedparam(&ptAttr, &ptSchedParam);
    pthread_attr_getschedpolicy(&ptAttr, &ptPolicy);
    pthread_attr_setschedpolicy(&ptAttr, SCHED_RR);
    pthread_attr_getschedpolicy(&ptAttr, &ptPolicy);

    /*
     * Create multiple threads for WMM Stream processing.
     */
    for(i = 0; i < WFA_THREADS_NUM; i++) {
        tdata[i].tid = i;
        tdata[i].dutHandle = &dutHandle;
        pthread_mutex_init(&wmm_thr[i].thr_flag_mutex, NULL);
        pthread_cond_init(&wmm_thr[i].thr_flag_cond, NULL);
        wmm_thr[i].thr_id = pthread_create(&wmm_thr[i].thr,
                                           &ptAttr, wfa_wmm_thread, &tdata[i]);
    }

    for(i = 0; i < WFA_MAX_TRAFFIC_STREAMS; i++) {
        tgSockfds[i] = -1;
    }

    DPRINT_INFO(WFA_OUT, "ready to receive\n");

    while(isExit) {
        /*
         * The timer will be set for transaction traffic if no echo is back
         * The timeout from the select call force to send a new packet
         */
        if(gtimeOut != 0) {
            /* timeout is set to usec */
            wfaSetTimer(1, gtimeOut * 1000, toutvalp);
        }

        /* need to check for tcp connection from client*/
        wfaInterFacePeerConn(&dutHandle);

        /* we just need to check client connecion*/
        memset(xcCmdBuf, 0, WFA_BUFF_1K);    /* reset the buffer */

        nbytes = 0;
        retStatus = wfaInterFaceDataRecv(&dutHandle, xcCmdBuf, WFA_BUFF_1K, &nbytes);

        if(nbytes < 0) {
            DPRINT_ERR(WFA_ERR, "data receive error\n");
            /* may not be correct idea unless recev wait till it gets some data*/
//          wfaInterFacePeerConnClose(&dutHandle);
//          continue;
        }
        else if(nbytes > 0) {
            /* command received */
            DPRINT_INFO(WFA_OUT, "recv cmd nbyte %d\n", nbytes);
#ifdef DEBUG

            for(i = 0; i < nbytes; i++) {
                printf("%02x ", xcCmdBuf[i] & 0xff);
            }

            printf("\n");
#endif

            memset(parmsVal, 0, MAX_PARMS_BUFF);
            wfaDecodeTLV(xcCmdBuf, nbytes, &xcCmdTag, &cmdLen, parmsVal);

            DPRINT_INFO(WFA_OUT, "tag = %d\n", xcCmdTag);
            memset(respBuf, 0, WFA_RESP_BUF_SZ);
            respLen = 0;

            /* reset two commond storages used by control functions */
            memset(gCmdStr, 0, WFA_CMD_STR_SZ);
            memset(&gGenericResp, 0, sizeof(dutCmdResponse_t));

            /* command process function defined in wfa_ca.c and wfa_tg.c */
            if(xcCmdTag != 0 && gWfaCmdFuncTbl[xcCmdTag] != NULL) {
                /* since the new commands are expanded to new block */
                gWfaCmdFuncTbl[xcCmdTag](cmdLen, parmsVal, &respLen, (char*) respBuf);
            }
            else {
                /* no command defined */
                gWfaCmdFuncTbl[0](cmdLen, parmsVal, &respLen, (char*) respBuf);
            }

            DPRINT_INFO(WFA_OUT, "send cmd nbyte %d\n", nbytes);

#ifdef DEBUG

            for(i = 0; i < respLen; i++) {
                printf("%02x ", respBuf[i] & 0xff);
            }

            printf("\n");
#endif

            retStatus = wfaInterFaceDataSend(&dutHandle, respBuf, respLen);

            if(retStatus == -1) {
                DPRINT_WARNING(WFA_WNG, "wfa-wfaCtrlSend Error\n");
            }
        }

    }/* end while isExit*/

    /* close the client connection after serving one command*/
    wfaInterFacePeerConnClose(&dutHandle);
    /*
     * necessarily free all mallocs for flat memory real-time systems
     */
    free(trafficBuf);
    free(toutvalp);
    free(respBuf);
    free(xcCmdBuf);
    free(parmsVal);

    for(i = 0; i < WFA_MAX_TRAFFIC_STREAMS; i++) {
        if(tgSockfds[i] != -1) {
            close(tgSockfds[i]);
            tgSockfds[i] = -1;
        }
    }

    DPRINT_INFO(WFA_OUT, "exit from application\n");

    closelog();
    return 0;
}
