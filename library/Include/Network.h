/************************************************************************
\file Network.h
\brief Implementation of the Class Network
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
#if !defined NETWORK_H
#define NETWORK_H

#include <memory>
#include "Result.h"
#include "BaseNode.h"
#include "Module.h"
#include "NodeType.h"
#include "PlkConfiguration.h"
#include "Exports.h"

namespace IndustrialNetwork
{
	namespace POWERLINK
	{
		namespace Core
		{
			namespace NetworkHandling
			{
				/**
				\brief
				\author rueckerc
				*/
				class DLLEXPORT Network
				{

					public:
						Network();
						Network(const std::string id);
						virtual ~Network();
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result AddNode(const IndustrialNetwork::POWERLINK::Core::Node::BaseNode& node);
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result GetNode(const std::uint8_t nodeID, IndustrialNetwork::POWERLINK::Core::Node::BaseNode& node);
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result RemoveNode(const std::uint8_t nodeID);
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result ReplaceNode(const std::uint8_t nodeID, const IndustrialNetwork::POWERLINK::Core::Node::BaseNode& node);

						std::vector<std::shared_ptr<IndustrialNetwork::POWERLINK::Core::Node::BaseNode>> GetNodes(const IndustrialNetwork::POWERLINK::Core::Node::NodeType& type);

						const std::string GetNetworkId();
						const std::uint32_t GetCycleTime();
						const std::uint32_t GetAsyncMTU();
						const std::uint32_t GetMultiplexedCycleLength();
						const std::uint32_t GetPrescaler();

						void SetCycleTime(const std::uint32_t cycleTime);
						void SetAsyncMTU(const std::uint32_t asyncMTU);
						void SetMultiplexedCycleLength(const std::uint32_t multiCycleLength);
						void SetPrescaler(const std::uint32_t prescaler);

					private:
						std::string networkId;
						std::uint32_t cycleTime;
						std::uint32_t asyncMTU;
						std::uint32_t multiplexedCycleLength;
						std::uint32_t prescaler;
						IndustrialNetwork::POWERLINK::Core::ConfigurationHandling::PlkConfiguration configuration;
						std::unordered_map<std::uint32_t, std::shared_ptr<IndustrialNetwork::POWERLINK::Core::Node::BaseNode>> nodeCollection;
				};

			}

		}

	}

}
#endif
