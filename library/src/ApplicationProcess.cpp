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
	datatypeList(std::vector<std::shared_ptr<ComplexDataType>>()),
	parameterList(std::vector<std::shared_ptr<Parameter>>()),
	parameterGroupList(std::vector<std::shared_ptr<ParameterGroup>>()),
	parameterTemplateList(std::vector<std::shared_ptr<ParameterTemplate>>())
{}

ApplicationProcess::~ApplicationProcess()
{
	this->datatypeList.clear();
	this->parameterList.clear();
	this->parameterGroupList.clear();
	this->parameterTemplateList.clear();
}

const std::vector<std::shared_ptr<Parameter>>& ApplicationProcess::GetParameterList()
{
	return this->parameterList;
}

const std::vector<std::shared_ptr<ComplexDataType>>& ApplicationProcess::GetDataTypeList()
{
	return this->datatypeList;
}

Result ApplicationProcess::AddComplexDataType(std::shared_ptr<ComplexDataType>& dt)
{
	for (auto& currentDt : this->datatypeList)
	{
		if (currentDt->GetUniqueID() == dt->GetUniqueID())
		{
			boost::format formatter(kMsgComplexDataTypeAlreadyExist);
			formatter
			% dt->GetUniqueID();
			LOG_ERROR() << formatter.str();
			return Result(ErrorCode::COMPLEX_DATATYPE_ALREADY_EXIST, formatter.str());
		}
	}
	this->datatypeList.push_back(dt);
	return Result();
}

Result ApplicationProcess::GetComplexDataType(const std::string& uniqueId, std::shared_ptr<ComplexDataType>& dt)
{
	for (auto& currentDt : this->datatypeList)
	{
		if (currentDt->GetUniqueID() == uniqueId)
		{
			dt = currentDt;
			return Result();
		}
	}

	boost::format formatter(kMsgComplexDatatypeNotFound);
	formatter
	% uniqueId;
	LOG_ERROR() << formatter.str();
	return Result(ErrorCode::COMPLEX_DATATYPE_NOT_FOUND, formatter.str());
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
			LOG_ERROR() << formatter.str();
			return Result(ErrorCode::PARAMETER_EXISTS, formatter.str());
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

Result ApplicationProcess::AddParameterGroup(std::shared_ptr<ParameterGroup>& paramGrp)
{
	for (auto& currentParamGrp : this->parameterGroupList)
	{
		if (currentParamGrp->GetUniqueId() == paramGrp->GetUniqueId())
		{
			boost::format formatter(kMsgParameterGroupAlreadyExist);
			formatter
			% paramGrp->GetUniqueId();
			LOG_ERROR() << formatter.str();
			return Result(ErrorCode::PARAMETER_GROUP_EXISTS, formatter.str());
		}
	}
	this->parameterGroupList.push_back(paramGrp);
	return Result();
}

Result ApplicationProcess::AddParameterTemplate(std::shared_ptr<ParameterTemplate>& paramTempl)
{
	for (auto& currentParamTempl : this->parameterTemplateList)
	{
		if (currentParamTempl->GetUniqueID() == paramTempl->GetUniqueID())
		{
			boost::format formatter(kMsgParameterTemplateAlreadyExist);
			formatter
			% paramTempl->GetUniqueID();
			LOG_ERROR() << formatter.str();
			return Result(ErrorCode::PARAMETER_TEMPLATE_EXISTS, formatter.str());
		}
	}
	this->parameterTemplateList.push_back(paramTempl);
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

	boost::format formatter(kMsgParameterNotFound);
	formatter
	% uniqueId;
	//LOG_ERROR() << formatter.str();
	return Result(ErrorCode::PARAMETER_NOT_FOUND, formatter.str());
}

Result ApplicationProcess::GetParameterGroup(const std::string& uniqueId, std::shared_ptr<ParameterGroup>& returnParam)
{
	for (auto& paramGrp : this->parameterGroupList)
	{
		if (paramGrp->GetUniqueId() == uniqueId)
		{
			returnParam = paramGrp;
			return Result();
		}
	}

	boost::format formatter(kMsgParameterGroupNonExisting);
	formatter
	% uniqueId;
	//LOG_ERROR() << formatter.str();
	return Result(ErrorCode::PARAMETER_GROUP_DOES_NOT_EXIST, formatter.str());
}
Result ApplicationProcess::GetParameterTemplate(const std::string& uniqueId, std::shared_ptr<ParameterTemplate>& returnParam)
{
	for (auto& paramTempl : this->parameterTemplateList)
	{
		if (paramTempl->GetUniqueID() == uniqueId)
		{
			returnParam = paramTempl;
			return Result();
		}
	}

	boost::format formatter(kMsgParameterTemplateNonExisting);
	formatter
	% uniqueId;
	LOG_ERROR() << formatter.str();
	return Result(ErrorCode::PARAMETER_TEMPLATE_DOES_NOT_EXIST, formatter.str());
}

Result ApplicationProcess::SetParameterActualValue(const std::string& uniqueId, const std::string& actualValue)
{
	for (auto& currentParam : this->parameterList)
	{
		if (currentParam->GetUniqueID() == uniqueId)
			return currentParam->SetParameterActualValue(actualValue);
	}
	boost::format formatter(kMsgParameterNotFound);
	formatter
	% uniqueId;
	LOG_ERROR() << formatter.str();
	return Result(ErrorCode::PARAMETER_NOT_FOUND, formatter.str());
}

Result ApplicationProcess::GetParameterActualValue(const std::string& uniqueId, std::string& actualValue)
{
	for (auto& currentParam : this->parameterList)
	{
		if (currentParam->GetUniqueID() == uniqueId)
		{
			if (currentParam->HasActualValue())
			{
				actualValue = currentParam->GetTypedParameterActualValue<std::string>();
				return Result();
			}
			else
			{
				boost::format formatter(kMsgParameterActualValueDoesNotExist);
				formatter
				% uniqueId;
				LOG_ERROR() << formatter.str();
				return Result(ErrorCode::PARAMETER_HAS_NO_ACTUAL_VALUE, formatter.str());
			}
		}
	}
	boost::format formatter(kMsgParameterNotFound);
	formatter
	% uniqueId;
	LOG_ERROR() << formatter.str();
	return Result(ErrorCode::PARAMETER_NOT_FOUND, formatter.str());
}

std::uint32_t ApplicationProcess::GetBitSize(const std::string& uniqueId)
{
	for (auto& param : this->parameterList)
	{
		if (param->GetUniqueID() == uniqueId)
			return param->GetBitSize();
	}

	boost::format formatter(kMsgParameterNotFound);
	formatter
	% uniqueId;
	LOG_ERROR() << formatter.str();
	return 0;
}