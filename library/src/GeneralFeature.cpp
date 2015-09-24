/************************************************************************
\file GeneralFeature.cpp
\brief Implementation of the Class GeneralFeature
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
#include "GeneralFeature.h"

using namespace IndustrialNetwork::POWERLINK::Core::ErrorHandling;
using namespace IndustrialNetwork::POWERLINK::Core::Utilities;
using namespace IndustrialNetwork::POWERLINK::Core::CoreConfiguration;

namespace IndustrialNetwork
{
	namespace POWERLINK
	{
		namespace Core
		{
			namespace Node
			{
				GeneralFeature::GeneralFeature(GeneralFeatureEnum type) : PlkFeature<GeneralFeatureEnum>(type)
				{
					SetTypedValues(PlkFeatureDefaultValues[type], "");
				}

				GeneralFeature::~GeneralFeature()
				{}

				const std::string& GeneralFeature::GetName() const
				{
					return PlkFeatureStrings[this->GetFeatureId()];
				}

				template<class T>
				Result GeneralFeature::GetDefaultValue(T& value)
				{
					if (this->GetUntypedDefaultValue().type() == typeid(T))
					{
						//return original stored value
						value = boost::any_cast<T>(this->GetUntypedDefaultValue());
						return Result();
					}
					//Datatype does not match
					boost::format formatter(kMsgDatatypeMismatch);
					formatter
					% typeid(T).name()
					% this->GetUntypedDefaultValue().type().name();
					LOG_FATAL() << formatter.str();
					return Result(ErrorCode::DATATYPE_MISMATCH, formatter.str());
				}
				template Result GeneralFeature::GetDefaultValue(bool& value);
				template Result GeneralFeature::GetDefaultValue(std::uint16_t& value);
				template Result GeneralFeature::GetDefaultValue(std::uint32_t& value);

				Result GeneralFeature::SetTypedValues(const std::string& defaultValue, const std::string& actualValue)
				{
					try
					{
						switch (this->GetFeatureId())
						{
							case GeneralFeatureEnum::CFMConfigManager:
							case GeneralFeatureEnum::DLLErrBadPhysMode:
							case GeneralFeatureEnum::DLLErrMacBuffer:
							case GeneralFeatureEnum::DLLFeatureCN:
							case GeneralFeatureEnum::DLLFeatureMN:
							case GeneralFeatureEnum::NMTExtNmtCmds:
							case GeneralFeatureEnum::NMTFlushArpEntry:
							case GeneralFeatureEnum::NMTIsochronous:
							case GeneralFeatureEnum::NMTNetHostNameSet:
							case GeneralFeatureEnum::NMTNodeIDByHW:
							case GeneralFeatureEnum::NMTNodeIDBySW:
							case GeneralFeatureEnum::NMTPublishActiveNodes:
							case GeneralFeatureEnum::NMTPublishConfigNodes:
							case GeneralFeatureEnum::NMTPublishEmergencyNew:
							case GeneralFeatureEnum::NMTPublishNodeState:
							case GeneralFeatureEnum::NMTPublishOperational:
							case GeneralFeatureEnum::NMTPublishPreOp1:
							case GeneralFeatureEnum::NMTPublishPreOp2:
							case GeneralFeatureEnum::NMTPublishReadyToOp:
							case GeneralFeatureEnum::NMTPublishStopped:
							case GeneralFeatureEnum::NMTPublishTime:
							case GeneralFeatureEnum::NWLForward:
							case GeneralFeatureEnum::NWLICMPSupport:
							case GeneralFeatureEnum::NWLIPSupport:
							case GeneralFeatureEnum::PDODynamicMapping:
							case GeneralFeatureEnum::PDOSelfReceipt:
							case GeneralFeatureEnum::PHYHubIntegrated:
							case GeneralFeatureEnum::RT1RT1SecuritySupport:
							case GeneralFeatureEnum::RT1RT1Support:
							case GeneralFeatureEnum::RT2RT2Support:
							case GeneralFeatureEnum::SDOClient:
							case GeneralFeatureEnum::SDOCmdFileRead:
							case GeneralFeatureEnum::SDOCmdFileWrite:
							case GeneralFeatureEnum::SDOCmdLinkName:
							case GeneralFeatureEnum::SDOCmdReadAllByIndex:
							case GeneralFeatureEnum::SDOCmdReadByName:
							case GeneralFeatureEnum::SDOCmdReadMultParam:
							case GeneralFeatureEnum::SDOCmdWriteAllByIndex:
							case GeneralFeatureEnum::SDOCmdWriteByName:
							case GeneralFeatureEnum::SDOCmdWriteMultParam:
							case GeneralFeatureEnum::SDOServer:
							case GeneralFeatureEnum::SDOSupportASnd:
							case GeneralFeatureEnum::SDOSupportPDO:
							case GeneralFeatureEnum::SDOSupportUdpIp:
							case GeneralFeatureEnum::DLLMultiplePReqPRes:
								{
									if (!defaultValue.empty())
									{
										bool value = StringToBool(defaultValue);
										this->SetUntypedDefaultValue(boost::any(value));
										break;
									}
									if (!actualValue.empty())
									{
										bool value = StringToBool(actualValue);
										this->SetUntypedActualValue(boost::any(value));
										break;
									}
								}
							case GeneralFeatureEnum::NMTBootTimeNotActive:
							case GeneralFeatureEnum::NMTCycleTimeGranularity:
							case GeneralFeatureEnum::NMTCycleTimeMax:
							case GeneralFeatureEnum::NMTCycleTimeMin:
							case GeneralFeatureEnum::NMTMinRedCycleTime:
							case GeneralFeatureEnum::NMTEmergencyQueueSize:
							case GeneralFeatureEnum::NMTErrorEntries:
							case GeneralFeatureEnum::NMTProductCode:
							case GeneralFeatureEnum::NMTRevisionNo:
							case GeneralFeatureEnum::PDOMaxDescrMem:
							case GeneralFeatureEnum::PDORPDOCycleDataLim:
							case GeneralFeatureEnum::PDOTPDOCycleDataLim:
							case GeneralFeatureEnum::PHYHubDelay:
							case GeneralFeatureEnum::PHYHubJitter:
							case GeneralFeatureEnum::SDOMaxConnections:
							case GeneralFeatureEnum::SDOMaxParallelConnections:
								{
									if (!defaultValue.empty())
									{
										std::uint32_t value = HexToInt<std::uint32_t>(defaultValue);
										this->SetUntypedDefaultValue(boost::any(value));
										break;
									}
									if (!actualValue.empty())
									{
										std::uint32_t value = HexToInt<std::uint32_t>(actualValue);
										this->SetUntypedActualValue(boost::any(value));
										break;
									}

								}
							case GeneralFeatureEnum::NMTMaxCNNodeID:
							case GeneralFeatureEnum::NMTMaxCNNumber:
							case GeneralFeatureEnum::NMTMaxHeartbeats:
							case GeneralFeatureEnum::PDOGranularity:
							case GeneralFeatureEnum::PDORPDOChannelObjects:
							case GeneralFeatureEnum::PDOTPDOChannelObjects:
							case GeneralFeatureEnum::PHYExtEPLPorts:
							case GeneralFeatureEnum::PDORPDOChannels:
							case GeneralFeatureEnum::PDORPDOOverallObjects:
							case GeneralFeatureEnum::PDOTPDOOverallObjects:
							case GeneralFeatureEnum::SDOSeqLayerTxHistorySize:
								{
									if (!defaultValue.empty())
									{
										std::uint16_t value = HexToInt<std::uint16_t>(defaultValue);
										this->SetUntypedDefaultValue(boost::any(value));
										break;
									}
									if (!actualValue.empty())
									{
										std::uint16_t value = HexToInt<std::uint16_t>(actualValue);
										this->SetUntypedActualValue(boost::any(value));
										break;
									}
								}
							default:
								break;
						}
					}
					catch (const std::exception& e)
					{
						boost::format formatter(kMsgGeneralFeatureDatatypeError);
						formatter
						% this->GetName();
						LOG_FATAL() << formatter.str() << " " << e.what();
						return Result(ErrorCode::GENERAL_FEATURE_VALUE_INVALID, formatter.str());
					}
					return Result();
				}

				template<class T>
				Result GeneralFeature::GetActualValue(T& value)
				{
					if (this->GetUntypedActualValue().type() == typeid(T))
					{
						//return original stored value
						value = boost::any_cast<T>(this->GetUntypedActualValue());
						return Result();
					}
					//Datatype does not match
					boost::format formatter(kMsgDatatypeMismatch);
					formatter
					% typeid(T).name()
					% this->GetUntypedActualValue().type().name();
					LOG_FATAL() << formatter.str();
					return Result(ErrorCode::DATATYPE_MISMATCH, formatter.str());
				}
				template Result GeneralFeature::GetActualValue(bool& value);
				template Result GeneralFeature::GetActualValue(std::uint16_t& value);
				template Result GeneralFeature::GetActualValue(std::uint32_t& value);

				template<class T>
				Result GeneralFeature::SetActualValue(const T actualValue)
				{
					if (this->GetUntypedActualValue().type() == typeid(T))
					{
						//return original stored value
						this->SetUntypedActualValue(boost::any(actualValue));
						return Result();
					}
					//Datatype does not match
					boost::format formatter(kMsgDatatypeMismatch);
					formatter
					% typeid(T).name()
					% this->GetUntypedActualValue().type().name();
					LOG_FATAL() << formatter.str();
					return Result(ErrorCode::DATATYPE_MISMATCH, formatter.str());
				}
				template Result GeneralFeature::SetActualValue(const bool value);
				template Result GeneralFeature::SetActualValue(const std::uint16_t value);
				template Result GeneralFeature::SetActualValue(const std::uint32_t value);
			}
		}
	}
}
