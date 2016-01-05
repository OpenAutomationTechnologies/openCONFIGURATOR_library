/************************************************************************
\file ManagingNodeMappingBuilder.cpp
\brief Implementation of the Class ManagingNodeMappingBuilder
\author rueckerc, Bernecker+Rainer Industrie Elektronik Ges.m.b.H.
\date 27-Apr-2015 09:38:09
************************************************************************/

/*------------------------------------------------------------------------------
Copyright (c) 2014, Bernecker+Rainer Industrie-Elektronik Ges.m.b.H. (B&R)
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
#include "ManagingNodeMappingBuilder.h"

using namespace IndustrialNetwork::POWERLINK::Core::Configuration;
using namespace IndustrialNetwork::POWERLINK::Core::ErrorHandling;
using namespace IndustrialNetwork::POWERLINK::Core::CoreConfiguration;
using namespace IndustrialNetwork::POWERLINK::Core::Node;
using namespace IndustrialNetwork::POWERLINK::Core::ObjectDictionary;
using namespace IndustrialNetwork::POWERLINK::Core::Utilities;

ManagingNodeMappingBuilder::ManagingNodeMappingBuilder() : BuildConfigurationSettingBuilder(),
	inputOffsets(std::make_shared<std::map<PlkDataType, std::uint32_t>>()),
	outputOffsets(std::make_shared<std::map<PlkDataType, std::uint32_t>>())
{}

ManagingNodeMappingBuilder::~ManagingNodeMappingBuilder(void)
{
	inputOffsets->clear();
	outputOffsets->clear();
}

Result ManagingNodeMappingBuilder::GenerateConfiguration(const std::string& value, const std::map<std::uint8_t, std::shared_ptr<BaseNode>>& nodeCollection)
{
	//Get Managing Node (this must not fail)
	const std::shared_ptr<ManagingNode>& mn = std::dynamic_pointer_cast<ManagingNode>(nodeCollection.at(240));
	mn->ClearMappingObjects(); //clear MN mapping
	inputOffsets->clear();
	outputOffsets->clear();

	//Generate MN RX Mapping
	Result res = GenerateMnMapping(value, Direction::RX, nodeCollection);
	if (!res.IsSuccessful())
		return res;

	//Generate MN TX Mapping
	return GenerateMnMapping(value, Direction::TX, nodeCollection);
}

Result ManagingNodeMappingBuilder::GenerateMnMapping(const std::string& value, Direction dir, const std::map<std::uint8_t, std::shared_ptr<BaseNode>>& nodeCollection)
{
	//Get Managing Node (this must not fail)
	const std::shared_ptr<ManagingNode>& mn = std::dynamic_pointer_cast<ManagingNode>(nodeCollection.at(240));

	//Init offsets
	for (auto& dynChannel : mn->GetDynamicChannels())
	{
		//input PI
		if (dynChannel->GetAccessType() == DynamicChannelAccessType::readOnly)
		{
			inputOffsets->insert(std::make_pair(dynChannel->GetDataType(), 0));
		}
		//output PI
		else if (dynChannel->GetAccessType() == DynamicChannelAccessType::writeOnly)
		{
			outputOffsets->insert(std::make_pair(dynChannel->GetDataType(), 0));
		}
		else if (dynChannel->GetAccessType() == DynamicChannelAccessType::readWriteOutput)
		{
			if (inputOffsets->find(dynChannel->GetDataType()) != inputOffsets->end())
				outputOffsets->insert(std::make_pair(dynChannel->GetDataType(), 0));
			else
				inputOffsets->insert(std::make_pair(dynChannel->GetDataType(), 0));
		}
	}

	//Direction from node point of view
	Direction nodeDir = Direction::TX;
	if (dir == Direction::TX)
		nodeDir = Direction::RX;

	Result res;
	std::uint32_t channelBitOffset = 0;
	std::uint32_t presMnOffset = 0;
	std::uint32_t index = 0;
	std::uint32_t subindex = 1;
	bool cnReceivePresMN = false;

	//Sort PResMN first
	std::vector<std::shared_ptr<BaseNode>> sortedCollection;
	if (dir == Direction::TX)
	{
		for (auto& node : nodeCollection)
		{
			//Cast pointer to CN
			std::shared_ptr<ControlledNode> cn = std::dynamic_pointer_cast<ControlledNode>(node.second);
			if (!cn)
				continue;

			if (cn->ReceivesPResMN() || cn->GetOperationMode() == PlkOperationMode::CHAINED)
				sortedCollection.push_back(node.second);

		}
	}
	for (auto& node : nodeCollection)
	{
		//Cast pointer to CN
		std::shared_ptr<ControlledNode> cn = std::dynamic_pointer_cast<ControlledNode>(node.second);
		if (!cn)
		{
			sortedCollection.push_back(node.second);
			continue;
		}

		if ((cn->ReceivesPResMN() == false && cn->GetOperationMode() != PlkOperationMode::CHAINED) || dir == Direction::RX)
			sortedCollection.push_back(node.second);
	}

	//Traverse all controlled nodes
	for (auto& node : sortedCollection)
	{
		//Continue if node is disabled
		if (node->IsEnabled() == false)
			continue;

		//Reset channel bit offset
		channelBitOffset = 0;

		if (!value.empty())
		{
			if (!this->GenerateForNode(value, node->GetNodeId())) //Check if values have to be generated
				continue;
		}

		//Cast pointer to CN
		std::shared_ptr<ControlledNode> cn = std::dynamic_pointer_cast<ControlledNode>(node); //Check if not RMN needs to be handled properly
		if (!cn)
			continue; //If RMN continue

		//TODO: Add RMN Handling

		std::uint32_t availableSubindices = 252; //from PI_SUB_INDEX_COUNT
		std::vector<std::shared_ptr<BaseProcessDataMapping>> mappingCollection;
		if (dir == Direction::RX)
			mappingCollection = cn->GetTransmitMapping();
		else if (dir == Direction::TX)
			mappingCollection = cn->GetReceiveMapping();

		//Set the PResMN offset once
		bool nodeOffsetSet = false;
		for (auto& mapping : mappingCollection)
		{
			if (dir == Direction::TX)
			{
				if ((mapping->GetDestinationNode() == 240 || cn->GetOperationMode() == PlkOperationMode::CHAINED)
				        && nodeOffsetSet == false)
				{
					//Set correct offset for CN receive PDO from MN
					cn->SetNodeDataPresMnOffset(presMnOffset);
					mn->AddNodeAssignment(NodeAssignment::NMT_NODEASSIGN_MN_PRES);
					cnReceivePresMN = true;

					//Update CN Mapping once with correct PResMN offset
					res = cn->UpdateProcessImage(nodeDir);
					if (!res.IsSuccessful())
						return res;

					nodeOffsetSet = true;
				}
				else if (mapping->GetDestinationNode() != 0
				         && mapping->GetDestinationNode() != 240) //Cross traffic mapping not from MN (neither from node '0' or '240')
					continue;
			}

			//Retrieve mapped object
			std::shared_ptr<BaseObject> foundObject;
			res = this->FindMappedObject(cn, mapping->GetMappingIndex(), mapping->GetMappingSubIndex(), foundObject);
			if (!res.IsSuccessful())
				return res;

			std::uint32_t bitoffset = 0;
			if (!foundObject->GetUniqueIdRef().is_initialized()
			        && foundObject->GetDataType().is_initialized()) //Mapping object has normal data type
			{
				//Get Dynamic Channel from MN for datatype and direction
				std::shared_ptr<DynamicChannel> dynChannel;
				res = mn->GetDynamicChannel(foundObject->GetDataType().get(), dir, dynChannel);
				if (!res.IsSuccessful())
					return res;

				//availableSubindices = dynChannel->GetMaxNumber() / (dynChannel->GetEndIndex() - dynChannel->GetStartIndex() + 1); //usually 254
				bitoffset = CalculateOffset(foundObject->GetDataType().get(), dir);

				//Get first index of dynamic channel
				index = dynChannel->GetStartIndex();
				if (bitoffset != 0) // not first run bitoffset is not zero
				{
					std::uint32_t byteLength = mapping->GetMappingLength() / 8; //Get Byte length of data mapped
					subindex = ((bitoffset / 8) / byteLength) + 1; //Get subindex for mapped data

					// if subindex exceeds nr of TPDO channel objects use next mapping object
					if (subindex > availableSubindices)
					{
						subindex--; //Decrement subindex
						std::uint32_t div = subindex / availableSubindices;
						index = dynChannel->GetStartIndex() + div; //Calculate new index
						subindex = (subindex % availableSubindices) + 1; //Calculate new subindex
					}
				}

				//MN PI is full
				if (index > dynChannel->GetEndIndex())
				{
					boost::format formatter(kMsgDynamicChannelExceeded);
					formatter
					% index
					% subindex
					% dynChannel->GetStartIndex()
					% dynChannel->GetEndIndex()
					% GetPlkDataTypeName(dynChannel->GetDataType())
					% DirectionTypeValues[(std::uint8_t) dir];
					LOG_WARN() << formatter.str();
				}

				std::uint32_t offsetToWrite = channelBitOffset;
				std::uint8_t nodeId = cn->GetNodeId();
				//Nodes output data mapped to PResMN
				if (mapping->GetDestinationNode() == 240 && dir == Direction::TX)
				{
					nodeId = 0;
					offsetToWrite = presMnOffset;
				}

				// Write mapping to MN object dictionary
				res = this->WriteMappingToForNode(nodeId, dir, this->GetMappingString(mapping->GetMappingLength(), offsetToWrite, subindex, index), mn);
				if (!res.IsSuccessful())
				{
					if (res.GetErrorType() == ErrorCode::CHANNEL_OBJECT_LIMIT_EXCEEDED)
					{
						boost::format formatter(kMsgChannelExceeded);
						formatter
						% DirectionTypeValues[(std::uint8_t) dir]
						% node->GetName()
						% (std::uint32_t) node->GetNodeId();
						LOG_WARN() << formatter.str();
						break;
					}
					else
						return res;
				}

				// Increment bitoffset with the length of the added mapping
				bitoffset += mapping->GetMappingLength();
				channelBitOffset += mapping->GetMappingLength();

				if (mapping->GetDestinationNode() == 240 && dir == Direction::TX)
				{
					presMnOffset += mapping->GetMappingLength();
				}
			}
			else if (foundObject->GetUniqueIdRef().is_initialized())
			{
				bool foundMapping = false;
				std::uint32_t bitCount = 0;
				std::uint32_t fillSize = 0;
				std::uint32_t size = mapping->GetMappingLength();

				//Get domain object parameter
				//std::shared_ptr<Parameter> param;
				//res = cn->GetApplicationProcess()->GetParameter(foundObject->GetUniqueIdRef().get(), param);
				//if (!res.IsSuccessful())
				//	return res;

				std::vector<std::shared_ptr<BaseProcessImageObject>> processImageCollection;
				if (dir == Direction::RX)
				{
					//CN network direction is TX
					processImageCollection = cn->GetTransmitProcessImage();
				}
				else if (dir == Direction::TX)
				{
					//CN network direction is RX
					processImageCollection = cn->GetReceiveProcessImage();
				}

				//Traverse process image
				for (auto& cnPIObject : processImageCollection)
				{
					//Check if CN PI object belongs to mapped domain
					if (cnPIObject->GetPiOffset() >= channelBitOffset / 8
					        || foundMapping) //allow offset gaps
					{
						foundMapping = true;
						if (fillSize < size) // add objects as longs as size fits
						{
							//domain contains bitstring
							if (cnPIObject->GetDataType() == IEC_Datatype::BITSTRING || cnPIObject->GetDataType() == IEC_Datatype::BOOL)
							{
								//Get bitstring size to calculate bit count
								bitCount += cnPIObject->GetSize();
								if (bitCount < 8) // continue because not full byte is filled
									continue;
							}

							//Get dynamic channel for datatype
							std::shared_ptr<DynamicChannel> dynChannel;
							res = mn->GetDynamicChannel(GetPlkDataType(cnPIObject->GetDataType()), dir, dynChannel);
							if (!res.IsSuccessful())
								return res;

							//availableSubindices = (std::uint16_t) dynChannel->GetMaxNumber() / (dynChannel->GetEndIndex() - dynChannel->GetStartIndex() + 1); //usually 254
							bitoffset = CalculateOffset(GetPlkDataType(cnPIObject->GetDataType()), dir);

							//Get the start index from dynamic channel
							index = dynChannel->GetStartIndex();
							if (bitoffset != 0)
							{
								std::uint32_t byteLength = 0;
								if (cnPIObject->GetSize() < 8)
									byteLength = 1;
								else
									byteLength = cnPIObject->GetSize() / 8;

								subindex = ((bitoffset / 8) / byteLength) + 1;

								if (subindex > availableSubindices) //Use next mapping object when channel objects are reached
								{
									subindex--;
									std::uint32_t div = subindex / availableSubindices;
									index = dynChannel->GetStartIndex() + div;
									subindex = (subindex % availableSubindices) + 1;
								}
							}

							//MN PI is full
							if (index > dynChannel->GetEndIndex())
							{
								boost::format formatter(kMsgDynamicChannelExceeded);
								formatter
								% index
								% subindex
								% dynChannel->GetStartIndex()
								% dynChannel->GetEndIndex()
								% GetPlkDataTypeName(dynChannel->GetDataType())
								% DirectionTypeValues[(std::uint8_t) dir];
								LOG_WARN() << formatter.str();
							}

							//Bitstring have a size below 1 Byte need special treatment
							std::uint32_t sizeToWrite = 0;
							if (cnPIObject->GetSize() < 8)
								sizeToWrite = 8; //Set size 8 because Bitstrings have been accumulated to one byte
							else
								sizeToWrite = cnPIObject->GetSize(); //All other objects have correct size

							std::uint32_t offsetToWrite = channelBitOffset;
							//Chained nodes output data mapped to PResMN
							std::uint8_t nodeId = cn->GetNodeId();
							if (mapping->GetDestinationNode() == 240 && dir == Direction::TX)
							{
								nodeId = 0;
								offsetToWrite = presMnOffset;
							}

							//Write mapping to MN object dictionary
							res = this->WriteMappingToForNode(nodeId, dir, this->GetMappingString(sizeToWrite, offsetToWrite, subindex, index), mn);
							if (!res.IsSuccessful())
								return res;

							//full byte is reached with bitstrings
							if (bitCount == 8)
							{
								bitoffset += 8; // Add one byte
								channelBitOffset += 8; // Add one byte
								fillSize += 8; // Add one byte
								bitCount = 0; //Reset bitcount

								if (mapping->GetDestinationNode() == 240 && dir == Direction::TX)
								{
									presMnOffset += 8;
								}
							}
							else //normal datatype
							{
								fillSize += cnPIObject->GetSize(); //Add datatype size to fillsize
								bitoffset += cnPIObject->GetSize(); //Add datatype size to offset
								channelBitOffset += cnPIObject->GetSize(); //Add datatype size to channel offset

								if (mapping->GetDestinationNode() == 240 && dir == Direction::TX)
								{
									presMnOffset += cnPIObject->GetSize();
								}
							}
						}
					}
				}
			}
		}
	}
	if (dir == Direction::TX
	        && cnReceivePresMN == false
	        && std::find(mn->GetNodeAssignment().begin(), mn->GetNodeAssignment().end(), NodeAssignment::NMT_NODEASSIGN_MN_PRES) != mn->GetNodeAssignment().end())
	{
		LOG_WARN() << kMsgManagingNodeTransmitPres;
	}
	return Result();
}

const std::string ManagingNodeMappingBuilder::GetMappingString(std::uint32_t bitSize, std::uint32_t bitOffset, std::uint32_t subindex, std::uint32_t index)
{
	std::stringstream mappingObjStr;
	mappingObjStr << "0x";
	//Write mapping length
	mappingObjStr << std::uppercase << std::hex << std::setw(4) << std::setfill('0') << std::right << bitSize;
	//Write mapping offset
	mappingObjStr << std::uppercase << std::hex << std::setw(4) << std::setfill('0') << std::right << bitOffset;
	//Write alignment
	mappingObjStr << std::uppercase << std::hex << std::setw(2) << std::setfill('0') << "";
	//Write mapping subindex
	mappingObjStr << std::uppercase << std::hex << std::setw(2) << std::setfill('0') << std::right << subindex;
	//Write mapping index
	mappingObjStr << std::uppercase << std::hex << std::setw(4) << std::setfill('0') << std::right << index;

	return mappingObjStr.str();
}

bool ManagingNodeMappingBuilder::GenerateForNode(const std::string& value, std::uint16_t nodeId)
{
	std::vector<std::string> nodeIds;
	boost::split(nodeIds, value, boost::is_any_of("; "));

	//Node ids can be hex or dec numbers
	for (auto& part : nodeIds)
	{
		if (HexToInt<std::uint16_t>(part) == nodeId)
			return true;
	}
	return false;
}

Result ManagingNodeMappingBuilder::FindMappedObject(const std::shared_ptr<BaseNode>& node, std::uint32_t dataIndex, std::uint16_t dataSubindex, std::shared_ptr<BaseObject>& foundObject)
{
	//Temporary object pointer
	std::shared_ptr<Object> dataObject;
	std::shared_ptr<SubObject> dataSubObject;

	//Check that mapped object exist
	if (dataSubindex == 0) //Can be either subindex 0 or object
	{
		//Check if subindex 0 exist
		Result res = node->GetSubObject(dataIndex, dataSubindex, dataSubObject, false);
		if (!res.IsSuccessful())
		{
			//Check if object exist
			res = node->GetObject(dataIndex, dataObject);
			if (!res.IsSuccessful())
			{
				boost::format formatter(kMsgNonExistingMappedObject);
				formatter
				% dataIndex
				% (std::uint32_t) node->GetNodeId();
				LOG_ERROR() << formatter.str();
				return Result(ErrorCode::MAPPED_OBJECT_DOES_NOT_EXIST, formatter.str());
			}
			else
				foundObject = dataObject; //Object found
		}
		else
			foundObject = dataSubObject; //SubObject found
	}
	else
	{
		//Normal subobject
		Result res = node->GetSubObject(dataIndex, dataSubindex, dataSubObject);
		if (!res.IsSuccessful())
		{
			boost::format formatter(kMsgNonExistingMappedSubObject);
			formatter
			% dataIndex
			% dataSubindex
			% (std::uint32_t) node->GetNodeId();
			LOG_ERROR() << formatter.str();
			return Result(ErrorCode::MAPPED_SUBOBJECT_DOES_NOT_EXIST, formatter.str());
		}
		foundObject = dataSubObject; //SubObject found
	}
	return Result();
}

Result ManagingNodeMappingBuilder::WriteMappingToForNode(std::uint16_t nodeId, Direction dir, const std::string& actualMappingValue, const std::shared_ptr<ManagingNode>& mn)
{
	std::uint32_t mappingObjectIndex = 0;
	std::uint32_t mappingBaseParameterIndex = 0;
	std::uint32_t mappingBaseObjectIndex = 0;
	std::uint16_t nrOfEntries = 1;
	bool useNewMappingParameter = false;

	//Set mapping parameter index according to direction
	if (dir == Direction::RX)
	{
		mappingBaseParameterIndex = 0x1400;
		mappingBaseObjectIndex = 0x1600;
	}

	else if (dir == Direction::TX)
	{
		mappingBaseParameterIndex = 0x1800;
		mappingBaseObjectIndex = 0x1A00;
	}

	//Get mapping object
	std::shared_ptr<SubObject> mappingParamObj;
	bool noChannelAvailable = true;
	for (auto& obj : mn->GetObjectDictionary())
	{
		if (obj.first >= mappingBaseParameterIndex && obj.first < (mappingBaseParameterIndex + 0x100)) //Traverse mapping parameter
		{
			//Get mapping parameter object
			std::shared_ptr<SubObject> nodeID;
			Result res = obj.second->GetSubObject(0x1, nodeID); //Find node Id parameter
			if (!res.IsSuccessful())
				return res;

			if (nodeID->HasActualValue()) //actual value exist
			{
				if (nodeId == nodeID->GetTypedActualValue<std::uint16_t>())
				{
					mappingObjectIndex = (obj.first - mappingBaseParameterIndex) + mappingBaseObjectIndex; //Calculate mapping object index for node

					//Get according mapping object - Check if mappings are available
					std::shared_ptr<Object> mappingObject;
					Result res = mn->GetObject(mappingObjectIndex, mappingObject);
					if (!res.IsSuccessful())
						return res;

					//Get mapping nrOfEntries
					std::shared_ptr<SubObject> nrOfEntriesObj;
					res = mappingObject->GetSubObject(0x0, nrOfEntriesObj);
					if (!res.IsSuccessful())
						return res;

					if (nrOfEntriesObj->HasActualValue())
					{
						if (nrOfEntriesObj->GetTypedActualValue<uint16_t>() == mappingObject->GetSubObjectDictionary().size() - 1) //Mapping object already full
						{
							if (nodeId == 0) //Overflow only for PResMN
								continue;
							else
								return Result(ErrorCode::INSUFFICIENT_MAPPING_OBJECTS);
						}
					}
					useNewMappingParameter = false; // Mapping parameter already exist for node
					noChannelAvailable = false;
					break; //End traversal
				}
				else
					continue;
			}
			else
			{
				if (useNewMappingParameter == false) // Take first unsused parameter and store it
				{
					mappingParamObj = nodeID;
					mappingObjectIndex = (obj.first - mappingBaseParameterIndex) + mappingBaseObjectIndex;
					useNewMappingParameter = true; // new mapping parameter stored
					noChannelAvailable = false;
				}
				continue;
			}
		}
	}
	//Check for no available channels
	if (noChannelAvailable)
		return Result(ErrorCode::CHANNEL_OBJECT_LIMIT_EXCEEDED);

	//Create new mapping parameter for node
	if (useNewMappingParameter)
	{
		Result res = mappingParamObj->SetTypedObjectActualValue(IntToHex(nodeId, 2, "0x"));
		if (!res.IsSuccessful())
			return res;
	}

	//Get according mapping object
	std::shared_ptr<Object> mappingObject;
	Result res = mn->GetObject(mappingObjectIndex, mappingObject);
	if (!res.IsSuccessful())
		return res;

	//Get mapping nrOfEntries
	std::shared_ptr<SubObject> nrOfEntriesObj;
	res = mappingObject->GetSubObject(0x0, nrOfEntriesObj);
	if (!res.IsSuccessful())
		return res;

	//Count mapping and set mapping object actual values
	for (auto& mapping : mappingObject->GetSubObjectDictionary())
	{
		if (mapping.first == 0x0)//Skip Nr Of Entries
			continue;

		//Mapping object exists increment nr of entries
		if (mapping.second->WriteToConfiguration())
		{
			nrOfEntries++;
			continue;
		}
		else
		{
			//Set mapping object value
			res = mapping.second->SetTypedObjectActualValue(actualMappingValue);
			if (!res.IsSuccessful())
				return res;
			break;
		}
	}
	//Update nr of entries
	return nrOfEntriesObj->SetTypedObjectActualValue(IntToHex(nrOfEntries, 2, "0x"));
}

std::uint32_t ManagingNodeMappingBuilder::CalculateOffset(PlkDataType dataType, Direction dir)
{
	std::uint32_t dataTypeSize = Utilities::GetIECDataTypeBitSize(Utilities::GetIECDataType(dataType));
	std::uint32_t returnOffset = 0;
	std::shared_ptr<std::map<PlkDataType, std::uint32_t>> offsets;

	if (dir == Direction::TX)
		offsets = this->inputOffsets;
	else if (dir == Direction::RX)
		offsets = this->outputOffsets;

	returnOffset = offsets->at(dataType);
	offsets->at(dataType) = returnOffset + dataTypeSize;

	for (auto it = offsets->begin(); it != offsets->end(); ++it)
	{
		if (it->first == dataType) //same dataType continue
			continue;

		if (it->second < offsets->at(dataType))
		{
			if (dataTypeSize > Utilities::GetIECDataTypeBitSize(Utilities::GetIECDataType(it->first)))
				it->second = offsets->at(dataType);
			else
				it->second = it->second + Utilities::GetIECDataTypeBitSize(Utilities::GetIECDataType(it->first));
		}
	}
	return returnOffset;
}