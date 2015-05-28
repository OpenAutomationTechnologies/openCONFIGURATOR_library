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

using namespace std;
using namespace IndustrialNetwork::POWERLINK::Core::Configuration;
using namespace IndustrialNetwork::POWERLINK::Core::ErrorHandling;
using namespace IndustrialNetwork::POWERLINK::Core::NetworkHandling;
using namespace IndustrialNetwork::POWERLINK::Core::CoreConfiguration;

ProjectManager::ProjectManager() :
	networkList(unordered_map<string, shared_ptr<Network>>())
{}

ProjectManager::~ProjectManager()
{}

ProjectManager& ProjectManager::GetInstance()
{
	static ProjectManager instance;
	return instance;
}

Result ProjectManager::AddNetwork(const string& networkId, shared_ptr<Network>& network)
{
	if (this->networkList.find(networkId) != this->networkList.end())
		return Result(ErrorCode::NETWORK_EXISTS);
	this->networkList.insert(make_pair(networkId, network));
	return Result();
}

Result ProjectManager::GetNetwork(const string networkId, shared_ptr<Network>& net)
{
	unordered_map<string, shared_ptr<Network>>::const_iterator got = this->networkList.find(networkId);
	if (got == this->networkList.end())
	{
		return Result(ErrorCode::NETWORK_DOES_NOT_EXIST);
	}
	else
	{
		net = this->networkList.find(networkId)->second;
		return Result();
	}
	return Result();
}

Result ProjectManager::RemoveNetwork(const string networkId)
{
	unordered_map<string, shared_ptr<Network>>::const_iterator got = this->networkList.find(networkId);
	if (got == this->networkList.end())
	{
		return Result(ErrorCode::NETWORK_DOES_NOT_EXIST);
	}
	else
	{
		this->networkList.erase(networkId);
		return Result();
	}
	return Result();
}

Result ProjectManager::BuildConfiguration(const string networkId, ostream& configuration)
{
	//NOT YET IMPLEMENTED
	return Result(ErrorCode::UNHANDLED_EXCEPTION);
}

Result ProjectManager::BuildProcessImage(const string networkId, ostream& configuration)
{
	//NOT YET IMPLEMENTED
	return Result(ErrorCode::UNHANDLED_EXCEPTION);
}

Result ProjectManager::GetNetworks(unordered_map<string, shared_ptr<Network>>& networkList)
{
	networkList = this->networkList;
	return Result();
}

Result ProjectManager::ClearNetworkList()
{
	this->networkList.clear();
	return Result();
}

const vector<string> ProjectManager::GetSupportedSettingIds()
{
	vector<string> vect(begin(BuildConfigurationIdName), end(BuildConfigurationIdName)) ;
	return vect;
}

Result ProjectManager::InitLoggingConfiguration(const string configFile)
{
	LoggingConfiguration::initConfiguration(configFile);
	return Result();
}

std::vector<std::string> ProjectManager::GetNetworkIds()
{
	vector<string> returnVec;
	unordered_map<string, shared_ptr<Network>>::iterator it;
	for (it = networkList.begin();  it != networkList.end(); ++it)
	{
		returnVec.push_back(it->first);
	}
	return returnVec;
}
