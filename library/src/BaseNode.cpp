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

using namespace std;
using namespace IndustrialNetwork::POWERLINK::Core::Node;
using namespace IndustrialNetwork::POWERLINK::Core::ObjectDictionary;
using namespace IndustrialNetwork::POWERLINK::Core::ErrorHandling;
using namespace IndustrialNetwork::POWERLINK::Core::CoreConfiguration;

BaseNode::BaseNode(uint8_t nodeId, const string& name) :
	nodeId(nodeId),
	name(name),
	objectDictionary(map<uint32_t, shared_ptr<Object>>()),
	applicationProcess(make_shared<ApplicationProcess>()),
	nodeAssignment(vector<NodeAssignment>()),
	networkManagement(make_shared<NetworkManagement>()),
	dynamicChannelList(vector<shared_ptr<DynamicChannel>>()),
	transmitMapping(vector<shared_ptr<TxProcessDataMappingObject>>()),
	receiveMapping(vector<shared_ptr<RxProcessDataMappingObject>>())
{}

BaseNode::~BaseNode()
{
	transmitMapping.clear();
	receiveMapping.clear();
}

const string& BaseNode::GetName()
{
	return this->name;
}

void BaseNode::SetName(const string& name)
{
	this->name = name;
}

uint8_t BaseNode::GetNodeIdentifier()
{
	return this->nodeId;
}

void BaseNode::SetNodeIdentifier(uint8_t nodeId)
{
	this->nodeId = nodeId;
}

const map<uint32_t, shared_ptr<Object>>& BaseNode::GetObjectDictionary()
{
	return objectDictionary;
}

void BaseNode::SetObjectDictionary(const map<uint32_t, shared_ptr<Object>>& od)
{
	objectDictionary = od;
}

vector<NodeAssignment>& BaseNode::GetNodeAssignment()
{
	return this->nodeAssignment;
}

shared_ptr<NetworkManagement>& BaseNode::GetNetworkManagement()
{
	return this->networkManagement;
}

shared_ptr<ApplicationProcess>& BaseNode::GetApplicationProcess()
{
	return this->applicationProcess;
}

Result BaseNode::AddObject(shared_ptr<Object>& objRef)
{
	if (this->objectDictionary.find(objRef->GetId()) != this->objectDictionary.end())
	{
		//Object already exists
		boost::format formatter(kMsgExistingObject);
		formatter
		% objRef->GetId()
		% (uint32_t) nodeId;
		LOG_FATAL() << formatter.str();
		return Result(ErrorCode::OBJECT_EXISTS, formatter.str());
	}

	this->objectDictionary.insert(pair<uint32_t, shared_ptr<Object>>(objRef->GetId(), objRef));

	//Log object creation
	boost::format formatter(kMsgObjectCreated);
	formatter
	% objRef->GetId()
	% (uint32_t) nodeId;
	LOG_INFO() << formatter.str();
	return Result();
}

Result BaseNode::AddSubObject(uint32_t objectId, shared_ptr<SubObject>& objRef)
{
	if (this->objectDictionary.find(objectId) == this->objectDictionary.end())
	{
		//Object does not exist
		boost::format formatter(kMsgNonExistingObject);
		formatter
		% objRef->GetId()
		% (uint32_t) nodeId;
		LOG_FATAL() << formatter.str();
		return Result(ErrorCode::OBJECT_DOES_NOT_EXIST, formatter.str());
	}

	return this->objectDictionary.find(objectId)->second->AddSubobject(objRef);
}

Result BaseNode::ForceObject(uint32_t objectId, bool force, string actualValue)
{
	auto iter = this->objectDictionary.find(objectId);
	if (iter == this->objectDictionary.end())
	{
		//Object does not exist
		boost::format formatter(kMsgNonExistingObject);
		formatter
		% objectId
		% (uint32_t) nodeId;
		LOG_FATAL() << formatter.str();
		return Result(ErrorCode::OBJECT_DOES_NOT_EXIST, formatter.str());
	}

	iter->second->SetForceToCDC(force);
	//Log info forced object
	boost::format formatter(kMsgForceObject);
	formatter
	% objectId
	% (uint32_t) nodeId;
	LOG_INFO() << formatter.str();

	if (!actualValue.empty())
	{
		Result res = iter->second->SetTypedObjectActualValue(actualValue);
		if(!res.IsSuccessful())
			return res;

		//Log info actual value set
		boost::format formatter(kMsgSetObjectActualValue);
		formatter
		% actualValue
		% objectId
		% nodeId
		% force;
		LOG_INFO() << formatter.str();
	}

	return Result();
}

Result BaseNode::SetObjectActualValue(uint32_t objectId, string actualValue)
{
	auto iter = this->objectDictionary.find(objectId);
	if (iter == this->objectDictionary.end())
	{
		//Object does not exist
		boost::format formatter(kMsgNonExistingObject);
		formatter
		% objectId
		% (uint32_t) nodeId;
		LOG_FATAL() << formatter.str();
		return Result(ErrorCode::OBJECT_DOES_NOT_EXIST, formatter.str());
	}
	if (iter->second->GetForceToCDC())
	{
		//Actual value must not be set because forced
		boost::format formatter(kMsgForcedValueOverwriteObject);
		formatter
		% actualValue
		% iter->first
		% (uint32_t) this->GetNodeIdentifier();
		LOG_FATAL() << formatter.str();
		return Result(ErrorCode::FORCED_VALUE_OVERWRITE, formatter.str());
	}
	if (!actualValue.empty())
	{
		Result res = iter->second->SetTypedObjectActualValue(actualValue);
		if(!res.IsSuccessful())
			return res;

		//Log info actual value set
		boost::format formatter(kMsgSetObjectActualValue);
		formatter
		% actualValue
		% objectId
		% (uint32_t) nodeId;
		LOG_INFO() << formatter.str();
		return Result();
	}
	else
	{
		//Actual value must not be set to empty
		boost::format formatter(kMsgEmptyArgument);
		formatter
		% "actual value";
		LOG_FATAL() << formatter.str();
		return Result(ErrorCode::ARGUMENT_INVALID_EMPTY, formatter.str());
	}

}

