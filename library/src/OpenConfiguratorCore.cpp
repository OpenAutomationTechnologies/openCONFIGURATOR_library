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

using namespace IndustrialNetwork::POWERLINK::Core::API;
using namespace IndustrialNetwork::POWERLINK::Core::ErrorHandling;
using namespace IndustrialNetwork::POWERLINK::Core::Configuration;
using namespace IndustrialNetwork::POWERLINK::Core::NetworkHandling;
using namespace IndustrialNetwork::POWERLINK::Core::Node;
using namespace IndustrialNetwork::POWERLINK::Core::ObjectDictionary;
using namespace IndustrialNetwork::POWERLINK::Core::CoreConfiguration;
using namespace IndustrialNetwork::POWERLINK::Core::Utilities;

OpenConfiguratorCore::OpenConfiguratorCore()
{}

OpenConfiguratorCore::~OpenConfiguratorCore()
{}

OpenConfiguratorCore& OpenConfiguratorCore::GetInstance()
{
	static OpenConfiguratorCore instance;
	return instance;
}

Result OpenConfiguratorCore::InitLoggingConfiguration(const std::string& configuration)
{
	return ProjectManager::GetInstance().InitLoggingConfiguration(configuration);
}

Result OpenConfiguratorCore::InitEclipseLoggingPath(const std::string& loggingPath)
{
	return ProjectManager::GetInstance().InitEclipseLoggingConfiguration(loggingPath);
}

Result OpenConfiguratorCore::GetSupportedSettingIds(std::vector<std::string>& supportedSettings)
{
	supportedSettings = ProjectManager::GetInstance().GetSupportedSettingIds();
	return Result();
}

Result OpenConfiguratorCore::CreateNetwork(const std::string& networkId)
{
	std::shared_ptr<Network> net = std::make_shared<Network>(networkId);
	return ProjectManager::GetInstance().AddNetwork(networkId, net);
}

Result OpenConfiguratorCore::RemoveNetwork(const std::string& networkId)
{
	return ProjectManager::GetInstance().RemoveNetwork(networkId);
}

Result OpenConfiguratorCore::GetNetworkIds(std::vector<std::string>& networkList)
{
	networkList = ProjectManager::GetInstance().GetNetworkIds();
	return Result();
}

Result OpenConfiguratorCore::ClearNetworks()
{
	return ProjectManager::GetInstance().ClearNetworkList();
}

Result OpenConfiguratorCore::GetNetwork(const std::string& networkId, Network& network)
{
	std::shared_ptr<Network> networkPtr;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, networkPtr);
	if (res.IsSuccessful())
		network = *networkPtr.get();
	return res;

}

Result OpenConfiguratorCore::SetCycleTime(const std::string& networkId, std::uint32_t cycleTime)
{
	std::shared_ptr<Network> networkPtr;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, networkPtr);
	if (!res.IsSuccessful())
		return res;

	networkPtr->SetCycleTime(cycleTime);

	std::shared_ptr<ManagingNode> nodePtr;
	res = networkPtr->GetManagingNode(nodePtr);
	if (!res.IsSuccessful())
		return res;

	return nodePtr->SetObjectActualValue(0x1006, IntToHex(cycleTime, 8, "0x"));
}

Result OpenConfiguratorCore::SetAsyncMtu(const std::string& networkId, std::uint16_t asyncMtu)
{
	std::shared_ptr<Network> networkPtr;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, networkPtr);
	if (!res.IsSuccessful())
		return res;

	networkPtr->SetAsyncMTU(asyncMtu);

	std::shared_ptr<ManagingNode> nodePtr;
	res = networkPtr->GetManagingNode(nodePtr);
	if (!res.IsSuccessful())
		return res;

	return nodePtr->SetSubObjectActualValue(0x1F98, 0x8, IntToHex(asyncMtu, 4, "0x"));
}

Result OpenConfiguratorCore::SetMultiplexedCycleCount(const std::string& networkId, std::uint16_t multiplexedCycleCount)
{
	std::shared_ptr<Network> networkPtr;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, networkPtr);
	if (!res.IsSuccessful())
		return res;

	networkPtr->SetMultiplexedCycleCount(multiplexedCycleCount);

	std::shared_ptr<ManagingNode> nodePtr;
	res = networkPtr->GetManagingNode(nodePtr);
	if (!res.IsSuccessful())
		return res;

	return nodePtr->SetSubObjectActualValue(0x1F98, 0x7, IntToHex(multiplexedCycleCount, 2, "0x"));
}

Result OpenConfiguratorCore::SetPrescaler(const std::string& networkId, std::uint16_t prescaler)
{
	std::shared_ptr<Network> networkPtr;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, networkPtr);
	if (!res.IsSuccessful())
		return res;

	networkPtr->SetPrescaler(prescaler);

	std::shared_ptr<ManagingNode> nodePtr;
	res = networkPtr->GetManagingNode(nodePtr);
	if (!res.IsSuccessful())
		return res;

	return nodePtr->SetSubObjectActualValue(0x1F98, 0x9, IntToHex(prescaler, 2, "0x"));
}

Result OpenConfiguratorCore::BuildConfiguration(const std::string& networkId, std::string& configurationOutput, std::vector<std::uint8_t>& binOutput)
{
	std::shared_ptr<Network> networkPtr;
	std::stringstream config;
	std::stringstream hexOutput;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, networkPtr);
	if (!res.IsSuccessful())
		return res;

	if (!networkPtr->HasControlledNodes())
	{
		return Result(ErrorCode::NO_CONTROLLED_NODES_CONFIGURED, kMsgNoNodesConfigured);
	}

	res = networkPtr->GenerateConfiguration();
	if (res.IsSuccessful())
		res = ConfigurationGenerator::GetInstance().GenerateNetworkConfiguration(networkPtr, config, hexOutput);

	configurationOutput = config.str();
	ConfigurationToAscii(hexOutput, binOutput);
	return res;
}

Result OpenConfiguratorCore::BuildXMLProcessImage(const std::string& networkId, const std::uint8_t nodeid, std::string& processImageOutput)
{

	std::shared_ptr<Network> networkPtr;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, networkPtr);
	if (!res.IsSuccessful())
		return res;

	if (!networkPtr->HasControlledNodes())
	{
		return Result(ErrorCode::NO_CONTROLLED_NODES_CONFIGURED, kMsgNoNodesConfigured);
	}

	res = networkPtr->GenerateConfiguration();
	if (res.IsSuccessful())
		processImageOutput = XmlProcessImageGenerator::GetInstance().Generate(nodeid, networkPtr);

	return res;
}

Result OpenConfiguratorCore::BuildNETProcessImage(const std::string& networkId, const std::uint8_t nodeid, std::string& processImageOutput)
{
	std::shared_ptr<Network> networkPtr;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, networkPtr);
	if (!res.IsSuccessful())
		return res;

	if (!networkPtr->HasControlledNodes())
	{
		return Result(ErrorCode::NO_CONTROLLED_NODES_CONFIGURED, kMsgNoNodesConfigured);
	}

	res = networkPtr->GenerateConfiguration();
	if (res.IsSuccessful())
		processImageOutput = NetProcessImageGenerator::GetInstance().Generate(nodeid, networkPtr);

	return res;
}

Result OpenConfiguratorCore::BuildCProcessImage(const std::string& networkId, const std::uint8_t nodeid, std::string& processImageOutput)
{
	std::shared_ptr<Network> networkPtr;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, networkPtr);
	if (!res.IsSuccessful())
		return res;

	if (!networkPtr->HasControlledNodes())
	{
		return Result(ErrorCode::NO_CONTROLLED_NODES_CONFIGURED, kMsgNoNodesConfigured);
	}

	res = networkPtr->GenerateConfiguration();
	if (res.IsSuccessful())
		processImageOutput = CProcessImageGenerator::GetInstance().Generate(nodeid, networkPtr);

	return res;
}

Result OpenConfiguratorCore::CreateNode(const std::string& networkId, const std::uint8_t nodeID, const std::string& nodeName, const bool isRmn)
{
	std::shared_ptr<Network> network;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, network);
	if (res.IsSuccessful())
	{
		//Add active managing node to network
		if (nodeID == 240)
		{
			std::shared_ptr<ManagingNode> node = std::make_shared<ManagingNode>(nodeID, nodeName);
			res = network->AddNode(node);
		}
		//Add redundant managing node to network
		else if ((nodeID >= 241 && nodeID <= 250) || isRmn) //allow the RMN to have normal nodeIDs
		{
			std::shared_ptr<ManagingNode> node = std::make_shared<ManagingNode>(nodeID, nodeName);
			res = network->AddNode(node);
		}
		//Add normal controlled node to network
		else if (nodeID >= 1 && nodeID <= 239)
		{
			std::shared_ptr<ControlledNode> node = std::make_shared<ControlledNode>(nodeID, nodeName);
			res = network->AddNode(node);
		}
		else
		{
			//Nodeid invalid
			res = Result(ErrorCode::NODEID_INVALID);
		}
	}

	return res;
}

