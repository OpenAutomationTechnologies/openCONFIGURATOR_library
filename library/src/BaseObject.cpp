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

using namespace IndustrialNetwork::POWERLINK::Core::Utilities;
using namespace IndustrialNetwork::POWERLINK::Core::ErrorHandling;
using namespace IndustrialNetwork::POWERLINK::Core::CoreConfiguration;

namespace IndustrialNetwork
{
	namespace POWERLINK
	{
		namespace Core
		{
			namespace ObjectDictionary
			{
				BaseObject::BaseObject(std::uint32_t id, ObjectType objectType, const std::string& name, std::uint8_t containingNode) : IBaseObject(id, objectType, name),
					forceToCDC(false),
					highLimit(boost::optional<boost::any>()),
					lowLimit(boost::optional<boost::any>()),
					uniqueIdRef(boost::optional<std::string>()),
					referencedParameter(std::shared_ptr<Parameter>()),
					referencedParameterGrp(std::shared_ptr<ParameterGroup>()),
					accessType(boost::optional<AccessType>()),
					dataType(boost::optional<PlkDataType>()),
					pdoMapping(boost::optional<PDOMapping>()),
					containingNode(containingNode),
					actualValueNotDefaultValue(false)
				{}

				BaseObject::BaseObject(std::uint32_t id, ObjectType objectType, const std::string& name, std::uint8_t containingNode, PlkDataType dataType, AccessType accessType, PDOMapping pdoMapping) : IBaseObject(id, objectType, name),
					forceToCDC(false),
					highLimit(boost::optional<boost::any>()),
					lowLimit(boost::optional<boost::any>()),
					uniqueIdRef(boost::optional<std::string>()),
					referencedParameter(std::shared_ptr<Parameter>()),
					referencedParameterGrp(std::shared_ptr<ParameterGroup>()),
					accessType(accessType),
					dataType(dataType),
					pdoMapping(pdoMapping),
					containingNode(containingNode),
					actualValueNotDefaultValue(false)
				{}

				BaseObject::BaseObject(std::uint32_t id, ObjectType objectType, const std::string& name, std::uint8_t containingNode, const std::string& uniqueIdRef) : IBaseObject(id, objectType, name),
					forceToCDC(false),
					highLimit(boost::optional<boost::any>()),
					lowLimit(boost::optional<boost::any>()),
					uniqueIdRef(uniqueIdRef),
					referencedParameter(std::shared_ptr<Parameter>()),
					referencedParameterGrp(std::shared_ptr<ParameterGroup>()),
					accessType(boost::optional<AccessType>()),
					dataType(PlkDataType::Domain),
					pdoMapping(boost::optional<PDOMapping>()),
					containingNode(containingNode),
					actualValueNotDefaultValue(false)
				{}

				BaseObject::~BaseObject()
				{}

				bool BaseObject::operator== (const BaseObject& baseObject) const
				{
					return (baseObject.GetObjectId() == this->GetObjectId());
				}

				bool BaseObject::GetForceToCDC() const
				{
					return forceToCDC;
				}

				void BaseObject::SetForceToCDC(bool force)
				{
					this->forceToCDC = force;
				}

				const boost::optional<boost::any>& BaseObject::GetHighLimit() const
				{
					return this->highLimit;
				}

				const boost::optional<std::string>& BaseObject::GetUniqueIdRef() const
				{
					return uniqueIdRef;
				}

				void BaseObject::SetUniqueIdRef(const std::string& uniqueIdRef)
				{
					this->uniqueIdRef = uniqueIdRef;
				}

				const boost::optional<AccessType>& BaseObject::GetAccessType() const
				{
					return accessType;
				}

				const boost::optional<PlkDataType>& BaseObject::GetDataType() const
				{
					return dataType;
				}

				const boost::optional<PDOMapping>& BaseObject::GetPDOMapping() const
				{
					return pdoMapping;
				}

				void BaseObject::SetAccessType(AccessType accessType)
				{
					this->accessType = accessType;
				}

				void BaseObject::SetDataType(PlkDataType dataType)
				{
					this->dataType = dataType;
				}

				void BaseObject::SetPDOMapping(PDOMapping pdoMapping)
				{
					this->pdoMapping = pdoMapping;
				}

				const boost::optional<boost::any>& BaseObject::GetLowLimit() const
				{
					return this->lowLimit;
				}

				const std::shared_ptr<Parameter>& BaseObject::GetReferencedParameter()
				{
					return this->referencedParameter;
				}

				const std::shared_ptr<ParameterGroup>& BaseObject::GetReferencedParameterGroup()
				{
					return this->referencedParameterGrp;
				}

