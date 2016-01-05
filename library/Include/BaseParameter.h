/************************************************************************
\file BaseParameter.h
\brief Implementation of the Class BaseParameter
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
#if !defined BASE_PARAMETER_H
#define BASE_PARAMETER_H

#include <memory>
#include <string>

#include <boost/optional.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>
#include <boost/any.hpp>
#include <boost/dynamic_bitset.hpp>

#include "ComplexDataType.h"
#include "PlkDataType.h"
#include "ParameterAccess.h"
#include "IEC_Datatype.h"
#include "Utilities.h"
#include "Result.h"
#include "LoggingConfiguration.h"

namespace IndustrialNetwork
{
	namespace POWERLINK
	{
		namespace Core
		{
			namespace ObjectDictionary
			{
				/**
				\brief Represents a base parameter.
				\author rueckerc, Bernecker+Rainer Industrie Elektronik Ges.m.b.H.
				*/
				class BaseParameter
				{

					public:
						BaseParameter(const std::string& uniqueID, IndustrialNetwork::POWERLINK::Core::ObjectDictionary::ParameterAccess parameterAccess, const std::string& dataTypeUniqueIDRef = "");
						BaseParameter(const std::string& uniqueID, IndustrialNetwork::POWERLINK::Core::ObjectDictionary::ParameterAccess parameterAccess, IndustrialNetwork::POWERLINK::Core::ObjectDictionary::IEC_Datatype datatype);
						explicit BaseParameter(const std::string& uniqueID);
						virtual ~BaseParameter();

						const std::string& GetUniqueID() const;

						const std::string& GetDataTypeUniqueIDRef() const;
						void SetDataTypeUniqueIDRef(const std::string& dataTypeUniqueIDRef);

						const std::shared_ptr<ComplexDataType>& GetComplexDataType() const;
						void SetComplexDataType(const std::shared_ptr<ComplexDataType>& complexType);

						IndustrialNetwork::POWERLINK::Core::ObjectDictionary::ParameterAccess GetParameterAccess() const;
						void SetParameterAccess(IndustrialNetwork::POWERLINK::Core::ObjectDictionary::ParameterAccess access);
						std::uint32_t GetBitSize() const;

						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result AddParameterAllowedRange(const std::string& minValue, const std::string& maxValue);
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result AddParameterAllowedValue(const std::string& allowedValue);

						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result AddParameterDefaultValue(const std::string& defaultValue);
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result SetParameterActualValue(const std::string& actualValue);

						const boost::dynamic_bitset<>& GetParameterActualValueBitSet(const std::string& tempActualValue = "");

						template<typename T>
						T GetTypedParameterActualValue();

						template<typename T>
						T GetTypedParameterDefaultValue();

						template<typename T>
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result ValidateParameterValue(T valueToValidate);

						const boost::optional<boost::any>& GetParameterDefaultValue() const;
						const boost::optional<boost::any>& GetParameterActualValue() const;

						const boost::optional<IndustrialNetwork::POWERLINK::Core::ObjectDictionary::IEC_Datatype>& GetDataType() const;
						void SetDataType(IndustrialNetwork::POWERLINK::Core::ObjectDictionary::IEC_Datatype dataType);

						bool HasActualValue() const;
						bool HasDefaultValue() const;

						bool WriteToConfiguration() const;

						void ClearActualValue();

					private:

						enum class ValueType : std::uint8_t
						{
							ACTUAL = 0,
							DEFAULT = 1,
							ALLOWED
						};

						std::string uniqueID;
						std::string dataTypeUniqueIDRef;
						std::shared_ptr<ComplexDataType> complexDataType;

						boost::optional<boost::any> defaultValue;
						boost::optional<boost::any> actualValue;

						std::vector<std::pair<boost::any, boost::any>> allowedRanges;
						std::vector<boost::any> allowedValues;

						IndustrialNetwork::POWERLINK::Core::ObjectDictionary::ParameterAccess parameterAccess;
						boost::optional<IndustrialNetwork::POWERLINK::Core::ObjectDictionary::IEC_Datatype> dataType;
						boost::dynamic_bitset<> actualValueBitset;
						bool actualValueNotDefaultValue;

						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result AddParameterValue(const std::string& value, ValueType type);
				};
			}
		}
	}
}
#endif