Result OpenConfiguratorCore::RemoveNode(const std::string& networkId, const std::uint8_t nodeID)
{
	std::shared_ptr<Network> network;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, network);
	if (res.IsSuccessful())
	{
		return network->RemoveNode(nodeID);
	}
	return res;
}

Result OpenConfiguratorCore::GetControlledNode(const std::string& networkId, const std::uint8_t nodeID, ControlledNode& node)
{
	std::shared_ptr<Network> network;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, network);
	if (res.IsSuccessful())
	{
		std::shared_ptr<BaseNode> nodePtr;
		res = network->GetBaseNode(nodeID, nodePtr);
		if (res.IsSuccessful())
		{
			std::shared_ptr<ControlledNode> cn = std::dynamic_pointer_cast<ControlledNode>(nodePtr);
			if (cn)
				node = *cn.get();
			else
				return Result(ErrorCode::NODE_IS_NOT_CONTROLLED_NODE);
		}
	}
	return res;
}

Result OpenConfiguratorCore::GetManagingNode(const std::string& networkId, ManagingNode& node)
{
	std::shared_ptr<Network> network;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, network);
	if (res.IsSuccessful())
	{
		std::shared_ptr<ManagingNode> nodePtr;
		res = network->GetManagingNode(nodePtr);
		if (res.IsSuccessful())
		{
			node = *nodePtr.get();
		}
	}
	return res;
}

Result OpenConfiguratorCore::GetAvailableNodeIds(const std::string& networkId, std::vector<std::uint8_t>& nodeIdCollection)
{
	std::shared_ptr<Network> network;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, network);
	if (res.IsSuccessful())
	{
		return network->GetAvailableNodeIds(nodeIdCollection);
	}
	return res;
}

Result OpenConfiguratorCore::CreateConfiguration(const std::string& networkId, const std::string& configID)
{
	std::shared_ptr<Network> network;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, network);
	if (res.IsSuccessful())
	{
		return network->AddConfiguration(configID);
	}
	return res;
}

Result OpenConfiguratorCore::RemoveConfiguration(const std::string& networkId, const std::string& configID)
{
	std::shared_ptr<Network> network;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, network);
	if (res.IsSuccessful())
	{
		return network->RemoveConfiguration(configID);
	}
	return res;
}

Result OpenConfiguratorCore::ReplaceConfigurationName(const std::string& networkId, const std::string& oldConfigId, const std::string& newConfigId)
{
	std::shared_ptr<Network> network;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, network);
	if (res.IsSuccessful())
	{
		return network->ReplaceConfigurationName(oldConfigId, newConfigId);
	}
	return res;
}

Result OpenConfiguratorCore::CreateConfigurationSetting(const std::string& networkId, const std::string& configID, const std::string& name, const std::string& value)
{
	std::shared_ptr<Network> network;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, network);
	if (res.IsSuccessful())
	{
		return network->AddConfigurationSetting(configID, std::make_shared<BuildConfigurationSetting>(name, value));
	}
	return res;
}

Result OpenConfiguratorCore::RemoveConfigurationSetting(const std::string& networkId, const std::string& configID, const std::string& name)
{
	std::shared_ptr<Network> network;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, network);
	if (res.IsSuccessful())
	{
		return network->RemoveConfigurationSetting(configID, name);
	}
	return res;
}

Result OpenConfiguratorCore::SetConfigurationSettingEnabled(const std::string& networkId, const std::string& configID, const std::string& settingID, bool enabled)
{
	std::shared_ptr<Network> network;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, network);
	if (res.IsSuccessful())
	{
		return network->SetConfigurationSettingEnabled(configID, settingID, enabled);
	}
	return res;
}

Result OpenConfiguratorCore::GetConfigurationSettings(const std::string& networkId, const std::string& configID, std::vector<BuildConfigurationSetting>& settings)
{
	std::shared_ptr<Network> network;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, network);
	if (res.IsSuccessful())
	{
		std::vector<std::shared_ptr<BuildConfigurationSetting>> configurationSettings;
		res = network->GetConfigurationSettings(configID, configurationSettings);
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

Result OpenConfiguratorCore::GetActiveConfiguration(const std::string& networkId, std::string& activeConfiguration)
{
	std::shared_ptr<Network> network;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, network);
	if (res.IsSuccessful())
	{
		activeConfiguration = network->GetActiveConfiguration();
	}
	return res;
}

Result OpenConfiguratorCore::SetActiveConfiguration(const std::string& networkId, const std::string& configID)
{
	std::shared_ptr<Network> network;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, network);
	if (res.IsSuccessful())
	{
		network->SetActiveConfiguration(configID);
	}
	return res;
}

Result OpenConfiguratorCore::GetBuildConfigurations(const std::string& networkId, std::vector<PlkConfiguration>& buildConfigurations)
{
	std::shared_ptr<Network> network;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, network);
	if (!res.IsSuccessful())
		return res;

	std::vector<std::shared_ptr<PlkConfiguration>> configurations;
	res = network->GetBuildConfigurations(configurations);
	if (!res.IsSuccessful())
		return res;

	for (auto& config : configurations)
	{
		buildConfigurations.push_back(*config.get());
	}
	return res;
}

Result OpenConfiguratorCore::CreateObject(const std::string& networkId, const std::uint8_t nodeId, std::uint32_t objectId, ObjectType objectType, const std::string& name, PlkDataType dataType, AccessType accessType, PDOMapping pdoMapping, const std::string& defaultValueToSet, const std::string& actualValueToSet)
{
	std::shared_ptr<Network> network;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, network);
	if (!res.IsSuccessful())
		return res;

	std::shared_ptr<BaseNode> node;
	res = network->GetBaseNode(nodeId, node);
	if (!res.IsSuccessful())
		return res;

	std::shared_ptr<Object> ptr(new Object(objectId, objectType, name, nodeId));

	if (dataType != PlkDataType::UNDEFINED)
		ptr->SetDataType(dataType);

	if (accessType != AccessType::UNDEFINED)
		ptr->SetAccessType(accessType);

	if (pdoMapping != PDOMapping::UNDEFINED)
		ptr->SetPDOMapping(pdoMapping);

	if (!defaultValueToSet.empty())
	{
		res = ptr->SetTypedObjectDefaultValue(defaultValueToSet);
		if (!res.IsSuccessful())
			return res;
	}

	if (!actualValueToSet.empty())
	{
		res = ptr->SetTypedObjectActualValue(actualValueToSet);
		if (!res.IsSuccessful())
			return res;
	}
	return node->AddObject(ptr);
}

Result OpenConfiguratorCore::SetObjectLimits(const std::string& networkId, const std::uint8_t nodeId, std::uint32_t objectId, const std::string& lowLimit, const std::string& highLimit)
{
	std::shared_ptr<Network> network;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, network);
	if (!res.IsSuccessful())
		return res;

	std::shared_ptr<BaseNode> node;
	res = network->GetBaseNode(nodeId, node);
	if (!res.IsSuccessful())
		return res;

	std::shared_ptr<Object> obj;
	res = node->GetObject(objectId, obj);
	if (!res.IsSuccessful())
		return res;

	res = obj->SetLowLimit(lowLimit);
	if (!res.IsSuccessful())
		return res;

	return obj->SetHighLimit(highLimit);
}

Result OpenConfiguratorCore::CreateDomainObject(const std::string& networkId, const std::uint8_t nodeId, std::uint32_t objectId, ObjectType objectType, const std::string& name, PDOMapping pdoMapping, const std::string& uniqueIdRef)
{
	std::shared_ptr<Network> network;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, network);
	if (!res.IsSuccessful())
		return res;

	std::shared_ptr<BaseNode> node;
	res = network->GetBaseNode(nodeId, node);
	if (!res.IsSuccessful())
		return res;

	std::shared_ptr<Parameter> param;
	res = node->GetApplicationProcess()->GetParameter(uniqueIdRef, param);
	if (!res.IsSuccessful())
		return res;

	std::shared_ptr<Object> ptr(new Object(objectId, objectType, name, nodeId, uniqueIdRef));

	if (pdoMapping != PDOMapping::UNDEFINED)
		ptr->SetPDOMapping(pdoMapping);

	ptr->SetComplexDataType(param);
	return node->AddObject(ptr);
}