				template<typename T>
				T BaseObject::GetTypedValue(ValueType type)
				{
					if (!this->GetDataType().is_initialized())
					{
						// Object has not datatype defined
						boost::format formatter(kMsgBaseObjectDataTypeError);
						formatter
						% this->GetName()
						% this->GetObjectId()
						% (std::uint32_t) this->GetContainingNode();
						LOG_FATAL() << formatter.str();
						throw Result(ErrorCode::OBJECT_HAS_NO_DATATYPE, formatter.str());
					}
					std::string dataTypeName;

					//Refresh referenced actual value
					if (this->uniqueIdRef.is_initialized() && type == ValueType::ACTUAL)
					{
						if (this->referencedParameter)
						{
							if (this->referencedParameter->HasActualValue())
							{
								this->SetTypedObjectActualValue("0x" + this->referencedParameter->GetTypedParameterActualValue<std::string>());
							}
						}
						else if (this->referencedParameterGrp)
						{
							std::string actualValueStr = "";
							auto bitSet = this->referencedParameterGrp->GetActualValueBitSet(GetBitSize());
							//LOG_FATAL() << bitSet;
							for (boost::dynamic_bitset<>::size_type bit = 0; bit <= bitSet.size() - 4; bit += 4)
							{
								std::stringstream actualValue;
								boost::dynamic_bitset<> hexDigit(4);
								hexDigit[0] = bitSet[bit];
								hexDigit[1] = bitSet[bit + 1];
								hexDigit[2] = bitSet[bit + 2];
								hexDigit[3] = bitSet[bit + 3];
								actualValue << std::hex << hexDigit.to_ulong();
								actualValueStr.insert(0, actualValue.str());
							}
							this->SetTypedObjectActualValue("0x" + actualValueStr);
						}
					}

					switch (type)
					{
						case ValueType::ACTUAL:
							{
								if (!this->GetActualValue().empty())
								{
									if (this->GetActualValue().type() == typeid(T))
									{
										//return original stored value
										return boost::any_cast<T>(this->GetActualValue());
									}
									dataTypeName = this->GetActualValue().type().name();
								}
								//No actual value present
								boost::format formatter(kMsgBaseObjectActualValue);
								formatter
								% this->GetName()
								% this->GetObjectId()
								% (std::uint32_t) this->GetContainingNode();
								LOG_FATAL() << formatter.str();
								throw Result(ErrorCode::OBJECT_HAS_NO_ACTUAL_VALUE, formatter.str());
							}
						case ValueType::DEFAULT:
							{
								if (!this->GetDefaultValue().empty())
								{
									if (this->GetDefaultValue().type() == typeid(T))
									{
										return boost::any_cast<T>(this->GetDefaultValue());
									}
									dataTypeName = this->GetDefaultValue().type().name();
									break;
								}
								//No actual value present
								boost::format formatter(kMsgBaseObjectDefaultValue);
								formatter
								% this->GetName()
								% this->GetObjectId()
								% (std::uint32_t) this->GetContainingNode();
								LOG_FATAL() << formatter.str();
								throw Result(ErrorCode::OBJECT_HAS_NO_DEFAULT_VALUE, formatter.str());
							}
						case ValueType::LOWLIMIT:
							{
								if (this->GetLowLimit().is_initialized())
								{
									if (this->GetLowLimit().get().type() == typeid(T))
									{
										return boost::any_cast<T>(this->GetLowLimit().get());
									}
									dataTypeName = this->GetLowLimit().get().type().name();
									break;
								}
								boost::format formatter(kMsgBaseObjectLowLimit);
								formatter
								% this->GetName()
								% this->GetObjectId()
								% (std::uint32_t) this->GetContainingNode();
								LOG_FATAL() << formatter.str();
								throw Result(ErrorCode::OBJECT_LIMITS_INVALID, formatter.str());
							}

						case ValueType::HIGHLIMIT:
							{
								if (this->GetHighLimit().is_initialized())
								{
									if (this->GetHighLimit().get().type() == typeid(T))
									{
										return boost::any_cast<T>(this->GetHighLimit().get());
									}
									dataTypeName = this->GetHighLimit().get().type().name();
									break;
								}
								boost::format formatter(kMsgBaseObjectHighLimit);
								formatter
								% this->GetName()
								% this->GetObjectId()
								% (std::uint32_t) this->GetContainingNode();
								LOG_FATAL() << formatter.str();
								throw Result(ErrorCode::OBJECT_LIMITS_INVALID, formatter.str());
							}
						default:
							break;
					}
					//Datatype does not match
					boost::format formatter(kMsgObjectDatatypeMismatch);
					formatter
					% this->GetName()
					% typeid(T).name()
					% dataTypeName;
					LOG_FATAL() << formatter.str();
					throw Result(ErrorCode::DATATYPE_MISMATCH, formatter.str());
				}

