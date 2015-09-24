/************************************************************************
\file PlkConfiguration.h
\brief Implementation of the Class PlkConfiguration
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
#if !defined PLK_CONFIGURATION_H
#define PLK_CONFIGURATION_H

#include <memory>
#include <chrono>
#include <algorithm>

#include "IBuildConfiguration.h"
#include "Result.h"
#include "ErrorCode.h"
#include "Constants.h"
#include "BuildConfigurationSetting.h"
#include "BaseNode.h"
#include "SubObject.h"
#include "ManagingNode.h"
#include "ControlledNode.h"
#include "LoggingConfiguration.h"
#include "Utilities.h"

namespace IndustrialNetwork
{
	namespace POWERLINK
	{
		namespace Core
		{
			namespace Configuration
			{
				/**
				\brief Calculates the POWERLINK network configuration.
				\author rueckerc, Bernecker+Rainer Industrie Elektronik Ges.m.b.H.
				*/
				class DLLEXPORT PlkConfiguration : public IndustrialNetwork::Fieldbus::IBuildConfiguration<IndustrialNetwork::POWERLINK::Core::Configuration::BuildConfigurationSetting>
				{

					public:
						PlkConfiguration();
						explicit PlkConfiguration(const std::string& name);
						~PlkConfiguration();
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result GenerateConfiguration(const std::map<std::uint8_t, std::shared_ptr<IndustrialNetwork::POWERLINK::Core::Node::BaseNode>>& nodeCollection);

						const std::string& GetConfigurationName() const;
						void SetConfigurationName(const std::string& config);
					private:
						std::string configurationName;

						//Distribute MN OD -> CN OD
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result DistributeCycleTime(const std::map<std::uint8_t, std::shared_ptr<IndustrialNetwork::POWERLINK::Core::Node::BaseNode>>& nodeCollection);
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result DistributeDateTimeStamps(const std::map<std::uint8_t, std::shared_ptr<IndustrialNetwork::POWERLINK::Core::Node::BaseNode>>& nodeCollection);
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result DistributeNodeAssignment(const std::map<std::uint8_t, std::shared_ptr<IndustrialNetwork::POWERLINK::Core::Node::BaseNode>>& nodeCollection);
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result DistributeMultiplCycleCount(const std::map<std::uint8_t, std::shared_ptr<IndustrialNetwork::POWERLINK::Core::Node::BaseNode>>& nodeCollection);
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result DistributeAsyncMtu(const std::map<std::uint8_t, std::shared_ptr<IndustrialNetwork::POWERLINK::Core::Node::BaseNode>>& nodeCollection);
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result DistributePrescaler(const std::map<std::uint8_t, std::shared_ptr<IndustrialNetwork::POWERLINK::Core::Node::BaseNode>>& nodeCollection);
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result DistributeMultiplCycleAssign(const std::map<std::uint8_t, std::shared_ptr<IndustrialNetwork::POWERLINK::Core::Node::BaseNode>>& nodeCollection);
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result DistributeSDOSeqLayerTimeout(const std::map<std::uint8_t, std::shared_ptr<IndustrialNetwork::POWERLINK::Core::Node::BaseNode>>& nodeCollection);
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result DistributeSDOCmdLayerTimeout(const std::map<std::uint8_t, std::shared_ptr<IndustrialNetwork::POWERLINK::Core::Node::BaseNode>>& nodeCollection);
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result DistributeIpDefaultGateway(const std::map<std::uint8_t, std::shared_ptr<IndustrialNetwork::POWERLINK::Core::Node::BaseNode>>& nodeCollection);

						//Distribute CN OD -> MN OD
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result DistributePResTimeOut(const std::map<std::uint8_t, std::shared_ptr<IndustrialNetwork::POWERLINK::Core::Node::BaseNode>>& nodeCollection);
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result DistributePReqPayloadLimit(const std::map<std::uint8_t, std::shared_ptr<IndustrialNetwork::POWERLINK::Core::Node::BaseNode>>& nodeCollection);
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result DistributePResPayloadLimit(const std::map<std::uint8_t, std::shared_ptr<IndustrialNetwork::POWERLINK::Core::Node::BaseNode>>& nodeCollection);

						//Write for all CNs
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result DistributeCNLossObjects(const std::map<std::uint8_t, std::shared_ptr<IndustrialNetwork::POWERLINK::Core::Node::BaseNode>>& nodeCollection);

						//Sync RMNs
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result SyncRedundantManagingNodes(const std::map<std::uint8_t, std::shared_ptr<IndustrialNetwork::POWERLINK::Core::Node::BaseNode>>& nodeCollection);

						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result ClearActualValues(const std::map<std::uint8_t, std::shared_ptr<IndustrialNetwork::POWERLINK::Core::Node::BaseNode>>& nodeCollection, std::shared_ptr<IndustrialNetwork::POWERLINK::Core::Node::BaseNode> node, std::uint32_t objectId);
				};
			}
		}
	}
}
#endif