Result OpenConfiguratorCore::CreateSubObject(const std::string& networkId, const std::uint8_t nodeId, std::uint32_t objectId, std::uint8_t subObjectId, ObjectType objectType, const std::string& name, PlkDataType dataType, AccessType accessType, PDOMapping pdoMapping, const std::string& defaultValueToSet, const std::string& actualValueToSet)
{
	std::shared_ptr<Network> network;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, network);
	if (!res.IsSuccessful())
		return res;

	std::shared_ptr<BaseNode> node;
	res = network->GetBaseNode(nodeId, node);
	if (!res.IsSuccessful())
		return res;

	std::shared_ptr<SubObject> ptr(new SubObject(subObjectId, objectType, name, nodeId));

	if (dataType != PlkDataType::UNDEFINED)
		ptr->SetDataType(dataType);

	if (accessType != AccessType::UNDEFINED)
		ptr->SetAccessType(accessType);

	if (pdoMapping != PDOMapping::UNDEFINED)
		ptr->SetPDOMapping(pdoMapping);

	if (!defaultValueToSet.empty())
	{
		res = ptr->SetTypedObjectDefaultValue(defaultValueToSet);
		if (!res.IsSuccessful())
			return res;
	}

	if (!actualValueToSet.empty())
	{
		res = ptr->SetTypedObjectActualValue(actualValueToSet);
		if (!res.IsSuccessful())
			return res;
	}

	return node->AddSubObject(objectId, ptr);
}

Result OpenConfiguratorCore::SetSubObjectLimits(const std::string& networkId, const std::uint8_t nodeId, std::uint32_t objectId, std::uint8_t subObjectId, const std::string& lowLimit, const std::string& highLimit)
{
	std::shared_ptr<Network> network;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, network);
	if (!res.IsSuccessful())
		return res;

	std::shared_ptr<BaseNode> node;
	res = network->GetBaseNode(nodeId, node);
	if (!res.IsSuccessful())
		return res;

	std::shared_ptr<SubObject> subObj;
	res = node->GetSubObject(objectId, subObjectId, subObj);
	if (!res.IsSuccessful())
		return res;

	res = subObj->SetLowLimit(lowLimit);
	if (!res.IsSuccessful())
		return res;

	return subObj->SetHighLimit(highLimit);
}

Result OpenConfiguratorCore::CreateDomainSubObject(const std::string& networkId, const std::uint8_t nodeId, std::uint32_t objectId, std::uint8_t subObjectId, ObjectType objectType, const std::string& name, PDOMapping pdoMapping, const std::string& uniqueIdRef)
{
	std::shared_ptr<Network> network;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, network);
	if (!res.IsSuccessful())
		return res;

	std::shared_ptr<BaseNode> node;
	res = network->GetBaseNode(nodeId, node);
	if (!res.IsSuccessful())
		return res;

	std::shared_ptr<Parameter> param;
	res = node->GetApplicationProcess()->GetParameter(uniqueIdRef, param);
	if (!res.IsSuccessful())
		return res;

	std::shared_ptr<SubObject> ptr(new SubObject(subObjectId, objectType, name, nodeId, uniqueIdRef));
	if (pdoMapping != PDOMapping::UNDEFINED)
		ptr->SetPDOMapping(pdoMapping);

	ptr->SetComplexDataType(param);
	return node->AddSubObject(objectId, ptr);

}

Result OpenConfiguratorCore::GetObjectSize(const std::string& networkId, const std::uint8_t nodeId, std::uint32_t objectId, std::uint32_t& size)
{
	std::shared_ptr<Network> network;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, network);
	if (!res.IsSuccessful())
		return res;

	std::shared_ptr<BaseNode> node;
	res = network->GetBaseNode(nodeId, node);
	if (!res.IsSuccessful())
		return res;

	std::shared_ptr<Object> object;
	res = node->GetObject(objectId, object);
	if (!res.IsSuccessful())
		return res;

	size = object->GetBitSize() / 8;
	return res;
}

Result OpenConfiguratorCore::GetSubObjectSize(const std::string& networkId, const std::uint8_t nodeId, std::uint32_t objectId, std::uint8_t subObjectId, uint32_t& size)
{
	std::shared_ptr<Network> network;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, network);
	if (!res.IsSuccessful())
		return res;

	std::shared_ptr<BaseNode> node;
	res = network->GetBaseNode(nodeId, node);
	if (!res.IsSuccessful())
		return res;

	std::shared_ptr<SubObject> subObject;
	res = node->GetSubObject(objectId, subObjectId, subObject);
	if (!res.IsSuccessful())
		return res;

	size = subObject->GetBitSize() / 8;
	return res;
}

Result OpenConfiguratorCore::GetFeatureDefaultValue(CNFeatureEnum feature, std::string& value)
{
	value = PlkFeatureDefaultValues[feature];
	return Result();
}
Result OpenConfiguratorCore::GetFeatureDefaultValue(MNFeatureEnum feature, std::string& value)
{
	value = PlkFeatureDefaultValues[feature];
	return Result();
}
Result OpenConfiguratorCore::GetFeatureDefaultValue(GeneralFeatureEnum feature, std::string& value)
{
	value = PlkFeatureDefaultValues[feature];
	return Result();
}

Result OpenConfiguratorCore::SetFeatureValue(const std::string& networkId, const std::uint8_t nodeId, CNFeatureEnum feature, const std::string& value)
{
	std::shared_ptr<Network> network;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, network);
	if (!res.IsSuccessful())
		return res;

	std::shared_ptr<BaseNode> node;
	res = network->GetBaseNode(nodeId, node);
	if (!res.IsSuccessful())
		return res;

	return node->GetNetworkManagement()->SetFeatureUntypedActualValue(feature, value);
}

Result OpenConfiguratorCore::SetFeatureValue(const std::string& networkId, const std::uint8_t nodeId, MNFeatureEnum feature, const std::string& value)
{
	std::shared_ptr<Network> network;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, network);
	if (!res.IsSuccessful())
		return res;

	std::shared_ptr<BaseNode> node;
	res = network->GetBaseNode(nodeId, node);
	if (!res.IsSuccessful())
		return res;

	return node->GetNetworkManagement()->SetFeatureUntypedActualValue(feature, value);
}

Result OpenConfiguratorCore::SetFeatureValue(const std::string& networkId, const std::uint8_t nodeId, GeneralFeatureEnum feature, const std::string& value)
{
	std::shared_ptr<Network> network;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, network);
	if (!res.IsSuccessful())
		return res;

	std::shared_ptr<BaseNode> node;
	res = network->GetBaseNode(nodeId, node);
	if (!res.IsSuccessful())
		return res;

	return node->GetNetworkManagement()->SetFeatureUntypedActualValue(feature, value);
}

Result OpenConfiguratorCore::CreateParameter(const std::string& networkId, const std::uint8_t nodeId, const std::string& uniqueID, ParameterAccess access, IEC_Datatype dataType)
{
	std::shared_ptr<Network> network;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, network);
	if (!res.IsSuccessful())
		return res;

	std::shared_ptr<BaseNode> node;
	res = network->GetBaseNode(nodeId, node);
	if (!res.IsSuccessful())
		return res;

	auto ptr = std::make_shared<Parameter>(uniqueID, access, dataType);
	return node->GetApplicationProcess()->AddParameter(ptr);

}

Result OpenConfiguratorCore::CreateParameter(const std::string& networkId, const std::uint8_t nodeId, const std::string& uniqueID, const std::string& uniqueIDRef, ParameterAccess access)
{
	std::shared_ptr<Network> network;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, network);
	if (!res.IsSuccessful())
		return res;

	std::shared_ptr<BaseNode> node;
	res = network->GetBaseNode(nodeId, node);
	if (!res.IsSuccessful())
		return res;

	auto ptr = std::make_shared<Parameter>(uniqueID, access, uniqueIDRef);
	return node->GetApplicationProcess()->AddParameter(ptr);
}

Result OpenConfiguratorCore::CreateStructDatatype(const std::string& networkId, const std::uint8_t nodeId, const std::string& uniqueId, const std::string& name)
{
	std::shared_ptr<Network> network;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, network);
	if (!res.IsSuccessful())
		return res;

	std::shared_ptr<BaseNode> node;
	res = network->GetBaseNode(nodeId, node);
	if (!res.IsSuccessful())
		return res;

	auto& parameterList = node->GetApplicationProcess()->GetParameterList();
	for (auto& param : parameterList)
	{
		if (param->GetUniqueIDRef() == uniqueId)
		{
			auto ptr = std::shared_ptr<ComplexDataType>(new StructDataType(uniqueId, name));
			param->SetComplexDataType(ptr);
			return Result();
		}
	}

	boost::format formatter(kMsgParameterNotFound);
	formatter
	% uniqueId
	% (std::uint32_t) node->GetNodeId();
	LOG_FATAL() << formatter.str();
	return Result(ErrorCode::PARAMETER_NOT_FOUND, formatter.str());
}

