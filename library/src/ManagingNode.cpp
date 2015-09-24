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
using namespace IndustrialNetwork::POWERLINK::Core::Utilities;

ManagingNode::ManagingNode(std::uint8_t nodeID, const std::string& nodeName) : BaseNode(nodeID, nodeName),
	dynamicChannelList(std::vector<std::shared_ptr<DynamicChannel>>()),
	rmnList(std::vector<std::uint16_t>())
{
	//this->AddNodeAssignment(NodeAssignment::MNT_NODEASSIGN_VALID);
	//this->AddNodeAssignment(NodeAssignment::NMT_NODEASSIGN_NODE_EXISTS);

	//if (nodeID != 240) //Add assignments for RMNs only
	//{
	//AddNodeAssignment(NodeAssignment::NMT_NODEASSIGN_NODE_IS_CN);
	//}
}

ManagingNode::~ManagingNode()
{
	this->dynamicChannelList.clear();
}

Result ManagingNode::AddNodeAssignment(const NodeAssignment& assign)
{
	switch (assign)
	{
		case NodeAssignment::NMT_NODEASSIGN_START_CN:
		case NodeAssignment::NMT_NODEASSIGN_MANDATORY_CN:
		case NodeAssignment::NMT_NODEASSIGN_KEEPALIVE:
		case NodeAssignment::NMT_NODEASSIGN_SWVERSIONCHECK:
		case NodeAssignment::NMT_NODEASSIGN_SWUPDATE:
		case NodeAssignment::NMT_NODEASSIGN_MULTIPLEXED_CN:
			{
				boost::format formatter(kMsgNodeAssignmentNotSupported);
				formatter
				% (std::uint32_t) assign
				% (std::uint32_t) this->GetNodeId();
				LOG_ERROR() << formatter.str();
				return Result(ErrorCode::NODE_ASSIGNMENT_NOT_SUPPORTED, formatter.str());
			}
		case NodeAssignment::MNT_NODEASSIGN_VALID:
		case NodeAssignment::NMT_NODEASSIGN_NODE_EXISTS:
		case NodeAssignment::NMT_NODEASSIGN_NODE_IS_CN:
		case NodeAssignment::NMT_NODEASSIGN_ASYNCONLY_NODE:
		case NodeAssignment::NMT_NODEASSIGN_RT1:
		case NodeAssignment::NMT_NODEASSIGN_RT2:
		case NodeAssignment::NMT_NODEASSIGN_MN_PRES:
			{
				auto it = std::find(this->GetNodeAssignment().begin(), this->GetNodeAssignment().end(), assign);
				if (it == this->GetNodeAssignment().end())
				{
					this->GetNodeAssignment().push_back(assign);
					return SetSubObjectActualValue(0x1F81, (std::uint32_t) this->GetNodeId(), IntToHex<std::uint32_t>(this->GetNodeAssignmentValue(), 0, "0x"));
				}
				else
				{
					boost::format formatter(kMsgNodeAssignmentAlreadyExists);
					formatter
					% (std::uint32_t) assign
					% (std::uint32_t) this->GetNodeId();
					LOG_INFO() << formatter.str();
					return Result();
				}
				break;
			}
		default:
			break;
	}
	return Result();
}

Result ManagingNode::RemoveNodeAssignment(const NodeAssignment& assign)
{
	this->GetNodeAssignment().erase(std::remove(this->GetNodeAssignment().begin(), this->GetNodeAssignment().end(), assign), this->GetNodeAssignment().end());
	return SetSubObjectActualValue(0x1F81, (std::uint32_t) this->GetNodeId(), IntToHex<std::uint32_t>(this->GetNodeAssignmentValue(), 0, "0x"));
}

std::uint32_t ManagingNode::GetNodeAssignmentValue()
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

