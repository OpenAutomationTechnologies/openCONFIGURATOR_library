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

Result OpenConfiguratorCore::InitLoggingConfiguration(const string& configuration)
{
	return ProjectManager::GetInstance().InitLoggingConfiguration(configuration);
}

Result OpenConfiguratorCore::GetSupportedSettingIds(vector<string>& supportedSettings)
{
	supportedSettings = ProjectManager::GetInstance().GetSupportedSettingIds();
	return Result();
}

Result OpenConfiguratorCore::CreateNetwork(const std::string& networkId)
{
	shared_ptr<Network> net = make_shared<Network>(networkId);
	return ProjectManager::GetInstance().AddNetwork(networkId, net);
}

Result OpenConfiguratorCore::RemoveNetwork(const std::string& networkId)
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

Result OpenConfiguratorCore::GetNetwork(const std::string& networkId, Network& network)
{
	shared_ptr<Network> networkPtr;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, networkPtr);
	if (res.IsSuccessful())
		network = *networkPtr.get();
	return res;

}

Result OpenConfiguratorCore::SetCycleTime(const std::string& networkId, uint32_t cycleTime)
{
	shared_ptr<Network> networkPtr;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, networkPtr);
	if (res.IsSuccessful())
		networkPtr->SetCycleTime(cycleTime);

	shared_ptr<ManagingNode> nodePtr;
	res = networkPtr->GetManagingNode(nodePtr);
	if (res.IsSuccessful())
	{
		stringstream cycleTimeStr;
		cycleTimeStr << cycleTime;
		return nodePtr->SetSubObjectActualValue(0x1006, 0x0, cycleTimeStr.str());
	}

	return res;
}

Result OpenConfiguratorCore::SetAsyncMtu(const std::string& networkId, uint32_t asyncMtu)
{
	shared_ptr<Network> networkPtr;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, networkPtr);
	if (res.IsSuccessful())
		networkPtr->SetAsyncMTU(asyncMtu);

	shared_ptr<ManagingNode> nodePtr;
	res = networkPtr->GetManagingNode(nodePtr);
	if (res.IsSuccessful())
	{
		stringstream asyncMtuStr;
		asyncMtuStr << asyncMtu;
		return nodePtr->SetSubObjectActualValue(0x1F98, 0x8, asyncMtuStr.str());
	}

	return res;

}

Result OpenConfiguratorCore::SetMultiplexedCycleLength(const std::string& networkId, uint16_t multiplexedCycleLength)
{
	shared_ptr<Network> networkPtr;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, networkPtr);
	if (res.IsSuccessful())
		networkPtr->SetMultiplexedCycleLength(multiplexedCycleLength);

	shared_ptr<ManagingNode> nodePtr;
	res = networkPtr->GetManagingNode(nodePtr);
	if (res.IsSuccessful())
	{
		stringstream multiplexedCycleCountStr;
		multiplexedCycleCountStr << multiplexedCycleLength;
		return nodePtr->SetSubObjectActualValue(0x1F98, 0x7, multiplexedCycleCountStr.str());
	}

	return res;
}

Result OpenConfiguratorCore::SetPrescaler(const std::string& networkId, uint16_t prescaler)
{
	shared_ptr<Network> networkPtr;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, networkPtr);
	if (res.IsSuccessful())
		networkPtr->SetPrescaler(prescaler);

	shared_ptr<ManagingNode> nodePtr;
	res = networkPtr->GetManagingNode(nodePtr);
	if (res.IsSuccessful())
	{
		stringstream prescalerStr;
		prescalerStr << prescaler;
		return nodePtr->SetSubObjectActualValue(0x1F98, 0x9, prescalerStr.str());
	}

	return res;
}

Result OpenConfiguratorCore::BuildConfiguration(const std::string& networkId, string& configurationOutput)
{
	shared_ptr<Network> networkPtr;
	stringstream config;
	stringstream hexOutput;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, networkPtr);
	if (res.IsSuccessful())
		res = networkPtr->GenerateConfiguration();
	if (res.IsSuccessful())
		res = ConfigurationGenerator::GetInstance().GenerateNetworkConfiguration(networkPtr, config, hexOutput);

	configurationOutput = config.str();
	cout << hexOutput.str() << endl << endl;

	return res;
}

