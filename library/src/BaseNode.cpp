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

using IndustrialNetwork::POWERLINK::Core::Node::BaseNode;
using namespace IndustrialNetwork::POWERLINK::Core::ObjectDictionary;
using namespace IndustrialNetwork::POWERLINK::Core::ErrorHandling;

BaseNode::BaseNode(uint8_t nodeId, const std::string& nodeName) :
	nodeIdentifier(nodeId),
	nodeName(nodeName),
	objectDictionary(std::unordered_map<std::uint32_t, std::shared_ptr<IndustrialNetwork::POWERLINK::Core::ObjectDictionary::Object>>()),
	applicationProcess(new ApplicationProcess()),
	nodeAssignment(std::vector<IndustrialNetwork::POWERLINK::Core::Node::NodeAssignment>()),
	networkManagement(new NetworkManagement()),
	dynamicChannelList(std::vector<std::shared_ptr<DynamicChannel>>()),
	transmitMapping(std::vector<std::shared_ptr<IndustrialNetwork::POWERLINK::Core::ObjectDictionary::TxProcessDataMappingObject>>()),
	receiveMapping(std::vector<std::shared_ptr<IndustrialNetwork::POWERLINK::Core::ObjectDictionary::RxProcessDataMappingObject>>())
{}

IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result BaseNode::AddNodeObject(IndustrialNetwork::POWERLINK::Core::ObjectDictionary::Object& objRef)
{
	std::shared_ptr<Object> ptr = std::make_shared<Object>(objRef);
	this->objectDictionary.insert(std::pair<std::uint32_t, std::shared_ptr<Object>>(objRef.GetObjectIdentifier(), ptr));
	return Result();
}

IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result BaseNode::ForceNodeObject(std::uint32_t nodeId, std::string actualValue)
{
	this->objectDictionary.find(nodeId)->second.get()->SetForceToCDC(true);
	if(actualValue != "")
		this->objectDictionary.find(nodeId)->second.get()->SetObjectActualValue(actualValue);

	return Result();
}
IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result BaseNode::SetNodeObjectActualValue(std::uint32_t nodeId, std::string actualValue)
{
	this->objectDictionary.find(nodeId)->second.get()->SetObjectActualValue(actualValue);
	return Result();
}
IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result BaseNode::GetNodeObject(std::uint32_t nodeId, IndustrialNetwork::POWERLINK::Core::ObjectDictionary::Object& objRef)
{
	objRef = *this->objectDictionary.find(nodeId)->second.get();
	return Result();
}

BaseNode::~BaseNode()
{
	transmitMapping.clear();
	receiveMapping.clear();
}

const std::string& BaseNode::GetNodeName()
{
	return nodeName;
}

void BaseNode::SetNodeName(const std::string& nodeName)
{
	this->nodeName = nodeName;
}

uint8_t BaseNode::GetNodeIdentifier()
{
	return nodeIdentifier;
}

void BaseNode::SetNodeIdentifier(uint8_t nodeId)
{
	nodeIdentifier = nodeId;
}

const std::unordered_map<uint32_t, std::shared_ptr<IndustrialNetwork::POWERLINK::Core::ObjectDictionary::Object>>& BaseNode::GetObjectDictionary()
{
	return objectDictionary;
}

void BaseNode::SetObjectDictionary(const std::unordered_map<uint32_t, std::shared_ptr<IndustrialNetwork::POWERLINK::Core::ObjectDictionary::Object>>& od)
{
	objectDictionary = od;
}