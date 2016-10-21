/************************************************************************
\file BaseNode.cpp
\brief Implementation of the Class BaseNode
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
#include "BaseNode.h"

using namespace IndustrialNetwork::POWERLINK::Core::Node;
using namespace IndustrialNetwork::POWERLINK::Core::ObjectDictionary;
using namespace IndustrialNetwork::POWERLINK::Core::ErrorHandling;
using namespace IndustrialNetwork::POWERLINK::Core::CoreConfiguration;
using namespace IndustrialNetwork::POWERLINK::Core::Utilities;


BaseNode::BaseNode(std::uint8_t nodeId, const std::string& name) :
	nodeId(nodeId),
	name(name),
	enabled(true),
	objectDictionary(std::map<std::uint32_t, std::shared_ptr<Object>>()),
	applicationProcess(std::make_shared<ApplicationProcess>()),
	nodeAssignment(std::vector<NodeAssignment>()),
	networkManagement(std::make_shared<NetworkManagement>()),
	dynamicChannelList(std::vector<std::shared_ptr<DynamicChannel>>()),
	transmitMapping(std::vector<std::shared_ptr<BaseProcessDataMapping>>()),
	receiveMapping(std::vector<std::shared_ptr<BaseProcessDataMapping>>()),
	transmitProcessImage(std::vector<std::shared_ptr<BaseProcessImageObject>>()),
	receiveProcessImage(std::vector<std::shared_ptr<BaseProcessImageObject>>()),
	ignoreNonExistingMappingObjects(false),
	ignoreInvalidMappingOffsets(false)
{}

BaseNode::~BaseNode()
{
	this->objectDictionary.clear();
	this->nodeAssignment.clear();
	this->dynamicChannelList.clear();
	this->transmitMapping.clear();
	this->receiveMapping.clear();
}

const std::string& BaseNode::GetName() const
{
	return this->name;
}

void BaseNode::SetName(const std::string& _name)
{
	this->name = _name;
}

std::uint8_t BaseNode::GetNodeId() const
{
	return this->nodeId;
}

void BaseNode::SetNodeId(std::uint8_t _nodeId)
{
	this->nodeId = _nodeId;
}

std::map<std::uint32_t, std::shared_ptr<Object>>& BaseNode::GetObjectDictionary()
{
	return objectDictionary;
}

std::vector<NodeAssignment>& BaseNode::GetNodeAssignment()
{
	return this->nodeAssignment;
}

std::shared_ptr<NetworkManagement>& BaseNode::GetNetworkManagement()
{
	return this->networkManagement;
}

std::shared_ptr<ApplicationProcess>& BaseNode::GetApplicationProcess()
{
	return this->applicationProcess;
}

std::vector<std::shared_ptr<BaseProcessDataMapping>>& BaseNode::GetTransmitMapping()
{
	return this->transmitMapping;
}

std::vector<std::shared_ptr<BaseProcessDataMapping>>& BaseNode::GetReceiveMapping()
{
	return this->receiveMapping;
}

Result BaseNode::AddObject(std::shared_ptr<Object>& objRef)
{
	if (this->objectDictionary.find(objRef->GetObjectId()) != this->objectDictionary.end())
	{
		//Object already exists
		boost::format formatter(kMsgExistingObject[static_cast<std::underlying_type<Language>::type>(LoggingConfiguration::GetInstance().GetCurrentLanguage())]);
		formatter
		% objRef->GetObjectId()
		% (std::uint32_t) nodeId;
		return Result(ErrorCode::OBJECT_EXISTS, formatter.str());
	}

	this->objectDictionary.insert(std::pair<std::uint32_t, std::shared_ptr<Object>>(objRef->GetObjectId(), objRef));

	//Log object creation
	boost::format formatter(kMsgObjectCreated[static_cast<std::underlying_type<Language>::type>(LoggingConfiguration::GetInstance().GetCurrentLanguage())]);
	formatter
	% objRef->GetObjectId()
	% (std::uint32_t) nodeId;
	LOG_INFO() << formatter.str();
	return Result();
}

Result BaseNode::AddSubObject(std::uint32_t objectId, std::shared_ptr<SubObject>& objRef)
{
	if (this->objectDictionary.find(objectId) == this->objectDictionary.end())
	{
		//Object does not exist
		boost::format formatter(kMsgNonExistingObject[static_cast<std::underlying_type<Language>::type>(LoggingConfiguration::GetInstance().GetCurrentLanguage())]);
		formatter
		% objectId
		% (std::uint32_t) nodeId;
		LOG_ERROR() << formatter.str();
		return Result(ErrorCode::OBJECT_DOES_NOT_EXIST, formatter.str());
	}
	return this->objectDictionary.find(objectId)->second->AddSubobject(objRef);
}

Result BaseNode::ForceObject(std::uint32_t objectId, bool force, bool validateOnly, const std::string& actualValue, bool enableLog)
{
	auto iter = this->objectDictionary.find(objectId);
	if (iter == this->objectDictionary.end())
	{
		//Object does not exist
		boost::format formatter(kMsgNonExistingObject[static_cast<std::underlying_type<Language>::type>(LoggingConfiguration::GetInstance().GetCurrentLanguage())]);
		formatter
		% objectId
		% (std::uint32_t) nodeId;
		if (enableLog)
		{
			LOG_ERROR() << formatter.str();
		}
		return Result(ErrorCode::OBJECT_DOES_NOT_EXIST, formatter.str());
	}

	if (iter->second->GetObjectType() != ObjectType::VAR)
	{
		boost::format formatter(kMsgBaseObjectValueSupport[static_cast<std::underlying_type<Language>::type>(LoggingConfiguration::GetInstance().GetCurrentLanguage())]);
		formatter
		% objectId
		% (std::uint32_t) nodeId;
		LOG_ERROR() << formatter.str();
		return Result(ErrorCode::OBJECT_TYPE_DOES_NOT_SUPPORT_VALUES, formatter.str());
	}

	if (iter->second->GetAccessType() == AccessType::COND
	        || iter->second->GetAccessType() == AccessType::CONST
	        || iter->second->GetAccessType() == AccessType::RO)
	{
		boost::format formatter(kMsgBaseObjectValueSupport[static_cast<std::underlying_type<Language>::type>(LoggingConfiguration::GetInstance().GetCurrentLanguage())]);
		formatter
		% objectId
		% (std::uint32_t) nodeId;
		LOG_ERROR() << formatter.str();
		return Result(ErrorCode::OBJECT_ACCESS_DOES_NOT_SUPPORT_VALUES, formatter.str());
	}

	if (validateOnly == false)
	{
		iter->second->SetForceToCDC(force);
		//Log info forced object
		boost::format log_format(kMsgForceObject[static_cast<std::underlying_type<Language>::type>(LoggingConfiguration::GetInstance().GetCurrentLanguage())]);
		log_format
		% objectId
		% (uint32_t) nodeId
		% force;
		LOG_INFO() << log_format.str();
	}

	if (!actualValue.empty())
	{
		Result res = iter->second->SetTypedObjectActualValue(actualValue, validateOnly);
		if (!res.IsSuccessful())
			return res;

		//Log info actual value set
		boost::format formatter(kMsgSetObjectActualValue[static_cast<std::underlying_type<Language>::type>(LoggingConfiguration::GetInstance().GetCurrentLanguage())]);
		formatter
		% actualValue
		% objectId
		% (uint32_t) nodeId;
		LOG_INFO() << formatter.str();
	}
	else
	{
		if (validateOnly == false)
			iter->second->ClearActualValue(); //Clear the actual value
	}
	return Result();
}

Result BaseNode::SetObjectActualValue(std::uint32_t objectId, const std::string& actualValue)
{
	auto iter = this->objectDictionary.find(objectId);
	if (iter == this->objectDictionary.end())
	{
		//Object does not exist
		boost::format formatter(kMsgNonExistingObject[static_cast<std::underlying_type<Language>::type>(LoggingConfiguration::GetInstance().GetCurrentLanguage())]);
		formatter
		% objectId
		% (std::uint32_t) nodeId;
		LOG_FATAL() << formatter.str();
		return Result(ErrorCode::OBJECT_DOES_NOT_EXIST, formatter.str());
	}
	if (iter->second->GetForceToCDC())
	{
		//Actual value must not be set because forced
		boost::format formatter(kMsgForcedValueOverwriteObject[static_cast<std::underlying_type<Language>::type>(LoggingConfiguration::GetInstance().GetCurrentLanguage())]);
		formatter
		% actualValue
		% iter->first
		% (uint32_t) this->GetNodeId();
		LOG_WARN() << formatter.str();
		return Result();
	}

	if (!actualValue.empty())
	{
		Result res = iter->second->SetTypedObjectActualValue(actualValue);
		if (!res.IsSuccessful())
			return res;

		//Log info actual value set
		boost::format formatter(kMsgSetObjectActualValue[static_cast<std::underlying_type<Language>::type>(LoggingConfiguration::GetInstance().GetCurrentLanguage())]);
		formatter
		% actualValue
		% objectId
		% (std::uint32_t) nodeId;
		LOG_INFO() << formatter.str();
		return Result();
	}
	else
	{
		iter->second->ClearActualValue(); //Clear the actual value
	}
	return Result();
}

Result BaseNode::GetObject(std::uint32_t objectId, std::shared_ptr<Object>& objRef, bool enableLog)
{
	auto iter = this->objectDictionary.find(objectId);
	if (iter == this->objectDictionary.end())
	{
		//Object does not exist
		boost::format formatter(kMsgNonExistingObject[static_cast<std::underlying_type<Language>::type>(LoggingConfiguration::GetInstance().GetCurrentLanguage())]);
		formatter
		% objectId
		% (uint32_t) nodeId;
		if (enableLog)
		{
			LOG_FATAL() << formatter.str();
		}
		return Result(ErrorCode::OBJECT_DOES_NOT_EXIST, formatter.str());
	}
	objRef = iter->second;
	return Result();
}

Result BaseNode::ForceSubObject(std::uint32_t objectId, std::uint32_t subObjectId, bool force, bool validateOnly, const std::string& actualValue, bool enableLog)
{
	auto iter = this->objectDictionary.find(objectId);
	if (iter == this->objectDictionary.end())
	{
		//Object does not exist
		boost::format formatter(kMsgNonExistingObject[static_cast<std::underlying_type<Language>::type>(LoggingConfiguration::GetInstance().GetCurrentLanguage())]);
		formatter
		% objectId
		% (std::uint32_t) nodeId;
		if (enableLog)
		{
			LOG_ERROR() << formatter.str();
		}
		return Result(ErrorCode::OBJECT_DOES_NOT_EXIST, formatter.str());
	}

	std::shared_ptr<SubObject> subObject;
	Result res = iter->second->GetSubObject(subObjectId, subObject, enableLog);
	if (res.IsSuccessful())
	{
		if (subObject->GetObjectType() != ObjectType::VAR)
		{
			boost::format formatter(kMsgBaseSubObjectValueSupport[static_cast<std::underlying_type<Language>::type>(LoggingConfiguration::GetInstance().GetCurrentLanguage())]);
			formatter
			% objectId
			% subObject->GetObjectId()
			% (std::uint32_t) nodeId;
			LOG_ERROR() << formatter.str();
			return Result(ErrorCode::OBJECT_TYPE_DOES_NOT_SUPPORT_VALUES, formatter.str());
		}

		if (subObject->GetAccessType() == AccessType::COND
		        || subObject->GetAccessType() == AccessType::CONST
		        || subObject->GetAccessType() == AccessType::RO)
		{
			boost::format formatter(kMsgBaseSubObjectValueSupport[static_cast<std::underlying_type<Language>::type>(LoggingConfiguration::GetInstance().GetCurrentLanguage())]);
			formatter
			% objectId
			% subObject->GetObjectId()
			% (std::uint32_t) nodeId;
			LOG_ERROR() << formatter.str();
			return Result(ErrorCode::OBJECT_ACCESS_DOES_NOT_SUPPORT_VALUES, formatter.str());
		}

		if (validateOnly == false)
		{
			subObject->SetForceToCDC(force);
			//Log info forced subobject
			boost::format log_format(kMsgForceSubObject[static_cast<std::underlying_type<Language>::type>(LoggingConfiguration::GetInstance().GetCurrentLanguage())]);
			log_format
			% objectId
			% subObjectId
			% (std::uint32_t) nodeId
			% force;
			LOG_INFO() << log_format.str();
		}

		if (!actualValue.empty())
		{
			res = subObject->SetTypedObjectActualValue(actualValue, validateOnly);
			if (!res.IsSuccessful())
				return res;

			//Log info actual value set
			boost::format formatter(kMsgSetSubObjectActualValue[static_cast<std::underlying_type<Language>::type>(LoggingConfiguration::GetInstance().GetCurrentLanguage())]);
			formatter
			% actualValue
			% objectId
			% subObjectId
			% (std::uint32_t) nodeId;
			LOG_INFO() << formatter.str();
		}
		else
		{
			if (validateOnly == false)
				subObject->ClearActualValue(); //Clear the actual value
		}
	}
	return res;
}

Result BaseNode::SetSubObjectActualValue(std::uint32_t objectId, std::uint32_t subObjectId, const std::string& actualValue)
{
	auto iter = this->objectDictionary.find(objectId);
	if (iter == this->objectDictionary.end())
	{
		//Object does not exist
		boost::format formatter(kMsgNonExistingObject[static_cast<std::underlying_type<Language>::type>(LoggingConfiguration::GetInstance().GetCurrentLanguage())]);
		formatter
		% objectId
		% (std::uint32_t) nodeId;
		LOG_ERROR() << formatter.str();
		return Result(ErrorCode::OBJECT_DOES_NOT_EXIST, formatter.str());
	}

	std::shared_ptr<SubObject> subObject;
	Result res = iter->second->GetSubObject(subObjectId, subObject);
	if (res.IsSuccessful())
	{
		if (subObject->GetForceToCDC())
		{
			//Actual value must not be set because forced
			boost::format formatter(kMsgForcedValueOverwriteSubObject[static_cast<std::underlying_type<Language>::type>(LoggingConfiguration::GetInstance().GetCurrentLanguage())]);
			formatter
			% actualValue
			% objectId
			% subObject->GetObjectId()
			% (std::uint32_t) this->GetNodeId();
			LOG_WARN() << formatter.str();
			return Result();
		}
		if (!actualValue.empty())
		{
			res = subObject->SetTypedObjectActualValue(actualValue);
			if (res.GetErrorType() == ErrorCode::OBJECT_ACTUAL_VALUE_EXCEEDS_HIGHLIMIT)
			{
				boost::format formatter(kMsgBaseObjectHighLimitSubObjectError[static_cast<std::underlying_type<Language>::type>(LoggingConfiguration::GetInstance().GetCurrentLanguage())]);
				formatter
				% subObject->GetName()
				% objectId
				% subObjectId
				% (std::uint32_t) subObject->GetContainingNode()
				% actualValue;
				LOG_ERROR() << formatter.str();
				return Result(ErrorCode::OBJECT_ACTUAL_VALUE_EXCEEDS_HIGHLIMIT, formatter.str());
			}
			else if (res.GetErrorType() == ErrorCode::OBJECT_ACTUAL_VALUE_DECEEDS_LOWLIMIT)
			{
				boost::format formatter(kMsgBaseObjectLowLimitSubObjectError[static_cast<std::underlying_type<Language>::type>(LoggingConfiguration::GetInstance().GetCurrentLanguage())]);
				formatter
				% subObject->GetName()
				% objectId
				% subObjectId
				% (std::uint32_t) subObject->GetContainingNode()
				% actualValue;
				LOG_ERROR() << formatter.str();
				return Result(ErrorCode::OBJECT_ACTUAL_VALUE_DECEEDS_LOWLIMIT, formatter.str());
			}
			else if (!res.IsSuccessful())
				return res;

			//Log info actual value set
			boost::format formatter(kMsgSetSubObjectActualValue[static_cast<std::underlying_type<Language>::type>(LoggingConfiguration::GetInstance().GetCurrentLanguage())]);
			formatter
			% actualValue
			% objectId
			% subObjectId
			% (std::uint32_t) nodeId;
			LOG_INFO() << formatter.str();
		}
		else
		{
			subObject->ClearActualValue(); //Clear the actual value
		}
	}
	return res;
}

Result BaseNode::GetSubObject(std::uint32_t objectId, std::uint32_t subObjectId, std::shared_ptr<SubObject>& subObjRef, bool enableLog)
{
	auto iter = this->objectDictionary.find(objectId);
	if (iter == this->objectDictionary.end())
	{
		//Object does not exist
		boost::format formatter(kMsgNonExistingObject[static_cast<std::underlying_type<Language>::type>(LoggingConfiguration::GetInstance().GetCurrentLanguage())]);
		formatter
		% objectId
		% (std::uint32_t) nodeId;
		if (enableLog)
		{
			LOG_ERROR() << formatter.str();
		}
		return Result(ErrorCode::OBJECT_DOES_NOT_EXIST, formatter.str());
	}
	return iter->second->GetSubObject(subObjectId, subObjRef, enableLog);
}

bool BaseNode::IsEnabled() const
{
	return this->enabled;
}

void BaseNode::SetEnabled(bool _enabled)
{
	boost::format formatter(kMsgNodeDisable[static_cast<std::underlying_type<Language>::type>(LoggingConfiguration::GetInstance().GetCurrentLanguage())]);
	formatter
	% (std::uint32_t) this->GetNodeId()
	% _enabled;
	LOG_INFO() << formatter.str();
	this->enabled = _enabled;
}

std::vector<std::shared_ptr<BaseProcessImageObject>>& BaseNode::GetTransmitProcessImage()
{
	return this->transmitProcessImage;
}

std::vector<std::shared_ptr<BaseProcessImageObject>>& BaseNode::GetReceiveProcessImage()
{
	return this->receiveProcessImage;
}

std::uint32_t BaseNode::GetTransmitProcessImageSize() const
{
	std::uint32_t size = 0;
	for (auto& transPI : this->transmitProcessImage)
	{
		size += transPI->GetSize();
	}
	return size;
}

std::uint32_t BaseNode::GetReceiveProcessImageSize() const
{
	std::uint32_t size = 0;
	for (auto& recPI : this->receiveProcessImage)
	{
		size += recPI->GetSize();
	}
	return size;
}

void BaseNode::ChangeMappingChannelNodeId(const std::uint8_t _nodeId, const std::uint8_t newNodeId)
{
	for (auto obj : this->GetObjectDictionary())
	{
		if ((obj.first >= 0x1400 && obj.first <= 0x14FF)
		        || (obj.first >= 0x1800 && obj.first <= 0x18FF))
		{
			std::shared_ptr<SubObject> subObj;
			Result res = obj.second->GetSubObject(0x1, subObj);
			if (!res.IsSuccessful())
				continue;

			if (subObj->HasActualValue())
			{
				if (subObj->GetTypedActualValue<std::uint16_t>() == nodeId)
				{
					subObj->SetTypedObjectActualValue(IntToHex((std::uint16_t)newNodeId, 2, "0x"));
				}
			}
		}
	}

	for (auto map : this->GetReceiveMapping())
	{
		if (map->GetDestinationNode() == _nodeId)
			map->SetDestinationNode(newNodeId);
	}

	for (auto map : this->GetTransmitMapping())
	{
		if (map->GetDestinationNode() == _nodeId)
			map->SetDestinationNode(newNodeId);
	}
}

void BaseNode::ClearMappingChannelforNode(const std::uint8_t _nodeId)
{
	for (auto obj : this->GetObjectDictionary())
	{
		if ((obj.first >= 0x1400) && (obj.first <= 0x14FF))
		{
			std::shared_ptr<SubObject> subObj;
			Result res = obj.second->GetSubObject(0x1, subObj);
			if (!res.IsSuccessful())
				continue;

			if (subObj->HasActualValue())
			{
				if (subObj->GetTypedActualValue<std::uint16_t>() == _nodeId)
				{
					subObj->ClearActualValue();

					std::uint32_t mappingIndex = (obj.first - 0x1400) + 0x1600;
					std::shared_ptr<Object> mappingObj;
					res = this->GetObject(mappingIndex, mappingObj);
					if (!res.IsSuccessful())
						continue;

					for (auto map : mappingObj->GetSubObjectDictionary())
					{
						map.second->ClearActualValue();
					}
				}
			}
		}
		else if ((obj.first >= 0x1800) && (obj.first <= 0x18FF))
		{
			std::shared_ptr<SubObject> subObj;
			Result res = obj.second->GetSubObject(0x1, subObj);
			if (!res.IsSuccessful())
				continue;

			if (subObj->HasActualValue())
			{
				if (subObj->GetTypedActualValue<std::uint16_t>() == _nodeId)
				{
					subObj->ClearActualValue();

					std::uint32_t mappingIndex = (obj.first - 0x1800) + 0x1A00;
					std::shared_ptr<Object> mappingObj;
					res = this->GetObject(mappingIndex, mappingObj);
					if (!res.IsSuccessful())
						continue;

					for (auto map : mappingObj->GetSubObjectDictionary())
					{
						map.second->ClearActualValue();
					}
				}
			}
		}
	}
}

bool BaseNode::IgnoreNonExistingMappingObjects() const
{
	return this->ignoreNonExistingMappingObjects;
}

void BaseNode::SetIgnoreNonExistingMappingObjects(bool _ignoreNonExistingMappingObjects)
{
	this->ignoreNonExistingMappingObjects = _ignoreNonExistingMappingObjects;
}

bool BaseNode::IgnoreInvalidMappingOffsets() const
{
	return this->ignoreInvalidMappingOffsets;
}

void BaseNode::SetIgnoreInvalidMappingOffsets(bool _ignoreInvalidMappingOffsets)
{
	this->ignoreInvalidMappingOffsets = _ignoreInvalidMappingOffsets;
}