Result OpenConfiguratorCore::BuildProcessImage(const std::string& networkId, string& configurationOutput)
{
	return Result(ErrorCode::UNHANDLED_EXCEPTION);
}

Result OpenConfiguratorCore::CreateNode(const std::string& networkId, const uint8_t nodeID, const std::string& nodeName, const bool isRmn)
{
	shared_ptr<Network> network;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, network);
	if (res.IsSuccessful())
	{
		//Add active managing node to network
		if (nodeID == 240)
		{
			shared_ptr<ManagingNode> node = make_shared<ManagingNode>(true, nodeID, nodeName);
			res = network->AddNode(node);
		}
		//Add redundant managing node to network
		else if ((nodeID >= 241 && nodeID <= 250) || isRmn) //allow the RMN to have normal nodeIDs
		{
			shared_ptr<ManagingNode> node = make_shared<ManagingNode>(false, nodeID, nodeName);
			res = network->AddNode(node);
		}
		//Add normal controlled node to network
		else if (nodeID >= 1 && nodeID <= 239)
		{
			shared_ptr<ControlledNode> node = make_shared<ControlledNode>(nodeID, nodeName);
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
	shared_ptr<Network> network;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, network);
	if (res.IsSuccessful())
	{
		return network->RemoveNode(nodeID);
	}
	return res;
}

Result OpenConfiguratorCore::GetControlledNode(const std::string& networkId, const uint8_t nodeID, ControlledNode& node)
{
	shared_ptr<Network> network;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, network);
	if (res.IsSuccessful())
	{
		shared_ptr<BaseNode> nodePtr;
		res = network->GetBaseNode(nodeID, nodePtr);
		if (res.IsSuccessful())
		{
			node = *dynamic_pointer_cast<ControlledNode>(nodePtr).get();
		}
	}
	return res;
}

Result OpenConfiguratorCore::GetManagingNode(const std::string& networkId, ManagingNode& node)
{
	shared_ptr<Network> network;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, network);
	if (res.IsSuccessful())
	{
		shared_ptr<ManagingNode> nodePtr;
		res = network->GetManagingNode(nodePtr);
		if (res.IsSuccessful())
		{
			node = *nodePtr.get();
		}
	}
	return res;
}

Result OpenConfiguratorCore::GetAvailableNodeIds(const std::string& networkId, vector<uint8_t>& nodeIdCollection)
{
	shared_ptr<Network> network;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, network);
	if (res.IsSuccessful())
	{
		return network->GetAvailableNodeIds(nodeIdCollection);
	}
	return res;
}

Result OpenConfiguratorCore::CreateConfiguration(const std::string& networkId, const std::string& configID)
{
	shared_ptr<Network> network;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, network);
	if (res.IsSuccessful())
	{
		return network->AddConfiguration(configID);
	}
	return res;
}

Result OpenConfiguratorCore::RemoveConfiguration(const std::string& networkId, const std::string& configID)
{
	shared_ptr<Network> network;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, network);
	if (res.IsSuccessful())
	{
		return network->RemoveConfiguration(configID);
	}
	return res;
}

Result OpenConfiguratorCore::ReplaceConfigurationName(const std::string& networkId, const std::string& oldConfigId, const std::string& newConfigId)
{
	shared_ptr<Network> network;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, network);
	if (res.IsSuccessful())
	{
		return network->ReplaceConfigurationName(oldConfigId, newConfigId);
	}
	return res;
}

Result OpenConfiguratorCore::CreateConfigurationSetting(const std::string& networkId, const std::string& configID, const std::string& name, const std::string& value)
{
	shared_ptr<Network> network;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, network);
	if (res.IsSuccessful())
	{
		return network->AddConfigurationSetting(configID, make_shared<BuildConfigurationSetting>(name, value));
	}
	return res;
}

Result OpenConfiguratorCore::RemoveConfigurationSetting(const std::string& networkId, const std::string& configID, const std::string& name)
{
	shared_ptr<Network> network;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, network);
	if (res.IsSuccessful())
	{
		return network->RemoveConfigurationSetting(configID, name);
	}
	return res;
}