Result OpenConfiguratorCore::CreateVarDeclaration(const std::string& networkId, const std::uint8_t nodeId, const std::string& structUniqueId, const std::string& uniqueId, const std::string& name, IEC_Datatype datatype, std::uint32_t size, const std::string& initialValue)
{
	std::shared_ptr<Network> network;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, network);
	if (!res.IsSuccessful())
		return res;

	std::shared_ptr<BaseNode> node;
	res = network->GetBaseNode(nodeId, node);
	if (!res.IsSuccessful())
		return res;

	auto& parameterList = node->GetApplicationProcess()->GetParameterList();
	for (auto& param : parameterList)
	{
		if (param->GetUniqueIDRef() == structUniqueId)
		{
			auto ptr = std::dynamic_pointer_cast<StructDataType>(param->GetComplexDataType());
			auto sharedVar = std::make_shared<VarDeclaration>(uniqueId, name, datatype, size, initialValue);
			return ptr->AddVarDeclaration(sharedVar);
		}
	}

	boost::format formatter(kMsgStructDatatypeNotFound);
	formatter
	% structUniqueId
	% (std::uint32_t) node->GetNodeId();
	LOG_FATAL() << formatter.str();
	return Result(ErrorCode::PARAMETER_NOT_FOUND, formatter.str());
}

Result OpenConfiguratorCore::CreateArrayDatatype(const std::string& networkId, const std::uint8_t nodeId, const std::string& uniqueId, const std::string& name, std::uint32_t lowerLimit, std::uint32_t upperLimit, IEC_Datatype dataType)
{
	std::shared_ptr<Network> network;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, network);
	if (!res.IsSuccessful())
		return res;

	std::shared_ptr<BaseNode> node;
	res = network->GetBaseNode(nodeId, node);
	if (!res.IsSuccessful())
		return res;

	auto& parameterList = node->GetApplicationProcess()->GetParameterList();
	for (auto& param : parameterList)
	{
		if (param->GetUniqueIDRef() == uniqueId)
		{
			auto ptr = std::shared_ptr<ComplexDataType>(new ArrayDataType(uniqueId, name, lowerLimit, upperLimit, dataType));
			param->SetComplexDataType(ptr);
			param->SetUniqueIDRef(uniqueId);
			return res;
		}
	}

	boost::format formatter(kMsgParameterNotFound);
	formatter
	% uniqueId
	% (std::uint32_t) node->GetNodeId();
	LOG_FATAL() << formatter.str();
	return Result(ErrorCode::PARAMETER_NOT_FOUND, formatter.str());
}

Result OpenConfiguratorCore::CreateEnumDatatype(const std::string& networkId, const std::uint8_t nodeId, const std::string& uniqueId, const std::string& name, IEC_Datatype dataType, std::uint32_t size)
{
	std::shared_ptr<Network> network;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, network);
	if (!res.IsSuccessful())
		return res;

	std::shared_ptr<BaseNode> node;
	res = network->GetBaseNode(nodeId, node);
	if (!res.IsSuccessful())
		return res;

	auto& parameterList = node->GetApplicationProcess()->GetParameterList();
	for (auto& param : parameterList)
	{
		if (param->GetUniqueIDRef() == uniqueId)
		{
			auto ptr = std::shared_ptr<ComplexDataType>(new EnumDataType(uniqueId, name, dataType, size));
			param->SetComplexDataType(ptr);
			return res;
		}
	}

	boost::format formatter(kMsgParameterNotFound);
	formatter
	% uniqueId
	% (std::uint32_t) node->GetNodeId();
	LOG_FATAL() << formatter.str();
	return Result(ErrorCode::PARAMETER_NOT_FOUND, formatter.str());
}

Result OpenConfiguratorCore::CreateEnumValue(const std::string& networkId, const std::uint8_t nodeId, const std::string& uniqueId, const std::string& name, const std::string& value)
{
	std::shared_ptr<Network> network;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, network);
	if (!res.IsSuccessful())
		return res;

	std::shared_ptr<BaseNode> node;
	res = network->GetBaseNode(nodeId, node);
	if (!res.IsSuccessful())
		return res;

	auto& parameterList = node->GetApplicationProcess()->GetParameterList();
	for (auto& param : parameterList)
	{
		if (param->GetUniqueIDRef() == uniqueId)
		{
			auto ptr = std::dynamic_pointer_cast<EnumDataType>(param->GetComplexDataType());
			return ptr->AddEnumValue(name, value);
		}
	}

	boost::format formatter(kMsgParameterNotFound);
	formatter
	% uniqueId
	% (std::uint32_t) node->GetNodeId();
	LOG_FATAL() << formatter.str();
	return Result(ErrorCode::PARAMETER_NOT_FOUND, formatter.str());

}

Result OpenConfiguratorCore::GetDatatypeSize(const std::string& networkId, const std::uint8_t nodeId, const std::string& dataTypeUniqueId, std::uint32_t& size)
{
	std::shared_ptr<Network> network;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, network);
	if (!res.IsSuccessful())
		return res;

	std::shared_ptr<BaseNode> node;
	res = network->GetBaseNode(nodeId, node);
	if (!res.IsSuccessful())
		return res;

	auto& parameterList = node->GetApplicationProcess()->GetParameterList();
	for (auto& param : parameterList)
	{
		if (param->GetUniqueID() == dataTypeUniqueId || param->GetUniqueIDRef() == dataTypeUniqueId)
		{
			size = param->GetBitSize() / 8;
			return Result();
		}
	}

	boost::format formatter(kMsgComplexDatatypeNotFoundOnNode);
	formatter
	% dataTypeUniqueId
	% (std::uint32_t) node->GetNodeId();
	LOG_FATAL() << formatter.str();
	return Result(ErrorCode::COMPLEX_DATATYPE_NOT_FOUND, formatter.str());
}

Result OpenConfiguratorCore::CreateDynamicChannel(const std::string& networkId, const std::uint8_t nodeId, PlkDataType dataType, DynamicChannelAccessType accessType, std::uint32_t startIndex, std::uint32_t endIndex, std::uint32_t maxNumber, std::uint32_t addressOffset, std::uint8_t bitAlignment)
{
	std::shared_ptr<Network> network;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, network);
	if (!res.IsSuccessful())
		return res;

	std::shared_ptr<BaseNode> node;
	res = network->GetBaseNode(nodeId, node);
	if (!res.IsSuccessful())
		return res;

	auto mnPtr = std::dynamic_pointer_cast<ManagingNode>(node);
	if (mnPtr)
	{
		auto ptr = std::shared_ptr<DynamicChannel>(new DynamicChannel(dataType, accessType, startIndex, endIndex, maxNumber, addressOffset, bitAlignment));
		mnPtr->AddDynamicChannel(ptr);
		return res;
	}
	else
		return Result(ErrorCode::NODE_IS_NOT_MANAGING_NODE);
}

Result OpenConfiguratorCore::SetObjectActualValue(const std::string& networkId, const std::uint8_t nodeId, std::uint32_t objectId, const std::string& actualValue, bool force)
{
	std::shared_ptr<Network> network;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, network);
	if (!res.IsSuccessful())
		return res;

	std::shared_ptr<BaseNode> node;
	res = network->GetBaseNode(nodeId, node);
	if (!res.IsSuccessful())
		return res;

	return node->ForceObject(objectId, force, actualValue);
}

Result OpenConfiguratorCore::SetSubObjectActualValue(const std::string& networkId, const std::uint8_t nodeId, std::uint32_t objectId, std::uint8_t subObjectId, const std::string& actualValue, bool force)
{
	std::shared_ptr<Network> network;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, network);
	if (!res.IsSuccessful())
		return res;

	std::shared_ptr<BaseNode> node;
	res = network->GetBaseNode(nodeId, node);
	if (!res.IsSuccessful())
		return res;

	return node->ForceSubObject(objectId, subObjectId, force, actualValue);
}

Result OpenConfiguratorCore::ResetOperationMode(const std::string& networkId, const std::uint8_t nodeId)
{
	std::shared_ptr<Network> network;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, network);
	if (!res.IsSuccessful())
		return res;

	return network->SetOperationMode(nodeId, PlkOperationMode::NORMAL);
}

Result OpenConfiguratorCore::SetOperationModeChained(const std::string& networkId, const std::uint8_t nodeId)
{
	std::shared_ptr<Network> network;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, network);
	if (!res.IsSuccessful())
		return res;

	return network->SetOperationMode(nodeId, PlkOperationMode::CHAINED);
}

Result OpenConfiguratorCore::SetOperationModeMultiplexed(const std::string& networkId, const std::uint8_t nodeId, const std::uint8_t multiplexedCycle)
{
	std::shared_ptr<Network> network;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, network);
	if (!res.IsSuccessful())
		return res;

	return network->SetOperationMode(nodeId, PlkOperationMode::MULTIPLEXED, multiplexedCycle);
}

