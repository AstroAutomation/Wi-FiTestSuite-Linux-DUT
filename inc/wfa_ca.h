/****************************************************************************
*
* Copyright (c) 2015 Wi-Fi Alliance
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

#ifndef _WFA_CA_H_
#define _WFA_CA_H_
#include <inttypes.h>

#define ENCRYPT_NONE         0
#define ENCRYPT_WEP          1
#define ENCRYPT_TKIP         2
#define ENCRYPT_AESCCMP      3
#define ENCRYPT_AESCCMP_TKIP 4

#define WMMAC_UPLINK         0
#define WMMAC_DOWNLINK       1
#define WMMAC_BIDIR          3
#define GROUP_WMMAC          0
#define GROUP_WMMCONF        1
#define WMMAC_ADDTS          0
#define WMMAC_DELTS          1
#define WMMAC_AC_BE          0
#define WMMAC_AC_BK          1
#define WMMAC_AC_VI          2
#define WMMAC_AC_VO          3

int wfaStaAssociate(int len, char *caCmdBuf, int *respLen, char *respBuf);
int wfaStaIsConnected(int len, char *caCmdBuf, int *respLen, char *respBuf);
int wfaStaGetIpConfig(int len, char *caCmdBuf, int *respLen, char *respBuf);
int wfaStaSetIpConfig(int len, char *caCmdBuf, int *respLen, char *respBuf);
int wfaStaVerifyIpConnection(int len, char *caCmdBuf, int *respLen, char *respBuf);
int wfaStaGetMacAddress(int len, char *caCmdBuf, int *respLen, char *respBuf);
int wfaStaSetMacAddr(int len, char *caCmdBuf, int *respLen, char *respBuf);

int wfaStaGetBSSID(int len, char *caCmdBuf, int *respLen, char *respBuf);
int wfaStaGetStats(int len, char *caCmdBuf, int *respLen, char *respBuf);
int wfaSetEncryption(int len, char *caCmdBuf, int *respLen, char *respBuf);
int wfaStaSetEapTLS(int len, char *caCmdBuf, int *respLen, char *respBuf);
int wfaStaSetPSK(int len, char *caCmdBuf, int *respLen, char *respBuf);
int wfaStaGetInfo(int len, char *caCmdBuf, int *respLen, char *respBuf);
int wfaDeviceGetInfo(int len, char *caCmdBuf, int *respLen, char *respBuf);
int wfaDeviceListIF(int len, char *caCmdBuf, int *respLen, char *respBuf);
int wfaStaSetEapTTLS(int len, char *caCmdBuf, int *respLen, char *respBuf);
int wfaStaSetEapSim(int len, char *caCmdBuf, int *respLen, char *respBuf);
int wfaStaSetPEAP(int len, char *caCmdBuf, int *respLen, char *respBuf);
int wfaStaSetEapSIM(int len, char *caCmdBuf, int *respLen, char *respBuf);
int wfaStaSetPEAP(int len, char *caCmdBuf, int *respLen, char *respBuf);
int wfaStaSetUAPSD(int len, char *caCmdBuf, int *respLen, char *respBuf);
int wfaStaSetIBSS(int len, char *caCmdBuf, int *respLen, char *respBuf);
int wfaStaDebugSet(int len, char *caCmdBuf, int *respLen, char *respBuf);
int wfaStaSetMode(int, char *, int *, char *);
int wfaStaUpload(int, char *, int *, char *);
int wfaStaSetWMM(int, char *, int *, char *);

int wfaStaPresetParams(int len, char *caCmdBuf, int *respLen, char *respBuf);
int wfaStaSetEapFAST(int len, char *caCmdBuf, int *respLen, char *respBuf);
int wfaStaSetEapAKA(int len, char *caCmdBuf, int *respLen, char *respBuf);
int wfaStaSetSystime(int len, char *caCmdBuf, int *respLen, char *respBuf);
int wfaStaResetTspecs(char* ifname);
int wfaStaSet11n(int len, char *caCmdBuf, int *respLen, char *respBuf);
int wfaStaSetWireless(int len, char *caCmdBuf, int *respLen, char *respBuf);
int wfaStaSendADDBA(int len, char *caCmdBuf, int *respLen, char *respBuf);
int wfaStaSetRIFS(int len, char *caCmdBuf, int *respLen, char *respBuf);
int wfaStaSendCoExistMGMT(int len, char *caCmdBuf, int *respLen, char *respBuf);
int wfaStaResetDefault(int len, char *caCmdBuf, int *respLen, char *respBuf);
int wfaStaDisconnect(int len, char *caCmdBuf, int *respLen, char *respBuf);
int wfaStaReAssociate(int, char*, int*, char*);

int wfaStaSetPwrSave(int, char *, int *, char *);
int wfaStaResetDefault(int, char *, int *, char *);
int wfaStaSendNeigReq(int, char *, int *, char *);
int wfaStaTestBedCmd(int len, char *caCmdBuf, int *respLen, char *respBuf);
int wfaStaSendFrame(int len, char *caCmdBuf, int *respLen, char *respBuf);
int wfaStaGetP2pDevAddress(int len, char *caCmdBuf, int *respLen, char *respBuf);
int wfaStaSetP2p(int len, char *caCmdBuf, int *respLen, char *respBuf);
int wfaStaP2pConnect(int len, char *caCmdBuf, int *respLen, char *respBuf);
int wfaStaStartAutoGo(int len, char *caCmdBuf, int *respLen, char *respBuf);
int wfaStaP2pStartGrpFormation(int len, char *caCmdBuf, int *respLen, char *respBuf);

int wfaStaP2pDissolve(int len, char *caCmdBuf, int *respLen, char *respBuf);
int wfaStaSendP2pInvReq(int len, char *caCmdBuf, int *respLen, char *respBuf);
int wfaStaAcceptP2pInvReq(int len, char *caCmdBuf, int *respLen, char *respBuf);
int wfaStaSendP2pProvDisReq(int len, char *caCmdBuf, int *respLen, char *respBuf);
int wfaStaSetWpsPbc(int len, char *caCmdBuf, int *respLen, char *respBuf);

int wfaStaWpsReadPin(int len, char *caCmdBuf, int *respLen, char *respBuf);
int wfaStaWpsEnterPin(int len, char *caCmdBuf, int *respLen, char *respBuf);
int wfaStaGetPsk(int len, char *caCmdBuf, int *respLen, char *respBuf);
int wfaStaP2pReset(int len, char *caCmdBuf, int *respLen, char *respBuf);
int wfaStaWpsReadLabel(int len, char *caCmdBuf, int *respLen, char *respBuf);
int wfaStaGetP2pIpConfig(int len, char *caCmdBuf, int *respLen, char *respBuf);
int wfaStaSendServiceDiscoveryReq(int len, char *caCmdBuf, int *respLen, char *respBuf);
int wfaStaSendP2pPresenceReq(int len, char *caCmdBuf, int *respLen, char *respBuf);
int wfaStaSetSleepReq(int len, char *caCmdBuf, int *respLen, char *respBuf);
int wfaStaSetOpportunisticPsReq(int len, char *caCmdBuf, int *respLen, char *respBuf);
int wfaStaAddArpTableEntry(int len, char *caCmdBuf, int *respLen, char *respBuf);
int wfaStaBlockICMPResponse(int len, char *caCmdBuf, int *respLen, char *respBuf);
int wfaStaDevSendFrame(int len, char *caCmdBuf, int *respLen, char *respBuf);
int wfaStaSetRadio(int len, char *caCmdBuf, int *respLen, char *respBuf);
int wfaStaSetRFeature(int len, char *caCmdBuf, int *respLen, char *respBuf);
int wfaStaSetSecurity(int len, char *caCmdBuf, int *respLen, char *respBuf);
int wfaStaScan(int len, char *caCmdBuf, int *respLen, char *respBuf);

int wfaStaStartWfdConnection(int len, char *caCmdBuf, int *respLen, char *respBuf);
int wfaStaCliCommand(int len, char *caCmdBuf, int *respLen, char *respBuf);
int wfaStaConnectGoStartWfd(int len, char *caCmdBuf, int *respLen, char *respBuf);
int wfaStaGenerateEvent(int len, char *caCmdBuf, int *respLen, char *respBuf);
int wfaStaReinvokeWfdSession(int len, char *caCmdBuf, int *respLen, char *respBuf);
int wfaStaGetParameter(int len, char *caCmdBuf, int *respLen, char *respBuf);
int wfaStaNfcAction(int len, char *caCmdBuf, int *respLen, char *respBuf);
int wfaStaInvokeCommand(int len, char *caCmdBuf, int *respLen, char *respBuf);
int wfaStaManageService(int len, char *caCmdBuf, int *respLen, char *respBuf);
int wfaStaGetEvents(int len, char *caCmdBuf, int *respLen, char *respBuf);
int wfaStaGetEventDetails(int len, char *caCmdBuf, int *respLen, char *respBuf);
int wfaStaExecAction(int len, char *caCmdBuf, int *respLen, char *respBuf);

#endif
