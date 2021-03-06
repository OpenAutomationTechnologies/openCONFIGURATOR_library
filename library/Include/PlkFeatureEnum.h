/************************************************************************
\file PlkFeatureEnum.h
\brief Implementation of the Class PlkFeatureEnum
\author rueckerc, Bernecker+Rainer Industrie Elektronik Ges.m.b.H.
\date 01-May-2015 12:00:00
************************************************************************/

/*------------------------------------------------------------------------------
Copyright (c) 2015, Bernecker+Rainer Industrie-Elektronik Ges.m.b.H. (B&R)
All rights reserved.
Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the copyright holders nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDERS BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
------------------------------------------------------------------------------*/
#if !defined PLKFEATURE_ENUM_H
#define PLKFEATURE_ENUM_H

#include <cstdint>

namespace IndustrialNetwork
{
	namespace POWERLINK
	{
		namespace Core
		{
			namespace Node
			{
				/**
				\brief Represents the POWERLINK features.
				\author rueckerc, Bernecker+Rainer Industrie Elektronik Ges.m.b.H.
				*/
				enum GeneralFeatureEnum
				{
					//POWERLINK general features
					CFMConfigManager = 0,
					DLLErrBadPhysMode,
					DLLErrMacBuffer,
					DLLFeatureCN,
					DLLFeatureMN,
					NMTBootTimeNotActive,
					NMTCycleTimeGranularity,
					NMTCycleTimeMax,
					NMTCycleTimeMin,
					NMTMinRedCycleTime,
					NMTEmergencyQueueSize,
					NMTErrorEntries,
					NMTExtNmtCmds,
					NMTFlushArpEntry,
					NMTIsochronous,
					NMTNetHostNameSet,
					NMTMaxCNNodeID,
					NMTMaxCNNumber,
					NMTMaxHeartbeats,
					NMTNodeIDByHW,
					NMTNodeIDBySW,
					NMTProductCode,
					NMTPublishActiveNodes,
					NMTPublishConfigNodes,
					NMTPublishEmergencyNew,
					NMTPublishNodeState,
					NMTPublishOperational,
					NMTPublishPreOp1,
					NMTPublishPreOp2,
					NMTPublishReadyToOp,
					NMTPublishStopped,
					NMTPublishTime,
					NMTRevisionNo,
					NWLForward,
					NWLICMPSupport,
					NWLIPSupport,
					PDODynamicMapping,
					PDOGranularity,
					PDOMaxDescrMem,
					PDORPDOChannelObjects,
					PDORPDOChannels,
					PDORPDOCycleDataLim,
					PDORPDOOverallObjects,
					PDOSelfReceipt,
					PDOTPDOChannelObjects,
					PDOTPDOCycleDataLim,
					PDOTPDOOverallObjects,
					PHYExtEPLPorts,
					PHYHubDelay,
					PHYHubIntegrated,
					PHYHubJitter,
					RT1RT1SecuritySupport,
					RT1RT1Support,
					RT2RT2Support,
					SDOClient,
					SDOCmdFileRead,
					SDOCmdFileWrite,
					SDOCmdLinkName,
					SDOCmdReadAllByIndex,
					SDOCmdReadByName,
					SDOCmdReadMultParam,
					SDOCmdWriteAllByIndex,
					SDOCmdWriteByName,
					SDOCmdWriteMultParam,
					SDOMaxConnections,
					SDOMaxParallelConnections,
					SDOSeqLayerTxHistorySize,
					SDOServer,
					SDOSupportASnd,
					SDOSupportPDO,
					SDOSupportUdpIp,
					DLLMultiplePReqPRes
				};
				enum MNFeatureEnum
				{
					//POWERLINK MN features >= 72
					DLLErrMNMultipleMN = 72,
					DLLMNFeatureMultiplex,
					DLLMNPResChaining,
					DLLMNFeaturePResTx,
					NMTMNASnd2SoC,
					NMTMNBasicEthernet,
					NMTMNMultiplCycMax,
					NMTMNPRes2PReq,
					NMTMNPRes2PRes,
					NMTMNPResRx2SoA,
					NMTMNPResTx2SoA,
					NMTMNSoA2ASndTx,
					NMTMNSoC2PReq,
					NMTNetTime,
					NMTNetTimeIsRealTime,
					NMTRelativeTime,
					NMTServiceUdpIp,
					NMTSimpleBoot,
					PDOTPDOChannels,
					NMTMNDNA,
					NMTMNRedundancy,
					DLLMNRingRedundancy,
					NMTMNMaxAsynchronousSlots
				};

				enum CNFeatureEnum
				{
					//POWERLINK CN features >= 95
					DLLCNFeatureMultiplex = 95,
					DLLCNPResChaining,
					NMTCNPreOp2ToReady2Op,
					NMTCNSoC2PReq,
					NMTCNSetNodeNumberTime,
					NMTCNDNA,
					NMTCNMaxAInv
				};

