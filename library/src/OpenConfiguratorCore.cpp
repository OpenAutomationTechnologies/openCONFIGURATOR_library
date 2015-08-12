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
#include <fstream>

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

Result OpenConfiguratorCore::SetCycleTime(const std::string& networkId, uint32_t cycleTime)
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

	std::stringstream cycleTimeStr;
	cycleTimeStr << cycleTime;
	return nodePtr->SetObjectActualValue(0x1006, cycleTimeStr.str());
}

Result OpenConfiguratorCore::SetAsyncMtu(const std::string& networkId, uint16_t asyncMtu)
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

	std::stringstream asyncMtuStr;
	asyncMtuStr << asyncMtu;
	return nodePtr->SetSubObjectActualValue(0x1F98, 0x8, asyncMtuStr.str());
}

Result OpenConfiguratorCore::SetMultiplexedCycleCount(const std::string& networkId, uint16_t multiplexedCycleCount)
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

	std::stringstream multiplexedCycleCountStr;
	multiplexedCycleCountStr << multiplexedCycleCount;
	return nodePtr->SetSubObjectActualValue(0x1F98, 0x7, multiplexedCycleCountStr.str());
}

Result OpenConfiguratorCore::SetPrescaler(const std::string& networkId, uint16_t prescaler)
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

	std::stringstream prescalerStr;
	prescalerStr << prescaler;
	return nodePtr->SetSubObjectActualValue(0x1F98, 0x9, prescalerStr.str());
}

Result OpenConfiguratorCore::BuildConfiguration(const std::string& networkId, std::string& configurationOutput, std::vector<std::uint8_t>& binOutput)
{
	std::shared_ptr<Network> networkPtr;
	std::stringstream config;
	std::stringstream hexOutput;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, networkPtr);
	if (res.IsSuccessful())
		res = networkPtr->GenerateConfiguration();
	if (res.IsSuccessful())
		res = ConfigurationGenerator::GetInstance().GenerateNetworkConfiguration(networkPtr, config, hexOutput);

	configurationOutput = config.str();
	ConfigurationToAscii(hexOutput, binOutput);
	return res;
}

Result OpenConfiguratorCore::BuildProcessImage(const std::string& networkId, std::string& configurationOutput)
{
	return Result(ErrorCode::UNHANDLED_EXCEPTION);
}

Result OpenConfiguratorCore::CreateNode(const std::string& networkId, const uint8_t nodeID, const std::string& nodeName, const bool isRmn)
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

Result OpenConfiguratorCore::RemoveNode(const std::string& networkId, const uint8_t nodeID)
{
	std::shared_ptr<Network> network;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, network);
	if (res.IsSuccessful())
	{
		return network->RemoveNode(nodeID);
	}
	return res;
}

Result OpenConfiguratorCore::GetControlledNode(const std::string& networkId, const uint8_t nodeID, ControlledNode& node)
{
	std::shared_ptr<Network> network;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, network);
	if (res.IsSuccessful())
	{
		std::shared_ptr<BaseNode> nodePtr;
		res = network->GetBaseNode(nodeID, nodePtr);
		if (res.IsSuccessful())
		{
			node = *std::dynamic_pointer_cast<ControlledNode>(nodePtr).get();
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

Result OpenConfiguratorCore::GetAvailableNodeIds(const std::string& networkId, std::vector<uint8_t>& nodeIdCollection)
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
	if (res.IsSuccessful())
	{
		std::vector<std::shared_ptr<PlkConfiguration>> configurations;
		res = network->GetBuildConfigurations(configurations);
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

Result OpenConfiguratorCore::CreateObject(const std::string& networkId, const uint8_t nodeId, uint32_t objectId, ObjectType objectType, const std::string& name, PlkDataType dataType, AccessType accessType, PDOMapping pdoMapping, const std::string& defaultValueToSet, const std::string& actualValueToSet)
{
	std::shared_ptr<Network> network;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, network);
	if (res.IsSuccessful())
	{
		std::shared_ptr<BaseNode> node;
		res = network->GetBaseNode(nodeId, node);

		if (res.IsSuccessful())
		{
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

			res = node->AddObject(ptr);
		}
	}
	return res;
}

Result OpenConfiguratorCore::SetObjectLimits(const std::string& networkId, const uint8_t nodeId, uint32_t objectId,  const std::string& lowLimit, const std::string& highLimit)
{
	std::shared_ptr<Network> network;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, network);
	if (res.IsSuccessful())
	{
		std::shared_ptr<BaseNode> node;
		res = network->GetBaseNode(nodeId, node);
		if (res.IsSuccessful())
		{
			std::shared_ptr<Object> obj;
			res = node->GetObject(objectId, obj);
			if (res.IsSuccessful())
			{
				res = obj->SetLowLimit(lowLimit);
				if (!res.IsSuccessful())
					return res;
				res = obj->SetHighLimit(highLimit);
			}
		}
	}
	return res;
}

Result OpenConfiguratorCore::CreateDomainObject(const std::string& networkId, const uint8_t nodeId, uint32_t objectId, ObjectType objectType, const std::string& name, const std::string& uniqueIdRef)
{
	std::shared_ptr<Network> network;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, network);
	if (res.IsSuccessful())
	{
		std::shared_ptr<BaseNode> node;
		res = network->GetBaseNode(nodeId, node);
		if (res.IsSuccessful())
		{
			std::shared_ptr<Parameter> param;
			res = node->GetApplicationProcess()->GetParameter(uniqueIdRef, param);
			if (res.IsSuccessful())
			{
				std::shared_ptr<Object> ptr(new Object(objectId, objectType, name, nodeId, uniqueIdRef));
				ptr->SetComplexDataType(param);
				res = node->AddObject(ptr);
			}
		}
	}
	return res;
}

