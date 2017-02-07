/************************************************************************
\file RedundantManagingNode.h
\brief Implementation of the Class RedundantManagingNode
\author rueckerc, Bernecker+Rainer Industrie Elektronik Ges.m.b.H.
\date 04-Feb-2017 09:38:08
************************************************************************/

/*------------------------------------------------------------------------------
Copyright (c) 2017, Bernecker+Rainer Industrie-Elektronik Ges.m.b.H. (B&R)
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
#if !defined REDUNDANT_MANAGING_NODE_H
#define REDUNDANT_MANAGING_NODE_H

#include <cstdint>
#include <vector>
#include "ControlledNode.h"
#include "ManagingNode.h"

namespace IndustrialNetwork
{
	namespace POWERLINK
	{
		namespace Core
		{
			namespace Node
			{
				/**
				\brief Representes the RedundantManagingNode in the POWERLINK network.
				\author rueckerc, Bernecker+Rainer Industrie Elektronik Ges.m.b.H.
				*/
				class DLLEXPORT RedundantManagingNode : public IndustrialNetwork::POWERLINK::Core::Node::ControlledNode
				{

					public:
						RedundantManagingNode(std::uint8_t nodeID, const std::string& nodeName = "");
						virtual ~RedundantManagingNode();

						std::uint16_t GetRmnCount() const;
						void AddRmnId(std::uint16_t nodeId);
						void RemoveRmnId(std::uint16_t nodeId);
						void ClearRmnList();
						const std::vector<std::uint16_t>& GetRmnList();

						std::uint32_t GetRMNConfigurationObjectCount();
						std::uint32_t GetRMNConfigurationObjectSize();

						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result SyncMNObjects(const std::shared_ptr<IndustrialNetwork::POWERLINK::Core::Node::ManagingNode>& mn);
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result ClearMappingObjects();
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result IsChannelFree(std::uint32_t commParamIndex, bool& isFree);

					private:

						std::vector<std::uint16_t> rmnList;

				};
			}
		}
	}
}
#endif
