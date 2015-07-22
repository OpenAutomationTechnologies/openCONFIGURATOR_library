/************************************************************************
\file DynamicChannel.cpp
\brief Implementation of the Class DynamicChannel
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
#include "DynamicChannel.h"


using namespace IndustrialNetwork::POWERLINK::Core::Node;
using namespace IndustrialNetwork::POWERLINK::Core::ObjectDictionary;

DynamicChannel::DynamicChannel(PlkDataType dataType, AccessType accessType, uint32_t startIndex, uint32_t endIndex, uint32_t maxNumber, uint32_t addressOffset, uint8_t bitAlignment) :
	dataType(dataType),
	accessType(accessType),
	startIndex(startIndex),
	endIndex(endIndex),
	maxNumber(maxNumber),
	addressOffset(addressOffset),
	bitAlignment((bitAlignment == 0) ? boost::optional<uint8_t>() : boost::optional<uint8_t>(bitAlignment))
{}


DynamicChannel::~DynamicChannel()
{}

PlkDataType DynamicChannel::GetDataType()
{
	return this->dataType;
}

AccessType DynamicChannel::GetAccessType()
{
	return this->accessType;
}

uint32_t DynamicChannel::GetStartIndex()
{
	return this->startIndex;
}

uint32_t DynamicChannel::GetMaxNumber()
{
	return this->maxNumber;
}

uint32_t DynamicChannel::GetAddressOffset()
{
	return this->addressOffset;
}

const boost::optional<std::uint32_t> DynamicChannel::GetBitAlignment()
{
	return this->bitAlignment;
}