				template<> std::string BaseObject::GetTypedValue<std::string>(ValueType type)
				{
					boost::any value;
					if (this->uniqueIdRef.is_initialized() && type == ValueType::ACTUAL)
					{
						//Refresh referenced actual value
						if (this->referencedParameter)
						{
							if (this->referencedParameter->HasActualValue())
							{
								this->SetTypedObjectActualValue("0x" + this->referencedParameter->GetTypedParameterActualValue<std::string>());
							}
						}
						else if (this->referencedParameterGrp)
						{
							std::string actualValueStr = "";
							auto bitSet = this->referencedParameterGrp->GetActualValueBitSet(GetBitSize());
							//LOG_FATAL() << bitSet;
							for (boost::dynamic_bitset<>::size_type bit = 0; bit <= bitSet.size() - 4; bit += 4)
							{
								std::stringstream actualValue;
								boost::dynamic_bitset<> hexDigit(4);
								hexDigit[0] = bitSet[bit];
								hexDigit[1] = bitSet[bit + 1];
								hexDigit[2] = bitSet[bit + 2];
								hexDigit[3] = bitSet[bit + 3];
								actualValue << std::hex << hexDigit.to_ulong();
								actualValueStr.insert(0, actualValue.str());
							}
							this->SetTypedObjectActualValue("0x" + actualValueStr);
						}
						value = this->GetActualValue();
					}
					else if (type == ValueType::ACTUAL)
					{
						if (this->GetActualValue().empty())
						{
							//No actual value present
							boost::format formatter(kMsgBaseObjectActualValue);
							formatter
							% this->GetName()
							% this->GetObjectId()
							% (std::uint32_t) this->GetContainingNode();
							LOG_FATAL() << formatter.str();
							throw Result(ErrorCode::OBJECT_HAS_NO_ACTUAL_VALUE, formatter.str());
						}
						value = this->GetActualValue();
					}
					else if (type == ValueType::DEFAULT)
					{
						if (this->GetDefaultValue().empty())
						{
							//No actual value present
							boost::format formatter(kMsgBaseObjectDefaultValue);
							formatter
							% this->GetName()
							% this->GetObjectId()
							% (std::uint32_t) this->GetContainingNode();
							LOG_FATAL() << formatter.str();
							throw Result(ErrorCode::OBJECT_HAS_NO_DEFAULT_VALUE, formatter.str());
						}
						value = this->GetDefaultValue();
					}
					else
						return "";

					std::stringstream convertString;
					switch (this->GetDataType().get())
					{
						case PlkDataType::BOOLEAN:
							{
								bool res = boost::any_cast<bool>(value);
								return (res) ? "01" : "00";
							}
						case PlkDataType::INTEGER8:
							{
								convertString << std::uppercase << std::setfill('0') << std::setw(2) << std::hex << boost::any_cast<std::int16_t>(value);
								return convertString.str();
							}
						case PlkDataType::INTEGER16:
							{
								convertString << std::uppercase << std::setfill('0') << std::setw(4) << std::hex << boost::any_cast<std::int16_t>(value);
								return convertString.str();
							}
						case PlkDataType::INTEGER24:
							{
								convertString << std::uppercase << std::setfill('0') << std::setw(6) << std::hex << boost::any_cast<std::int32_t>(value);
								return convertString.str();
							}
						case PlkDataType::INTEGER32:
							{
								convertString << std::uppercase << std::setfill('0') << std::setw(8) << std::hex << boost::any_cast<std::int32_t>(value);
								return convertString.str();
							}
						case PlkDataType::UNSIGNED8:
							{
								convertString << std::uppercase << std::setfill('0') << std::setw(2) << std::hex << boost::any_cast<std::uint16_t>(value);
								return convertString.str();
							}
						case PlkDataType::UNSIGNED16:
							{
								convertString << std::uppercase << std::setfill('0') << std::setw(4) << std::hex << boost::any_cast<std::uint16_t>(value);
								return convertString.str();
							}
						case PlkDataType::UNSIGNED24:
							{
								convertString << std::uppercase << std::setfill('0') << std::setw(6) << std::hex << boost::any_cast<std::uint32_t>(value);
								return convertString.str();
							}
						case PlkDataType::UNSIGNED32:
							{
								convertString << std::uppercase << std::setfill('0') << std::setw(8) << std::hex << boost::any_cast<std::uint32_t>(value);
								return convertString.str();
							}
						case PlkDataType::REAL32:
							{
								convertString << std::hex << std::uppercase << std::setfill('0') << std::setw(8) << FloatToSinglePrecisisionHex(boost::any_cast<float>(value));
								return convertString.str();
							}
						case PlkDataType::REAL64:
							{
								convertString << std::hex << std::uppercase << std::setfill('0') << std::setw(16) << DoubleToDoublePrecisisionHex(boost::any_cast<double>(value));
								return convertString.str();
							}
						case PlkDataType::Domain:
							break;
						case PlkDataType::INTEGER40:
							{
								convertString << std::uppercase << std::setfill('0') << std::setw(10) << std::hex << boost::any_cast<std::int64_t>(value);
								return convertString.str();
							}
						case PlkDataType::INTEGER48:
							{
								convertString << std::uppercase << std::setfill('0') << std::setw(12) << std::hex << boost::any_cast<std::int64_t>(value);
								return convertString.str();
							}
						case PlkDataType::INTEGER56:
							{
								convertString << std::uppercase << std::setfill('0') << std::setw(14) << std::hex << boost::any_cast<std::int64_t>(value);
								return convertString.str();
							}
						case PlkDataType::INTEGER64:
							{
								convertString << std::uppercase << std::setfill('0') << std::setw(16) << std::hex << boost::any_cast<std::int64_t>(value);
								return convertString.str();
							}
						case PlkDataType::UNSIGNED40:
							{
								convertString << std::uppercase << std::setfill('0') << std::setw(10) << std::hex << boost::any_cast<std::uint64_t>(value);
								return convertString.str();
							}
						case PlkDataType::MAC_ADDRESS:
						case PlkDataType::UNSIGNED48:
							{
								convertString << std::uppercase << std::setfill('0') << std::setw(12) << std::hex << boost::any_cast<std::uint64_t>(value);
								return convertString.str();
							}
						case PlkDataType::UNSIGNED56:
							{
								convertString << std::uppercase << std::setfill('0') << std::setw(14) << std::hex << boost::any_cast<std::uint64_t>(value);
								return convertString.str();
							}
						case PlkDataType::UNSIGNED64:
							{
								convertString << std::uppercase << std::setfill('0') << std::setw(16) << std::hex << boost::any_cast<std::uint64_t>(value);
								return convertString.str();
							}
						case PlkDataType::VISIBLE_STRING:
						case PlkDataType::OCTET_STRING:
						case PlkDataType::UNICODE_STRING:
							{
								std::string actualValue = boost::any_cast<std::string>(value);
								for (std::uint32_t i = 0; i < actualValue.length(); ++i)
								{
									convertString << std::uppercase << std::hex << unsigned(actualValue.at(i));
								}
								return convertString.str();
							}
						case PlkDataType::IP_ADDRESS:
							{
								std::string actualValue = boost::any_cast<std::string>(value);
								if (actualValue.substr(0, 2) == "0x")
								{
									convertString << std::uppercase << std::setw(8) << std::setfill('0') << std::hex << HexToInt<std::uint32_t>(actualValue);
								}
								else
								{
									std::vector<std::string> ipAddressParts;
									boost::split(ipAddressParts, actualValue, boost::is_any_of("."));

									for (auto& part : ipAddressParts)
									{
										convertString << std::uppercase << std::setw(2) << std::setfill('0') << std::hex << HexToInt<std::uint32_t>(part);
									}
								}
								return convertString.str();
							}
						case PlkDataType::TIME_OF_DAY:
						case PlkDataType::TIME_DIFF:
						case PlkDataType::NETTIME:
							{
								return boost::any_cast<std::string>(value);
							}
						default:
							break;
					}
					return "";
				}
				template std::uint16_t BaseObject::GetTypedValue<std::uint16_t>(ValueType type);
				template std::uint32_t BaseObject::GetTypedValue<std::uint32_t>(ValueType type);
				template std::uint64_t BaseObject::GetTypedValue<std::uint64_t>(ValueType type);
				template std::int16_t BaseObject::GetTypedValue<std::int16_t>(ValueType type);
				template std::int32_t BaseObject::GetTypedValue<std::int32_t>(ValueType type);
				template std::int64_t BaseObject::GetTypedValue<std::int64_t>(ValueType type);
				template float BaseObject::GetTypedValue<float>(ValueType type);
				template double BaseObject::GetTypedValue<double>(ValueType type);

				template<typename T>
				T BaseObject::GetTypedHighLimit()
				{
					return GetTypedValue<T>(ValueType::HIGHLIMIT);
				}
				template std::uint16_t BaseObject::GetTypedHighLimit<std::uint16_t>();
				template std::uint32_t BaseObject::GetTypedHighLimit<std::uint32_t>();
				template std::uint64_t BaseObject::GetTypedHighLimit<std::uint64_t>();
				template std::int16_t BaseObject::GetTypedHighLimit<std::int16_t>();
				template std::int32_t BaseObject::GetTypedHighLimit<std::int32_t>();
				template std::int64_t BaseObject::GetTypedHighLimit<std::int64_t>();
				template float BaseObject::GetTypedHighLimit<float>();
				template double BaseObject::GetTypedHighLimit<double>();

				template<typename T>
				T BaseObject::GetTypedLowLimit()
				{
					return GetTypedValue<T>(ValueType::LOWLIMIT);
				}
				template std::uint16_t BaseObject::GetTypedLowLimit<std::uint16_t>();
				template std::uint32_t BaseObject::GetTypedLowLimit<std::uint32_t>();
				template std::uint64_t BaseObject::GetTypedLowLimit<std::uint64_t>();
				template std::int16_t BaseObject::GetTypedLowLimit<std::int16_t>();
				template std::int32_t BaseObject::GetTypedLowLimit<std::int32_t>();
				template std::int64_t BaseObject::GetTypedLowLimit<std::int64_t>();
				template float BaseObject::GetTypedLowLimit<float>();
				template double BaseObject::GetTypedLowLimit<double>();

				template<typename T>
				T BaseObject::GetTypedActualValue()
				{
					return GetTypedValue<T>(ValueType::ACTUAL);
				}
				template<> std::string BaseObject::GetTypedActualValue<std::string>()
				{
					return GetTypedValue<std::string>(ValueType::ACTUAL);
				}
				template bool BaseObject::GetTypedActualValue<bool>();
				template std::uint16_t BaseObject::GetTypedActualValue<std::uint16_t>();
				template std::uint32_t BaseObject::GetTypedActualValue<std::uint32_t>();
				template std::uint64_t BaseObject::GetTypedActualValue<std::uint64_t>();
				template std::int16_t BaseObject::GetTypedActualValue<std::int16_t>();
				template std::int32_t BaseObject::GetTypedActualValue<std::int32_t>();
				template std::int64_t BaseObject::GetTypedActualValue<std::int64_t>();
				template double BaseObject::GetTypedActualValue<double>();

