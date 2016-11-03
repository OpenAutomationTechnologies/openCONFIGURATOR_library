/************************************************************************
\file Module.h
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
#if !defined MODULE_H
#define MODULE_H

#include <cstdint>
#include <iostream>
#include <memory>
#include <map>

#include "BaseNode.h"
#include "BaseObject.h"
#include "SortEnums.h"
#include "NodeAssignment.h"
#include "Result.h"
#include "ModuleInterface.h"
#include "SubObject.h"

namespace IndustrialNetwork
{
	namespace POWERLINK
	{
		namespace Core
		{
			namespace ModularNode
			{
				/**
				\brief Represents a module in the POWERLINK network.
				\author rueckerc, Bernecker+Rainer Industrie Elektronik Ges.m.b.H.
				*/
				class Module : public IndustrialNetwork::POWERLINK::Core::Node::BaseNode
				{
					public:
						Module(std::uint8_t containingNode, const std::string& moduleId, const std::string& moduleType, const ModuleAddressing& addressing, std::uint32_t address, std::uint32_t position, const std::string& moduleName);
						Module(std::uint8_t containingNode, const std::string& moduleId, const std::string& moduleType, const ModuleAddressing& addressing, std::uint32_t address, std::uint32_t position, const std::string& moduleName, std::uint16_t minPosition, std::uint16_t maxPosition, std::uint16_t minAddress, std::uint16_t maxAddress, std::uint16_t maxCount);
						virtual ~Module();

						const std::string& GetModuleId() const;
						const std::string& GetModuleType() const;

						std::uint32_t GetAddress() const;
						void SetAddress(std::uint32_t address);

						std::uint32_t GetPosition() const;
						void SetPosition(std::uint32_t position);

						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result AddNodeAssignment(const IndustrialNetwork::POWERLINK::Core::Node::NodeAssignment& assign);
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result RemoveNodeAssignment(const IndustrialNetwork::POWERLINK::Core::Node::NodeAssignment& assign);
						std::uint32_t GetNodeAssignmentValue();

						std::uint32_t GetConfigurationObjectCount();
						std::uint32_t GetConfigurationObjectSize();

						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result CalculatePReqPayloadLimit();
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result CalculatePResPayloadLimit();

						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result CreateParamMapping(const std::string& paramName, const std::string& mappedParamName);
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result GetMappedParameterName(const std::string& parameterName, std::string& mappedParameterName);
						const std::map<std::string, std::string>& GetParameterNameMapping() const;

						std::map<std::pair<std::uint32_t, std::uint32_t>, std::shared_ptr<IndustrialNetwork::POWERLINK::Core::ObjectDictionary::SubObject>>& GetDisabledSubindices();
						std::shared_ptr<IndustrialNetwork::POWERLINK::Core::ModularNode::ModuleInterface>& GetModuleInterface();

					private:
						std::uint32_t position;
						std::uint32_t address;
						std::shared_ptr<IndustrialNetwork::POWERLINK::Core::ModularNode::ModuleInterface> moduleInterface;
						std::map<std::string, std::string> parameterNameMapping;
						std::map<std::pair<std::uint32_t, std::uint32_t>, std::shared_ptr<IndustrialNetwork::POWERLINK::Core::ObjectDictionary::SubObject>> disabledSubindices;
				};
			}
		}
	}
}
#endif
