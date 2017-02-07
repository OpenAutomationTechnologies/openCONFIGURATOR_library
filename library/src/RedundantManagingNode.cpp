/************************************************************************
\file RedundantManagingNode.cpp
\brief Implementation of the Class RedundantManagingNode
\author rueckerc, Bernecker+Rainer Industrie Elektronik Ges.m.b.H.
\date 01-May-2017 12:00:00
************************************************************************/

/*------------------------------------------------------------------------------
Copyright (c) 2017, Bernecker+Rainer Industrie-Elektronik Ges.m.b.H. (B&R)
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
#include "RedundantManagingNode.h"

using namespace IndustrialNetwork::POWERLINK::Core::Node;
using namespace IndustrialNetwork::POWERLINK::Core::ErrorHandling;
using namespace IndustrialNetwork::POWERLINK::Core::ObjectDictionary;
using namespace IndustrialNetwork::POWERLINK::Core::CoreConfiguration;
using namespace IndustrialNetwork::POWERLINK::Core::Utilities;

RedundantManagingNode::RedundantManagingNode(std::uint8_t nodeID, const std::string& nodeName) : ControlledNode(nodeID, nodeName),
	rmnList(std::vector<std::uint16_t>())
{}

RedundantManagingNode::~RedundantManagingNode()
{}

std::uint16_t RedundantManagingNode::GetRmnCount() const
{
	return (std::uint16_t) this->rmnList.size();
}

void RedundantManagingNode::AddRmnId(std::uint16_t id)
{
	this->rmnList.push_back(id);
	//Sort and remove duplicates
	std::sort(rmnList.begin(), rmnList.end());
	rmnList.erase(std::unique(rmnList.begin(), rmnList.end()), rmnList.end());
}

void RedundantManagingNode::ClearRmnList()
{
	this->rmnList.clear();
}

void RedundantManagingNode::RemoveRmnId(std::uint16_t _nodeId)
{
	this->rmnList.erase(std::remove(this->rmnList.begin(), this->rmnList.end(), _nodeId), this->rmnList.end());
}

const std::vector<std::uint16_t>& RedundantManagingNode::GetRmnList()
{
	return this->rmnList;
}

Result RedundantManagingNode::ClearMappingObjects()
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
			bool nrOfEntriesCleared = false;
			for (auto& subobject : object.second->GetSubObjectDictionary())
			{
				if (subobject.first != 0 && subobject.second->HasActualValue())
				{
					if (subobject.second->GetTypedActualValue<std::uint64_t>() != 0)
					{
						std::shared_ptr<BaseProcessDataMapping> mappingPtr = std::shared_ptr<BaseProcessDataMapping>(new BaseProcessDataMapping(subobject.second->GetTypedActualValue<std::string>(), 0));
						if (mappingPtr->GetMappingIndex() >= 0xA000) //Clear only dynamic channel mappings
						{
							if (nrOfEntriesCleared == false)
							{
								std::shared_ptr<SubObject> nrOfEntries;
								Result res = this->GetSubObject(object.first, 0x0, nrOfEntries);
								if (!res.IsSuccessful())
									return res;
								nrOfEntries->ClearActualValue();
								nrOfEntriesCleared = true;
							}
							subobject.second->ClearActualValue(); //Clear Mapping Objects
						}
					}
				}
			}
		}
	}
	return Result();
}

std::uint32_t RedundantManagingNode::GetRMNConfigurationObjectCount()
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

	boost::format formatter(kMsgNodeObjectCount[static_cast<std::underlying_type<Language>::type>(LoggingConfiguration::GetInstance().GetCurrentLanguage())]);
	formatter
	% (std::uint32_t) this->GetNodeId()
	% count;
	LOG_INFO() << formatter.str();
	return count;
}

std::uint32_t RedundantManagingNode::GetRMNConfigurationObjectSize()
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

	boost::format formatter(kMsgNodeObjectCountSize[static_cast<std::underlying_type<Language>::type>(LoggingConfiguration::GetInstance().GetCurrentLanguage())]);
	formatter
	% (std::uint32_t) this->GetNodeId()
	% size;
	LOG_INFO() << formatter.str();
	return size;
}

Result RedundantManagingNode::SyncMNObjects(const std::shared_ptr<ManagingNode>& mn)
{
	Result res;
	for (auto& obj : mn->GetObjectDictionary())
	{
		if ((obj.first >= 0x1800 && obj.first < 0x1900)
		        || (obj.first >= 0x1400 && obj.first < 0x1500))
		{
			std::shared_ptr<SubObject> nodeIdSubObj;
			res = obj.second->GetSubObject(0x1, nodeIdSubObj);
			if (!res.IsSuccessful())
				return res;
			if (nodeIdSubObj->HasActualValue())
			{
				if (nodeIdSubObj->GetTypedActualValue<std::uint16_t>() != 0 && nodeIdSubObj->GetTypedActualValue<std::uint16_t>() != (std::uint16_t) this->GetNodeId())
				{
					bool channelIsFree = false;
					res = IsChannelFree(obj.first, channelIsFree);
					if (!res.IsSuccessful())
						return res;

					if (channelIsFree)
					{
						res = this->SetSubObjectActualValue(obj.first, 0x1, "0x" + nodeIdSubObj->GetTypedActualValue<std::string>());
						if (!res.IsSuccessful())
							return res;

						std::uint32_t channelIndex = 0;
						if (obj.first >= 0x1400 && obj.first < 0x1500)
							channelIndex = obj.first - 0x1400 + 0x1600;
						else if (obj.first >= 0x1800 && obj.first < 0x1900)
							channelIndex = obj.first - 0x1800 + 0x1A00;

						std::shared_ptr<Object> mnObject;
						res = mn->GetObject(channelIndex, mnObject);
						if (!res.IsSuccessful())
							return res;

						for (auto& mapping : mnObject->GetSubObjectDictionary())
						{
							if (mapping.second->HasActualValue())
							{
								res = this->SetSubObjectActualValue(channelIndex, mapping.first, "0x" + mapping.second->GetTypedActualValue<std::string>());
								if (!res.IsSuccessful())
									return res;
							}
						}
					}
					else
					{
						std::uint32_t commParamIndex = obj.first;

						while (channelIsFree == false)
						{
							commParamIndex++;
							res = IsChannelFree(commParamIndex, channelIsFree);
							if (!res.IsSuccessful())
								return res;
						}

						res = this->SetSubObjectActualValue(commParamIndex, 0x1, "0x" + nodeIdSubObj->GetTypedActualValue<std::string>());
						if (!res.IsSuccessful())
							return res;

						std::uint32_t channelIndex = 0;
						if (commParamIndex >= 0x1400 && commParamIndex < 0x1500)
							channelIndex = commParamIndex - 0x1400 + 0x1600;
						else if (commParamIndex >= 0x1800 && commParamIndex < 0x1900)
							channelIndex = commParamIndex - 0x1800 + 0x1A00;

						std::uint32_t mnchannelIndex = 0;
						if (obj.first >= 0x1400 && obj.first < 0x1500)
							mnchannelIndex = obj.first - 0x1400 + 0x1600;
						else if (obj.first >= 0x1800 && obj.first < 0x1900)
							mnchannelIndex = obj.first - 0x1800 + 0x1A00;

						std::shared_ptr<Object> mnObject;
						res = mn->GetObject(mnchannelIndex, mnObject);
						if (!res.IsSuccessful())
							return res;

						for (auto& mapping : mnObject->GetSubObjectDictionary())
						{
							if (mapping.second->HasActualValue())
							{
								res = this->SetSubObjectActualValue(channelIndex, mapping.first, "0x" + mapping.second->GetTypedActualValue<std::string>());
								if (!res.IsSuccessful())
									return res;
							}
						}
					}
				}
			}
			continue;
		}
		else if ((obj.first >= 0x1600 && obj.first < 0x1700)
		         || (obj.first >= 0x1A00 && obj.first < 0x1B00))
		{
			continue;
		}
		else
		{
			if (obj.second->HasActualValue())
			{
				res = this->SetObjectActualValue(obj.first, "0x" + obj.second->GetTypedActualValue<std::string>());
				if (!res.IsSuccessful())
					return res;
			}
			for (auto& subObj : obj.second->GetSubObjectDictionary())
			{
				if (obj.first == 0x1F81 && subObj.first == 240)
					continue;
				if (subObj.second->HasActualValue())
				{
					res = this->SetSubObjectActualValue(obj.first, subObj.first, "0x" + subObj.second->GetTypedActualValue<std::string>());
					if (!res.IsSuccessful())
						return res;
				}
			}
		}
	}
	return Result();
}

Result RedundantManagingNode::IsChannelFree(std::uint32_t commParamIndex, bool& isFree)
{
	isFree = false;
	std::shared_ptr<SubObject> commParam;
	Result res = this->GetSubObject(commParamIndex, 0x1, commParam);
	if (!res.IsSuccessful())
		return res;
	if (commParam->HasActualValue())
		return res;

	std::uint32_t channelIndex = 0;
	if (commParamIndex >= 0x1400 && commParamIndex < 0x1500)
		channelIndex = commParamIndex - 0x1400 + 0x1600;
	else if (commParamIndex >= 0x1800 && commParamIndex < 0x1900)
		channelIndex = commParamIndex - 0x1800 + 0x1A00;

	std::shared_ptr<SubObject> NrOfEntries;
	res = this->GetSubObject(channelIndex, 0x1, NrOfEntries);
	if (!res.IsSuccessful())
		return res;
	if (NrOfEntries->HasActualValue())
		return res;

	isFree = true;
	return Result();

}
