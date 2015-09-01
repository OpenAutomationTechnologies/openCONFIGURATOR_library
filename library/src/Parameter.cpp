/************************************************************************
\file Parameter.cpp
\brief Implementation of the Class Parameter
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
#include "Parameter.h"

using namespace IndustrialNetwork::POWERLINK::Core::ObjectDictionary;
using namespace IndustrialNetwork::POWERLINK::Core::Utilities;

Parameter::Parameter(const std::string& uniqueID, ParameterAccess parameterAccess, const std::string& uniqueIDRef) :
	uniqueID(uniqueID),
	uniqueIDRef(uniqueIDRef),
	complexDataType(),
	parameterAccess(parameterAccess),
	dataType(IEC_Datatype::UNDEFINED)
{}

Parameter::Parameter(const std::string& uniqueID, ParameterAccess parameterAccess, IEC_Datatype dataType) :
	uniqueID(uniqueID),
	uniqueIDRef(),
	complexDataType(),
	parameterAccess(parameterAccess),
	dataType(dataType)
{}

Parameter::~Parameter()
{}

const std::string& Parameter::GetUniqueID()
{
	return this->uniqueID;
}

const std::string& Parameter::GetUniqueIDRef()
{
	return this->uniqueIDRef;
}

void Parameter::SetUniqueIDRef(const std::string& uniqueIDRef)
{
	this->uniqueIDRef = uniqueIDRef;
}

const std::shared_ptr<ComplexDataType>& Parameter::GetComplexDataType()
{
	return this->complexDataType;
}

void Parameter::SetComplexDataType(std::shared_ptr<ComplexDataType>& complexType)
{
	this->complexDataType = complexType;
}

ParameterAccess Parameter::GetParameterAccess()
{
	return this->parameterAccess;
}

std::uint32_t Parameter::GetBitSize()
{
	if (!this->uniqueIDRef.empty())
		return this->complexDataType->GetBitSize();
	else
		return GetIECDataTypeBitSize(this->dataType);
}