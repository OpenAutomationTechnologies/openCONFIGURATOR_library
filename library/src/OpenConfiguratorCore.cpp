/************************************************************************
\file OpenConfiguratorCore.cpp
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
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES{}
LOSS OF USE, DATA, OR PROFITS{} OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
------------------------------------------------------------------------------*/
#include "OpenConfiguratorCore.h"

using namespace std;
using namespace IndustrialNetwork::POWERLINK::Core::API;
using namespace IndustrialNetwork::POWERLINK::Core::ErrorHandling;
using namespace IndustrialNetwork::POWERLINK::Core::Configuration;
using namespace IndustrialNetwork::POWERLINK::Core::NetworkHandling;
using namespace IndustrialNetwork::POWERLINK::Core::Node;
using namespace IndustrialNetwork::POWERLINK::Core::ObjectDictionary;
using namespace IndustrialNetwork::POWERLINK::Core::CoreConfiguration;

OpenConfiguratorCore::OpenConfiguratorCore()
{}

OpenConfiguratorCore::~OpenConfiguratorCore()
{}

OpenConfiguratorCore& OpenConfiguratorCore::GetInstance()
{
	static OpenConfiguratorCore instance;
	return instance;
}

Result OpenConfiguratorCore::InitLoggingConfiguration(const string configFile)
{
	return ProjectManager::GetInstance().InitLoggingConfiguration(configFile);
}

Result OpenConfiguratorCore::GetSupportedSettingIds(vector<string>& supportedSettings)
{
	supportedSettings = ProjectManager::GetInstance().GetSupportedSettingIds();
	return Result();
}

Result OpenConfiguratorCore::CreateNetwork(const string networkId)
{
	shared_ptr<Network> net = make_shared<Network>(networkId);
	return ProjectManager::GetInstance().AddNetwork(networkId, net);
}

Result OpenConfiguratorCore::RemoveNetwork(const string networkId)
{
	return ProjectManager::GetInstance().RemoveNetwork(networkId);
}

Result OpenConfiguratorCore::GetNetworkIds(vector<string>& networkList)
{
	networkList = ProjectManager::GetInstance().GetNetworkIds();
	return Result();
}

Result OpenConfiguratorCore::ClearNetworks()
{
	return ProjectManager::GetInstance().ClearNetworkList();
}

Result OpenConfiguratorCore::GetNetwork(const string networkId, Network& network)
{
	shared_ptr<Network> networkPtr;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, networkPtr);
	if (res.IsSuccessful())
		network = *networkPtr.get();
	return res;

}

Result OpenConfiguratorCore::SetCycleTime(const string networkId, uint32_t cycleTime)
{
	shared_ptr<Network> networkPtr;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, networkPtr);
	if (res.IsSuccessful())
		networkPtr.get()->SetCycleTime(cycleTime);
	return res;
}

Result OpenConfiguratorCore::SetAsyncMtu(const string networkId, uint32_t asyncMtu)
{
	shared_ptr<Network> networkPtr;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, networkPtr);
	if (res.IsSuccessful())
		networkPtr.get()->SetAsyncMTU(asyncMtu);
	return res;

}

Result OpenConfiguratorCore::SetMultiplexedCycleLength(const string networkId, uint32_t multiplexedCycleLength)
{
	shared_ptr<Network> networkPtr;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, networkPtr);
	if (res.IsSuccessful())
		networkPtr.get()->SetMultiplexedCycleLength(multiplexedCycleLength);
	return res;
}

Result OpenConfiguratorCore::SetPrescaler(const string networkId, uint32_t prescaler)
{
	shared_ptr<Network> networkPtr;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, networkPtr);
	if (res.IsSuccessful())
		networkPtr->SetPrescaler(prescaler);
	return res;
}

Result OpenConfiguratorCore::BuildConfiguration(const string networkId, ostream& configuration)
{
	return Result(ErrorCode::UNHANDLED_EXCEPTION);
}

Result OpenConfiguratorCore::BuildProcessImage(const string networkId, ostream& configuration)
{
	return Result(ErrorCode::UNHANDLED_EXCEPTION);
}

Result OpenConfiguratorCore::CreateNode(const string networkId, const uint8_t nodeID, const string nodeName)
{
	shared_ptr<Network> network;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, network);
	if (res.IsSuccessful())
	{
		//Add active managing node to network
		if (nodeID == 240)
		{
			shared_ptr<ManagingNode> node = make_shared<ManagingNode>(true, nodeName);
			res = network.get()->AddNode(node);
		}
		//Add redundant managing node to network
		else if (nodeID >= 241 && nodeID <= 250)
		{
			shared_ptr<ManagingNode> node = make_shared<ManagingNode>(false, nodeName);
			res = network.get()->AddNode(node);
		}
		//Add normal controlled node to network
		else if (nodeID >= 1 && nodeID <= 239)
		{
			shared_ptr<ControlledNode> node = make_shared<ControlledNode>(nodeID, nodeName);
			res = network.get()->AddNode(node);
		}
		else
		{
			//Nodeid invalid
			res = Result(ErrorCode::NODEID_INVALID);
		}
	}

	return res;
}