Result OpenConfiguratorCore::CreateSubObject(const std::string& networkId, const uint8_t nodeId, uint32_t objectId, uint8_t subObjectId, ObjectType objectType, const std::string& name, PlkDataType dataType, AccessType accessType, PDOMapping pdoMapping, const std::string& defaultValueToSet, const std::string& actualValueToSet)
{
	std::shared_ptr<Network> network;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, network);
	if (res.IsSuccessful())
	{
		std::shared_ptr<BaseNode> node;
		res = network->GetBaseNode(nodeId, node);
		if (res.IsSuccessful())
		{
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

			res = node->AddSubObject(objectId, ptr);
		}
	}
	return res;
}

Result OpenConfiguratorCore::SetSubObjectLimits(const std::string& networkId, const uint8_t nodeId, uint32_t objectId, uint8_t subObjectId, const std::string& lowLimit, const std::string& highLimit)
{
	std::shared_ptr<Network> network;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, network);
	if (res.IsSuccessful())
	{
		std::shared_ptr<BaseNode> node;
		res = network->GetBaseNode(nodeId, node);
		if (res.IsSuccessful())
		{
			std::shared_ptr<SubObject> subObj;
			res = node->GetSubObject(objectId, subObjectId, subObj);
			if (res.IsSuccessful())
			{
				res = subObj->SetLowLimit(lowLimit);
				if (!res.IsSuccessful())
					return res;
				res = subObj->SetHighLimit(highLimit);
			}
		}
	}
	return res;
}

Result OpenConfiguratorCore::CreateDomainSubObject(const std::string& networkId, const uint8_t nodeId, uint32_t objectId, uint8_t subObjectId, ObjectType objectType, const std::string& name, const std::string& uniqueIdRef)
{
	std::shared_ptr<Network> network;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, network);
	if (res.IsSuccessful())
	{
		std::shared_ptr<BaseNode> node;
		res = network->GetBaseNode(nodeId, node);
		if (res.IsSuccessful())
		{
			std::shared_ptr<Parameter> param;
			res = node->GetApplicationProcess()->GetParameter(uniqueIdRef, param);
			if (res.IsSuccessful())
			{
				std::shared_ptr<SubObject> ptr(new SubObject(subObjectId, objectType, name, nodeId, uniqueIdRef));
				ptr->SetComplexDataType(param);
				res = node->AddSubObject(objectId, ptr);
			}
		}
	}
	return res;
}

Result OpenConfiguratorCore::GetObjectSize(const std::string& networkId, const uint8_t nodeId, uint32_t objectId, uint32_t& size)
{
	std::shared_ptr<Network> network;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, network);
	if (res.IsSuccessful())
	{
		std::shared_ptr<BaseNode> node;
		res = network->GetBaseNode(nodeId, node);
		if (res.IsSuccessful())
		{
			std::shared_ptr<Object> object;
			res = node->GetObject(objectId, object);
			if (res.IsSuccessful())
			{
				size = object->GetBitSize() / 8;
			}
		}
	}
	return res;
}

