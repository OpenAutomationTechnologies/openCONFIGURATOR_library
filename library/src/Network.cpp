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
using namespace IndustrialNetwork::POWERLINK::Core::ObjectDictionary;
using namespace IndustrialNetwork::POWERLINK::Core::ModularNode;

Network::Network() :
	networkId(""),
	cycleTime(0),
	asyncMTU(300), //DS 301 V1.2.1
	multiplexedCycleCount(0), //DS 301 V1.2.1
	prescaler(2), //DS 301 V1.2.1
	lossOfSoCTolerance(100000),
	nodeCollection(std::map<std::uint8_t, std::shared_ptr<BaseNode>>()),
	buildConfigurations(std::vector<std::shared_ptr<PlkConfiguration>>()),
	activeConfiguration("")
{}

Network::Network(const std::string& networkId) :
	networkId(networkId),
	cycleTime(0),
	asyncMTU(300), //DS 301 V1.2.1
	multiplexedCycleCount(0), //DS 301 V1.2.1
	prescaler(2), //DS 301 V1.2.1
	lossOfSoCTolerance(100000),
	nodeCollection(std::map<uint8_t, std::shared_ptr<BaseNode>>()),
	buildConfigurations(std::vector<std::shared_ptr<PlkConfiguration>>()),
	activeConfiguration("")
{}

Network::~Network()
{
	this->buildConfigurations.clear();
	this->nodeCollection.clear();
}

Result Network::AddNode(std::shared_ptr<ModularControlledNode>& node)
{
	//Managing Node has to be added first
	std::shared_ptr<ManagingNode> mn;
	Result res = this->GetManagingNode(mn);
	if (!res.IsSuccessful())
		return res;

	for (auto& var : this->nodeCollection)
	{
		if (var.first == node->GetNodeId())
		{
			//Node already exists
			boost::format formatter(kMsgExistingNode);
			formatter
			% (std::uint32_t) var.first;
			LOG_ERROR() << "[" + networkId + "] " + formatter.str();
			return Result(ErrorCode::NODE_EXISTS, formatter.str());
		}
	}

	//Log info node created
	boost::format formatter(kMsgNodeCreated);
	formatter
	% (std::uint32_t) node->GetNodeId();
	LOG_INFO() << "[" + networkId + "] " + formatter.str();
	this->nodeCollection.insert(std::pair<std::uint8_t, std::shared_ptr<BaseNode>>(node->GetNodeId(), node));

	//Set Node Assignement with actual value "0"
	mn->SetSubObjectActualValue(0x1F81, node->GetNodeId(), "0");

	return Result();
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
		if (var.first == node->GetNodeId())
		{
			//Node already exists
			boost::format formatter(kMsgExistingNode);
			formatter
			% (std::uint32_t) var.first;
			LOG_ERROR() << "[" + networkId + "] " + formatter.str();
			return Result(ErrorCode::NODE_EXISTS, formatter.str());
		}
	}

	//Log info node created
	boost::format formatter(kMsgNodeCreated);
	formatter
	% (std::uint32_t) node->GetNodeId();
	LOG_INFO() << "[" + networkId + "] " + formatter.str();
	this->nodeCollection.insert(std::pair<std::uint8_t, std::shared_ptr<BaseNode>>(node->GetNodeId(), node));

	//Set Node Assignement with actual value "0"
	mn->SetSubObjectActualValue(0x1F81, node->GetNodeId(), "0");

	return Result();
}

Result Network::AddNode(std::shared_ptr<ManagingNode>& node)
{
	for (auto& var : this->nodeCollection)
	{
		if (var.first == node->GetNodeId())
		{
			//Node already exists
			boost::format formatter(kMsgExistingNode);
			formatter
			% (std::uint32_t) node->GetNodeId();
			LOG_ERROR() << "[" + networkId + "] " + formatter.str();
			return Result(ErrorCode::NODE_EXISTS, formatter.str());
		}
	}
	//Log info node created
	boost::format formatter(kMsgNodeCreated);
	formatter
	% (std::uint32_t) node->GetNodeId();
	LOG_INFO() << "[" + networkId + "] " + formatter.str();

	this->nodeCollection.insert(std::pair<std::uint8_t, std::shared_ptr<BaseNode>>(node->GetNodeId(), node));

	//if not active MN
	if (node->GetNodeId() != 240)
	{
		for (auto& rmn : this->nodeCollection)
		{
			std::shared_ptr<ManagingNode> rmnPtr = std::dynamic_pointer_cast<ManagingNode>(rmn.second);
			if (rmnPtr.get())
			{
				//Add RMN id
				rmnPtr->AddRmnId(node->GetNodeId());
			}
		}
	}
	return Result();
}

