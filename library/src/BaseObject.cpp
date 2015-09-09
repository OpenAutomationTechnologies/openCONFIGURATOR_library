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
				BaseObject::BaseObject() : IBaseObject(),
					forceToCDC(false),
					highLimit(),
					lowLimit(),
					uniqueIdRef(),
					complexDataType(),
					accessType(),
					dataType(),
					pdoMapping(),
					containingNode(),
					actualValueNotDefaultValue(false)
				{}

				BaseObject::BaseObject(std::uint32_t id, ObjectType objectType, const std::string& name, std::uint8_t containingNode) : IBaseObject(id, objectType, name),
					forceToCDC(false),
					highLimit(boost::optional<boost::any>()),
					lowLimit(boost::optional<boost::any>()),
					uniqueIdRef(boost::optional<std::string>()),
					complexDataType(),
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
					complexDataType(),
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
					complexDataType(),
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

				template<typename T>
				T BaseObject::GetTypedHighLimit()
				{
					if (this->GetDataType().is_initialized() && this->GetHighLimit().is_initialized())
					{
						if (this->GetHighLimit().get().type() == typeid(T))
						{
							return boost::any_cast<T>(this->GetHighLimit().get());
						}
						//Datatype does not match
						boost::format formatter(kMsgObjectDatatypeMismatch);
						formatter
						% this->GetName()
						% typeid(T).name()
						% this->GetHighLimit().get().type().name();
						LOG_FATAL() << formatter.str();
						throw Result(ErrorCode::DATATYPE_MISMATCH, formatter.str());
					}
					// Object has not datatype defined
					boost::format formatter(kMsgBaseObjectDataTypeError);
					formatter
					% this->GetName()
					% this->GetObjectId()
					% (std::uint32_t) this->GetContainingNode();
					LOG_FATAL() << formatter.str();
					throw Result(ErrorCode::OBJECT_HAS_NO_DATATYPE, formatter.str());
				}
				template std::uint16_t BaseObject::GetTypedHighLimit<std::uint16_t>();
				template std::uint32_t BaseObject::GetTypedHighLimit<std::uint32_t>();
				template std::uint64_t BaseObject::GetTypedHighLimit<std::uint64_t>();
				template std::int16_t BaseObject::GetTypedHighLimit<std::int16_t>();
				template std::int32_t BaseObject::GetTypedHighLimit<std::int32_t>();
				template std::int64_t BaseObject::GetTypedHighLimit<std::int64_t>();
				template float BaseObject::GetTypedHighLimit<float>();
				template double BaseObject::GetTypedHighLimit<double>();

				Result BaseObject::SetHighLimit(const std::string& highLimit)
				{
					try
					{
						//Set empty is valid behavior leave uninitialised
						if (highLimit.empty())
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

						switch (this->GetDataType().get())
						{
							case PlkDataType::INTEGER8:
								{
									std::int16_t value = HexToInt<std::int16_t>(highLimit);
									this->highLimit = value;
									break;
								}

							case PlkDataType::INTEGER16:
								{
									std::int16_t value = HexToInt<std::int16_t>(highLimit);
									this->highLimit = value;
									break;
								}
							case PlkDataType::INTEGER24:
							case PlkDataType::INTEGER32:
								{
									std::int32_t value = HexToInt<std::int32_t>(highLimit);
									this->highLimit = value;
									break;
								}
							case PlkDataType::UNSIGNED8:
								{
									std::uint16_t value = HexToInt<std::uint16_t>(highLimit);
									this->highLimit = value;
									break;
								}
							case PlkDataType::UNSIGNED16:
								{

									std::uint16_t value = HexToInt<std::uint16_t>(highLimit);
									this->highLimit = value;
									break;
								}
							case PlkDataType::UNSIGNED24:
							case PlkDataType::UNSIGNED32:
								{

									std::uint32_t value = HexToInt<std::uint32_t>(highLimit);
									this->highLimit = value;
									break;
								}
							case PlkDataType::REAL32:
								{
									float value = boost::lexical_cast<float>(highLimit);
									this->highLimit = value;
									break;
								}
							case PlkDataType::REAL64:
								{
									double value = boost::lexical_cast<double>(highLimit);
									this->highLimit = value;
									break;
								}
							case PlkDataType::INTEGER40:
							case PlkDataType::INTEGER48:
							case PlkDataType::INTEGER56:
							case PlkDataType::INTEGER64:
								{

									std::int64_t value = HexToInt<std::int64_t>(highLimit);
									this->highLimit = value;
									break;
								}
							case PlkDataType::UNSIGNED40:
							case PlkDataType::UNSIGNED48:
							case PlkDataType::UNSIGNED56:
							case PlkDataType::UNSIGNED64:
								{
									std::uint64_t value = HexToInt<std::uint64_t>(highLimit);
									this->highLimit = value;
									break;
								}
							case PlkDataType::BOOLEAN:
							case PlkDataType::Domain:
							case PlkDataType::MAC_ADDRESS:
							case PlkDataType::IP_ADDRESS:
							case PlkDataType::NETTIME:
							case PlkDataType::VISIBLE_STRING:
							case PlkDataType::OCTET_STRING:
							case PlkDataType::UNICODE_STRING:
							case PlkDataType::TIME_OF_DAY:
							case PlkDataType::TIME_DIFF:
								{
									return Result(ErrorCode::DATATYPE_DOES_NOT_SUPPORT_LIMITS);
								}
							default:
								break;
						}
					}
					catch (const std::exception& e)
					{
						boost::format formatter(kMsgHighLimitDatatypeError);
						formatter
						% highLimit
						% (std::uint32_t) this->GetDataType().get();
						LOG_FATAL() << formatter.str() << e.what();
						return Result(ErrorCode::HIGH_LIMIT_INVALID, formatter.str());
					}
					return Result();
				}

				const boost::optional<boost::any>& BaseObject::GetLowLimit() const
				{
					return this->lowLimit;
				}

				template<typename T>
				T BaseObject::GetTypedLowLimit()
				{
					if (this->GetDataType().is_initialized() && this->GetLowLimit().is_initialized())
					{
						if (this->GetLowLimit().get().type() == typeid(T))
						{
							return boost::any_cast<T>(this->GetLowLimit().get());
						}
						//Datatype does not match
						boost::format formatter(kMsgObjectDatatypeMismatch);
						formatter
						% this->GetName()
						% typeid(T).name()
						% this->GetLowLimit().get().type().name();
						LOG_FATAL() << formatter.str();
						throw Result(ErrorCode::DATATYPE_MISMATCH, formatter.str());
					}
					// Object has not datatype defined
					boost::format formatter(kMsgBaseObjectDataTypeError);
					formatter
					% this->GetName()
					% this->GetObjectId()
					% (std::uint32_t) this->GetContainingNode();
					LOG_FATAL() << formatter.str();
					throw Result(ErrorCode::OBJECT_HAS_NO_DATATYPE, formatter.str());
				}
				template std::uint16_t BaseObject::GetTypedLowLimit<std::uint16_t>();
				template std::uint32_t BaseObject::GetTypedLowLimit<std::uint32_t>();
				template std::uint64_t BaseObject::GetTypedLowLimit<std::uint64_t>();
				template std::int16_t BaseObject::GetTypedLowLimit<std::int16_t>();
				template std::int32_t BaseObject::GetTypedLowLimit<std::int32_t>();
				template std::int64_t BaseObject::GetTypedLowLimit<std::int64_t>();
				template float BaseObject::GetTypedLowLimit<float>();
				template double BaseObject::GetTypedLowLimit<double>();

				Result BaseObject::SetLowLimit(const std::string& lowLimit)
				{
					try
					{
						if (lowLimit.empty())
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

						switch (this->GetDataType().get())
						{
							case PlkDataType::INTEGER8:
								{
									std::int16_t value = HexToInt<std::int16_t>(lowLimit);
									this->lowLimit = value;
									break;
								}
							case PlkDataType::INTEGER16:
								{
									std::int16_t value = HexToInt<std::int16_t>(lowLimit);
									this->lowLimit = value;
									break;
								}
							case PlkDataType::INTEGER24:
							case PlkDataType::INTEGER32:
								{
									std::int32_t value = HexToInt<std::int32_t>(lowLimit);
									this->lowLimit = value;
									break;
								}
							case PlkDataType::UNSIGNED8:
								{
									std::uint16_t value = HexToInt<std::uint16_t>(lowLimit);
									this->lowLimit = value;
									break;
								}
							case PlkDataType::UNSIGNED16:
								{
									std::uint16_t value = HexToInt<std::uint16_t>(lowLimit);
									this->lowLimit = value;
									break;
								}
							case PlkDataType::UNSIGNED24:
							case PlkDataType::UNSIGNED32:
								{
									std::uint32_t value = HexToInt<std::uint32_t>(lowLimit);
									this->lowLimit = value;
									break;
								}
							case PlkDataType::REAL32:
								{
									float value = boost::lexical_cast<float>(lowLimit);
									this->lowLimit = value;
									break;
								}
							case PlkDataType::REAL64:
								{
									double value = boost::lexical_cast<double>(lowLimit);
									this->lowLimit = value;
									break;
								}
							case PlkDataType::INTEGER40:
							case PlkDataType::INTEGER48:
							case PlkDataType::INTEGER56:
							case PlkDataType::INTEGER64:
								{
									int64_t value = HexToInt<std::int64_t>(lowLimit);
									this->lowLimit = value;
									break;
								}
							case PlkDataType::UNSIGNED40:
							case PlkDataType::UNSIGNED48:
							case PlkDataType::UNSIGNED56:
							case PlkDataType::UNSIGNED64:
								{
									std::uint64_t value = HexToInt<std::uint64_t>(lowLimit);
									this->lowLimit = value;
									break;
								}
							case PlkDataType::BOOLEAN:
							case PlkDataType::Domain:
							case PlkDataType::MAC_ADDRESS:
							case PlkDataType::IP_ADDRESS:
							case PlkDataType::NETTIME:
							case PlkDataType::VISIBLE_STRING:
							case PlkDataType::OCTET_STRING:
							case PlkDataType::UNICODE_STRING:
							case PlkDataType::TIME_OF_DAY:
							case PlkDataType::TIME_DIFF:
								{
									return Result(ErrorCode::DATATYPE_DOES_NOT_SUPPORT_LIMITS);
								}
							default:
								break;
						}
					}
					catch (const std::exception& e)
					{
						boost::format formatter(kMsgLowLimitDatatypeError);
						formatter
						% lowLimit
						% (std::uint32_t) GetDataType().get();
						LOG_FATAL() << formatter.str() << e.what();
						return Result(ErrorCode::LOW_LIMIT_INVALID, formatter.str());
					}
					return Result();
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

				template<typename T>
				T BaseObject::GetTypedActualValue()
				{
					if (this->GetDataType().is_initialized() && !this->GetActualValue().empty())
					{
						if (this->GetActualValue().type() == typeid(T))
						{
							//return original stored value
							return boost::any_cast<T>(this->GetActualValue());
						}
						//Datatype does not match
						boost::format formatter(kMsgObjectDatatypeMismatch);
						formatter
						% this->GetName()
						% typeid(T).name()
						% this->GetActualValue().type().name();
						LOG_FATAL() << formatter.str();
						throw Result(ErrorCode::DATATYPE_MISMATCH, formatter.str());
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

				template<> std::string BaseObject::GetTypedActualValue<std::string>()
				{
					if (!this->GetDataType().is_initialized() && !this->GetActualValue().empty())
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

					std::stringstream convertString;
					switch (this->GetDataType().get())
					{
						case PlkDataType::BOOLEAN:
							{
								bool res = boost::any_cast<bool>(this->GetActualValue());
								return (res) ? "01" : "00";
							}
						case PlkDataType::INTEGER8:
							{
								convertString << std::uppercase << std::setfill('0') << std::setw(2) << std::hex << boost::any_cast<std::int16_t>(this->GetActualValue());
								return convertString.str();
							}
						case PlkDataType::INTEGER16:
							{
								convertString << std::uppercase << std::setfill('0') << std::setw(4) << std::hex << boost::any_cast<std::int16_t>(this->GetActualValue());
								return convertString.str();
							}
						case PlkDataType::INTEGER24:
							{
								convertString << std::uppercase << std::setfill('0') << std::setw(6) << std::hex << boost::any_cast<std::int32_t>(this->GetActualValue());
								return convertString.str();
							}
						case PlkDataType::INTEGER32:
							{
								convertString << std::uppercase << std::setfill('0') << std::setw(8) << std::hex << boost::any_cast<std::int32_t>(this->GetActualValue());
								return convertString.str();
							}
						case PlkDataType::UNSIGNED8:
							{
								convertString << std::uppercase << std::setfill('0') << std::setw(2) << std::hex << boost::any_cast<std::uint16_t>(this->GetActualValue());
								return convertString.str();
							}
						case PlkDataType::UNSIGNED16:
							{
								convertString << std::uppercase << std::setfill('0') << std::setw(4) << std::hex << boost::any_cast<std::uint16_t>(this->GetActualValue());
								return convertString.str();
							}
						case PlkDataType::UNSIGNED24:
							{
								convertString << std::uppercase << std::setfill('0') << std::setw(6) << std::hex << boost::any_cast<std::uint32_t>(this->GetActualValue());
								return convertString.str();
							}
						case PlkDataType::UNSIGNED32:
							{
								convertString << std::uppercase << std::setfill('0') << std::setw(8) << std::hex << boost::any_cast<std::uint32_t>(this->GetActualValue());
								return convertString.str();
							}
						case PlkDataType::REAL32:
							{
								convertString << std::hex << std::uppercase << std::setfill('0') << std::setw(8) << FloatToSinglePrecisisionHex(boost::any_cast<float>(this->GetActualValue()));
								return convertString.str();
							}
						case PlkDataType::REAL64:
							{
								convertString << std::hex << std::uppercase << std::setfill('0') << std::setw(16) << DoubleToDoublePrecisisionHex(boost::any_cast<double>(this->GetActualValue()));
								return convertString.str();
							}
						case PlkDataType::Domain:
							break;
						case PlkDataType::INTEGER40:
							{
								convertString << std::uppercase << std::setfill('0') << std::setw(10) << std::hex << boost::any_cast<std::int64_t>(this->GetActualValue());
								return convertString.str();
							}
						case PlkDataType::INTEGER48:
							{
								convertString << std::uppercase << std::setfill('0') << std::setw(12) << std::hex << boost::any_cast<std::int64_t>(this->GetActualValue());
								return convertString.str();
							}
						case PlkDataType::INTEGER56:
							{
								convertString << std::uppercase << std::setfill('0') << std::setw(14) << std::hex << boost::any_cast<std::int64_t>(this->GetActualValue());
								return convertString.str();
							}
						case PlkDataType::INTEGER64:
							{
								convertString << std::uppercase << std::setfill('0') << std::setw(16) << std::hex << boost::any_cast<std::int64_t>(this->GetActualValue());
								return convertString.str();
							}
						case PlkDataType::UNSIGNED40:
							{
								convertString << std::uppercase << std::setfill('0') << std::setw(10) << std::hex << boost::any_cast<std::uint64_t>(this->GetActualValue());
								return convertString.str();
							}
						case PlkDataType::MAC_ADDRESS:
						case PlkDataType::UNSIGNED48:
							{
								convertString << std::uppercase << std::setfill('0') << std::setw(12) << std::hex << boost::any_cast<std::uint64_t>(this->GetActualValue());
								return convertString.str();
							}
						case PlkDataType::UNSIGNED56:
							{
								convertString << std::uppercase << std::setfill('0') << std::setw(14) << std::hex << boost::any_cast<std::uint64_t>(this->GetActualValue());
								return convertString.str();
							}
						case PlkDataType::UNSIGNED64:
							{
								convertString << std::uppercase << std::setfill('0') << std::setw(16) << std::hex << boost::any_cast<std::uint64_t>(this->GetActualValue());
								return convertString.str();
							}
						case PlkDataType::VISIBLE_STRING:
						case PlkDataType::OCTET_STRING:
						case PlkDataType::UNICODE_STRING:
							{
								std::string actualValue = boost::any_cast<std::string>(this->GetActualValue());
								for (std::uint32_t i = 0; i < actualValue.length(); ++i)
								{
									convertString << std::uppercase << std::hex << unsigned(actualValue.at(i));
								}
								return convertString.str();
							}
						case PlkDataType::IP_ADDRESS:
							{
								std::string actualValue = boost::any_cast<std::string>(this->GetActualValue());
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
								return boost::any_cast<std::string>(this->GetActualValue());
							}
						default:
							break;
					}
					return "";
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
					if (this->GetDataType().is_initialized() && !this->GetDefaultValue().empty())
					{
						if (this->GetDefaultValue().type() == typeid(T))
						{
							return boost::any_cast<T>(this->GetDefaultValue());
						}
						//Datatype does not match
						boost::format formatter(kMsgObjectDatatypeMismatch);
						formatter
						% this->GetName()
						% typeid(T).name()
						% this->GetDefaultValue().type().name();
						LOG_FATAL() << formatter.str();
						throw Result(ErrorCode::DATATYPE_MISMATCH, formatter.str());
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

				template<> std::string BaseObject::GetTypedDefaultValue<std::string>()
				{
					if (!this->GetDataType().is_initialized() && !this->GetDefaultValue().empty())
					{
						boost::format formatter(kMsgBaseObjectDefaultValue);
						formatter
						% this->GetName()
						% this->GetObjectId()
						% (std::uint32_t) this->GetContainingNode();
						LOG_FATAL() << formatter.str();
						throw Result(ErrorCode::OBJECT_HAS_NO_DEFAULT_VALUE, formatter.str());
					}

					std::stringstream convertString;
					switch (this->GetDataType().get())
					{
						case PlkDataType::BOOLEAN:
							{
								bool res = boost::any_cast<bool>(this->GetDefaultValue());
								return (res) ? "01" : "00";
							}
						case PlkDataType::INTEGER8:
							{
								convertString << std::uppercase << std::setfill('0') << std::setw(2) << std::hex << boost::any_cast<std::int16_t>(this->GetDefaultValue());
								return convertString.str();
							}
						case PlkDataType::INTEGER16:
							{
								convertString << std::uppercase << std::setfill('0') << std::setw(4) << std::hex << boost::any_cast<std::int16_t>(this->GetDefaultValue());
								return convertString.str();
							}
						case PlkDataType::INTEGER24:
							{
								convertString << std::uppercase << std::setfill('0') << std::setw(6) << std::hex << boost::any_cast<std::int32_t>(this->GetDefaultValue());
								return convertString.str();
							}
						case PlkDataType::INTEGER32:
							{
								convertString << std::uppercase << std::setfill('0') << std::setw(8) << std::hex << boost::any_cast<std::int32_t>(this->GetDefaultValue());
								return convertString.str();
							}
						case PlkDataType::UNSIGNED8:
							{
								convertString << std::uppercase << std::setfill('0') << std::setw(2) << std::hex << boost::any_cast<std::uint16_t>(this->GetDefaultValue());
								return convertString.str();
							}
						case PlkDataType::UNSIGNED16:
							{
								convertString << std::uppercase << std::setfill('0') << std::setw(4) << std::hex << boost::any_cast<std::uint16_t>(this->GetDefaultValue());
								return convertString.str();
							}
						case PlkDataType::UNSIGNED24:
							{
								convertString << std::uppercase << std::setfill('0') << std::setw(6) << std::hex << boost::any_cast<std::uint32_t>(this->GetDefaultValue());
								return convertString.str();
							}
						case PlkDataType::UNSIGNED32:
							{
								convertString << std::uppercase << std::setfill('0') << std::setw(8) << std::hex << boost::any_cast<std::uint32_t>(this->GetDefaultValue());
								return convertString.str();
							}
						case PlkDataType::REAL32:
							{
								convertString << std::hex << std::uppercase << std::setfill('0') << std::setw(8) << FloatToSinglePrecisisionHex(boost::any_cast<float>(this->GetDefaultValue()));
								return convertString.str();
							}
						case PlkDataType::REAL64:
							{
								convertString << std::hex << std::uppercase << std::setfill('0') << std::setw(16) << DoubleToDoublePrecisisionHex(boost::any_cast<double>(this->GetDefaultValue()));
								return convertString.str();
							}
						case PlkDataType::Domain:
							break;
						case PlkDataType::INTEGER40:
							{
								convertString << std::uppercase << std::setfill('0') << std::setw(10) << std::hex << boost::any_cast<std::int64_t>(this->GetDefaultValue());
								return convertString.str();
							}
						case PlkDataType::INTEGER48:
							{
								convertString << std::uppercase << std::setfill('0') << std::setw(12) << std::hex << boost::any_cast<std::int64_t>(this->GetDefaultValue());
								return convertString.str();
							}
						case PlkDataType::INTEGER56:
							{
								convertString << std::uppercase << std::setfill('0') << std::setw(14) << std::hex << boost::any_cast<std::int64_t>(this->GetDefaultValue());
								return convertString.str();
							}
						case PlkDataType::INTEGER64:
							{
								convertString << std::uppercase << std::setfill('0') << std::setw(16) << std::hex << boost::any_cast<std::int64_t>(this->GetDefaultValue());
								return convertString.str();
							}
						case PlkDataType::UNSIGNED40:
							{
								convertString << std::uppercase << std::setfill('0') << std::setw(10) << std::hex << boost::any_cast<std::uint64_t>(this->GetDefaultValue());
								return convertString.str();
							}
						case PlkDataType::MAC_ADDRESS:
						case PlkDataType::UNSIGNED48:
							{
								convertString << std::uppercase << std::setfill('0') << std::setw(12) << std::hex << boost::any_cast<std::uint64_t>(this->GetDefaultValue());
								return convertString.str();
							}
						case PlkDataType::UNSIGNED56:
							{
								convertString << std::uppercase << std::setfill('0') << std::setw(14) << std::hex << boost::any_cast<std::uint64_t>(this->GetDefaultValue());
								return convertString.str();
							}
						case PlkDataType::UNSIGNED64:
							{
								convertString << std::uppercase << std::setfill('0') << std::setw(16) << std::hex << boost::any_cast<std::uint64_t>(this->GetDefaultValue());
								return convertString.str();
							}
						case PlkDataType::VISIBLE_STRING:
						case PlkDataType::OCTET_STRING:
						case PlkDataType::UNICODE_STRING:
							{
								std::string defaultValue = boost::any_cast<std::string>(this->GetDefaultValue());
								for (std::uint32_t i = 0; i < defaultValue.length(); ++i)
								{
									convertString << std::uppercase << std::hex << unsigned(defaultValue.at(i));
								}
								return convertString.str();
							}
						case PlkDataType::IP_ADDRESS:
							{
								std::string defaultValue = boost::any_cast<std::string>(this->GetDefaultValue());
								std::vector<std::string> ipAddressParts;
								boost::split(ipAddressParts, defaultValue, boost::is_any_of("."));

								for (auto& part : ipAddressParts)
								{
									convertString << std::uppercase << std::setw(2) << std::setfill('0') << std::hex << HexToInt<std::uint32_t>(part);
								}
								return convertString.str();
							}
						case PlkDataType::TIME_OF_DAY:
						case PlkDataType::TIME_DIFF:
						case PlkDataType::NETTIME:
							{
								return boost::any_cast<std::string>(this->GetDefaultValue());
							}
						default:
							break;
					}
					return "";
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

				Result BaseObject::SetTypedObjectActualValue(const std::string& actualValue)
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
							LOG_FATAL() << formatter.str();
							return Result(ErrorCode::OBJECT_HAS_NO_DATATYPE, formatter.str());
						}

						switch (this->GetDataType().get())
						{
							case PlkDataType::BOOLEAN:
								{
									bool value = StringToBool(actualValue);
									this->SetActualValue(boost::any(value));
									if (this->GetDefaultValue().empty())
										this->actualValueNotDefaultValue = true;
									else if (this->GetTypedDefaultValue<bool>() != value)
										this->actualValueNotDefaultValue = true;
									else
										this->actualValueNotDefaultValue = false;

									break;
								}
							case PlkDataType::INTEGER8:
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
											% this->GetTypedHighLimit<std::int16_t>();
											LOG_FATAL() << formatter.str();
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
											% this->GetTypedLowLimit<std::int16_t>();
											LOG_FATAL() << formatter.str();
											return Result(ErrorCode::OBJECT_ACTUAL_VALUE_DECEEDS_LOWLIMIT, formatter.str());
										}
									}
									this->SetActualValue(boost::any(value));
									if (this->GetDefaultValue().empty())
										this->actualValueNotDefaultValue = true;
									else if (this->GetTypedDefaultValue<std::int16_t>() != value)
										this->actualValueNotDefaultValue = true;
									else
										this->actualValueNotDefaultValue = false;
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
											% this->GetTypedHighLimit<std::int32_t>();
											LOG_FATAL() << formatter.str();
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
											% this->GetTypedLowLimit<std::int32_t>();
											LOG_FATAL() << formatter.str();
											return Result(ErrorCode::OBJECT_ACTUAL_VALUE_DECEEDS_LOWLIMIT, formatter.str());
										}
									}
									this->SetActualValue(boost::any(value));
									if (this->GetDefaultValue().empty())
										this->actualValueNotDefaultValue = true;
									else if (this->GetTypedDefaultValue<std::int32_t>() != value)
										this->actualValueNotDefaultValue = true;
									break;
								}
							case PlkDataType::UNSIGNED8:
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
											% this->GetTypedHighLimit<std::uint16_t>();
											LOG_FATAL() << formatter.str();
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
											% this->GetTypedLowLimit<std::uint16_t>();
											LOG_FATAL() << formatter.str();
											return Result(ErrorCode::OBJECT_ACTUAL_VALUE_DECEEDS_LOWLIMIT, formatter.str());
										}
									}
									this->SetActualValue(boost::any(value));
									if (this->GetDefaultValue().empty())
										this->actualValueNotDefaultValue = true;
									else if (this->GetTypedDefaultValue<std::uint16_t>() != value)
										this->actualValueNotDefaultValue = true;
									else
										this->actualValueNotDefaultValue = false;
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
											% this->GetTypedHighLimit<std::uint32_t>();
											LOG_FATAL() << formatter.str();
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
											% this->GetTypedLowLimit<std::uint32_t>();
											LOG_FATAL() << formatter.str();
											return Result(ErrorCode::OBJECT_ACTUAL_VALUE_DECEEDS_LOWLIMIT, formatter.str());
										}
									}
									this->SetActualValue(boost::any(value));
									if (this->GetDefaultValue().empty())
										this->actualValueNotDefaultValue = true;
									else if (this->GetTypedDefaultValue<std::uint32_t>() != value)
										this->actualValueNotDefaultValue = true;
									else
										this->actualValueNotDefaultValue = false;
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
											% this->GetTypedHighLimit<std::int32_t>();
											LOG_FATAL() << formatter.str();
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
											% this->GetTypedLowLimit<std::int32_t>();
											LOG_FATAL() << formatter.str();
											return Result(ErrorCode::OBJECT_ACTUAL_VALUE_DECEEDS_LOWLIMIT, formatter.str());
										}
									}
									this->SetActualValue(boost::any(value));
									if (this->GetDefaultValue().empty())
										this->actualValueNotDefaultValue = true;
									else if (this->GetTypedDefaultValue<std::int32_t>() != value)
										this->actualValueNotDefaultValue = true;
									else
										this->actualValueNotDefaultValue = false;

									break;
								}
							case PlkDataType::REAL32:
								{
									float value = boost::lexical_cast<float>(actualValue);
									this->SetActualValue(boost::any(value));
									if (this->GetDefaultValue().empty())
										this->actualValueNotDefaultValue = true;
									else if (this->GetTypedDefaultValue<float>() != value)
										this->actualValueNotDefaultValue = true;
									else
										this->actualValueNotDefaultValue = false;
									break;
								}
							case PlkDataType::REAL64:
								{
									double value = boost::lexical_cast<double>(actualValue);
									this->SetActualValue(boost::any(value));
									if (this->GetDefaultValue().empty())
										this->actualValueNotDefaultValue = true;
									else if (this->GetTypedDefaultValue<double>() != value)
										this->actualValueNotDefaultValue = true;
									else
										this->actualValueNotDefaultValue = false;
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
											% this->GetTypedHighLimit<std::int64_t>();
											LOG_FATAL() << formatter.str();
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
											% this->GetTypedHighLimit<std::int64_t>();
											LOG_FATAL() << formatter.str();
											return Result(ErrorCode::OBJECT_ACTUAL_VALUE_DECEEDS_LOWLIMIT, formatter.str());
										}
									}
									this->SetActualValue(boost::any(value));
									if (this->GetDefaultValue().empty())
										this->actualValueNotDefaultValue = true;
									else if (this->GetTypedDefaultValue<std::int64_t>() != value)
										this->actualValueNotDefaultValue = true;
									else
										this->actualValueNotDefaultValue = false;
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
											% this->GetTypedHighLimit<std::uint64_t>();
											LOG_FATAL() << formatter.str();
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
											% this->GetTypedLowLimit<std::uint64_t>();
											LOG_FATAL() << formatter.str();
											return Result(ErrorCode::OBJECT_ACTUAL_VALUE_DECEEDS_LOWLIMIT, formatter.str());
										}
									}
									if (this->GetDefaultValue().empty())
										this->actualValueNotDefaultValue = true;
									else if (this->GetTypedDefaultValue<std::uint64_t>() != value)
										this->actualValueNotDefaultValue = true;
									else
										this->actualValueNotDefaultValue = false;
									this->SetActualValue(boost::any(value));
									break;
								}
							case PlkDataType::IP_ADDRESS:
							case PlkDataType::NETTIME:
							case PlkDataType::VISIBLE_STRING:
							case PlkDataType::OCTET_STRING:
							case PlkDataType::UNICODE_STRING:
							case PlkDataType::TIME_OF_DAY:
							case PlkDataType::TIME_DIFF:
								{
									this->SetActualValue(boost::any(actualValue));
									if (this->GetDefaultValue().empty())
										this->actualValueNotDefaultValue = true;
									else if (this->GetTypedDefaultValue<std::string>() != actualValue)
										this->actualValueNotDefaultValue = true;
									else
										this->actualValueNotDefaultValue = false;
									break;
								}
							default:
								break;
						}
					}
					catch (const std::exception& e)
					{
						boost::format formatter(kMsgActualValueDatatypeError);
						formatter
						% actualValue
						% (std::uint32_t) GetDataType().get();
						LOG_FATAL() << formatter.str() << e.what();
						return Result(ErrorCode::ACTUAL_VALUE_INVALID, formatter.str());
					}
					return Result();
				}

				Result BaseObject::SetTypedObjectDefaultValue(const std::string& defaultValue)
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
							LOG_FATAL() << formatter.str();
							return Result(ErrorCode::OBJECT_HAS_NO_DATATYPE, formatter.str());
						}

						switch (this->GetDataType().get())
						{
							case PlkDataType::BOOLEAN:
								{
									bool value = StringToBool(defaultValue);
									this->SetDefaultValue(boost::any(value));
									break;
								}
							case PlkDataType::INTEGER8: //Set int16_t datatype to avoid stream problems with char datatypes
							case PlkDataType::INTEGER16:
								{
									int16_t value = HexToInt<std::int16_t>(defaultValue);
									this->SetDefaultValue(boost::any(value));
									break;
								}
							case PlkDataType::INTEGER32:
								{
									std::int32_t value = HexToInt<std::int32_t>(defaultValue);
									this->SetDefaultValue(boost::any(value));
									break;
								}
							case PlkDataType::UNSIGNED8: //Set uint16_t datatype to avoid stream problems with char datatypes
							case PlkDataType::UNSIGNED16:
								{
									uint16_t value = HexToInt<std::uint16_t>(defaultValue);
									this->SetDefaultValue(boost::any(value));
									break;
								}
							case PlkDataType::UNSIGNED32:
								{
									std::uint32_t value = HexToInt<std::uint32_t>(defaultValue);
									this->SetDefaultValue(boost::any(value));
									break;
								}
							case PlkDataType::REAL32:
								{
									float value = boost::lexical_cast<float>(defaultValue);
									this->SetDefaultValue(boost::any(value));
									break;
								}
							case PlkDataType::Domain:
								//Both values must be empty if Domain datatype
								break;
							case PlkDataType::INTEGER24:
								{
									std::int32_t value = HexToInt<std::int32_t>(defaultValue);
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
							case PlkDataType::INTEGER48:
							case PlkDataType::INTEGER56:
							case PlkDataType::INTEGER64:
								{
									int64_t value = HexToInt<std::int64_t>(defaultValue);
									this->SetDefaultValue(boost::any(value));
									break;
								}
							case PlkDataType::UNSIGNED24:
								{
									std::uint32_t value = HexToInt<std::uint32_t>(defaultValue);
									this->SetDefaultValue(boost::any(value));
									break;
								}
							case PlkDataType::MAC_ADDRESS:
							case PlkDataType::UNSIGNED40:
							case PlkDataType::UNSIGNED48:
							case PlkDataType::UNSIGNED56:
							case PlkDataType::UNSIGNED64:
								{
									uint64_t value = HexToInt<std::uint64_t>(defaultValue);
									this->SetDefaultValue(boost::any(value));
									break;
								}
							case PlkDataType::VISIBLE_STRING:
							case PlkDataType::OCTET_STRING:
							case PlkDataType::UNICODE_STRING:
							case PlkDataType::TIME_OF_DAY:
							case PlkDataType::TIME_DIFF:
							case PlkDataType::IP_ADDRESS:
							case PlkDataType::NETTIME:
								{
									this->SetDefaultValue(boost::any(defaultValue));
									break;
								}
							default:
								break;
						}
					}
					catch (const std::exception& e)
					{
						boost::format formatter(kMsgDefaultValueDatatypeError);
						formatter
						% defaultValue
						% (std::uint32_t) GetDataType().get();
						LOG_FATAL() << formatter.str() << e.what();
						return Result(ErrorCode::DEFAULT_VALUE_INVALID, formatter.str());
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
										return this->complexDataType->GetBitSize();
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
					this->complexDataType = parameter;
				}

				bool BaseObject::WriteToConfiguration() const
				{
					if ((this->forceToCDC == true //Object forced
					        || this->actualValueNotDefaultValue == true) //Actual value != default value
					        && (this->GetAccessType() != AccessType::COND //correct AccessType
					            && this->GetAccessType() != AccessType::CONST
					            && this->GetAccessType() != AccessType::RO))
						return true;
					else
						return false;
				}

				bool BaseObject::HasActualValue()
				{
					if (this->GetActualValue().empty())
						return false;
					return true;
				}

				bool BaseObject::HasDefaultValue()
				{
					if (this->GetDefaultValue().empty())
						return false;
					return true;
				}

				void BaseObject::ClearActualValue()
				{
					this->SetActualValue(boost::any());
					this->actualValueNotDefaultValue = false;
				}
			}
		}
	}
}

