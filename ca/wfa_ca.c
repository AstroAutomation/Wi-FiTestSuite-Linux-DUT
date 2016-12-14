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
 * File: wfa_ca.c
 *       This is the main program for Control Agent.
 *
 */
#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for socket(), connect(), send(), and recv() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_addr() */
#include <stdlib.h>     /* for atoi() and exit() */
#include <string.h>     /* for memset() */
#include <unistd.h>     /* for close() */
#include <sys/select.h>
#include <getopt.h>     /* for get_option */


#include "wfa_debug.h"
#include "wfa_main.h"
#include "wfa_types.h"
#include "wfa_tlv.h"
#include "wfa_tg.h"
#include "wfa_cmds.h"
#include "wfa_miscs.h"
#include "wfa_sock.h"
#include "wfa_nw_al.h"
#include "wfa_ca.h"
#include "wfa_agtctrl.h"

#define WFA_ENV_AGENT_IPADDR "WFA_ENV_AGENT_IPADDR"

extern dutCommandRespFuncPtr wfaCmdRespProcFuncTbl[];
extern typeNameStr_t nameStr[];
extern char gRespStr[];

int gtgSend, gtgRecv, gtgTransac;


char gnetIf[32] = "any";
char gCaNetIf[32] = "any";

char logPath[100] = "";

tgStream_t*    theStreams;
long          itimeout = 0;

unsigned short wfa_defined_debug = WFA_DEBUG_ERR | WFA_DEBUG_WARNING | WFA_DEBUG_INFO;
unsigned short dfd_lvl = WFA_DEBUG_DEFAULT | WFA_DEBUG_ERR | WFA_DEBUG_INFO;

/*
 * the output format can be redefined for file output.
 */

void help_usage(char* str)
{
    printf("Usage:  %s -i <eth iface> -I <interface> -P <local port> -T <type> [Options: ] \n", str);
    printf("  Options:                                                                \n"
           "     -i   --iface       ethernet interface for control agent              \n"
           "     -I   --dutif       interface either ethernet or serial device name   \n"
           "     -T   --type        type interface like serial(1) TCP(2) UDP(3)       \n"
           "     -P   --port        local sever port address to start server          \n"
           "     -d   --dutip       dut ip address in case of TCP/UDP                 \n"
           "     -r   --dutport     dut port address in case of TCP                   \n"
           "     -b   --baud        baud rate in case of serial                       \n"
           "     -g   --log         log file path for debug                           \n"
           "     -h   --help        display usage                                     \n");
}


static const struct option longIPOptions[] = {
    { "iface",      required_argument, NULL, 'i' },
    { "dutif",      required_argument, NULL, 'I' },
    { "type",       required_argument, NULL, 'T' },
    { "dutip",      required_argument, NULL, 'd' },
    { "port",       required_argument, NULL, 'P' },
    { "dutport",    required_argument, NULL, 'r' },
    { "baud",       required_argument, NULL, 'b' },
    { "log",        required_argument, NULL, 'g' },
    { "help",       no_argument, NULL, 'h' },
    { NULL,         no_argument, NULL, 0 }
};
static const char* optionString = "i:I:T:d:P:r:b:g:h";


