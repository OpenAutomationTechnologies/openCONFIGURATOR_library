/************************************************************************
\file BaseIndex.cpp
\brief Implementation of the Class BaseIndex
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
#include "BaseObject.h"

using namespace IndustrialNetwork::POWERLINK::Core::ObjectDictionary;

BaseObject::BaseObject(uint32_t id,  PlkDataType type) : IBaseObject(id, type),
	forceToCDC(false),
	highLimit(),
	lowLimit(),
	uniqueIdRef(),
	accessType(),
	objectType(),
	pdoMapping()
{}

BaseObject::~BaseObject()
{}

bool BaseObject::operator== (const BaseObject& baseObject) const
{
	return (baseObject.GetObjectIdentifier() == this->GetObjectIdentifier());
}

bool BaseObject::GetForceToCDC() const
{
	return forceToCDC;
}

void BaseObject::SetForceToCDC(bool force)
{
	this->forceToCDC = force;
}

boost::optional<uint32_t> BaseObject::GetHighLimit() const
{
	return highLimit;
}

void BaseObject::SetHighLimit(uint32_t highLimit)
{
	this->highLimit = highLimit;
}

boost::optional<std::string> BaseObject::GetUniqueIdRef() const
{
	return uniqueIdRef;
}

void BaseObject::SetUniqueIdRef(boost::optional<std::string>& uniqueIdRef)
{
	this->uniqueIdRef = uniqueIdRef;
}

IndustrialNetwork::POWERLINK::Core::ObjectDictionary::AccessType BaseObject::GetAccessType() const
{
	return accessType;
}

void BaseObject::SetAccessType(IndustrialNetwork::POWERLINK::Core::ObjectDictionary::AccessType accessType)
{
	this->accessType = accessType;
}

IndustrialNetwork::POWERLINK::Core::ObjectDictionary::ObjectType BaseObject::GetObjectType() const
{
	return objectType;
}

void BaseObject::SetObjectType(IndustrialNetwork::POWERLINK::Core::ObjectDictionary::ObjectType objectType)
{
	this->objectType = objectType;
}

IndustrialNetwork::POWERLINK::Core::ObjectDictionary::PDOMapping BaseObject::GetPDOMapping() const
{
	return pdoMapping;
}

void BaseObject::SetPDOMapping(IndustrialNetwork::POWERLINK::Core::ObjectDictionary::PDOMapping pdoMapping)
{
	this->pdoMapping = pdoMapping;
}