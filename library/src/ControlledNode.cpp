/************************************************************************
\file ControlledNode.cpp
\brief Implementation of the Class ControlledNode
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
#include "ControlledNode.h"

using namespace IndustrialNetwork::POWERLINK::Core::Node;
using namespace IndustrialNetwork::POWERLINK::Core::ErrorHandling;
using namespace IndustrialNetwork::POWERLINK::Core::ObjectDictionary;
using namespace IndustrialNetwork::POWERLINK::Core::CoreConfiguration;

ControlledNode::ControlledNode(uint8_t nodeID, const std::string& nodeName) : BaseNode(nodeID, nodeName),
	operationMode(PlkOperationMode::NORMAL)
{
	this->AddNodeAssignement(NodeAssignment::MNT_NODEASSIGN_VALID);
	this->AddNodeAssignement(NodeAssignment::NMT_NODEASSIGN_NODE_EXISTS);
	this->AddNodeAssignement(NodeAssignment::NMT_NODEASSIGN_NODE_IS_CN);
	this->AddNodeAssignement(NodeAssignment::NMT_NODEASSIGN_START_CN);
}

ControlledNode::~ControlledNode()
{}

bool ControlledNode::AddNodeAssignement(NodeAssignment assign)
{

	if (assign == NodeAssignment::NMT_NODEASSIGN_MN_PRES)
	{
		return false;
	}
	else
	{
		auto it = find(this->GetNodeAssignment().begin(), this->GetNodeAssignment().end(), assign);
		if (it == this->GetNodeAssignment().end())
			this->GetNodeAssignment().push_back(assign);
		else
			return false;

	}
	return true;
}

bool ControlledNode::RemoveNodeAssignment(NodeAssignment assign)
{
	this->GetNodeAssignment().erase(remove(this->GetNodeAssignment().begin(), this->GetNodeAssignment().end(), assign), this->GetNodeAssignment().end());
	return true;
}

uint32_t ControlledNode::GetNodeAssignmentValue()
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

Result ControlledNode::MapToFrame(BaseObject& index, uint32_t position, Direction dir)
{
	return Result();
}

Result ControlledNode::GetAllMappableObjects(std::vector<std::shared_ptr<BaseObject>>& objects, Direction dir)
{
	return Result();
}

Result ControlledNode::MapAllRxObjects()
{
	return Result();
}
Result ControlledNode::MapAllTxObjects()
{
	return Result();
}

uint32_t ControlledNode::GetConfigurationObjectCount()
{
	uint32_t count = 0;
	for (auto& object : this->GetObjectDictionary())
	{
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
			else if (subobject.second->WriteToConfiguration())
			{
				count++;
			}
		}
	}

	boost::format formatter(kMsgNodeObjectCount);
	formatter
	% (uint32_t) this->GetNodeIdentifier()
	% count;
	LOG_INFO() << formatter.str();
	return count;
}

uint32_t ControlledNode::GetConfigurationObjectSize()
{
	uint32_t size = 0;
	for (auto& object : this->GetObjectDictionary())
	{
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
			else if (subobject.second->WriteToConfiguration())
			{
				size += subobject.second->GetBitSize();
			}
		}
	}

	boost::format formatter(kMsgNodeObjectCountSize);
	formatter
	% (uint32_t) this->GetNodeIdentifier()
	% size;
	LOG_INFO() << formatter.str();
	return size;
}

Result ControlledNode::SetOperationMode(PlkOperationMode operationMode)
{
	if (operationMode == this->operationMode)
		return Result();

	if (operationMode == PlkOperationMode::NORMAL)
	{
		this->operationMode = operationMode;
		this->RemoveNodeAssignment(NodeAssignment::NMT_NODEASSIGN_MULTIPLEXED_CN);
		this->RemoveNodeAssignment(NodeAssignment::NMT_NODEASSIGN_PRES_CHAINING);
	}
	else if (operationMode == PlkOperationMode::MULTIPLEXED)
	{
		bool operationModeSupported = false;
		Result res = this->GetNetworkManagement()->GetFeatureActualValue<bool>(CNFeatureEnum::DLLCNFeatureMultiplex, operationModeSupported);
		if (!res.IsSuccessful())
			return res;

		if (operationModeSupported)
		{
			this->operationMode = operationMode;
			this->AddNodeAssignement(NodeAssignment::NMT_NODEASSIGN_MULTIPLEXED_CN);
		}
		else
		{
			return Result(ErrorCode::MULTIPLEXING_NOT_SUPPORTED);
		}

	}
	else if (operationMode == PlkOperationMode::CHAINED)
	{
		bool operationModeSupported = false;
		Result res = this->GetNetworkManagement()->GetFeatureActualValue<bool>(CNFeatureEnum::DLLCNPResChaining, operationModeSupported);
		if (!res.IsSuccessful())
			return res;

		if (operationModeSupported)
		{
			this->operationMode = operationMode;
			this->AddNodeAssignement(NodeAssignment::NMT_NODEASSIGN_PRES_CHAINING);
		}
		else
		{
			return Result(ErrorCode::CHAINING_NOT_SUPPORTED);
		}
	}
	return Result();
}

PlkOperationMode ControlledNode::GetOperationMode()
{
	return this->operationMode;
}
