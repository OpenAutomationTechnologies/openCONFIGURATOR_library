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
{}

ControlledNode::~ControlledNode()
{}

Result ControlledNode::AddNodeAssignment(const NodeAssignment& assign)
{
	if (assign == NodeAssignment::NMT_NODEASSIGN_MN_PRES && this->GetNodeId() < 240)
	{
		boost::format formatter(kMsgNodeAssignmentNotSupported[static_cast<std::underlying_type<Language>::type>(LoggingConfiguration::GetInstance().GetCurrentLanguage())]);
		formatter
		% GetNodeAssignmentName(assign)
		% (std::uint32_t) this->GetNodeId();
		LOG_ERROR() << formatter.str();
		return Result(ErrorCode::NODE_ASSIGNMENT_NOT_SUPPORTED, formatter.str());
	}

	auto it = find(this->GetNodeAssignment().begin(), this->GetNodeAssignment().end(), assign);
	if (it == this->GetNodeAssignment().end())
		this->GetNodeAssignment().push_back(assign);
	else
	{
		boost::format formatter(kMsgNodeAssignmentAlreadyExists[static_cast<std::underlying_type<Language>::type>(LoggingConfiguration::GetInstance().GetCurrentLanguage())]);
		formatter
		% GetNodeAssignmentName(assign)
		% (std::uint32_t) this->GetNodeId();
		LOG_INFO() << formatter.str();
		return Result();
	}
	return Result();
}

Result ControlledNode::RemoveNodeAssignment(const NodeAssignment& assign)
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

Result ControlledNode::MapObject(std::uint32_t index, const Direction& dir, std::uint16_t channelNr, std::uint32_t position, std::uint16_t fromNode, bool updateNrOfEntries)
{
	//Retrieve object to be mapped
	std::shared_ptr<Object> objToMap;
	Result res = this->GetObject(index, objToMap);
	if (!res.IsSuccessful())
		return res;

	//Map object
	return this->MapBaseObject(objToMap, index, 0, dir, updateNrOfEntries, channelNr, position, fromNode);
}

Result ControlledNode::MapSubObject(std::uint32_t index, std::uint16_t subindex, const Direction& dir, std::uint16_t channelNr, std::uint32_t position, std::uint16_t fromNode, bool updateNrOfEntries)
{
	//retrieve sub object to be mapped
	std::shared_ptr<SubObject> objToMap;
	Result res = this->GetSubObject(index, subindex, objToMap);
	if (!res.IsSuccessful())
		return res;

	//Map subobject
	return this->MapBaseObject(objToMap, index, subindex, dir, updateNrOfEntries, channelNr, position, fromNode);
}