				template<typename T>
				T BaseObject::GetTypedDefaultValue()
				{
					return GetTypedValue<T>(ValueType::DEFAULT);
				}
				template<> std::string BaseObject::GetTypedDefaultValue<std::string>()
				{
					return GetTypedValue<std::string>(ValueType::DEFAULT);
				}
				template bool BaseObject::GetTypedDefaultValue<bool>();
				template std::uint16_t BaseObject::GetTypedDefaultValue<std::uint16_t>();
				template std::uint32_t BaseObject::GetTypedDefaultValue<std::uint32_t>();
				template std::uint64_t BaseObject::GetTypedDefaultValue<std::uint64_t>();
				template std::int16_t BaseObject::GetTypedDefaultValue<std::int16_t>();
				template std::int32_t BaseObject::GetTypedDefaultValue<std::int32_t>();
				template std::int64_t BaseObject::GetTypedDefaultValue<std::int64_t>();
				template float BaseObject::GetTypedDefaultValue<float>();
				template double BaseObject::GetTypedDefaultValue<double>();

				Result BaseObject::SetValue(const std::string& valueStr, ValueType type)
				{
					try
					{
						//Set empty is valid behavior leave uninitialised
						if (valueStr.empty())
							return Result();

						if (!this->GetDataType().is_initialized())
						{
							// Object has not datatype defined
							boost::format formatter(kMsgBaseObjectDataTypeError);
							formatter
							% this->GetName()
							% this->GetObjectId()
							% (std::uint32_t) this->GetContainingNode();
							LOG_FATAL() << formatter.str();
							return Result(ErrorCode::OBJECT_HAS_NO_DATATYPE, formatter.str());
						}
						boost::any valueToSet;

						switch (this->GetDataType().get())
						{
							case PlkDataType::INTEGER8:
								{
									std::int16_t value = HexToInt<std::int16_t>(valueStr);
									if (value < -128 || value > 127)
										throw std::range_error("");
									valueToSet = value;
									break;
								}
							case PlkDataType::INTEGER16:
								{
									std::int16_t value = HexToInt<std::int16_t>(valueStr);
									valueToSet = value;
									break;
								}
							case PlkDataType::INTEGER24:
							case PlkDataType::INTEGER32:
								{
									std::int32_t value = HexToInt<std::int32_t>(valueStr);
									valueToSet = value;
									break;
								}
							case PlkDataType::UNSIGNED8:
								{
									std::uint16_t value = HexToInt<std::uint16_t>(valueStr);
									if (value > 255)
										throw std::range_error("");
									valueToSet = value;
									break;
								}
							case PlkDataType::UNSIGNED16:
								{
									std::uint16_t value = HexToInt<std::uint16_t>(valueStr);
									valueToSet = value;
									break;
								}
							case PlkDataType::UNSIGNED24:
							case PlkDataType::UNSIGNED32:
								{
									std::uint32_t value = HexToInt<std::uint32_t>(valueStr);
									valueToSet = value;
									break;
								}
							case PlkDataType::REAL32:
								{
									float value = 0;
									if (valueStr.substr(0, 2) == "0x"
									        || valueStr.substr(0, 2) == "0X")
									{
										value = SinglePrecisisionHexToFloat(valueStr);
									}
									else
									{
										value = boost::lexical_cast<float>(valueStr);
									}
									valueToSet = value;
									break;
								}
							case PlkDataType::REAL64:
								{
									double value = 0;
									if (valueStr.substr(0, 2) == "0x"
									        || valueStr.substr(0, 2) == "0X")
									{
										value = DoublePrecisisionHexToDouble(valueStr);
									}
									else
									{
										value = boost::lexical_cast<double>(valueStr);
									}
									valueToSet = value;
									break;
								}
							case PlkDataType::INTEGER40:
							case PlkDataType::INTEGER48:
							case PlkDataType::INTEGER56:
							case PlkDataType::INTEGER64:
								{

									std::int64_t value = HexToInt<std::int64_t>(valueStr);
									valueToSet = value;
									break;
								}
							case PlkDataType::MAC_ADDRESS:
								{
									if (type == ValueType::LOWLIMIT || type == ValueType::HIGHLIMIT)
										return Result(ErrorCode::DATATYPE_DOES_NOT_SUPPORT_LIMITS);
								}
							case PlkDataType::UNSIGNED40:
							case PlkDataType::UNSIGNED48:
							case PlkDataType::UNSIGNED56:
							case PlkDataType::UNSIGNED64:
								{
									std::uint64_t value = HexToInt<std::uint64_t>(valueStr);
									valueToSet = value;
									break;
								}
							case PlkDataType::BOOLEAN:
								{
									bool value = StringToBool(valueStr);
									valueToSet = value;
									break;

								}
							case PlkDataType::IP_ADDRESS:
							case PlkDataType::NETTIME:
							case PlkDataType::VISIBLE_STRING:
							case PlkDataType::OCTET_STRING:
							case PlkDataType::UNICODE_STRING:
							case PlkDataType::TIME_OF_DAY:
							case PlkDataType::TIME_DIFF:
							case PlkDataType::Domain:
								{
									if (type == ValueType::LOWLIMIT || type == ValueType::HIGHLIMIT)
										return Result(ErrorCode::DATATYPE_DOES_NOT_SUPPORT_LIMITS);
									else
										valueToSet = valueStr;
									break;
								}
							default:
								{
									return Result(ErrorCode::OBJECT_TYPE_DOES_NOT_SUPPORT_VALUES);
								}
						}

						switch (type)
						{
							case ValueType::DEFAULT:
								{
									this->SetDefaultValue(valueToSet);
									break;
								}
							case ValueType::LOWLIMIT:
								{
									this->lowLimit = valueToSet;
									break;
								}
							case ValueType::HIGHLIMIT:
								{
									this->highLimit = valueToSet;
									break;
								}
							case ValueType::ACTUAL:
							default:
								break;
						}
						return Result();
					}
					catch (const std::exception& e)
					{
						switch (type)
						{
							case ValueType::DEFAULT:
								{
									boost::format formatter(kMsgDefaultValueDatatypeError);
									formatter
									% valueStr
									% GetPlkDataTypeName(GetDataType().get());
									LOG_FATAL() << formatter.str() << " " << e.what();
									return Result(ErrorCode::DEFAULT_VALUE_INVALID, formatter.str());
								}
							case ValueType::LOWLIMIT:
								{
									boost::format formatter(kMsgLowLimitDatatypeError);
									formatter
									% valueStr
									% GetPlkDataTypeName(GetDataType().get());
									LOG_FATAL() << formatter.str() << " " << e.what();
									return Result(ErrorCode::LOW_LIMIT_INVALID, formatter.str());
								}
							case ValueType::HIGHLIMIT:
								{
									boost::format formatter(kMsgHighLimitDatatypeError);
									formatter
									% valueStr
									% GetPlkDataTypeName(this->GetDataType().get());
									LOG_FATAL() << formatter.str() << " " << e.what();
									return Result(ErrorCode::HIGH_LIMIT_INVALID, formatter.str());
								}
							case ValueType::ACTUAL:
							default:
								break;
						}
						return Result(ErrorCode::UNHANDLED_EXCEPTION, e.what());
					}
				}