Result Network::GetBaseNode(const std::uint8_t nodeID, std::shared_ptr<BaseNode>& node)
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
	boost::format formatter(kMsgNonExistingNode);
	formatter
	% (std::uint32_t) nodeID;
	LOG_ERROR() << "[" + networkId + "] " + formatter.str();
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
	boost::format formatter(kMsgNonExistingNode);
	formatter
	% 240;
	LOG_ERROR() << "[" + networkId + "] " + formatter.str();
	return Result(ErrorCode::NODE_DOES_NOT_EXIST, formatter.str());
}

Result Network::RemoveNode(const std::uint8_t nodeID)
{
	if (nodeID == 240)
		return Result(ErrorCode::NODEID_INVALID);

	auto it = this->nodeCollection.find(nodeID);
	if (it == this->nodeCollection.end())
	{
		//Node does not exist
		boost::format formatter(kMsgNonExistingNode);
		formatter
		% (std::uint32_t) nodeID;
		LOG_ERROR() << "[" + networkId + "] " + formatter.str();
		return Result(ErrorCode::NODE_DOES_NOT_EXIST, formatter.str());
	}

	if (std::dynamic_pointer_cast<ManagingNode>(it->second))//if RMN is removed
	{
		std::shared_ptr<ManagingNode> mn;
		Result res = this->GetManagingNode(mn);
		if (!res.IsSuccessful())
			return res;
		mn->RemoveRmnId((std::uint16_t) it->second->GetNodeId()); //Decrement the RMN count in MN

	}
	this->nodeCollection.erase(it);

	//Log info node removed
	boost::format formatter(kMsgNodeRemoved);
	formatter
	% (std::uint32_t) nodeID;
	LOG_INFO() << "[" + networkId + "] " + formatter.str();

	//Remove CN related MN and RMN objects
	for (auto& node : this->nodeCollection)
	{
		if (std::dynamic_pointer_cast<ManagingNode>(node.second))
		{
			//Reset 0x1F26 / nodeID
			node.second->ForceSubObject(0x1F26, nodeID, false);
			//Reset 0x1F27 / nodeID
			node.second->ForceSubObject(0x1F27, nodeID, false);
			//Reset 0x1F82 / nodeID
			node.second->ForceSubObject(0x1F81, nodeID, false);
			//Reset 0x1F9B / nodeID
			node.second->ForceSubObject(0x1F9B, nodeID, false, false, "", false);
			//Reset 0x1F92 / nodeID
			node.second->ForceSubObject(0x1F92, nodeID, false);
			//Reset 0x1F8B / nodeID
			node.second->ForceSubObject(0x1F8B, nodeID, false);
			//Reset 0x1C09 / nodeID
			node.second->ForceSubObject(0x1C09, nodeID, false);
			//Reset 0x1F8D / nodeID from all CNs
			node.second->ForceSubObject(0x1F8D, nodeID, false);
		}
		else
		{
			//Reset 0x1F8D / nodeID from all CNs
			node.second->ForceSubObject(0x1F8D, nodeID, false, false, "", false);
			//Reset 0x1F8B / nodeID
			node.second->ForceSubObject(0x1F8B, nodeID, false, false, "", false);
			//Reset 0x1F9B / nodeID from all CNs
			node.second->ForceSubObject(0x1F9B, nodeID, false, false, "", false);
		}

		node.second->ClearMappingChannelforNode(nodeID);
	}
	return Result();
}

