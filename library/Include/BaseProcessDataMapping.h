/************************************************************************
\file BaseProcessDataMapping.h
\brief Implementation of the Class BaseProcessDataMapping
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
#if !defined BASEPROCESS_DATA_MAPPING_H
#define BASEPROCESS_DATA_MAPPING_H

#include <cstdint>
#include <string>
#include <vector>
#include <memory>

#include "Constants.h"
#include "Utilities.h"
#include "BaseObject.h"

namespace IndustrialNetwork
{
	namespace POWERLINK
	{
		namespace Core
		{
			namespace ObjectDictionary
			{
				/**
				\brief Represents the a process data mapping of a POWERLINK node.
				\author rueckerc, Bernecker+Rainer Industrie Elektronik Ges.m.b.H.
				*/
				class DLLEXPORT BaseProcessDataMapping
				{
					public:
						BaseProcessDataMapping(std::uint32_t object, std::uint32_t subobject, const std::shared_ptr<IndustrialNetwork::POWERLINK::Core::ObjectDictionary::BaseObject>& obj, std::uint32_t index, std::uint16_t subindex, std::uint16_t offset, std::uint16_t length, std::uint16_t nodeId, bool staticMapping = false);
						BaseProcessDataMapping(std::uint32_t object, std::uint32_t subobject, const std::shared_ptr<IndustrialNetwork::POWERLINK::Core::ObjectDictionary::BaseObject>& obj, std::uint64_t actualValue, std::uint16_t nodeId, bool staticMapping = false);
						BaseProcessDataMapping(std::uint32_t object, std::uint32_t subobject, const std::shared_ptr<IndustrialNetwork::POWERLINK::Core::ObjectDictionary::BaseObject>& obj, const std::string& actualValue, std::uint16_t nodeId, bool staticMapping = false);
						BaseProcessDataMapping(const std::string& actualValue, std::uint16_t nodeId, bool staticMapping = false);

						virtual ~BaseProcessDataMapping();

						void InitMappingObject(std::uint64_t actualValue);
						void InitMappingObject(const std::string& actualValue);

						std::uint32_t GetOffsetGap() const;
						void SetOffsetGap(std::uint32_t offsetGap);

						std::uint16_t GetDestinationNode() const;
						void SetDestinationNode(std::uint16_t nodeId);

						std::uint32_t GetMappingIndex() const;
						void SetMappingIndex(std::uint32_t index);

						std::uint16_t GetMappingSubIndex() const;
						void SetMappinSubgIndex(std::uint16_t subindex);

						std::uint32_t GetMappingOffset() const;
						void SetMappingOffset(std::uint32_t offset);

						std::uint32_t GetMappingLength() const;
						void SetMappingLength(std::uint32_t length);

						const std::string ToString(bool addPrefix);

						std::uint64_t GetValue();

						std::uint32_t GetObject() const;
						std::uint32_t GetSubObject() const;

						const std::shared_ptr<IndustrialNetwork::POWERLINK::Core::ObjectDictionary::BaseObject>& GetRelatedObject() const;
						void SetRelatedObject(std::shared_ptr<IndustrialNetwork::POWERLINK::Core::ObjectDictionary::BaseObject>& object);

					private:
						std::uint32_t object;
						std::uint32_t subobject;
						std::shared_ptr<IndustrialNetwork::POWERLINK::Core::ObjectDictionary::BaseObject> relatedObject;

						std::uint16_t destinationNode;
						std::uint32_t index;
						std::uint16_t subindex;
						std::uint32_t offset;
						std::uint32_t length;

						bool staticMapping;
						std::uint32_t offsetGap;

				};
			}
		}
	}
}
#endif