Result OpenConfiguratorCore::GetSubObjectSize(const std::string& networkId, const uint8_t nodeId, uint32_t objectId, uint8_t subObjectId, uint32_t& size)
{
	std::shared_ptr<Network> network;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, network);
	if (res.IsSuccessful())
	{
		std::shared_ptr<BaseNode> node;
		res = network->GetBaseNode(nodeId, node);
		if (res.IsSuccessful())
		{
			std::shared_ptr<Object> object;
			res = node->GetObject(objectId, object);
			if (res.IsSuccessful())
			{
				std::shared_ptr<SubObject> subObject;
				res = object->GetSubObject(subObjectId, subObject);
				if (res.IsSuccessful())
				{
					size = subObject->GetBitSize() / 8;
				}
			}
		}
	}
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

Result OpenConfiguratorCore::SetFeatureValue(const std::string& networkId, const uint8_t nodeId, CNFeatureEnum feature, const std::string& value)
{
	std::shared_ptr<Network> network;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, network);
	if (res.IsSuccessful())
	{
		std::shared_ptr<BaseNode> node;
		res = network->GetBaseNode(nodeId, node);
		if (res.IsSuccessful())
		{
			return node->GetNetworkManagement()->SetFeatureUntypedActualValue(feature, value);
		}

	}
	return res;
}

Result OpenConfiguratorCore::SetFeatureValue(const std::string& networkId, const uint8_t nodeId, MNFeatureEnum feature, const std::string& value)
{
	std::shared_ptr<Network> network;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, network);
	if (res.IsSuccessful())
	{
		std::shared_ptr<BaseNode> node;
		res = network->GetBaseNode(nodeId, node);
		if (res.IsSuccessful())
		{
			return node->GetNetworkManagement()->SetFeatureUntypedActualValue(feature, value);
		}
	}
	return res;
}

Result OpenConfiguratorCore::SetFeatureValue(const std::string& networkId, const uint8_t nodeId, GeneralFeatureEnum feature, const std::string& value)
{
	std::shared_ptr<Network> network;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, network);
	if (res.IsSuccessful())
	{
		std::shared_ptr<BaseNode> node;
		res = network->GetBaseNode(nodeId, node);
		if (res.IsSuccessful())
		{
			return node->GetNetworkManagement()->SetFeatureUntypedActualValue(feature, value);
		}
	}
	return res;
}

Result OpenConfiguratorCore::CreateParameter(const std::string& networkId, const uint8_t nodeId, const std::string& uniqueID, ParameterAccess access, IEC_Datatype dataType)
{
	std::shared_ptr<Network> network;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, network);
	if (res.IsSuccessful())
	{
		std::shared_ptr<BaseNode> node;
		res = network->GetBaseNode(nodeId, node);
		if (res.IsSuccessful())
		{
			auto ptr = std::make_shared<Parameter>(uniqueID, access, dataType);
			return node->GetApplicationProcess()->AddParameter(ptr);
		}
	}
	return res;
}

Result OpenConfiguratorCore::CreateParameter(const std::string& networkId, const uint8_t nodeId, const std::string& uniqueID, const std::string& uniqueIDRef, ParameterAccess access)
{
	std::shared_ptr<Network> network;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, network);
	if (res.IsSuccessful())
	{
		std::shared_ptr<BaseNode> node;
		res = network->GetBaseNode(nodeId, node);
		if (res.IsSuccessful())
		{
			auto ptr = std::make_shared<Parameter>(uniqueID, access, uniqueIDRef);
			return node->GetApplicationProcess()->AddParameter(ptr);
		}
	}
	return res;
}

Result OpenConfiguratorCore::CreateStructDatatype(const std::string& networkId, const uint8_t nodeId, const std::string& uniqueId, const std::string& name)
{
	std::shared_ptr<Network> network;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, network);
	if (res.IsSuccessful())
	{
		std::shared_ptr<BaseNode> node;
		res = network->GetBaseNode(nodeId, node);
		if (res.IsSuccessful())
		{
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
			% (uint32_t) node->GetNodeIdentifier();
			LOG_FATAL() << formatter.str();
			return Result(ErrorCode::PARAMETER_NOT_FOUND, formatter.str());
		}
	}
	return res;
}

Result OpenConfiguratorCore::CreateVarDeclaration(const std::string& networkId, const uint8_t nodeId, const std::string& structUniqueId, const std::string& uniqueId, const std::string& name, IEC_Datatype datatype, uint32_t size, const std::string& initialValue)
{
	std::shared_ptr<Network> network;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, network);
	if (res.IsSuccessful())
	{
		std::shared_ptr<BaseNode> node;
		res = network->GetBaseNode(nodeId, node);
		if (res.IsSuccessful())
		{
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
			% (uint32_t) node->GetNodeIdentifier();
			LOG_FATAL() << formatter.str();
			return Result(ErrorCode::PARAMETER_NOT_FOUND, formatter.str());
		}
	}
	return res;
}