Result ManagingNode::GetDynamicChannel(PlkDataType dataType, Direction dir, std::shared_ptr<DynamicChannel>& returnChannel)
{
	for (auto& dynamicChannel : this->dynamicChannelList)
	{
		if (dynamicChannel->GetDataType() == dataType)
		{
			if (dir == Direction::RX &&
			        (dynamicChannel->GetAccessType() == DynamicChannelAccessType::readWriteOutput
			         || dynamicChannel->GetAccessType() == DynamicChannelAccessType::writeOnly))
			{
				returnChannel = dynamicChannel;
				return Result();
			}
			else if (dir == Direction::TX &&
			         (dynamicChannel->GetAccessType() == DynamicChannelAccessType::readOnly
			          || dynamicChannel->GetAccessType() == DynamicChannelAccessType::readWriteOutput))
			{
				returnChannel = dynamicChannel;
				return Result();
			}
		}
	}

	boost::format formatter(kMsgDynamicChannelNotFound);
	formatter
	% GetPlkDataTypeName(dataType)
	% DirectionTypeValues[(std::uint8_t) dir];
	LOG_ERROR() << formatter.str();
	return Result(ErrorCode::DYNAMIC_CHANNEL_NOT_FOUND, formatter.str());
}

const std::vector<std::shared_ptr<DynamicChannel>>& ManagingNode::GetDynamicChannels()
{
	return this->dynamicChannelList;
}

