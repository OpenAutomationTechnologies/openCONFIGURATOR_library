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
#include "LoggingConfiguration.h"

using IndustrialNetwork::POWERLINK::Core::Configuration::ProjectManager;
using namespace IndustrialNetwork::POWERLINK::Core::Configuration;
using namespace IndustrialNetwork::POWERLINK::Core::ErrorHandling;
using namespace IndustrialNetwork::POWERLINK::Core::NetworkHandling;
using namespace IndustrialNetwork::POWERLINK::Core::CoreConfiguration;


ProjectManager::ProjectManager() :
	networkList(std::map<std::string, IndustrialNetwork::POWERLINK::Core::NetworkHandling::Network>())
{}



ProjectManager::~ProjectManager()
{

}


ProjectManager& ProjectManager::GetInstance()
{
	static ProjectManager instance;
	return instance;
}

IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result ProjectManager::AddNetwork(const std::string& networkId, IndustrialNetwork::POWERLINK::Core::NetworkHandling::Network& net)
{
	this->networkList.insert(std::pair<std::string, Network>(networkId, net));
	return Result();
}
IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result ProjectManager::GetNetwork(const std::string networkID, IndustrialNetwork::POWERLINK::Core::NetworkHandling::Network& net)
{
	std::map<std::string, Network>::const_iterator got = this->networkList.find(networkID);
	if (got == this->networkList.end())
	{
		return Result(ErrorCode::UNHANDLED_EXCEPTION);
	}
	else
	{
		net = this->networkList.find(networkID)->second;
		return Result();
	}
	return Result();
}
IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result ProjectManager::RemoveNetwork(const std::string networkId)
{
	std::map<std::string, Network>::const_iterator got = this->networkList.find(networkId);
	if (got == this->networkList.end())
	{
		return Result(ErrorCode::UNHANDLED_EXCEPTION);
	}
	else
	{
		this->networkList.erase(networkId);
		return Result();
	}
	return Result();
}
IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result ProjectManager::BuildConfiguration(const std::string networkId,std::ostream& configuration)
{
	return Result();
}

IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result ProjectManager::BuildProcessImage(const std::string networkId, std::ostream& configuration)
{
	return Result();
}

IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result ProjectManager::GetNetworks(std::map<std::string, Network>& networkList)
{
	networkList = this->networkList;
	return Result();
}

IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result ProjectManager::ClearNetworkList()
{
	this->networkList.clear();
	return Result();
}

const std::vector<std::string> ProjectManager::GetSupportedSettingIds()
{
	std::vector<std::string> vect(std::begin(BuildConfigurationIdName), std::end(BuildConfigurationIdName)) ;
	return vect;
}


IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result ProjectManager::InitLoggingConfiguration(const std::string configFile)
{
	LoggingConfiguration::initConfiguration(configFile);
	return Result();
}
