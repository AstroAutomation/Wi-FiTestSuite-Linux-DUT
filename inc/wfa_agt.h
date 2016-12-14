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

#ifndef WFA_AGT_H
#define WFA_AGT_H

typedef int (*xcCommandFuncPtr)(int len, char *parms, int *respLen, char *respBuf);

int agtCmdProcGetVersion(int len, char *parms, int *respLen, char *respBuf);

int cmdProcNotDefinedYet(char *, char *, int *);
int xcCmdProcGetVersion(char *, char *, int *);
int xcCmdProcAgentConfig(char *, char *, int *);
int xcCmdProcAgentSend(char *, char *, int *);
int xcCmdProcAgentRecvStart(char *, char *, int *);
int xcCmdProcAgentRecvStop(char *, char *, int *);
int xcCmdProcAgentReset(char *, char *, int *);
int xcCmdProcStaGetIpConfig(char *, char *, int *);
int xcCmdProcStaSetIpConfig(char *, char *, int *);
int xcCmdProcStaGetMacAddress(char *pcmdStr, char *, int *);
int xcCmdProcStaSetMacAddress(char *pcmdStr, char *, int *);
int xcCmdProcStaIsConnected(char *pcmdStr, char *, int *);
int xcCmdProcStaVerifyIpConnection(char *pcmdStr, char *, int *);
int xcCmdProcStaGetBSSID(char *pcmdStr, char *, int *);
int xcCmdProcStaGetStats(char *pcmdStr, char *, int *);
int xcCmdProcStaSetEncryption(char *pcmdStr, char *, int *);
int xcCmdProcStaSetPSK(char *pcmdStr, char *, int *);
int xcCmdProcStaSetEapTLS(char *pcmdStr, char *, int *);
int xcCmdProcStaSetEapTTLS(char *pcmdStr, char *, int *);
int xcCmdProcStaSetEapSIM(char *pcmdStr, char *, int *);
int xcCmdProcStaSetPEAP(char *pcmdStr, char *, int *);
int xcCmdProcStaSetIBSS(char *pcmdStr, char *, int *);
int xcCmdProcDeviceGetInfo(char *pcmdStr, char *, int *);
int xcCmdProcDeviceListIF(char *pcmdStr, char *, int *);
int xcCmdProcStaAssociate(char *pcmdStr, char *, int *);
int xcCmdProcStaSetUAPSD(char *pcmdStr, char *, int *);
int xcCmdProcStaGetInfo(char *pcmdStr, char *, int *);
int xcCmdProcAgentSendPing(char *pcmdStr, char *, int *);
int xcCmdProcAgentStopPing(char *pcmdStr, char *, int *);
int xcCmdProcStaDebugSet(char *pcmdStr, char *, int *);
int xcCmdProcStaUpload(char *pcmdStr, char *, int *);
int xcCmdProcStaSetMode(char *pcmStr, char *, int *);
int xcCmdProcStaSetWMM(char *pcmStr, char *, int *);
int xcCmdProcStaPresetTestParameters(char *pcmStr, char *, int *);
int xcCmdProcStaSetEapFAST(char *pcmStr, char *, int *);
int xcCmdProcStaSetEapAKA(char *pcmStr, char *, int *);
int xcCmdProcStaSetSystime(char *pcmStr, char *, int *);

int xcCmdProcStaSet11n(char *pcmStr, char *, int *);
int xcCmdProcStaSetWireless(char *pcmStr, char *, int *);
int xcCmdProcStaSendADDBA(char *pcmStr, char *, int *);
int xcCmdProcStaSetRIFS(char *, char *, int *);
int xcCmdProcStaSendCoExistMGMT(char *, char *, int *);
int xcCmdProcStaResetDefault(char *, char *, int *);
int xcCmdProcStaDisconnect(char *, char *, int *);

int xcCmdProcStaReAssociate(char *pcmStr, char*, int *);
int xcCmdProcStaResetDefault(char *pcmdStr, char *aBuf, int *aLen);
int xcCmdProcStaSetPwrSave(char *pcmdStr, char *aBuf, int *aLen);

int xcCmdProcStaGetP2pDevAddress(char *pcmStr, char *, int *);
int xcCmdProcStaSetP2p(char *pcmStr, char *, int *);
int xcCmdProcStaP2pConnect(char *pcmStr, char *, int *);
int xcCmdProcStaP2pStartAutoGo(char *pcmStr, char *, int *);
int xcCmdProcStaP2pStartGroupFormation(char *pcmStr, char *, int *);
int xcCmdProcStaP2pDissolve(char *pcmStr, char *, int *);
int xcCmdProcStaSendP2pInvReq(char *pcmStr, char *, int *);
int xcCmdProcStaAcceptP2pInvReq(char *pcmStr, char *, int *);
int xcCmdProcStaSendP2pProvDisReq(char *pcmStr, char *, int *);
int xcCmdProcStaSetWpsPbc(char *pcmStr, char *, int *);
int xcCmdProcStaWpsReadPin(char *pcmStr, char *, int *);
int xcCmdProcStaWpsEnterPin(char *pcmStr, char *, int *);
int xcCmdProcStaGetPsk(char *pcmStr, char *, int *);
int xcCmdProcStaP2pReset(char *pcmStr, char *, int *);
int xcCmdProcStaWpsReadLabel(char *pcmStr, char *, int *);
int xcCmdProcStaGetP2pIpConfig(char *pcmStr, char *, int *);
int xcCmdProcStaSendServiceDiscoveryReq(char *pcmStr, char *, int *);
int xcCmdProcStaSendP2pPresenceReq(char *pcmStr, char *, int *);
int xcCmdProcStaSetSleepReq(char *pcmStr, char *, int *);
int xcCmdProcStaSetOpportunistcPsReq(char *pcmStr, char *, int *);
int xcCmdProcStaAddARPTableEntry(char *, char *, int *);
int xcCmdProcStaBlockICMPResponse(char *, char *, int *);

int xcCmdProcStaDevSendFrame(char *pcmdStr, char *aBuf, int *aLen);
int xcCmdProcStaSetSecurity(char *pcmdStr, char *aBuf, int *aLen);

int xcCmdProcStaSetRadio(char *pcmdStr, char *aBuf, int *aLen);
int xcCmdProcStaSetRFeature(char *pcmdStr, char *aBuf, int *aLen);

int xcCmdProcStaStartWfdConnection(char *pcmdStr, char *aBuf, int *aLen);
int xcCmdProcStaCliCommand(char *, char *, int *);

int xcCmdProcStaConnectGoStartWfd(char *pcmdStr, char *aBuf, int *aLen);
int xcCmdProcStaGenerateEvent(char *, char *, int *);
int xcCmdProcStaReinvokeWfdSession(char *, char *, int *);
int xcCmdProcStaGetParameter(char *, char *, int *);


int xcCmdProcStaNfcAction(char *, char *, int *);
int xcCmdProcStaExecAction(char *pcmdStr, char *, int *);

int xcCmdProcStaInvokeCommand(char *, char *, int *);
int xcCmdProcStaManageService(char *, char *, int *);

int xcCmdProcStaGetEvents(char *pcmdStr, char *, int *);
int xcCmdProcStaGetEventDetails(char *, char *, int *);
int xcCmdProcApBroadcast(char*, char*, int*);

#endif