Result OpenConfiguratorCore::SetConfigurationSettingEnabled(const std::string& networkId, const std::string& configID, const std::string& settingID, bool enabled)
{
	shared_ptr<Network> network;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, network);
	if (res.IsSuccessful())
	{
		return network->SetConfigurationSettingEnabled(configID, settingID, enabled);
	}
	return res;
}

Result OpenConfiguratorCore::GetConfigurationSettings(const std::string& networkId, const std::string& configID, vector<BuildConfigurationSetting>& settings)
{
	shared_ptr<Network> network;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, network);
	if (res.IsSuccessful())
	{
		vector<shared_ptr<BuildConfigurationSetting>> configurationSettings;
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

Result OpenConfiguratorCore::GetActiveConfiguration(const std::string& networkId, string& activeConfiguration)
{
	shared_ptr<Network> network;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, network);
	if (res.IsSuccessful())
	{
		activeConfiguration = network->GetActiveConfiguration();
	}
	return res;
}

Result OpenConfiguratorCore::SetActiveConfiguration(const std::string& networkId, const std::string& configID)
{
	shared_ptr<Network> network;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, network);
	if (res.IsSuccessful())
	{
		network->SetActiveConfiguration(configID);
	}
	return res;
}

Result OpenConfiguratorCore::GetBuildConfigurations(const std::string& networkId, vector<PlkConfiguration>& buildConfigurations)
{
	shared_ptr<Network> network;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, network);
	if (res.IsSuccessful())
	{
		vector<shared_ptr<PlkConfiguration>> configurations;
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

Result OpenConfiguratorCore::CreateObject(const std::string& networkId, const uint8_t nodeId, uint32_t objectId, ObjectType objectType, const string& name, PlkDataType dataType, AccessType accessType, PDOMapping pdoMapping, const string& defaultValueToSet, const string& actualValueToSet)
{
	shared_ptr<Network> network;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, network);
	if (res.IsSuccessful())
	{
		shared_ptr<BaseNode> node;
		res = network->GetBaseNode(nodeId, node);

		if (res.IsSuccessful())
		{
			shared_ptr<Object> ptr(new Object(objectId, objectType, name, nodeId));

			if (dataType != PlkDataType::UNDEFINED)
				ptr->SetDataType(dataType);

			if (accessType != AccessType::UNDEFINED)
				ptr->SetAccessType(accessType);

			if (pdoMapping != PDOMapping::UNDEFINED)
				ptr->SetPDOMapping(pdoMapping);

			if (!defaultValueToSet.empty())
				ptr->SetTypedObjectDefaultValue(defaultValueToSet);

			if (!actualValueToSet.empty())
				ptr->SetTypedObjectActualValue(actualValueToSet);

			res = node->AddObject(ptr);
		}
	}
	return res;
}

Result OpenConfiguratorCore::SetObjectLimits(const std::string& networkId, const uint8_t nodeId, uint32_t objectId, int64_t highlimit, int64_t lowLimit)
{
	if (lowLimit > highlimit)
		return Result(ErrorCode::OBJECT_LIMITS_INVALID);

	shared_ptr<Network> network;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, network);
	if (res.IsSuccessful())
	{
		shared_ptr<BaseNode> node;
		res = network->GetBaseNode(nodeId, node);
		if (res.IsSuccessful())
		{
			shared_ptr<Object> obj;
			res = node->GetObject(objectId, obj);
			if (res.IsSuccessful())
			{
				obj->SetHighLimit(highlimit);
				obj->SetLowLimit(lowLimit);
			}
		}
	}
	return res;
}

Result OpenConfiguratorCore::CreateDomainObject(const std::string& networkId, const uint8_t nodeId, uint32_t objectId, ObjectType objectType, const string& name, const string& uniqueIdRef)
{
	shared_ptr<Network> network;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, network);
	if (res.IsSuccessful())
	{
		shared_ptr<BaseNode> node;
		res = network->GetBaseNode(nodeId, node);
		if (res.IsSuccessful())
		{
			shared_ptr<Parameter> param;
			res = node->GetApplicationProcess()->GetParameter(uniqueIdRef, param);
			if (res.IsSuccessful())
			{
				shared_ptr<Object> ptr(new Object(objectId, objectType, name, nodeId, uniqueIdRef));
				ptr->SetComplexDataType(param);
				res = node->AddObject(ptr);
			}
		}
	}
	return res;
}

