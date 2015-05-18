/************************************************************************
\file NetworkManagement.cpp
\brief Implementation of the Class NetworkManagement
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
#include "Network.h"

using IndustrialNetwork::POWERLINK::Core::NetworkHandling::Network;
using namespace IndustrialNetwork::POWERLINK::Core::ErrorHandling;

Network::Network() :
	networkId(""),
	cycleTime(0),
	asyncMTU(0),
	multiplexedCycleLength(0),
	prescaler(0),
	configuration(),
	nodeCollection()
{}

Network::Network(const std::string id) :
	networkId(id),
	cycleTime(0),
	asyncMTU(0),
	multiplexedCycleLength(0),
	prescaler(0),
	configuration(),
	nodeCollection()
{}

Network::~Network()
{}

Result Network::AddNode(const IndustrialNetwork::POWERLINK::Core::Node::BaseNode& node)
{
	return Result();
}
Result Network::GetNode(const uint8_t nodeID, IndustrialNetwork::POWERLINK::Core::Node::BaseNode& node)
{
	return Result();
}
Result Network::RemoveNode(const uint8_t nodeID)
{
	return Result();
}
Result Network::ReplaceNode(const uint8_t nodeID, const IndustrialNetwork::POWERLINK::Core::Node::BaseNode& node)
{
	return Result();
}

std::vector<std::shared_ptr<IndustrialNetwork::POWERLINK::Core::Node::BaseNode>> Network::GetNodes(const IndustrialNetwork::POWERLINK::Core::Node::NodeType& type)
{
	return std::vector<std::shared_ptr<IndustrialNetwork::POWERLINK::Core::Node::BaseNode>>();
}

const std::string Network::GetNetworkId()
{
	return this->networkId;
}

const uint32_t Network::GetCycleTime()
{
	return this->cycleTime;
}

const uint32_t Network::GetAsyncMTU()
{
	return this->asyncMTU;
}

const uint32_t Network::GetMultiplexedCycleLength()
{
	return this->multiplexedCycleLength;
}

const uint32_t Network::GetPrescaler()
{
	return this->prescaler;
}

void Network::SetCycleTime(const uint32_t cycleTime)
{
	this->cycleTime = cycleTime;
}

void Network::SetAsyncMTU(const uint32_t asyncMTU)
{
	this->asyncMTU = asyncMTU;
}

void Network::SetMultiplexedCycleLength(const uint32_t multiCycleLength)
{
	this->multiplexedCycleLength = multiCycleLength;
}

void Network::SetPrescaler(const uint32_t prescaler)
{
	this->prescaler = prescaler;
}