Result OpenConfiguratorCore::AddNodeAssignment(const std::string& networkId, const std::uint8_t nodeId, const NodeAssignment assignment)
{
	std::shared_ptr<Network> network;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, network);
	if (!res.IsSuccessful())
		return res;

	std::shared_ptr<BaseNode> node;
	res = network->GetBaseNode(nodeId, node);
	if (!res.IsSuccessful())
		return res;

	return node->AddNodeAssignement(assignment);
}

Result OpenConfiguratorCore::RemoveNodeAssignment(const std::string& networkId, const std::uint8_t nodeId, const NodeAssignment assignment)
{
	std::shared_ptr<Network> network;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, network);
	if (!res.IsSuccessful())
		return res;

	std::shared_ptr<BaseNode> node;
	res = network->GetBaseNode(nodeId, node);
	if (!res.IsSuccessful())
		return res;

	return node->RemoveNodeAssignment(assignment);
}

Result OpenConfiguratorCore::EnableNode(const std::string& networkId, const std::uint8_t nodeID, const bool enable)
{
	std::shared_ptr<Network> network;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, network);
	if (res.IsSuccessful())
	{
		return network->EnableNode(nodeID, enable);
	}
	return res;
}

Result OpenConfiguratorCore::GetObjectActualValue(const std::string& networkId, const std::uint8_t nodeId, std::uint32_t objectId, std::string& actualValueReturn)
{
	std::shared_ptr<Network> network;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, network);
	if (!res.IsSuccessful())
		return res;

	std::shared_ptr<BaseNode> node;
	res = network->GetBaseNode(nodeId, node);
	if (!res.IsSuccessful())
		return res;

	std::shared_ptr<Object> obj;
	res = node->GetObject(objectId, obj);
	if (!res.IsSuccessful())
		return res;

	if (obj->WriteToConfiguration())
	{
		std::string actualValue = "0x";
		actualValue.append(obj->GetTypedActualValue<std::string>());
		actualValueReturn = actualValue;
		return res;
	}
	//No actual value present
	boost::format formatter(kMsgObjectNoActualValue);
	formatter
	% objectId
	% (std::uint32_t) nodeId;
	LOG_FATAL() << formatter.str();
	return Result(ErrorCode::OBJECT_HAS_NO_ACTUAL_VALUE, formatter.str());
}

Result OpenConfiguratorCore::GetSubObjectActualValue(const std::string& networkId, const std::uint8_t nodeId, std::uint32_t objectId, std::uint8_t subObjectId, std::string& actualValueReturn)
{
	std::shared_ptr<Network> network;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, network);
	if (!res.IsSuccessful())
		return res;

	std::shared_ptr<BaseNode> node;
	res = network->GetBaseNode(nodeId, node);
	if (!res.IsSuccessful())
		return res;

	std::shared_ptr<SubObject> subobj;
	res = node->GetSubObject(objectId, subObjectId, subobj);
	if (!res.IsSuccessful())
		return res;

	if (subobj->WriteToConfiguration())
	{
		std::string actualValue = "0x";
		actualValue.append(subobj->GetTypedActualValue<std::string>());
		actualValueReturn = actualValue;
		return res;
	}
	//No actual value present
	boost::format formatter(kMsgSubObjectNoActualValue);
	formatter
	% objectId
	% (std::uint32_t) subObjectId
	% (std::uint32_t) nodeId;
	LOG_FATAL() << formatter.str();
	return Result(ErrorCode::OBJECT_HAS_NO_ACTUAL_VALUE, formatter.str());
}

Result OpenConfiguratorCore::GetObjectsWithActualValue(const std::string& networkId, const std::uint8_t nodeId, std::map<std::pair<std::uint32_t, std::int32_t> , std::string>& objects)
{
	std::shared_ptr<Network> network;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, network);
	if (!res.IsSuccessful())
		return res;

	std::shared_ptr<BaseNode> node;
	res = network->GetBaseNode(nodeId, node);
	if (!res.IsSuccessful())
		return res;

	for (auto& obj : node->GetObjectDictionary())
	{
		if (obj.second->HasActualValue())
		{
			auto pair = std::pair<std::uint32_t, std::int32_t>(obj.first, -1);
			objects.insert(std::pair<std::pair<std::uint32_t, std::int32_t> , std::string>(pair, "0x" + obj.second->GetTypedActualValue<std::string>()));
		}

		auto& subod = obj.second->GetSubObjectDictionary();
		for (auto& subobj : subod)
		{
			if (subobj.second->HasActualValue())
			{
				auto pair = std::pair<std::uint32_t, std::int32_t>(obj.first, subobj.first);
				objects.insert(std::pair<std::pair<std::uint32_t, std::int32_t> , std::string>(pair, "0x" + subobj.second->GetTypedActualValue<std::string>()));
			}
		}
	}
	return res;
}

Result OpenConfiguratorCore::SetNodeId(const std::string& networkId, const std::uint8_t nodeId, const std::uint8_t newNodeId)
{
	std::shared_ptr<Network> network;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, network);
	if (!res.IsSuccessful())
		return res;

	return network->SetNodeId(nodeId, newNodeId);
}

Result OpenConfiguratorCore::SetNodeName(const std::string& networkId, const std::uint8_t nodeId, const std::string& nodeName)
{
	std::shared_ptr<Network> network;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, network);
	if (!res.IsSuccessful())
		return res;

	std::shared_ptr<BaseNode> node;
	res = network->GetBaseNode(nodeId, node);
	if (!res.IsSuccessful())
		return res;

	if (!nodeName.empty())
		node->SetName(nodeName);

	return res;
}

Result OpenConfiguratorCore::SetAsyncSlotTimeout(const std::string& networkId, const std::uint8_t nodeId, std::uint32_t asyncSlotTimeout)
{
	std::shared_ptr<Network> network;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, network);
	if (!res.IsSuccessful())
		return res;

	std::shared_ptr<BaseNode> node;
	res = network->GetBaseNode(nodeId, node);
	if (!res.IsSuccessful())
		return res;

	auto ptr = std::dynamic_pointer_cast<ManagingNode>(node);
	if (ptr)
		res = ptr->ForceSubObject(0x1F8A, 0x2, false, IntToHex(asyncSlotTimeout, 8, "0x"));
	else
	{
		boost::format formatter(kMsgNonManagingNode);
		formatter
		% nodeId;
		LOG_FATAL() << formatter.str();
		return Result(ErrorCode::NODE_IS_NOT_MANAGING_NODE, formatter.str());
	}
	return res;
}

Result OpenConfiguratorCore::SetAsndMaxNr(const std::string& networkId, const std::uint8_t nodeId, std::uint8_t asndMaxNr)
{
	std::shared_ptr<Network> network;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, network);
	if (!res.IsSuccessful())
		return res;

	std::shared_ptr<BaseNode> node;
	res = network->GetBaseNode(nodeId, node);
	if (!res.IsSuccessful())
		return res;

	auto ptr = std::dynamic_pointer_cast<ManagingNode>(node);
	if (ptr)
		res = ptr->ForceSubObject(0x1F8A, 0x3, false, IntToHex((std::uint16_t) asndMaxNr, 2, "0x"));
	else
	{
		boost::format formatter(kMsgNonManagingNode);
		formatter
		% nodeId;
		LOG_FATAL() << formatter.str();
		return Result(ErrorCode::NODE_IS_NOT_MANAGING_NODE, formatter.str());
	}
	return res;
}

Result OpenConfiguratorCore::SetPResTimeOut(const std::string& networkId, const std::uint8_t nodeId, const std::uint32_t presTimeout)
{
	std::shared_ptr<Network> network;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, network);
	if (!res.IsSuccessful())
		return res;

	std::shared_ptr<ManagingNode> mnPtr;
	res = network->GetManagingNode(mnPtr);
	if (!res.IsSuccessful())
		return res;

	return mnPtr->ForceSubObject(0x1F92, (std::uint32_t) nodeId, false, IntToHex(presTimeout, 8, "0x"));
}

Result OpenConfiguratorCore::SetRedundantManagingNodeWaitNotActive(const std::string& networkId, const std::uint8_t nodeId, const std::uint32_t waitNotActive)
{
	std::shared_ptr<Network> network;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, network);
	if (!res.IsSuccessful())
		return res;

	std::shared_ptr<BaseNode> node;
	res = network->GetBaseNode(nodeId, node);
	if (!res.IsSuccessful())
		return res;

	auto ptr = std::dynamic_pointer_cast<ManagingNode>(node);
	if (ptr && nodeId != 240)
		res = ptr->ForceSubObject(0x1F89, 0x1, false, IntToHex(waitNotActive, 8, "0x"));
	else
	{
		boost::format formatter(kMsgNonRedundantManagingNode);
		formatter
		% nodeId;
		LOG_FATAL() << formatter.str();
		return Result(ErrorCode::NODE_IS_NOT_REDUNDANT_MANAGING_NODE, formatter.str());
	}
	return res;
}

