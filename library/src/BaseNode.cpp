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

BaseNode::BaseNode(uint8_t nodeId, const string& name) :
	nodeId(nodeId),
	name(name),
	objectDictionary(unordered_map<uint32_t, shared_ptr<Object>>()),
	applicationProcess(make_shared<ApplicationProcess>()),
	nodeAssignment(vector<NodeAssignment>()),
	networkManagement(make_shared<NetworkManagement>()),
	dynamicChannelList(vector<shared_ptr<DynamicChannel>>()),
	transmitMapping(vector<shared_ptr<TxProcessDataMappingObject>>()),
	receiveMapping(vector<shared_ptr<RxProcessDataMappingObject>>())
{}

Result BaseNode::AddObject(shared_ptr<Object>& objRef)
{
	if (this->objectDictionary.find(objRef.get()->GetId()) != this->objectDictionary.end())
		return Result(ErrorCode::OBJECT_EXISTS);

	this->objectDictionary.insert(pair<uint32_t, shared_ptr<Object>>(objRef.get()->GetId(), objRef));
	return Result();
}

Result BaseNode::AddSubObject(std::uint32_t objectId, shared_ptr<SubObject>& objRef)
{
	if (this->objectDictionary.find(objectId) == this->objectDictionary.end())
		return Result(ErrorCode::OBJECT_DOES_NOT_EXIST);

	return this->objectDictionary.find(objRef.get()->GetId())->second.get()->AddSubobject(objRef);
}

Result BaseNode::ForceObject(uint32_t nodeId, string actualValue)
{
	this->objectDictionary.find(nodeId)->second.get()->SetForceToCDC(true);
	if (actualValue != "")
		this->objectDictionary.find(nodeId)->second.get()->SetActualValue(actualValue);

	return Result();
}

Result BaseNode::SetObjectActualValue(uint32_t nodeId, string actualValue)
{
	this->objectDictionary.find(nodeId)->second.get()->SetActualValue(actualValue);
	return Result();
}

Result BaseNode::GetObject(uint32_t nodeId, Object& objRef)
{
	objRef = *this->objectDictionary.find(nodeId)->second.get();
	return Result();
}

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

const unordered_map<uint32_t, shared_ptr<Object>>& BaseNode::GetObjectDictionary()
{
	return objectDictionary;
}

void BaseNode::SetObjectDictionary(const unordered_map<uint32_t, shared_ptr<Object>>& od)
{
	objectDictionary = od;
}

std::vector<NodeAssignment>& BaseNode::GetNodeAssignment()
{
	return this->nodeAssignment;
}

std::shared_ptr<NetworkManagement>& BaseNode::GetNetworkManagement()
{
	return this->networkManagement;
}