				static const std::string PlkFeatureStrings[] =
				{
					"CFMConfigManager",
					"DLLErrBadPhysMode",
					"DLLErrMacBuffer",
					"DLLFeatureCN",
					"DLLFeatureMN",
					"NMTBootTimeNotActive",
					"NMTCycleTimeGranularity",
					"NMTCycleTimeMax",
					"NMTCycleTimeMin",
					"NMTMinRedCycleTime",
					"NMTEmergencyQueueSize",
					"NMTErrorEntries",
					"NMTExtNmtCmds",
					"NMTFlushArpEntry",
					"NMTIsochronous",
					"NMTNetHostNameSet",
					"NMTMaxCNNodeID",
					"NMTMaxCNNumber",
					"NMTMaxHeartbeats",
					"NMTNodeIDByHW",
					"NMTNodeIDBySW",
					"NMTProductCode",
					"NMTPublishActiveNodes",
					"NMTPublishConfigNodes",
					"NMTPublishEmergencyNew",
					"NMTPublishNodeState",
					"NMTPublishOperational",
					"NMTPublishPreOp1",
					"NMTPublishPreOp2",
					"NMTPublishReadyToOp",
					"NMTPublishStopped",
					"NMTPublishTime",
					"NMTRevisionNo",
					"NWLForward",
					"NWLICMPSupport",
					"NWLIPSupport",
					"PDODynamicMapping",
					"PDOGranularity",
					"PDOMaxDescrMem",
					"PDORPDOChannelObjects",
					"PDORPDOChannels",
					"PDORPDOCycleDataLim",
					"PDORPDOOverallObjects",
					"PDOSelfReceipt",
					"PDOTPDOChannelObjects",
					"PDOTPDOCycleDataLim",
					"PDOTPDOOverallObjects",
					"PHYExtEPLPorts",
					"PHYHubDelay",
					"PHYHubIntegrated",
					"PHYHubJitter",
					"RT1RT1SecuritySupport",
					"RT1RT1Support",
					"RT2RT2Support",
					"SDOClient",
					"SDOCmdFileRead",
					"SDOCmdFileWrite",
					"SDOCmdLinkName",
					"SDOCmdReadAllByIndex",
					"SDOCmdReadByName",
					"SDOCmdReadMultParam",
					"SDOCmdWriteAllByIndex",
					"SDOCmdWriteByName",
					"SDOCmdWriteMultParam",
					"SDOMaxConnections",
					"SDOMaxParallelConnections",
					"SDOSeqLayerTxHistorySize",
					"SDOServer",
					"SDOSupportASnd",
					"SDOSupportPDO",
					"SDOSupportUdpIp",
					"DLLMultiplePReqPRes",
					"DLLErrMNMultipleMN",
					"DLLMNFeatureMultiplex",
					"DLLMNPResChaining",
					"DLLMNFeaturePResTx",
					"NMTMNASnd2SoC",
					"NMTMNBasicEthernet",
					"NMTMNMultiplCycMax",
					"NMTMNPRes2PReq",
					"NMTMNPRes2PRes",
					"NMTMNPResRx2SoA",
					"NMTMNPResTx2SoA",
					"NMTMNSoA2ASndTx",
					"NMTMNSoC2PReq",
					"NMTNetTime",
					"NMTNetTimeIsRealTime",
					"NMTRelativeTime",
					"NMTServiceUdpIp",
					"NMTSimpleBoot",
					"PDOTPDOChannels",
					"NMTMNDNA",
					"NMTMNRedundancy",
					"DLLMNRingRedundancy",
					"NMTMNMaxAsynchronousSlots",
					"DLLCNFeatureMultiplex",
					"DLLCNPResChaining",
					"NMTCNPreOp2ToReady2Op",
					"NMTCNSoC2PReq",
					"NMTCNSetNodeNumberTime",
					"NMTCNDNA",
					"NMTCNMaxAInv"
				};

				static const std::string PlkFeatureDefaultValues[] =
				{
					"false",
					"false",
					"false",
					"true",
					"",
					"",
					"1",
					"",
					"",
					"0",
					"0",
					"",
					"false",
					"false",
					"true",
					"false",
					"239",
					"239",
					"254",
					"true",
					"false",
					"0",
					"false",
					"false",
					"false",
					"false",
					"false",
					"false",
					"false",
					"false",
					"false",
					"false",
					"0",
					"false",
					"false",
					"true",
					"true",
					"8",
					"4294967295",
					"254",
					"256",
					"4294967295",
					"65535",
					"false",
					"254",
					"4294967295",
					"65535",
					"2",
					"460",
					"true",
					"70",
					"false",
					"false",
					"false",
					"true",
					"false",
					"false",
					"false",
					"false",
					"false",
					"false",
					"false",
					"false",
					"false",
					"1",
					"1",
					"5",
					"true",
					"false",
					"false",
					"false",
					"false",
					"false",
					"true",
					"false",
					"true",
					"",
					"false",
					"0",
					"",
					"",
					"",
					"",
					"",
					"",
					"false",
					"false",
					"false",
					"false",
					"",
					"256",
					"false",
					"false",
					"false",
					"1",
					"false",
					"false",
					"",
					"",
					"0",
					"false",
					"0"
				};
			}
		}
	}
}
#endif