Result ControlledNode::MapBaseObject(const std::shared_ptr<BaseObject>& objToMap, std::uint32_t index, std::uint16_t subindex, const Direction& dir, bool updateNrOfEntries, std::uint16_t channelNr, std::uint32_t position, std::uint16_t fromNode)
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
	if (objToMap->GetUniqueIdRef().is_initialized())
	{
		ParameterAccess access = ParameterAccess::noAccess;
		std::string uniqueID = "";
		if (objToMap->GetReferencedParameter().get())
		{
			access = objToMap->GetReferencedParameter()->GetParameterAccess();
			uniqueID = objToMap->GetReferencedParameter()->GetUniqueID();
		}
		else if (objToMap->GetReferencedParameterGroup().get())
		{
			uniqueID = objToMap->GetReferencedParameterGroup()->GetUniqueId();
			Result res = objToMap->GetReferencedParameterGroup()->GetParameterGroupAccess(access);
			if (!res.IsSuccessful())
				return res;
		}

		//Check Parameter access depeding on direction
		if (dir == Direction::RX)
		{
			//Check complex data type access
			if (access == ParameterAccess::readWriteInput
			        || access == ParameterAccess::read)
			{
				//Parameter access does not match mapping direction
				boost::format formatter(kMsgAccessTypeForParameterInvalid[static_cast<std::underlying_type<Language>::type>(LoggingConfiguration::GetInstance().GetCurrentLanguage())]);
				formatter
				% uniqueID
				% index
				% subindex
				% (std::uint32_t) this->GetNodeId()
				% ParameterAccessValues[static_cast<std::underlying_type<ParameterAccess>::type>(access)];
				LOG_ERROR() << formatter.str();
				return Result(ErrorCode::PARAMETER_ACCESS_INVALID, formatter.str());
			}
		}
		else if (dir == Direction::TX)
		{
			if (access == ParameterAccess::readWriteOutput
			        || access == ParameterAccess::write)
			{
				//Parameter access does not match mapping direction
				boost::format formatter(kMsgAccessTypeForParameterInvalid[static_cast<std::underlying_type<Language>::type>(LoggingConfiguration::GetInstance().GetCurrentLanguage())]);
				formatter
				% uniqueID
				% index
				% subindex
				% (std::uint32_t) this->GetNodeId()
				% ParameterAccessValues[static_cast<std::underlying_type<ParameterAccess>::type>(access)];
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
				boost::format formatter(kMsgMappingTypeForPdoInvalid[static_cast<std::underlying_type<Language>::type>(LoggingConfiguration::GetInstance().GetCurrentLanguage())]);
				formatter
				% index
				% subindex
				% (std::uint32_t) this->GetNodeId()
				% PDOMappingValues[static_cast<std::underlying_type<PDOMapping>::type>(objToMap->GetPDOMapping().get())];
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
				boost::format formatter(kMsgMappingTypeForPdoInvalid[static_cast<std::underlying_type<Language>::type>(LoggingConfiguration::GetInstance().GetCurrentLanguage())]);
				formatter
				% index
				% subindex
				% (std::uint32_t) this->GetNodeId()
				% PDOMappingValues[static_cast<std::underlying_type<PDOMapping>::type>(objToMap->GetPDOMapping().get())];
				LOG_ERROR() << formatter.str();
				return Result(ErrorCode::MAPPING_TYPE_FOR_PDO_INVALID, formatter.str());
			}
		}
	}
	//Every object to be mapped needs a PDOMapping attribute
	else
	{
		boost::format formatter(kMsgMappingTypeForPdoInvalid[static_cast<std::underlying_type<Language>::type>(LoggingConfiguration::GetInstance().GetCurrentLanguage())]);
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
				boost::format formatter(kMsgAccessTypeForPdoInvalid[static_cast<std::underlying_type<Language>::type>(LoggingConfiguration::GetInstance().GetCurrentLanguage())]);
				formatter
				% index
				% subindex
				% (std::uint32_t) this->GetNodeId()
				% AccessTypeValues[static_cast<std::underlying_type<AccessType>::type>(objToMap->GetAccessType().get())];
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
				boost::format formatter(kMsgAccessTypeForPdoInvalid[static_cast<std::underlying_type<Language>::type>(LoggingConfiguration::GetInstance().GetCurrentLanguage())]);
				formatter
				% index
				% subindex
				% (std::uint32_t) this->GetNodeId()
				% AccessTypeValues[static_cast<std::underlying_type<AccessType>::type>(objToMap->GetAccessType().get())];
				LOG_ERROR() << formatter.str();
				return Result(ErrorCode::ACCESS_TYPE_FOR_PDO_INVALID, formatter.str());
			}
		}
	}
	//Domain object must not have an access type all the other must have an access type attribute
	else if (!objToMap->GetUniqueIdRef().is_initialized())
	{
		boost::format formatter(kMsgAccessTypeForParameterInvalid[static_cast<std::underlying_type<Language>::type>(LoggingConfiguration::GetInstance().GetCurrentLanguage())]);
		formatter
		% index
		% subindex
		% (std::uint32_t) this->GetNodeId()
		% PDOMappingValues[static_cast<std::underlying_type<PDOMapping>::type>(objToMap->GetPDOMapping().get())];
		LOG_ERROR() << formatter.str();
		return Result(ErrorCode::MAPPING_TYPE_FOR_PDO_INVALID, formatter.str());
	}

	//Check Size of Object to be mapped
	if (objToMap->GetBitSize() == 0)
	{
		boost::format formatter(kMsgMappingSizeZero[static_cast<std::underlying_type<Language>::type>(LoggingConfiguration::GetInstance().GetCurrentLanguage())]);
		formatter
		% index
		% subindex
		% (std::uint32_t) this->GetNodeId();
		LOG_ERROR() << formatter.str();
		return Result(ErrorCode::MAPPING_INVALID, formatter.str());
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
		boost::format formatter(kMsgInsufficientMappingObjects[static_cast<std::underlying_type<Language>::type>(LoggingConfiguration::GetInstance().GetCurrentLanguage())]);
		formatter
		% (std::uint32_t) this->GetNodeId()
		% (std::uint32_t) mappingObj->GetSubObjectDictionary().size()
		% position;
		LOG_FATAL() << formatter.str();
		return Result(ErrorCode::INSUFFICIENT_MAPPING_OBJECTS, formatter.str());
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
					BaseProcessDataMapping object = BaseProcessDataMapping(tMapping.second->GetTypedActualValue<std::string>(), this->GetNodeId());
					offset = object.GetMappingOffset();
				}
			}

			if (offset < expectedOffset)
				offset = expectedOffset;

			if (offset + objToMap->GetBitSize() > (1490 * 8))
			{
				boost::format formatter(kMsgIsochronousMaxPayloadExceeded[static_cast<std::underlying_type<Language>::type>(LoggingConfiguration::GetInstance().GetCurrentLanguage())]);
				formatter
				% (std::uint32_t) this->GetNodeId()
				% DirectionTypeValues[static_cast<std::underlying_type<Direction>::type>(dir)]
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
				BaseProcessDataMapping object = BaseProcessDataMapping(tMapping.second->GetTypedActualValue<std::string>(), this->GetNodeId());

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
				boost::format formatter(kMsgNrOfEntriesInvalid[static_cast<std::underlying_type<Language>::type>(LoggingConfiguration::GetInstance().GetCurrentLanguage())]);
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

Result ControlledNode::MapAllTxObjects(std::uint16_t channelNr, bool updateNrOfEntries)
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
						mappingObjNrOfEntries = subobject.second->GetTypedActualValue<std::uint16_t>(); //Set actual nr of mapping objects
						if (mappingObjNrOfEntries == 0)
							break;
						count += 2; //Add count for mapping set and reset
					}
				}
				else if (mappingObjCount < mappingObjNrOfEntries)
				{
					if (subobject.second->WriteToConfiguration()) //Only count mapping objects of they are activated
					{
						count++;
						mappingObjCount++;
					}
				}
			}
			else if (subobject.second->WriteToConfiguration())
			{
				count++;
			}
		}
	}

	boost::format formatter(kMsgNodeObjectCount[static_cast<std::underlying_type<Language>::type>(LoggingConfiguration::GetInstance().GetCurrentLanguage())]);
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
						mappingObjNrOfEntries = subobject.second->GetTypedActualValue<uint16_t>(); //Set actual nr of mapping objects
						if (mappingObjNrOfEntries == 0)
							break;
						size += 2 * subobject.second->GetBitSize(); //Add size of NrOfEntries set and reset
					}
				}
				else if (mappingObjCount < mappingObjNrOfEntries)
				{
					if (subobject.second->WriteToConfiguration()) //Only count mapping objects of they are activated
					{
						size += subobject.second->GetBitSize();
						mappingObjCount++;
					}
				}
			}
			else if (subobject.second->WriteToConfiguration())
			{
				size += subobject.second->GetBitSize();
			}
		}
	}

	boost::format formatter(kMsgNodeObjectCountSize[static_cast<std::underlying_type<Language>::type>(LoggingConfiguration::GetInstance().GetCurrentLanguage())]);
	formatter
	% (std::uint32_t) this->GetNodeId()
	% size;
	LOG_INFO() << formatter.str();
	return size;
}