std::uint32_t ManagingNode::GetConfigurationObjectCount()
{
	std::uint32_t count = 0;
	for (auto& object : this->GetObjectDictionary())
	{
		if (object.second->GetObjectType() == ObjectType::VAR && object.second->WriteToConfiguration())
		{
			count++;
		}

		std::uint32_t mappingObjNrOfEntries = 0;
		std::uint32_t mappingObjCount = 0;
		for (auto& subobject : object.second->GetSubObjectDictionary())
		{
			if (object.first >= 0x1600 && object.first < 0x1700) //Count for reset and actual NrOfEntries
			{
				if (subobject.second->WriteToConfiguration() && subobject.first == 0x0)
				{
					count += 2; //Add count for mapping set and reset
					mappingObjNrOfEntries = subobject.second->GetTypedActualValue<std::uint16_t>(); //Set actual nr of mapping objects
				}
				else if (subobject.second->WriteToConfiguration() && mappingObjCount < mappingObjNrOfEntries) //Only count mapping objects of they are activated
				{
					count++;
					mappingObjCount++;
				}
			}
			else if (object.first >= 0x1A00 && object.first < 0x1B00) //Count for reset and actual NrOfEntries
			{
				if (subobject.second->WriteToConfiguration() && subobject.first == 0x0)
				{
					count += 2; //Add count for mapping set and reset
					mappingObjNrOfEntries = subobject.second->GetTypedActualValue<std::uint16_t>(); //Set actual nr of mapping objects
				}
				else if (subobject.second->WriteToConfiguration() && mappingObjCount < mappingObjNrOfEntries) //Only count mapping objects of they are activated
				{
					count++;
					mappingObjCount++;
				}
			}
			else if (object.first == 0x1F81 && subobject.first != 0x0) //Count for node assignment and reassignment
			{
				if (subobject.second->WriteToConfiguration())
				{
					if (subobject.first == 240)
					{
						if (std::find(this->GetNodeAssignment().begin(), this->GetNodeAssignment().end(), NodeAssignment::NMT_NODEASSIGN_MN_PRES) != this->GetNodeAssignment().end())
						{
							count++; //For managing node assignement on MN or RMN calculate only one object
						}
					}
					else
					{
						count += 2; //Add assignment and reassignment count
						if (this->GetNodeId() == 240) //Count 1F22 only for active managing node
							count++;
					}
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
	% (std::uint32_t) this->GetNodeId()
	% count;
	LOG_INFO() << formatter.str();
	return count;
}

std::uint32_t ManagingNode::GetConfigurationObjectSize()
{
	std::uint32_t size = 0;
	for (auto& object : this->GetObjectDictionary())
	{
		if (object.second->GetObjectType() == ObjectType::VAR && object.second->WriteToConfiguration())
		{
			size += object.second->GetBitSize();
		}

		std::uint32_t mappingObjNrOfEntries = 0;
		std::uint32_t mappingObjCount = 0;
		for (auto& subobject : object.second->GetSubObjectDictionary())
		{
			if (object.first >= 0x1600 && object.first < 0x1700) //Count for reset and actual NrOfEntries
			{
				if (subobject.second->WriteToConfiguration() && subobject.first == 0x0)
				{
					size += 2 * subobject.second->GetBitSize(); //Add size of NrOfEntries set and reset
					mappingObjNrOfEntries = subobject.second->GetTypedActualValue<std::uint16_t>(); //Set actual nr of mapping objects
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
					mappingObjNrOfEntries = subobject.second->GetTypedActualValue<std::uint16_t>(); //Set actual nr of mapping objects
				}
				else if (subobject.second->WriteToConfiguration() && mappingObjCount < mappingObjNrOfEntries) //Only count mapping objects of they are activated
				{
					size += subobject.second->GetBitSize();
					mappingObjCount++;
				}
			}
			else if (object.first == 0x1F81 && subobject.first != 0x0) //Size for node assignment and reassignment
			{
				if (subobject.second->WriteToConfiguration())
				{
					if (subobject.first == 240)
					{
						if (std::find(this->GetNodeAssignment().begin(), this->GetNodeAssignment().end(), NodeAssignment::NMT_NODEASSIGN_MN_PRES) != this->GetNodeAssignment().end())
						{
							size += subobject.second->GetBitSize(); //For managing node assignment on MN or RMN calculate only one object
						}
					}
					else
					{
						size += 2 * subobject.second->GetBitSize();
						if (this->GetNodeId() == 240) //Count 1F22 only for active managing node
							size += subobject.second->GetBitSize();
					}
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
	% (std::uint32_t) this->GetNodeId()
	% size;
	LOG_INFO() << formatter.str();
	return size;
}

std::uint16_t ManagingNode::GetRmnCount() const
{
	return (std::uint16_t) this->rmnList.size();
}

void ManagingNode::AddRmnId(std::uint16_t id)
{
	this->rmnList.push_back(id);
	//Sort and remove duplicates
	std::sort(rmnList.begin(), rmnList.end());
	rmnList.erase(std::unique(rmnList.begin(), rmnList.end()), rmnList.end());
}

void ManagingNode::ClearRmnList()
{
	this->rmnList.clear();
}

void ManagingNode::RemoveRmnId(std::uint16_t nodeId)
{
	this->rmnList.erase(std::remove(this->rmnList.begin(), this->rmnList.end(), nodeId), this->rmnList.end());
}

const std::vector<std::uint16_t>& ManagingNode::GetRmnList()
{
	return this->rmnList;
}

Result ManagingNode::SetMultiplexedCycle(const std::uint8_t nodeID, const std::uint8_t multiplexedCycle)
{
	std::shared_ptr<SubObject> multplCycleCount;
	Result res = this->GetSubObject(0x1F98, 0x7, multplCycleCount);
	if (!res.IsSuccessful())
		return res;

	std::uint16_t cycle_count = 0;
	if (multplCycleCount->WriteToConfiguration())
		cycle_count = multplCycleCount->GetTypedActualValue<std::uint16_t>();
	else
		cycle_count = multplCycleCount->GetTypedDefaultValue<std::uint16_t>();

	if (multiplexedCycle > cycle_count)
	{
		boost::format formatter(kMsgMultiplexCycleAssignInvalid);
		formatter
		% (std::uint32_t) multiplexedCycle
		% (std::uint32_t) nodeID
		% cycle_count;
		LOG_ERROR() << formatter.str();
		return Result(ErrorCode::MULTIPLEX_CYCLE_ASSIGN_INVALID, formatter.str());
	}

	std::shared_ptr<SubObject> multplCycleAssign;
	res = this->GetSubObject(0x1F9B, nodeID, multplCycleAssign);
	if (!res.IsSuccessful())
		return res;

	if (MultiplexedCycleAlreadyAssigned(multiplexedCycle))
	{
		boost::format formatter(kMsgMultiplexCycleAlreadyAssigned);
		formatter
		% (std::uint32_t) multiplexedCycle;
		LOG_ERROR() << formatter.str();
		return Result(ErrorCode::MULTIPLEX_CYCLE_ASSIGN_INVALID, formatter.str());
	}
	return multplCycleAssign->SetTypedObjectActualValue(IntToHex((std::uint32_t) multiplexedCycle, 2, "0x"));
}

Result ManagingNode::ResetMultiplexedCycle(const std::uint8_t nodeID)
{
	std::shared_ptr<SubObject> multplCycleAssign;
	Result res = this->GetSubObject(0x1F9B, nodeID, multplCycleAssign);
	if (!res.IsSuccessful())
		return res;

	multplCycleAssign->ClearActualValue();
	return Result();
}

bool ManagingNode::MultiplexedCycleAlreadyAssigned(std::uint8_t multiplexedCycle)
{
	std::shared_ptr<Object> multplCycleAssign;
	this->GetObject(0x1F9B, multplCycleAssign);
	for (auto& cycleAssign : multplCycleAssign->GetSubObjectDictionary())
	{
		if (cycleAssign.first == 0)
			continue;

		if (cycleAssign.second->WriteToConfiguration())
		{
			if (cycleAssign.second->GetTypedActualValue<std::uint16_t>() == multiplexedCycle)
			{
				return true;
			}
		}
	}
	return false;
}

IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result ManagingNode::CalculatePReqPayloadLimit()
{
	if (this->GetNodeId() != 240) //Only calculate for RMN
		return Result();

	std::uint32_t preqPayloadLimit = 0;
	for (auto& object : this->GetObjectDictionary())
	{
		if (object.first >= 0x1600 && object.first < 0x1700)
		{
			//Check corresponding 0x14XX object for origin of Rx Data
			std::uint32_t commParamIndex = (object.first - 0x1600) + 0x1400;
			std::shared_ptr<SubObject> paramObj;
			Result res = this->GetSubObject(commParamIndex, 0x1, paramObj);
			if (!res.IsSuccessful())
				return res; //0x14XX/ 0x1 NodeID_U8 does not exist

			if (paramObj->WriteToConfiguration())
				continue; //Cross traffic does not count to PreqPayloadLimit

			std::uint16_t numberOfIndicesToWrite = 0;
			auto& nrOfEntriesObj = object.second->GetSubObjectDictionary().at((std::uint8_t) 0); //GetNrOfEntries and only count the valid ones
			if (nrOfEntriesObj->WriteToConfiguration())
				numberOfIndicesToWrite = nrOfEntriesObj->GetTypedActualValue<std::uint16_t>();

			std::uint16_t count = 0;

			for (auto& subobject : object.second->GetSubObjectDictionary())
			{
				if (subobject.second->WriteToConfiguration() && subobject.first != 0 && count < numberOfIndicesToWrite)
				{
					BaseProcessDataMapping mapping = BaseProcessDataMapping(subobject.second->GetTypedActualValue<std::string>(), this->GetNodeId());
					preqPayloadLimit += mapping.GetMappingLength() / 8;
					count++;
				}
			}
		}
	}

	if (preqPayloadLimit <= 36)
		return this->SetSubObjectActualValue(0x1F98, 0x4, "36"); //Set to default value
	else
		return this->SetSubObjectActualValue(0x1F98, 0x4, IntToHex(preqPayloadLimit, 4, "0x"));
}

IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result ManagingNode::CalculatePResPayloadLimit()
{
	std::uint32_t presPayloadLimit = 0;
	std::uint32_t mappingObjectIndex = 0x1A00;
	std::uint32_t mappingParameterIndex = 0x1800;
	std::uint16_t mappedFromNode = 0;

	for (auto& obj : this->GetObjectDictionary())
	{
		if (obj.first >= mappingParameterIndex && obj.first < (mappingParameterIndex + 0x100))
		{
			//Get mapping parameter object
			std::shared_ptr<SubObject> nodeID;
			Result res = obj.second->GetSubObject(0x1, nodeID);
			if (!res.IsSuccessful())
				continue;

			if (nodeID->HasActualValue())
			{
				if (nodeID->GetTypedActualValue<std::uint16_t>() != mappedFromNode) //not PResMN
					continue;
			}
			else
				continue;

			//Get according mapping object
			std::shared_ptr<Object> mappingObject;
			res = this->GetObject(((obj.first - mappingParameterIndex) + mappingObjectIndex), mappingObject);
			if (!res.IsSuccessful())
				return res;

			//Get mapping nrOfEntries
			std::shared_ptr<SubObject> nrOfEntriesObj;
			res = mappingObject->GetSubObject(0x0, nrOfEntriesObj);
			if (!res.IsSuccessful())
				return res;

			std::uint16_t numberOfIndicesToWrite = 0;
			if (nrOfEntriesObj->WriteToConfiguration())
				numberOfIndicesToWrite = nrOfEntriesObj->GetTypedActualValue<std::uint16_t>();
			else if (nrOfEntriesObj->HasDefaultValue())
				numberOfIndicesToWrite = nrOfEntriesObj->GetTypedDefaultValue<std::uint16_t>();

			for (auto& subobject : mappingObject->GetSubObjectDictionary())
			{
				if (subobject.second->WriteToConfiguration() && subobject.first != 0 && numberOfIndicesToWrite != 0)
				{
					BaseProcessDataMapping mapping = BaseProcessDataMapping(subobject.second->GetTypedActualValue<std::string>(), this->GetNodeId());
					presPayloadLimit += mapping.GetMappingLength() / 8;
					numberOfIndicesToWrite--;
				}
			}
		}
	}
	if (presPayloadLimit <= 36)
		return this->SetSubObjectActualValue(0x1F98, 0x5, "36");
	else
		return this->SetSubObjectActualValue(0x1F98, 0x5, IntToHex(presPayloadLimit, 4, "0x"));
}

Result ManagingNode::MapObject(std::uint32_t, Direction, std::uint16_t, std::uint32_t, std::uint16_t, bool)
{
	return Result();
}

Result ManagingNode::MapSubObject(std::uint32_t, std::uint16_t, Direction, std::uint16_t, std::uint32_t, std::uint16_t, bool)
{
	return Result();
}

Result ManagingNode::MapAllRxObjects(std::uint16_t, bool)
{
	return Result();
}

Result ManagingNode::MapAllTxObjects(std::uint16_t, bool)
{
	return Result();
}

Result ManagingNode::UpdateProcessImage(const std::map<std::uint8_t, std::shared_ptr<BaseNode>>& nodeCollection, Direction dir)
{
	if (dir == Direction::RX)
		this->GetReceiveProcessImage().clear();
	else if (dir == Direction::TX)
		this->GetTransmitProcessImage().clear();

	return this->UpdateProcessDataMapping(nodeCollection, dir);
}

IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result ManagingNode::UpdateProcessDataMapping(const std::map<std::uint8_t, std::shared_ptr<BaseNode>>& nodeCollection, Direction dir)
{
	std::uint32_t mappingParameterIndex = 0;
	std::uint32_t mappingObjectIndex = 0;
	std::uint16_t mappedFromNode = 0;
	std::uint16_t nrOfEntries = 0;
	std::vector<std::shared_ptr<SubObject>> presMn;
	bool presMNProcessed = false;

	if (dir == Direction::RX)
	{
		this->GetReceiveMapping().clear();
		mappingParameterIndex = 0x1400;
		mappingObjectIndex = 0x1600;
	}
	else if (dir == Direction::TX)
	{
		this->GetTransmitMapping().clear();
		mappingParameterIndex = 0x1800;
		mappingObjectIndex = 0x1A00;

		//Check for PResMN
		for (auto& obj : this->GetObjectDictionary())
		{
			if (obj.first >= mappingParameterIndex && obj.first < (mappingParameterIndex + 0x100))
			{
				//Get mapping parameter object
				std::shared_ptr<SubObject> nodeID;
				Result res = obj.second->GetSubObject(0x1, nodeID);
				if (!res.IsSuccessful())
					continue;

				if (nodeID->HasActualValue())
					mappedFromNode = nodeID->GetTypedActualValue<std::uint16_t>();
				else
					continue;

				if (mappedFromNode != 0)
					continue;

				//Get according mapping object
				std::shared_ptr<Object> mappingObject;
				res = this->GetObject(((obj.first - mappingParameterIndex) + mappingObjectIndex), mappingObject);
				if (!res.IsSuccessful())
					return res;

				for (auto& mappingObj : mappingObject->GetSubObjectDictionary())
				{
					if (mappingObj.first == 0)
						continue;

					presMn.push_back(mappingObj.second);
				}
			}
		}
	}

	for (auto& obj : this->GetObjectDictionary())
	{
		if (obj.first >= mappingParameterIndex && obj.first < (mappingParameterIndex + 0x100))
		{
			//Get mapping parameter object
			std::shared_ptr<SubObject> nodeID;
			Result res = obj.second->GetSubObject(0x1, nodeID);
			if (!res.IsSuccessful())
				continue;

			if (nodeID->HasActualValue())
				mappedFromNode = nodeID->GetTypedActualValue<std::uint16_t>();
			else
				continue;

			//Get according mapping object
			std::shared_ptr<Object> mappingObject;
			res = this->GetObject(((obj.first - mappingParameterIndex) + mappingObjectIndex), mappingObject);
			if (!res.IsSuccessful())
				return res;

			//Get mapping nrOfEntries
			std::shared_ptr<SubObject> nrOfEntriesObj;
			res = mappingObject->GetSubObject(0x0, nrOfEntriesObj);
			if (!res.IsSuccessful())
				return res;

			if (nrOfEntriesObj->WriteToConfiguration())
				nrOfEntries = nrOfEntriesObj->GetTypedActualValue<std::uint16_t>();
			else if (nrOfEntriesObj->HasDefaultValue())
				nrOfEntries = nrOfEntriesObj->GetTypedDefaultValue<std::uint16_t>();

			//No enabled mappings
			if (nrOfEntries == 0)
				continue;

			//PResMN - process all channels at once
			if (mappedFromNode == 0 && dir == Direction::TX && presMNProcessed == false)
			{
				for (auto& node : nodeCollection)
				{
					if (node.second->IsEnabled() == false)
						continue;

					const std::shared_ptr<ControlledNode>& cn = std::dynamic_pointer_cast<ControlledNode>(node.second);
					if (cn == NULL) //Node is not a CN
						continue;

					for (auto& mapping : presMn)
					{
						if (mapping->HasActualValue())
						{
							if (mapping->GetTypedActualValue<std::uint64_t>() != 0)
							{
								std::shared_ptr<BaseProcessDataMapping> mappingPtr = std::shared_ptr<BaseProcessDataMapping>(new BaseProcessDataMapping(node.first,
								        mapping->GetObjectId(), mapping, mapping->GetTypedActualValue<std::string>(), this->GetNodeId(), false));

								if (cn->GetNodeDataPresMnOffset() > mappingPtr->GetMappingOffset())
									continue;

								if (mappingPtr->GetMappingOffset() >= cn->GetNodeDataPresMnCurrentOffset())
									break;

								mappingPtr->SetMappingOffset(mappingPtr->GetMappingOffset() - cn->GetNodeDataPresMnOffset());
								res = CheckProcessDataMapping(node.second, mappingPtr, Direction::RX);
								if (!res.IsSuccessful())
									return res;

								this->GetTransmitMapping().push_back(mappingPtr);
							}
						}
						else if (mapping->HasDefaultValue())
						{
							if (mapping->GetTypedDefaultValue<std::uint64_t>() != 0)
							{
								std::shared_ptr<BaseProcessDataMapping> mappingPtr = std::shared_ptr<BaseProcessDataMapping>(new BaseProcessDataMapping(node.first,
								        mapping->GetObjectId(), mapping, mapping->GetTypedDefaultValue<std::string>(), this->GetNodeId(), true));

								if (cn->GetNodeDataPresMnOffset() > mappingPtr->GetMappingOffset())
									continue;

								if (mappingPtr->GetMappingOffset() >= cn->GetNodeDataPresMnCurrentOffset())
									break;

								mappingPtr->SetMappingOffset(mappingPtr->GetMappingOffset() - cn->GetNodeDataPresMnOffset());
								res = CheckProcessDataMapping(node.second, mappingPtr, Direction::RX);
								if (!res.IsSuccessful())
									return res;

								this->GetTransmitMapping().push_back(mappingPtr);
							}
						}
					}
				}
				presMNProcessed = true;
			}
			else
			{
				//Node mapped does not longer exist
				if (nodeCollection.find((std::uint8_t) mappedFromNode) == nodeCollection.end())
				{
					for (auto& mapping : mappingObject->GetSubObjectDictionary())
					{
						mapping.second->ClearActualValue(); //delete mapping actual values
						nodeID->ClearActualValue(); //clear node id mapping parameter
					}
					continue;
				}

				for (auto& mapping : mappingObject->GetSubObjectDictionary())
				{
					if (mapping.first == 0)
						continue;

					if (mapping.second->HasActualValue())
					{
						if (mapping.second->GetTypedActualValue<std::uint64_t>() != 0)
						{
							std::shared_ptr<BaseProcessDataMapping> mappingPtr = std::shared_ptr<BaseProcessDataMapping>(new BaseProcessDataMapping(mappedFromNode,
							        mapping.second->GetObjectId(), mapping.second, mapping.second->GetTypedActualValue<std::string>(), this->GetNodeId(), false));

							Direction nodeDir = Direction::RX;
							if (dir == Direction::RX)
								nodeDir = Direction::TX;

							res = CheckProcessDataMapping(nodeCollection.at((std::uint8_t) mappedFromNode), mappingPtr, nodeDir);
							if (!res.IsSuccessful())
								return res;

							if (dir == Direction::RX)
								this->GetReceiveMapping().push_back(mappingPtr);
							else if (dir == Direction::TX)
								this->GetTransmitMapping().push_back(mappingPtr);

							nrOfEntries--;
						}
					}
					else if (mapping.second->HasDefaultValue())
					{
						if (mapping.second->GetTypedDefaultValue<std::uint64_t>() != 0)
						{
							std::shared_ptr<BaseProcessDataMapping> mappingPtr = std::shared_ptr<BaseProcessDataMapping>(new BaseProcessDataMapping(mappedFromNode,
							        mapping.second->GetObjectId(), mapping.second, mapping.second->GetTypedDefaultValue<std::string>(), this->GetNodeId(), true));

							Direction nodeDir = Direction::RX;
							if (dir == Direction::RX)
								nodeDir = Direction::TX;

							res = CheckProcessDataMapping(nodeCollection.at((std::uint8_t) mappedFromNode), mappingPtr, nodeDir);
							if (!res.IsSuccessful())
								return res;

							if (dir == Direction::RX)
								this->GetReceiveMapping().push_back(mappingPtr);
							else if (dir == Direction::TX)
								this->GetTransmitMapping().push_back(mappingPtr);

							nrOfEntries--;
						}
					}
					else
					{
						return Result(ErrorCode::MAPPING_INVALID);
					}
				}
			}
		}
	}

	std::uint32_t piOffset = 0;
	std::uint32_t bitCount = 0;
	if (dir == Direction::RX)
	{
		for (auto& receivePI : this->GetReceiveProcessImage())
		{
			receivePI->SetPIOffset(piOffset);
			if (receivePI->GetDataType() == IEC_Datatype::BITSTRING || receivePI->GetDataType() == IEC_Datatype::BOOL)
			{
				bitCount += receivePI->GetSize();
				if (bitCount == 8)
				{
					piOffset++;
					bitCount = 0;
				}
			}
			else
				piOffset += receivePI->GetSize() / 8;
		}
	}
	else if (dir == Direction::TX)
	{
		for (auto& transmitPI : this->GetTransmitProcessImage())
		{
			transmitPI->SetPIOffset(piOffset);
			if (transmitPI->GetDataType() == IEC_Datatype::BITSTRING || transmitPI->GetDataType() == IEC_Datatype::BOOL)
			{
				bitCount += transmitPI->GetSize();
				if (bitCount == 8)
				{
					piOffset++;
					bitCount = 0;
				}
			}
			else
				piOffset += transmitPI->GetSize() / 8;
		}
	}
	return Result();
}

Result ManagingNode::CheckProcessDataMapping(const std::shared_ptr<BaseNode>& node, const std::shared_ptr<BaseProcessDataMapping>& mnMappingObject, Direction dir)
{
	std::uint32_t size = mnMappingObject->GetMappingLength();
	//Correct offset
	std::uint32_t channelOffset = mnMappingObject->GetMappingOffset();

	std::vector<std::shared_ptr<BaseProcessImageObject>> piCollection;
	if (dir == Direction::RX)
		piCollection = node->GetReceiveProcessImage();
	else if (dir == Direction::TX)
		piCollection = node->GetTransmitProcessImage();

	bool foundMapping = false;
	std::uint32_t fillSize = 0;
	for (auto& cnPIObject : piCollection)
	{
		if (cnPIObject->GetPiOffset() >= channelOffset / 8
		        || foundMapping) //allow Gaps
		{
			foundMapping = true;
			if (fillSize < size) // as long as size is not reached
			{
				std::shared_ptr<BaseProcessImageObject> mnPiObj = std::make_shared<BaseProcessImageObject>(
				            cnPIObject->GetName(),
				            cnPIObject->GetDataType(),
				            cnPIObject->GetPiOffset(),
				            cnPIObject->GetBitOffset().is_initialized() == true ? cnPIObject->GetBitOffset().get() : 0,
				            cnPIObject->GetSize());

				if (dir == Direction::RX)
					this->GetTransmitProcessImage().push_back(mnPiObj);
				else if (dir == Direction::TX)
					this->GetReceiveProcessImage().push_back(mnPiObj);

				fillSize += cnPIObject->GetSize();
			}
			else
			{
				break;
			}
		}
	}

	if (foundMapping)
		return Result();
	return Result(ErrorCode::MAPPING_INVALID);
}

void ManagingNode::ClearMappingObjects()
{
	for (auto& object : this->GetObjectDictionary())
	{
		if ((object.first >= 0x1800 && object.first < 0x1900)
		        || (object.first >= 0x1400 && object.first < 0x1500))
		{
			for (auto& subobject : object.second->GetSubObjectDictionary())
			{
				if (subobject.first == 1)
				{
					subobject.second->ClearActualValue(); //Clear Mapping Parameter Node Ids
					break;
				}
			}
		}
		else if ((object.first >= 0x1600 && object.first < 0x1700)
		         || (object.first >= 0x1A00 && object.first < 0x1B00))
		{
			for (auto& subobject : object.second->GetSubObjectDictionary())
			{
				subobject.second->ClearActualValue(); //Clear Mapping Objects
			}
		}
	}
}
