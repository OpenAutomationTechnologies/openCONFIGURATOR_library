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
	nodeDataPresMnOffset(0)
{
	this->AddNodeAssignement(NodeAssignment::MNT_NODEASSIGN_VALID);
	this->AddNodeAssignement(NodeAssignment::NMT_NODEASSIGN_NODE_EXISTS);
	this->AddNodeAssignement(NodeAssignment::NMT_NODEASSIGN_NODE_IS_CN);
	this->AddNodeAssignement(NodeAssignment::NMT_NODEASSIGN_START_CN);
}

ControlledNode::~ControlledNode()
{}

Result ControlledNode::AddNodeAssignement(NodeAssignment assign)
{
	if (assign == NodeAssignment::NMT_NODEASSIGN_MN_PRES)
	{
		boost::format formatter(kMsgNodeAssignmentNotSupported);
		formatter
		% (std::uint32_t) assign
		% (std::uint32_t) this->GetNodeId();
		LOG_FATAL() << formatter.str();
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
		LOG_WARN() << formatter.str();
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

Result ControlledNode::MapObject(std::uint32_t index, const Direction dir, std::uint32_t position, std::uint16_t fromNode)
{
	//Retrieve object to be mapped
	std::shared_ptr<Object> objToMap;
	Result res = this->GetObject(index, objToMap);
	if (!res.IsSuccessful())
		return res;

	//Map object
	return this->MapBaseObject(objToMap, index, 0, dir, false, position, fromNode);
}

Result ControlledNode::MapSubObject(std::uint32_t index, std::uint16_t subindex, const Direction dir, std::uint32_t position, std::uint16_t fromNode)
{
	//retrieve sub object to be mapped
	std::shared_ptr<SubObject> objToMap;
	Result res = this->GetSubObject(index, subindex, objToMap);
	if (!res.IsSuccessful())
		return res;

	//Map subobject
	return this->MapBaseObject(objToMap, index, subindex, dir, false, position, fromNode);
}

Result ControlledNode::MapBaseObject(const std::shared_ptr<BaseObject>& objToMap, std::uint32_t index, std::uint16_t subindex, const Direction dir, bool updateNrOfEntries, std::uint32_t position, std::uint16_t fromNode)
{
	//Set fromNode for PresChained nodes
	if (dir == Direction::RX
	        && this->GetOperationMode() == PlkOperationMode::CHAINED
	        && fromNode == 0)
	{
		fromNode = 240;
	}

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
				LOG_FATAL() << formatter.str();
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
				LOG_FATAL() << formatter.str();
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
			        && objToMap->GetPDOMapping().get() != PDOMapping::OPTIONAL)
			{
				boost::format formatter(kMsgMappingTypeForPdoInvalid);
				formatter
				% index
				% subindex
				% (std::uint32_t) this->GetNodeId()
				% PDOMappingValues[(std::uint8_t) objToMap->GetPDOMapping().get()];
				LOG_FATAL() << formatter.str();
				return Result(ErrorCode::MAPPING_TYPE_FOR_PDO_INVALID, formatter.str());
			}
		}
		else if (dir == Direction::TX)
		{
			if (objToMap->GetPDOMapping().get() != PDOMapping::TPDO
			        && objToMap->GetPDOMapping().get() != PDOMapping::OPTIONAL)
			{
				boost::format formatter(kMsgMappingTypeForPdoInvalid);
				formatter
				% index
				% subindex
				% (std::uint32_t) this->GetNodeId()
				% PDOMappingValues[(std::uint8_t) objToMap->GetPDOMapping().get()];
				LOG_FATAL() << formatter.str();
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
		LOG_FATAL() << formatter.str();
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
				LOG_FATAL() << formatter.str();
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
				LOG_FATAL() << formatter.str();
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
		LOG_FATAL() << formatter.str();
		return Result(ErrorCode::MAPPING_TYPE_FOR_PDO_INVALID, formatter.str());
	}

	//Calculate mapping parameter
	std::uint32_t mappingObjectIndex = 0;
	std::uint32_t mappingParameterIndex = 0x1400;

	if (dir == Direction::RX)
	{
		bool mappingParameterFound = false;
		//Find appropriate index for the node
		//default mapping from managing node
		bool writeNewMappingParameterObject = false;
		std::shared_ptr<SubObject> mappingParameter;
		//Traverse 0x1400 - 0x14FF
		for (; mappingParameterIndex < 0x1500; mappingParameterIndex++)
		{
			//Get SubObject 0x1 "NodeID"
			std::shared_ptr<SubObject> subObj;
			Result res = this->GetSubObject(mappingParameterIndex, 0x1, subObj);
			if (!res.IsSuccessful())
				continue;

			//Has actual node id value
			if (subObj->WriteToConfiguration())
			{
				//Get node is value
				std::uint16_t nodeId = subObj->GetTypedActualValue<uint16_t>();
				//If node ids match calculate mapping object index
				if (fromNode == nodeId)
				{
					mappingObjectIndex = (mappingParameterIndex - 0x1400) + 0x1600;
					writeNewMappingParameterObject = false; //Correct
					mappingParameterFound = true;
					break;
				}
			}
			//Take the first available mapping index usually 0x1600 for default TPDO mapping
			else if (subObj->WriteToConfiguration() == false && fromNode == 0)
			{
				mappingObjectIndex = (mappingParameterIndex - 0x1400) + 0x1600;
				writeNewMappingParameterObject = false;
				mappingParameterFound = true;
				break;
			}
			//Find and store the first available if no appropriate mapping parameter object found
			else if (subObj->WriteToConfiguration() == false
			         && fromNode != 0
			         && writeNewMappingParameterObject == false) //Take the first available mapping index for a node
			{
				mappingObjectIndex = (mappingParameterIndex - 0x1400) + 0x1600; //Store this index if no appropriate one is found
				writeNewMappingParameterObject = true;
				mappingParameterFound = true;
				mappingParameter = subObj;
			}
		}

		//Set node id for new mapping parameter
		if (writeNewMappingParameterObject)
		{
			std::stringstream convert;
			convert << fromNode;
			mappingParameter->SetTypedObjectActualValue(convert.str());
			mappingParameterFound = true;
		}

		//No mapping parameter has been found or is available
		if (!mappingParameterFound)
		{
			return Result(ErrorCode::INSUFFICIENT_MAPPING_OBJECTS);
		}
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
	std::uint32_t offset = 0; // Mapping offset
	std::uint32_t expectedOffset = 0; //calculated mapping offset with object + datasize

	if (dir == Direction::RX
	        && this->GetOperationMode() == PlkOperationMode::CHAINED
	        && fromNode == 0)
	{
		offset = this->nodeDataPresMnOffset; // start with Offset from the MN
		expectedOffset = this->nodeDataPresMnOffset;
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
			if (tMapping.second->WriteToConfiguration())
			{
				BaseProcessDataMapping object = BaseProcessDataMapping(tMapping.second->GetTypedActualValue<std::string>(), this->GetNodeId(), false);
				offset = object.GetMappingOffset();
			}

			if (offset < expectedOffset)
				offset = expectedOffset;

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
			continue;

		}

		// position not reached calculate the offsets
		// skip empty mapping objects
		if (tMapping.second->WriteToConfiguration())
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
			}
		}
	}

	//update nr of entries of the mapping object
	if (updateNrOfEntries)
	{
		std::shared_ptr<SubObject> nrOfEntriesObj;
		res = mappingObj->GetSubObject(0, nrOfEntriesObj); //Get Subobject NrOfEntries
		if (!res.IsSuccessful())
			return res;

		std::stringstream convert;
		convert << nrOfEntries - 1;
		nrOfEntriesObj->SetTypedObjectActualValue(convert.str()); //reduce because of sub index 0
	}

	return res;
}

