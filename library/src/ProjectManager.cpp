/************************************************************************
\file ProjectManager.cpp
\brief Implementation of the Class ProjectManager
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
#include "ProjectManager.h"

using namespace IndustrialNetwork::POWERLINK::Core::Configuration;
using namespace IndustrialNetwork::POWERLINK::Core::ErrorHandling;
using namespace IndustrialNetwork::POWERLINK::Core::NetworkHandling;
using namespace IndustrialNetwork::POWERLINK::Core::CoreConfiguration;

ProjectManager::ProjectManager() :
	networkList(std::map<std::string, std::shared_ptr<Network>>())
{}

ProjectManager::~ProjectManager()
{}

ProjectManager& ProjectManager::GetInstance()
{
	static ProjectManager instance;
	return instance;
}

Result ProjectManager::AddNetwork(const std::string& networkId, std::shared_ptr<Network>& network)
{
	if (this->networkList.find(networkId) != this->networkList.end())
	{
		//Network already exists
		boost::format formatter(kMsgExistingNetwork);
		formatter
		% networkId;
		LOG_ERROR() << "[" + networkId + "] " + formatter.str();
		return Result(ErrorCode::NETWORK_EXISTS, formatter.str());
	}
	this->networkList.insert(make_pair(networkId, network));
	//Log info network created
	boost::format formatter(kMsgNetworkCreated);
	formatter
	% networkId;
	LOG_INFO() << "[" + networkId + "] " + formatter.str();
	return Result();
}

Result ProjectManager::GetNetwork(const std::string& networkId, std::shared_ptr<Network>& net)
{
	auto got = this->networkList.find(networkId);
	if (got == this->networkList.end())
	{
		//Network does not exist
		boost::format formatter(kMsgNonExistingNetwork);
		formatter
		% networkId;
		LOG_ERROR() << "[" + networkId + "] " + formatter.str();
		return Result(ErrorCode::NETWORK_DOES_NOT_EXIST, formatter.str());
	}
	else
	{
		net = got->second;
		return Result();
	}
}

Result ProjectManager::RemoveNetwork(const std::string& networkId)
{
	auto got = this->networkList.find(networkId);
	if (got == this->networkList.end())
	{
		//Network does not exist
		boost::format formatter(kMsgNonExistingNetwork);
		formatter
		% networkId;
		LOG_ERROR() << "[" + networkId + "] " + formatter.str();
		return Result(ErrorCode::NETWORK_DOES_NOT_EXIST, formatter.str());
	}
	else
	{
		this->networkList.erase(networkId);
		//Log info network removed
		boost::format formatter(kMsgNetworkRemoved);
		formatter
		% networkId;
		LOG_INFO() << "[" + networkId + "] " + formatter.str();
		return Result();
	}
}

Result ProjectManager::GetNetworks(std::map<std::string, std::shared_ptr<Network>>& _networkList)
{
	_networkList = this->networkList;
	return Result();
}

Result ProjectManager::ClearNetworkList()
{
	this->networkList.clear();
	LOG_INFO() << "Network list cleared.";
	return Result();
}

const std::vector<std::string> ProjectManager::GetSupportedSettingIds()
{
	std::vector<std::string> vect(begin(BuildConfigurationIdName), end(BuildConfigurationIdName));
	LOG_INFO() << "Returned supported configuration setting ids.";
	return vect;
}

Result ProjectManager::InitLoggingConfiguration(const std::string& configuration)
{
	Result res = LoggingConfiguration::InitConfiguration(configuration);
	if (res.IsSuccessful())
	{
		//Log info logging initialised
		boost::format formatter(kMsgLoggingInitialised);
		formatter
		% configuration;
		LOG_INFO() << formatter.str();
	}
	return res;
}

Result ProjectManager::InitEclipseLoggingConfiguration(const std::string& loggingPath)
{
	Result res = LoggingConfiguration::InitEclipseConfiguration(loggingPath);
	if (res.IsSuccessful())
	{
		//Log info logging initialised
		boost::format formatter(kMsgLoggingInitialised);
		formatter
		% loggingPath;
		LOG_INFO() << formatter.str();
	}
	return res;
}

std::vector<std::string> ProjectManager::GetNetworkIds()
{
	std::vector<std::string> returnVec;
	for (auto it = networkList.begin(); it != networkList.end(); ++it)
	{
		returnVec.push_back(it->first);
	}
	return returnVec;
}