				Result BaseObject::SetHighLimit(const std::string& highLimit)
				{
					return SetValue(highLimit, ValueType::HIGHLIMIT);
				}

				Result BaseObject::SetLowLimit(const std::string& lowLimit)
				{
					return SetValue(lowLimit, ValueType::LOWLIMIT);
				}

				Result BaseObject::SetTypedObjectDefaultValue(const std::string& defaultValue)
				{
					return SetValue(defaultValue, ValueType::DEFAULT);
				}

				Result BaseObject::SetTypedObjectActualValue(const std::string& actualValue, bool validateOnly)
				{
					try
					{
						if (!this->GetDataType().is_initialized())
						{
							// Object has not datatype defined
							boost::format formatter(kMsgBaseObjectDataTypeError);
							formatter
							% this->GetName()
							% this->GetObjectId()
							% (std::uint32_t) this->GetContainingNode();
							if (validateOnly == false)
							{
								LOG_FATAL() << formatter.str();
							}
							return Result(ErrorCode::OBJECT_HAS_NO_DATATYPE, formatter.str());
						}

						switch (this->GetDataType().get())
						{
							case PlkDataType::BOOLEAN:
								{
									bool value = StringToBool(actualValue);
									if (validateOnly == false)
									{
										this->SetActualValue(boost::any(value));
										if (this->GetDefaultValue().empty())
											this->actualValueNotDefaultValue = true;
										else if (this->GetTypedDefaultValue<bool>() != value)
											this->actualValueNotDefaultValue = true;
										else
											this->actualValueNotDefaultValue = false;
									}
									break;
								}
							case PlkDataType::INTEGER8:
								{
									std::int16_t value = HexToInt<std::int16_t>(actualValue);
									if (value < -128 || value > 127)
										throw std::range_error("");

									if (this->highLimit.is_initialized())
									{
										if (value > this->GetTypedHighLimit<std::int16_t>())
										{
											boost::format formatter(kMsgBaseObjectHighLimitError);
											formatter
											% this->GetName()
											% this->GetObjectId()
											% (std::uint32_t) this->GetContainingNode()
											% actualValue
											% this->GetTypedHighLimit<std::int16_t>();
											if (validateOnly == false)
											{
												LOG_ERROR() << formatter.str();
											}
											return Result(ErrorCode::OBJECT_ACTUAL_VALUE_EXCEEDS_HIGHLIMIT, formatter.str());
										}
									}
									if (this->lowLimit.is_initialized())
									{
										if (value < this->GetTypedLowLimit<std::int16_t>())
										{
											boost::format formatter(kMsgBaseObjectLowLimitError);
											formatter
											% this->GetName()
											% this->GetObjectId()
											% (std::uint32_t) this->GetContainingNode()
											% actualValue
											% this->GetTypedLowLimit<std::int16_t>();
											if (validateOnly == false)
											{
												LOG_ERROR() << formatter.str();
											}
											return Result(ErrorCode::OBJECT_ACTUAL_VALUE_DECEEDS_LOWLIMIT, formatter.str());
										}
									}
									if (validateOnly == false)
									{
										this->SetActualValue(boost::any(value));
										if (this->GetDefaultValue().empty())
											this->actualValueNotDefaultValue = true;
										else if (this->GetTypedDefaultValue<std::int16_t>() != value)
											this->actualValueNotDefaultValue = true;
										else
											this->actualValueNotDefaultValue = false;
									}
									break;
								}
							case PlkDataType::INTEGER16:
								{
									std::int16_t value = HexToInt<std::int16_t>(actualValue);
									if (this->highLimit.is_initialized())
									{
										if (value > this->GetTypedHighLimit<std::int16_t>())
										{
											boost::format formatter(kMsgBaseObjectHighLimitError);
											formatter
											% this->GetName()
											% this->GetObjectId()
											% (std::uint32_t) this->GetContainingNode()
											% actualValue
											% this->GetTypedHighLimit<std::int16_t>();
											if (validateOnly == false)
											{
												LOG_ERROR() << formatter.str();
											}
											return Result(ErrorCode::OBJECT_ACTUAL_VALUE_EXCEEDS_HIGHLIMIT, formatter.str());
										}
									}
									if (this->lowLimit.is_initialized())
									{
										if (value < this->GetTypedLowLimit<std::int16_t>())
										{
											boost::format formatter(kMsgBaseObjectLowLimitError);
											formatter
											% this->GetName()
											% this->GetObjectId()
											% (std::uint32_t) this->GetContainingNode()
											% actualValue
											% this->GetTypedLowLimit<std::int16_t>();
											if (validateOnly == false)
											{
												LOG_ERROR() << formatter.str();
											}
											return Result(ErrorCode::OBJECT_ACTUAL_VALUE_DECEEDS_LOWLIMIT, formatter.str());
										}
									}
									if (validateOnly == false)
									{
										this->SetActualValue(boost::any(value));
										if (this->GetDefaultValue().empty())
											this->actualValueNotDefaultValue = true;
										else if (this->GetTypedDefaultValue<std::int16_t>() != value)
											this->actualValueNotDefaultValue = true;
										else
											this->actualValueNotDefaultValue = false;
									}
									break;
								}
							case PlkDataType::INTEGER32:
								{
									std::int32_t value = HexToInt<std::int32_t>(actualValue);
									if (this->highLimit.is_initialized())
									{
										if (value > this->GetTypedHighLimit<std::int32_t>())
										{
											boost::format formatter(kMsgBaseObjectHighLimitError);
											formatter
											% this->GetName()
											% this->GetObjectId()
											% (std::uint32_t) this->GetContainingNode()
											% actualValue
											% this->GetTypedHighLimit<std::int32_t>();
											if (validateOnly == false)
											{
												LOG_ERROR() << formatter.str();
											}
											return Result(ErrorCode::OBJECT_ACTUAL_VALUE_EXCEEDS_HIGHLIMIT, formatter.str());
										}
									}
									if (this->lowLimit.is_initialized())
									{
										if (value < this->GetTypedLowLimit<std::int32_t>())
										{
											boost::format formatter(kMsgBaseObjectLowLimitError);
											formatter
											% this->GetName()
											% this->GetObjectId()
											% (std::uint32_t) this->GetContainingNode()
											% actualValue
											% this->GetTypedLowLimit<std::int32_t>();
											if (validateOnly == false)
											{
												LOG_ERROR() << formatter.str();
											}
											return Result(ErrorCode::OBJECT_ACTUAL_VALUE_DECEEDS_LOWLIMIT, formatter.str());
										}
									}
									if (validateOnly == false)
									{
										this->SetActualValue(boost::any(value));
										if (this->GetDefaultValue().empty())
											this->actualValueNotDefaultValue = true;
										else if (this->GetTypedDefaultValue<std::int32_t>() != value)
											this->actualValueNotDefaultValue = true;
									}
									break;
								}
							case PlkDataType::UNSIGNED8:
								{
									std::uint16_t value = HexToInt<std::uint16_t>(actualValue);
									if (value > 255)
										throw std::range_error("");

									if (this->highLimit.is_initialized())
									{
										if (value > this->GetTypedHighLimit<std::uint16_t>())
										{
											boost::format formatter(kMsgBaseObjectHighLimitError);
											formatter
											% this->GetName()
											% this->GetObjectId()
											% (std::uint32_t) this->GetContainingNode()
											% actualValue
											% this->GetTypedHighLimit<std::uint16_t>();
											if (validateOnly == false)
											{
												LOG_ERROR() << formatter.str();
											}
											return Result(ErrorCode::OBJECT_ACTUAL_VALUE_EXCEEDS_HIGHLIMIT, formatter.str());
										}
									}
									if (this->lowLimit.is_initialized())
									{
										if (value < this->GetTypedLowLimit<std::uint16_t>())
										{
											boost::format formatter(kMsgBaseObjectLowLimitError);
											formatter
											% this->GetName()
											% this->GetObjectId()
											% (std::uint32_t) this->GetContainingNode()
											% actualValue
											% this->GetTypedLowLimit<std::uint16_t>();
											if (validateOnly == false)
											{
												LOG_ERROR() << formatter.str();
											}
											return Result(ErrorCode::OBJECT_ACTUAL_VALUE_DECEEDS_LOWLIMIT, formatter.str());
										}
									}
									if (validateOnly == false)
									{
										this->SetActualValue(boost::any(value));
										if (this->GetDefaultValue().empty())
											this->actualValueNotDefaultValue = true;
										else if (this->GetTypedDefaultValue<std::uint16_t>() != value)
											this->actualValueNotDefaultValue = true;
										else
											this->actualValueNotDefaultValue = false;
									}
									break;
								}
							case PlkDataType::UNSIGNED16:
								{
									uint16_t value = HexToInt<std::uint16_t>(actualValue);
									if (this->highLimit.is_initialized())
									{
										if (value > this->GetTypedHighLimit<std::uint16_t>())
										{
											boost::format formatter(kMsgBaseObjectHighLimitError);
											formatter
											% this->GetName()
											% this->GetObjectId()
											% (std::uint32_t) this->GetContainingNode()
											% actualValue
											% this->GetTypedHighLimit<std::uint16_t>();
											if (validateOnly == false)
											{
												LOG_ERROR() << formatter.str();
											}
											return Result(ErrorCode::OBJECT_ACTUAL_VALUE_EXCEEDS_HIGHLIMIT, formatter.str());
										}
									}
									if (this->lowLimit.is_initialized())
									{
										if (value < this->GetTypedLowLimit<std::uint16_t>())
										{
											boost::format formatter(kMsgBaseObjectLowLimitError);
											formatter
											% this->GetName()
											% this->GetObjectId()
											% (std::uint32_t) this->GetContainingNode()
											% actualValue
											% this->GetTypedLowLimit<std::uint16_t>();
											if (validateOnly == false)
											{
												LOG_ERROR() << formatter.str();
											}
											return Result(ErrorCode::OBJECT_ACTUAL_VALUE_DECEEDS_LOWLIMIT, formatter.str());
										}
									}
									if (validateOnly == false)
									{
										this->SetActualValue(boost::any(value));
										if (this->GetDefaultValue().empty())
											this->actualValueNotDefaultValue = true;
										else if (this->GetTypedDefaultValue<std::uint16_t>() != value)
											this->actualValueNotDefaultValue = true;
										else
											this->actualValueNotDefaultValue = false;
									}
									break;
								}
							case PlkDataType::UNSIGNED24:
							case PlkDataType::UNSIGNED32:
								{
									std::uint32_t value = HexToInt<std::uint32_t>(actualValue);
									if (this->highLimit.is_initialized())
									{
										if (value > this->GetTypedHighLimit<std::uint32_t>())
										{
											boost::format formatter(kMsgBaseObjectHighLimitError);
											formatter
											% this->GetName()
											% this->GetObjectId()
											% (std::uint32_t) this->GetContainingNode()
											% actualValue
											% this->GetTypedHighLimit<std::uint32_t>();
											if (validateOnly == false)
											{
												LOG_ERROR() << formatter.str();
											}
											return Result(ErrorCode::OBJECT_ACTUAL_VALUE_EXCEEDS_HIGHLIMIT, formatter.str());
										}
									}
									if (this->lowLimit.is_initialized())
									{
										if (value < this->GetTypedLowLimit<std::uint32_t>())
										{
											boost::format formatter(kMsgBaseObjectLowLimitError);
											formatter
											% this->GetName()
											% this->GetObjectId()
											% (std::uint32_t) this->GetContainingNode()
											% actualValue
											% this->GetTypedLowLimit<std::uint32_t>();
											if (validateOnly == false)
											{
												LOG_ERROR() << formatter.str();
											}
											return Result(ErrorCode::OBJECT_ACTUAL_VALUE_DECEEDS_LOWLIMIT, formatter.str());
										}
									}
									if (validateOnly == false)
									{
										this->SetActualValue(boost::any(value));
										if (this->GetDefaultValue().empty())
											this->actualValueNotDefaultValue = true;
										else if (this->GetTypedDefaultValue<std::uint32_t>() != value)
											this->actualValueNotDefaultValue = true;
										else
											this->actualValueNotDefaultValue = false;
									}
									break;
								}
							case PlkDataType::Domain:
								//Both values must be empty if Domain datatype
								break;
							case PlkDataType::INTEGER24:
								{
									std::int32_t value = HexToInt<std::int32_t>(actualValue);
									if (this->highLimit.is_initialized())
									{
										if (value > this->GetTypedHighLimit<std::int32_t>())
										{
											boost::format formatter(kMsgBaseObjectHighLimitError);
											formatter
											% this->GetName()
											% this->GetObjectId()
											% (std::uint32_t) this->GetContainingNode()
											% actualValue
											% this->GetTypedHighLimit<std::int32_t>();
											if (validateOnly == false)
											{
												LOG_ERROR() << formatter.str();
											}
											return Result(ErrorCode::OBJECT_ACTUAL_VALUE_EXCEEDS_HIGHLIMIT, formatter.str());
										}
									}
									if (this->lowLimit.is_initialized())
									{
										if (value < this->GetTypedLowLimit<std::int32_t>())
										{
											boost::format formatter(kMsgBaseObjectLowLimitError);
											formatter
											% this->GetName()
											% this->GetObjectId()
											% (std::uint32_t) this->GetContainingNode()
											% actualValue
											% this->GetTypedLowLimit<std::int32_t>();
											if (validateOnly == false)
											{
												LOG_ERROR() << formatter.str();
											}
											return Result(ErrorCode::OBJECT_ACTUAL_VALUE_DECEEDS_LOWLIMIT, formatter.str());
										}
									}
									if (validateOnly == false)
									{
										this->SetActualValue(boost::any(value));
										if (this->GetDefaultValue().empty())
											this->actualValueNotDefaultValue = true;
										else if (this->GetTypedDefaultValue<std::int32_t>() != value)
											this->actualValueNotDefaultValue = true;
										else
											this->actualValueNotDefaultValue = false;
									}
									break;
								}
							case PlkDataType::REAL32:
								{
									float value = 0;
									if (actualValue.substr(0, 2) == "0x"
									        || actualValue.substr(0, 2) == "0X")
									{
										value = SinglePrecisisionHexToFloat(actualValue);
									}
									else
									{
										value = boost::lexical_cast<float>(actualValue);
									}
									if (validateOnly == false)
									{
										this->SetActualValue(boost::any(value));
										if (this->GetDefaultValue().empty())
											this->actualValueNotDefaultValue = true;
										else if (this->GetTypedDefaultValue<float>() != value)
											this->actualValueNotDefaultValue = true;
										else
											this->actualValueNotDefaultValue = false;
									}
									break;
								}
							case PlkDataType::REAL64:
								{
									double value = 0;
									if (actualValue.substr(0, 2) == "0x"
									        || actualValue.substr(0, 2) == "0X")
									{
										value = DoublePrecisisionHexToDouble(actualValue);
									}
									else
									{
										value = boost::lexical_cast<double>(actualValue);
									}
									if (validateOnly == false)
									{
										this->SetActualValue(boost::any(value));
										if (this->GetDefaultValue().empty())
											this->actualValueNotDefaultValue = true;
										else if (this->GetTypedDefaultValue<double>() != value)
											this->actualValueNotDefaultValue = true;
										else
											this->actualValueNotDefaultValue = false;
									}
									break;
								}
							case PlkDataType::INTEGER40:
							case PlkDataType::INTEGER48:
							case PlkDataType::INTEGER56:
							case PlkDataType::INTEGER64:
								{
									int64_t value = HexToInt<std::int64_t>(actualValue);
									if (this->highLimit.is_initialized())
									{
										if (value > this->GetTypedHighLimit<std::int64_t>())
										{
											boost::format formatter(kMsgBaseObjectHighLimitError);
											formatter
											% this->GetName()
											% this->GetObjectId()
											% (std::uint32_t) this->GetContainingNode()
											% actualValue
											% this->GetTypedHighLimit<std::int64_t>();
											if (validateOnly == false)
											{
												LOG_ERROR() << formatter.str();
											}
											return Result(ErrorCode::OBJECT_ACTUAL_VALUE_EXCEEDS_HIGHLIMIT, formatter.str());
										}
									}
									if (this->lowLimit.is_initialized())
									{
										if (value < this->GetTypedHighLimit<std::int64_t>())
										{
											boost::format formatter(kMsgBaseObjectLowLimitError);
											formatter
											% this->GetName()
											% this->GetObjectId()
											% (std::uint32_t) this->GetContainingNode()
											% actualValue
											% this->GetTypedHighLimit<std::int64_t>();
											if (validateOnly == false)
											{
												LOG_ERROR() << formatter.str();
											}
											return Result(ErrorCode::OBJECT_ACTUAL_VALUE_DECEEDS_LOWLIMIT, formatter.str());
										}
									}
									if (validateOnly == false)
									{
										this->SetActualValue(boost::any(value));
										if (this->GetDefaultValue().empty())
											this->actualValueNotDefaultValue = true;
										else if (this->GetTypedDefaultValue<std::int64_t>() != value)
											this->actualValueNotDefaultValue = true;
										else
											this->actualValueNotDefaultValue = false;
									}
									break;
								}
							case PlkDataType::MAC_ADDRESS:
							case PlkDataType::UNSIGNED40:
							case PlkDataType::UNSIGNED48:
							case PlkDataType::UNSIGNED56:
							case PlkDataType::UNSIGNED64:
								{
									uint64_t value = HexToInt<std::uint64_t>(actualValue);
									if (this->highLimit.is_initialized())
									{
										if (value > this->GetTypedHighLimit<std::uint64_t>())
										{
											boost::format formatter(kMsgBaseObjectHighLimitError);
											formatter
											% this->GetName()
											% this->GetObjectId()
											% (std::uint32_t) this->GetContainingNode()
											% actualValue
											% this->GetTypedHighLimit<std::uint64_t>();
											if (validateOnly == false)
											{
												LOG_ERROR() << formatter.str();
											}
											return Result(ErrorCode::OBJECT_ACTUAL_VALUE_EXCEEDS_HIGHLIMIT, formatter.str());
										}
									}
									if (this->lowLimit.is_initialized())
									{
										if (value < this->GetTypedLowLimit<std::uint64_t>())
										{
											boost::format formatter(kMsgBaseObjectLowLimitError);
											formatter
											% this->GetName()
											% this->GetObjectId()
											% (std::uint32_t) this->GetContainingNode()
											% actualValue
											% this->GetTypedLowLimit<std::uint64_t>();
											if (validateOnly == false)
											{
												LOG_ERROR() << formatter.str();
											}
											return Result(ErrorCode::OBJECT_ACTUAL_VALUE_DECEEDS_LOWLIMIT, formatter.str());
										}
									}
									if (validateOnly == false)
									{
										this->SetActualValue(boost::any(value));
										if (this->GetDefaultValue().empty())
											this->actualValueNotDefaultValue = true;
										else if (this->GetTypedDefaultValue<std::uint64_t>() != value)
											this->actualValueNotDefaultValue = true;
										else
											this->actualValueNotDefaultValue = false;
									}
									break;
								}
							case PlkDataType::IP_ADDRESS:
								{
									if (!IsIPAddress(actualValue))
										throw std::range_error("");
								}
							case PlkDataType::NETTIME:
							case PlkDataType::VISIBLE_STRING:
							case PlkDataType::OCTET_STRING:
							case PlkDataType::UNICODE_STRING:
							case PlkDataType::TIME_OF_DAY:
							case PlkDataType::TIME_DIFF:
								{
									if (validateOnly == false)
									{
										this->SetActualValue(boost::any(actualValue));
										if (this->GetDefaultValue().empty())
											this->actualValueNotDefaultValue = true;
										else if (this->GetTypedDefaultValue<std::string>() != actualValue)
											this->actualValueNotDefaultValue = true;
										else
											this->actualValueNotDefaultValue = false;
									}
									break;
								}
							default:
								break;
						}
					}
					catch (const std::exception&)
					{
						boost::format formatter(kMsgActualValueDatatypeError);
						formatter
						% actualValue
						% GetPlkDataTypeName(GetDataType().get());
						if (validateOnly == false)
						{
							LOG_FATAL() << formatter.str();// << e.what();
						}
						return Result(ErrorCode::ACTUAL_VALUE_INVALID, formatter.str());
					}
					return Result();
				}

