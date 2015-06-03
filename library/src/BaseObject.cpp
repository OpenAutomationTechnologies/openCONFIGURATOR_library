/************************************************************************
\file BaseIndex.cpp
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
#include "BaseObject.h"

using namespace std;
using namespace IndustrialNetwork::POWERLINK::Core::Utilities;
using namespace IndustrialNetwork::POWERLINK::Core::ErrorHandling;

namespace IndustrialNetwork
{
	namespace POWERLINK
	{
		namespace Core
		{
			namespace ObjectDictionary
			{
				BaseObject::BaseObject() : IBaseObject(),
					forceToCDC(false),
					highLimit(),
					lowLimit(),
					uniqueIdRef(),
					accessType(),
					objectType(),
					pdoMapping()
				{}

				BaseObject::BaseObject(uint32_t id,  PlkDataType type) : IBaseObject(id, type),
					forceToCDC(false),
					highLimit(),
					lowLimit(),
					uniqueIdRef(),
					accessType(),
					objectType(),
					pdoMapping()
				{}

				BaseObject::BaseObject(uint32_t id, PlkDataType type, AccessType accessType, ObjectType objectType, PDOMapping pdoMapping, string defaultValue, string actualValue, uint32_t highlimit, uint32_t lowLimit,  string uniqueIdRef, string name) : IBaseObject(id, type, name),
					forceToCDC(false),
					highLimit(highlimit),
					lowLimit(lowLimit),
					uniqueIdRef(uniqueIdRef),
					accessType(accessType),
					objectType(objectType),
					pdoMapping(pdoMapping)
				{
					SetTypedObjectValues(defaultValue, actualValue);
				}

				BaseObject::~BaseObject()
				{}

				bool BaseObject::operator== (const BaseObject& baseObject) const
				{
					return (baseObject.GetId() == this->GetId());
				}

				bool BaseObject::GetForceToCDC() const
				{
					return forceToCDC;
				}

				void BaseObject::SetForceToCDC(bool force)
				{
					this->forceToCDC = force;
				}

				boost::optional<uint32_t> BaseObject::GetHighLimit() const
				{
					return highLimit;
				}

				void BaseObject::SetHighLimit(uint32_t highLimit)
				{
					this->highLimit = highLimit;
				}

				boost::optional<uint32_t> BaseObject::GetLowLimit() const
				{
					return lowLimit;
				}

				void BaseObject::SetLowLimit(uint32_t lowLimit)
				{
					this->lowLimit = lowLimit;
				}

				boost::optional<string> BaseObject::GetUniqueIdRef() const
				{
					return uniqueIdRef;
				}

				void BaseObject::SetUniqueIdRef(boost::optional<string>& uniqueIdRef)
				{
					this->uniqueIdRef = uniqueIdRef;
				}

				AccessType BaseObject::GetAccessType() const
				{
					return accessType;
				}

				void BaseObject::SetAccessType(AccessType accessType)
				{
					this->accessType = accessType;
				}

				ObjectType BaseObject::GetObjectType() const
				{
					return objectType;
				}

				void BaseObject::SetObjectType(ObjectType objectType)
				{
					this->objectType = objectType;
				}

				PDOMapping BaseObject::GetPDOMapping() const
				{
					return pdoMapping;
				}

				void BaseObject::SetPDOMapping(PDOMapping pdoMapping)
				{
					this->pdoMapping = pdoMapping;
				}

				template<typename T>
				T BaseObject::GetTypedActualValue()
				{
					if (this->GetActualValue().type() == typeid(T))
					{
						//return original stored value
						return boost::any_cast<T>(this->GetActualValue());
					}
					throw Result(ErrorCode::DATATYPE_MISMATCH);
				}

				template string BaseObject::GetTypedActualValue<string>();
				template bool BaseObject::GetTypedActualValue<bool>();
				template uint8_t BaseObject::GetTypedActualValue<uint8_t>();
				template uint16_t BaseObject::GetTypedActualValue<uint16_t>();
				template uint32_t BaseObject::GetTypedActualValue<uint32_t>();
				template uint64_t BaseObject::GetTypedActualValue<uint64_t>();
				template int8_t BaseObject::GetTypedActualValue<int8_t>();
				template int16_t BaseObject::GetTypedActualValue<int16_t>();
				template int32_t BaseObject::GetTypedActualValue<int32_t>();
				template int64_t BaseObject::GetTypedActualValue<int64_t>();
				template double BaseObject::GetTypedActualValue<double>();

				template<typename T>
				T BaseObject::GetTypedDefaultValue()
				{
					if (this->GetDefaultValue().type() == typeid(T))
					{
						return boost::any_cast<T>(this->GetActualValue());
					}
					throw Result(ErrorCode::DATATYPE_MISMATCH);
				}

				template string BaseObject::GetTypedDefaultValue<string>();
				template bool BaseObject::GetTypedDefaultValue<bool>();
				template uint8_t BaseObject::GetTypedDefaultValue<uint8_t>();
				template uint16_t BaseObject::GetTypedDefaultValue<uint16_t>();
				template uint32_t BaseObject::GetTypedDefaultValue<uint32_t>();
				template uint64_t BaseObject::GetTypedDefaultValue<uint64_t>();
				template int8_t BaseObject::GetTypedDefaultValue<int8_t>();
				template int16_t BaseObject::GetTypedDefaultValue<int16_t>();
				template int32_t BaseObject::GetTypedDefaultValue<int32_t>();
				template int64_t BaseObject::GetTypedDefaultValue<int64_t>();
				template double BaseObject::GetTypedDefaultValue<double>();

				void BaseObject::SetTypedObjectValues(string defaultValue, string actualValue)
				{
					if (!defaultValue.empty())
					{
						switch (this->GetDataType())
						{
							case PlkDataType::BOOLEAN:
								{
									bool value = StringToBool(defaultValue);
									this->SetDefaultValue(boost::any(value));
									break;
								}
							case PlkDataType::INTEGER8:
								{
									int8_t value = HexToInt<int8_t>(defaultValue);
									this->SetDefaultValue(boost::any(value));
									break;
								}
							case PlkDataType::INTEGER16:
								{
									int16_t value = HexToInt<int16_t>(defaultValue);
									this->SetDefaultValue(boost::any(value));
									break;
								}
							case PlkDataType::INTEGER32:
								{
									int32_t value = HexToInt<int32_t>(defaultValue);
									this->SetDefaultValue(boost::any(value));
									break;
								}
							case PlkDataType::UNSIGNED8:
								{
									uint8_t value = HexToInt<uint8_t>(defaultValue);
									this->SetDefaultValue(boost::any(value));
									break;
								}
							case PlkDataType::UNSIGNED16:
								{
									uint16_t value = HexToInt<uint16_t>(defaultValue);
									this->SetDefaultValue(boost::any(value));
									break;
								}
							case PlkDataType::UNSIGNED32:
								{
									uint32_t value = HexToInt<uint32_t>(defaultValue);
									this->SetDefaultValue(boost::any(value));
									break;
								}
							case PlkDataType::REAL32:
								{
									double value = boost::lexical_cast<double>(defaultValue);
									this->SetDefaultValue(boost::any(value));
									break;
								}
							case PlkDataType::VISIBLE_STRING:
								{
									this->SetDefaultValue(boost::any(defaultValue));
									break;
								}
							case PlkDataType::OCTET_STRING:
								{
									this->SetDefaultValue(boost::any(defaultValue));
									break;
								}
							case PlkDataType::UNICODE_STRING:
								{
									this->SetDefaultValue(boost::any(defaultValue));
									break;
								}
							case PlkDataType::TIME_OF_DAY:
								{
									this->SetDefaultValue(boost::any(defaultValue));
									break;
								}
							case PlkDataType::TIME_DIFF:
								{
									this->SetDefaultValue(boost::any(defaultValue));
									break;
								}
							case PlkDataType::Domain:
								//Both values must be empty if Domain datatype
								break;
							case PlkDataType::INTEGER24:
								{
									int32_t value = HexToInt<int32_t>(defaultValue);
									this->SetDefaultValue(boost::any(value));
									break;
								}
							case PlkDataType::REAL64:
								{
									double value = boost::lexical_cast<double>(defaultValue);
									this->SetDefaultValue(boost::any(value));
									break;
								}
							case PlkDataType::INTEGER40:
								{
									int64_t value = HexToInt<int64_t>(defaultValue);
									this->SetDefaultValue(boost::any(value));
									break;
								}
							case PlkDataType::INTEGER48:
								{
									int64_t value = HexToInt<int64_t>(defaultValue);
									this->SetDefaultValue(boost::any(value));
									break;
								}
							case PlkDataType::INTEGER56:
								{
									int64_t value = HexToInt<int64_t>(defaultValue);
									this->SetDefaultValue(boost::any(value));
									break;
								}
							case PlkDataType::INTEGER64:
								{
									int64_t value = HexToInt<int64_t>(defaultValue);
									this->SetDefaultValue(boost::any(value));
									break;
								}
							case PlkDataType::UNSIGNED24:
								{
									uint32_t value = HexToInt<uint32_t>(defaultValue);
									this->SetDefaultValue(boost::any(value));
									break;
								}
							case PlkDataType::UNSIGNED40:
								{
									uint64_t value = HexToInt<uint64_t>(defaultValue);
									this->SetDefaultValue(boost::any(value));
									break;
								}
							case PlkDataType::UNSIGNED48:
								{
									uint64_t value = HexToInt<uint64_t>(defaultValue);
									this->SetDefaultValue(boost::any(value));
									break;
								}
							case PlkDataType::UNSIGNED56:
								{
									uint64_t value = HexToInt<uint64_t>(defaultValue);
									this->SetDefaultValue(boost::any(value));
									break;
								}
							case PlkDataType::UNSIGNED64:
								{
									uint64_t value = HexToInt<uint64_t>(defaultValue);
									this->SetDefaultValue(boost::any(value));
									break;
								}
							case PlkDataType::MAC_ADDRESS:
								{
									this->SetDefaultValue(boost::any(defaultValue));
									break;
								}
							case PlkDataType::IP_ADDRESS:
								{
									this->SetDefaultValue(boost::any(defaultValue));
									break;
								}
							case PlkDataType::NETTIME:
								{
									this->SetDefaultValue(boost::any(defaultValue));
									break;
								}
							default:
								break;
						}
					}
					if (!actualValue.empty())
					{
						switch (this->GetDataType())
						{
							case PlkDataType::BOOLEAN:
								{
									bool value = StringToBool(actualValue);
									this->SetActualValue(boost::any(value));
									break;
								}
							case PlkDataType::INTEGER8:
								{
									int8_t value = HexToInt<int8_t>(actualValue);
									this->SetActualValue(boost::any(value));
									break;
								}
							case PlkDataType::INTEGER16:
								{
									int16_t value = HexToInt<int16_t>(actualValue);
									this->SetActualValue(boost::any(value));
									break;
								}
							case PlkDataType::INTEGER32:
								{
									int32_t value = HexToInt<int32_t>(actualValue);
									this->SetActualValue(boost::any(value));
									break;
								}
							case PlkDataType::UNSIGNED8:
								{
									uint8_t value = HexToInt<uint8_t>(actualValue);
									this->SetActualValue(boost::any(value));
									break;
								}
							case PlkDataType::UNSIGNED16:
								{
									uint16_t value = HexToInt<uint16_t>(actualValue);
									this->SetActualValue(boost::any(value));
									break;
								}
							case PlkDataType::UNSIGNED32:
								{
									uint32_t value = HexToInt<uint32_t>(actualValue);
									this->SetActualValue(boost::any(value));
									break;
								}
							case PlkDataType::REAL32:
								{
									double value = boost::lexical_cast<double>(actualValue);
									this->SetActualValue(boost::any(value));
									break;
								}
							case PlkDataType::VISIBLE_STRING:
								{
									this->SetActualValue(boost::any(actualValue));
									break;
								}
							case PlkDataType::OCTET_STRING:
								{
									this->SetActualValue(boost::any(defaultValue));
									break;
								}
							case PlkDataType::UNICODE_STRING:
								{
									this->SetActualValue(boost::any(actualValue));
									break;
								}
							case PlkDataType::TIME_OF_DAY:
								{
									this->SetActualValue(boost::any(actualValue));
									break;
								}
							case PlkDataType::TIME_DIFF:
								{
									this->SetActualValue(boost::any(actualValue));
									break;
								}
							case PlkDataType::Domain:
								//Both values must be empty if Domain datatype
								break;
							case PlkDataType::INTEGER24:
								{
									int32_t value = HexToInt<int32_t>(actualValue);
									this->SetActualValue(boost::any(value));
									break;
								}
							case PlkDataType::REAL64:
								{
									double value = boost::lexical_cast<double>(actualValue);
									this->SetActualValue(boost::any(value));
									break;
								}
							case PlkDataType::INTEGER40:
								{
									int64_t value = HexToInt<int64_t>(actualValue);
									this->SetActualValue(boost::any(value));
									break;
								}
							case PlkDataType::INTEGER48:
								{
									int64_t value = HexToInt<int64_t>(actualValue);
									this->SetActualValue(boost::any(value));
									break;
								}
							case PlkDataType::INTEGER56:
								{
									int64_t value = HexToInt<int64_t>(actualValue);
									this->SetActualValue(boost::any(value));
									break;
								}
							case PlkDataType::INTEGER64:
								{
									int64_t value = HexToInt<int64_t>(actualValue);
									this->SetActualValue(boost::any(value));
									break;
								}
							case PlkDataType::UNSIGNED24:
								{
									uint32_t value = HexToInt<uint32_t>(actualValue);
									this->SetActualValue(boost::any(value));
									break;
								}
							case PlkDataType::UNSIGNED40:
								{
									uint64_t value = HexToInt<uint64_t>(actualValue);
									this->SetActualValue(boost::any(value));
									break;
								}
							case PlkDataType::UNSIGNED48:
								{
									uint64_t value = HexToInt<uint64_t>(actualValue);
									this->SetActualValue(boost::any(value));
									break;
								}
							case PlkDataType::UNSIGNED56:
								{
									uint64_t value = HexToInt<uint64_t>(defaultValue);
									this->SetActualValue(boost::any(value));
									break;
								}
							case PlkDataType::UNSIGNED64:
								{
									uint64_t value = HexToInt<uint64_t>(actualValue);
									this->SetActualValue(boost::any(value));
									break;
								}
							case PlkDataType::MAC_ADDRESS:
								{
									this->SetActualValue(boost::any(actualValue));
									break;
								}
							case PlkDataType::IP_ADDRESS:
								{
									this->SetActualValue(boost::any(actualValue));
									break;
								}
							case PlkDataType::NETTIME:
								{
									this->SetActualValue(boost::any(actualValue));
									break;
								}
							default:
								break;
						}
					}
				}
			}
		}
	}
}