Result OpenConfiguratorCore::SetRedundantManagingNodePriority(const std::string& networkId, const std::uint8_t nodeId, const std::uint32_t priority)
{
	std::shared_ptr<Network> network;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, network);
	if (!res.IsSuccessful())
		return res;

	std::shared_ptr<BaseNode> node;
	res = network->GetBaseNode(nodeId, node);
	if (!res.IsSuccessful())
		return res;

	auto ptr = std::dynamic_pointer_cast<ManagingNode>(node);
	if (ptr && nodeId != 240)
		res = ptr->ForceSubObject(0x1F89, 0xA, false, IntToHex(priority, 8, "0x"));
	else
	{
		boost::format formatter(kMsgNonRedundantManagingNode);
		formatter
		% nodeId;
		LOG_FATAL() << formatter.str();
		return Result(ErrorCode::NODE_IS_NOT_REDUNDANT_MANAGING_NODE, formatter.str());
	}
	return res;
}

Result OpenConfiguratorCore::GetCycleTime(const std::string& networkId, std::uint32_t& cycleTime)
{
	std::shared_ptr<Network> networkPtr;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, networkPtr);
	if (!res.IsSuccessful())
		return res;

	std::shared_ptr<ManagingNode> nodePtr;
	res = networkPtr->GetManagingNode(nodePtr);
	if (!res.IsSuccessful())
		return res;

	std::shared_ptr<Object> object;
	res = nodePtr->GetObject(0x1006, object);
	if (!res.IsSuccessful())
		return res;
	if (object->WriteToConfiguration())
		cycleTime = object->GetTypedActualValue<std::uint32_t>();
	else
	{
		boost::format formatter(kMsgObjectNoActualValue);
		formatter
		% (std::uint32_t) 0x1006
		% (std::uint32_t) 240;
		LOG_FATAL() << formatter.str();
		return Result(ErrorCode::OBJECT_HAS_NO_ACTUAL_VALUE, formatter.str());
	}
	return res;
}

Result OpenConfiguratorCore::GetAsyncMtu(const std::string& networkId, std::uint16_t& asyncMtu)
{
	std::shared_ptr<Network> networkPtr;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, networkPtr);
	if (!res.IsSuccessful())
		return res;

	std::shared_ptr<ManagingNode> nodePtr;
	res = networkPtr->GetManagingNode(nodePtr);
	if (!res.IsSuccessful())
		return res;

	std::shared_ptr<SubObject> subobject;
	res = nodePtr->GetSubObject(0x1F98, 0x8, subobject);
	if (!res.IsSuccessful())
		return res;

	if (subobject->WriteToConfiguration())
		asyncMtu = subobject->GetTypedActualValue<std::uint16_t>();
	else
	{
		boost::format formatter(kMsgSubObjectNoActualValue);
		formatter
		% (std::uint32_t) 0x1F98
		% (std::uint32_t) 0x8
		% (std::uint32_t) 240;
		LOG_FATAL() << formatter.str();
		return Result(ErrorCode::OBJECT_HAS_NO_ACTUAL_VALUE, formatter.str());
	}
	return res;
}

Result OpenConfiguratorCore::GetMultiplexedCycleCount(const std::string& networkId, std::uint16_t& multiplexedCycleLength)
{
	std::shared_ptr<Network> networkPtr;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, networkPtr);
	if (!res.IsSuccessful())
		return res;

	std::shared_ptr<ManagingNode> nodePtr;
	res = networkPtr->GetManagingNode(nodePtr);
	if (!res.IsSuccessful())
		return res;

	std::shared_ptr<SubObject> subobject;
	res = nodePtr->GetSubObject(0x1F98, 0x7, subobject);
	if (!res.IsSuccessful())
		return res;

	if (subobject->WriteToConfiguration())
		multiplexedCycleLength = subobject->GetTypedActualValue<std::uint16_t>();
	else
	{
		boost::format formatter(kMsgSubObjectNoActualValue);
		formatter
		% (std::uint32_t) 0x1F98
		% (std::uint32_t) 0x7
		% (std::uint32_t) 240;
		LOG_FATAL() << formatter.str();
		return Result(ErrorCode::OBJECT_HAS_NO_ACTUAL_VALUE, formatter.str());
	}
	return res;
}
Result OpenConfiguratorCore::GetPrescaler(const std::string& networkId, std::uint16_t& prescaler)
{
	std::shared_ptr<Network> networkPtr;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, networkPtr);
	if (!res.IsSuccessful())
		return res;

	std::shared_ptr<ManagingNode> nodePtr;
	res = networkPtr->GetManagingNode(nodePtr);
	if (!res.IsSuccessful())
		return res;

	std::shared_ptr<SubObject> subobject;
	res = nodePtr->GetSubObject(0x1F98, 0x9, subobject);
	if (!res.IsSuccessful())
		return res;

	if (subobject->WriteToConfiguration())
		prescaler = subobject->GetTypedActualValue<std::uint16_t>();
	else
	{
		boost::format formatter(kMsgSubObjectNoActualValue);
		formatter
		% (std::uint32_t) 0x1F98
		% (std::uint32_t) 0x9
		% (std::uint32_t) 240;
		LOG_FATAL() << formatter.str();
		return Result(ErrorCode::OBJECT_HAS_NO_ACTUAL_VALUE, formatter.str());
	}
	return res;
}

Result OpenConfiguratorCore::GetAsyncSlotTimeout(const std::string& networkId, const std::uint8_t nodeId, std::uint32_t& asyncSlotTimeout)
{
	std::shared_ptr<Network> networkPtr;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, networkPtr);
	if (!res.IsSuccessful())
		return res;

	std::shared_ptr<ManagingNode> nodePtr;
	res = networkPtr->GetManagingNode(nodePtr);
	if (!res.IsSuccessful())
		return res;

	auto ptr = std::dynamic_pointer_cast<ManagingNode>(nodePtr);
	if (ptr)
	{
		std::shared_ptr<SubObject> subobject;
		res = nodePtr->GetSubObject(0x1F8A, 0x2, subobject);
		if (!res.IsSuccessful())
			return res;

		if (subobject->WriteToConfiguration())
			asyncSlotTimeout = subobject->GetTypedActualValue<std::uint32_t>();
		else
		{
			boost::format formatter(kMsgSubObjectNoActualValue);
			formatter
			% (std::uint32_t) 0x1F8A
			% (std::uint32_t) 0x2
			% (std::uint32_t) 240;
			LOG_FATAL() << formatter.str();
			return Result(ErrorCode::OBJECT_HAS_NO_ACTUAL_VALUE, formatter.str());
		}
	}
	else
	{
		boost::format formatter(kMsgNonManagingNode);
		formatter
		% nodeId;
		LOG_FATAL() << formatter.str();
		return Result(ErrorCode::NODE_IS_NOT_MANAGING_NODE, formatter.str());
	}
	return res;
}

Result OpenConfiguratorCore::GetAsndMaxNr(const std::string& networkId, const std::uint8_t nodeId, std::uint16_t& asndMaxNr)
{
	std::shared_ptr<Network> networkPtr;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, networkPtr);
	if (!res.IsSuccessful())
		return res;

	std::shared_ptr<BaseNode> nodePtr;
	res = networkPtr->GetBaseNode(nodeId, nodePtr);
	if (!res.IsSuccessful())
		return res;

	auto ptr = std::dynamic_pointer_cast<ManagingNode>(nodePtr);
	if (ptr)
	{
		std::shared_ptr<SubObject> subobject;
		res = nodePtr->GetSubObject(0x1F8A, 0x3, subobject);
		if (!res.IsSuccessful())
			return res;

		if (subobject->WriteToConfiguration())
			asndMaxNr = subobject->GetTypedActualValue<std::uint16_t>();
		else
		{
			boost::format formatter(kMsgSubObjectNoActualValue);
			formatter
			% (std::uint32_t) 0x1F8A
			% (std::uint32_t) 0x3
			% (std::uint32_t) 240;
			LOG_FATAL() << formatter.str();
			return Result(ErrorCode::OBJECT_HAS_NO_ACTUAL_VALUE, formatter.str());
		}
	}
	else
	{
		boost::format formatter(kMsgNonManagingNode);
		formatter
		% nodeId;
		LOG_FATAL() << formatter.str();
		return Result(ErrorCode::NODE_IS_NOT_MANAGING_NODE, formatter.str());
	}
	return res;
}

