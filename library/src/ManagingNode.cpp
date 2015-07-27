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
	this->AddNodeAssignement(NodeAssignment::MNT_NODEASSIGN_VALID);
	this->AddNodeAssignement(NodeAssignment::NMT_NODEASSIGN_NODE_EXISTS);

	if (nodeID != 240) //Add assignments for RMNs only
	{
		AddNodeAssignement(NodeAssignment::NMT_NODEASSIGN_NODE_IS_CN);
	}
}

ManagingNode::~ManagingNode()
{
	this->dynamicChannelList.clear();
}

Result ManagingNode::AddNodeAssignement(NodeAssignment assign)
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
				% (uint32_t) assign
				% (uint32_t) this->GetNodeIdentifier();
				LOG_FATAL() << formatter.str();
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
				auto it = find(this->GetNodeAssignment().begin(), this->GetNodeAssignment().end(), assign);
				if (it == this->GetNodeAssignment().end())
				{
					this->GetNodeAssignment().push_back(assign);
					if (assign == NodeAssignment::NMT_NODEASSIGN_MN_PRES)
					{
						this->SetSubObjectActualValue(0x1F81, 240, IntToHex<uint32_t>(this->GetNodeAssignmentValue(), 0, "0x"));
					}
					return Result();
				}
				else
				{
					boost::format formatter(kMsgNodeAssignmentAlreadyExists);
					formatter
					% (uint32_t) assign
					% (uint32_t) this->GetNodeIdentifier();
					LOG_WARN() << formatter.str();
					return Result();
				}

				break;
			}
		default:
			break;
	}
	return Result();
}

Result ManagingNode::RemoveNodeAssignment(NodeAssignment assign)
{
	this->GetNodeAssignment().erase(remove(this->GetNodeAssignment().begin(), this->GetNodeAssignment().end(), assign), this->GetNodeAssignment().end());
	if (assign == NodeAssignment::NMT_NODEASSIGN_MN_PRES) //Reset the node assignement on the managing node
	{
		this->SetSubObjectActualValue(0x1F81, 240, "");
	}
	return Result();
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
	% DirectionTypeValues[(std::uint32_t) dir];
	LOG_FATAL() << formatter.str();
	return Result(ErrorCode::DYNAMIC_CHANNEL_NOT_FOUND, formatter.str());
}

