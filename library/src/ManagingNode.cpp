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

using namespace IndustrialNetwork::POWERLINK::Core::Node;
using namespace IndustrialNetwork::POWERLINK::Core::ObjectDictionary;
using namespace IndustrialNetwork::POWERLINK::Core::ErrorHandling;
using namespace IndustrialNetwork::POWERLINK::Core::CoreConfiguration;

ManagingNode::ManagingNode(std::uint8_t nodeID, const std::string& nodeName) : BaseNode(nodeID, nodeName),
	dynamicChannelList(std::vector<std::shared_ptr<DynamicChannel>>()),
	rmnCount(0)
{
	if (nodeID != 240) //Add assignments for RMNs only
	{
		AddNodeAssignement(NodeAssignment::MNT_NODEASSIGN_VALID);
		AddNodeAssignement(NodeAssignment::NMT_NODEASSIGN_NODE_EXISTS);
		AddNodeAssignement(NodeAssignment::NMT_NODEASSIGN_NODE_IS_CN);
	}
}

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
		case NodeAssignment::MNT_NODEASSIGN_VALID:
		case NodeAssignment::NMT_NODEASSIGN_NODE_EXISTS:
		case NodeAssignment::NMT_NODEASSIGN_NODE_IS_CN:
		case NodeAssignment::NMT_NODEASSIGN_ASYNCONLY_NODE:
		case NodeAssignment::NMT_NODEASSIGN_RT1:
		case NodeAssignment::NMT_NODEASSIGN_RT2:
		case NodeAssignment::NMT_NODEASSIGN_MN_PRES:
			{
				auto it = find(this->GetNodeAssignment().begin(), this->GetNodeAssignment().end(), assign);
				if (it == this->GetNodeAssignment().end())
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

	return static_cast<std::underlying_type<NodeAssignment>::type>(assign);
}

void ManagingNode::AddDynamicChannel(std::shared_ptr<DynamicChannel>& channelRef)
{
	this->dynamicChannelList.push_back(channelRef);
}

bool ManagingNode::GetDynamicChannel(PlkDataType dataType, std::shared_ptr<DynamicChannel>& retChannel)
{
	for (auto& channel : this->dynamicChannelList)
	{
		if (channel->GetDataType() == dataType)
		{
			retChannel = channel;
			return true;
		}
	}
	return false;
}

uint32_t ManagingNode::GetConfigurationObjectCount()
{
	uint32_t count = 0;
	for (auto& object : this->GetObjectDictionary())
	{
		if (object.second->GetObjectType() == ObjectType::VAR && object.second->WriteToConfiguration())
		{
			count++;
		}

		uint32_t mappingObjNrOfEntries = 0;
		uint32_t mappingObjCount = 0;
		for (auto& subobject : object.second->GetSubObjectCollection())
		{
			if (object.first >= 0x1600 && object.first < 0x1700) //Count for reset and actual NrOfEntries
			{
				if (subobject.second->WriteToConfiguration() && subobject.first == 0x0)
				{
					count += 2; //Add count for mapping set and reset
					mappingObjNrOfEntries = subobject.second->GetTypedActualValue<uint16_t>(); //Set actual nr of mapping objects
				}
				else if (subobject.second->WriteToConfiguration() && mappingObjCount < mappingObjNrOfEntries) //Only count mapping objects of they are activated
				{
					count++;
					mappingObjCount++;
				}
			}
			else if (object.first >= 0x1A00 && object.first < 0x1B00) //Count for reset and actual NrOfEntries
			{
				if (subobject.second->WriteToConfiguration()  && subobject.first == 0x0)
				{
					count += 2; //Add count for mapping set and reset
					mappingObjNrOfEntries = subobject.second->GetTypedActualValue<uint16_t>(); //Set actual nr of mapping objects
				}
				else if (subobject.second->WriteToConfiguration() && mappingObjCount < mappingObjNrOfEntries) //Only count mapping objects of they are activated
				{
					count++;
					mappingObjCount++;
				}
			}
			else if (object.first == 0x1F81 && subobject.first != 0x0) //Count for node assignement and reassignment
			{
				if (subobject.second->WriteToConfiguration())
				{
					count += 2; //Add assignment and reassignment count
					if (this->GetNodeIdentifier() == 240) //Count 1F22 only for active managing node
						count++;
				}
			}
			else if (subobject.second->WriteToConfiguration())
			{
				count++;
			}
		}
	}
	//Remove reassignment count for RMNs
	count -= this->GetRmnCount();

	boost::format formatter(kMsgNodeObjectCount);
	formatter
	% (uint32_t) this->GetNodeIdentifier()
	% count;
	LOG_INFO() << formatter.str();
	return count;
}