Result ControlledNode::SetOperationMode(const PlkOperationMode& _operationMode)
{
	if (_operationMode == this->operationMode)
		return Result();

	if (_operationMode == PlkOperationMode::NORMAL)
	{

		this->RemoveNodeAssignment(NodeAssignment::NMT_NODEASSIGN_MULTIPLEXED_CN);
		this->RemoveNodeAssignment(NodeAssignment::NMT_NODEASSIGN_PRES_CHAINING);

		this->operationMode = _operationMode;
	}
	else if (_operationMode == PlkOperationMode::MULTIPLEXED)
	{
		bool operationModeSupported = false;
		this->GetNetworkManagement()->GetFeatureActualValue<bool>(CNFeatureEnum::DLLCNFeatureMultiplex, operationModeSupported);

		if (operationModeSupported)
		{
			this->operationMode = _operationMode;
			this->AddNodeAssignment(NodeAssignment::NMT_NODEASSIGN_MULTIPLEXED_CN);
		}
		else
		{
			boost::format formatter(kMsgMultiplexingNotSupported[static_cast<std::underlying_type<Language>::type>(LoggingConfiguration::GetInstance().GetCurrentLanguage())]);
			formatter
			% (std::uint32_t) this->GetNodeId();
			LOG_ERROR() << formatter.str();
			return Result(ErrorCode::MULTIPLEXING_NOT_SUPPORTED, formatter.str());
		}

	}
	else if (_operationMode == PlkOperationMode::CHAINED)
	{
		bool operationModeSupported = false;
		this->GetNetworkManagement()->GetFeatureActualValue<bool>(CNFeatureEnum::DLLCNPResChaining, operationModeSupported);

		if (operationModeSupported)
		{
			this->operationMode = _operationMode;
			this->AddNodeAssignment(NodeAssignment::NMT_NODEASSIGN_PRES_CHAINING);
			this->UpdateProcessImage(Direction::RX);
		}
		else
		{
			boost::format formatter(kMsgChainingNotSupported[static_cast<std::underlying_type<Language>::type>(LoggingConfiguration::GetInstance().GetCurrentLanguage())]);
			formatter
			% (std::uint32_t) this->GetNodeId();
			LOG_ERROR() << formatter.str();
			return Result(ErrorCode::CHAINING_NOT_SUPPORTED, formatter.str());
		}
	}
	return Result();
}

const PlkOperationMode& ControlledNode::GetOperationMode() const
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

			if (paramObj->HasActualValue())
			{
				if (paramObj->GetTypedActualValue<std::uint16_t>() != 0)
					continue; //Cross traffic does not count to PreqPayloadLimit
			}

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

	if (preqPayloadLimit <= 36 && preqPayloadLimit > 0)
	{
		return this->SetSubObjectActualValue(0x1F98, 0x4, "36"); //Set to minimum value
	}
	else if (preqPayloadLimit > 36)
	{
		return this->SetSubObjectActualValue(0x1F98, 0x4, IntToHex(preqPayloadLimit, 4, "0x"));
	}
	else
	{
		std::shared_ptr<SubObject> subObj;
		Result res = this->GetSubObject(0x1F98, 0x4, subObj, false);
		if (!res.IsSuccessful())
			return res;
		if (subObj->HasDefaultValue())
			return this->SetSubObjectActualValue(0x1F98, 0x4, ""); //Clear if existing
		else
			return this->SetSubObjectActualValue(0x1F98, 0x4, "36"); //Set to minimum value
	}
}

IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result ControlledNode::CalculatePResPayloadLimit()
{
	std::uint32_t presPayloadLimit = 0;
	std::uint32_t mappingObjectIndex = 0x1A00;
	std::uint32_t mappingParameterIndex = 0x1800;

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
				if (nodeID->GetTypedActualValue<std::uint16_t>() != 0)
					continue;
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

	if (presPayloadLimit > 0 && presPayloadLimit <= 36)
	{
		return this->SetSubObjectActualValue(0x1F98, 0x5, "36"); //Set to minimum value
	}
	else if (presPayloadLimit > 36)
	{
		return this->SetSubObjectActualValue(0x1F98, 0x5, IntToHex(presPayloadLimit, 4, "0x"));
	}
	else
	{
		std::shared_ptr<SubObject> subObj;
		Result res = this->GetSubObject(0x1F98, 0x5, subObj, false);
		if (!res.IsSuccessful())
			return res;
		if (subObj->HasDefaultValue())
			return this->SetSubObjectActualValue(0x1F98, 0x5, ""); //Clear if existing
		else
			return this->SetSubObjectActualValue(0x1F98, 0x5, "36"); //Set to minimum value
	}
}