Result OpenConfiguratorCore::CreateArrayDatatype(const std::string& networkId, const uint8_t nodeId, const std::string& uniqueId, const std::string& name, uint32_t lowerLimit, uint32_t upperLimit, IEC_Datatype dataType)
{
	std::shared_ptr<Network> network;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, network);
	if (res.IsSuccessful())
	{
		std::shared_ptr<BaseNode> node;
		res = network->GetBaseNode(nodeId, node);
		if (res.IsSuccessful())
		{
			auto& parameterList = node->GetApplicationProcess()->GetParameterList();
			for (auto& param : parameterList)
			{
				if (param->GetUniqueIDRef() == uniqueId)
				{
					auto ptr = std::shared_ptr<ComplexDataType>(new ArrayDataType(uniqueId, name, lowerLimit, upperLimit, dataType));
					param->SetComplexDataType(ptr);
					param->SetUniqueIDRef(uniqueId);
					return Result();
				}
			}

			boost::format formatter(kMsgParameterNotFound);
			formatter
			% uniqueId
			% (uint32_t) node->GetNodeIdentifier();
			LOG_FATAL() << formatter.str();
			return Result(ErrorCode::PARAMETER_NOT_FOUND, formatter.str());
		}
	}
	return res;
}

Result OpenConfiguratorCore::CreateEnumDatatype(const std::string& networkId, const uint8_t nodeId, const std::string& uniqueId, const std::string& name, IEC_Datatype dataType, uint32_t size)
{
	std::shared_ptr<Network> network;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, network);
	if (res.IsSuccessful())
	{
		std::shared_ptr<BaseNode> node;
		res = network->GetBaseNode(nodeId, node);
		if (res.IsSuccessful())
		{
			auto& parameterList = node->GetApplicationProcess()->GetParameterList();
			for (auto& param : parameterList)
			{
				if (param->GetUniqueIDRef() == uniqueId)
				{
					auto ptr = std::shared_ptr<ComplexDataType>(new EnumDataType(uniqueId, name, dataType, size));
					param->SetComplexDataType(ptr);
					return Result();
				}
			}

			boost::format formatter(kMsgParameterNotFound);
			formatter
			% uniqueId
			% (uint32_t) node->GetNodeIdentifier();
			LOG_FATAL() << formatter.str();
			return Result(ErrorCode::PARAMETER_NOT_FOUND, formatter.str());
		}
	}
	return res;
}

Result OpenConfiguratorCore::CreateEnumValue(const std::string& networkId, const uint8_t nodeId, const std::string& uniqueId, const std::string& name, const std::string& value)
{
	std::shared_ptr<Network> network;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, network);
	if (res.IsSuccessful())
	{
		std::shared_ptr<BaseNode> node;
		res = network->GetBaseNode(nodeId, node);
		if (res.IsSuccessful())
		{
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
			% (uint32_t) node->GetNodeIdentifier();
			LOG_FATAL() << formatter.str();
			return Result(ErrorCode::PARAMETER_NOT_FOUND, formatter.str());
		}
	}
	return res;
}

Result OpenConfiguratorCore::GetDatatypeSize(const std::string& networkId, const uint8_t nodeId, const std::string& dataTypeUniqueId, uint32_t& size)
{
	std::shared_ptr<Network> network;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, network);
	if (res.IsSuccessful())
	{
		std::shared_ptr<BaseNode> node;
		res = network->GetBaseNode(nodeId, node);
		if (res.IsSuccessful())
		{
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
			% (uint32_t) node->GetNodeIdentifier();
			LOG_FATAL() << formatter.str();
			return Result(ErrorCode::COMPLEX_DATATYPE_NOT_FOUND, formatter.str());
		}
	}
	return res;
}

Result OpenConfiguratorCore::CreateDynamicChannel(const std::string& networkId, const uint8_t nodeId, PlkDataType dataType, DynamicChannelAccessType accessType, uint32_t startIndex, uint32_t endIndex, uint32_t maxNumber, uint32_t addressOffset, uint8_t bitAlignment)
{
	std::shared_ptr<Network> network;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, network);
	if (res.IsSuccessful())
	{
		std::shared_ptr<BaseNode> node;
		res = network->GetBaseNode(nodeId, node);
		if (res.IsSuccessful())
		{
			auto mnPtr = std::dynamic_pointer_cast<ManagingNode>(node);
			auto ptr = std::shared_ptr<DynamicChannel>(new DynamicChannel(dataType, accessType, startIndex, endIndex, maxNumber, addressOffset, bitAlignment));

			mnPtr->AddDynamicChannel(ptr);
			return Result();
		}
	}
	return res;
}

