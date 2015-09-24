/************************************************************************
\file Interface.h
\brief Implementation of the Class Interface
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
#if !defined INTERFACE_H
#define INTERFACE_H

#include <vector>
#include <map>
#include <string>
#include <memory>
#include <cstdint>

#include "SortEnums.h"
#include "Module.h"
#include "Range.h"
#include "ErrorCode.h"
#include "IBaseInterface.h"

namespace IndustrialNetwork
{
	namespace POWERLINK
	{
		namespace Core
		{
			namespace ModularNode
			{
				/**
				\brief Represents an interface the modular head node.
				\author rueckerc, Bernecker+Rainer Industrie Elektronik Ges.m.b.H.
				*/
				class Interface : public IBaseInterface
				{

					public:
						Interface(const std::string& uniqueId, const std::string& type, ModuleAddressing moduleAddressing, std::uint32_t maxModules, bool unusedSlots, bool multipleModules);
						virtual ~Interface();

						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result AddRange(const std::shared_ptr<IndustrialNetwork::POWERLINK::Core::ModularNode::Range>& range);
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result AddModule(std::uint32_t position, const std::shared_ptr<IndustrialNetwork::POWERLINK::Core::ModularNode::Module>& module);
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result RemoveModule(const std::string& moduleId, std::uint32_t modulePosition);

						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result GetModule(const std::string& moduleId, std::uint32_t modulePosition, std::shared_ptr<Module>& module);
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result ChangeModulePosition(const std::string& moduleId, std::uint32_t oldPos, std::uint32_t newPos);
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result EnableModule(const std::string& moduleId, std::uint32_t modulePosition, bool enable);
						std::uint32_t GetModuleCount() const;

						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result GetRange(const std::string& name, std::shared_ptr<Range>& retRange);
						const std::map<std::uint32_t, std::shared_ptr<IndustrialNetwork::POWERLINK::Core::ModularNode::Module>>& GetModuleCollection();
						const std::vector<std::shared_ptr<IndustrialNetwork::POWERLINK::Core::ModularNode::Range>>& GetRangeList();

						bool GetUnusedSlots() const;
						bool GetMultipleModules() const;
						std::uint32_t GetMaxModules() const;
						bool IsPositionOccupied(std::uint32_t pos) const;

					private:
						bool unusedSlots;
						bool multipleModules;
						std::uint32_t maxModules;

						std::vector<std::shared_ptr<IndustrialNetwork::POWERLINK::Core::ModularNode::Range>> rangeList;
						std::map<std::uint32_t, std::shared_ptr<IndustrialNetwork::POWERLINK::Core::ModularNode::Module>> moduleCollection;

				};
			}
		}
	}
}
#endif