				uint8_t BaseObject::GetContainingNode()
				{
					return this->containingNode;
				}

				std::uint32_t BaseObject::GetBitSize()
				{
					if (this->GetDataType().is_initialized())
					{
						switch (this->GetDataType().get())
						{
							case PlkDataType::BOOLEAN:
							case PlkDataType::UNSIGNED8:
							case PlkDataType::INTEGER8:
								return 8;
							case PlkDataType::UNSIGNED16:
							case PlkDataType::INTEGER16:
								return 16;
							case PlkDataType::INTEGER32:
							case PlkDataType::UNSIGNED32:
							case PlkDataType::REAL32:
							case PlkDataType::IP_ADDRESS:
								return 32;
							case PlkDataType::VISIBLE_STRING:
							case PlkDataType::OCTET_STRING:
							case PlkDataType::UNICODE_STRING:
								{
									if (this->GetTypedActualValue<std::string>().size() != 0)
										return 8 * (std::uint32_t) this->GetTypedActualValue<std::string>().size();
									else if (this->GetTypedDefaultValue<std::string>().size() != 0)
										return 8 * (std::uint32_t) this->GetTypedDefaultValue<std::string>().size();
									break;
								}
							/*case PlkDataType::UNICODE_STRING:
								{
									if (this->GetTypedActualValue<std::string>().size() != 0)
										return 16 * (std::uint32_t) this->GetTypedActualValue<std::string>().size();
									else if (this->GetTypedDefaultValue<std::string>().size() != 0)
										return 16 * (std::uint32_t) this->GetTypedDefaultValue<std::string>().size();
									break;
						}*///Since we do not have Unicode support calculate normal octet size for std::string
							case PlkDataType::Domain:
								{
									if (this->GetUniqueIdRef().is_initialized())
									{
										if (this->referencedParameter)
											return this->referencedParameter->GetBitSize();
										else if (this->referencedParameterGrp)
											return this->referencedParameterGrp->GetBitSize();
									}
									break;
								}
							case PlkDataType::INTEGER24:
							case PlkDataType::UNSIGNED24:
								return 24;
							case PlkDataType::INTEGER40:
							case PlkDataType::UNSIGNED40:
								return 40;
							case PlkDataType::INTEGER48:
							case PlkDataType::UNSIGNED48:
							case PlkDataType::TIME_OF_DAY:
							case PlkDataType::TIME_DIFF:
							case PlkDataType::MAC_ADDRESS:
								return 48;
							case PlkDataType::INTEGER56:
							case PlkDataType::UNSIGNED56:
								return 56;
							case PlkDataType::INTEGER64:
							case PlkDataType::UNSIGNED64:
							case PlkDataType::REAL64:
							case PlkDataType::NETTIME:
								return 64;
							default:
								break;
						}
					}
					return 0;
				}

