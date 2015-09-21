/************************************************************************
\file ProjectManager.h
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
#if !defined PROJECT_MANAGER_H
#define PROJECT_MANAGER_H

#include <map>
#include <memory>
#include <sstream>

#include "Network.h"
#include "PlkConfiguration.h"
#include "LoggingConfiguration.h"
#include "Constants.h"

namespace IndustrialNetwork
{
	namespace POWERLINK
	{
		namespace Core
		{
			namespace Configuration
			{
				/**
				\brief Handles different projects in the core library.
				\author rueckerc, Bernecker+Rainer Industrie Elektronik Ges.m.b.H.
				*/
				class DLLEXPORT ProjectManager
				{

					public:
						virtual ~ProjectManager();

						/**
						\brief Retrieve the singleton instance of the class.
						\return ProjectManager instance
						*/
						static ProjectManager& GetInstance();

						/**
						\brief Add a network configuration to the core library.
						\param[in] networkId Network reference to be added.
						\param[in] network Network ID reference to be added.
						\return IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result
						*/

						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result AddNetwork(const std::string& networkId, std::shared_ptr<IndustrialNetwork::POWERLINK::Core::NetworkHandling::Network>& network);

						/**
						\brief Retrieve a network configuration from the library.
						\param[in] networkId Network identifier.
						\param[out] net NetworkManagement reference to be retrieved.
						\return IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result
						*/
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result GetNetwork(const std::string& networkId, std::shared_ptr<IndustrialNetwork::POWERLINK::Core::NetworkHandling::Network>& net);

						/**
						\brief Removes a network configuration from the list.
						\param[in] networkId to identify the configuration.
						\return IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result
						*/
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result RemoveNetwork(const std::string& networkId);

						/**
						\brief Retrieve a copied list of all managed network configuration.
						\param[out] networkList& to the network list.
						\return IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result
						*/
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result GetNetworks(std::map<std::string, std::shared_ptr<IndustrialNetwork::POWERLINK::Core::NetworkHandling::Network>>& networkList);

						/**
						\brief Clears the network list of the library.
						\return IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result
						*/
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result ClearNetworkList();

						/**
						\brief Get all available auto generation setting ids.
						\return std::vector containing the ids.
						*/
						const std::vector<std::string> GetSupportedSettingIds();

						/**
						\brief Define the logging configuration for the boost log library.
						\param[in] configuration Logging configuration.
						\return IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result
						*/
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result InitLoggingConfiguration(const std::string& configuration);
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result InitEclipseLoggingConfiguration(const std::string& loggingPath);

						/**
						\brief Returns a std::vector including all the existing network ids.
						\return Vector with the network ids. Vector is empty if no network exists.
						*/
						std::vector<std::string> GetNetworkIds();

					private:
						//singleton
						ProjectManager();
						ProjectManager(ProjectManager const&);
						void operator=(ProjectManager const&);

						/**
						List of managed networks.
						*/
						std::map<std::string, std::shared_ptr<IndustrialNetwork::POWERLINK::Core::NetworkHandling::Network>> networkList;
				};
			}
		}
	}
}
#endif
