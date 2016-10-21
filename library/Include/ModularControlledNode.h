/************************************************************************
\file ModularControlledNode.h
\brief Implementation of the Class ModularControlledNode
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
#if !defined MODULAR_CONTROLLED_NODE_H
#define MODULAR_CONTROLLED_NODE_H

#include <iostream>
#include <cstdint>

#include "Result.h"
#include "ControlledNode.h"
#include "Module.h"
#include "Interface.h"
#include "Object.h"
#include "SubObject.h"


namespace IndustrialNetwork
{
	namespace POWERLINK
	{
		namespace Core
		{
			/**
			\brief Modular node related namespace
			\author rueckerc, Bernecker+Rainer Industrie Elektronik Ges.m.b.H.
			*/
			namespace ModularNode
			{
				/**
				\brief Represents a modular controlled node in the POWERLINK network.
				\author rueckerc, Bernecker+Rainer Industrie Elektronik Ges.m.b.H.
				*/
				class ModularControlledNode : public IndustrialNetwork::POWERLINK::Core::Node::ControlledNode
				{
					public:
						ModularControlledNode(std::uint8_t nodeID, const std::string& nodeName);
						virtual ~ModularControlledNode();

						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result AddInterface(const std::string& uniqueId, const std::string& type, ModuleAddressing moduleAddressing, std::uint32_t maxModules, bool unusedSlots, bool multipleModules);
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result AddModule(const std::string& interfaceId, const std::string& moduleId, const std::string& moduleType, ModuleAddressing addressing, std::uint32_t modulePosition, std::uint32_t moduleAddress, const std::string& moduleName, std::uint16_t minPosition, std::uint16_t maxPosition, std::uint16_t minAddress, std::uint16_t maxAddress, std::uint16_t maxCount);
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result GetModule(const std::string& interfaceId, const std::string& moduleId, std::uint32_t modulePosition, std::shared_ptr<Module>& moduleRet);
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result AddRange(const std::string& interfaceId, const std::string& name, std::uint32_t baseIndex, std::uint32_t maxIndex, std::uint32_t maxSubIndex, std::uint32_t sortStep, const SortMode& sortMode, const SortNumber& sortNumber, const IndustrialNetwork::POWERLINK::Core::ObjectDictionary::PDOMapping& pdoMapping);
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result ChangeModuleOrderOnInterface(const std::string& interfaceId, const std::string& moduleId, std::uint32_t oldPos, std::uint32_t newPos);
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result EnableModule(const std::string& interfaceId, const std::string& moduleId, std::uint32_t modulePosition, bool enable);
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result RemoveModule(const std::string& interfaceId, const std::string& moduleId, std::uint32_t position);
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result SetModuleAddress(const std::string& interfaceId, const std::string& moduleId, std::uint32_t modulePosition, std::uint32_t address);
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result AddObjectToModule(const std::string& interfaceId, std::uint32_t modulePosition, const std::string& moduleId, const std::string& rangeSelector, std::uint32_t& objectId, std::shared_ptr<IndustrialNetwork::POWERLINK::Core::ObjectDictionary::Object> obj, bool createNew = true);
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result AddSubObjectToModule(const std::string& interfaceId, std::uint32_t modulePosition, const std::string& moduleId, std::uint32_t& objectId, std::uint16_t& subObjectId, std::shared_ptr<IndustrialNetwork::POWERLINK::Core::ObjectDictionary::SubObject> subObj, bool createNew = true);
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result GetModuleObjectCurrentIndex(const std::string& interfaceId, const std::string& moduleId, std::uint32_t modulePosition, std::uint32_t originalObjectId, std::int32_t originalSubObjectId, std::uint32_t& objectId, std::int32_t& subObjectId);
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result GetParameterCurrentName(const std::string& interfaceId, const std::string& moduleId, std::uint32_t modulePosition, const std::string& originalParamName, std::string& parameterName);

					private:
						std::vector<std::shared_ptr<IndustrialNetwork::POWERLINK::Core::ModularNode::Interface>> interfaceList;

						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result RemoveObjectFromOd(std::uint32_t objectId);
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result RemoveSubObjectsFromOd(std::uint32_t objectId, std::uint32_t subObjectId);
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result UpdateControlledNodeOd();
				};
			}
		}
	}
}
#endif
