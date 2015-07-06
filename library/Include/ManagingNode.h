/************************************************************************
\file ManagingNode.h
\brief Implementation of the Class ManagingNode
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
#if !defined MANAGING_NODE_H
#define MANAGING_NODE_H

#include <vector>
#include <memory>

#include "BaseNode.h"
#include "Constants.h"
#include "DynamicChannel.h"
#include "PlkDataType.h"

namespace IndustrialNetwork
{
	namespace POWERLINK
	{
		namespace Core
		{
			namespace Node
			{
				/**
				\brief Represents the Managing Nodes in the POWERLINK network.
				\author rueckerc, Bernecker+Rainer Industrie Elektronik Ges.m.b.H.
				*/
				class DLLEXPORT ManagingNode : public IndustrialNetwork::POWERLINK::Core::Node::BaseNode
				{

					public:
						ManagingNode(bool activ, std::uint8_t nodeID = 240, const std::string nodeName = "");
						virtual ~ManagingNode();

						bool AddNodeAssignement(NodeAssignment);
						bool RemoveNodeAssignment(NodeAssignment);
						std::uint32_t GetNodeAssignmentValue();

						bool GetActive();
						void SetActive(bool active);

						void AddDynamicChannel(std::shared_ptr<DynamicChannel>& channelRef);
						bool GetDynamicChannel(IndustrialNetwork::POWERLINK::Core::ObjectDictionary::PlkDataType dataType, std::shared_ptr<DynamicChannel>& retChannel);
						
						std::uint32_t GetConfigurationObjectCount();
						std::uint32_t GetConfigurationObjectSize();


					private:
						bool active;
						std::vector<std::shared_ptr<DynamicChannel>> dynamicChannelList;
				};

			}

		}

	}

}
#endif
