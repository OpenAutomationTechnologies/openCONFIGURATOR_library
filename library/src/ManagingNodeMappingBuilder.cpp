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

ManagingNodeMappingBuilder::ManagingNodeMappingBuilder() : BuildConfigurationSettingBuilder()
{}

ManagingNodeMappingBuilder::~ManagingNodeMappingBuilder(void)
{}

Result ManagingNodeMappingBuilder::GenerateConfiguration(const std::string& value, const std::map<std::uint8_t, std::shared_ptr<BaseNode>>& nodeCollection)
{
	//Get Managing Node (this must not fail)
	const std::shared_ptr<ManagingNode>& mn = std::dynamic_pointer_cast<ManagingNode>(nodeCollection.at(240));
	mn->ClearMappingObjects(); //clear MN mapping

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

	//Direction from node point of view
	Direction nodeDir = Direction::TX;
	if (dir == Direction::TX)
		nodeDir = Direction::RX;

	std::uint32_t bitoffset = 0;
	std::uint32_t channelBitOffset = 0;
	std::uint32_t presMnOffset = 0;
	std::uint32_t index = 0;
	std::uint32_t subindex = 1;

	//Traverse all controlled nodes
	for (auto& node : nodeCollection)
	{
		//Reset channel bit offset
		channelBitOffset = 0;

		//CN receive default from 0 for MN
		std::uint16_t receiveFromNode = 0;
		if (!value.empty())
		{
			if (!this->GenerateForNode(value, node.first)) //Check if values have to be generated
				continue;
		}

		//Cast pointer to CN
		std::shared_ptr<ControlledNode> cn = std::dynamic_pointer_cast<ControlledNode>(node.second); //Check if not RMN needs to be handled properly
		if (!cn)
			continue; //If RMN continue

		//TODO: Add RMN Handling

		//Node is PRes Chained set receive from not to 240 (PResMN)
		if (cn->GetOperationMode() == PlkOperationMode::CHAINED
		        && dir == Direction::TX)
		{
			receiveFromNode = 240;
			//Set correct offset for CN receive PDO form MN
			cn->SetNodeDataPresMnOffset(presMnOffset);
		}

		//Update CN Mapping
		cn->UpdateProcessImage(nodeDir);

		//Get MN PDO Channel Objects for direction
		std::uint16_t pdoChannelObjects = 1;
		Result res;
		std::vector<std::shared_ptr<BaseProcessDataMapping>> mappingCollection;
		if (dir == Direction::RX)
		{
			res = mn->GetNetworkManagement()->GetFeatureActualValue<std::uint16_t>(GeneralFeatureEnum::PDORPDOChannelObjects, pdoChannelObjects);
			//Traverse Tx Mapping from Node to create Rx Mapping on MN
			mappingCollection = cn->GetTransmitMapping();
		}
		else if (dir == Direction::TX)
		{
			res = mn->GetNetworkManagement()->GetFeatureActualValue<std::uint16_t>(GeneralFeatureEnum::PDOTPDOChannelObjects, pdoChannelObjects);
			//Traverse Rx Mapping from Node to create Tx Mapping on MN
			mappingCollection = cn->GetReceiveMapping();
		}
		if (!res.IsSuccessful()) //Use default value
			pdoChannelObjects = 254;

		for (auto& mapping : mappingCollection)
		{
			if (dir == Direction::TX)
			{
				if (mapping->GetDestinationNode() != receiveFromNode) //Cross traffic mapping not from MN
					continue;
			}

			//Retrieve mapped object
			std::shared_ptr<BaseObject> foundObject;
			res = this->FindMappedObject(cn, mapping->GetMappingIndex(), mapping->GetMappingSubIndex(), foundObject);
			if (!res.IsSuccessful())
				return res;

			if (!foundObject->GetUniqueIdRef().is_initialized()
			        && foundObject->GetDataType().is_initialized()) //Mapping object has normal data type
			{
				//Get Dynamic Channel from MN for datatype amd direction
				std::shared_ptr<DynamicChannel> dynChannel;
				res = mn->GetDynamicChannel(foundObject->GetDataType().get(), dir, dynChannel);
				if (!res.IsSuccessful())
					continue; //return res;

				//Get first index of dynamic channel
				index = dynChannel->GetStartIndex();
				if (bitoffset != 0) // not first run bitoffset is not zero
				{
					std::uint32_t byteLength = mapping->GetMappingLength() / 8; //Get Byte length of data mapped
					subindex = ((bitoffset / 8) / byteLength) + 1; //Get subindex for mapped data

					// if subindex exceeds nr of TPDO channel objects use next mapping object
					if (subindex > pdoChannelObjects)
					{
						subindex--; //Decrement subindex
						std::uint32_t div = subindex / pdoChannelObjects;
						index = dynChannel->GetStartIndex() + div; //Calculate new index
						subindex = (subindex % pdoChannelObjects) + 1; //Calczulate new subindex
					}
				}

				std::uint32_t offsetToWrite = channelBitOffset;
				//Chained nodes output data mapped to PResMN
				std::uint8_t nodeId = cn->GetNodeId();
				if (cn->GetOperationMode() == PlkOperationMode::CHAINED
				        && dir == Direction::TX)
				{
					nodeId = 240;
					offsetToWrite = presMnOffset;
				}

				// Write mapping to MN object dictionary
				res = this->WriteMappingToForNode(nodeId, dir, this->GetMappingString(mapping->GetMappingLength(), offsetToWrite, subindex, index), mn);
				if (!res.IsSuccessful())
					return res;

				// Increment bitoffset with the length of the added mapping
				bitoffset += mapping->GetMappingLength();
				channelBitOffset += mapping->GetMappingLength();

				if (cn->GetOperationMode() == PlkOperationMode::CHAINED
				        && dir == Direction::TX)
				{
					presMnOffset += mapping->GetMappingLength();
				}
			}
			else if (foundObject->GetUniqueIdRef().is_initialized()
			         && foundObject->GetDataType().get() == PlkDataType::Domain) // CN mapps a domain object
			{
				bool foundMapping = false;
				std::uint32_t bitCount = 0;
				std::uint32_t fillSize = 0;
				std::uint32_t size = mapping->GetMappingLength();

				//Get domain object parameter
				std::shared_ptr<Parameter> param;
				res = cn->GetApplicationProcess()->GetParameter(foundObject->GetUniqueIdRef().get(), param);
				if (!res.IsSuccessful())
					return res;

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
					if (cnPIObject->GetPiOffset() >= mapping->GetMappingOffset() / 8
					        || foundMapping) //allow offset gaps
					{
						foundMapping = true;
						if (fillSize < size) // add objects as longs as size fits
						{
							//domain contains bitstring
							if (cnPIObject->GetDataType() == IEC_Datatype::BITSTRING)
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

								if (subindex > pdoChannelObjects) //Use next mapping object when channel objects are reached
								{
									subindex--;
									std::uint32_t div = subindex / pdoChannelObjects;
									index = dynChannel->GetStartIndex() + div;
									subindex = (subindex % pdoChannelObjects) + 1;
								}
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
							if (cn->GetOperationMode() == PlkOperationMode::CHAINED
							        && dir == Direction::TX)
							{
								nodeId = 240;
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

								if (cn->GetOperationMode() == PlkOperationMode::CHAINED
								        && dir == Direction::TX)
								{
									presMnOffset += 8;
								}
							}
							else //normal datatype
							{
								fillSize += cnPIObject->GetSize(); //Add datatype size to fillsize
								bitoffset += cnPIObject->GetSize(); //Add datatype size to offset
								channelBitOffset += cnPIObject->GetSize(); //Add datatype size to channel offset

								if (cn->GetOperationMode() == PlkOperationMode::CHAINED
								        && dir == Direction::TX)
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
	boost::split(nodeIds, value, boost::is_any_of(";"));

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
		Result res = node->GetSubObject(dataIndex, dataSubindex, dataSubObject);
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
				LOG_FATAL() << formatter.str();
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
			LOG_FATAL() << formatter.str();
			return Result(ErrorCode::MAPPED_SUBOBJECT_DOES_NOT_EXIST, formatter.str());
		}
		foundObject = dataSubObject; //SubObject found
	}
	return Result();
}

Result ManagingNodeMappingBuilder::WriteMappingToForNode(std::uint16_t nodeId, Direction dir, const std::string& actualMappingValue, const std::shared_ptr<ManagingNode>& mn)
{
	std::uint32_t mappingParameterIndex = 0;
	std::uint32_t mappingObjectIndex = 0;
	std::uint16_t nrOfEntries = 1;
	bool useNewMappingParameter = false;

	//Set mapping parameter index according to direction
	if (dir == Direction::RX)
	{
		mappingParameterIndex = 0x1400;
		mappingObjectIndex = 0x1600;
	}

	else if (dir == Direction::TX)
	{
		mappingParameterIndex = 0x1800;
		mappingObjectIndex = 0x1A00;
	}

	//Get mapping object
	std::shared_ptr<SubObject> mappingParamObj;
	for (auto& obj : mn->GetObjectDictionary())
	{
		if (obj.first >= mappingParameterIndex && obj.first < (mappingParameterIndex + 0x100)) //Traverse mapping parameter
		{
			//Get mapping parameter object
			std::shared_ptr<SubObject> nodeID;
			Result res = obj.second->GetSubObject(0x1, nodeID); //Find node Id parameter
			if (!res.IsSuccessful())
				return res;

			if (nodeID->WriteToConfiguration()) //actual value exist
			{
				if (nodeId == nodeID->GetTypedActualValue<std::uint16_t>())
				{
					mappingObjectIndex = (obj.first - mappingParameterIndex) + mappingObjectIndex; //Calculate mapping object index for node
					useNewMappingParameter = false; // Mapping paramter already exist for node
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
					mappingObjectIndex = (obj.first - mappingParameterIndex) + mappingObjectIndex;
					useNewMappingParameter = true; // new mapping parameter stored
				}
				continue;
			}
		}
	}
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