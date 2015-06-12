/************************************************************************
\file ApplicationProcess.cpp
\brief Implementation of the Class ApplicationProcess
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
#include "ApplicationProcess.h"

using namespace std;
using namespace IndustrialNetwork::POWERLINK::Core::ObjectDictionary;
using namespace IndustrialNetwork::POWERLINK::Core::ErrorHandling;

ApplicationProcess::ApplicationProcess() :
	parameterList(vector<shared_ptr<Parameter>>())
{}

ApplicationProcess::~ApplicationProcess()
{}

const vector<shared_ptr<Parameter>>& ApplicationProcess::GetParameterList()
{
	return parameterList;
}

Result ApplicationProcess::AddParameter(shared_ptr<Parameter>& param)
{
	for (auto& currentParam : this->parameterList)
	{
		if (currentParam.get()->GetUniqueID() == param.get()->GetUniqueID())
			return Result(ErrorCode::PARAMETER_NOT_FOUND);
	}
	this->parameterList.push_back(param);
	return Result();
}

Result ApplicationProcess::GetParameter(string uniqueId, shared_ptr<Parameter>& returnParam)
{
	for (auto& param : this->parameterList)
	{
		if (param.get()->GetUniqueID() == uniqueId)
		{
			returnParam = param;
			return Result();
		}
	}
	return Result(ErrorCode::PARAMETER_NOT_FOUND);
}

Result ApplicationProcess::GetComplexDataType(string uniqueId, shared_ptr<ComplexDataType>& returnType)
{
	for (auto& param : this->parameterList)
	{
		if (param.get()->GetUniqueID() == uniqueId)
		{
			returnType = param.get()->GetComplexDataType();
			return Result();
		}
	}
	return Result(ErrorCode::PARAMETER_NOT_FOUND);
}

uint32_t ApplicationProcess::GetBitSize(std::string uniqueId)
{
	for (auto& param : this->parameterList)
	{
		if (param.get()->GetUniqueID() == uniqueId)
		{
			return param.get()->GetComplexDataType().get()->GetBitSize();
		}
	}
	return 0;
}