Result OpenConfiguratorCore::RemoveNode(const string networkId, const uint8_t nodeID)
{
	shared_ptr<Network> network;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, network);
	if (res.IsSuccessful())
	{
		return network.get()->RemoveNode(nodeID);
	}
	return res;
}

Result OpenConfiguratorCore::GetControlledNode(const string networkId, const uint8_t nodeID, ControlledNode& node)
{
	shared_ptr<Network> network;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, network);
	if (res.IsSuccessful())
	{
		shared_ptr<ControlledNode> nodePtr;
		res = network.get()->GetControlledNode(nodeID, nodePtr);
		if (res.IsSuccessful())
		{
			node = *nodePtr.get();
		}
	}
	return res;
}

Result OpenConfiguratorCore::GetManagingNode(const string networkId, ManagingNode& node)
{
	shared_ptr<Network> network;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, network);
	if (res.IsSuccessful())
	{
		shared_ptr<ManagingNode> nodePtr;
		res = network.get()->GetManagingNode(nodePtr);
		if (res.IsSuccessful())
		{
			node = *nodePtr.get();
		}
	}
	return res;
}

Result OpenConfiguratorCore::GetAvailableNodeIds(const string networkId, vector<uint8_t>& nodeIdCollection)
{
	shared_ptr<Network> network;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, network);
	if (res.IsSuccessful())
	{
		return network.get()->GetAvailableNodeIds(nodeIdCollection);
	}
	return res;
}

Result OpenConfiguratorCore::CreateConfiguration(const string networkId, const string configID)
{
	shared_ptr<Network> network;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, network);
	if (res.IsSuccessful())
	{
		return network.get()->AddConfiguration(configID);
	}
	return res;
}

Result OpenConfiguratorCore::RemoveConfiguration(const string networkId, const string configID)
{
	shared_ptr<Network> network;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, network);
	if (res.IsSuccessful())
	{
		return network.get()->RemoveConfiguration(configID);
	}
	return res;
}

Result OpenConfiguratorCore::ReplaceConfigurationName(const string networkId, const string oldConfigId, const string newConfigId)
{
	shared_ptr<Network> network;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, network);
	if (res.IsSuccessful())
	{
		return network.get()->ReplaceConfigurationName(oldConfigId, newConfigId);
	}
	return res;
}

Result OpenConfiguratorCore::CreateConfigurationSetting(const string networkId, const string configID,
        const string name, const string value)
{
	shared_ptr<Network> network;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, network);
	if (res.IsSuccessful())
	{
		return network.get()->AddConfigurationSetting(configID, make_shared<BuildConfigurationSetting>(name, value));
	}
	return res;
}

Result OpenConfiguratorCore::RemoveConfigurationSetting(const string networkId, const string configID, const string name)
{
	shared_ptr<Network> network;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, network);
	if (res.IsSuccessful())
	{
		return network.get()->RemoveConfigurationSetting(configID, name);
	}
	return res;
}

Result OpenConfiguratorCore::SetConfigurationSettingEnabled(const string networkId, const string configID, const string settingID, bool enabled)
{
	shared_ptr<Network> network;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, network);
	if (res.IsSuccessful())
	{
		return network.get()->SetConfigurationSettingEnabled(configID, settingID, enabled);
	}
	return res;
}

Result OpenConfiguratorCore::GetConfigurationSettings(const string networkId, const string configID, vector<BuildConfigurationSetting>& settings)
{
	shared_ptr<Network> network;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, network);
	if (res.IsSuccessful())
	{
		vector<shared_ptr<BuildConfigurationSetting>> configurationSettings;
		res = network.get()->GetConfigurationSettings(configID, configurationSettings);
		if (res.IsSuccessful())
		{
			for (auto& config : configurationSettings)
			{
				settings.push_back(*config.get());
			}
		}
	}
	return res;
}

Result OpenConfiguratorCore::GetActiveConfiguration(const string networkId, string& activeConfiguration)
{
	shared_ptr<Network> network;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, network);
	if (res.IsSuccessful())
	{
		activeConfiguration = network.get()->GetActiveConfiguration();
	}
	return res;
}

Result OpenConfiguratorCore::SetActiveConfiguration(const string networkId, const string configID)
{
	shared_ptr<Network> network;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, network);
	if (res.IsSuccessful())
	{
		network.get()->SetActiveConfiguration(configID);
	}
	return res;
}

Result OpenConfiguratorCore::GetBuildConfigurations(const string networkId, vector<PlkConfiguration>& buildConfigurations)
{
	shared_ptr<Network> network;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, network);
	if (res.IsSuccessful())
	{
		vector<shared_ptr<PlkConfiguration>> configurations;
		res = network.get()->GetBuildConfigurations(configurations);
		if (res.IsSuccessful())
		{
			for (auto& config : configurations)
			{
				buildConfigurations.push_back(*config.get());
			}
		}
	}
	return res;
}

