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

using namespace IndustrialNetwork::POWERLINK::Core::ObjectDictionary;
using namespace IndustrialNetwork::POWERLINK::Core::ErrorHandling;
using namespace IndustrialNetwork::POWERLINK::Core::CoreConfiguration;

ApplicationProcess::ApplicationProcess() :
	parameterList(std::vector<std::shared_ptr<Parameter>>())
{}

ApplicationProcess::~ApplicationProcess()
{
	this->parameterList.clear();
}

const std::vector<std::shared_ptr<Parameter>>& ApplicationProcess::GetParameterList()
{
	return parameterList;
}

Result ApplicationProcess::AddParameter(std::shared_ptr<Parameter>& param)
{
	for (auto& currentParam : this->parameterList)
	{
		if (currentParam->GetUniqueID() == param->GetUniqueID())
		{
			boost::format formatter(kMsgParameterAlreadyExist);
			formatter
			% param->GetUniqueID();
			LOG_FATAL() << formatter.str();
			return Result(ErrorCode::PARAMETER_NOT_FOUND, formatter.str());
		}
	}
	this->parameterList.push_back(param);
	//Parameter created
	boost::format formatter(kMsgParameterCreated);
	formatter
	% param->GetUniqueID();
	LOG_INFO() << formatter.str();
	return Result();
}

Result ApplicationProcess::GetParameter(const std::string& uniqueId, std::shared_ptr<Parameter>& returnParam)
{
	for (auto& param : this->parameterList)
	{
		if (param->GetUniqueID() == uniqueId)
		{
			returnParam = param;
			//Parameter returned
			boost::format formatter(kMsgParameterReturned);
			formatter
			% param->GetUniqueID();
			LOG_INFO() << formatter.str();
			return Result();
		}
	}

	boost::format formatter(kMsgParameterAlreadyExist);
	formatter
	% uniqueId;
	LOG_FATAL() << formatter.str();
	return Result(ErrorCode::PARAMETER_NOT_FOUND, formatter.str());
}

Result ApplicationProcess::GetComplexDataType(const std::string& uniqueId, std::shared_ptr<ComplexDataType>& returnType)
{
	for (auto& param : this->parameterList)
	{
		if (param->GetUniqueID() == uniqueId)
		{
			returnType = param->GetComplexDataType();
			boost::format formatter(kMsgComplexDataTypeCreated);
			formatter
			% uniqueId;
			LOG_INFO() << formatter.str();
			return Result();
		}
	}
	boost::format formatter(kMsgComplexDatatypeNotFound);
	formatter
	% uniqueId;
	LOG_FATAL() << formatter.str();
	return Result(ErrorCode::COMPLEX_DATATYPE_NOT_FOUND, formatter.str());
}

uint32_t ApplicationProcess::GetBitSize(const std::string& uniqueId)
{
	for (auto& param : this->parameterList)
	{
		if (param->GetUniqueID() == uniqueId)
		{
			return param->GetComplexDataType()->GetBitSize();
		}
	}
	boost::format formatter(kMsgComplexDataTypeSizeInvalid);
	formatter
	% uniqueId;
	LOG_FATAL() << formatter.str();
	return 0;
}