/************************************************************************
\file Range.h
\brief Implementation of the Class Range
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
#if !defined MODULE_RANGE_H
#define MODULE_RANGE_H

#include <string>
#include <cstdint>
#include <map>
#include <set>

#include "SortEnums.h"
#include "PDOMapping.h"
#include "Result.h"
#include "LoggingConfiguration.h"

namespace IndustrialNetwork
{
	namespace POWERLINK
	{
		namespace Core
		{
			namespace ModularNode
			{
				/**
				\brief Represents a range in the modular controlled node.
				\author rueckerc, Bernecker+Rainer Industrie Elektronik Ges.m.b.H.
				*/
				class Range
				{

					public:
						Range(const std::string& name, std::uint32_t baseIndex, std::uint32_t maxIndex, std::uint32_t maxSubIndex, std::uint32_t sortStep = 1, const SortMode& sortMode = SortMode::INDEX, const SortNumber& sortNumber = SortNumber::CONTINUOUS, const IndustrialNetwork::POWERLINK::Core::ObjectDictionary::PDOMapping& pdoMapping = IndustrialNetwork::POWERLINK::Core::ObjectDictionary::PDOMapping::UNDEFINED);
						virtual ~Range();

						const std::string& GetName() const;
						std::uint32_t GetBaseIndex() const;
						std::uint32_t GetMaxIndex() const;
						std::uint32_t GetMaxSubIndex() const;
						const SortMode& GetSortMode() const;
						const SortNumber& GetSortNumber() const;
						const IndustrialNetwork::POWERLINK::Core::ObjectDictionary::PDOMapping& GetPdoMpapping() const;

						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result GetNextIndex(std::uint32_t& index, std::uint32_t address);
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result GetNextSubIndex(std::uint32_t& index, std::uint16_t& subindex, std::uint32_t address);
						void Reset();

					private:
						std::string name;
						std::uint32_t baseIndex;
						std::uint32_t maxIndex;
						std::uint32_t maxSubIndex;
						std::uint32_t sortStep;
						SortMode sortMode;
						SortNumber sortNumber;
						IndustrialNetwork::POWERLINK::Core::ObjectDictionary::PDOMapping pdoMapping;

						std::uint32_t currentIndex;
						std::uint32_t currentSubIndex;
						std::map<std::uint32_t, std::int64_t> takenIndices;
						std::set<std::uint32_t> takenAddresses;
				};
			}
		}
	}
}
#endif