Result OpenConfiguratorCore::CreateSubObject(const std::string& networkId, const uint8_t nodeId, uint32_t objectId, uint32_t subObjectId, ObjectType objectType, const string& name, PlkDataType dataType, AccessType accessType, PDOMapping pdoMapping, const string& defaultValueToSet, const string& actualValueToSet)
{
	shared_ptr<Network> network;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, network);
	if (res.IsSuccessful())
	{
		shared_ptr<BaseNode> node;
		res = network->GetBaseNode(nodeId, node);
		if (res.IsSuccessful())
		{
			shared_ptr<SubObject> ptr(new SubObject(subObjectId, objectType, name, nodeId));

			if (dataType != PlkDataType::UNDEFINED)
				ptr->SetDataType(dataType);

			if (accessType != AccessType::UNDEFINED)
				ptr->SetAccessType(accessType);

			if (pdoMapping != PDOMapping::UNDEFINED)
				ptr->SetPDOMapping(pdoMapping);

			if (!defaultValueToSet.empty())
				ptr->SetTypedObjectDefaultValue(defaultValueToSet);

			if (!actualValueToSet.empty())
				ptr->SetTypedObjectActualValue(actualValueToSet);

			res = node->AddSubObject(objectId, ptr);
		}
	}
	return res;
}

Result OpenConfiguratorCore::SetSubObjectLimits(const std::string& networkId, const uint8_t nodeId, uint32_t objectId, uint32_t subObjectId, int64_t highlimit, int64_t lowLimit)
{
	if (lowLimit > highlimit)
		return Result(ErrorCode::OBJECT_LIMITS_INVALID);

	shared_ptr<Network> network;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, network);
	if (res.IsSuccessful())
	{
		shared_ptr<BaseNode> node;
		res = network->GetBaseNode(nodeId, node);
		if (res.IsSuccessful())
		{
			shared_ptr<SubObject> subObj;
			res = node->GetSubObject(objectId, subObjectId, subObj);
			if (res.IsSuccessful())
			{
				subObj->SetHighLimit(highlimit);
				subObj->SetLowLimit(lowLimit);
			}
		}
	}
	return res;
}

Result OpenConfiguratorCore::CreateDomainSubObject(const std::string& networkId, const uint8_t nodeId, uint32_t objectId, uint32_t subObjectId, ObjectType objectType, const string& name, const string& uniqueIdRef)
{
	shared_ptr<Network> network;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, network);
	if (res.IsSuccessful())
	{
		shared_ptr<BaseNode> node;
		res = network->GetBaseNode(nodeId, node);
		if (res.IsSuccessful())
		{
			shared_ptr<Parameter> param;
			res = node->GetApplicationProcess()->GetParameter(uniqueIdRef, param);
			if (res.IsSuccessful())
			{
				shared_ptr<SubObject> ptr(new SubObject(subObjectId, objectType, name, nodeId, uniqueIdRef));
				ptr->SetComplexDataType(param);
				res = node->AddSubObject(objectId, ptr);
			}
		}
	}
	return res;
}

Result OpenConfiguratorCore::GetObjectSize(const std::string& networkId, const uint8_t nodeId, uint32_t objectId, uint32_t& size)
{
	shared_ptr<Network> network;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, network);
	if (res.IsSuccessful())
	{
		shared_ptr<BaseNode> node;
		res = network->GetBaseNode(nodeId, node);
		if (res.IsSuccessful())
		{
			shared_ptr<Object> object;
			res = node->GetObject(objectId, object);
			if (res.IsSuccessful())
			{
				size = object->GetBitSize() / 8;
			}
		}
	}
	return res;
}

