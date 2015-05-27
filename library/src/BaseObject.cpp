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

using namespace std;
using namespace IndustrialNetwork::POWERLINK::Core::ObjectDictionary;

BaseObject::BaseObject() : IBaseObject(),
	forceToCDC(false),
	highLimit(),
	lowLimit(),
	uniqueIdRef(),
	accessType(),
	objectType(),
	pdoMapping()
{}

BaseObject::BaseObject(uint32_t id,  PlkDataType type) : IBaseObject(id, type),
	forceToCDC(false),
	highLimit(),
	lowLimit(),
	uniqueIdRef(),
	accessType(),
	objectType(),
	pdoMapping()
{}


BaseObject::BaseObject(uint32_t id, string defaultValue, PlkDataType type, AccessType accessType, ObjectType objectType, PDOMapping pdoMapping, string name) : IBaseObject(id, defaultValue, type, name),
	forceToCDC(false),
	highLimit(),
	lowLimit(),
	uniqueIdRef(),
	accessType(accessType),
	objectType(objectType),
	pdoMapping(pdoMapping)
{}

BaseObject::BaseObject(uint32_t id, string defaultValue, PlkDataType type, AccessType accessType, ObjectType objectType, PDOMapping pdoMapping, uint32_t highlimit, uint32_t lowLimit, string uniqueIdRef, string name) : IBaseObject(id, defaultValue, type, name),
	forceToCDC(false),
	highLimit(highlimit),
	lowLimit(lowLimit),
	uniqueIdRef(uniqueIdRef),
	accessType(accessType),
	objectType(objectType),
	pdoMapping(pdoMapping)
{}

BaseObject::~BaseObject()
{}

bool BaseObject::operator== (const BaseObject& baseObject) const
{
	return (baseObject.GetId() == this->GetId());
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

boost::optional<uint32_t> BaseObject::GetLowLimit() const
{
	return lowLimit;
}

void BaseObject::SetLowLimit(uint32_t lowLimit)
{
	this->lowLimit = lowLimit;
}

boost::optional<string> BaseObject::GetUniqueIdRef() const
{
	return uniqueIdRef;
}

void BaseObject::SetUniqueIdRef(boost::optional<string>& uniqueIdRef)
{
	this->uniqueIdRef = uniqueIdRef;
}

AccessType BaseObject::GetAccessType() const
{
	return accessType;
}

void BaseObject::SetAccessType(AccessType accessType)
{
	this->accessType = accessType;
}

ObjectType BaseObject::GetObjectType() const
{
	return objectType;
}

void BaseObject::SetObjectType(ObjectType objectType)
{
	this->objectType = objectType;
}

PDOMapping BaseObject::GetPDOMapping() const
{
	return pdoMapping;
}

void BaseObject::SetPDOMapping(PDOMapping pdoMapping)
{
	this->pdoMapping = pdoMapping;
}