				void BaseObject::SetComplexDataType(std::shared_ptr<Parameter>& parameter)
				{
					this->referencedParameter = parameter;
					if (parameter->GetParameterAccess() != ParameterAccess::undefined)
					{
						if (this->accessType.is_initialized())
						{
							if (this->accessType.get() != Utilities::GetAccessTypeFromParameterAccessType(parameter->GetParameterAccess()))
								this->accessType = Utilities::GetAccessTypeFromParameterAccessType(parameter->GetParameterAccess());
						}
						else
							this->accessType = Utilities::GetAccessTypeFromParameterAccessType(parameter->GetParameterAccess());
					}
					if (parameter->GetDataType().is_initialized())
					{
						if (this->dataType.is_initialized())
						{
							if (this->dataType.get() != Utilities::GetPlkDataType(parameter->GetDataType().get()))
								this->dataType = Utilities::GetPlkDataType(parameter->GetDataType().get());
						}
						else
							this->dataType = Utilities::GetPlkDataType(parameter->GetDataType().get());
					}
				}

				void BaseObject::SetComplexDataType(std::shared_ptr<ParameterGroup>& parameterGrp)
				{
					this->referencedParameterGrp = parameterGrp;
				}

				bool BaseObject::HasActualValue() const
				{
					if (this->GetActualValue().empty())
						return false;
					return true;
				}

				bool BaseObject::HasDefaultValue() const
				{
					if (this->GetDefaultValue().empty())
						return false;
					return true;
				}

				bool BaseObject::WriteToConfiguration() const
				{
					if ((this->forceToCDC == true //Object forced
					        || this->actualValueNotDefaultValue == true) //Actual value != default value
					        && (this->GetAccessType() != AccessType::COND //correct AccessType
					            && this->GetAccessType() != AccessType::CONST
					            && this->GetAccessType() != AccessType::RO))
						return true;
					else if (this->uniqueIdRef.is_initialized() && this->referencedParameter)
					{
						if (this->referencedParameter->WriteToConfiguration() || this->actualValueNotDefaultValue == true)
						{
							return true;
						}
						return false;
					}
					else if (this->uniqueIdRef.is_initialized() && this->referencedParameterGrp)
					{
						if (!(this->HasDefaultValue() == false && this->HasActualValue() == false) || this->actualValueNotDefaultValue == true)
						{
							return true;
						}
						return false;
					}
					return false;
				}

				void BaseObject::ClearActualValue()
				{
					this->SetActualValue(boost::any());
					this->actualValueNotDefaultValue = false;
					this->forceToCDC = false;
				}
			}
		}
	}
}