Result OpenConfiguratorCore::GetSubObjectSize(const std::string& networkId, const uint8_t nodeId, uint32_t objectId, uint32_t subObjectId, uint32_t& size)
{
	shared_ptr<Network> network;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, network);
	if (res.IsSuccessful())
	{
		shared_ptr<BaseNode> node;
		res = network->GetBaseNode(nodeId, node);
		if (res.IsSuccessful())
		{
			shared_ptr<Object> object;
			res = node->GetObject(objectId, object);
			if (res.IsSuccessful())
			{
				shared_ptr<SubObject> subObject;
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

Result OpenConfiguratorCore::SetFeatureValue(const std::string& networkId, const uint8_t nodeId, CNFeatureEnum feature, const std::string& value)
{
	shared_ptr<Network> network;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, network);
	if (res.IsSuccessful())
	{
		shared_ptr<BaseNode> node;
		res = network->GetBaseNode(nodeId, node);
		if (res.IsSuccessful())
		{
			node->GetNetworkManagement()->SetFeatureUntypedActualValue(feature, value);
		}

	}
	return res;
}

Result OpenConfiguratorCore::SetFeatureValue(const std::string& networkId, const uint8_t nodeId, MNFeatureEnum feature, const std::string& value)
{
	shared_ptr<Network> network;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, network);
	if (res.IsSuccessful())
	{
		shared_ptr<BaseNode> node;
		res = network->GetBaseNode(nodeId, node);
		if (res.IsSuccessful())
		{
			node->GetNetworkManagement()->SetFeatureUntypedActualValue(feature, value);
		}
	}
	return res;
}

Result OpenConfiguratorCore::SetFeatureValue(const std::string& networkId, const uint8_t nodeId, GeneralFeatureEnum feature, const std::string& value)
{
	shared_ptr<Network> network;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, network);
	if (res.IsSuccessful())
	{
		shared_ptr<BaseNode> node;
		res = network->GetBaseNode(nodeId, node);
		if (res.IsSuccessful())
		{
			node->GetNetworkManagement()->SetFeatureUntypedActualValue(feature, value);
		}
	}
	return res;
}

Result OpenConfiguratorCore::CreateParameter(const std::string& networkId, const uint8_t nodeId, const std::string& uniqueID, ParameterAccess access, IEC_Datatype dataType)
{
	shared_ptr<Network> network;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, network);
	if (res.IsSuccessful())
	{
		shared_ptr<BaseNode> node;
		res = network->GetBaseNode(nodeId, node);
		if (res.IsSuccessful())
		{
			auto ptr = make_shared<Parameter>(uniqueID, access, dataType);
			return node->GetApplicationProcess()->AddParameter(ptr);
		}
	}
	return res;
}

Result OpenConfiguratorCore::CreateParameter(const std::string& networkId, const uint8_t nodeId, const std::string& uniqueID, ParameterAccess access)
{
	shared_ptr<Network> network;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, network);
	if (res.IsSuccessful())
	{
		shared_ptr<BaseNode> node;
		res = network->GetBaseNode(nodeId, node);
		if (res.IsSuccessful())
		{
			auto ptr = make_shared<Parameter>(uniqueID, access);
			return node->GetApplicationProcess()->AddParameter(ptr);
		}
	}
	return res;
}

Result OpenConfiguratorCore::CreateStructDatatype(const std::string& networkId, const uint8_t nodeId, const std::string& parameterUniqueId, const std::string& uniqueId, const std::string& name)
{
	shared_ptr<Network> network;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, network);
	if (res.IsSuccessful())
	{
		shared_ptr<BaseNode> node;
		res = network->GetBaseNode(nodeId, node);
		if (res.IsSuccessful())
		{
			auto& parameterList = node->GetApplicationProcess()->GetParameterList();
			for (auto& param : parameterList)
			{
				if (param->GetUniqueID() == parameterUniqueId)
				{
					auto ptr = shared_ptr<ComplexDataType>(new StructDataType(uniqueId, name));
					param->SetComplexDataType(ptr);
					param->SetUniqueIDRef(uniqueId);
					return Result();
				}
			}

			boost::format formatter(kMsgParameterNotFound);
			formatter
			% parameterUniqueId
			% (uint32_t) node->GetNodeIdentifier();
			LOG_FATAL() << formatter.str();
			return Result(ErrorCode::PARAMETER_NOT_FOUND, formatter.str());
		}
	}
	return res;
}