Result OpenConfiguratorCore::CreateObject(const string networkId, const uint8_t nodeId, uint32_t objectId, PlkDataType type, AccessType accessType, ObjectType objectType, PDOMapping pdoMapping, string defaultValue, string actualValue, uint32_t highlimit, uint32_t lowLimit, string uniqueIdRef, string name)
{
	shared_ptr<Network> network;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, network);
	if (res.IsSuccessful())
	{
		shared_ptr<ControlledNode> node;
		res = network.get()->GetControlledNode(nodeId, node);
		if (res.IsSuccessful())
		{
			shared_ptr<Object> ptr(new Object(objectId, type, accessType, objectType, pdoMapping, nodeId, defaultValue, actualValue, highlimit, lowLimit, uniqueIdRef, name));
			res = node.get()->AddObject(ptr);
		}
	}
	return res;
}

Result OpenConfiguratorCore::CreateDomainObject(const string networkId, const uint8_t nodeId, uint32_t objectId, PlkDataType type, AccessType accessType, ObjectType objectType, PDOMapping pdoMapping, const string uniqueIdRef, const string name)
{
	shared_ptr<Network> network;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, network);
	if (res.IsSuccessful())
	{
		shared_ptr<ControlledNode> node;
		res = network.get()->GetControlledNode(nodeId, node);
		if (res.IsSuccessful())
		{
			shared_ptr<Object> ptr(new Object(objectId, type, accessType, objectType, pdoMapping, nodeId, uniqueIdRef, name));
			res = node.get()->AddObject(ptr);
		}
	}
	return res;
}

Result OpenConfiguratorCore::CreateSubObject(const string networkId, const uint8_t nodeId, uint32_t objectId, uint32_t subObjectId, PlkDataType type, AccessType accessType, ObjectType objectType, PDOMapping pdoMapping, string defaultValue, string actualValue, uint32_t highlimit, uint32_t lowLimit, string uniqueIdRef, string name)
{
	shared_ptr<Network> network;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, network);
	if (res.IsSuccessful())
	{
		shared_ptr<ControlledNode> node;
		res = network.get()->GetControlledNode(nodeId, node);
		if (res.IsSuccessful())
		{
			shared_ptr<SubObject> ptr(new SubObject(subObjectId, type, accessType, objectType, pdoMapping, nodeId, defaultValue, actualValue, highlimit, lowLimit, uniqueIdRef, name));
			res = node.get()->AddSubObject(objectId, ptr);
		}
	}
	return res;
}

Result OpenConfiguratorCore::CreateDomainSubObject(const string networkId, const uint8_t nodeId, uint32_t objectId, uint32_t subObjectId, PlkDataType type, AccessType accessType, ObjectType objectType, PDOMapping pdoMapping, const string uniqueIdRef, const string name)
{
	shared_ptr<Network> network;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, network);
	if (res.IsSuccessful())
	{
		shared_ptr<ControlledNode> node;
		res = network.get()->GetControlledNode(nodeId, node);
		if (res.IsSuccessful())
		{
			shared_ptr<SubObject> ptr(new SubObject(subObjectId, type, accessType, objectType, pdoMapping, nodeId, uniqueIdRef, name));
			res = node.get()->AddSubObject(objectId, ptr);
		}
	}
	return res;
}

Result OpenConfiguratorCore::GetFeatureDefaultValue(CNFeatureEnum feature, string& value)
{
	value = PlkFeatureDefaultValues[feature];
	return Result();
}
Result OpenConfiguratorCore::GetFeatureDefaultValue(MNFeatureEnum feature, string& value)
{
	value = PlkFeatureDefaultValues[feature];
	return Result();
}
Result OpenConfiguratorCore::GetFeatureDefaultValue(GeneralFeatureEnum feature, string& value)
{
	value = PlkFeatureDefaultValues[feature];
	return Result();
}

Result OpenConfiguratorCore::SetFeatureValue(const string networkId, const uint8_t nodeId, CNFeatureEnum feature, const string value)
{
	shared_ptr<Network> network;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, network);
	if (res.IsSuccessful())
	{
		shared_ptr<ControlledNode> node;
		res = network.get()->GetControlledNode(nodeId, node);
		if (res.IsSuccessful())
		{
			node.get()->GetNetworkManagement().get()->SetFeatureUntypedActualValue(feature, value);
		}

	}
	return res;
}

Result OpenConfiguratorCore::SetFeatureValue(const string networkId, const uint8_t nodeId, MNFeatureEnum feature, const string value)
{
	shared_ptr<Network> network;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, network);
	if (res.IsSuccessful())
	{
		shared_ptr<ControlledNode> node;
		res = network.get()->GetControlledNode(nodeId, node);
		if (res.IsSuccessful())
		{
			node.get()->GetNetworkManagement().get()->SetFeatureUntypedActualValue(feature, value);
		}
	}
	return res;
}

Result OpenConfiguratorCore::SetFeatureValue(const string networkId, const uint8_t nodeId, GeneralFeatureEnum feature, const string value)
{
	shared_ptr<Network> network;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, network);
	if (res.IsSuccessful())
	{
		shared_ptr<ControlledNode> node;
		res = network.get()->GetControlledNode(nodeId, node);
		if (res.IsSuccessful())
		{
			node.get()->GetNetworkManagement().get()->SetFeatureUntypedActualValue(feature, value);
		}

	}
	return res;
}