Result Network::SetNodeId(const std::uint8_t nodeId, const std::uint8_t newNodeId)
{
	if (nodeId == 240)
		return Result(ErrorCode::NODEID_INVALID);

	auto it = this->nodeCollection.find(nodeId);
	if (it == this->nodeCollection.end())
	{
		//Node does not exist
		boost::format formatter(kMsgNonExistingNode);
		formatter
		% (std::uint32_t) nodeId;
		LOG_ERROR() << "[" + networkId + "] " + formatter.str();
		return Result(ErrorCode::NODE_DOES_NOT_EXIST, formatter.str());
	}

	auto ite = this->nodeCollection.find(newNodeId);
	if (ite != this->nodeCollection.end())
	{
		//Node does not exist
		boost::format formatter(kMsgExistingNode);
		formatter
		% (std::uint32_t) newNodeId;
		LOG_ERROR() << "[" + networkId + "] " + formatter.str();
		return Result(ErrorCode::NODE_EXISTS, formatter.str());
	}

	if (std::dynamic_pointer_cast<ManagingNode>(it->second))//if RMN nodeId is changed
	{
		std::shared_ptr<ManagingNode> mn;
		Result res = this->GetManagingNode(mn);
		if (!res.IsSuccessful())
			return res;
		mn->RemoveRmnId((std::uint16_t) nodeId); //Remove the old RMN Id from MN
		mn->AddRmnId((std::uint16_t) newNodeId); //Add the new RMN id to MN
	}

	it->second->SetNodeId(newNodeId);
	this->nodeCollection.insert(std::pair<std::uint8_t, std::shared_ptr<BaseNode>>(newNodeId, it->second));
	this->nodeCollection.erase(nodeId);

	//Remove CN related MN and RMN objects
	for (auto& node : this->nodeCollection)
	{
		if ((std::uint32_t)node.first == newNodeId)
			continue;

		if (std::dynamic_pointer_cast<ManagingNode>(node.second))
		{
			if ((std::uint32_t)node.first == 240)
			{
				std::shared_ptr<SubObject> subObj;
				Result res = node.second->GetSubObject(0x1F92, (std::uint32_t) nodeId, subObj);
				if (!res.IsSuccessful())
					return res;
				if (subObj->HasActualValue())
				{
					res = node.second->SetSubObjectActualValue(0x1F92, (std::uint32_t) newNodeId, "0x" + subObj->GetTypedActualValue<std::string>());
					if (!res.IsSuccessful())
						return res;
				}
			}
			//Reset 0x1F26 / nodeID
			node.second->ForceSubObject(0x1F26, nodeId, false, false, "");
			//Reset 0x1F27 / nodeID
			node.second->ForceSubObject(0x1F27, nodeId, false, false, "");
			//Reset 0x1F81 / nodeID
			node.second->ForceSubObject(0x1F81, nodeId, false, false, "");
			//Reset 0x1F9B / nodeID
			node.second->ForceSubObject(0x1F9B, nodeId, false, false, "", false);
			//Reset 0x1F92 / nodeID
			node.second->ForceSubObject(0x1F92, nodeId, false, false, "");
			//Reset 0x1F8B / nodeID
			node.second->ForceSubObject(0x1F8B, nodeId, false, false, "");
			//Reset 0x1C09 / nodeID
			node.second->ForceSubObject(0x1C09, nodeId, false, false, "");
			//Reset 0x1F8D / nodeID from all CNs
			node.second->ForceSubObject(0x1F8D, nodeId, false, false, "");
		}
		else
		{
			//Reset 0x1F8D / nodeID from all CNs
			node.second->ForceSubObject(0x1F8D, nodeId, false, false, "", false);
			//Reset 0x1F8B / nodeID
			node.second->ForceSubObject(0x1F8B, nodeId, false, false, "", false);
			//Reset 0x1F9B / nodeID from all CNs
			node.second->ForceSubObject(0x1F9B, nodeId, false, false, "", false);
		}

		node.second->ChangeMappingChannelNodeId(nodeId, newNodeId);
	}

	return Result();
}

Result Network::GetNodes(std::map<std::uint8_t, std::shared_ptr<BaseNode>>& nodeCollection)
{
	nodeCollection = this->nodeCollection;
	return Result();
}

Result Network::GetAvailableNodeIds(std::vector<std::uint8_t>& nodeIdCollection)
{
	for (auto& var : this->nodeCollection)
	{
		nodeIdCollection.push_back(var.first);
	}
	return Result();
}

const std::string& Network::GetNetworkId() const
{
	return this->networkId;
}

std::uint32_t Network::GetCycleTime() const
{
	return this->cycleTime;
}

std::uint16_t Network::GetAsyncMTU() const
{
	return this->asyncMTU;
}

std::uint16_t Network::GetMultiplexedCycleCount() const
{
	return this->multiplexedCycleCount;
}

std::uint16_t Network::GetPrescaler() const
{
	return this->prescaler;
}

