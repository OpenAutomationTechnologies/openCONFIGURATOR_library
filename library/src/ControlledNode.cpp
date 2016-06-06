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
using namespace IndustrialNetwork::POWERLINK::Core::Utilities;

ControlledNode::ControlledNode(std::uint8_t nodeID, const std::string& nodeName) : BaseNode(nodeID, nodeName),
	operationMode(PlkOperationMode::NORMAL),
	nodeDataPresMnOffset(0),
	nodeDataPresMnCurrentOffset(0),
	receivesPResMN(false)
{
	//this->AddNodeAssignment(NodeAssignment::MNT_NODEASSIGN_VALID);
	//this->AddNodeAssignment(NodeAssignment::NMT_NODEASSIGN_NODE_EXISTS);
	//this->AddNodeAssignment(NodeAssignment::NMT_NODEASSIGN_NODE_IS_CN);
	//this->AddNodeAssignment(NodeAssignment::NMT_NODEASSIGN_START_CN);
}

ControlledNode::~ControlledNode()
{}

Result ControlledNode::AddNodeAssignment(NodeAssignment assign)
{
	if (assign == NodeAssignment::NMT_NODEASSIGN_MN_PRES)
	{
		boost::format formatter(kMsgNodeAssignmentNotSupported);
		formatter
		% (std::uint32_t) assign
		% (std::uint32_t) this->GetNodeId();
		LOG_ERROR() << formatter.str();
		return Result(ErrorCode::NODE_ASSIGNMENT_NOT_SUPPORTED, formatter.str());
	}

	auto it = find(this->GetNodeAssignment().begin(), this->GetNodeAssignment().end(), assign);
	if (it == this->GetNodeAssignment().end())
		this->GetNodeAssignment().push_back(assign);
	else
	{
		boost::format formatter(kMsgNodeAssignmentAlreadyExists);
		formatter
		% (std::uint32_t) assign
		% (std::uint32_t) this->GetNodeId();
		LOG_INFO() << formatter.str();
		return Result();
	}
	return Result();
}

Result ControlledNode::RemoveNodeAssignment(NodeAssignment assign)
{
	this->GetNodeAssignment().erase(remove(this->GetNodeAssignment().begin(), this->GetNodeAssignment().end(), assign), this->GetNodeAssignment().end());
	return Result();
}

std::uint32_t ControlledNode::GetNodeAssignmentValue()
{
	if (this->GetNodeAssignment().empty())
		return 0;

	NodeAssignment assign = this->GetNodeAssignment()[0];
	for (auto var : this->GetNodeAssignment())
	{
		assign |= var;
	}

	return static_cast<std::underlying_type<NodeAssignment>::type>(assign);
}

Result ControlledNode::MapObject(std::uint32_t index, const Direction dir, std::uint16_t channelNr, std::uint32_t position, std::uint16_t fromNode, bool updateNrOfEntries)
{
	//Retrieve object to be mapped
	std::shared_ptr<Object> objToMap;
	Result res = this->GetObject(index, objToMap);
	if (!res.IsSuccessful())
		return res;

	//Map object
	return this->MapBaseObject(objToMap, index, 0, dir, updateNrOfEntries, channelNr, position, fromNode);
}

Result ControlledNode::MapSubObject(std::uint32_t index, std::uint16_t subindex, const Direction dir,  std::uint16_t channelNr, std::uint32_t position, std::uint16_t fromNode, bool updateNrOfEntries)
{
	//retrieve sub object to be mapped
	std::shared_ptr<SubObject> objToMap;
	Result res = this->GetSubObject(index, subindex, objToMap);
	if (!res.IsSuccessful())
		return res;

	//Map subobject
	return this->MapBaseObject(objToMap, index, subindex, dir, updateNrOfEntries, channelNr, position, fromNode);
}