Result OpenConfiguratorCore::GetPResTimeOut(const std::string& networkId, const std::uint8_t nodeId, std::uint32_t& presTimeout)
{
	std::shared_ptr<Network> networkPtr;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, networkPtr);
	if (!res.IsSuccessful())
		return res;

	std::shared_ptr<ManagingNode> nodePtr;
	res = networkPtr->GetManagingNode(nodePtr);
	if (!res.IsSuccessful())
		return res;

	std::shared_ptr<SubObject> subobject;
	res = nodePtr->GetSubObject(0x1F92, (std::uint32_t) nodeId, subobject);
	if (!res.IsSuccessful())
		return res;

	if (subobject->WriteToConfiguration())
		presTimeout = subobject->GetTypedActualValue<std::uint32_t>();
	else
	{
		boost::format formatter(kMsgSubObjectNoActualValue);
		formatter
		% (std::uint32_t) 0x1F92
		% (std::uint32_t) nodeId
		% (std::uint32_t) 240;
		LOG_FATAL() << formatter.str();
		return Result(ErrorCode::OBJECT_HAS_NO_ACTUAL_VALUE, formatter.str());
	}


	return res;
}

Result OpenConfiguratorCore::GetRedundantManagingNodeWaitNotActive(const std::string& networkId, const std::uint8_t nodeId, std::uint32_t& waitNotActive)
{
	std::shared_ptr<Network> networkPtr;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, networkPtr);
	if (!res.IsSuccessful())
		return res;

	std::shared_ptr<ManagingNode> nodePtr;
	res = networkPtr->GetManagingNode(nodePtr);
	if (!res.IsSuccessful())
		return res;

	auto ptr = std::dynamic_pointer_cast<ManagingNode>(nodePtr);
	if (ptr && nodeId != 240)
	{
		std::shared_ptr<SubObject> subobject;
		res = nodePtr->GetSubObject(0x1F89, 0x1, subobject);
		if (!res.IsSuccessful())
			return res;

		if (subobject->WriteToConfiguration())
			waitNotActive = subobject->GetTypedActualValue<std::uint32_t>();
		else
		{
			boost::format formatter(kMsgSubObjectNoActualValue);
			formatter
			% (std::uint32_t) 0x1F89
			% (std::uint32_t) 0x1
			% (std::uint32_t) 240;
			LOG_FATAL() << formatter.str();
			return Result(ErrorCode::OBJECT_HAS_NO_ACTUAL_VALUE, formatter.str());
		}
	}
	else
	{
		boost::format formatter(kMsgNonRedundantManagingNode);
		formatter
		% nodeId;
		LOG_FATAL() << formatter.str();
		return Result(ErrorCode::NODE_IS_NOT_REDUNDANT_MANAGING_NODE, formatter.str());
	}
	return res;
}

Result OpenConfiguratorCore::GetRedundantManagingNodePriority(const std::string& networkId, const std::uint8_t nodeId, std::uint32_t& priority)
{
	std::shared_ptr<Network> networkPtr;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, networkPtr);
	if (!res.IsSuccessful())
		return res;

	std::shared_ptr<ManagingNode> nodePtr;
	res = networkPtr->GetManagingNode(nodePtr);
	if (!res.IsSuccessful())
		return res;

	auto ptr = std::dynamic_pointer_cast<ManagingNode>(nodePtr);
	if (ptr && nodeId != 240)
	{
		std::shared_ptr<SubObject> subobject;
		res = nodePtr->GetSubObject(0x1F89, 0xA, subobject);
		if (!res.IsSuccessful())
			return res;

		if (subobject->WriteToConfiguration())
			priority = subobject->GetTypedActualValue<std::uint32_t>();
		else
		{
			boost::format formatter(kMsgSubObjectNoActualValue);
			formatter
			% (std::uint32_t) 0x1F89
			% (std::uint32_t) 0xA
			% (std::uint32_t) 240;
			LOG_FATAL() << formatter.str();
			return Result(ErrorCode::OBJECT_HAS_NO_ACTUAL_VALUE, formatter.str());
		}
	}
	else
	{
		boost::format formatter(kMsgNonRedundantManagingNode);
		formatter
		% nodeId;
		LOG_FATAL() << formatter.str();
		return Result(ErrorCode::NODE_IS_NOT_REDUNDANT_MANAGING_NODE, formatter.str());
	}
	return res;
}

Result OpenConfiguratorCore::GetNodeAssignment(const std::string& networkId, const std::uint8_t nodeID, std::vector<std::uint32_t>& assignmentColl)
{
	std::shared_ptr<Network> networkPtr;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, networkPtr);
	if (!res.IsSuccessful())
		return res;

	std::shared_ptr<BaseNode> nodePtr;
	res = networkPtr->GetBaseNode(nodeID, nodePtr);
	if (!res.IsSuccessful())
		return res;

	for (auto assign : nodePtr->GetNodeAssignment())
	{
		assignmentColl.push_back(static_cast<std::underlying_type<NodeAssignment>::type>(assign));
	}
	return res;
}

Result OpenConfiguratorCore::SetLossOfSocTolerance(const std::string& networkId, const std::uint8_t nodeId, const std::uint32_t lossOfSocTolerance)
{
	std::shared_ptr<Network> network;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, network);
	if (!res.IsSuccessful())
		return res;

	std::shared_ptr<BaseNode> node;
	res = network->GetBaseNode(nodeId, node);
	if (!res.IsSuccessful())
		return res;

	return node->ForceObject(0x1C14, false, IntToHex(lossOfSocTolerance, 8, "0x"));
}

Result OpenConfiguratorCore::GetLossOfSocTolerance(const std::string& networkId, const std::uint8_t nodeId, std::uint32_t& lossOfSocTolerance)
{
	std::shared_ptr<Network> networkPtr;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, networkPtr);
	if (!res.IsSuccessful())
		return res;

	std::shared_ptr<BaseNode> nodePtr;
	res = networkPtr->GetBaseNode(nodeId, nodePtr);
	if (!res.IsSuccessful())
		return res;

	auto ptr = std::dynamic_pointer_cast<ControlledNode>(nodePtr);
	if (ptr)
	{
		std::shared_ptr<Object> object;
		res = nodePtr->GetObject(0x1C14, object);
		if (!res.IsSuccessful())
			return res;

		if (object->WriteToConfiguration())
			lossOfSocTolerance = object->GetTypedActualValue<std::uint32_t>();
		else
		{
			boost::format formatter(kMsgObjectNoActualValue);
			formatter
			% (std::uint32_t) 0x1C14
			% (std::uint32_t) 240;
			LOG_FATAL() << formatter.str();
			return Result(ErrorCode::OBJECT_HAS_NO_ACTUAL_VALUE, formatter.str());
		}
	}
	else
	{
		boost::format formatter(kMsgNonControlledNode);
		formatter
		% nodeId;
		LOG_FATAL() << formatter.str();
		return Result(ErrorCode::NODE_IS_NOT_CONTROLLED_NODE, formatter.str());
	}
	return res;
}

Result OpenConfiguratorCore::MapAllObjectsToChannel(const std::string& networkId, const std::uint8_t nodeId, const Direction dir, std::uint16_t channelNr, bool updateNrEntries)
{
	std::shared_ptr<Network> networkPtr;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, networkPtr);
	if (!res.IsSuccessful())
		return res;

	std::shared_ptr<BaseNode> nodePtr;
	res = networkPtr->GetBaseNode(nodeId, nodePtr);
	if (!res.IsSuccessful())
		return res;

	auto cn = std::dynamic_pointer_cast<ControlledNode>(nodePtr);
	if (cn)
	{
		if (dir == Direction::RX)
		{
			res =  cn->MapAllRxObjects(channelNr, updateNrEntries);
			if (!res.IsSuccessful())
				return Result(res.GetErrorType(), "Insufficient mapping objects available : " + res.GetErrorMessage());
			else
				return res;
		}
		else if (dir == Direction::TX)
		{
			res =  cn->MapAllTxObjects(channelNr, updateNrEntries);
			if (!res.IsSuccessful())
				return Result(res.GetErrorType(), "Insufficient mapping objects available : " + res.GetErrorMessage());
			else
				return res;
		}
	}

	return Result(ErrorCode::NODE_IS_NOT_CONTROLLED_NODE);
}

Result OpenConfiguratorCore::MapObjectToChannel(const std::string& networkId, const std::uint8_t nodeId, const Direction dir, std::uint16_t channelNr, std::uint16_t position, std::uint32_t objectIdToBeMapped, std::uint16_t fromNode, bool updateNrEntries)
{
	std::shared_ptr<Network> networkPtr;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, networkPtr);
	if (!res.IsSuccessful())
		return res;

	std::shared_ptr<BaseNode> nodePtr;
	res = networkPtr->GetBaseNode(nodeId, nodePtr);
	if (!res.IsSuccessful())
		return res;

	auto cn = std::dynamic_pointer_cast<ControlledNode>(nodePtr);
	if (cn)
		return cn->MapObject(objectIdToBeMapped, dir, channelNr, position, fromNode, updateNrEntries);
	else
		return Result(ErrorCode::NODE_IS_NOT_CONTROLLED_NODE);
}