std::uint32_t Network::GetLossOfSoCTolerance() const
{
	return this->lossOfSoCTolerance;
}

Result Network::SetCycleTime(const std::uint32_t cycleTime)
{
	Result res = CheckCycleTime(cycleTime);
	if (!res.IsSuccessful())
		return res;

	this->cycleTime = cycleTime;
	return res;
}

void Network::SetAsyncMTU(const std::uint16_t asyncMTU)
{
	this->asyncMTU = asyncMTU;
}

void Network::SetMultiplexedCycleCount(const std::uint16_t multiCycleCount)
{
	this->multiplexedCycleCount = multiCycleCount;
}

void Network::SetPrescaler(const std::uint16_t prescaler)
{
	this->prescaler = prescaler;
}

Result Network::SetLossOfSoCTolerance(std::uint32_t lossOfSocTolerance)
{
	for (auto& node : this->nodeCollection)
	{
		std::shared_ptr<Object> obj;
		Result res = node.second->GetObject(0x1C14, obj, false);
		if (!res.IsSuccessful())
			continue;

		if (obj->GetForceToCDC())
			continue;

		res = node.second->ForceObject(0x1C14, false, false, IntToHex(lossOfSocTolerance, 8, "0x"));
		if (!res.IsSuccessful())
			return res;
	}
	this->lossOfSoCTolerance = lossOfSocTolerance;
	return Result();
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
					LOG_INFO() << "[" + networkId + "] " + formatter.str();
					return Result();
				}
			}
			//Configuration setting does not exist
			boost::format formatter(kMsgConfigurationSettingNonExisting);
			formatter
			% configID
			% settingName
			% this->networkId;
			LOG_ERROR() << "[" + networkId + "] " + formatter.str();
			return Result(ErrorCode::BUILD_SETTING_DOES_NOT_EXIST, formatter.str());
		}
	}
	//Configuration does not exist
	boost::format formatter(kMsgConfigurationNonExisting);
	formatter
	% configID
	% this->networkId;
	LOG_ERROR() << "[" + networkId + "] " + formatter.str();
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
					LOG_ERROR() << "[" + networkId + "] " + formatter.str();
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
			LOG_INFO() << "[" + networkId + "] " + formatter.str();
			return Result();
		}
	}
	//Configuration does not exist
	boost::format formatter(kMsgConfigurationNonExisting);
	formatter
	% configID
	% this->networkId;
	LOG_ERROR() << "[" + networkId + "] " + formatter.str();
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
				LOG_ERROR() << "[" + networkId + "] " + formatter.str();
				return Result(ErrorCode::BUILD_SETTING_DOES_NOT_EXIST, formatter.str());
			}

			config->GetBuildConfigurationSettings().erase(it);
			//Log info setting removed
			boost::format formatter(kMsgConfigurationSettingRemoved);
			formatter
			% configID
			% settingName
			% this->networkId;
			LOG_INFO() << "[" + networkId + "] " + formatter.str();
			return Result();
		}
	}
	//Configuration setting does not exist
	boost::format formatter(kMsgConfigurationNonExisting);
	formatter
	% configID
	% this->networkId;
	LOG_ERROR() << "[" + networkId + "] " + formatter.str();
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
			LOG_ERROR() << "[" + networkId + "] " + formatter.str();
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
	LOG_INFO() << "[" + networkId + "] " + formatter.str();

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
		LOG_ERROR() << "[" + networkId + "] " + formatter.str();
		return Result(ErrorCode::BUILD_CONFIGURATION_DOES_NOT_EXIST);
	}

	this->buildConfigurations.erase(it);
	//Log info configuration removed
	boost::format formatter(kMsgConfigurationRemoved);
	formatter
	% configID
	% this->networkId;
	LOG_INFO() << "[" + networkId + "] " + formatter.str();
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
			LOG_INFO() << "[" + networkId + "] " + formatter.str();
			return Result();
		}
	}
	//Configuration does not exist
	boost::format formatter(kMsgConfigurationNonExisting);
	formatter
	% oldConfigID
	% this->networkId;
	LOG_ERROR() << "[" + networkId + "] " + formatter.str();
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
	LOG_ERROR() << "[" + networkId + "] " + formatter.str();
	return Result(ErrorCode::BUILD_CONFIGURATION_DOES_NOT_EXIST);
}

