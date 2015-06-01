/************************************************************************
\file OpenConfiguratorCore.h
\brief Implementation of the Class OpenConfiguratorCore
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
#if !defined OPEN_CONFIGURATOR_CORE_H
#define OPEN_CONFIGURATOR_CORE_H

#include <map>
#include <memory>
#include <utility>
#include <iostream>

#include "BaseNode.h"
#include "Constants.h"
#include "ProjectManager.h"
#include "Network.h"
#include "PlkDataType.h"
#include "AccessType.h"

namespace IndustrialNetwork
{
	namespace POWERLINK
	{
		namespace Core
		{
			namespace API
			{
				class DLLEXPORT OpenConfiguratorCore
				{

					public:
						virtual ~OpenConfiguratorCore();

						static OpenConfiguratorCore& GetInstance();

						// Library related API
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result InitLoggingConfiguration(const std::string configFile);
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result GetSupportedSettingIds(std::vector<std::string>& supportedSettings);

						//Network related API
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result CreateNetwork(const std::string networkId);
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result RemoveNetwork(const std::string networkId);
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result GetNetwork(const std::string networkId, IndustrialNetwork::POWERLINK::Core::NetworkHandling::Network& network);
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result GetNetworkIds(std::vector<std::string>& networkList);
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result ClearNetworks();

						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result SetCycleTime(const std::string networkId, std::uint32_t cycleTime);
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result SetAsyncMtu(const std::string networkId, std::uint32_t asyncMtu);
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result SetMultiplexedCycleLength(const std::string networkId, std::uint32_t multiplexedCycleLength);
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result SetPrescaler(const std::string networkId, std::uint32_t prescaler);

						//Build related API
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result BuildConfiguration(const std::string networkId, std::ostream& configuration);
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result BuildProcessImage(const std::string networkId, std::ostream& configuration);

						//Node related API
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result CreateNode(const std::string networkId, const std::uint8_t nodeID, const std::string nodeName);
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result RemoveNode(const std::string networkId, const std::uint8_t nodeID);

						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result GetControlledNode(const std::string networkId, const std::uint8_t nodeID, IndustrialNetwork::POWERLINK::Core::Node::ControlledNode& node);
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result GetManagingNode(const std::string networkId, IndustrialNetwork::POWERLINK::Core::Node::ManagingNode& node);
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result GetAvailableNodeIds(const std::string networkId, std::vector<std::uint8_t>& nodeIdCollection);

						//Modular Node API
						//IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result CreateModularHeadNode();
						//IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result CreateModule();

						//IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result GetModularHeadNode();
						//IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result GetModule();
						//IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result ReorderModules();
						//IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result DeleteModule();

						//Build Configuration related API
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result SetConfigurationSettingEnabled(const std::string networkId, const std::string  configID, const std::string  settingID, bool enabled);
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result CreateConfigurationSetting(const std::string networkId, const std::string configID, const std::string name, const std::string value);
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result RemoveConfigurationSetting(const std::string networkId, const std::string configID, const std::string name);
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result CreateConfiguration(const std::string networkId, const std::string configID);
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result RemoveConfiguration(const std::string networkId, const std::string configID);
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result ReplaceConfigurationName(const std::string networkId, const std::string oldConfigId, const std::string newConfigId);
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result GetConfigurationSettings(const std::string networkId, const std::string configID, std::vector<IndustrialNetwork::POWERLINK::Core::Configuration::BuildConfigurationSetting>&);
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result GetActiveConfiguration(const std::string networkId, std::string& activeConfiguration);
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result SetActiveConfiguration(const std::string networkId, const std::string configID);
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result GetBuildConfigurations(const std::string networkId, std::vector<IndustrialNetwork::POWERLINK::Core::Configuration::PlkConfiguration>& bcfgs);

						//Object related API
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result CreateObject(const std::string networkId, const std::uint8_t nodeId, std::uint32_t objectId, IndustrialNetwork::POWERLINK::Core::ObjectDictionary::PlkDataType type, IndustrialNetwork::POWERLINK::Core::ObjectDictionary::AccessType accessType, IndustrialNetwork::POWERLINK::Core::ObjectDictionary::ObjectType objectType, IndustrialNetwork::POWERLINK::Core::ObjectDictionary::PDOMapping pdoMapping, std::string defaultValue = "", std::string actualValue = "", std::uint32_t highlimit = 0, std::uint32_t lowLimit = 0, std::string uniqueIdRef = "", std::string name = "");
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result CreateSubObject(const std::string networkId, const std::uint8_t nodeId, std::uint32_t objectId, std::uint32_t subObjectId, IndustrialNetwork::POWERLINK::Core::ObjectDictionary::PlkDataType type, IndustrialNetwork::POWERLINK::Core::ObjectDictionary::AccessType accessType, IndustrialNetwork::POWERLINK::Core::ObjectDictionary::ObjectType objectType, IndustrialNetwork::POWERLINK::Core::ObjectDictionary::PDOMapping pdoMapping, std::string defaultValue = "", std::string actualValue = "", std::uint32_t highlimit = 0, std::uint32_t lowLimit = 0, std::string uniqueIdRef = "", std::string name = "");

						//IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result GetObject();
						//IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result GetSubObject();

						//Mapping API

					private:
						//singleton
						OpenConfiguratorCore();
						OpenConfiguratorCore(OpenConfiguratorCore const&);
						void operator=(OpenConfiguratorCore const&);
				};
			}
		}
	}
}

#endif