Result ControlledNode::MapBaseObject(const std::shared_ptr<BaseObject>& objToMap, std::uint32_t index, std::uint16_t subindex, const Direction dir, bool updateNrOfEntries, std::uint16_t channelNr, std::uint32_t position, std::uint16_t fromNode)
{
	//Set fromNode for PresChained nodes
	if (dir == Direction::RX
	        && this->GetOperationMode() == PlkOperationMode::CHAINED)
	{
		fromNode = 240;
		receivesPResMN = true;
	}

	if (fromNode == 240)
		receivesPResMN = true;

	//Object has domain datatype
	if (objToMap->GetUniqueIdRef().is_initialized() &&
	        objToMap->GetDataType().get() == PlkDataType::Domain)
	{
		//Retrieve domain parameter
		std::shared_ptr<Parameter> param;
		Result res = this->GetApplicationProcess()->GetParameter(objToMap->GetUniqueIdRef().get(), param);
		if (!res.IsSuccessful())
			return res;

		//Check Parameter access depeding on direction
		if (dir == Direction::RX)
		{
			//Check complex data type access
			if (param->GetParameterAccess() == ParameterAccess::readWriteInput
			        || param->GetParameterAccess() == ParameterAccess::read)
			{
				//Parameter access does not match mapping direction
				boost::format formatter(kMsgAccessTypeForParameterInvalid);
				formatter
				% param->GetUniqueID()
				% index
				% subindex
				% (std::uint32_t) this->GetNodeId()
				% ParameterAccessValues[(std::uint8_t) param->GetParameterAccess()];
				LOG_ERROR() << formatter.str();
				return Result(ErrorCode::PARAMETER_ACCESS_INVALID, formatter.str());
			}
		}
		else if (dir == Direction::TX)
		{
			if (param->GetParameterAccess() == ParameterAccess::readWriteOutput
			        || param->GetParameterAccess() == ParameterAccess::write)
			{
				//Parameter access does not match mapping direction
				boost::format formatter(kMsgAccessTypeForParameterInvalid);
				formatter
				% param->GetUniqueID()
				% index
				% subindex
				% (std::uint32_t) this->GetNodeId()
				% ParameterAccessValues[(std::uint8_t) param->GetParameterAccess()];
				LOG_ERROR() << formatter.str();
				return Result(ErrorCode::PARAMETER_ACCESS_INVALID, formatter.str());
			}
		}
	}

	//Check PDOMapping
	if (objToMap->GetPDOMapping().is_initialized())
	{
		if (dir == Direction::RX)
		{
			if (objToMap->GetPDOMapping().get() != PDOMapping::RPDO
			        && objToMap->GetPDOMapping().get() != PDOMapping::OPTIONAL
			        && objToMap->GetPDOMapping().get() != PDOMapping::DEFAULT)
			{
				boost::format formatter(kMsgMappingTypeForPdoInvalid);
				formatter
				% index
				% subindex
				% (std::uint32_t) this->GetNodeId()
				% PDOMappingValues[(std::uint8_t) objToMap->GetPDOMapping().get()];
				LOG_ERROR() << formatter.str();
				return Result(ErrorCode::MAPPING_TYPE_FOR_PDO_INVALID, formatter.str());
			}
		}
		else if (dir == Direction::TX)
		{
			if (objToMap->GetPDOMapping().get() != PDOMapping::TPDO
			        && objToMap->GetPDOMapping().get() != PDOMapping::OPTIONAL
			        && objToMap->GetPDOMapping().get() != PDOMapping::DEFAULT)
			{
				boost::format formatter(kMsgMappingTypeForPdoInvalid);
				formatter
				% index
				% subindex
				% (std::uint32_t) this->GetNodeId()
				% PDOMappingValues[(std::uint8_t) objToMap->GetPDOMapping().get()];
				LOG_ERROR() << formatter.str();
				return Result(ErrorCode::MAPPING_TYPE_FOR_PDO_INVALID, formatter.str());
			}
		}
	}
	//Every object to be mapped needs a PDOMapping attribute
	else
	{
		boost::format formatter(kMsgMappingTypeForPdoInvalid);
		formatter
		% index
		% subindex
		% (std::uint32_t) this->GetNodeId()
		% "Not defined";
		LOG_ERROR() << formatter.str();
		return Result(ErrorCode::MAPPING_TYPE_FOR_PDO_INVALID, formatter.str());
	}

	//Check AccessType
	if (objToMap->GetAccessType().is_initialized())
	{
		if (dir == Direction::RX)
		{
			if (objToMap->GetAccessType().get() != AccessType::WO
			        && objToMap->GetAccessType().get() != AccessType::WOS
			        && objToMap->GetAccessType().get() != AccessType::RW)
			{
				boost::format formatter(kMsgAccessTypeForPdoInvalid);
				formatter
				% index
				% subindex
				% (std::uint32_t) this->GetNodeId()
				% AccessTypeValues[(std::uint8_t) objToMap->GetAccessType().get()];
				LOG_ERROR() << formatter.str();
				return Result(ErrorCode::ACCESS_TYPE_FOR_PDO_INVALID, formatter.str());
			}
		}
		else if (dir == Direction::TX)
		{
			if (objToMap->GetAccessType().get() != AccessType::RO
			        && objToMap->GetAccessType().get() != AccessType::RW
			        && objToMap->GetAccessType().get() != AccessType::RWS)
			{
				boost::format formatter(kMsgAccessTypeForPdoInvalid);
				formatter
				% index
				% subindex
				% (std::uint32_t) this->GetNodeId()
				% AccessTypeValues[(std::uint8_t) objToMap->GetAccessType().get()];
				LOG_ERROR() << formatter.str();
				return Result(ErrorCode::ACCESS_TYPE_FOR_PDO_INVALID, formatter.str());
			}
		}
	}
	//Domain object must not have an access type all the other must have an access type attribute
	else if (!objToMap->GetUniqueIdRef().is_initialized())
	{
		boost::format formatter(kMsgAccessTypeForParameterInvalid);
		formatter
		% index
		% subindex
		% (std::uint32_t) this->GetNodeId()
		% PDOMappingValues[(std::uint8_t) objToMap->GetPDOMapping().get()];
		LOG_ERROR() << formatter.str();
		return Result(ErrorCode::MAPPING_TYPE_FOR_PDO_INVALID, formatter.str());
	}

	//Calculate mapping parameter
	std::uint32_t mappingObjectIndex = 0;
	std::uint32_t mappingParameterIndex = 0;

	if (dir == Direction::RX)
	{
		mappingObjectIndex = 0x1600 + channelNr;
		mappingParameterIndex = 0x1400 + channelNr;

		//Get SubObject 0x1 "NodeID" for the mapping parameter
		std::shared_ptr<SubObject> subObj;
		Result res = this->GetSubObject(mappingParameterIndex, 0x1, subObj, false);
		if (!res.IsSuccessful())
			return res;
		//Set Mapping Channel
		subObj->SetTypedObjectActualValue(IntToHex(fromNode, 2, "0x"));
	}
	else if (dir == Direction::TX)
	{
		mappingObjectIndex = 0x1A00; //usually a node only has one Tx PDO
	}

	//Get mapping object
	std::shared_ptr<Object> mappingObj;
	Result res = this->GetObject(mappingObjectIndex, mappingObj);
	if (!res.IsSuccessful())
		return res;

	std::uint16_t nrOfEntries = 0; //mapping object NrOfEntries
	std::uint16_t validMappings = 0;
	std::uint32_t offset = 0; // Mapping offset
	std::uint32_t expectedOffset = 0; //calculated mapping offset with object + datasize

	if (dir == Direction::RX
	        && (fromNode == 240 || this->GetOperationMode() == PlkOperationMode::CHAINED))
	{
		offset = this->nodeDataPresMnCurrentOffset; // start with Offset from the MN
		expectedOffset = this->nodeDataPresMnCurrentOffset;
	}

	if (mappingObj->GetSubObjectDictionary().size() < position)
	{
		return Result(ErrorCode::INSUFFICIENT_MAPPING_OBJECTS);
	}

	for (auto& tMapping : mappingObj->GetSubObjectDictionary())
	{
		//Skip the first subobject
		if (nrOfEntries == 0)
		{
			nrOfEntries++;
			continue;
		}

		//Get mapping object on destined position
		std::shared_ptr<SubObject> currentMappingObj;
		res = this->GetSubObject(mappingObjectIndex, position, currentMappingObj);
		if (!res.IsSuccessful())
			return res;

		//if traversal has reached the position
		if (nrOfEntries == position)
		{
			//there is an existing mapping retrieve the offset
			if (tMapping.second->HasActualValue())
			{
				if (tMapping.second->GetTypedActualValue<std::uint64_t>() != 0)
				{
					BaseProcessDataMapping object = BaseProcessDataMapping(tMapping.second->GetTypedActualValue<std::string>(), this->GetNodeId(), false);
					offset = object.GetMappingOffset();
				}
			}

			if (offset < expectedOffset)
				offset = expectedOffset;

			if (offset + objToMap->GetBitSize() > (1490 * 8))
			{
				boost::format formatter(kMsgIsochronousMaxPayloadExceeded);
				formatter
				% (std::uint32_t) this->GetNodeId()
				% DirectionTypeValues[(std::uint8_t) dir]
				% ((offset + objToMap->GetBitSize()) / 8);
				LOG_FATAL() << formatter.str();
				return Result(ErrorCode::CHANNEL_PAYLOAD_LIMIT_EXCEEDED, formatter.str());
			}

			//Build mapping string
			std::stringstream mappingObjStr;
			mappingObjStr << "0x";

			//Write mapping length
			mappingObjStr << std::uppercase << std::hex << std::setw(4) << std::setfill('0') << std::right << objToMap->GetBitSize();
			//Write mapping offset
			mappingObjStr << std::uppercase << std::hex << std::setw(4) << std::setfill('0') << std::right << offset;
			//Write alignment
			mappingObjStr << std::uppercase << std::hex << std::setw(2) << std::setfill('0') << "";
			//Write mapping subindex
			mappingObjStr << std::uppercase << std::hex << std::setw(2) << std::setfill('0') << std::right << subindex;
			//Write mapping index
			mappingObjStr << std::uppercase << std::hex << std::setw(4) << std::setfill('0') << std::right << index;

			//Set new actual value
			tMapping.second->SetTypedObjectActualValue(mappingObjStr.str());

			expectedOffset = offset + objToMap->GetBitSize();
			nrOfEntries++;
			validMappings++;
			continue;

		}

		// position not reached calculate the offsets
		// skip empty mapping objects
		if (tMapping.second->HasActualValue())
		{
			if (tMapping.second->GetTypedActualValue<std::uint64_t>() != 0)
			{
				BaseProcessDataMapping object = BaseProcessDataMapping(tMapping.second->GetTypedActualValue<std::string>(), this->GetNodeId(), false);

				//offset is ok go on
				if (expectedOffset >= object.GetMappingOffset())
				{
					//offset is ok write current mapping offset
					offset = object.GetMappingOffset();
					//expected offset equals current + length
					expectedOffset = offset + object.GetMappingLength();
					//nr of mapping entries increased
					nrOfEntries++;
					validMappings++;
					continue;
				}
				else
				{
					//offset wrong use expected offset
					object.SetMappingOffset(expectedOffset);
					//set offset as expected
					offset = expectedOffset;
					//calculate new expected offset
					expectedOffset = offset + object.GetMappingLength();

					//write mapping object
					tMapping.second->SetTypedObjectActualValue(object.ToString(true));
					//nr of mapping entries increased
					nrOfEntries++;
					validMappings++;
					continue;
				}
			}
		}
		nrOfEntries++;
	}

	//update nr of entries of the mapping object
	if (updateNrOfEntries)
	{
		std::shared_ptr<SubObject> nrOfEntriesObj;
		res = mappingObj->GetSubObject(0, nrOfEntriesObj); //Get Subobject NrOfEntries
		if (!res.IsSuccessful())
			return res;

		if (nrOfEntriesObj->HasActualValue())
		{
			std::uint16_t enabledMappingEntries = nrOfEntriesObj->GetTypedActualValue<std::uint16_t>();
			if (enabledMappingEntries > validMappings)
			{
				boost::format formatter(kMsgNrOfEntriesInvalid);
				formatter
				% mappingObj->GetObjectId()
				% (std::uint32_t) this->GetNodeId()
				% enabledMappingEntries
				% nrOfEntries;
				LOG_WARN() << formatter.str();
			}
		}

		nrOfEntriesObj->SetTypedObjectActualValue(IntToHex((std::uint16_t) validMappings, 2, "0x"));
	}

	return this->UpdateProcessImage(dir);
}