Result Network::GetBuildConfigurations(std::vector<std::shared_ptr<PlkConfiguration>>& bcfgs)
{
	bcfgs = this->buildConfigurations;
	return Result();
}

const std::string& Network::GetActiveConfiguration() const
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
			LOG_INFO() << "[" + networkId + "] " + formatter.str();
			return Result();
		}
	}
	//Configuration setting does not exist
	boost::format formatter(kMsgConfigurationNonExisting);
	formatter
	% configID
	% this->networkId;
	LOG_ERROR() << "[" + networkId + "] " + formatter.str();
	return Result(ErrorCode::BUILD_CONFIGURATION_DOES_NOT_EXIST);
}

Result Network::GenerateConfiguration()
{
	std::shared_ptr<ManagingNode> mn;
	Result res = this->GetManagingNode(mn);
	if (!res.IsSuccessful())
		return res;

	std::shared_ptr<Object> obj;
	res = mn->GetObject(0x1006, obj);
	if (!res.IsSuccessful())
		return res;

	std::uint32_t currCycleTime = 0;
	//Get MN Cycle Time
	if (obj->HasActualValue())
		currCycleTime = obj->GetTypedActualValue<std::uint32_t>();
	else
		return Result(ErrorCode::CYCLE_TIME_NOT_SET, kMsgCycleTimeOnMnNotSet);

	//Check CycleTime
	res = CheckCycleTime(currCycleTime);
	if (!res.IsSuccessful())
		return res;

	if (this->cycleTime != currCycleTime)
		this->cycleTime = currCycleTime;

	//Get LossOfSoCTolerance if existing in the MN and not equal to project LossOfSoCTolerance
	std::uint32_t mnLossOfSoCTolerance = 0;
	res = mn->GetObject(0x1C14, obj, false);
	if (res.IsSuccessful())
	{
		if (obj->HasActualValue())
		{
			mnLossOfSoCTolerance = obj->GetTypedActualValue<std::uint32_t>();
			if (this->lossOfSoCTolerance != mnLossOfSoCTolerance)
				this->lossOfSoCTolerance = mnLossOfSoCTolerance;
		}
	}

	//Distribute LossOfSoCTolerance
	res = this->SetLossOfSoCTolerance(this->lossOfSoCTolerance);
	if (!res.IsSuccessful())
		return res;

	//Build will fail for RMN and chained nodes
	if (mn->GetRmnCount() > 0)
	{
		for (auto& node : this->nodeCollection)
		{
			std::shared_ptr<ControlledNode> cn = std::dynamic_pointer_cast<ControlledNode>(node.second);
			if (cn)
			{
				if (cn->GetOperationMode() == PlkOperationMode::CHAINED)
					return Result(ErrorCode::CHAINING_NOT_SUPPORTED, kMsgChainingRmnNotSupported);
			}
		}
	}

	for (auto& node : this->nodeCollection)
	{
		std::shared_ptr<ManagingNode> mn = std::dynamic_pointer_cast<ManagingNode>(node.second);
		std::shared_ptr<ControlledNode> cn = std::dynamic_pointer_cast<ControlledNode>(node.second);
		if (mn.get() && node.first == 240) //only managing node
		{
			mn->UpdateProcessImage(this->nodeCollection, Direction::RX);
			mn->UpdateProcessImage(this->nodeCollection, Direction::TX);
		}
		else if (cn.get())
		{
			res = cn->UpdateProcessImage(Direction::RX);
			if (!res.IsSuccessful())
				return res;
			res = cn->UpdateProcessImage(Direction::TX);
			if (!res.IsSuccessful())
				return res;
		}
	}

	for (auto& config : this->buildConfigurations)
	{
		if (config->GetConfigurationName() == this->GetActiveConfiguration())
		{
			res = config->GenerateConfiguration(this->nodeCollection);
		}
	}
	return res;
}