Result BaseNode::GetObject(uint32_t objectId, shared_ptr<Object>& objRef)
{
	auto iter = this->objectDictionary.find(objectId);
	if (iter == this->objectDictionary.end())
	{
		//Object does not exist
		boost::format formatter(kMsgNonExistingObject);
		formatter
		% objectId
		% (uint32_t) nodeId;
		LOG_FATAL() << formatter.str();
		return Result(ErrorCode::OBJECT_DOES_NOT_EXIST, formatter.str());
	}
	objRef = iter->second;
	return Result();
}

Result BaseNode::ForceSubObject(uint32_t objectId, uint32_t subObjectId, bool force, string actualValue)
{
	auto iter = this->objectDictionary.find(objectId);
	if (iter == this->objectDictionary.end())
	{
		//Object does not exist
		boost::format formatter(kMsgNonExistingObject);
		formatter
		% objectId
		% (uint32_t) nodeId;
		LOG_FATAL() << formatter.str();
		return Result(ErrorCode::OBJECT_DOES_NOT_EXIST, formatter.str());
	}

	shared_ptr<SubObject> subObject;
	Result res = iter->second->GetSubObject(subObjectId, subObject);
	if (res.IsSuccessful())
	{
		subObject->SetForceToCDC(force);
		//Log info forced subobject
		boost::format formatter(kMsgForceSubObject);
		formatter
		% objectId
		% subObjectId
		% (uint32_t) nodeId
		% force;
		LOG_INFO() << formatter.str();

		if (!actualValue.empty())
		{
			res = subObject->SetTypedObjectActualValue(actualValue);
			if(!res.IsSuccessful())
				return res;

			//Log info actual value set
			boost::format formatter(kMsgSetSubObjectActualValue);
			formatter
			% actualValue
			% objectId
			% subObjectId
			% (uint32_t) nodeId;
			LOG_INFO() << formatter.str();
		}
		else
		{
			//Actual value must not be set to empty
			boost::format formatter(kMsgEmptyArgument);
			formatter
			% "actual value";
			LOG_FATAL() << formatter.str();
			res = Result(ErrorCode::ARGUMENT_INVALID_EMPTY, formatter.str());
		}
	}
	return res;
}

Result BaseNode::SetSubObjectActualValue(uint32_t objectId, uint32_t subObjectId, string actualValue)
{
	auto iter = this->objectDictionary.find(objectId);
	if (iter == this->objectDictionary.end())
	{
		//Object does not exist
		boost::format formatter(kMsgNonExistingObject);
		formatter
		% objectId
		% (uint32_t) nodeId;
		LOG_FATAL() << formatter.str();
		return Result(ErrorCode::OBJECT_DOES_NOT_EXIST, formatter.str());
	}

	shared_ptr<SubObject> subObject;
	Result res = iter->second->GetSubObject(subObjectId, subObject);
	if (res.IsSuccessful())
	{
		if (subObject->GetForceToCDC())
		{
			//Actual value must not be set because forced
			boost::format formatter(kMsgForcedValueOverwriteSubObject);
			formatter
			% actualValue
			% objectId
			% subObject->GetId()
			% (uint32_t) this->GetNodeIdentifier();
			LOG_WARN() << formatter.str();
			return Result();
		}
		if (!actualValue.empty())
		{
			res = subObject->SetTypedObjectActualValue(actualValue);
			if(!res.IsSuccessful())
				return res;

			//Log info actual value set
			boost::format formatter(kMsgSetSubObjectActualValue);
			formatter
			% actualValue
			% objectId
			% subObjectId
			% (uint32_t) nodeId;
			LOG_INFO() << formatter.str();
		}
		else
		{
			//Actual value must not be set to empty
			boost::format formatter(kMsgEmptyArgument);
			formatter
			% "actual value";
			LOG_FATAL() << formatter.str();
			res = Result(ErrorCode::ARGUMENT_INVALID_EMPTY, formatter.str());
		}
	}
	return res;
}

Result BaseNode::GetSubObject(uint32_t objectId, uint32_t subObjectId, shared_ptr<IndustrialNetwork::POWERLINK::Core::ObjectDictionary::SubObject>& subObjRef)
{
	auto iter = this->objectDictionary.find(objectId);
	if (iter == this->objectDictionary.end())
	{
		//Object does not exist
		boost::format formatter(kMsgNonExistingObject);
		formatter
		% objectId
		% (uint32_t) nodeId;
		LOG_FATAL() << formatter.str();
		return Result(ErrorCode::OBJECT_DOES_NOT_EXIST, formatter.str());
	}
	return iter->second->GetSubObject(subObjectId, subObjRef);
}
