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

#ifndef WFA_CA_RESP_H
#define WFA_CA_RESP_H
#include <inttypes.h>
#include <wfa_rsp.h>
#include <wfa_cmds.h>

typedef int (*dutCommandRespFuncPtr)(char *cmdBuf);

int caCmdNotDefinedYet(char *cmdBuf);
int wfaStaVerifyIpConnectResp(char *cmdBuf);
int wfaStaSetIpConfigResp(char *cmdBuf);
int wfaStaIsConnectedResp(char *cmdBuf);
int wfaStaGetIpConfigResp(char *cmdBuf);
int wfaGetVersionResp(char *cmdBuf);
int wfaStaGetInfoResp(char *cmdBuf);
int wfaTrafficAgentConfigResp(char *cmdBuf);
int wfaTrafficAgentSendResp(char *cmdBuf);
int wfaTrafficAgentRecvStartResp(char *cmdBuf);
int wfaTrafficAgentRecvStopResp(char *cmdBuf);
int wfaTrafficAgentResetResp(char *cmdBuf);
int wfaTrafficAgentPingStartResp(char *cmdBuf);
int wfaTrafficAgentPingStopResp(char *cmdBuf);
int wfaStaGetMacAddressResp(char *cmdBuf);
int wfaStaGetBSSIDResp(char *cmdBuf);
int wfaStaSetEncryptionResp(char *cmdBuf);
int wfaStaSetEapTLSResp(char *cmdBuf);
int wfaStaSetPSKResp(char *cmdBuf);
int wfaStaSetEapTTLSResp(char *cmdBuf);
int wfaStaSetEapSIMResp(char *cmdBuf);
int wfaStaSetEapPEAPResp(char *cmdBuf);
int wfaStaSetSecurityResp(char *cmdBuf);
int wfaStaAssociateResp(char *cmdBuf);
int wfaStaScanResp(char *cmdBuf);
int wfaStaSetIBSSResp(char *cmdBuf);
int wfaStaGetStatsResp(char *cmdBuf);
int wfaDeviceGetInfoResp(char *cmdBuf);
int wfaDeviceListIFResp(char *cmdBuf);
int wfaStaDevSendFrameResp(char *cmdBuf);
int wfaStaDebugSetResp(char *cmdBuf);
void wfaStaDebugHexDump(char *cmdBuf);
int wfaStaSetModeResp(char *cmdBuf);
int wfaStaUploadResp(char *cmdBuf);
int wfaStaSetUAPSDResp(char *cmdBuf);
int wfaStaSetModeResp(char *cmdBuf);
int wfaStaSetWMMResp(char *cmdBuf);
int wfaStaPresetParametersResp(char *cmdBuf);
int wfaStaSetEapFASTResp(char *cmdBuf);
int wfaStaSetEapAKAResp(char * cmdBuf);
int wfaStaSetSystimeResp(char * cmdBuf);

int wfaStaSet11nResp(char * cmdBuf);
int wfaStaSetWirelessResp(char * cmdBuf);
int wfaStaSendADDBAResp(char * cmdBuf);
int wfaStaCoexMgmtResp(char * cmdBuf);
int wfaStaRifsTestResp(char * cmdBuf);
int wfaStaResetDefaultResp(char * cmdBuf);
int wfaStaDisconnectResp(char * cmdBuf);

int wfaStandardReturn(char *cmdBuf);

int wfaStaGenericResp(char *cmdBuf);
/* P2P */
int wfaStaGetP2pDevAddressResp(char *cmdBuf);
int wfaStaSetP2pResp(char *cmdBuf);
int wfaStaP2pConnectResp(char *cmdBuf);
int wfaStaStartAutoGO(char *cmdBuf);
int wfaStaP2pStartGrpFormResp(char *cmdBuf);
int wfaStaP2pDissolveResp(char *cmdBuf);
int wfaStaSendP2pInvReqResp(char *cmdBuf);
int wfaStaAcceptP2pInvReqResp(char *cmdBuf);
int wfaStaSendP2pProvDisReqResp(char *cmdBuf);
int wfaStaSetWpsPbcResp(char *cmdBuf);
int wfaStaWpsReadPinResp(char *cmdBuf);
int wfaStaWpsEnterPinResp(char *cmdBuf);
int wfaStaGetPskResp(char *cmdBuf);
int wfaStaP2pResetResp(char *cmdBuf);
int wfaStaWpsReadLabelResp(char *cmdBuf);
int wfaStaGetP2pIpConfigResp(char *cmdBuf);
int wfaStaSendServiceDiscoveryReqResp(char *cmdBuf);
int wfaStaSendP2pPresenceReqResp(char *cmdBuf);
int wfaStaSetSleepReqResp(char *cmdBuf);
int wfaStaSetOpportunisticPsReqResp(char *cmdBuf);
int wfaStaAddArpTableEntryResp(char * cmdBuf);
int wfaStaBlockICMPResponseResp(char * cmdBuf);
/* P2P */
int wfaStaStartWfdConnectionResp(char * cmdBuf);
int wfaStaCliCmdResp(char * cmdBuf);
int wfaStaConnectGoStartWfdResp(char * cmdBuf);
int wfaStaGenericResp(char * cmdBuf);
int wfaStaGetParameterResp(char * cmdBuf);
int wfaStaNfcActionResp(char * cmdBuf);
int wfaStaInvokeCommandResp(char * cmdBuf);
int wfaStaManageServiceResp(char * cmdBuf);
int wfaStaGetEventsResp(char * cmdBuf);
int wfaStaGetEventDataResp(char * cmdBuf);
int wfaStaExecActionResp(char *cmdBuf);

/* traffic generator */
void wfaSendPing(tgPingStart_t *staPing, float *interval, int streamid);
int wfaStopPing(dutCmdResponse_t *stpResp, int streamid);

#endif
