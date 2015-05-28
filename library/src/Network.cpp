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

Network::Network() :
	networkId(""),
	cycleTime(0),
	asyncMTU(300), //DS 301 V1.2.1
	multiplexedCycleLength(0), //DS 301 V1.2.1
	prescaler(2), //DS 301 V1.2.1
	nodeCollection(unordered_map<uint8_t, shared_ptr<BaseNode>>()),
	buildConfigurations(vector<shared_ptr<PlkConfiguration>>()),
	activeConfiguration("")
{}

Network::Network(const string networkId) :
	networkId(networkId),
	cycleTime(0),
	asyncMTU(300), //DS 301 V1.2.1
	multiplexedCycleLength(0), //DS 301 V1.2.1
	prescaler(2), //DS 301 V1.2.1
	nodeCollection(unordered_map<uint8_t, shared_ptr<BaseNode>>()),
	buildConfigurations(vector<shared_ptr<PlkConfiguration>>()),
	activeConfiguration("")
{}

Network::~Network()
{}

Result Network::AddNode(shared_ptr<ControlledNode>& node)
{
	for (auto var : this->nodeCollection)
	{
		if (var.first == node.get()->GetNodeIdentifier())
		{
			return Result(ErrorCode::NODE_EXISTS);
		}
	}
	this->nodeCollection.insert(pair<uint8_t, shared_ptr<BaseNode>>(node.get()->GetNodeIdentifier(), node));
	return Result();
}

Result Network::AddNode(shared_ptr<ManagingNode>& node)
{
	for (auto var : this->nodeCollection)
	{
		if (var.first == node.get()->GetNodeIdentifier())
		{
			return Result(ErrorCode::NODE_EXISTS);
		}
	}
	this->nodeCollection.insert(pair<uint8_t, shared_ptr<BaseNode>>(node.get()->GetNodeIdentifier(), node));
	return Result();
}

Result Network::GetControlledNode(const uint8_t nodeID, shared_ptr<ControlledNode>& node)
{
	for (auto var : this->nodeCollection)
	{
		if (var.first == nodeID)
		{
			node = dynamic_pointer_cast<ControlledNode>(var.second);
			return Result();
		}
	}
	return Result(ErrorCode::NODE_DOES_NOT_EXIST);
}

Result Network::GetManagingNode(shared_ptr<ManagingNode>& node)
{
	for (auto var : this->nodeCollection)
	{
		if (var.first == 240)
		{
			node = dynamic_pointer_cast<ManagingNode>(var.second);
			return Result();
		}
	}
	return Result(ErrorCode::NODE_DOES_NOT_EXIST);
}

Result Network::RemoveNode(const uint8_t nodeID)
{
	unordered_map<uint8_t, shared_ptr<BaseNode>>::iterator it;
	for (it = this->nodeCollection.begin() ; it != this->nodeCollection.end(); ++it)
	{
		if (it->first == nodeID)
			break;
	}
	if (it == this->nodeCollection.end())
		return Result(ErrorCode::NODE_DOES_NOT_EXIST);

	this->nodeCollection.erase(it);
	return Result();
}

Result Network::GetNodes(vector<shared_ptr<BaseNode>>& nodeCollection)
{
	for (auto var : this->nodeCollection)
	{
		nodeCollection.push_back(var.second);
	}
	return Result();
}

Result Network::GetAvailableNodeIds(vector<uint8_t>& nodeIdCollection)
{
	for (auto var : this->nodeCollection)
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
	this->cycleTime = cycleTime;
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

Result Network::SetConfigurationSettingEnabled(const string configName, const string settingName, bool enabled)
{
	for (auto config : this->buildConfigurations)
	{
		if (config.get()->GetConfigurationName() == configName)
		{
			for (auto setting : config.get()->GetBuildConfigurationSettings())
			{
				if (setting.get()->GetName() == settingName)
				{
					setting.get()->SetEnabled(enabled);
					return Result();
				}
			}
		}
	}
	return Result(ErrorCode::BUILD_CONFIGURATION_DOES_NOT_EXIST);
}


Result Network::AddConfigurationSetting(const string configID, shared_ptr<BuildConfigurationSetting> newSetting)
{
	for (auto& config : this->buildConfigurations)
	{
		if (config.get()->GetConfigurationName() == configID)
		{
			for (auto& setting : config.get()->GetBuildConfigurationSettings())
			{
				if (setting.get()->GetName() == newSetting.get()->GetName() && setting.get()->GetValue() == newSetting.get()->GetValue())
				{
					return Result(ErrorCode::BUILD_SETTING_EXISTS);
				}
			}
			config.get()->AddBuildConfigurationSetting(newSetting);
			return Result();
		}
	}
	return Result(ErrorCode::BUILD_CONFIGURATION_DOES_NOT_EXIST);
}

Result Network::RemoveConfigurationSetting(string configID, const string name)
{
	for (auto& config : this->buildConfigurations)
	{
		if (config.get()->GetConfigurationName() == configID)
		{
			vector<shared_ptr<BuildConfigurationSetting>>::iterator it;
			for (it = config.get()->GetBuildConfigurationSettings().begin() ; it != config.get()->GetBuildConfigurationSettings().end(); ++it)
			{
				if (it->get()->GetName() == name)
					break;
			}
			if (it == config.get()->GetBuildConfigurationSettings().end())
				return Result(ErrorCode::BUILD_SETTING_DOES_NOT_EXIST);

			config.get()->GetBuildConfigurationSettings().erase(it);
			return Result();
		}
	}
	return Result(ErrorCode::BUILD_CONFIGURATION_DOES_NOT_EXIST);
}

Result Network::AddConfiguration(string configID)
{
	for (auto& config : this->buildConfigurations)
	{
		if (config.get()->GetConfigurationName() == configID)
		{
			return Result(ErrorCode::BUILD_CONFIGURATION_EXISTS);
		}
	}
	shared_ptr<PlkConfiguration> config = make_shared<PlkConfiguration>(configID);
	this->buildConfigurations.push_back(config);

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
		return Result(ErrorCode::BUILD_CONFIGURATION_DOES_NOT_EXIST);

	this->buildConfigurations.erase(it);
	return Result();
}

Result Network::ReplaceConfigurationName(const string oldConfigID, const string newConfigID)
{
	for (auto& config : this->buildConfigurations)
	{
		if (config.get()->GetConfigurationName() == oldConfigID)
		{
			config.get()->SetConfigurationName(newConfigID);
			return Result();
		}
	}
	return Result(ErrorCode::BUILD_SETTING_DOES_NOT_EXIST);
}

Result Network::GetConfigurationSettings(string configID, vector<shared_ptr<BuildConfigurationSetting>>& returnRef)
{
	for (auto& config : this->buildConfigurations)
	{
		if (config.get()->GetConfigurationName() == configID)
		{
			returnRef = config.get()->GetBuildConfigurationSettings();
			return Result();
		}
	}
	return Result(ErrorCode::BUILD_SETTING_DOES_NOT_EXIST);
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

bool Network::SetActiveConfiguration(const string configID)
{
	for (auto& config : this->buildConfigurations)
	{
		if (config.get()->GetConfigurationName() == configID)
		{
			this->activeConfiguration = configID;
			return true;
		}
	}
	return false;
}