Result Network::SetOperationMode(const std::uint8_t nodeID, const PlkOperationMode& mode, const std::uint8_t multiplexedCycle)
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

		//default value is always false
		bool operationModeSupported = false;
		if (mode == PlkOperationMode::CHAINED)
		{
			mn->GetNetworkManagement()->GetFeatureActualValue<bool>(MNFeatureEnum::DLLMNPResChaining, operationModeSupported);
			if (!operationModeSupported)
				return Result(ErrorCode::CHAINING_NOT_SUPPORTED);

			if (mn->GetRmnCount() > 0)
				return Result(ErrorCode::CHAINING_NOT_SUPPORTED, kMsgChainingRmnNotSupported);

		}
		else if (mode == PlkOperationMode::MULTIPLEXED)
		{
			mn->GetNetworkManagement()->GetFeatureActualValue<bool>(MNFeatureEnum::DLLMNFeatureMultiplex, operationModeSupported);
			if (!operationModeSupported)
				return Result(ErrorCode::MULTIPLEXING_NOT_SUPPORTED);
		}

		res = cn->SetOperationMode(mode);
		if (!res.IsSuccessful())
			return res;

		switch (mode)
		{
			case PlkOperationMode::NORMAL:
				{
					//Check if multiplexing supported
					bool featureMultiplex = false;
					mn->GetNetworkManagement()->GetFeatureActualValue(MNFeatureEnum::DLLMNFeatureMultiplex, featureMultiplex);
					if (featureMultiplex)
					{
						res = mn->ResetMultiplexedCycle(nodeID);
						if (!res.IsSuccessful())
							return res;
					}
					//Clear MN node assignment on CN
					std::shared_ptr<SubObject> mnNodeAssignment;
					res = cn->GetSubObject(0x1F81, 240, mnNodeAssignment);
					if (!res.IsSuccessful())
						return res;
					mnNodeAssignment->ClearActualValue();

					break;
				}
			case PlkOperationMode::MULTIPLEXED:
				{
					if (multiplexedCycle != 0)
					{
						res = mn->SetMultiplexedCycle(nodeID, multiplexedCycle);
						if (!res.IsSuccessful())
							return res;

						//Clear MN node assignment on CN
						std::shared_ptr<SubObject> mnNodeAssignment;
						res = cn->GetSubObject(0x1F81, 240, mnNodeAssignment);
						if (!res.IsSuccessful())
							return res;
						mnNodeAssignment->ClearActualValue();
					}
					break;
				}
			case PlkOperationMode::CHAINED:
				{
					//Add PresMN on managing node
					mn->AddNodeAssignment(NodeAssignment::NMT_NODEASSIGN_MN_PRES);

					//Check if multiplexing supported
					bool featureMultiplex = false;
					mn->GetNetworkManagement()->GetFeatureActualValue(MNFeatureEnum::DLLMNFeatureMultiplex, featureMultiplex);
					if (featureMultiplex)
						res = mn->ResetMultiplexedCycle(nodeID);

					break;
				}
			default:
				break;
		}
	}
	else
		return Result(ErrorCode::NODE_CONFIGURATION_ERROR);

	return res;
}

Result Network::EnableNode(const std::uint8_t nodeID, bool enable)
{
	if (nodeID == 240)
	{
		if (enable == false)
		{
			boost::format formatter(kMsgManagingNodeDisable);
			formatter
			% this->networkId;
			LOG_ERROR() << "[" + networkId + "] " + formatter.str();
			return Result(ErrorCode::MANAGING_NODE_DISABLE_INVALID, formatter.str());
		}
		else
			return Result();
	}

	auto it = this->nodeCollection.find(nodeID);
	if (it == this->nodeCollection.end())
	{
		//Node does not exist
		boost::format formatter(kMsgNonExistingNode);
		formatter
		% (std::uint32_t) nodeID;
		LOG_ERROR() << "[" + networkId + "] " + formatter.str();
		return Result(ErrorCode::NODE_DOES_NOT_EXIST, formatter.str());
	}

	if (enable)
	{
		it->second->SetEnabled(true);
		if (std::dynamic_pointer_cast<ManagingNode>(it->second))//if RMN is enabled
		{
			std::shared_ptr<ManagingNode> mn;
			Result res = this->GetManagingNode(mn);
			if (!res.IsSuccessful())
				return res;
			mn->AddRmnId(it->second->GetNodeId()); //Add the RMN Id in MN
		}
	}
	else
	{
		it->second->SetEnabled(false);
		if (std::dynamic_pointer_cast<ManagingNode>(it->second))//if RMN is disabled
		{
			std::shared_ptr<ManagingNode> mn;
			Result res = this->GetManagingNode(mn);
			if (!res.IsSuccessful())
				return res;
			mn->RemoveRmnId(it->second->GetNodeId()); //Remove the RMN Id in MN
		}

		//Remove CN related MN and RMN objects
		for (auto& node : this->nodeCollection)
		{
			if (std::dynamic_pointer_cast<ManagingNode>(node.second))
			{
				//Reset 0x1F26 / nodeID
				node.second->ForceSubObject(0x1F26, nodeID, false, false, "");
				//Reset 0x1F27 / nodeID
				node.second->ForceSubObject(0x1F27, nodeID, false, false, "");
				//Reset 0x1F82 / nodeID
				node.second->ForceSubObject(0x1F81, nodeID, false, false, "");
				//Reset 0x1F9B / nodeID
				node.second->ForceSubObject(0x1F9B, nodeID, false, false, "", false);
				//Reset 0x1F92 / nodeID
				node.second->ForceSubObject(0x1F92, nodeID, false, false, "");
				//Reset 0x1F8B / nodeID
				node.second->ForceSubObject(0x1F8B, nodeID, false, false, "");
				//Reset 0x1C09 / nodeID
				node.second->ForceSubObject(0x1C09, nodeID, false, false, "");
				//Reset 0x1F8D / nodeID from all CNs
				node.second->ForceSubObject(0x1F8D, nodeID, false, false, "");
			}
			else
			{
				//Reset 0x1F8D / nodeID from all CNs
				node.second->ForceSubObject(0x1F8D, nodeID, false, false, "", false);
				//Reset 0x1F8B / nodeID
				node.second->ForceSubObject(0x1F8B, nodeID, false, false,  "", false);
				//Reset 0x1F9B / nodeID from all CNs
				node.second->ForceSubObject(0x1F9B, nodeID, false, false,  "", false);
			}

			node.second->ClearMappingChannelforNode(nodeID);
		}
	}
	return Result();
}

