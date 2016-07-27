/************************************************************************
\file Module.cpp
\brief Implementation of the Class Module
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
#include "Module.h"

using namespace IndustrialNetwork::POWERLINK::Core::ModularNode;
using namespace IndustrialNetwork::POWERLINK::Core::ErrorHandling;
using namespace IndustrialNetwork::POWERLINK::Core::Node;
using namespace IndustrialNetwork::POWERLINK::Core::ObjectDictionary;
using namespace IndustrialNetwork::POWERLINK::Core::CoreConfiguration;

Module::Module(std::uint8_t containingNode, const std::string& moduleId, const std::string& moduleType, const ModuleAddressing& addressing, std::uint32_t address, std::uint32_t position, const std::string& moduleName): BaseNode(containingNode, moduleName),
	position(position),
	address(address),
	moduleInterface(std::shared_ptr<ModuleInterface>(new ModuleInterface(moduleId, moduleType, addressing, 0, 0, 0, 0, 0))),
	parameterNameMapping(std::map<std::string, std::string>()),
	disabledSubindices(std::map<std::pair<std::uint32_t, std::uint32_t>, std::shared_ptr<SubObject>>())
{}

Module::Module(std::uint8_t containingNode, const std::string& moduleId, const std::string& moduleType, const ModuleAddressing& addressing, std::uint32_t address, std::uint32_t position, const std::string& moduleName, std::uint16_t minPosition, std::uint16_t maxPosition, std::uint16_t minAddress, std::uint16_t maxAddress, std::uint16_t maxCount) : BaseNode(containingNode, moduleName),
	position(position),
	address(address),
	moduleInterface(std::shared_ptr<ModuleInterface>(new ModuleInterface(moduleId, moduleType, addressing, minPosition, maxPosition, minAddress, maxAddress, maxCount))),
	parameterNameMapping(std::map<std::string, std::string>()),
	disabledSubindices(std::map<std::pair<std::uint32_t, std::uint32_t>, std::shared_ptr<SubObject>>())
{}

Module::~Module()
{}

const std::string& Module::GetModuleId() const
{
	return this->moduleInterface->GetUniqueId();
}

Result Module::AddNodeAssignment(const NodeAssignment&)
{
	return Result();
}

Result Module::RemoveNodeAssignment(const NodeAssignment&)
{
	return Result();
}

std::uint32_t Module::GetNodeAssignmentValue()
{
	return 0;
}

std::uint32_t Module::GetConfigurationObjectCount()
{
	return 0;
}

std::uint32_t Module::GetConfigurationObjectSize()
{
	return 0;
}

Result Module::CalculatePReqPayloadLimit()
{
	return Result();
}

Result Module::CalculatePResPayloadLimit()
{
	return Result();
}

std::uint32_t Module::GetAddress() const
{
	return this->address;
}

void Module::SetAddress(std::uint32_t _address)
{
	this->address = _address;
}

std::uint32_t Module::GetPosition() const
{
	return this->position;
}

void Module::SetPosition(std::uint32_t _position)
{
	if (this->moduleInterface->GetModuleAddressing() == ModuleAddressing::POSITION)
	{
		for (auto& obj : this->GetObjectDictionary())
		{
			for (auto subObj = obj.second->GetSubObjectDictionary().begin(); subObj != obj.second->GetSubObjectDictionary().end(); ++subObj)
			{
				if (subObj->first == this->position && this->position == subObj->second->GetModulePosition())
				{
					if (obj.second->GetSubObjectDictionary().find(_position) != obj.second->GetSubObjectDictionary().end())
					{
						auto oldSubobject = obj.second->GetSubObjectDictionary().find(this->position);
						auto newSubobject = obj.second->GetSubObjectDictionary().find(_position);

						std::pair<std::uint32_t, std::shared_ptr<SubObject>> switchedoldSubobject(_position, oldSubobject->second);
						std::pair<std::uint32_t, std::shared_ptr<SubObject>> switchedNewSubObject(this->position, newSubobject->second);

						oldSubobject->second->SetModulePosition(_position);
						newSubobject->second->SetModulePosition(this->position);

						obj.second->GetSubObjectDictionary().erase(oldSubobject);
						obj.second->GetSubObjectDictionary().erase(newSubobject);

						obj.second->GetSubObjectDictionary().insert(switchedoldSubobject);
						obj.second->GetSubObjectDictionary().insert(switchedNewSubObject);
						break;
					}
					else
					{
						subObj->second->SetModulePosition(_position);
						obj.second->GetSubObjectDictionary().insert(std::pair<std::uint32_t, std::shared_ptr<SubObject>>(_position, subObj->second));
						obj.second->GetSubObjectDictionary().erase(subObj);
						break;
					}
				}
			}
		}
		this->SetAddress(_position);
	}
	this->position = _position;
}

const std::map<std::string, std::string>& Module::GetParameterNameMapping() const
{
	return this->parameterNameMapping;
}

Result Module::CreateParamMapping(const std::string& paramName, const std::string& mappedParamName)
{
	if (!this->parameterNameMapping.empty())
	{
		if (this->parameterNameMapping.find(paramName) != this->parameterNameMapping.end())
		{
			boost::format formatter(kMsgParameterAlreadyExist);
			formatter
			% paramName;
			LOG_ERROR() << formatter.str();
			return Result(ErrorCode::PARAMETER_EXISTS, formatter.str());
		}
	}
	this->parameterNameMapping.insert(std::pair<std::string, std::string>(paramName, mappedParamName));
	return Result();
}

Result Module::GetMappedParameterName(const std::string& parameterName, std::string& mappedParameterName)
{
	if (!this->parameterNameMapping.empty())
	{
		if (this->parameterNameMapping.find(parameterName) == this->parameterNameMapping.end())
		{
			boost::format formatter(kMsgParameterNotFound);
			formatter
			% parameterName;
			LOG_ERROR() << formatter.str();
			return Result(ErrorCode::PARAMETER_NOT_FOUND, formatter.str());
		}
	}
	else
	{
		boost::format formatter(kMsgParameterNotFound);
		formatter
		% parameterName;
		LOG_ERROR() << formatter.str();
		return Result(ErrorCode::PARAMETER_NOT_FOUND, formatter.str());
	}
	mappedParameterName = this->parameterNameMapping.find(parameterName)->second;
	return Result();
}

std::map<std::pair<std::uint32_t, std::uint32_t>, std::shared_ptr<SubObject>>& Module::GetDisabledSubindices()
{
	return this->disabledSubindices;
}