Result OpenConfiguratorCore::CreateVarDeclaration(const std::string& networkId, const uint8_t nodeId, const std::string& structUniqueId, const std::string& uniqueId, const std::string& name, IEC_Datatype datatype, uint32_t size, const std::string& initialValue)
{
	shared_ptr<Network> network;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, network);
	if (res.IsSuccessful())
	{
		shared_ptr<BaseNode> node;
		res = network->GetBaseNode(nodeId, node);
		if (res.IsSuccessful())
		{
			auto& parameterList = node->GetApplicationProcess()->GetParameterList();
			for (auto& param : parameterList)
			{
				if (param->GetUniqueIDRef() == structUniqueId)
				{
					auto ptr = dynamic_pointer_cast<StructDataType>(param->GetComplexDataType());
					auto sharedVar = make_shared<VarDeclaration>(uniqueId, name, datatype, size, initialValue);
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

Result OpenConfiguratorCore::CreateArrayDatatype(const std::string& networkId, const uint8_t nodeId, const std::string& parameterUniqueId, const std::string& uniqueId, const std::string& name, uint32_t lowerLimit, uint32_t upperLimit, IEC_Datatype dataType)
{
	shared_ptr<Network> network;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, network);
	if (res.IsSuccessful())
	{
		shared_ptr<BaseNode> node;
		res = network->GetBaseNode(nodeId, node);
		if (res.IsSuccessful())
		{
			auto& parameterList = node->GetApplicationProcess()->GetParameterList();
			for (auto& param : parameterList)
			{
				if (param->GetUniqueID() == parameterUniqueId)
				{
					auto ptr = shared_ptr<ComplexDataType>(new ArrayDataType(uniqueId, name, lowerLimit, upperLimit, dataType));
					param->SetComplexDataType(ptr);
					param->SetUniqueIDRef(uniqueId);
					return Result();
				}
			}

			boost::format formatter(kMsgParameterNotFound);
			formatter
			% parameterUniqueId
			% (uint32_t) node->GetNodeIdentifier();
			LOG_FATAL() << formatter.str();
			return Result(ErrorCode::PARAMETER_NOT_FOUND, formatter.str());
		}
	}
	return res;
}

Result OpenConfiguratorCore::CreateEnumDatatype(const std::string& networkId, const uint8_t nodeId, const std::string& parameterUniqueId, const std::string& uniqueId, const std::string& name, IEC_Datatype dataType, uint32_t size)
{
	shared_ptr<Network> network;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, network);
	if (res.IsSuccessful())
	{
		shared_ptr<BaseNode> node;
		res = network->GetBaseNode(nodeId, node);
		if (res.IsSuccessful())
		{
			auto& parameterList = node->GetApplicationProcess()->GetParameterList();
			for (auto& param : parameterList)
			{
				if (param->GetUniqueIDRef() == uniqueId)
				{
					auto ptr = shared_ptr<ComplexDataType>(new EnumDataType(uniqueId, name, dataType, size));
					param->SetComplexDataType(ptr);
					return Result();
				}
			}

			boost::format formatter(kMsgParameterNotFound);
			formatter
			% parameterUniqueId
			% (uint32_t) node->GetNodeIdentifier();
			LOG_FATAL() << formatter.str();
			return Result(ErrorCode::PARAMETER_NOT_FOUND, formatter.str());
		}
	}
	return res;
}

Result OpenConfiguratorCore::CreateEnumValue(const std::string& networkId, const uint8_t nodeId, const std::string& uniqueId, const std::string& name, const std::string& value)
{
	shared_ptr<Network> network;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, network);
	if (res.IsSuccessful())
	{
		shared_ptr<BaseNode> node;
		res = network->GetBaseNode(nodeId, node);
		if (res.IsSuccessful())
		{
			auto& parameterList = node->GetApplicationProcess()->GetParameterList();
			for (auto& param : parameterList)
			{
				if (param->GetUniqueIDRef() == uniqueId)
				{
					auto ptr = dynamic_pointer_cast<EnumDataType>(param->GetComplexDataType());
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
	shared_ptr<Network> network;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, network);
	if (res.IsSuccessful())
	{
		shared_ptr<BaseNode> node;
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

			boost::format formatter(kMsgComplexDatatypeNotFound);
			formatter
			% dataTypeUniqueId
			% (uint32_t) node->GetNodeIdentifier();
			LOG_FATAL() << formatter.str();
			return Result(ErrorCode::COMPLEX_DATATYPE_NOT_FOUND, formatter.str());
		}
	}
	return res;
}

Result OpenConfiguratorCore::CreateDynamicChannel(const std::string& networkId, const uint8_t nodeId, PlkDataType dataType, AccessType accessType, uint32_t startIndex, uint32_t endIndex, uint32_t maxNumber, uint32_t addressOffset, uint8_t bitAlignment)
{
	shared_ptr<Network> network;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, network);
	if (res.IsSuccessful())
	{
		shared_ptr<BaseNode> node;
		res = network->GetBaseNode(nodeId, node);
		if (res.IsSuccessful())
		{
			auto mnPtr = dynamic_pointer_cast<ManagingNode>(node);
			auto ptr = shared_ptr<DynamicChannel>(new DynamicChannel(dataType, accessType, startIndex, endIndex, maxNumber, addressOffset, bitAlignment));

			mnPtr->AddDynamicChannel(ptr);
			return Result();
		}
	}
	return res;
}

Result OpenConfiguratorCore::SetActiveManagingNode(const std::string& networkId, const uint8_t nodeID)
{
	//Only redundant managing nodes can be set active
	if (nodeID > 240 && nodeID < 253)
	{
		shared_ptr<Network> network;
		Result res = ProjectManager::GetInstance().GetNetwork(networkId, network);
		if (res.IsSuccessful())
		{
			shared_ptr<BaseNode> node;
			res = network->GetBaseNode(nodeID, node);
			if (res.IsSuccessful())
			{
				auto mnPtr = dynamic_pointer_cast<ManagingNode>(node);
				shared_ptr<ManagingNode> oldMN;
				res = network->GetManagingNode(oldMN);
				if (res.IsSuccessful())
				{
					//Change nodeId from old RMN and set active to false
					oldMN->SetActive(false);
					oldMN->SetNodeIdentifier(nodeID);

					//Set new master managing node
					mnPtr->SetActive(true);
					mnPtr->SetNodeIdentifier(240);
				}
			}
		}
		return res;
	}
	return Result(ErrorCode::NODEID_INVALID);
}

Result OpenConfiguratorCore::SetObjectActualValue(const std::string& networkId, const uint8_t nodeId, uint32_t objectId, const string& actualValue, bool force)
{
	shared_ptr<Network> network;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, network);
	if (res.IsSuccessful())
	{
		shared_ptr<BaseNode> node;
		res = network->GetBaseNode(nodeId, node);
		if (res.IsSuccessful())
		{
			res = node->ForceObject(objectId, force, actualValue);
		}
	}
	return res;
}

Result OpenConfiguratorCore::SetSubObjectActualValue(const std::string& networkId, const uint8_t nodeId, uint32_t objectId, uint32_t subObjectId, const string& actualValue, bool force)
{
	shared_ptr<Network> network;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, network);
	if (res.IsSuccessful())
	{
		shared_ptr<BaseNode> node;
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
	shared_ptr<Network> network;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, network);
	if (!res.IsSuccessful())
		return res;

	return network->SetOperationMode(nodeId, PlkOperationMode::NORMAL);
}

Result OpenConfiguratorCore::SetOperationModeChained(const std::string& networkId, const std::uint8_t nodeId)
{
	shared_ptr<Network> network;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, network);
	if (!res.IsSuccessful())
		return res;

	return network->SetOperationMode(nodeId, PlkOperationMode::CHAINED);
}

Result OpenConfiguratorCore::SetOperationModeMultiplexed(const std::string& networkId, const std::uint8_t nodeId, const std::uint8_t multiplexedCycle)
{
	shared_ptr<Network> network;
	Result res = ProjectManager::GetInstance().GetNetwork(networkId, network);
	if (!res.IsSuccessful())
		return res;

	return network->SetOperationMode(nodeId, PlkOperationMode::MULTIPLEXED, multiplexedCycle);
}