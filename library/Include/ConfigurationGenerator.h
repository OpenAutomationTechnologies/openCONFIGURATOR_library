/************************************************************************
\file ConfigurationGenerator.h
\brief Implementation of the Class ConfigurationGenerator
\author rueckerc, Bernecker+Rainer Industrie Elektronik Ges.m.b.H.
\date 24-Feb-2015 13:06:38
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

#if !defined CONFIGURATION_GENERATOR_H
#define CONFIGURATION_GENERATOR_H

#include <memory>
#include <sstream>
#include <iomanip>
#include <cstdio>
#include <algorithm>

#include <boost/date_time.hpp>

#include "BaseNode.h"
#include "Result.h"
#include "Network.h"
#include "ManagingNode.h"
#include "Object.h"
#include "LoggingConfiguration.h"
#include "Utilities.h"

namespace IndustrialNetwork
{
	namespace POWERLINK
	{
		namespace Core
		{
			/**
			\brief %Network handling related namespace
			\author rueckerc, Bernecker+Rainer Industrie Elektronik Ges.m.b.H.
			*/
			namespace NetworkHandling
			{

				/**
				\brief Computes the POWERLINK network configuration.
				\author rueckerc, Bernecker+Rainer Industrie Elektronik Ges.m.b.H.
				*/
				class ConfigurationGenerator
				{
					public:

						virtual ~ConfigurationGenerator();
						static ConfigurationGenerator& GetInstance();

						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result GenerateNetworkConfiguration(const std::shared_ptr<Network>& net, std::stringstream& configurationOutput, std::stringstream& hexOutput);

					private:
						//singleton
						ConfigurationGenerator();
						ConfigurationGenerator(ConfigurationGenerator const&);
						void operator=(ConfigurationGenerator const&);

						void WriteHeader(const std::shared_ptr<Network>& net, std::stringstream& configurationOutput);
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result WriteManagingNodeObjectCount(const std::shared_ptr<Network>& net, std::stringstream& configurationOutput, std::stringstream& hexOutput);
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result WriteNodeAssignment(const std::shared_ptr<Network>& net, std::stringstream& configurationOutput, std::stringstream& hexOutput, bool writeNodeValid, bool writeComments);

						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result WriteManagingNodeConfiguration(const std::shared_ptr<Network>& net, std::stringstream& configurationOutput, std::stringstream& hexOutput);
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result WriteControlledNodeConfiguration(const std::shared_ptr<Network>& net, const std::shared_ptr<IndustrialNetwork::POWERLINK::Core::Node::BaseNode>& node, std::stringstream& configurationOutput, std::stringstream& hexOutput);
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result WriteRedundantManagingNodeConfiguration(const std::shared_ptr<Network>& net, const std::shared_ptr<IndustrialNetwork::POWERLINK::Core::Node::BaseNode>& node, std::stringstream& configurationOutput, std::stringstream& hexOutput);

						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result WriteMappingNrOfEntriesZero(const std::shared_ptr<IndustrialNetwork::POWERLINK::Core::Node::BaseNode>& node, std::stringstream& configurationOutput, std::stringstream& hexOutput);
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result WriteMappingObjects(const std::shared_ptr<IndustrialNetwork::POWERLINK::Core::Node::BaseNode>& node, std::stringstream& configurationOutput, std::stringstream& hexOutput);
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result WriteMappingNrOfEntries(const std::shared_ptr<IndustrialNetwork::POWERLINK::Core::Node::BaseNode>& node, std::stringstream& configurationOutput, std::stringstream& hexOutput);
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result WriteCommunicationProfileArea(const std::shared_ptr<IndustrialNetwork::POWERLINK::Core::Node::BaseNode>& node, std::stringstream& configurationOutput, std::stringstream& hexOutput);
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result WriteManufacturerSpecificProfileArea(const std::shared_ptr<IndustrialNetwork::POWERLINK::Core::Node::BaseNode>& node, std::stringstream& configurationOutput, std::stringstream& hexOutput);
				};
			}
		}
	}
}

#endif