Result ControlledNode::MapAllRxObjects(bool updateNrOfEntries)
{
	//start on position 1
	std::uint32_t position = 1;

	for (auto& obj : this->GetObjectDictionary())
	{
		if (obj.second->GetPDOMapping().is_initialized())
		{
			if (obj.second->GetPDOMapping() == PDOMapping::RPDO)
			{
				Result res = this->MapBaseObject(obj.second, obj.first, 0, Direction::RX, updateNrOfEntries, position, 0);
				if (!res.IsSuccessful())
					return res;
				position++;
			}
		}

		for (auto& subobj : obj.second->GetSubObjectDictionary())
		{
			if (!subobj.second->GetPDOMapping().is_initialized())
				continue;

			if (subobj.second->GetPDOMapping() == PDOMapping::RPDO)
			{
				Result res = this->MapBaseObject(subobj.second, obj.first, (std::uint16_t) subobj.first, Direction::RX, updateNrOfEntries, position, 0);
				if (!res.IsSuccessful())
					return res;
				position++;
			}
		}
	}
	return Result();
}

Result ControlledNode::MapAllTxObjects(bool updateNrOfEntries)
{
	//start on position 1
	std::uint32_t position = 1;

	for (auto& obj : this->GetObjectDictionary())
	{
		if (obj.second->GetPDOMapping().is_initialized())
		{
			if (obj.second->GetPDOMapping() == PDOMapping::TPDO)
			{
				Result res = this->MapBaseObject(obj.second, obj.first, 0, Direction::TX, updateNrOfEntries, position, 0);
				if (!res.IsSuccessful())
					return res;
				position++;
			}
		}

		for (auto& subobj : obj.second->GetSubObjectDictionary())
		{
			if (!subobj.second->GetPDOMapping().is_initialized())
				continue;

			if (subobj.second->GetPDOMapping() == PDOMapping::TPDO)
			{
				Result res = this->MapBaseObject(subobj.second, obj.first, (std::uint16_t) subobj.first, Direction::TX, updateNrOfEntries, position, 0);
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
			boost::format formatter(kMsgMultiplexingNotSupported);
			formatter
			% (std::uint32_t) this->GetNodeId();
			LOG_FATAL() << formatter.str();
			return Result(ErrorCode::MULTIPLEXING_NOT_SUPPORTED, formatter.str());
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
			boost::format formatter(kMsgChainingNotSupported);
			formatter
			% (std::uint32_t) this->GetNodeId();
			LOG_FATAL() << formatter.str();
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
			auto& nrOfEntriesObj = object.second->GetSubObjectDictionary().at((std::uint8_t) 0); //GetNrOfEntries and only count the valid ones
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
		std::stringstream convert;
		convert << preqPayloadLimit;
		return this->SetSubObjectActualValue(0x1F98, 0x4, convert.str());
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
			auto& nrOfEntriesObj = object.second->GetSubObjectDictionary().at((std::uint8_t) 0);
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
		std::stringstream convert;
		convert << presPayloadLimit;
		return this->SetSubObjectActualValue(0x1F98, 0x5, convert.str());
	}
}

Result ControlledNode::GetDataObjectFromMapping(const std::shared_ptr<BaseProcessDataMapping>& mapping, std::shared_ptr<BaseObject>& returnObject)
{
	std::uint32_t dataIndex = mapping->GetMappingIndex();
	std::uint16_t dataSubindex = mapping->GetMappingSubIndex();

	std::shared_ptr<Object> dataObject;
	std::shared_ptr<SubObject> dataSubObject;
	if (dataSubindex == 0)
	{
		Result res = this->GetSubObject(dataIndex, dataSubindex, dataSubObject);
		if (!res.IsSuccessful())
		{
			res = this->GetObject(dataIndex, dataObject);
			if (!res.IsSuccessful())
			{
				boost::format formatter(kMsgNonExistingMappedObject);
				formatter
				% dataIndex
				% (std::uint32_t) this->GetNodeId();
				LOG_FATAL() << formatter.str();
				return Result(ErrorCode::MAPPED_OBJECT_DOES_NOT_EXIST, formatter.str());
			}
			else
				returnObject = dataObject;
		}
		else
			returnObject = dataSubObject;
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
			LOG_FATAL() << formatter.str();
			return Result(ErrorCode::MAPPED_SUBOBJECT_DOES_NOT_EXIST, formatter.str());
		}
		returnObject = dataSubObject;
	}
	return Result();
}

Result ControlledNode::UpdateProcessImage(Direction dir)
{
	this->UpdateProcessDataMapping(dir);

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
		Result res = GetDataObjectFromMapping(mapObj, dataObject);
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
				std::uint32_t varCount = 0;
				for (auto& varDecl : structDt->GetVarDeclarations())
				{
					std::stringstream nameBuilder;
					nameBuilder << "CN" << IntToHex<std::uint32_t>((std::uint32_t) this->GetNodeId(), 2);
					nameBuilder << ".";
					nameBuilder << "DOM";
					nameBuilder << IntToHex<std::uint32_t>(domainCount, 2);
					nameBuilder	<< ".";
					nameBuilder << "VAR";
					nameBuilder << IntToHex<std::uint32_t>(varCount, 2);
					nameBuilder	<< ".";
					nameBuilder << structDt->GetName();
					nameBuilder << ".";
					nameBuilder << varDecl->GetName();

					if (varDecl->GetDataType() == IEC_Datatype::BITSTRING)
					{
						std::shared_ptr<BaseProcessImageObject> piObj = std::make_shared<BaseProcessImageObject>(
						            nameBuilder.str(),
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
						            nameBuilder.str(),
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
					varCount++;
				}
			}
			else if (arrayDt.get())
			{
				std::uint32_t bitOffset = 0;
				for (std::uint32_t i = arrayDt->GetLowerLimit(); i < arrayDt->GetUpperLimit(); i++)
				{
					std::stringstream nameBuilder;
					nameBuilder << "CN" << IntToHex<std::uint32_t>((std::uint32_t) this->GetNodeId(), 2);
					nameBuilder << ".";
					nameBuilder << "DOM";
					nameBuilder << IntToHex<std::uint32_t>(domainCount, 2);
					nameBuilder	<< ".";
					nameBuilder << "VAR";
					nameBuilder << IntToHex<std::uint32_t>(i, 2);
					nameBuilder	<< ".";
					nameBuilder << arrayDt->GetName();

					if (arrayDt->GetDataType() == IEC_Datatype::BITSTRING)
					{
						std::shared_ptr<BaseProcessImageObject> piObj = std::make_shared<BaseProcessImageObject>(
						            nameBuilder.str(),
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
						            nameBuilder.str(),
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
			std::shared_ptr<BaseProcessImageObject> piObj = std::make_shared<BaseProcessImageObject>(
			            dataObject->GetName(),
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
			bool updateOffset = false;
			//Get mapping parameter object
			std::shared_ptr<SubObject> nodeID;
			Result res = obj.second->GetSubObject(0x1, nodeID);
			if (!res.IsSuccessful())
				return res;

			if (nodeID->WriteToConfiguration())
				mappedFromNode = nodeID->GetTypedActualValue<std::uint16_t>();

			if (this->GetOperationMode() == PlkOperationMode::CHAINED
			        && mappedFromNode == 240)
			{
				expectedOffset = this->nodeDataPresMnOffset;
				updateOffset = true;
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
				nrOfEntries = nrOfEntriesObj->GetTypedActualValue<std::uint16_t>();
			}
			else if (nrOfEntriesObj->HasDefaultValue())
			{
				nrOfEntries = nrOfEntriesObj->GetTypedDefaultValue<std::uint16_t>();
			}

			for (auto& mapping : mappingObject->GetSubObjectDictionary())
			{
				if (mapping.first == 0)
					continue;

				if (nrOfEntries == 0)
					break;

				if (mapping.second->WriteToConfiguration()
				        && mapping.second->GetTypedActualValue<std::uint64_t>() != 0)
				{
					std::shared_ptr<BaseProcessDataMapping> mappingPtr = std::shared_ptr<BaseProcessDataMapping>(new BaseProcessDataMapping(mappingObject->GetObjectId(),
					        mapping.second->GetObjectId(), mapping.second, mapping.second->GetTypedActualValue<std::string>(), mappedFromNode, false));

					if (updateOffset)
					{
						mappingPtr->SetMappingOffset(expectedOffset);
						mapping.second->SetTypedObjectActualValue(IntToHex<std::uint64_t>(mappingPtr->GetValue(), 16, "0x"));
					}

					res = CheckProcessDataMapping(mappingPtr, expectedOffset, dir);
					if (!res.IsSuccessful())
						return res;

					if (dir == Direction::RX)
						this->GetReceiveMapping().push_back(mappingPtr);
					else if (dir == Direction::TX)
						this->GetTransmitMapping().push_back(mappingPtr);

					nrOfEntries--;
					expectedOffset += mappingPtr->GetMappingLength();
				}
				else if (mapping.second->HasDefaultValue()
				         && mapping.second->GetTypedDefaultValue<std::uint64_t>() != 0)
				{
					std::shared_ptr<BaseProcessDataMapping> mappingPtr = std::shared_ptr<BaseProcessDataMapping>(new BaseProcessDataMapping(mappingObject->GetObjectId(),
					        mapping.second->GetObjectId(), mapping.second, mapping.second->GetTypedDefaultValue<std::string>(), mappedFromNode, true));

					if (updateOffset)
					{
						mappingPtr->SetMappingOffset(expectedOffset);
						mapping.second->SetTypedObjectActualValue(IntToHex<std::uint64_t>(mappingPtr->GetValue(), 16, "0x"));
					}

					res = CheckProcessDataMapping(mappingPtr, expectedOffset, dir);
					if (!res.IsSuccessful())
						return res;

					if (dir == Direction::RX)
						this->GetReceiveMapping().push_back(mappingPtr);
					else if (dir == Direction::TX)
						this->GetTransmitMapping().push_back(mappingPtr);

					nrOfEntries--;
					expectedOffset += mappingPtr->GetMappingLength();
				}
				else
				{
					return Result(ErrorCode::MAPPING_INVALID);
				}
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
		Result res = this->GetSubObject(dataIndex, dataSubindex, dataSubObject);
		if (!res.IsSuccessful())
		{
			res = this->GetObject(dataIndex, dataObject);
			if (!res.IsSuccessful())
			{
				boost::format formatter(kMsgNonExistingMappedObject);
				formatter
				% dataIndex
				% (std::uint32_t) this->GetNodeId();
				LOG_FATAL() << formatter.str();
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
			LOG_FATAL() << formatter.str();
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
				LOG_FATAL() << formatter.str();
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
				LOG_FATAL() << formatter.str();
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
		LOG_FATAL() << formatter.str();
		return Result(ErrorCode::PDO_OFFSET_INVALID, formatter.str());
	}

	//Check PDOMapping
	if (foundObject->GetPDOMapping().is_initialized())
	{
		if (dir == Direction::RX)
		{
			if (foundObject->GetPDOMapping().get() != PDOMapping::RPDO
			        && foundObject->GetPDOMapping().get() != PDOMapping::OPTIONAL)
			{
				boost::format formatter(kMsgMappingTypeForPdoInvalid);
				formatter
				% dataIndex
				% dataSubindex
				% (std::uint32_t) this->GetNodeId()
				% PDOMappingValues[(std::uint8_t) foundObject->GetPDOMapping().get()];
				LOG_FATAL() << formatter.str();
				return Result(ErrorCode::MAPPING_TYPE_FOR_PDO_INVALID, formatter.str());
			}
		}
		else if (dir == Direction::TX)
		{
			if (foundObject->GetPDOMapping().get() != PDOMapping::TPDO
			        && foundObject->GetPDOMapping().get() != PDOMapping::OPTIONAL)
			{
				boost::format formatter(kMsgMappingTypeForPdoInvalid);
				formatter
				% dataIndex
				% dataSubindex
				% (std::uint32_t) this->GetNodeId()
				% PDOMappingValues[(std::uint8_t) foundObject->GetPDOMapping().get()];
				LOG_FATAL() << formatter.str();
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
		LOG_FATAL() << formatter.str();
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
				LOG_FATAL() << formatter.str();
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
				LOG_FATAL() << formatter.str();
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
		LOG_FATAL() << formatter.str();
		return Result(ErrorCode::ACCESS_TYPE_FOR_PDO_INVALID, formatter.str());
	}
	return Result();
}

void ControlledNode::SetNodeDataPresMnOffset(std::uint32_t offset)
{
	this->nodeDataPresMnOffset = offset;
}