int main(int argc, char* argv[])
{
    t_ifaceHandle  ctrlRecvHandle;
    t_ifaceHandle  dutHandle;
    int typeOfConn = 0;
    int opt;
    int optionIndex = 0;
    /* remote port for sending command over tcp */
    unsigned short remoteServPort = 0;
    unsigned short locPortNo = 0;
    int baudrate = 0;
    int retStatus = 0;

    /*char *tstr=NULL;*/
    char dutSrvIP[WFA_IP_ADDR_STR_LEN];
#if DEBUG
    int bytesRcvd;
#endif

    char cmdName[WFA_BUFF_32];
    int i, isFound = 0, nbytes, ret_status;
    uint16_t tag;
    int cmdLen = WFA_BUFF_1K;
    /* int maxfdn1; */
    char xcCmdBuf[WFA_BUFF_4K];
    char caCmdBuf[WFA_BUFF_4K];
    char pcmdBuf[WFA_BUFF_1K];
    char* pcmdStr = NULL;
    char respStr[WFA_BUFF_512];

    openlog("wfa_ca", LOG_CONS | LOG_NDELAY, 0);

    while((opt = getopt_long(argc, argv, optionString, longIPOptions, &optionIndex)) != -1) {
        switch(opt) {
            case 'i':
                printf("option -i with value %s\n", optarg);
                strncpy(gCaNetIf, optarg, 31);
                break;

            case 'I':
                printf("option -I with value %s\n", optarg);
                strncpy(gnetIf, optarg, 31);
                break;

            case 'T':
                /* interface type */
                /* none: 0, serial: 1, tcp: 2, udp: 3 */
                typeOfConn = atoi(optarg);
                break;

            case 'P':
                /* local server port*/
                locPortNo = atoi(optarg);
                break;

            case 'b':
                /* baudrate for serial operation*/
                baudrate = atoi(optarg);
                break;

            case 'd':
                if(isIpV4Addr(optarg) == WFA_ERROR) {
                    printf("invalid ip address %s\n", optarg);
                    help_usage(argv[0]);
                    exit(1);
                }

                strncpy(dutSrvIP, optarg, WFA_IP_ADDR_STR_LEN);
                break;

            case 'r':
                /* dut server port  use in case dut use TCP/UDP*/
                remoteServPort = atoi(optarg);
                break;

            case 'g':
                /* log path for debug */
                printf("option -I with value %s\n", optarg);
                strncpy(logPath, optarg, sizeof(logPath) - 1);
                break;

            case 'h':
                help_usage(argv[0]);
                exit(1);
                break;

            case ':':
                printf("option  --%s must have value \n", longIPOptions[optionIndex].name);
                break;

            case '?':
                /* getopt_long() set a variable, just keep going */
                break;
        }
    }

    if(locPortNo == 0) {
        printf("Requires local port for server to start %d\n", locPortNo);
        help_usage(argv[0]);
        exit(1);
    }

    strcpy(ctrlRecvHandle.if_attr.ipConn.device, gCaNetIf);
    ctrlRecvHandle.if_attr.ipConn.port = locPortNo;
    ctrlRecvHandle.if_attr.ipConn.sockfd = -1;
    ctrlRecvHandle.if_attr.ipConn.clientSockfd = -1;


    if((CONN_TYPE_SERIAL == typeOfConn)) {
        if(baudrate == 0) {
            printf("Invalid serial baud rate \n");
            exit(1);
        }

        dutHandle.if_attr.serial.fd = -1;
        dutHandle.if_attr.serial.baudrate = baudrate;
        strcpy(dutHandle.if_attr.serial.device, gnetIf);
    }
    else if(CONN_TYPE_TCP == typeOfConn || CONN_TYPE_UDP == typeOfConn) {
        if(remoteServPort == 0) {
            printf("Invalid remote port for IP connection\n");
            exit(1);
        }

        dutHandle.if_attr.ipConn.sockfd = -1;
        dutHandle.if_attr.ipConn.srvFlag = 0;
        dutHandle.if_attr.ipConn.port = remoteServPort;
        strcpy(dutHandle.if_attr.ipConn.srcIpaddr, dutSrvIP);
    }
    else {
        printf("type (-T) should be with correct value %d\n", typeOfConn);
        help_usage(argv[0]);
        exit(1);
    }

    /* check need in case logpath allocated as require*/
    if(logPath != NULL && strlen(logPath)) {
        FILE* logfile;
        int fd;
        logfile = fopen(logPath, "a");

        if(logfile != NULL) {
            fd = fileno(logfile);
            DPRINT_INFO(WFA_OUT, "redirecting the output to %s\n", logPath);
            dup2(fd, 1);
            dup2(fd, 2);
        }
        else {
            DPRINT_ERR(WFA_ERR, "Cant open the log file continuing without redirecting\n");
        }
    }

    /*infinite loop */
    for(;;) {
        if(ctrlRecvHandle.if_attr.ipConn.sockfd == -1) {
            printf("CA server starting \n");
            retStatus = wfaOpenInterFace(&ctrlRecvHandle, gCaNetIf, CONN_TYPE_TCP, CONNECTION_SERVER);

            if(retStatus) {
                printf("CA server failed to start \n");
                sleep(5);
                continue;
            }
        }

        wfaInterFacePeerConn(&ctrlRecvHandle);

        memset(xcCmdBuf, 0, WFA_BUFF_4K);
        memset(gRespStr, 0, WFA_BUFF_512);

        if(ctrlRecvHandle.if_attr.ipConn.clientSockfd < 0)
            // no client connection
        {
            continue;
        }

        retStatus = wfaInterFaceDataRecv(&ctrlRecvHandle, xcCmdBuf, WFA_BUFF_1K, &nbytes);

        if(nbytes < 0) {
            printf("Error: %s. Restarting connection.\n", strerror(errno));
            /* may not be correct idea unless recev wait till it gets some data*/
            wfaInterFacePeerConnClose(&ctrlRecvHandle);
            continue;
        }
        else if(nbytes == 0) {
            continue;
        }

        // clear the crlf at the end of the message
        if(xcCmdBuf[nbytes - 1] == '\n' && xcCmdBuf[nbytes - 2] == '\r') {
            xcCmdBuf[nbytes - 1] = 0;
            xcCmdBuf[nbytes - 2] = 0;
            nbytes -= 2;
        }

        // strip off trailing spaces because for some reason it causes a problem
        while(xcCmdBuf[nbytes - 1] == '\t' || xcCmdBuf[nbytes - 1] == ' ') {
            xcCmdBuf[nbytes - 1] = '\0';
            nbytes--;
        }

        if(nbytes == 0 || strlen(xcCmdBuf) == 0) {
            continue;
        }

        printf("Received command from testbed: {%s}\n", xcCmdBuf);

        /* send back to command line or TM.*/
        memset(respStr, 0, WFA_BUFF_128);
        sprintf(respStr, "status,RUNNING\r\n");
        retStatus = wfaInterFaceDataSend(&ctrlRecvHandle, respStr, strlen(respStr));

        if(CONN_TYPE_TCP == typeOfConn) {
            wfaOpenInterFace(&dutHandle, gnetIf, CONN_TYPE_TCP, CONNECTION_CLIENT);
            wfaInterFacePeerInfoSet(&dutHandle, dutSrvIP, remoteServPort, 0, typeOfConn);
            retStatus = wfaInterFacePeerConn(&dutHandle);

            if(WFA_ERROR == retStatus) {
                printf("Dut Connection failed {status,ERROR}\n");
                retStatus = wfaInterFaceDataSend(&ctrlRecvHandle, "status,ERROR\r\n", strlen("status,ERROR\r\n"));
                wfaInterFaceClose(&dutHandle);
                continue;
            }
        }
        else if(CONN_TYPE_SERIAL == typeOfConn) {
            dutHandle.if_attr.serial.baudrate = baudrate;
            wfaOpenInterFace(&dutHandle, gnetIf, CONN_TYPE_SERIAL, CONNECTION_CLIENT);
            wfaInterFacePeerInfoSet(&dutHandle, dutSrvIP, remoteServPort, 115200, typeOfConn);
        }

        isFound = 0;
        memcpy(cmdName, strtok_r((char*) xcCmdBuf, ",", (char**) &pcmdStr), sizeof(cmdName));

        i = 0;

        while(nameStr[i].type != -1) {
            if(strstr(cmdName, nameStr[i].name) != NULL) {
                isFound = 1;
                break;
            }

            i++;
        }

        if(isFound == 0) {
            sleep(1);
            sprintf(respStr, "status,INVALID,no_such_command\r\n");
            printf("Command failed {%s}\n", respStr);
            retStatus = wfaInterFaceDataSend(&ctrlRecvHandle, respStr, strlen(respStr));
            continue;
        }

        memset(pcmdBuf, 0, WFA_BUFF_1K);

        if(nameStr[i].cmdProcFunc(pcmdStr, pcmdBuf, &cmdLen) != WFA_SUCCESS) {
            sprintf(respStr, "status,INVALID,incorrect_syntax\r\n");
            printf("Command failed {%s}\n", respStr);
            retStatus = wfaInterFaceDataSend(&ctrlRecvHandle, respStr, strlen(respStr));
            continue;
        }

        /* send to DUT.*/
#ifdef DEBUG
        DPRINT_INFO(WFA_OUT, "send to DUT\n");

        for(i = 0; i < cmdLen; i++) {
            printf("%02x ", pcmdBuf[i]);
        }

        printf("\n");
#endif

        retStatus = wfaInterFaceDataSend(&dutHandle, pcmdBuf, cmdLen);

        if(retStatus == -1) {
            sprintf(respStr, "status,INVALID,send_failed\r\n");
            printf("Command failed {%s}\n", respStr);
            retStatus = wfaInterFaceDataSend(&ctrlRecvHandle, respStr, strlen(respStr));
            wfaInterFaceClose(&dutHandle);
            continue;
        }

        memset(respStr, 0, WFA_BUFF_128);
        memset(caCmdBuf, 0, WFA_BUFF_4K);
        nbytes = 0;
        retStatus = wfaInterFaceDataRecv(&dutHandle, caCmdBuf, WFA_BUFF_4K, &nbytes);



        if(retStatus != WFA_SUCCESS || nbytes <= 0) {
            sprintf(respStr, "status,INVALID,response_timeout\r\n");
            printf("Command failed {%s}\n", respStr);
            retStatus = wfaInterFaceDataSend(&ctrlRecvHandle, respStr, strlen(respStr));
            wfaInterFaceClose(&dutHandle);
            continue;
        }
        else {
#ifdef DEBUG
            DPRINT_INFO(WFA_OUT, "read from DUT\n");

            for(i = 0; i < nbytes; i++) {
                DPRINT_INFO(WFA_OUT, "%02x ", (caCmdBuf[i] & 0xff));
            }

            DPRINT_INFO(WFA_OUT, "\n");
#endif
            wfaTLV* tlvHdr = (wfaTLV*) caCmdBuf;
            tag = tlvHdr->tag;
            memcpy(&ret_status, caCmdBuf + WFA_TLV_HDR_LEN, 4);

            if(tag != 0 && wfaCmdRespProcFuncTbl[tag] != NULL) {
                wfaCmdRespProcFuncTbl[tag](caCmdBuf);
                printf("Command response to testbed {%s}\n", gRespStr);
                /*send back to ctrl agent*/
                retStatus = wfaInterFaceDataSend(&ctrlRecvHandle, gRespStr, strlen(gRespStr));
            }
            else {
                DPRINT_WARNING(WFA_WNG, "function not defined\n");
                sprintf(respStr, "status,INVALID\r\n");
                retStatus = wfaInterFaceDataSend(&ctrlRecvHandle, respStr, strlen(respStr));
            }
        }

        /* close the dut connection */
        wfaInterFaceClose(&dutHandle);
    } /* for */

    wfaInterFacePeerConnClose(&ctrlRecvHandle);
    wfaInterFaceClose(&ctrlRecvHandle);

    closelog();

    return 0;
}
