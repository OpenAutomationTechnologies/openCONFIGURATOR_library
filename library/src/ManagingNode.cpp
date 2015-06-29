/************************************************************************
\file ManagingNode.cpp
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
#include "ManagingNode.h"

using namespace std;
using namespace IndustrialNetwork::POWERLINK::Core::Node;

ManagingNode::ManagingNode(bool active, std::uint8_t nodeID, const string nodeName) : BaseNode(nodeID, nodeName),
	active(active)
{}

ManagingNode::~ManagingNode()
{}

bool ManagingNode::AddNodeAssignement(NodeAssignment assign)
{
	switch (assign)
	{

		case NodeAssignment::NMT_NODEASSIGN_START_CN:
		case NodeAssignment::NMT_NODEASSIGN_MANDATORY_CN:
		case NodeAssignment::NMT_NODEASSIGN_KEEPALIVE:
		case NodeAssignment::NMT_NODEASSIGN_SWVERSIONCHECK:
		case NodeAssignment::NMT_NODEASSIGN_SWUPDATE:
		case NodeAssignment::NMT_NODEASSIGN_MULTIPLEXED_CN:
			return false;
		case NodeAssignment::NMT_NODEASSIGN_NODE_EXISTS:
		case NodeAssignment::NMT_NODEASSIGN_NODE_IS_CN:
		case NodeAssignment::NMT_NODEASSIGN_ASYNCONLY_NODE:
		case NodeAssignment::NMT_NODEASSIGN_RT1:
		case NodeAssignment::NMT_NODEASSIGN_RT2:
		case NodeAssignment::NMT_NODEASSIGN_MN_PRES:
			{
				auto it = find(this->GetNodeAssignment().begin(), this->GetNodeAssignment().end(), assign);
				if (it != this->GetNodeAssignment().end())
				{
					this->GetNodeAssignment().push_back(assign);
					return true;
				}
				else
					return false;

				break;
			}
		default:
			break;
	}
	return false;
}

bool ManagingNode::RemoveNodeAssignment(NodeAssignment assign)
{
	this->GetNodeAssignment().erase(remove(this->GetNodeAssignment().begin(), this->GetNodeAssignment().end(), assign), this->GetNodeAssignment().end());
	return true;
}

uint32_t ManagingNode::GetNodeAssignmentValue()
{
	if (this->GetNodeAssignment().empty())
		return 0;

	NodeAssignment assign = this->GetNodeAssignment()[0];
	for (auto var : this->GetNodeAssignment())
	{
		assign |=  var;
	}

	return static_cast<underlying_type<NodeAssignment>::type>(assign);
}

bool ManagingNode::GetActive()
{
	return this->active;
}

void ManagingNode::SetActive(bool active)
{
	this->active = active;
}