uint32_t ManagingNode::GetConfigurationObjectSize()
{
	uint32_t size = 0;
	for (auto& object : this->GetObjectDictionary())
	{
		if (object.second->GetObjectType() == ObjectType::VAR && object.second->WriteToConfiguration())
		{
			size += object.second->GetBitSize();
		}

		uint32_t mappingObjNrOfEntries = 0;
		uint32_t mappingObjCount = 0;
		for (auto& subobject : object.second->GetSubObjectCollection())
		{
			if (object.first >= 0x1600 && object.first < 0x1700) //Count for reset and actual NrOfEntries
			{
				if (subobject.second->WriteToConfiguration() && subobject.first == 0x0)
				{
					size += 2 * subobject.second->GetBitSize(); //Add size of NrOfEntries set and reset
					mappingObjNrOfEntries = subobject.second->GetTypedActualValue<uint16_t>(); //Set actual nr of mapping objects
				}
				else if (subobject.second->WriteToConfiguration() && mappingObjCount < mappingObjNrOfEntries) //Only count mapping objects of they are activated
				{
					size += subobject.second->GetBitSize();
					mappingObjCount++;
				}
			}
			else if (object.first >= 0x1A00 && object.first < 0x1B00) //Count for reset and actual NrOfEntries
			{
				if (subobject.second->WriteToConfiguration() && subobject.first == 0x0)
				{
					size += 2 * subobject.second->GetBitSize();//Add size of NrOfEntries
					mappingObjNrOfEntries = subobject.second->GetTypedActualValue<uint16_t>(); //Set actual nr of mapping objects
				}
				else if (subobject.second->WriteToConfiguration() && mappingObjCount < mappingObjNrOfEntries) //Only count mapping objects of they are activated
				{
					size += subobject.second->GetBitSize();
					mappingObjCount++;
				}
			}
			else if (object.first == 0x1F81 && subobject.first != 0x0) //Size for node assignement and reassignment
			{
				if (subobject.second->WriteToConfiguration())
				{
					size += 2 * subobject.second->GetBitSize();
					if (this->GetNodeIdentifier() == 240) //Count 1F22 only for active managing node
						size += subobject.second->GetBitSize();
				}
			}
			else if (subobject.second->WriteToConfiguration())
			{
				size += subobject.second->GetBitSize();
			}
		}
	}
	//Remove reassignment size count for RMNs
	size -= 32 * this->GetRmnCount();

	boost::format formatter(kMsgNodeObjectCountSize);
	formatter
	% (uint32_t) this->GetNodeIdentifier()
	% size;
	LOG_INFO() << formatter.str();
	return size;
}

uint16_t ManagingNode::GetRmnCount()
{
	return this->rmnCount;
}

void ManagingNode::SetRmnCount(std::uint16_t count)
{
	this->rmnCount = count;
}

Result ManagingNode::SetMultiplexedCycle(const std::uint8_t nodeID, const std::uint8_t multiplexedCycle)
{
	std::shared_ptr<SubObject> multplCycleCount;
	Result res = this->GetSubObject(0x1F98, 0x7, multplCycleCount);
	if (!res.IsSuccessful())
		return res;

	uint16_t cycle_count = 0;
	if (multplCycleCount->WriteToConfiguration())
		cycle_count = multplCycleCount->GetTypedActualValue<uint16_t>();
	else
		cycle_count = multplCycleCount->GetTypedDefaultValue<uint16_t>();

	if (multiplexedCycle > cycle_count)
		return Result(ErrorCode::MULTIPLEX_CYCLE_ASSIGN_INVALID);

	std::shared_ptr<SubObject> multplCycleAssign;
	res = this->GetSubObject(0x1F9B, nodeID, multplCycleAssign);
	if (!res.IsSuccessful())
		return res;

	if (multplCycleAssign->WriteToConfiguration())
		return Result(ErrorCode::MULTIPLEX_CYCLE_ASSIGN_INVALID);

	std::stringstream nodeIdStr;
	nodeIdStr << (uint32_t) multiplexedCycle;
	multplCycleAssign->SetTypedObjectActualValue(nodeIdStr.str());

	return res;
}

Result ManagingNode::ResetMultiplexedCycle(const std::uint8_t nodeID)
{
	std::shared_ptr<SubObject> multplCycleAssign;
	Result res = this->GetSubObject(0x1F9B, nodeID, multplCycleAssign);
	if (!res.IsSuccessful())
		return res;

	multplCycleAssign->SetTypedObjectActualValue("0");

	return res;
}