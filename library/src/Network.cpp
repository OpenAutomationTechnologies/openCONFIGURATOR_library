/************************************************************************
\file NetworkManagement.cpp
\brief Implementation of the Class NetworkManagement
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
#include "Network.h"


using namespace IndustrialNetwork::POWERLINK::Core::NetworkHandling;
using namespace IndustrialNetwork::POWERLINK::Core::ErrorHandling;
using namespace IndustrialNetwork::POWERLINK::Core::Node;
using namespace IndustrialNetwork::POWERLINK::Core::Configuration;
using namespace IndustrialNetwork::POWERLINK::Core::CoreConfiguration;
using namespace IndustrialNetwork::POWERLINK::Core::Utilities;

Network::Network() :
	networkId(""),
	cycleTime(0),
	asyncMTU(300), //DS 301 V1.2.1
	multiplexedCycleCount(0), //DS 301 V1.2.1
	prescaler(2), //DS 301 V1.2.1
	nodeCollection(std::map<uint8_t, std::shared_ptr<BaseNode>>()),
	buildConfigurations(std::vector<std::shared_ptr<PlkConfiguration>>()),
	activeConfiguration("")
{}

Network::Network(const std::string& networkId) :
	networkId(networkId),
	cycleTime(0),
	asyncMTU(300), //DS 301 V1.2.1
	multiplexedCycleCount(0), //DS 301 V1.2.1
	prescaler(2), //DS 301 V1.2.1
	nodeCollection(std::map<uint8_t, std::shared_ptr<BaseNode>>()),
	buildConfigurations(std::vector<std::shared_ptr<PlkConfiguration>>()),
	activeConfiguration("")
{}

Network::~Network()
{
	this->buildConfigurations.clear();
	this->nodeCollection.clear();
}

Result Network::AddNode(std::shared_ptr<ControlledNode>& node)
{
	//Managing Node has to be added first
	std::shared_ptr<ManagingNode> mn;
	Result res = this->GetManagingNode(mn);
	if (!res.IsSuccessful())
		return res;

	for (auto& var : this->nodeCollection)
	{
		if (var.first == node->GetNodeIdentifier())
		{
			//Node already exists
			boost::format formatter(kMsgExistingNode);
			formatter
			% (uint32_t) var.first;
			LOG_FATAL() << formatter.str();
			return Result(ErrorCode::NODE_EXISTS, formatter.str());
		}
	}

	//Log info node created
	boost::format formatter(kMsgNodeCreated);
	formatter
	% (uint32_t) node->GetNodeIdentifier();
	LOG_INFO() << formatter.str();
	this->nodeCollection.insert(std::pair<uint8_t, std::shared_ptr<BaseNode>>(node->GetNodeIdentifier(), node));

	//Set Node Assignement with actual value "0"
	mn->SetSubObjectActualValue(0x1F81, node->GetNodeIdentifier(), "0");

	return Result();
}

Result Network::AddNode(std::shared_ptr<ManagingNode>& node)
{
	for (auto& var : this->nodeCollection)
	{
		if (var.first == node->GetNodeIdentifier())
		{
			//Node already exists
			boost::format formatter(kMsgExistingNode);
			formatter
			% (uint32_t) node->GetNodeIdentifier();
			LOG_FATAL() << formatter.str();
			return Result(ErrorCode::NODE_EXISTS, formatter.str());
		}
	}
	//Log info node created
	boost::format formatter(kMsgNodeCreated);
	formatter
	% (uint32_t) node->GetNodeIdentifier();
	LOG_INFO() << formatter.str();

	this->nodeCollection.insert(std::pair<uint8_t, std::shared_ptr<BaseNode>>(node->GetNodeIdentifier(), node));

	//if not active MN
	if (node->GetNodeIdentifier() != 240)
	{
		for (auto& rmn : this->nodeCollection)
		{
			std::shared_ptr<ManagingNode> rmnPtr = std::dynamic_pointer_cast<ManagingNode>(rmn.second);
			if (rmnPtr.get())
			{
				//Increment RMN count
				rmnPtr->SetRmnCount(rmnPtr->GetRmnCount() + 1);
			}
		}
	}
	return Result();
}

Result Network::GetBaseNode(const uint8_t nodeID, std::shared_ptr<BaseNode>& node)
{
	for (auto& var : this->nodeCollection)
	{
		if (var.first == nodeID)
		{
			node = var.second;
			return Result();
		}
	}

	//Node does not exist
	boost::format formatter(kMsgExistingNode);
	formatter
	% (uint32_t) nodeID;
	LOG_FATAL() << formatter.str();
	return Result(ErrorCode::NODE_DOES_NOT_EXIST, formatter.str());
}

Result Network::GetManagingNode(std::shared_ptr<ManagingNode>& node)
{
	for (auto& var : this->nodeCollection)
	{
		if (var.first == 240)
		{
			node = std::dynamic_pointer_cast<ManagingNode>(var.second);
			return Result();
		}
	}

	//Node does not exist
	boost::format formatter(kMsgExistingNode);
	formatter
	% 240;
	LOG_FATAL() << formatter.str();
	return Result(ErrorCode::NODE_DOES_NOT_EXIST, formatter.str());
}

Result Network::RemoveNode(const uint8_t nodeID)
{
	if (nodeID == 240)
		return Result(ErrorCode::NODEID_INVALID);

	auto it = this->nodeCollection.find(nodeID);
	if (it == this->nodeCollection.end())
	{
		//Node does not exist
		boost::format formatter(kMsgExistingNode);
		formatter
		% (uint32_t) nodeID;
		LOG_FATAL() << formatter.str();
		return Result(ErrorCode::NODE_DOES_NOT_EXIST, formatter.str());
	}

	if (std::dynamic_pointer_cast<ManagingNode>(it->second))//if RMN is removed
	{
		std::shared_ptr<ManagingNode> mn;
		Result res = this->GetManagingNode(mn);
		if (!res.IsSuccessful())
			return res;
		mn->SetRmnCount(mn->GetRmnCount() - 1); //Decrement the RMN count in MN

	}
	this->nodeCollection.erase(it);
	//Log info node removed
	boost::format formatter(kMsgNodeRemoved);
	formatter
	% (uint32_t) nodeID;
	LOG_INFO() << formatter.str();

	//Remove CN related MN and RMN objects
	for (auto& rmn : this->nodeCollection)
	{
		if (std::dynamic_pointer_cast<ManagingNode>(rmn.second))
		{
			//Reset 0x1F26 / nodeID
			rmn.second->ForceSubObject(0x1F26, nodeID, false, "0");
			//Reset 0x1F27 / nodeID
			rmn.second->ForceSubObject(0x1F27, nodeID, false, "0");
			//Reset 0x1F82 / nodeID
			rmn.second->ForceSubObject(0x1F81, nodeID, false, "0");
			//Reset 0x1F9B / nodeID
			rmn.second->ForceSubObject(0x1F9B, nodeID, false, "0");
			//Reset 0x1F92 / nodeID
			rmn.second->ForceSubObject(0x1F92, nodeID, false, "25000");
			//Reset 0x1F92 / nodeID
			rmn.second->ForceSubObject(0x1F8B, nodeID, false, "36");
			//Reset 0x1F8D / nodeID from all CNs
			rmn.second->ForceSubObject(0x1F8D, nodeID, false, "36");
		}
		else
		{
			//Reset 0x1F8D / nodeID from all CNs
			rmn.second->ForceSubObject(0x1F8D, nodeID, false, "0");
		}
	}
	return Result();
}

Result Network::GetNodes(std::map<uint8_t, std::shared_ptr<BaseNode>>& nodeCollection)
{
	nodeCollection = this->nodeCollection;
	return Result();
}

Result Network::GetAvailableNodeIds(std::vector<uint8_t>& nodeIdCollection)
{
	for (auto& var : this->nodeCollection)
	{
		nodeIdCollection.push_back(var.first);
	}
	return Result();
}

const std::string& Network::GetNetworkId()
{
	return this->networkId;
}

uint32_t Network::GetCycleTime()
{
	return this->cycleTime;
}

uint16_t Network::GetAsyncMTU()
{
	return this->asyncMTU;
}

uint16_t Network::GetMultiplexedCycleCount()
{
	return this->multiplexedCycleCount;
}

uint16_t Network::GetPrescaler()
{
	return this->prescaler;
}

void Network::SetCycleTime(const uint32_t cycleTime)
{
	this->cycleTime = cycleTime;
}

void Network::SetAsyncMTU(const uint16_t asyncMTU)
{
	this->asyncMTU = asyncMTU;
}

void Network::SetMultiplexedCycleCount(const uint16_t multiCycleCount)
{
	this->multiplexedCycleCount  = multiCycleCount;
}

void Network::SetPrescaler(const uint16_t prescaler)
{
	this->prescaler = prescaler;
}

Result Network::SetConfigurationSettingEnabled(const std::string& configID, const std::string& settingName, bool enabled)
{
	for (auto& config : this->buildConfigurations)
	{
		if (config->GetConfigurationName() == configID)
		{
			for (auto& setting : config->GetBuildConfigurationSettings())
			{
				if (setting->GetName() == settingName)
				{
					setting->SetEnabled(enabled);
					//Convert bool to std::string for boost::format
					std::stringstream enabledStr;
					enabledStr << std::boolalpha << enabled;
					//Long info setting enabled
					boost::format formatter(kMsgConfigurationSettingEnabled);
					formatter
					% configID
					% settingName
					% this->networkId
					% enabledStr.str();
					LOG_INFO() << formatter.str();
					return Result();
				}
			}
			//Configuration setting does not exist
			boost::format formatter(kMsgConfigurationSettingNonExisting);
			formatter
			% configID
			% settingName
			% this->networkId;
			LOG_FATAL() << formatter.str();
			return Result(ErrorCode::BUILD_SETTING_DOES_NOT_EXIST, formatter.str());
		}
	}
	//Configuration does not exist
	boost::format formatter(kMsgConfigurationNonExisting);
	formatter
	% configID
	% this->networkId;
	LOG_FATAL() << formatter.str();
	return Result(ErrorCode::BUILD_CONFIGURATION_DOES_NOT_EXIST, formatter.str());
}


Result Network::AddConfigurationSetting(const std::string& configID, std::shared_ptr<BuildConfigurationSetting> newSetting)
{
	for (auto& config : this->buildConfigurations)
	{
		if (config->GetConfigurationName() == configID)
		{
			for (auto& setting : config->GetBuildConfigurationSettings())
			{
				if (setting->GetName() == newSetting->GetName() && setting->GetValue() == newSetting->GetValue())
				{
					//Configuration setting already exist
					boost::format formatter(kMsgConfigurationSettingExisting);
					formatter
					% newSetting->GetName()
					% configID
					% this->networkId;
					LOG_FATAL() << formatter.str();
					return Result(ErrorCode::BUILD_SETTING_EXISTS, formatter.str());
				}
			}
			config->AddBuildConfigurationSetting(newSetting);

			//Log info setting added
			boost::format formatter(kMsgConfigurationSettingAdded);
			formatter
			% newSetting->GetName()
			% configID
			% this->networkId;
			LOG_INFO() << formatter.str();
			return Result();
		}
	}
	//Configuration does not exist
	boost::format formatter(kMsgConfigurationNonExisting);
	formatter
	% configID
	% this->networkId;
	LOG_FATAL() << formatter.str();
	return Result(ErrorCode::BUILD_CONFIGURATION_DOES_NOT_EXIST, formatter.str());
}

Result Network::RemoveConfigurationSetting(const std::string& configID, const std::string& settingName)
{
	for (auto& config : this->buildConfigurations)
	{
		if (config->GetConfigurationName() == configID)
		{
			std::vector<std::shared_ptr<BuildConfigurationSetting>>::iterator it;
			for (it = config->GetBuildConfigurationSettings().begin() ; it != config->GetBuildConfigurationSettings().end(); ++it)
			{
				if (it->get()->GetName() == settingName)
					break;
			}
			if (it == config->GetBuildConfigurationSettings().end())
			{
				//Setting does not exist
				boost::format formatter(kMsgConfigurationSettingNonExisting);
				formatter
				% configID
				% settingName
				% this->networkId;
				LOG_FATAL() << formatter.str();
				return Result(ErrorCode::BUILD_SETTING_DOES_NOT_EXIST, formatter.str());
			}

			config->GetBuildConfigurationSettings().erase(it);
			//Log info setting removed
			boost::format formatter(kMsgConfigurationSettingRemoved);
			formatter
			% configID
			% settingName
			% this->networkId;
			LOG_INFO() << formatter.str();
			return Result();
		}
	}
	//Configuration setting does not exist
	boost::format formatter(kMsgConfigurationNonExisting);
	formatter
	% configID
	% this->networkId;
	LOG_FATAL() << formatter.str();
	return Result(ErrorCode::BUILD_CONFIGURATION_DOES_NOT_EXIST, formatter.str());
}

Result Network::AddConfiguration(const std::string& configID)
{
	for (auto& config : this->buildConfigurations)
	{
		if (config->GetConfigurationName() == configID)
		{
			//Configuration setting does not exist
			boost::format formatter(kMsgConfigurationExisting);
			formatter
			% configID
			% this->networkId;
			LOG_FATAL() << formatter.str();
			return Result(ErrorCode::BUILD_CONFIGURATION_EXISTS, formatter.str());
		}
	}
	std::shared_ptr<PlkConfiguration> config = std::make_shared<PlkConfiguration>(configID);
	this->buildConfigurations.push_back(config);
	//Log info configuration added
	boost::format formatter(kMsgConfigurationAdded);
	formatter
	% configID
	% this->networkId;
	LOG_INFO() << formatter.str();

	//first configuration added is set active one
	if (this->buildConfigurations.size() == 1)
		this->activeConfiguration = configID;

	return Result();
}

Result Network::RemoveConfiguration(const std::string& configID)
{
	if (this->activeConfiguration == configID)
		return Result(ErrorCode::BUILD_CONFIGURATION_IS_ACTIVE);

	std::vector<std::shared_ptr<PlkConfiguration>>::iterator it;
	for (it = this->buildConfigurations.begin() ; it != this->buildConfigurations.end(); ++it)
	{
		if (it->get()->GetConfigurationName() == configID)
			break;
	}
	if (it == this->buildConfigurations.end())
	{
		//Configuration does not exist
		boost::format formatter(kMsgConfigurationNonExisting);
		formatter
		% configID
		% this->networkId;
		LOG_FATAL() << formatter.str();
		return Result(ErrorCode::BUILD_CONFIGURATION_DOES_NOT_EXIST);
	}

	this->buildConfigurations.erase(it);
	//Log info configuration removed
	boost::format formatter(kMsgConfigurationRemoved);
	formatter
	% configID
	% this->networkId;
	LOG_INFO() << formatter.str();
	return Result();
}

Result Network::ReplaceConfigurationName(const std::string& oldConfigID, const std::string& newConfigID)
{
	for (auto& config : this->buildConfigurations)
	{
		if (config->GetConfigurationName() == oldConfigID)
		{
			config->SetConfigurationName(newConfigID);
			//Log info configuration is renamed
			boost::format formatter(kMsgConfigurationRenamed);
			formatter
			% oldConfigID
			% newConfigID
			% this->networkId;
			LOG_INFO() << formatter.str();
			return Result();
		}
	}
	//Configuration does not exist
	boost::format formatter(kMsgConfigurationNonExisting);
	formatter
	% oldConfigID
	% this->networkId;
	LOG_FATAL() << formatter.str();
	return Result(ErrorCode::BUILD_CONFIGURATION_DOES_NOT_EXIST);
}

Result Network::GetConfigurationSettings(const std::string& configID, std::vector<std::shared_ptr<BuildConfigurationSetting>>& returnRef)
{
	for (auto& config : this->buildConfigurations)
	{
		if (config->GetConfigurationName() == configID)
		{
			returnRef = config->GetBuildConfigurationSettings();
			return Result();
		}
	}
	//Configuration does not exist
	boost::format formatter(kMsgConfigurationNonExisting);
	formatter
	% configID
	% this->networkId;
	LOG_FATAL() << formatter.str();
	return Result(ErrorCode::BUILD_CONFIGURATION_DOES_NOT_EXIST);
}

Result Network::GetBuildConfigurations(std::vector<std::shared_ptr<PlkConfiguration>>& bcfgs)
{
	bcfgs = this->buildConfigurations;
	return Result();
}

const std::string& Network::GetActiveConfiguration()
{
	return this->activeConfiguration;
}

Result Network::SetActiveConfiguration(const std::string& configID)
{
	for (auto& config : this->buildConfigurations)
	{
		if (config->GetConfigurationName() == configID)
		{
			this->activeConfiguration = configID;
			//Log info configuration is active
			boost::format formatter(kMsgConfigurationActive);
			formatter
			% configID
			% this->networkId;
			LOG_INFO() << formatter.str();
			return Result();
		}
	}
	//Configuration setting does not exist
	boost::format formatter(kMsgConfigurationNonExisting);
	formatter
	% configID
	% this->networkId;
	LOG_FATAL() << formatter.str();
	return Result(ErrorCode::BUILD_CONFIGURATION_DOES_NOT_EXIST);
}

Result Network::GenerateConfiguration()
{
	Result res;
	for (auto& config : this->buildConfigurations)
	{
		if (config->GetConfigurationName() == this->GetActiveConfiguration())
		{
			res = config->GenerateConfiguration(this->nodeCollection);
		}
	}
	return res;
}

Result Network::SetOperationMode(const std::uint8_t nodeID, const PlkOperationMode mode, const std::uint8_t multiplexedCycle)
{
	std::shared_ptr<BaseNode> node;
	Result res = this->GetBaseNode(nodeID, node);
	if (!res.IsSuccessful())
		return res;

	std::shared_ptr<ControlledNode> cn = std::dynamic_pointer_cast<ControlledNode>(node);
	if (cn)
	{
		std::shared_ptr<ManagingNode> mn;
		res = this->GetManagingNode(mn);
		if (!res.IsSuccessful())
			return res;

		bool operationModeSupported = false;
		if (mode == PlkOperationMode::CHAINED)
		{
			res = mn->GetNetworkManagement()->GetFeatureActualValue<bool>(MNFeatureEnum::DLLMNPResChaining, operationModeSupported);
			if (!res.IsSuccessful())
				return res;
			if (!operationModeSupported)
				return Result(ErrorCode::CHAINING_NOT_SUPPORTED);
		}
		else if (mode == PlkOperationMode::MULTIPLEXED)
		{
			res = mn->GetNetworkManagement()->GetFeatureActualValue<bool>(MNFeatureEnum::DLLMNFeatureMultiplex, operationModeSupported);
			if (!res.IsSuccessful())
				return res;
			if (!operationModeSupported)
				return Result(ErrorCode::MULTIPLEXING_NOT_SUPPORTED);
		}

		if (cn->GetOperationMode() == mode)
			return Result();

		res = cn->SetOperationMode(mode);
		if (!res.IsSuccessful())
			return res;


		switch (mode)
		{
			case PlkOperationMode::NORMAL:
				{
					res = mn->ResetMultiplexedCycle(nodeID);
					break;
				}
			case PlkOperationMode::MULTIPLEXED:
				{
					if (multiplexedCycle != 0)
					{
						res = mn->SetMultiplexedCycle(nodeID, multiplexedCycle);
					}
					break;
				}
			case PlkOperationMode::CHAINED:
				{
					mn->AddNodeAssignement(NodeAssignment::NMT_NODEASSIGN_MN_PRES);
					uint32_t mnAssignValue = mn->GetNodeAssignmentValue();
					res = cn->SetSubObjectActualValue(0x1F81, 240, IntToHex<unsigned int>(mnAssignValue, 0, "0x"));
					if (!res.IsSuccessful())
						return res;
					res = mn->SetSubObjectActualValue(0x1F81, 240, IntToHex<unsigned int>(mnAssignValue, 0, "0x"));
					if (!res.IsSuccessful())
						return res;
					res = mn->ResetMultiplexedCycle(nodeID);
					break;
				}
			default:
				break;
		}

	}
	else
		return Result(ErrorCode::NODE_CONFIGURATION_ERROR);

	return Result();
}