Result OpenConfiguratorCore::MapSubObjectToChannel(const std::string& networkId, const std::uint8_t nodeId, const Direction dir, std::uint16_t channelNr, std::uint16_t position, std::uint32_t objectIdToBeMapped, std::uint16_t suObjectIdToBeMapped, std::uint16_t fromNode, bool updateNrEntries)
{
	std::shared_ptr<Network> networkPtr;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, networkPtr);
	if (!res.IsSuccessful())
		return res;

	std::shared_ptr<BaseNode> nodePtr;
	res = networkPtr->GetBaseNode(nodeId, nodePtr);
	if (!res.IsSuccessful())
		return res;

	auto cn = std::dynamic_pointer_cast<ControlledNode>(nodePtr);
	if (cn)
		return cn->MapSubObject(objectIdToBeMapped, suObjectIdToBeMapped, dir, channelNr, position, fromNode, updateNrEntries);
	else
		return Result(ErrorCode::NODE_IS_NOT_CONTROLLED_NODE);
}

Result OpenConfiguratorCore::GetChannelSize(const std::string& networkId, const std::uint8_t nodeId, const Direction dir, std::uint16_t channelNr, std::uint32_t& size)
{
	std::shared_ptr<Network> networkPtr;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, networkPtr);
	if (!res.IsSuccessful())
		return res;

	std::shared_ptr<BaseNode> nodePtr;
	res = networkPtr->GetBaseNode(nodeId, nodePtr);
	if (!res.IsSuccessful())
		return res;
	std::uint32_t channelObjectId = 0;

	if (dir == Direction::RX)
		channelObjectId = 0x1600 + channelNr;
	else if (dir == Direction::TX)
		channelObjectId = 0x1A00 + channelNr;

	std::shared_ptr<Object> mappingChannel;
	res = nodePtr->GetObject(channelObjectId, mappingChannel);
	if (!res.IsSuccessful())
		return res;

	std::uint16_t nrOfEntries = 0;
	for (auto& channelObj : mappingChannel->GetSubObjectDictionary())
	{
		if (channelObj.first == 0)
		{
			if (channelObj.second->HasActualValue())
			{
				nrOfEntries = channelObj.second->GetTypedActualValue<std::uint16_t>();
			}
			else if (channelObj.second->HasDefaultValue())
			{
				nrOfEntries = channelObj.second->GetTypedDefaultValue<std::uint16_t>();
			}
			continue;
		}

		if (nrOfEntries == 0)
			return Result();

		if (channelObj.second->HasActualValue())
		{
			std::uint64_t mappingValue = channelObj.second->GetTypedActualValue<std::uint64_t>();
			if (mappingValue != 0)
			{
				BaseProcessDataMapping mapping = BaseProcessDataMapping(channelObj.second->GetTypedActualValue<std::string>(), 0);
				size += mapping.GetMappingLength() / 8;
				nrOfEntries--;
			}
		}
		else if (channelObj.second->HasDefaultValue())
		{
			std::uint64_t mappingValue = channelObj.second->GetTypedDefaultValue<std::uint64_t>();
			if (mappingValue != 0)
			{
				BaseProcessDataMapping mapping = BaseProcessDataMapping(channelObj.second->GetTypedDefaultValue<std::string>(), 0);
				size += mapping.GetMappingLength() / 8;
				nrOfEntries--;
			}
		}
		else
			continue;
	}

	return res;
}

Result OpenConfiguratorCore::GetChannelActualValues(const std::string& networkId, const std::uint8_t nodeId, const Direction dir, std::uint16_t channelNr, std::map<std::pair<std::uint32_t, std::int32_t> , std::string>& objects)
{
	//Get the network
	std::shared_ptr<Network> networkPtr;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, networkPtr);
	if (!res.IsSuccessful())
		return res;

	//Get the node
	std::shared_ptr<BaseNode> nodePtr;
	res = networkPtr->GetBaseNode(nodeId, nodePtr);
	if (!res.IsSuccessful())
		return res;

	//Calculate the channel object index
	std::uint32_t channelObjectId = 0;

	if (dir == Direction::RX)
		channelObjectId = 0x1600 + channelNr;
	else if (dir == Direction::TX)
		channelObjectId = 0x1A00 + channelNr;

	std::shared_ptr<Object> mappingChannel;
	res = nodePtr->GetObject(channelObjectId, mappingChannel);
	if (!res.IsSuccessful())
		return res;

	for (auto& subobj : mappingChannel->GetSubObjectDictionary())
	{
		if (subobj.second->HasActualValue())
		{
			auto pair = std::pair<std::uint32_t, std::int32_t>(channelObjectId, subobj.first);
			objects.insert(std::pair<std::pair<std::uint32_t, std::int32_t> , std::string>(pair, "0x" + subobj.second->GetTypedActualValue<std::string>()));
		}
	}
	return res;
}

Result OpenConfiguratorCore::MoveMappingObject(const std::string& networkId, const std::uint8_t nodeId, const Direction dir, std::uint16_t channelNr, std::uint16_t oldPosition, std::uint16_t newPosition)
{
	std::shared_ptr<Network> networkPtr;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, networkPtr);
	if (!res.IsSuccessful())
		return res;

	std::shared_ptr<BaseNode> nodePtr;
	res = networkPtr->GetBaseNode(nodeId, nodePtr);
	if (!res.IsSuccessful())
		return res;

	auto cn = std::dynamic_pointer_cast<ControlledNode>(nodePtr);
	if (cn)
		return cn->MoveMappingObject(dir, channelNr, oldPosition, newPosition);
	else
		return Result(ErrorCode::NODE_IS_NOT_CONTROLLED_NODE, kMsgMappingOperationNotSupported);
}

Result OpenConfiguratorCore::ClearMappingObject(const std::string& networkId, const std::uint8_t nodeId, const Direction dir, std::uint16_t channelNr, std::uint16_t position)
{
	std::shared_ptr<Network> networkPtr;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, networkPtr);
	if (!res.IsSuccessful())
		return res;

	std::shared_ptr<BaseNode> nodePtr;
	res = networkPtr->GetBaseNode(nodeId, nodePtr);
	if (!res.IsSuccessful())
		return res;

	std::uint32_t channelObjectId = 0;

	if (dir == Direction::RX)
		channelObjectId = 0x1600 + channelNr;
	else if (dir == Direction::TX)
		channelObjectId = 0x1A00 + channelNr;


	std::shared_ptr<Object> mappingChannel;
	res = nodePtr->GetObject(channelObjectId, mappingChannel);
	if (!res.IsSuccessful())
		return res;

	for (auto& channelObj : mappingChannel->GetSubObjectDictionary())
	{
		if (channelObj.first == position)
		{
			channelObj.second->SetTypedObjectActualValue("0x0");
			break;
		}
	}

	auto cn = std::dynamic_pointer_cast<ControlledNode>(nodePtr);
	if (cn)
		return cn->UpdateProcessImage(dir);
	else
		return Result(ErrorCode::NODE_IS_NOT_CONTROLLED_NODE);
}

Result OpenConfiguratorCore::ClearMappingChannel(const std::string& networkId, const std::uint8_t nodeId, const Direction dir, std::uint16_t channelNr)
{
	std::shared_ptr<Network> networkPtr;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, networkPtr);
	if (!res.IsSuccessful())
		return res;

	std::shared_ptr<BaseNode> nodePtr;
	res = networkPtr->GetBaseNode(nodeId, nodePtr);
	if (!res.IsSuccessful())
		return res;

	std::uint32_t channelObjectId = 0;

	if (dir == Direction::RX)
		channelObjectId = 0x1600 + channelNr;
	else if (dir == Direction::TX)
		channelObjectId = 0x1A00 + channelNr;

	std::shared_ptr<Object> mappingChannel;
	res = nodePtr->GetObject(channelObjectId, mappingChannel);
	if (!res.IsSuccessful())
		return res;

	for (auto& channelObj : mappingChannel->GetSubObjectDictionary())
	{
		if (channelObj.first == 0)
		{
			channelObj.second->SetTypedObjectActualValue("0x0");
			continue;
		}
		channelObj.second->SetTypedObjectActualValue("0x0");
	}

	auto cn = std::dynamic_pointer_cast<ControlledNode>(nodePtr);
	if (cn)
		return cn->UpdateProcessImage(dir);
	else
		return Result(ErrorCode::NODE_IS_NOT_CONTROLLED_NODE);
}

//Result OpenConfiguratorCore::CreateOffsetGap(const std::string& networkId, const std::uint8_t nodeId, const Direction dir, std::uint16_t channelNr,
//        std::uint16_t position, std::uint32_t gapSize)
//{
//	return Result();
//}
//
//Result OpenConfiguratorCore::GetOffsetGapSize(const std::string& networkId, const std::uint8_t nodeId, const Direction dir, std::uint16_t channelNr, std::uint16_t position, std::uint8_t mappingSubObjectId, std::uint32_t& gapSize)
//{
//	return Result();
//}