Result ControlledNode::MapAllRxObjects(std::uint16_t channelNr, bool updateNrOfEntries)
{
	//start on position 1
	std::uint32_t position = 1;

	for (auto& obj : this->GetObjectDictionary())
	{
		if (obj.second->GetPDOMapping().is_initialized())
		{
			if ((obj.second->GetPDOMapping() == PDOMapping::RPDO || obj.second->GetPDOMapping() == PDOMapping::DEFAULT || obj.second->GetPDOMapping() == PDOMapping::OPTIONAL)
			        && (obj.second->GetAccessType() == AccessType::RW || obj.second->GetAccessType() == AccessType::WO))
			{
				Result res = this->MapBaseObject(obj.second, obj.first, 0, Direction::RX, updateNrOfEntries, channelNr, position, 0);
				if (!res.IsSuccessful())
					return res;
				position++;
			}
		}

		for (auto& subobj : obj.second->GetSubObjectDictionary())
		{
			if (!subobj.second->GetPDOMapping().is_initialized())
				continue;

			if ((subobj.second->GetPDOMapping() == PDOMapping::RPDO || subobj.second->GetPDOMapping() == PDOMapping::DEFAULT || subobj.second->GetPDOMapping() == PDOMapping::OPTIONAL)
			        && (subobj.second->GetAccessType() == AccessType::RW || subobj.second->GetAccessType() == AccessType::WO))
			{
				Result res = this->MapBaseObject(subobj.second, obj.first, (std::uint16_t) subobj.first, Direction::RX, updateNrOfEntries, channelNr, position, 0);
				if (!res.IsSuccessful())
					return res;
				position++;
			}
		}
	}
	return Result();
}

Result ControlledNode::MapAllTxObjects(std::uint16_t channelNr,  bool updateNrOfEntries)
{
	//start on position 1
	std::uint32_t position = 1;

	for (auto& obj : this->GetObjectDictionary())
	{
		if (obj.second->GetPDOMapping().is_initialized())
		{
			if ((obj.second->GetPDOMapping() == PDOMapping::TPDO || obj.second->GetPDOMapping() == PDOMapping::DEFAULT || obj.second->GetPDOMapping() == PDOMapping::OPTIONAL)
			        && (obj.second->GetAccessType() == AccessType::RW || obj.second->GetAccessType() == AccessType::RO))
			{
				Result res = this->MapBaseObject(obj.second, obj.first, 0, Direction::TX, updateNrOfEntries, channelNr, position, 0);
				if (!res.IsSuccessful())
					return res;
				position++;
			}
		}

		for (auto& subobj : obj.second->GetSubObjectDictionary())
		{
			if (!subobj.second->GetPDOMapping().is_initialized())
				continue;

			if ((subobj.second->GetPDOMapping() == PDOMapping::TPDO || subobj.second->GetPDOMapping() == PDOMapping::DEFAULT || subobj.second->GetPDOMapping() == PDOMapping::OPTIONAL)
			        && (subobj.second->GetAccessType() == AccessType::RW || subobj.second->GetAccessType() == AccessType::RO))
			{
				Result res = this->MapBaseObject(subobj.second, obj.first, (std::uint16_t) subobj.first, Direction::TX, updateNrOfEntries, channelNr, position, 0);
				if (!res.IsSuccessful())
					return res;
				position ++;
			}
		}
	}
	return Result();
}