Result OpenConfiguratorCore::SetObjectActualValue(const std::string& networkId, const uint8_t nodeId, uint32_t objectId, const std::string& actualValue, bool force)
{
	std::shared_ptr<Network> network;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, network);
	if (res.IsSuccessful())
	{
		std::shared_ptr<BaseNode> node;
		res = network->GetBaseNode(nodeId, node);
		if (res.IsSuccessful())
		{
			res = node->ForceObject(objectId, force, actualValue);
		}
	}
	return res;
}

Result OpenConfiguratorCore::SetSubObjectActualValue(const std::string& networkId, const uint8_t nodeId, uint32_t objectId, uint8_t subObjectId, const std::string& actualValue, bool force)
{
	std::shared_ptr<Network> network;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, network);
	if (res.IsSuccessful())
	{
		std::shared_ptr<BaseNode> node;
		res = network->GetBaseNode(nodeId, node);
		if (res.IsSuccessful())
		{
			res = node->ForceSubObject(objectId, subObjectId, force, actualValue);
		}
	}
	return res;
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

Result OpenConfiguratorCore::CreateNodeAssignment(const std::string& networkId, const std::uint8_t nodeId, const NodeAssignment assignment)
{
	std::shared_ptr<Network> network;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, network);
	if (res.IsSuccessful())
	{
		std::shared_ptr<BaseNode> node;
		res = network->GetBaseNode(nodeId, node);
		if (res.IsSuccessful())
		{
			return node->AddNodeAssignement(assignment);
		}
	}
	return res;
}

Result OpenConfiguratorCore::RemoveNodeAssignment(const std::string& networkId, const std::uint8_t nodeId, const NodeAssignment assignment)
{
	std::shared_ptr<Network> network;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, network);
	if (res.IsSuccessful())
	{
		std::shared_ptr<BaseNode> node;
		res = network->GetBaseNode(nodeId, node);
		if (res.IsSuccessful())
		{
			return node->RemoveNodeAssignment(assignment);
		}
	}
	return res;
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
	if (res.IsSuccessful())
	{
		std::shared_ptr<BaseNode> node;
		res = network->GetBaseNode(nodeId, node);
		if (res.IsSuccessful())
		{
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
			% (uint32_t) nodeId;
			LOG_FATAL() << formatter.str();
			return Result(ErrorCode::OBJECT_HAS_NO_ACTUAL_VALUE, formatter.str());
		}
	}
	return res;
}

Result OpenConfiguratorCore::GetSubObjectActualValue(const std::string& networkId, const std::uint8_t nodeId, std::uint32_t objectId, std::uint8_t subObjectId, std::string& actualValueReturn)
{
	std::shared_ptr<Network> network;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, network);
	if (res.IsSuccessful())
	{
		std::shared_ptr<BaseNode> node;
		res = network->GetBaseNode(nodeId, node);
		if (res.IsSuccessful())
		{
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
			% subObjectId
			% (uint32_t) nodeId;
			LOG_FATAL() << formatter.str();
			return Result(ErrorCode::OBJECT_HAS_NO_ACTUAL_VALUE, formatter.str());
		}
	}
	return res;
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

	std::shared_ptr<Object> obj;
	auto& od = node->GetObjectDictionary();
	for (auto& obj : od)
	{
		if (obj.second->WriteToConfiguration())
		{
			auto pair = std::pair<std::uint32_t, std::int32_t>(obj.first, -1);
			objects.insert(std::pair<std::pair<std::uint32_t, std::int32_t> , std::string>(pair, "0x" + obj.second->GetTypedActualValue<std::string>()));
		}

		auto& subod = obj.second->GetSubObjectCollection();
		for (auto& subobj : subod)
		{
			if (subobj.second->WriteToConfiguration())
			{
				auto pair = std::pair<std::uint32_t, std::int32_t>(obj.first, subobj.first);
				objects.insert(std::pair<std::pair<std::uint32_t, std::int32_t> , std::string>(pair, "0x" + subobj.second->GetTypedActualValue<std::string>()));
			}
		}
	}
	return res;
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

	std::stringstream convert;
	convert << asyncSlotTimeout;

	auto ptr = std::dynamic_pointer_cast<ManagingNode>(node);
	if (ptr)
		res = ptr->ForceSubObject(0x1F8A, 0x2, false, convert.str());
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

	std::stringstream convert;
	convert << asndMaxNr;

	auto ptr = std::dynamic_pointer_cast<ManagingNode>(node);
	if (ptr)
		res = ptr->ForceSubObject(0x1F8A, 0x3, false, convert.str());
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