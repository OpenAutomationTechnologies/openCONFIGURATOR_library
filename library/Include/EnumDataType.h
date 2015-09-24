/************************************************************************
\file EnumDataType.h
\brief Implementation of the Class EnumDataType
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
#if !defined ENUM_DATA_TYPE_H
#define ENUM_DATA_TYPE_H

#include <unordered_map>
#include <utility>

#include "ComplexDataType.h"
#include "IEC_Datatype.h"
#include "Result.h"
#include "Utilities.h"

namespace IndustrialNetwork
{
	namespace POWERLINK
	{
		namespace Core
		{
			namespace ObjectDictionary
			{
				/**
				\brief Represents a complex enum datatype of a POWERLINK node.
				\author rueckerc, Bernecker+Rainer Industrie Elektronik Ges.m.b.H.
				*/
				class EnumDataType : public IndustrialNetwork::POWERLINK::Core::ObjectDictionary::ComplexDataType
				{
					public:
						EnumDataType(const std::string& uniqueID, const std::string& name, IndustrialNetwork::POWERLINK::Core::ObjectDictionary::IEC_Datatype datatType, std::uint32_t size = 0);
						virtual ~EnumDataType();

						const IndustrialNetwork::POWERLINK::Core::ObjectDictionary::IEC_Datatype& GetDataType() const;
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result AddEnumValue(const std::string& name, const std::string& value);

						virtual std::uint32_t GetBitSize() const;

					private:
						IndustrialNetwork::POWERLINK::Core::ObjectDictionary::IEC_Datatype dataType;
						std::unordered_map<std::string, std::string> enumValues;
				};
			}
		}
	}
}
#endif
