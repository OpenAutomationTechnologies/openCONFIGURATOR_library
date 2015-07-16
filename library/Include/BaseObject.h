/************************************************************************
\file BaseIndex.h
\brief Implementation of the Class BaseIndex
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
#if !defined BASE_OBJECT_H
#define BASE_OBJECT_H

#include <cstdint>
#include <vector>
#include <string>
#include <sstream>

#include <boost/optional.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>

#include "Constants.h"
#include "IBaseObject.h"
#include "ObjectType.h"
#include "PDOMapping.h"
#include "AccessType.h"
#include "PlkDataType.h"
#include "Utilities.h"
#include "Result.h"
#include "Parameter.h"
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
				\brief Represents the common basis for all POWERLINK objects.
				\author rueckerc, Bernecker+Rainer Industrie Elektronik Ges.m.b.H.
				*/
				class DLLEXPORT BaseObject : public IndustrialNetwork::Fieldbus::IBaseObject<std::uint32_t, ObjectType>
				{

					public:
						BaseObject();
						BaseObject(std::uint32_t id, ObjectType objectType, std::string name, std::uint8_t containingNode);
						BaseObject(std::uint32_t id, ObjectType objectType, std::string name, std::uint8_t containingNode, PlkDataType dataType, AccessType accessType, PDOMapping pdoMapping);
						BaseObject(std::uint32_t id, ObjectType objectType, std::string name, std::uint8_t containingNode, std::string uniqueIdRef);

						bool operator== (const BaseObject& BaseObject) const;
						virtual ~BaseObject();

						bool GetForceToCDC() const;
						void SetForceToCDC(bool force);

						bool WriteToConfiguration() const;

						const boost::optional<std::int64_t>& GetHighLimit() const;
						void SetHighLimit(std::int64_t highLimit);

						const boost::optional<std::int64_t>& GetLowLimit() const;
						void SetLowLimit(std::int64_t lowLimit);

						const boost::optional<std::string>& GetUniqueIdRef() const;
						void SetUniqueIdRef(const std::string& uniqueIdRef);

						const boost::optional<IndustrialNetwork::POWERLINK::Core::ObjectDictionary::AccessType>& GetAccessType() const;
						void SetAccessType(IndustrialNetwork::POWERLINK::Core::ObjectDictionary::AccessType accessType);

						const boost::optional<IndustrialNetwork::POWERLINK::Core::ObjectDictionary::PlkDataType>& GetDataType() const;
						void SetDataType(IndustrialNetwork::POWERLINK::Core::ObjectDictionary::PlkDataType dataType);

						const boost::optional<IndustrialNetwork::POWERLINK::Core::ObjectDictionary::PDOMapping>& GetPDOMapping() const;
						void SetPDOMapping(IndustrialNetwork::POWERLINK::Core::ObjectDictionary::PDOMapping pdoMapping);

						template<typename T>
						T GetTypedActualValue();

						template<typename T>
						T GetTypedDefaultValue();

						std::uint8_t GetContainingNode();
						void SetTypedObjectActualValue(const std::string& actualValueToSet);
						void SetTypedObjectDefaultValue(const std::string& defaultValueToSet);

						void SetComplexDataType(std::shared_ptr<Parameter>& parameter);

						std::uint32_t GetBitSize();

					private:

						bool forceToCDC;
						boost::optional<std::int64_t> highLimit;
						boost::optional<std::int64_t> lowLimit;
						boost::optional<std::string> uniqueIdRef;
						std::shared_ptr<Parameter> complexDataType;
						boost::optional<IndustrialNetwork::POWERLINK::Core::ObjectDictionary::AccessType> accessType;
						boost::optional<IndustrialNetwork::POWERLINK::Core::ObjectDictionary::PlkDataType> dataType;
						boost::optional<IndustrialNetwork::POWERLINK::Core::ObjectDictionary::PDOMapping> pdoMapping;
						std::uint8_t containingNode;
						bool actualValueNotDefaultValue;
				};

			}

		}

	}

}
#endif
