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

using namespace std;
using namespace IndustrialNetwork::POWERLINK::Core::NetworkHandling;
using namespace IndustrialNetwork::POWERLINK::Core::ErrorHandling;
using namespace IndustrialNetwork::POWERLINK::Core::Node;
using namespace IndustrialNetwork::POWERLINK::Core::Configuration;
using namespace IndustrialNetwork::POWERLINK::Core::CoreConfiguration;

Network::Network() :
	networkId(""),
	cycleTime(0),
	asyncMTU(300), //DS 301 V1.2.1
	multiplexedCycleLength(0), //DS 301 V1.2.1
	prescaler(2), //DS 301 V1.2.1
	nodeCollection(map<uint8_t, shared_ptr<BaseNode>>()),
	buildConfigurations(vector<shared_ptr<PlkConfiguration>>()),
	activeConfiguration("")
{}

Network::Network(const string networkId) :
	networkId(networkId),
	cycleTime(0),
	asyncMTU(300), //DS 301 V1.2.1
	multiplexedCycleLength(0), //DS 301 V1.2.1
	prescaler(2), //DS 301 V1.2.1
	nodeCollection(map<uint8_t, shared_ptr<BaseNode>>()),
	buildConfigurations(vector<shared_ptr<PlkConfiguration>>()),
	activeConfiguration("")
{}

Network::~Network()
{}

Result Network::AddNode(shared_ptr<ControlledNode>& node)
{
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
	this->nodeCollection.insert(pair<uint8_t, shared_ptr<BaseNode>>(node->GetNodeIdentifier(), node));

	//Force Node Assignement with actual value "0"
	this->nodeCollection.at(240)->ForceSubObject(0x1F81, node->GetNodeIdentifier(), true, "0");

	return Result();
}

Result Network::AddNode(shared_ptr<ManagingNode>& node)
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
	this->nodeCollection.insert(pair<uint8_t, shared_ptr<BaseNode>>(node->GetNodeIdentifier(), node));
	return Result();
}

Result Network::GetBaseNode(const uint8_t nodeID, shared_ptr<BaseNode>& node)
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

Result Network::GetManagingNode(shared_ptr<ManagingNode>& node)
{
	for (auto& var : this->nodeCollection)
	{
		if (var.first == 240)
		{
			node = dynamic_pointer_cast<ManagingNode>(var.second);
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
	map<uint8_t, shared_ptr<BaseNode>>::iterator it;
	for (it = this->nodeCollection.begin() ; it != this->nodeCollection.end(); ++it)
	{
		if (it->first == nodeID)
			break;
	}
	if (it == this->nodeCollection.end())
	{
		//Node does not exist
		boost::format formatter(kMsgExistingNode);
		formatter
		% (uint32_t) nodeID;
		LOG_FATAL() << formatter.str();
		return Result(ErrorCode::NODE_DOES_NOT_EXIST, formatter.str());
	}

	this->nodeCollection.erase(it);
	//Log info node removed
	boost::format formatter(kMsgNodeRemoved);
	formatter
	% (uint32_t) nodeID;
	LOG_INFO() << formatter.str();

	//Remove CN related MN objects
	shared_ptr<ManagingNode> mn;
	Result res = this->GetManagingNode(mn);
	if (res.IsSuccessful())
	{
		//Reset 0x1F26 / nodeID
		mn->ForceSubObject(0x1F26, nodeID, false, "0");
		//Reset 0x1F27 / nodeID
		mn->ForceSubObject(0x1F27, nodeID, false, "0");
		//Reset 0x1F82 / nodeID
		mn->ForceSubObject(0x1F81, nodeID, false, "0");
	}
	return res;
}

Result Network::GetNodes(map<uint8_t, shared_ptr<BaseNode>>& nodeCollection)
{
	nodeCollection = this->nodeCollection;
	return Result();
}

Result Network::GetAvailableNodeIds(vector<uint8_t>& nodeIdCollection)
{
	for (auto& var : this->nodeCollection)
	{
		nodeIdCollection.push_back(var.first);
	}
	return Result();
}

const string Network::GetNetworkId()
{
	return this->networkId;
}

uint32_t Network::GetCycleTime()
{
	return this->cycleTime;
}

uint32_t Network::GetAsyncMTU()
{
	return this->asyncMTU;
}

uint32_t Network::GetMultiplexedCycleLength()
{
	return this->multiplexedCycleLength;
}

uint32_t Network::GetPrescaler()
{
	return this->prescaler;
}

void Network::SetCycleTime(const uint32_t cycleTime)
{
	stringstream cycleTimeStr;
	this->cycleTime = cycleTime;
	cycleTimeStr << cycleTime;
	this->nodeCollection.at(240)->SetSubObjectActualValue(0x1006, 0x0, cycleTimeStr.str());
}

void Network::SetAsyncMTU(const uint32_t asyncMTU)
{
	this->asyncMTU = asyncMTU;
}

void Network::SetMultiplexedCycleLength(const uint32_t multiCycleLength)
{
	this->multiplexedCycleLength = multiCycleLength;
}

void Network::SetPrescaler(const uint32_t prescaler)
{
	this->prescaler = prescaler;
}

Result Network::SetConfigurationSettingEnabled(const string configID, const string settingName, bool enabled)
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
					//Convert bool to string for boost::format
					stringstream enabledStr;
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


Result Network::AddConfigurationSetting(const string configID, shared_ptr<BuildConfigurationSetting> newSetting)
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

Result Network::RemoveConfigurationSetting(string configID, const string settingName)
{
	for (auto& config : this->buildConfigurations)
	{
		if (config->GetConfigurationName() == configID)
		{
			vector<shared_ptr<BuildConfigurationSetting>>::iterator it;
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

Result Network::AddConfiguration(string configID)
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
	shared_ptr<PlkConfiguration> config = make_shared<PlkConfiguration>(configID);
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

Result Network::RemoveConfiguration(string configID)
{
	if (this->activeConfiguration == configID)
		return Result(ErrorCode::BUILD_CONFIGURATION_IS_ACTIVE);

	vector<shared_ptr<PlkConfiguration>>::iterator it;
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

Result Network::ReplaceConfigurationName(const string oldConfigID, const string newConfigID)
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

Result Network::GetConfigurationSettings(string configID, vector<shared_ptr<BuildConfigurationSetting>>& returnRef)
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

Result Network::GetBuildConfigurations(std::vector<shared_ptr<PlkConfiguration>>& bcfgs)
{
	bcfgs = this->buildConfigurations;
	return Result();
}

const string& Network::GetActiveConfiguration()
{
	return this->activeConfiguration;
}

Result Network::SetActiveConfiguration(const string configID)
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