bool Network::HasControlledNodes() const
{
	if (this->nodeCollection.empty())
		return false;

	for (auto& node : this->nodeCollection)
	{
		if (std::dynamic_pointer_cast<ControlledNode>(node.second) && node.second->IsEnabled() != false)
			return true;
	}
	return false;
}

IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result Network::CheckCycleTime(const std::uint32_t cycleTime)
{
	std::uint32_t minCycleTimeCN = 0;
	std::uint32_t maxCycleTimeCN = 0;
	std::uint32_t currentCycleTimeGranularity = 0;

	for (auto& node : this->nodeCollection)
	{
		//Optional feature defaults to '1'
		Result res = node.second->GetNetworkManagement()->GetFeatureActualValue<std::uint32_t>(GeneralFeatureEnum::NMTCycleTimeGranularity, currentCycleTimeGranularity);
		if (res.IsSuccessful())
		{
			if (cycleTime % currentCycleTimeGranularity != 0)
			{
				boost::format formatter(kMsgCycleTimeGran);
				formatter
				% cycleTime
				% currentCycleTimeGranularity
				% node.second->GetName()
				% (std::uint32_t) node.first;
				LOG_ERROR() << "[" + networkId + "] " + formatter.str();
				return Result(ErrorCode::CYCLE_TIME_ERROR, formatter.str());
			}
		}

		//Mandatory feature has to have an actual value
		res = node.second->GetNetworkManagement()->GetFeatureActualValue<std::uint32_t>(GeneralFeatureEnum::NMTCycleTimeMin, minCycleTimeCN);
		if (!res.IsSuccessful())
			return res;

		if (cycleTime < minCycleTimeCN)
		{
			boost::format formatter(kMsgCycleTimeMin);
			formatter
			% cycleTime
			% minCycleTimeCN
			% node.second->GetName()
			% (std::uint32_t) node.first;
			LOG_ERROR() << "[" + networkId + "] " + formatter.str();
			return Result(ErrorCode::CYCLE_TIME_ERROR, formatter.str());
		}

		//Mandatory feature has to have an actual value
		res = node.second->GetNetworkManagement()->GetFeatureActualValue<std::uint32_t>(GeneralFeatureEnum::NMTCycleTimeMax, maxCycleTimeCN);
		if (!res.IsSuccessful())
			return res;

		if (cycleTime > maxCycleTimeCN)
		{
			boost::format formatter(kMsgCycleTimeMax);
			formatter
			% cycleTime
			% maxCycleTimeCN
			% node.second->GetName()
			% (std::uint32_t) node.first;
			LOG_ERROR() << "[" + networkId + "] " + formatter.str();
			return Result(ErrorCode::CYCLE_TIME_ERROR, formatter.str());
		}

	}
	return Result();
}