Result ControlledNode::GetDataObjectFromMapping(const std::shared_ptr<BaseProcessDataMapping>& mapping, std::shared_ptr<BaseObject>& returnObject, std::string& retName)
{
	std::uint32_t dataIndex = mapping->GetMappingIndex();
	std::uint16_t dataSubindex = mapping->GetMappingSubIndex();

	std::shared_ptr<Object> dataObject;
	Result res = this->GetObject(dataIndex, dataObject, !this->IgnoreNonExistingMappingObjects());
	if (!res.IsSuccessful())
	{
		boost::format formatter(kMsgNonExistingMappedObject[static_cast<std::underlying_type<Language>::type>(LoggingConfiguration::GetInstance().GetCurrentLanguage())]);
		formatter
		% dataIndex
		% (std::uint32_t) this->GetNodeId();
		if (this->IgnoreNonExistingMappingObjects())
		{
			if (this->GetNodeId() < 240)
			{
				LOG_WARN() << "Invalid process image ignored : " << formatter.str();
			}
		}
		else
		{
			LOG_ERROR() << formatter.str();
		}
		return Result(ErrorCode::MAPPED_OBJECT_DOES_NOT_EXIST, formatter.str());
	}
	retName.append(dataObject->GetName());

	std::shared_ptr<SubObject> dataSubObject;
	if (dataSubindex == 0)
	{
		res = this->GetSubObject(dataIndex, dataSubindex, dataSubObject, false);
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
		res = this->GetSubObject(dataIndex, dataSubindex, dataSubObject, !this->IgnoreNonExistingMappingObjects());
		if (!res.IsSuccessful())
		{
			boost::format formatter(kMsgNonExistingMappedSubObject[static_cast<std::underlying_type<Language>::type>(LoggingConfiguration::GetInstance().GetCurrentLanguage())]);
			formatter
			% dataIndex
			% dataSubindex
			% (std::uint32_t) this->GetNodeId();
			if (this->IgnoreNonExistingMappingObjects())
			{
				if (this->GetNodeId() < 240)
				{
					LOG_WARN() << kMsgInvalidProcessImage[static_cast<std::underlying_type<Language>::type>(LoggingConfiguration::GetInstance().GetCurrentLanguage())] << formatter.str();
				}
			}
			else
			{
				LOG_ERROR() << formatter.str();
			}
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

Result ControlledNode::UpdateProcessImage(const Direction& dir)
{
	Result res = this->UpdateProcessDataMapping(dir);
	if (!res.IsSuccessful())
		return res;
	if (dir == Direction::RX)
		this->GetReceiveProcessImage().clear();
	else if (dir == Direction::TX)
		this->GetTransmitProcessImage().clear();

	std::uint32_t piOffset = 0;
	std::uint32_t bitOffset = 0;
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
		res = GetDataObjectFromMapping(mapObj, dataObject, dataName);
		if (!res.IsSuccessful())
		{
			if (this->IgnoreNonExistingMappingObjects())
				continue;
			else
				return res;
		}

		//Complex data object
		if (dataObject->GetUniqueIdRef().is_initialized())
		{
			if (dataObject->GetReferencedParameterGroup().get())
			{
				res = ProcessParameterGroup(dataObject->GetReferencedParameterGroup()->GetUniqueId(), mapObj, dataObject->GetReferencedParameterGroup(), dataName, dir, piOffset, domainCount);
				if (!res.IsSuccessful())
					return res;
				domainCount++;
				if (dir == Direction::RX)
				{
					if (this->GetReceiveProcessImage().back()->GetDataType() == IEC_Datatype::BOOL ||
					        this->GetReceiveProcessImage().back()->GetDataType() == IEC_Datatype::BITSTRING)
					{
						piOffset++;
					}
				}
				else if (dir == Direction::TX)
					if (this->GetTransmitProcessImage().back()->GetDataType() == IEC_Datatype::BOOL ||
					        this->GetTransmitProcessImage().back()->GetDataType() == IEC_Datatype::BITSTRING)
					{
						piOffset++;
					}
				continue;
			}

			//Check parameter access
			std::shared_ptr<StructDataType> structDt = std::dynamic_pointer_cast<StructDataType>(dataObject->GetReferencedParameter()->GetComplexDataType());
			std::shared_ptr<ArrayDataType> arrayDt = std::dynamic_pointer_cast<ArrayDataType>(dataObject->GetReferencedParameter()->GetComplexDataType());
			//std::shared_ptr<EnumDataType> enumDt = std::dynamic_pointer_cast<EnumDataType>(dataObject->GetReferencedParameter()->GetComplexDataType());
			if (structDt.get())
			{
				ProcessComplexDatatype(dataObject->GetReferencedParameter()->GetUniqueID(), mapObj, structDt, dataName, dir, piOffset, domainCount);
			}
			else if (arrayDt.get())
			{
				ProcessComplexDatatype(dataObject->GetReferencedParameter()->GetUniqueID(), mapObj, arrayDt, dataName, dir, piOffset, domainCount);
			}
			/*else if (enumDt.get())
			{
				ProcessComplexDatatype(dataObject->GetReferencedParameter()->GetUniqueID(), mapObj ,enumDt, dataName, dir, piOffset, domainCount);
			}*/
			else
			{
				ProcessComplexDatatype(mapObj, dataObject->GetReferencedParameter(), dataName, dir, piOffset, bitOffset, domainCount);
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
			std::replace(piName.begin(), piName.end(), '.', '_');
			std::shared_ptr<BaseProcessImageObject> piObj = std::make_shared<BaseProcessImageObject>(
			            piName,
			            GetIECDataType(dataObject->GetDataType().get()),
			            piOffset,
			            GetIECDataTypeBitSize(GetIECDataType(dataObject->GetDataType().get())));
			piObj->SetMappingObjectIndex(mapObj->GetObject());
			piObj->SetMappingObjectSubIndex(mapObj->GetSubObject());
			piObj->SetSourceNodeId(this->GetNodeId());

			if (dir == Direction::RX)
				this->GetReceiveProcessImage().push_back(piObj);
			else if (dir == Direction::TX)
				this->GetTransmitProcessImage().push_back(piObj);
			piOffset += GetIECDataTypeBitSize(GetIECDataType(dataObject->GetDataType().get())) / 8 ;
		}
	}

	//Search for equal names in the PI and add a count number
	std::vector<std::shared_ptr<BaseProcessImageObject>>* piCollection = NULL;
	if (dir == Direction::RX)
		piCollection = &this->GetReceiveProcessImage();
	else if (dir == Direction::TX)
		piCollection = &this->GetTransmitProcessImage();

	//Align bit data objects
	std::uint32_t bitCount = 0;
	piOffset = 0;
	std::string piName = "";
	for (auto it = piCollection->begin(); it != piCollection->end(); ++it)
	{
		if (it->get()->GetPiOffset() != piOffset)
		{
			if (bitCount != 0)
			{
				std::shared_ptr<BaseProcessImageObject> piObj = std::make_shared<BaseProcessImageObject>(
				            piName,
				            IEC_Datatype::BITSTRING,
				            piOffset,
				            bitCount,
				            (8 - bitCount));
				bitCount = 0;
				it = piCollection->insert(it, piObj);
				it++;
			}
		}

		if (it->get()->GetDataType() == IEC_Datatype::BITSTRING || it->get()->GetDataType() == IEC_Datatype::BOOL)
		{
			bitCount += it->get()->GetSize();
			if (bitCount == 8)
				bitCount = 0;

			piName = it->get()->GetName().substr(0, it->get()->GetName().find_first_of("_"));
			piName = piName + "_Unused_Data";
		}

		if (it == (piCollection->end() - 1) && bitCount != 0)
		{
			std::shared_ptr<BaseProcessImageObject> piObj = std::make_shared<BaseProcessImageObject>(
			            piName,
			            IEC_Datatype::BITSTRING,
			            piOffset,
			            bitCount,
			            (8 - bitCount));
			piCollection->push_back(piObj);
			break;
		}
		piOffset = it->get()->GetPiOffset();
	}

	//Add unused data for byte sized data in the PI
	for (auto it = piCollection->begin(); it != piCollection->end(); ++it)
	{
		if (it != piCollection->end() - 1)
		{
			std::uint32_t targetPiOffset = (it + 1)->get()->GetPiOffset();
			std::uint32_t calculatedTargetPiOffset = 0;
			if (it->get()->GetDataType() == IEC_Datatype::BITSTRING || it->get()->GetDataType() == IEC_Datatype::BOOL)
				calculatedTargetPiOffset = it->get()->GetPiOffset() + 1;
			else
				calculatedTargetPiOffset = it->get()->GetPiOffset() + (Utilities::GetIECDataTypeBitSize(it->get()->GetDataType()) / 8);
			if (calculatedTargetPiOffset != targetPiOffset)
			{
				std::uint32_t newtargetPiOffset = (it + 1)->get()->GetPiOffset();
				for (std::uint32_t i = it->get()->GetPiOffset() + 1; i < newtargetPiOffset; i++)
				{
					std::shared_ptr<BaseProcessImageObject> piObj = std::make_shared<BaseProcessImageObject>(
					            piName,
					            IEC_Datatype::USINT,
					            i,
					            8);
					it = piCollection->insert(it + 1, piObj);
					if (it + 1 != piCollection->end())
						it++;
				}
			}
		}
	}

	for (auto& pi : *piCollection)
	{
		std::uint16_t findCount = 0;
		for (auto& pi_count : *piCollection)
		{
			if (pi->GetName() == pi_count->GetName())
				findCount++;
		}
		if (findCount != 1)
		{
			std::uint16_t varCount = 1;
			std::string originalName = pi->GetName();
			for (auto& pi_change : *piCollection)
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

IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result ControlledNode::UpdateProcessDataMapping(const Direction& dir)
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

			bool defaultMapping = false;
			if (nrOfEntriesObj->HasActualValue())
			{
				nrOfEntries = nrOfEntriesObj->GetTypedActualValue<std::uint16_t>();
			}
			else if (nrOfEntriesObj->HasDefaultValue())
			{
				nrOfEntries = nrOfEntriesObj->GetTypedDefaultValue<std::uint16_t>();
				defaultMapping = true;
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
						        mapping.second->GetObjectId(), mapping.second, mapping.second->GetTypedActualValue<std::string>(), mappedFromNode));

						if (expectedOffset + mappingPtr->GetMappingLength() > (1490 * 8))
						{
							boost::format formatter(kMsgIsochronousMaxPayloadExceeded[static_cast<std::underlying_type<Language>::type>(LoggingConfiguration::GetInstance().GetCurrentLanguage())]);
							formatter
							% (std::uint32_t) this->GetNodeId()
							% DirectionTypeValues[static_cast<std::underlying_type<Direction>::type>(dir)]
							% ((expectedOffset + mappingPtr->GetMappingLength()) / 8);
							LOG_FATAL() << formatter.str();
							return Result(ErrorCode::CHANNEL_PAYLOAD_LIMIT_EXCEEDED, formatter.str());
						}

						if (mappingPtr->GetMappingOffset() != expectedOffset)
						{
							boost::format formatter(kMsgPdoOffsetInvalid[static_cast<std::underlying_type<Language>::type>(LoggingConfiguration::GetInstance().GetCurrentLanguage())]);
							formatter
							% mappingObject->GetObjectId()
							% mapping.first
							% (std::uint32_t) this->GetNodeId()
							% mappingPtr->GetMappingOffset()
							% expectedOffset;
							if (this->IgnoreInvalidMappingOffsets())
							{
								expectedOffset = mappingPtr->GetMappingOffset();
								LOG_WARN() << kMsgInvalidMappingOffset[static_cast<std::underlying_type<Language>::type>(LoggingConfiguration::GetInstance().GetCurrentLanguage())] << formatter.str();
							}
							else
							{
								if (GetOperationMode() != PlkOperationMode::CHAINED)
								{
									LOG_WARN() << formatter.str() << kMsgInvalidMappingRecalculated[static_cast<std::underlying_type<Language>::type>(LoggingConfiguration::GetInstance().GetCurrentLanguage())];
								}
								mappingPtr->SetMappingOffset(expectedOffset);
							}
						}

						res = CheckProcessDataMapping(mappingPtr, expectedOffset, dir);
						if (!res.IsSuccessful())
							return res;

						mapping.second->SetTypedObjectActualValue(IntToHex<std::uint64_t>(mappingPtr->GetValue(), 16, "0x"));

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
						        mapping.second->GetObjectId(), mapping.second, mapping.second->GetTypedDefaultValue<std::string>(), mappedFromNode));

						if (expectedOffset + mappingPtr->GetMappingLength() > (1490 * 8))
						{
							boost::format formatter(kMsgIsochronousMaxPayloadExceeded[static_cast<std::underlying_type<Language>::type>(LoggingConfiguration::GetInstance().GetCurrentLanguage())]);
							formatter
							% (std::uint32_t) this->GetNodeId()
							% DirectionTypeValues[static_cast<std::underlying_type<Direction>::type>(dir)]
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
			if (nrOfEntries > countNrOfEntries || (defaultMapping && nrOfEntries != countNrOfEntries))
			{
				nrOfEntriesObj->SetTypedObjectActualValue(IntToHex<std::uint16_t>(countNrOfEntries, 2, "0x"));

				boost::format formatter(kMsgNrOfEntriesInvalid[static_cast<std::underlying_type<Language>::type>(LoggingConfiguration::GetInstance().GetCurrentLanguage())]);
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

Result ControlledNode::CheckProcessDataMapping(const std::shared_ptr<BaseProcessDataMapping>& mapping, std::uint32_t expectedOffset, const Direction& dir)
{
	std::uint32_t dataIndex = mapping->GetMappingIndex();
	std::uint16_t dataSubindex = mapping->GetMappingSubIndex();
	std::uint32_t mapping_size = mapping->GetMappingLength();

	std::shared_ptr<Object> dataObject;
	std::shared_ptr<SubObject> dataSubObject;
	std::shared_ptr<BaseObject> foundObject;

	//Check that mapped object exist
	if (dataSubindex == 0)
	{
		Result res = this->GetSubObject(dataIndex, dataSubindex, dataSubObject, false);
		if (!res.IsSuccessful())
		{
			res = this->GetObject(dataIndex, dataObject, !this->IgnoreNonExistingMappingObjects());
			if (!res.IsSuccessful())
			{
				boost::format formatter(kMsgNonExistingMappedObject[static_cast<std::underlying_type<Language>::type>(LoggingConfiguration::GetInstance().GetCurrentLanguage())]);
				formatter
				% dataIndex
				% (std::uint32_t) this->GetNodeId();
				if (this->IgnoreNonExistingMappingObjects())
				{
					if (this->GetNodeId() < 240)
					{
						LOG_WARN() << kMsgInvalidMappingReference[static_cast<std::underlying_type<Language>::type>(LoggingConfiguration::GetInstance().GetCurrentLanguage())] << formatter.str();
					}
					return Result();
				}
				else
				{
					LOG_ERROR() << formatter.str();
					return Result(ErrorCode::MAPPED_OBJECT_DOES_NOT_EXIST, formatter.str());
				}
			}
			else
				foundObject = dataObject;
		}
		else
			foundObject = dataSubObject;
	}
	else
	{
		Result res = this->GetSubObject(dataIndex, dataSubindex, dataSubObject, !this->IgnoreNonExistingMappingObjects());
		if (!res.IsSuccessful())
		{
			boost::format formatter(kMsgNonExistingMappedSubObject[static_cast<std::underlying_type<Language>::type>(LoggingConfiguration::GetInstance().GetCurrentLanguage())]);
			formatter
			% dataIndex
			% dataSubindex
			% (std::uint32_t) this->GetNodeId();
			if (this->IgnoreNonExistingMappingObjects())
			{
				if (this->GetNodeId() < 240)
				{
					LOG_WARN() << kMsgInvalidMappingReference[static_cast<std::underlying_type<Language>::type>(LoggingConfiguration::GetInstance().GetCurrentLanguage())] << formatter.str();
				}
				return Result();
			}
			else
			{
				LOG_ERROR() << formatter.str();
				return Result(ErrorCode::MAPPED_OBJECT_DOES_NOT_EXIST, formatter.str());
			}
		}
		foundObject = dataSubObject;
	}

	//Check data size
	//Check normal data type size
	if (!foundObject->GetUniqueIdRef().is_initialized())
	{
		if (foundObject->GetBitSize() != mapping_size)
		{
			boost::format formatter(kMsgMappedObjectSizeInvalid[static_cast<std::underlying_type<Language>::type>(LoggingConfiguration::GetInstance().GetCurrentLanguage())]);
			formatter
			% dataIndex
			% dataSubindex
			% foundObject->GetBitSize()
			% mapping_size
			% (std::uint32_t) this->GetNodeId();
			LOG_ERROR() << formatter.str();
			return Result(ErrorCode::OBJECT_SIZE_MAPPED_INVALID, formatter.str());
		}
	}
	//Check complex data type size
	else
	{
		std::uint32_t size = 0;
		ParameterAccess access = ParameterAccess::noAccess;
		std::string uniqueID = "";
		if (foundObject->GetReferencedParameter().get())
		{
			size = foundObject->GetReferencedParameter()->GetBitSize();
			uniqueID = foundObject->GetReferencedParameter()->GetUniqueID();
			access = foundObject->GetReferencedParameter()->GetParameterAccess();
		}
		else if (foundObject->GetReferencedParameterGroup().get())
		{
			size = foundObject->GetReferencedParameterGroup()->GetBitSize();
			uniqueID = foundObject->GetReferencedParameterGroup()->GetUniqueId();
			Result res = foundObject->GetReferencedParameterGroup()->GetParameterGroupAccess(access);
			if (!res.IsSuccessful())
				return res;
		}

		if (foundObject->GetBitSize() != size)
		{
			boost::format formatter(kMsgMappedObjectSizeInvalid[static_cast<std::underlying_type<Language>::type>(LoggingConfiguration::GetInstance().GetCurrentLanguage())]);
			formatter
			% dataIndex
			% dataSubindex
			% foundObject->GetBitSize()
			% size
			% (std::uint32_t) this->GetNodeId();
			LOG_ERROR() << formatter.str();
			return Result(ErrorCode::OBJECT_SIZE_MAPPED_INVALID, formatter.str());
		}
		//Correct size if a parameter has changed the size of the PI
		else if (mapping_size != size && foundObject->GetBitSize() == size)
		{
			mapping->SetMappingLength(size);
			mapping->GetRelatedObject()->SetTypedObjectActualValue(mapping->ToString(true));
		}

		if (dir == Direction::RX)
		{
			//Check complex data type access
			if (access == ParameterAccess::readWriteInput
			        || access == ParameterAccess::read)
			{
				//Parameter access does not match mapping direction
				boost::format formatter(kMsgAccessTypeForParameterInvalid[static_cast<std::underlying_type<Language>::type>(LoggingConfiguration::GetInstance().GetCurrentLanguage())]);
				formatter
				% uniqueID
				% dataIndex
				% dataSubindex
				% (std::uint32_t) this->GetNodeId()
				% ParameterAccessValues[static_cast<std::underlying_type<ParameterAccess>::type>(access)];
				LOG_ERROR() << formatter.str();
				return Result(ErrorCode::PARAMETER_ACCESS_INVALID, formatter.str());
			}
		}
		else if (dir == Direction::TX)
		{
			if (access == ParameterAccess::readWriteOutput
			        || access == ParameterAccess::write)
			{
				//Parameter access does not match mapping direction
				boost::format formatter(kMsgAccessTypeForParameterInvalid[static_cast<std::underlying_type<Language>::type>(LoggingConfiguration::GetInstance().GetCurrentLanguage())]);
				formatter
				% uniqueID
				% dataIndex
				% dataSubindex
				% (std::uint32_t) this->GetNodeId()
				% ParameterAccessValues[static_cast<std::underlying_type<ParameterAccess>::type>(access)];
				LOG_ERROR() << formatter.str();
				return Result(ErrorCode::PARAMETER_ACCESS_INVALID, formatter.str());
			}
		}
	}

	//Check mapping offset
	if (mapping->GetMappingOffset() < expectedOffset) //Allow offset gaps
	{
		boost::format formatter(kMsgPdoOffsetInvalid[static_cast<std::underlying_type<Language>::type>(LoggingConfiguration::GetInstance().GetCurrentLanguage())]);
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
				boost::format formatter(kMsgMappingTypeForPdoInvalid[static_cast<std::underlying_type<Language>::type>(LoggingConfiguration::GetInstance().GetCurrentLanguage())]);
				formatter
				% dataIndex
				% dataSubindex
				% (std::uint32_t) this->GetNodeId()
				% PDOMappingValues[static_cast<std::underlying_type<PDOMapping>::type>(foundObject->GetPDOMapping().get())];
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
				boost::format formatter(kMsgMappingTypeForPdoInvalid[static_cast<std::underlying_type<Language>::type>(LoggingConfiguration::GetInstance().GetCurrentLanguage())]);
				formatter
				% dataIndex
				% dataSubindex
				% (std::uint32_t) this->GetNodeId()
				% PDOMappingValues[static_cast<std::underlying_type<PDOMapping>::type>(foundObject->GetPDOMapping().get())];
				LOG_ERROR() << formatter.str();
				return Result(ErrorCode::MAPPING_TYPE_FOR_PDO_INVALID, formatter.str());
			}
		}
	}
	else
	{
		boost::format formatter(kMsgMappingTypeForPdoInvalid[static_cast<std::underlying_type<Language>::type>(LoggingConfiguration::GetInstance().GetCurrentLanguage())]);
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
				boost::format formatter(kMsgAccessTypeForPdoInvalid[static_cast<std::underlying_type<Language>::type>(LoggingConfiguration::GetInstance().GetCurrentLanguage())]);
				formatter
				% dataIndex
				% dataSubindex
				% (std::uint32_t) this->GetNodeId()
				% AccessTypeValues[static_cast<std::underlying_type<AccessType>::type>(foundObject->GetAccessType().get())];
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
				boost::format formatter(kMsgAccessTypeForPdoInvalid[static_cast<std::underlying_type<Language>::type>(LoggingConfiguration::GetInstance().GetCurrentLanguage())]);
				formatter
				% dataIndex
				% dataSubindex
				% (std::uint32_t) this->GetNodeId()
				% AccessTypeValues[static_cast<std::underlying_type<AccessType>::type>(foundObject->GetAccessType().get())];
				LOG_ERROR() << formatter.str();
				return Result(ErrorCode::ACCESS_TYPE_FOR_PDO_INVALID, formatter.str());
			}
		}
	}
	else if (!foundObject->GetUniqueIdRef().is_initialized()) //Domain object may not have an access type
	{
		boost::format formatter(kMsgAccessTypeForPdoInvalid[static_cast<std::underlying_type<Language>::type>(LoggingConfiguration::GetInstance().GetCurrentLanguage())]);
		formatter
		% dataIndex
		% dataSubindex
		% (std::uint32_t) this->GetNodeId()
		% AccessTypeValues[static_cast<std::underlying_type<AccessType>::type>(foundObject->GetAccessType().get())];
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

std::uint32_t ControlledNode::GetNodeDataPresMnOffset() const
{
	return this->nodeDataPresMnOffset;
}

std::uint32_t ControlledNode::GetNodeDataPresMnCurrentOffset() const
{
	return this->nodeDataPresMnCurrentOffset;
}

Result ControlledNode::MoveMappingObject(const Direction& dir, std::uint16_t channelNr, std::uint16_t oldPosition, std::uint16_t newPosition)
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

bool ControlledNode::ReceivesPResMN() const
{
	return this->receivesPResMN;
}

Result ControlledNode::ProcessParameterGroup(const std::string& paramName, const std::shared_ptr<BaseProcessDataMapping>& mappingObject, const std::shared_ptr<ParameterGroup>& grp, const std::string& dataName, const Direction& dir, std::uint32_t& piOffset, std::uint32_t domainCount)
{
	std::uint32_t bitOffset = 0;
	for (auto& paramGrpEntry : grp->GetParameterGroupEntries())
	{
		auto paramGrp = std::dynamic_pointer_cast<ParameterGroup>(paramGrpEntry);
		auto paramRef = std::dynamic_pointer_cast<ParameterReference>(paramGrpEntry);
		if (paramGrp)
		{
			if (paramGrp->CheckParameterGroupCondition())
			{
				if (paramGrp->GetBitOffset() % 8 == 0)
				{
					piOffset += paramGrp->GetBitOffset() / 8;
					bitOffset = 0;
				}
				ProcessParameterGroup(paramName, mappingObject, paramGrp, dataName, dir, piOffset, domainCount);
			}
		}
		else if (paramRef)
		{
			if (paramRef->GetReferencedParameter()->GetComplexDataType())
			{
				std::shared_ptr<StructDataType> structDt = std::dynamic_pointer_cast<StructDataType>(paramRef->GetReferencedParameter()->GetComplexDataType());
				std::shared_ptr<ArrayDataType> arrayDt = std::dynamic_pointer_cast<ArrayDataType>(paramRef->GetReferencedParameter()->GetComplexDataType());
				if (structDt.get())
				{
					ProcessComplexDatatype(paramName, mappingObject, structDt, dataName, dir, piOffset, domainCount);
				}
				else if (arrayDt.get())
				{
					ProcessComplexDatatype(paramName, mappingObject, arrayDt, dataName, dir, piOffset, domainCount);
				}
			}
			else if (paramRef->GetReferencedParameter()->GetDataType().is_initialized())
				ProcessComplexDatatype(mappingObject, paramRef->GetReferencedParameter(), dataName, dir, piOffset, bitOffset, domainCount);
		}
	}
	return Result();
}

void ControlledNode::ProcessComplexDatatype(const std::shared_ptr<BaseProcessDataMapping>& mappingObject, const std::shared_ptr<Parameter>& param, const std::string& dataName, const Direction& dir, std::uint32_t& piOffset, std::uint32_t& bitOffset, std::uint32_t domainCount)
{
	std::stringstream nameBuilder;
	nameBuilder << "CN" << (std::uint32_t) this->GetNodeId();
	nameBuilder << "_";
	nameBuilder << "DOM";
	nameBuilder << IntToHex<std::uint32_t>(domainCount, 2);
	nameBuilder << "_";
	nameBuilder << dataName;
	nameBuilder << "_";
	nameBuilder << param->GetUniqueID();

	std::string piName = nameBuilder.str();
	boost::trim(piName);
	boost::remove_erase_if(piName, boost::is_any_of("!\\/(){}[],*'"));
	std::replace(piName.begin(), piName.end(), ' ', '_');
	std::replace(piName.begin(), piName.end(), '.', '_');

	if (bitOffset == 8)
	{
		piOffset++;
		bitOffset = 0;
	}

	if (param->GetDataType().get() == IEC_Datatype::BITSTRING || param->GetDataType().get() == IEC_Datatype::BOOL)
	{
		std::shared_ptr<BaseProcessImageObject> piObj = std::make_shared<BaseProcessImageObject>(
		            piName,
		            param->GetDataType().get(),
		            piOffset,
		            bitOffset,
		            GetIECDataTypeBitSize(param->GetDataType().get()));
		piObj->SetMappingObjectIndex(mappingObject->GetObject());
		piObj->SetMappingObjectSubIndex(mappingObject->GetSubObject());
		piObj->SetMappingObjectParameter(param->GetUniqueID());
		piObj->SetSourceNodeId(this->GetNodeId());

		if (dir == Direction::RX)
			this->GetReceiveProcessImage().push_back(piObj);
		else if (dir == Direction::TX)
			this->GetTransmitProcessImage().push_back(piObj);
		bitOffset += GetIECDataTypeBitSize(param->GetDataType().get());
	}
	else
	{
		if (bitOffset != 0)
		{
			piOffset++;
			bitOffset = 0;
		}

		std::shared_ptr<BaseProcessImageObject> piObj = std::make_shared<BaseProcessImageObject>(
		            piName,
		            param->GetDataType().get(),
		            piOffset,
		            GetIECDataTypeBitSize(param->GetDataType().get()));
		piObj->SetMappingObjectIndex(mappingObject->GetObject());
		piObj->SetMappingObjectSubIndex(mappingObject->GetSubObject());
		piObj->SetMappingObjectParameter(param->GetUniqueID());
		piObj->SetSourceNodeId(this->GetNodeId());

		if (dir == Direction::RX)
			this->GetReceiveProcessImage().push_back(piObj);
		else if (dir == Direction::TX)
			this->GetTransmitProcessImage().push_back(piObj);
		piOffset += GetIECDataTypeBitSize(param->GetDataType().get()) / 8 ;
	}
}

void ControlledNode::ProcessComplexDatatype(const std::string& paramName, const std::shared_ptr<BaseProcessDataMapping>& mappingObject, const std::shared_ptr<StructDataType>& structDt, const std::string& dataName, const Direction& dir, std::uint32_t& piOffset, std::uint32_t domainCount)
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
		nameBuilder << varDecl->GetName();

		std::string piName = nameBuilder.str();
		boost::trim(piName);
		boost::remove_erase_if(piName, boost::is_any_of("!\\/(){}[],*'"));
		std::replace(piName.begin(), piName.end(), ' ', '_');
		std::replace(piName.begin(), piName.end(), '.', '_');
		if (varDecl->GetDataType() == IEC_Datatype::BITSTRING || varDecl->GetDataType() == IEC_Datatype::BOOL)
		{
			std::shared_ptr<BaseProcessImageObject> piObj = std::make_shared<BaseProcessImageObject>(
			            piName,
			            varDecl->GetDataType(),
			            piOffset, bitOffset,
			            varDecl->GetBitSize());
			piObj->SetMappingObjectIndex(mappingObject->GetObject());
			piObj->SetMappingObjectSubIndex(mappingObject->GetSubObject());
			piObj->SetMappingObjectParameter(paramName);
			piObj->SetSourceNodeId(this->GetNodeId());

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
			piObj->SetMappingObjectIndex(mappingObject->GetObject());
			piObj->SetMappingObjectSubIndex(mappingObject->GetSubObject());
			piObj->SetMappingObjectParameter(paramName);
			piObj->SetSourceNodeId(this->GetNodeId());

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

void ControlledNode::ProcessComplexDatatype(const std::string& paramName, const std::shared_ptr<BaseProcessDataMapping>& mappingObject, const std::shared_ptr<ArrayDataType>& arrayDt, const std::string& dataName, const Direction& dir, std::uint32_t& piOffset, std::uint32_t domainCount)
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
		std::replace(piName.begin(), piName.end(), '.', '_');
		if (arrayDt->GetDataType() == IEC_Datatype::BITSTRING || arrayDt->GetDataType() == IEC_Datatype::BOOL)
		{
			std::shared_ptr<BaseProcessImageObject> piObj = std::make_shared<BaseProcessImageObject>(
			            piName,
			            arrayDt->GetDataType(),
			            piOffset,
			            bitOffset,
			            arrayDt->GetBitSize());
			piObj->SetMappingObjectIndex(mappingObject->GetObject());
			piObj->SetMappingObjectSubIndex(mappingObject->GetSubObject());
			piObj->SetMappingObjectParameter(paramName);
			piObj->SetSourceNodeId(this->GetNodeId());

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
			piObj->SetMappingObjectIndex(mappingObject->GetObject());
			piObj->SetMappingObjectSubIndex(mappingObject->GetSubObject());
			piObj->SetMappingObjectParameter(paramName);
			piObj->SetSourceNodeId(this->GetNodeId());

			this->GetTransmitProcessImage().push_back(piObj);
			piOffset += arrayDt->GetBitSize() / 8 ;
		}
	}
}