const std::vector<std::shared_ptr<DynamicChannel>>& ManagingNode::GetDynamicChannels()
{
	return this->dynamicChannelList;
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
			else if (object.first == 0x1F81 && subobject.first != 0x0) //Count for node assignement and reassignment
			{
				if (subobject.second->WriteToConfiguration())
				{
					if (subobject.first == 240)
					{
						count++; //For managing node assignement on MN or RMN calculate only one object
					}
					else
					{
						count += 2; //Add assignment and reassignment count
						if (this->GetNodeIdentifier() == 240) //Count 1F22 only for active managing node
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
					if (subobject.first == 240)
					{
						size += subobject.second->GetBitSize(); //For managing node assignement on MN or RMN calculate only one object
					}
					else
					{
						size += 2 * subobject.second->GetBitSize();
						if (this->GetNodeIdentifier() == 240) //Count 1F22 only for active managing node
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
	% (uint32_t) this->GetNodeIdentifier()
	% size;
	LOG_INFO() << formatter.str();
	return size;
}

std::uint16_t ManagingNode::GetRmnCount()
{
	return (std::uint16_t) this->rmnList.size();
}

void ManagingNode::AddRmnId(std::uint16_t count)
{
	this->rmnList.push_back(count);
	//Sort and remove duplicates
	std::sort(rmnList.begin(), rmnList.end());
	rmnList.erase(std::unique(rmnList.begin(), rmnList.end()), rmnList.end());
}

void ManagingNode::RemoveRmnId(std::uint16_t nodeId)
{
	this->rmnList.erase(std::remove(this->rmnList.begin(), this->rmnList.end(), nodeId), this->rmnList.end());
}

const std::vector<std::uint16_t>& ManagingNode::GetRmnIds()
{
	return this->rmnList;
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
	{
		boost::format formatter(kMsgMultiplexCycleAssignInvalid);
		formatter
		% (uint32_t) multiplexedCycle
		% (uint32_t) nodeID
		% cycle_count;
		LOG_FATAL() << formatter.str();
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
		% (uint32_t) multiplexedCycle;
		LOG_FATAL() << formatter.str();
		return Result(ErrorCode::MULTIPLEX_CYCLE_ASSIGN_INVALID, formatter.str());
	}

	std::stringstream nodeIdStr;
	nodeIdStr << (uint32_t) multiplexedCycle;
	return multplCycleAssign->SetTypedObjectActualValue(nodeIdStr.str());
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
	for (auto& cycleAssign : multplCycleAssign->GetSubObjectCollection())
	{
		if (cycleAssign.first == 0)
			continue;

		if (cycleAssign.second->WriteToConfiguration())
		{
			if (cycleAssign.second->GetTypedActualValue<uint16_t>() == multiplexedCycle)
			{
				return true;
			}
		}
	}
	return false;
}

IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result ManagingNode::CalculatePReqPayloadLimit()
{
	if (this->GetNodeIdentifier() != 240) //Only calculate for RMN
		return Result();

	uint32_t preqPayloadLimit = 0;
	for (auto& object : this->GetObjectDictionary())
	{
		if (object.first >= 0x1600 && object.first < 0x1700)
		{
			//Check corresponding 0x14XX object for origin of Rx Data
			uint32_t commParamIndex = (object.first - 0x1600) + 0x1400;
			std::shared_ptr<SubObject> paramObj;
			Result res = this->GetSubObject(commParamIndex, 0x1, paramObj);
			if (!res.IsSuccessful())
				return res; //0x14XX/ 0x1 NodeID_U8 does not exist

			if (paramObj->WriteToConfiguration())
				continue; //Cross traffic does not count to PreqPayloadLimit

			uint16_t numberOfIndicesToWrite = 0;
			auto& nrOfEntriesObj = object.second->GetSubObjectCollection().at((uint8_t) 0); //GetNrOfEntries and only count the valid ones
			if (nrOfEntriesObj->WriteToConfiguration())
				numberOfIndicesToWrite = nrOfEntriesObj->GetTypedActualValue<uint16_t>();

			uint16_t count = 0;

			for (auto& subobject : object.second->GetSubObjectCollection())
			{
				if (subobject.second->WriteToConfiguration() && subobject.first != 0 && count < numberOfIndicesToWrite)
				{
					BaseProcessDataMapping mapping = BaseProcessDataMapping(subobject.second->GetTypedActualValue<std::string>(), this->GetNodeIdentifier());
					preqPayloadLimit += mapping.GetMappingLength() / 8;
					count++;
				}
			}
		}
	}

	if (preqPayloadLimit <= 36)
	{
		return this->SetSubObjectActualValue(0x1F98, 0x4, "36"); //Set to default value
	}
	else
	{
		std::stringstream convert;
		convert << preqPayloadLimit;
		return this->SetSubObjectActualValue(0x1F98, 0x4, convert.str());
	}
}

IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result ManagingNode::CalculatePResPayloadLimit()
{
	//Calculate only if managing node transmits PRes
	if (std::find(this->GetNodeAssignment().begin(), this->GetNodeAssignment().end(), NodeAssignment::NMT_NODEASSIGN_MN_PRES) != this->GetNodeAssignment().end())
	{
		uint32_t presPayloadLimit = 0;
		for (auto& object : this->GetObjectDictionary())
		{

			if (object.first >= 0x1A00 && object.first < 0x1B00)
			{
				uint16_t numberOfIndicesToWrite = 0;
				auto& nrOfEntriesObj = object.second->GetSubObjectCollection().at((uint8_t) 0);
				if (nrOfEntriesObj->WriteToConfiguration())
					numberOfIndicesToWrite = nrOfEntriesObj->GetTypedActualValue<uint16_t>();

				uint16_t count = 0;

				for (auto& subobject : object.second->GetSubObjectCollection())
				{
					if (subobject.second->WriteToConfiguration() && subobject.first != 0 && count < numberOfIndicesToWrite)
					{
						BaseProcessDataMapping mapping = BaseProcessDataMapping(subobject.second->GetTypedActualValue<std::string>(), this->GetNodeIdentifier());
						presPayloadLimit += mapping.GetMappingLength() / 8;
						count++;
					}
				}
			}
		}

		if (presPayloadLimit <= 36)
		{
			return this->SetSubObjectActualValue(0x1F98, 0x5, "36");
		}
		else
		{
			std::stringstream convert;
			convert << presPayloadLimit;
			return this->SetSubObjectActualValue(0x1F98, 0x5, convert.str());
		}
	}
	return Result();
}

Result ManagingNode::MapObject(std::uint32_t, Direction, std::uint32_t, std::uint16_t)
{
	return Result();
}

Result ManagingNode::MapSubObject(std::uint32_t, std::uint16_t, Direction, std::uint32_t, std::uint16_t)
{
	return Result();
}

Result ManagingNode::MapAllRxObjects(bool)
{
	return Result();
}

Result ManagingNode::MapAllTxObjects(bool)
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

IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result ManagingNode::UpdateProcessDataMapping(const std::map<uint8_t, std::shared_ptr<BaseNode>>& nodeCollection, Direction dir)
{
	std::uint32_t mappingParameterIndex = 0;
	std::uint32_t mappingObjectIndex = 0;
	std::uint16_t mappedFromNode = 0;
	std::uint16_t nrOfEntries = 0;
	bool defaultMapping = false;

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
	}

	for (auto& obj : this->GetObjectDictionary())
	{
		if (obj.first >= mappingParameterIndex && obj.first < (mappingParameterIndex + 0x100))
		{
			//Get mapping parameter object
			std::shared_ptr<SubObject> nodeID;
			Result res = obj.second->GetSubObject(0x1, nodeID);
			if (!res.IsSuccessful())
				return res;

			if (nodeID->WriteToConfiguration())
			{
				mappedFromNode = nodeID->GetTypedActualValue<uint16_t>();
			}

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
			{
				nrOfEntries = nrOfEntriesObj->GetTypedActualValue<uint16_t>();
			}
			else if (nrOfEntriesObj->HasDefaultValue())
			{
				nrOfEntries = nrOfEntriesObj->GetTypedDefaultValue<uint16_t>();
				defaultMapping = true;
			}

			for (auto& mapping : mappingObject->GetSubObjectCollection())
			{
				if (mapping.first == 0)
					continue;

				if (nrOfEntries == 0)
					break;

				if (defaultMapping && mapping.second->HasDefaultValue())
				{
					std::shared_ptr<BaseProcessDataMapping> mappingPtr = std::shared_ptr<BaseProcessDataMapping>(new BaseProcessDataMapping(mappedFromNode,
					        mapping.second->GetId(), mapping.second, mapping.second->GetTypedDefaultValue<std::string>(), this->GetNodeIdentifier(), true));

					res = CheckProcessDataMapping(nodeCollection.at((uint8_t)mappedFromNode), mappingPtr, dir);
					if (!res.IsSuccessful())
						return res;

					if (dir == Direction::RX)
						this->GetReceiveMapping().push_back(mappingPtr);
					else if (dir == Direction::TX)
						this->GetTransmitMapping().push_back(mappingPtr);

					nrOfEntries--;
				}
				else if (mapping.second->WriteToConfiguration())
				{
					std::shared_ptr<BaseProcessDataMapping> mappingPtr = std::shared_ptr<BaseProcessDataMapping>(new BaseProcessDataMapping(mappedFromNode,
					        mapping.second->GetId(), mapping.second, mapping.second->GetTypedActualValue<std::string>(), this->GetNodeIdentifier(), false));

					Direction nodeDir = Direction::RX;
					if (dir == Direction::RX)
						nodeDir = Direction::TX;

					res = CheckProcessDataMapping(nodeCollection.at((uint8_t)mappedFromNode), mappingPtr, nodeDir);
					if (!res.IsSuccessful())
						return res;

					if (dir == Direction::RX)
						this->GetReceiveMapping().push_back(mappingPtr);
					else if (dir == Direction::TX)
						this->GetTransmitMapping().push_back(mappingPtr);

					nrOfEntries--;
				}
				else
				{
					return Result(ErrorCode::MAPPING_INVALID);
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
			if (receivePI->GetDataType() == IEC_Datatype::BITSTRING)
			{
				bitCount += receivePI->GetSize();
				if (bitCount == 8)
					piOffset++;
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
			if (transmitPI->GetDataType() == IEC_Datatype::BITSTRING)
			{
				bitCount += transmitPI->GetSize();
				if (bitCount == 8)
					piOffset++;
			}
			else
				piOffset += transmitPI->GetSize() / 8;
		}
	}
	return Result();
}

Result ManagingNode::CheckProcessDataMapping(const std::shared_ptr<BaseNode>& node, const std::shared_ptr<BaseProcessDataMapping>& mnMappingObject, Direction dir)
{
	const std::shared_ptr<ControlledNode>& cn = std::dynamic_pointer_cast<ControlledNode>(node);
	if (cn.get())
	{
		cn->UpdateProcessImage(dir);
	}
	std::uint32_t size = mnMappingObject->GetMappingLength();

	std::vector<std::shared_ptr<BaseProcessImageObject>> piCollection;
	if (dir == Direction::RX)
		piCollection = node->GetReceiveProcessImage();
	else if (dir == Direction::TX)
		piCollection = node->GetTransmitProcessImage();

	//Check DynamicChannel To Be Done
	std::shared_ptr<DynamicChannel> dynChannel;

	bool foundMapping = false;
	std::uint32_t fillSize = 0;
	for (auto& cnPIObject : piCollection)
	{
		if (cnPIObject->GetPiOffset() >= mnMappingObject->GetMappingOffset() / 8
		        || foundMapping) //allow Gaps
		{
			foundMapping = true;
			if (fillSize < size) // as long as size is not reached
			{
				if (dir == Direction::RX)
					this->GetTransmitProcessImage().push_back(cnPIObject);
				else if (dir == Direction::TX)
					this->GetReceiveProcessImage().push_back(cnPIObject);

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
			for (auto& subobject : object.second->GetSubObjectCollection())
			{
				if (subobject.second->WriteToConfiguration() && subobject.first == 1)
				{
					subobject.second->ClearActualValue(); //Clear Mapping Parameter Node Ids
				}
			}
		}
		else if ((object.first >= 0x1600 && object.first < 0x1700)
		         || (object.first >= 0x1A00 && object.first < 0x1B00))
		{
			for (auto& subobject : object.second->GetSubObjectCollection())
			{
				if (subobject.second->WriteToConfiguration())
				{
					subobject.second->ClearActualValue(); //Clear Mapping Objects
				}
			}
		}
	}

}