std::uint32_t ControlledNode::GetConfigurationObjectCount()
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
			if ((object.first >= 0x1600 && object.first < 0x1700)
			        || (object.first >= 0x1A00 && object.first < 0x1B00)) //Count for reset and actual NrOfEntries
			{
				if (subobject.first == 0x0)
				{
					if (subobject.second->WriteToConfiguration())
					{
						count += 2; //Add count for mapping set and reset
						mappingObjNrOfEntries = subobject.second->GetTypedActualValue<std::uint16_t>(); //Set actual nr of mapping objects
					}
					/*else if (subobject.second->HasDefaultValue())
					{
						if (subobject.second->GetTypedDefaultValue<std::uint16_t>() != 0)
						{
							count += 2; //Add count for mapping set and reset
							mappingObjNrOfEntries = subobject.second->GetTypedDefaultValue<std::uint16_t>(); //Set actual nr of mapping objects
						}
					}*/
				}
				else if (mappingObjCount < mappingObjNrOfEntries)
				{
					if (subobject.second->WriteToConfiguration()) //Only count mapping objects of they are activated
					{
						count++;
						mappingObjCount++;
					}
					/*else if (subobject.second->HasDefaultValue())
					{
						if (subobject.second->GetTypedDefaultValue<std::uint64_t>() != 0)
						{
							count++;
							mappingObjCount++;
						}
					}*/
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
	% (std::uint32_t) this->GetNodeId()
	% count;
	LOG_INFO() << formatter.str();
	return count;
}

std::uint32_t ControlledNode::GetConfigurationObjectSize()
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
			if ((object.first >= 0x1600 && object.first < 0x1700)
			        || (object.first >= 0x1A00 && object.first < 0x1B00)) //Count for reset and actual NrOfEntries
			{
				if (subobject.first == 0x0)
				{
					if (subobject.second->WriteToConfiguration())
					{
						size += 2 * subobject.second->GetBitSize(); //Add size of NrOfEntries set and reset
						mappingObjNrOfEntries = subobject.second->GetTypedActualValue<uint16_t>(); //Set actual nr of mapping objects
					}
					/*else if (subobject.second->HasDefaultValue())
					{
						if (subobject.second->GetTypedDefaultValue<std::uint16_t>() != 0)
						{
							size += 2 * subobject.second->GetBitSize(); //Add size of NrOfEntries set and reset
							mappingObjNrOfEntries = subobject.second->GetTypedDefaultValue<uint16_t>(); //Set actual nr of mapping objects
						}
					}*/
				}
				else if (mappingObjCount < mappingObjNrOfEntries)
				{
					if (subobject.second->WriteToConfiguration()) //Only count mapping objects of they are activated
					{
						size += subobject.second->GetBitSize();
						mappingObjCount++;
					}
					/*else if (subobject.second->HasDefaultValue())
					{
						if (subobject.second->GetTypedDefaultValue<std::uint64_t>() != 0)
						{
							size += subobject.second->GetBitSize();
							mappingObjCount++;
						}
					}*/
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
	% (std::uint32_t) this->GetNodeId()
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

		this->RemoveNodeAssignment(NodeAssignment::NMT_NODEASSIGN_MULTIPLEXED_CN);
		this->RemoveNodeAssignment(NodeAssignment::NMT_NODEASSIGN_PRES_CHAINING);

		this->operationMode = operationMode;
	}
	else if (operationMode == PlkOperationMode::MULTIPLEXED)
	{
		bool operationModeSupported = false;
		this->GetNetworkManagement()->GetFeatureActualValue<bool>(CNFeatureEnum::DLLCNFeatureMultiplex, operationModeSupported);

		if (operationModeSupported)
		{
			this->operationMode = operationMode;
			this->AddNodeAssignment(NodeAssignment::NMT_NODEASSIGN_MULTIPLEXED_CN);
		}
		else
		{
			boost::format formatter(kMsgMultiplexingNotSupported);
			formatter
			% (std::uint32_t) this->GetNodeId();
			LOG_ERROR() << formatter.str();
			return Result(ErrorCode::MULTIPLEXING_NOT_SUPPORTED, formatter.str());
		}

	}
	else if (operationMode == PlkOperationMode::CHAINED)
	{
		bool operationModeSupported = false;
		this->GetNetworkManagement()->GetFeatureActualValue<bool>(CNFeatureEnum::DLLCNPResChaining, operationModeSupported);

		if (operationModeSupported)
		{
			this->operationMode = operationMode;
			this->AddNodeAssignment(NodeAssignment::NMT_NODEASSIGN_PRES_CHAINING);
			this->UpdateProcessImage(Direction::RX);
		}
		else
		{
			boost::format formatter(kMsgChainingNotSupported);
			formatter
			% (std::uint32_t) this->GetNodeId();
			LOG_ERROR() << formatter.str();
			return Result(ErrorCode::CHAINING_NOT_SUPPORTED, formatter.str());
		}
	}
	return Result();
}

PlkOperationMode ControlledNode::GetOperationMode()
{
	return this->operationMode;
}

IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result ControlledNode::CalculatePReqPayloadLimit()
{
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
			auto& nrOfEntriesObj = object.second->GetSubObjectDictionary().at((std::uint16_t) 0); //GetNrOfEntries and only count the valid ones
			if (nrOfEntriesObj->WriteToConfiguration())
				numberOfIndicesToWrite = nrOfEntriesObj->GetTypedActualValue<std::uint16_t>();

			std::uint16_t count = 0;

			for (auto& subobject : object.second->GetSubObjectDictionary())
			{
				if (subobject.second->WriteToConfiguration() && subobject.first != 0 && count < numberOfIndicesToWrite)
				{
					BaseProcessDataMapping mapping = BaseProcessDataMapping(subobject.second->GetTypedActualValue<std::string>(), this->GetNodeId());
					preqPayloadLimit += mapping.GetMappingLength() / 8; //Byte Size
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
		return this->SetSubObjectActualValue(0x1F98, 0x4, IntToHex(preqPayloadLimit, 4, "0x"));
	}
}

IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result ControlledNode::CalculatePResPayloadLimit()
{
	std::uint32_t presPayloadLimit = 0;
	for (auto& object : this->GetObjectDictionary())
	{
		if (object.first >= 0x1A00 && object.first < 0x1B00)
		{
			std::uint16_t numberOfIndicesToWrite = 0;
			auto& nrOfEntriesObj = object.second->GetSubObjectDictionary().at((std::uint16_t) 0);
			if (nrOfEntriesObj->WriteToConfiguration())
				numberOfIndicesToWrite = nrOfEntriesObj->GetTypedActualValue<std::uint16_t>(); //GetNrOfEntries and only count the valid ones

			std::uint16_t count = 0;
			for (auto& subobject : object.second->GetSubObjectDictionary())
			{
				if (subobject.second->WriteToConfiguration() && subobject.first != 0 && count < numberOfIndicesToWrite)
				{
					BaseProcessDataMapping mapping = BaseProcessDataMapping(subobject.second->GetTypedActualValue<std::string>(), this->GetNodeId());
					presPayloadLimit += mapping.GetMappingLength() / 8; //Byte Size
					count++;
				}
			}
		}
	}

	if (presPayloadLimit <= 36)
	{
		return this->SetSubObjectActualValue(0x1F98, 0x5, "36"); //Set to default value
	}
	else
	{
		return this->SetSubObjectActualValue(0x1F98, 0x5, IntToHex(presPayloadLimit, 4, "0x"));
	}
}

Result ControlledNode::GetDataObjectFromMapping(const std::shared_ptr<BaseProcessDataMapping>& mapping, std::shared_ptr<BaseObject>& returnObject, std::string& retName)
{
	std::uint32_t dataIndex = mapping->GetMappingIndex();
	std::uint16_t dataSubindex = mapping->GetMappingSubIndex();

	std::shared_ptr<Object> dataObject;
	Result res = this->GetObject(dataIndex, dataObject);
	if (!res.IsSuccessful())
	{
		boost::format formatter(kMsgNonExistingMappedObject);
		formatter
		% dataIndex
		% (std::uint32_t) this->GetNodeId();
		LOG_ERROR() << formatter.str();
		return Result(ErrorCode::MAPPED_OBJECT_DOES_NOT_EXIST, formatter.str());
	}
	retName.append(dataObject->GetName());

	std::shared_ptr<SubObject> dataSubObject;
	if (dataSubindex == 0)
	{
		Result res = this->GetSubObject(dataIndex, dataSubindex, dataSubObject, false);
		if (!res.IsSuccessful())
			returnObject = dataObject;
		else
		{
			returnObject = dataSubObject;
			retName.append("_" + dataSubObject->GetName());
		}
	}
	else
	{
		Result res = this->GetSubObject(dataIndex, dataSubindex, dataSubObject);
		if (!res.IsSuccessful())
		{
			boost::format formatter(kMsgNonExistingMappedSubObject);
			formatter
			% dataIndex
			% dataSubindex
			% (std::uint32_t) this->GetNodeId();
			LOG_ERROR() << formatter.str();
			return Result(ErrorCode::MAPPED_SUBOBJECT_DOES_NOT_EXIST, formatter.str());
		}
		else
		{
			returnObject = dataSubObject;
			retName.append("_" + dataSubObject->GetName());
		}
	}
	return Result();
}

Result ControlledNode::UpdateProcessImage(Direction dir)
{
	Result res = this->UpdateProcessDataMapping(dir);
	if (!res.IsSuccessful())
		return res;
	if (dir == Direction::RX)
		this->GetReceiveProcessImage().clear();
	else if (dir == Direction::TX)
		this->GetTransmitProcessImage().clear();

	std::uint32_t piOffset = 0;
	std::uint32_t domainCount = 1;

	std::vector<std::shared_ptr<BaseProcessDataMapping>> mappingVector;
	if (dir == Direction::RX)
		mappingVector = this->GetReceiveMapping();
	else if (dir == Direction::TX)
		mappingVector = this->GetTransmitMapping();

	for (auto& mapObj : mappingVector)
	{
		std::shared_ptr<BaseObject> dataObject;
		std::string dataName;
		Result res = GetDataObjectFromMapping(mapObj, dataObject, dataName);
		if (!res.IsSuccessful())
			return res;

		//Domain object
		if (dataObject->GetUniqueIdRef().is_initialized())
		{
			std::shared_ptr<Parameter> param;
			this->GetApplicationProcess()->GetParameter(dataObject->GetUniqueIdRef().get(), param);
			//Check parameter access

			std::shared_ptr<StructDataType> structDt = std::dynamic_pointer_cast<StructDataType>(param->GetComplexDataType());
			std::shared_ptr<ArrayDataType> arrayDt = std::dynamic_pointer_cast<ArrayDataType>(param->GetComplexDataType());
			std::shared_ptr<EnumDataType> enumDt = std::dynamic_pointer_cast<EnumDataType>(param->GetComplexDataType());
			if (structDt.get())
			{
				std::uint32_t bitOffset = 0;
				for (auto& varDecl : structDt->GetVarDeclarations())
				{
					std::stringstream nameBuilder;
					nameBuilder << "CN" << (std::uint32_t) this->GetNodeId();
					nameBuilder << "_";
					nameBuilder << "DOM";
					nameBuilder << IntToHex<std::uint32_t>(domainCount, 2);
					nameBuilder << "_";
					nameBuilder << dataName;
					nameBuilder	<< "_";
					//nameBuilder << structDt->GetName();
					//nameBuilder << "_";
					nameBuilder << varDecl->GetName();

					std::string piName = nameBuilder.str();
					boost::trim(piName);
					boost::remove_erase_if(piName, boost::is_any_of("!\\/(){}[],*'"));
					std::replace(piName.begin(), piName.end(), ' ', '_');
					if (varDecl->GetDataType() == IEC_Datatype::BITSTRING)
					{
						std::shared_ptr<BaseProcessImageObject> piObj = std::make_shared<BaseProcessImageObject>(
						            piName,
						            varDecl->GetDataType(),
						            piOffset, bitOffset,
						            varDecl->GetBitSize());

						if (dir == Direction::RX)
							this->GetReceiveProcessImage().push_back(piObj);
						else if (dir == Direction::TX)
							this->GetTransmitProcessImage().push_back(piObj);
						bitOffset += varDecl->GetSize();
					}
					else
					{
						std::shared_ptr<BaseProcessImageObject> piObj = std::make_shared<BaseProcessImageObject>(
						            piName,
						            varDecl->GetDataType(),
						            piOffset,
						            varDecl->GetBitSize());
						if (dir == Direction::RX)
							this->GetReceiveProcessImage().push_back(piObj);
						else if (dir == Direction::TX)
							this->GetTransmitProcessImage().push_back(piObj);
						piOffset += varDecl->GetBitSize() / 8 ;
					}

					if (bitOffset == 8)
					{
						piOffset++;
						bitOffset = 0;
					}
				}
			}
			else if (arrayDt.get())
			{
				std::uint32_t bitOffset = 0;
				for (std::uint32_t i = arrayDt->GetLowerLimit(); i < arrayDt->GetUpperLimit(); i++)
				{
					std::stringstream nameBuilder;
					nameBuilder << "CN" << (std::uint32_t) this->GetNodeId();
					nameBuilder << "_";
					nameBuilder << "DOM";
					nameBuilder << IntToHex<std::uint32_t>(domainCount, 2);
					nameBuilder << "_";
					nameBuilder << dataName;
					nameBuilder	<< "_";
					nameBuilder << arrayDt->GetName();

					std::string piName = nameBuilder.str();
					boost::trim(piName);
					boost::remove_erase_if(piName, boost::is_any_of("!\\/(){}[],*'"));
					std::replace(piName.begin(), piName.end(), ' ', '_');
					if (arrayDt->GetDataType() == IEC_Datatype::BITSTRING)
					{
						std::shared_ptr<BaseProcessImageObject> piObj = std::make_shared<BaseProcessImageObject>(
						            piName,
						            arrayDt->GetDataType(),
						            piOffset,
						            bitOffset,
						            arrayDt->GetBitSize());
						if (dir == Direction::RX)
							this->GetReceiveProcessImage().push_back(piObj);
						else if (dir == Direction::TX)
							this->GetTransmitProcessImage().push_back(piObj);
						bitOffset += arrayDt->GetBitSize();

						if (bitOffset == 8)
						{
							piOffset++;
							bitOffset = 0;
						}
					}
					else
					{
						std::shared_ptr<BaseProcessImageObject> piObj = std::make_shared<BaseProcessImageObject>(
						            piName,
						            arrayDt->GetDataType(),
						            piOffset,
						            arrayDt->GetBitSize());
						this->GetTransmitProcessImage().push_back(piObj);
						piOffset += arrayDt->GetBitSize() / 8 ;
					}
				}
			}
			else if (enumDt.get())
			{
				//Not supported at the moment
			}
			else
			{
				//return unsupported complex datatype
			}
			domainCount++;

		}
		else
		{
			std::stringstream nameBuilder;
			nameBuilder << "CN" << (std::uint32_t) this->GetNodeId();
			nameBuilder << "_";
			nameBuilder << dataName;

			std::string piName = nameBuilder.str();
			boost::trim(piName);
			boost::remove_erase_if(piName, boost::is_any_of("!\\/(){}[],*'"));
			std::replace(piName.begin(), piName.end(), ' ', '_');
			std::shared_ptr<BaseProcessImageObject> piObj = std::make_shared<BaseProcessImageObject>(
			            piName,
			            GetIECDataType(dataObject->GetDataType().get()),
			            piOffset,
			            GetIECDataTypeBitSize(GetIECDataType(dataObject->GetDataType().get())));

			if (dir == Direction::RX)
				this->GetReceiveProcessImage().push_back(piObj);
			else if (dir == Direction::TX)
				this->GetTransmitProcessImage().push_back(piObj);
			piOffset += GetIECDataTypeBitSize(GetIECDataType(dataObject->GetDataType().get())) / 8 ;
		}
	}

	//Search for equal names in the PI and add a count number
	std::vector<std::shared_ptr<BaseProcessImageObject>> piCollection;
	if (dir == Direction::RX)
		piCollection = this->GetReceiveProcessImage();
	else if (dir == Direction::TX)
		piCollection = this->GetTransmitProcessImage();

	for (auto& pi : piCollection)
	{
		std::uint16_t findCount = 0;
		for (auto& pi_count : piCollection)
		{
			if (pi->GetName() == pi_count->GetName())
				findCount++;
		}
		if (findCount != 1)
		{
			std::uint16_t varCount = 1;
			std::string originalName = pi->GetName();
			for (auto& pi_change : piCollection)
			{
				if (pi_change->GetName() == originalName)
				{
					std::stringstream varName;
					varName << originalName << "_" << varCount;
					pi_change->SetName(varName.str());
					varCount++;
				}
			}
		}
	}
	return Result();
}

IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result ControlledNode::UpdateProcessDataMapping(Direction dir)
{
	std::uint32_t mappingParameterIndex = 0;
	std::uint32_t mappingObjectIndex = 0;

	if (dir == Direction::RX)
	{
		this->GetReceiveMapping().clear();
		mappingParameterIndex = 0x1400;
		mappingObjectIndex = 0x1600;
		this->nodeDataPresMnCurrentOffset = this->nodeDataPresMnOffset;
	}

	else if (dir == Direction::TX)
	{
		this->GetTransmitMapping().clear();
		mappingParameterIndex = 0x1800;
		mappingObjectIndex = 0x1A00;
	}

	for (auto& obj : this->GetObjectDictionary())
	{
		std::uint16_t mappedFromNode = 0;
		std::uint16_t nrOfEntries = 0;
		std::uint32_t expectedOffset = 0;
		if (obj.first >= mappingParameterIndex && obj.first < (mappingParameterIndex + 0x100))
		{
			//Get mapping parameter object
			std::shared_ptr<SubObject> nodeID;
			Result res = obj.second->GetSubObject(0x1, nodeID);
			if (!res.IsSuccessful())
				return res;

			if (nodeID->WriteToConfiguration())
				mappedFromNode = nodeID->GetTypedActualValue<std::uint16_t>();

			if (((this->GetOperationMode() == PlkOperationMode::CHAINED && mappedFromNode == 0)
			        || mappedFromNode == 240)
			        && dir == Direction::RX) //Node mode has changed to chaining or is already chained
			{
				if (this->GetOperationMode() == PlkOperationMode::CHAINED && mappedFromNode == 0)
					mappedFromNode = 240; //Make sure that chained node receives from PResMN
				expectedOffset = this->nodeDataPresMnCurrentOffset;
				receivesPResMN = true;
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

			if (nrOfEntriesObj->HasActualValue())
			{
				nrOfEntries = nrOfEntriesObj->GetTypedActualValue<std::uint16_t>();
			}
			else if (nrOfEntriesObj->HasDefaultValue())
			{
				nrOfEntries = nrOfEntriesObj->GetTypedDefaultValue<std::uint16_t>();
			}

			//If NrOfEntries is zero continue with next channel
			if (nrOfEntries == 0)
				continue;

			//correct receive from for chained nodes
			else if (nrOfEntries != 0
			         && mappedFromNode == 240)
			{
				nodeID->SetTypedObjectActualValue("240");
			}

			std::uint16_t countNrOfEntries = 0;
			for (auto& mapping : mappingObject->GetSubObjectDictionary())
			{
				if (mapping.first == 0)
					continue;

				if (countNrOfEntries == nrOfEntries)
					break;

				if (mapping.second->HasActualValue())
				{
					if (mapping.second->GetTypedActualValue<std::uint64_t>() != 0)
					{
						std::shared_ptr<BaseProcessDataMapping> mappingPtr = std::shared_ptr<BaseProcessDataMapping>(new BaseProcessDataMapping(mappingObject->GetObjectId(),
						        mapping.second->GetObjectId(), mapping.second, mapping.second->GetTypedActualValue<std::string>(), mappedFromNode, false));

						if (expectedOffset + mappingPtr->GetMappingLength() > (1490 * 8))
						{
							boost::format formatter(kMsgIsochronousMaxPayloadExceeded);
							formatter
							% (std::uint32_t) this->GetNodeId()
							% DirectionTypeValues[(std::uint8_t) dir]
							% ((expectedOffset + mappingPtr->GetMappingLength()) / 8);
							LOG_FATAL() << formatter.str();
							return Result(ErrorCode::CHANNEL_PAYLOAD_LIMIT_EXCEEDED, formatter.str());
						}

						mappingPtr->SetMappingOffset(expectedOffset);
						mapping.second->SetTypedObjectActualValue(IntToHex<std::uint64_t>(mappingPtr->GetValue(), 16, "0x"));
						res = CheckProcessDataMapping(mappingPtr, expectedOffset, dir);
						if (!res.IsSuccessful())
							return res;

						if (dir == Direction::RX)
							this->GetReceiveMapping().push_back(mappingPtr);
						else if (dir == Direction::TX)
							this->GetTransmitMapping().push_back(mappingPtr);

						countNrOfEntries++;
						expectedOffset += mappingPtr->GetMappingLength();
					}

				}
				else if (mapping.second->HasDefaultValue())
				{
					if (mapping.second->GetTypedDefaultValue<std::uint64_t>() != 0)
					{
						std::shared_ptr<BaseProcessDataMapping> mappingPtr = std::shared_ptr<BaseProcessDataMapping>(new BaseProcessDataMapping(mappingObject->GetObjectId(),
						        mapping.second->GetObjectId(), mapping.second, mapping.second->GetTypedDefaultValue<std::string>(), mappedFromNode, true));

						if (expectedOffset + mappingPtr->GetMappingLength() > (1490 * 8))
						{
							boost::format formatter(kMsgIsochronousMaxPayloadExceeded);
							formatter
							% (std::uint32_t) this->GetNodeId()
							% DirectionTypeValues[(std::uint8_t) dir]
							% ((expectedOffset + mappingPtr->GetMappingLength()) / 8);
							LOG_FATAL() << formatter.str();
							return Result(ErrorCode::CHANNEL_PAYLOAD_LIMIT_EXCEEDED, formatter.str());

						}
						res = CheckProcessDataMapping(mappingPtr, expectedOffset, dir);
						if (!res.IsSuccessful())
							return res;

						if (dir == Direction::RX)
							this->GetReceiveMapping().push_back(mappingPtr);
						else if (dir == Direction::TX)
							this->GetTransmitMapping().push_back(mappingPtr);

						countNrOfEntries++;
						expectedOffset += mappingPtr->GetMappingLength();
					}
				}
			}

			//Store the current PResMN offset if there is a second channel receiving data from PResMN
			if (mappedFromNode == 240)
				this->nodeDataPresMnCurrentOffset = expectedOffset;

			//correct NrOfEntries if there are too much mappings validated
			if (nrOfEntries  > countNrOfEntries)
			{
				nrOfEntriesObj->SetTypedObjectActualValue(IntToHex<std::uint16_t>(countNrOfEntries, 2, "0x"));

				boost::format formatter(kMsgNrOfEntriesInvalid);
				formatter
				% mappingObject->GetObjectId()
				% (std::uint32_t) this->GetNodeId()
				% countNrOfEntries
				% nrOfEntries;
				LOG_WARN() << formatter.str();
			}
		}
	}
	return Result();
}

Result ControlledNode::CheckProcessDataMapping(const std::shared_ptr<BaseProcessDataMapping>& mapping, std::uint32_t expectedOffset, Direction dir)
{
	std::uint32_t dataIndex = mapping->GetMappingIndex();
	std::uint16_t dataSubindex = mapping->GetMappingSubIndex();
	std::uint32_t size = mapping->GetMappingLength();

	std::shared_ptr<Object> dataObject;
	std::shared_ptr<SubObject> dataSubObject;
	std::shared_ptr<BaseObject> foundObject;

	//Check that mapped object exist
	if (dataSubindex == 0)
	{
		Result res = this->GetSubObject(dataIndex, dataSubindex, dataSubObject, false);
		if (!res.IsSuccessful())
		{
			res = this->GetObject(dataIndex, dataObject);
			if (!res.IsSuccessful())
			{
				boost::format formatter(kMsgNonExistingMappedObject);
				formatter
				% dataIndex
				% (std::uint32_t) this->GetNodeId();
				LOG_ERROR() << formatter.str();
				return Result(ErrorCode::MAPPED_OBJECT_DOES_NOT_EXIST, formatter.str());
			}
			else
				foundObject = dataObject;
		}
		else
			foundObject = dataSubObject;
	}
	else
	{
		Result res = this->GetSubObject(dataIndex, dataSubindex, dataSubObject);
		if (!res.IsSuccessful())
		{
			boost::format formatter(kMsgNonExistingMappedSubObject);
			formatter
			% dataIndex
			% dataSubindex
			% (std::uint32_t) this->GetNodeId();
			LOG_ERROR() << formatter.str();
			return Result(ErrorCode::MAPPED_SUBOBJECT_DOES_NOT_EXIST, formatter.str());
		}
		foundObject = dataSubObject;
	}

	//Check data size
	//Check normal data type size
	if (!foundObject->GetUniqueIdRef().is_initialized())
	{
		if (foundObject->GetBitSize() != size)
		{
			return Result(ErrorCode::OBJECT_SIZE_MAPPED_INVALID);
		}
	}
	//Check complex data type size
	else
	{
		std::shared_ptr<Parameter> param;
		Result res = this->GetApplicationProcess()->GetParameter(foundObject->GetUniqueIdRef().get(), param);
		if (!res.IsSuccessful())
			return res;

		if (foundObject->GetBitSize() != param->GetBitSize())
		{
			return Result(ErrorCode::OBJECT_SIZE_MAPPED_INVALID);
		}

		if (dir == Direction::RX)
		{
			//Check complex data type access
			if (param->GetParameterAccess() == ParameterAccess::readWriteInput
			        || param->GetParameterAccess() == ParameterAccess::read)
			{
				//Parameter access does not match mapping direction
				boost::format formatter(kMsgAccessTypeForParameterInvalid);
				formatter
				% param->GetUniqueID()
				% dataIndex
				% dataSubindex
				% (std::uint32_t) this->GetNodeId()
				% ParameterAccessValues[(std::uint8_t) param->GetParameterAccess()];
				LOG_ERROR() << formatter.str();
				return Result(ErrorCode::PARAMETER_ACCESS_INVALID, formatter.str());
			}
		}
		else if (dir == Direction::TX)
		{
			if (param->GetParameterAccess() == ParameterAccess::readWriteOutput
			        || param->GetParameterAccess() == ParameterAccess::write)
			{
				//Parameter access does not match mapping direction
				boost::format formatter(kMsgAccessTypeForParameterInvalid);
				formatter
				% param->GetUniqueID()
				% dataIndex
				% dataSubindex
				% (std::uint32_t) this->GetNodeId()
				% ParameterAccessValues[(std::uint8_t) param->GetParameterAccess()];
				LOG_ERROR() << formatter.str();
				return Result(ErrorCode::PARAMETER_ACCESS_INVALID, formatter.str());
			}
		}
	}

	//Check mapping offset
	if (mapping->GetMappingOffset() < expectedOffset) //Allow offset gaps
	{
		boost::format formatter(kMsgPdoOffsetInvalid);
		formatter
		% dataIndex
		% dataSubindex
		% (std::uint32_t) this->GetNodeId()
		% mapping->GetMappingOffset()
		% expectedOffset;
		LOG_ERROR() << formatter.str();
		return Result(ErrorCode::PDO_OFFSET_INVALID, formatter.str());
	}

	//Check PDOMapping
	if (foundObject->GetPDOMapping().is_initialized())
	{
		if (dir == Direction::RX)
		{
			if (foundObject->GetPDOMapping().get() != PDOMapping::RPDO
			        && foundObject->GetPDOMapping().get() != PDOMapping::OPTIONAL
			        && foundObject->GetPDOMapping().get() != PDOMapping::DEFAULT)
			{
				boost::format formatter(kMsgMappingTypeForPdoInvalid);
				formatter
				% dataIndex
				% dataSubindex
				% (std::uint32_t) this->GetNodeId()
				% PDOMappingValues[(std::uint8_t) foundObject->GetPDOMapping().get()];
				LOG_ERROR() << formatter.str();
				return Result(ErrorCode::MAPPING_TYPE_FOR_PDO_INVALID, formatter.str());
			}
		}
		else if (dir == Direction::TX)
		{
			if (foundObject->GetPDOMapping().get() != PDOMapping::TPDO
			        && foundObject->GetPDOMapping().get() != PDOMapping::OPTIONAL
			        && foundObject->GetPDOMapping().get() != PDOMapping::DEFAULT)
			{
				boost::format formatter(kMsgMappingTypeForPdoInvalid);
				formatter
				% dataIndex
				% dataSubindex
				% (std::uint32_t) this->GetNodeId()
				% PDOMappingValues[(std::uint8_t) foundObject->GetPDOMapping().get()];
				LOG_ERROR() << formatter.str();
				return Result(ErrorCode::MAPPING_TYPE_FOR_PDO_INVALID, formatter.str());
			}
		}
	}
	else
	{
		boost::format formatter(kMsgMappingTypeForPdoInvalid);
		formatter
		% dataIndex
		% dataSubindex
		% (std::uint32_t) this->GetNodeId()
		% "Not defined";
		LOG_ERROR() << formatter.str();
		return Result(ErrorCode::MAPPING_TYPE_FOR_PDO_INVALID, formatter.str());
	}

	//Check AccessType
	if (foundObject->GetAccessType().is_initialized())
	{
		if (dir == Direction::RX)
		{
			if (foundObject->GetAccessType().get() != AccessType::WO
			        && foundObject->GetAccessType().get() != AccessType::WOS
			        && foundObject->GetAccessType().get() != AccessType::RW)
			{
				boost::format formatter(kMsgAccessTypeForPdoInvalid);
				formatter
				% dataIndex
				% dataSubindex
				% (std::uint32_t) this->GetNodeId()
				% AccessTypeValues[(std::uint8_t) foundObject->GetAccessType().get()];
				LOG_ERROR() << formatter.str();
				return Result(ErrorCode::ACCESS_TYPE_FOR_PDO_INVALID, formatter.str());
			}
		}
		else if (dir == Direction::TX)
		{
			if (foundObject->GetAccessType().get() != AccessType::RO
			        && foundObject->GetAccessType().get() != AccessType::RW
			        && foundObject->GetAccessType().get() != AccessType::RWS)
			{
				boost::format formatter(kMsgAccessTypeForPdoInvalid);
				formatter
				% dataIndex
				% dataSubindex
				% (std::uint32_t) this->GetNodeId()
				% AccessTypeValues[(std::uint8_t) foundObject->GetAccessType().get()];
				LOG_ERROR() << formatter.str();
				return Result(ErrorCode::ACCESS_TYPE_FOR_PDO_INVALID, formatter.str());
			}
		}
	}
	else if (!foundObject->GetUniqueIdRef().is_initialized()) //Domain object may not have an access type
	{
		boost::format formatter(kMsgAccessTypeForPdoInvalid);
		formatter
		% dataIndex
		% dataSubindex
		% (std::uint32_t) this->GetNodeId()
		% AccessTypeValues[(std::uint8_t) foundObject->GetAccessType().get()];
		LOG_ERROR() << formatter.str();
		return Result(ErrorCode::ACCESS_TYPE_FOR_PDO_INVALID, formatter.str());
	}
	return Result();
}

void ControlledNode::SetNodeDataPresMnOffset(std::uint32_t offset)
{
	this->nodeDataPresMnOffset = offset;
	this->nodeDataPresMnCurrentOffset = offset;
	this->receivesPResMN = true;
}

std::uint32_t ControlledNode::GetNodeDataPresMnOffset()
{
	return this->nodeDataPresMnOffset;
}

std::uint32_t ControlledNode::GetNodeDataPresMnCurrentOffset()
{
	return this->nodeDataPresMnCurrentOffset;
}

Result ControlledNode::MoveMappingObject(const Direction dir, std::uint16_t channelNr, std::uint16_t oldPosition, std::uint16_t newPosition)
{
	if (oldPosition == newPosition)
		return Result();

	std::uint32_t channelObjectId = 0;
	bool foundOldValue = false;
	bool foundNewValue = false;
	std::shared_ptr<SubObject> oldValue;
	std::shared_ptr<SubObject> newValue;

	if (dir == Direction::RX)
		channelObjectId = 0x1600 + channelNr;
	else if (dir == Direction::TX)
		channelObjectId = 0x1A00 + channelNr;


	std::shared_ptr<Object> mappingChannel;
	Result res = this->GetObject(channelObjectId, mappingChannel);
	if (!res.IsSuccessful())
		return res;

	for (auto& channelObj : mappingChannel->GetSubObjectDictionary())
	{
		if (channelObj.first == oldPosition && foundOldValue == false)
		{
			oldValue = channelObj.second;
			foundOldValue = true;
			continue;
		}
		else if (channelObj.first == newPosition && foundNewValue == false)
		{
			newValue = channelObj.second;
			foundNewValue = true;
			continue;
		}

		if (foundNewValue && foundOldValue)
			break;
	}

	//Both values are empty - no operation needed
	if (oldValue->HasActualValue() == false
	        && newValue->HasActualValue() == false)
	{
		oldValue->SetTypedObjectActualValue("0x0");
		newValue->SetTypedObjectActualValue("0x0");
		return Result();
	}

	//Move to empty object is allowed to empty object
	if (oldValue->HasActualValue() == false)
		oldValue->SetTypedObjectActualValue("0x0");

	//Move to empty object is allowed add "0x0" as actual value
	if (newValue->HasActualValue() == false)
		newValue->SetTypedObjectActualValue("0x0");


	std::string newValueStr = "0x" + newValue->GetTypedActualValue<std::string>();
	std::string oldValueStr = "0x" + oldValue->GetTypedActualValue<std::string>();

	//Switch the mappings
	if (newValue->HasActualValue())
		oldValue->SetTypedObjectActualValue(newValueStr); //prefix otherwise the it will assume dec value
	if (oldValue->HasActualValue())
		newValue->SetTypedObjectActualValue(oldValueStr);

	//Recalculate Offset
	std::uint32_t mappingOffset = 0;
	std::uint32_t mappingSize = 0;
	for (auto& channelObj : mappingChannel->GetSubObjectDictionary())
	{
		if (channelObj.first == 0)
			continue;

		if (channelObj.second->HasActualValue())
		{
			if (channelObj.second->GetTypedActualValue<std::uint64_t>() != 0)
			{
				BaseProcessDataMapping mappingValue = BaseProcessDataMapping(channelObj.second->GetTypedActualValue<std::string>(), 0);
				mappingValue.SetMappingOffset(mappingOffset + mappingSize);
				mappingOffset = mappingValue.GetMappingOffset();
				mappingSize = mappingValue.GetMappingLength();

				channelObj.second->SetTypedObjectActualValue(mappingValue.ToString(true));
			}
			else
				channelObj.second->ClearActualValue();
		}
	}
	return this->UpdateProcessImage(dir);
}

bool ControlledNode::ReceivesPResMN()
{
	return this->receivesPResMN;
}