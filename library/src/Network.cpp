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

Network::Network() :
	networkId(""),
	cycleTime(0),
	asyncMTU(0),
	multiplexedCycleLength(0),
	prescaler(0),
	nodeCollection(),
	buildConfigurations(),
	activeConfiguration()
{}

Network::Network(const std::string id) :
	networkId(id),
	cycleTime(0),
	asyncMTU(0),
	multiplexedCycleLength(0),
	prescaler(0),
	nodeCollection(),
	buildConfigurations(),
	activeConfiguration()
{}

Network::~Network()
{}

IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result Network::AddNode(IndustrialNetwork::POWERLINK::Core::Node::ControlledNode& node)
{
	for (auto var : this->nodeCollection)
	{
		if (var.first == node.GetNodeIdentifier())
		{
			return Result(ErrorCode::NODE_EXISTS);
		}
	}
	std::shared_ptr<IndustrialNetwork::POWERLINK::Core::Node::BaseNode> ptr = std::make_shared<ControlledNode>(node);
	this->nodeCollection.insert(std::pair<std::uint8_t, std::shared_ptr<IndustrialNetwork::POWERLINK::Core::Node::BaseNode>>(node.GetNodeIdentifier(), ptr));
	return Result();
}

IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result Network::AddNode(IndustrialNetwork::POWERLINK::Core::Node::ManagingNode& node)
{
	for (auto var : this->nodeCollection)
	{
		if (var.first == node.GetNodeIdentifier())
		{
			return Result(ErrorCode::NODE_EXISTS);
		}
	}
	std::shared_ptr<IndustrialNetwork::POWERLINK::Core::Node::BaseNode> ptr = std::make_shared<ManagingNode>(node);
	this->nodeCollection.insert(std::pair<std::uint8_t, std::shared_ptr<IndustrialNetwork::POWERLINK::Core::Node::BaseNode>>(node.GetNodeIdentifier(), ptr));
	return Result();
}

Result Network::GetNode(const uint8_t nodeID, IndustrialNetwork::POWERLINK::Core::Node::BaseNode& node)
{
	for (auto var : this->nodeCollection)
	{
		if (var.first == nodeID)
		{
			node = *var.second.get();
			return Result();
		}
	}
	return Result(ErrorCode::NODE_DOES_NOT_EXIST);
}
Result Network::RemoveNode(const uint8_t nodeID)
{
	std::unordered_map<std::uint8_t, std::shared_ptr<IndustrialNetwork::POWERLINK::Core::Node::BaseNode>>::iterator it;
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

Result Network::ReplaceNode(const uint8_t nodeID, const IndustrialNetwork::POWERLINK::Core::Node::BaseNode& node)
{
	return Result();
}

std::vector<std::shared_ptr<IndustrialNetwork::POWERLINK::Core::Node::BaseNode>> Network::GetNodes(const IndustrialNetwork::POWERLINK::Core::Node::NodeType& type)
{
	return std::vector<std::shared_ptr<IndustrialNetwork::POWERLINK::Core::Node::BaseNode>>();
}

const std::string Network::GetNetworkId()
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

bool Network::SetConfigurationSettingEnabled(const std::string configName, const std::string settingName, bool enabled)
{
	for (auto config : this->buildConfigurations)
	{
		if (config.GetConfigurationName() == configName)
		{
			for (auto setting : config.GetBuildConfigurationSettings())
			{
				if (setting.GetName() == settingName)
				{
					setting.SetEnabled(enabled);
					return true;
				}
			}
		}
	}
	return false;
}


bool Network::AddConfigurationSetting(const std::string configID, BuildConfigurationSetting newSetting)
{
	for (auto& config : this->buildConfigurations)
	{
		if (config.GetConfigurationName() == configID)
		{
			for (auto& setting : config.GetBuildConfigurationSettings())
			{
				if (setting.GetName() == newSetting.GetName() && setting.GetValue() == newSetting.GetValue())
				{
					return false;
				}
			}
			config.AddBuildConfigurationSetting(newSetting);
			return true;
		}
	}
	return false;
}

bool Network::RemoveConfigurationSetting(std::string configID, const std::string name)
{
	for (auto& config : this->buildConfigurations)
	{
		if (config.GetConfigurationName() == configID)
		{
			std::vector<BuildConfigurationSetting>::iterator it;
			for (it = config.GetBuildConfigurationSettings().begin() ; it != config.GetBuildConfigurationSettings().end(); ++it)
			{
				if (it->GetName() == name)
					break;
			}
			if (it == config.GetBuildConfigurationSettings().end())
				return false;

			config.GetBuildConfigurationSettings().erase(it);
			return true;
		}
	}
	return false;
}

bool Network::AddConfiguration(std::string configID)
{
	for (auto& config : this->buildConfigurations)
	{
		if (config.GetConfigurationName() == configID)
		{
			return false;
		}
	}
	this->buildConfigurations.push_back(PlkConfiguration(configID));

	if (this->buildConfigurations.size() == 1)
		this->activeConfiguration = configID;

	return true;
}

bool Network::RemoveConfiguration(std::string configID)
{
	if (this->activeConfiguration == configID)
		return false;

	std::vector<PlkConfiguration>::iterator it;
	for (it = this->buildConfigurations.begin() ; it != this->buildConfigurations.end(); ++it)
	{
		if (it->GetConfigurationName() == configID)
			break;
	}
	if (it == this->buildConfigurations.end())
		return false;

	this->buildConfigurations.erase(it);
	return true;
}

bool Network::ReplaceConfigurationName(const std::string oldConfigID, const std::string newConfigID)
{
	for (auto& config : this->buildConfigurations)
	{
		if (config.GetConfigurationName() == oldConfigID)
		{
			config.SetConfigurationName(newConfigID);
			return true;
		}
	}
	return false;
}

bool Network::GetConfigurationSettings(std::string configID, std::vector<BuildConfigurationSetting>& returnRef)
{
	for (auto& config : this->buildConfigurations)
	{
		if (config.GetConfigurationName() == configID)
		{
			returnRef = config.GetBuildConfigurationSettings();
			return true;
		}
	}
	return false;
}

const std::string& Network::GetActiveConfiguration()
{
	return this->activeConfiguration;
}

bool Network::SetActiveConfiguration(const std::string configID)
{
	for (auto& config : this->buildConfigurations)
	{
		if (config.GetConfigurationName() == configID)
		{
			this->activeConfiguration = configID;
			return true;
		}
	}
	return false;
}
