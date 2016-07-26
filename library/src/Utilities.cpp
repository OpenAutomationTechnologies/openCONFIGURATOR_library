/************************************************************************
\file Utilities.cpp
\brief	Utility functions used throughout the codebase.
\author RueckerC, Bernecker + Rainer Industrie Elektronik Ges.m.b.H.
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
#include "Utilities.h"

using namespace IndustrialNetwork::POWERLINK::Core::ObjectDictionary;

namespace IndustrialNetwork
{
	namespace POWERLINK
	{
		namespace Core
		{
			namespace Utilities
			{
				template <typename T>
				const std::string IntToHex(const T number, const std::uint32_t padLength, const std::string& prefix, const std::string& suffix)
				{
					std::ostringstream hexStream;
					hexStream << std::setfill('0')
					          << std::setw(padLength)
					          << std::hex
					          << std::uppercase
					          << number
					          << suffix;
					return (prefix + hexStream.str());
				}
				template const std::string IntToHex<std::uint16_t>(const std::uint16_t number, const std::uint32_t padLength, const std::string& prefix, const std::string& suffix);
				template const std::string IntToHex<std::uint32_t>(const std::uint32_t number, const std::uint32_t padLength, const std::string& prefix, const std::string& suffix);
				template const std::string IntToHex<std::uint64_t>(const std::uint64_t number, const std::uint32_t padLength, const std::string& prefix, const std::string& suffix);

				template <typename T>
				T HexToInt(const std::string& hexString)
				{
					std::stringstream stream;
					stream.exceptions(std::ios::failbit);

					T value = 0;
					if (((hexString.substr(0, 2) == "0x")
					        || (hexString.substr(0, 2) == "0X"))
					        && (hexString.find_first_not_of("0123456789abcdefABCDEF", 2) == std::string::npos
					            && hexString.size() > 2))
					{
						// Strip prefix if necessary
						std::string valueStr = (hexString.substr(0, 2) == "0x" || hexString.substr(0, 2) == "0X")
						                       ? hexString.substr(2)
						                       : hexString;

						stream << std::hex << valueStr;
						stream >> value;
					}
					else if ((hexString.find_first_not_of("-0123456789") == std::string::npos))
					{
						stream << std::dec << hexString;
						stream >> value;
					}
					else
						throw std::range_error("Value format incorrect");

					return value;
				}

				template std::uint8_t HexToInt<std::uint8_t>(const std::string& hexString);
				template std::uint16_t HexToInt<std::uint16_t>(const std::string& hexString);
				template std::uint32_t HexToInt<std::uint32_t>(const std::string& hexString);
				template unsigned long HexToInt<unsigned long>(const std::string& hexString);
				template unsigned long long HexToInt<unsigned long long>(const std::string& hexString);

				template std::int8_t HexToInt<std::int8_t>(const std::string& hexString);
				template std::int16_t HexToInt<std::int16_t>(const std::string& hexString);
				template std::int32_t HexToInt<std::int32_t>(const std::string& hexString);
				template long HexToInt<long>(const std::string& hexString);
				template long long HexToInt<long long>(const std::string& hexString);

				bool StringToBool(const std::string& str)
				{
					std::string boolString = str;
					std::transform(boolString.begin(), boolString.end(), boolString.begin(), ::tolower);
					if (boolString == "true" || boolString == "false")
					{
						bool returnValue = false;
						std::istringstream is(boolString);
						is >> std::boolalpha >> returnValue;
						return returnValue;
					}
					else
					{
						std::uint16_t value = Utilities::HexToInt<std::uint16_t>(str);
						if (value == 1)
							return true;
						else
							return false;
					}
				}

				std::uint32_t GetIECDataTypeBitSize(const IEC_Datatype& dataType)
				{
					switch (dataType)
					{
						case IEC_Datatype::UNDEFINED:
						case IEC_Datatype::STRING:
						case IEC_Datatype::WSTRING:
							break;
						case IEC_Datatype::BITSTRING:
						case IEC_Datatype::BOOL:
							{
								return 1; //Bit datatypes
							}
						case IEC_Datatype::BYTE:
						case IEC_Datatype::_CHAR:
						case IEC_Datatype::SINT:
						case IEC_Datatype::USINT:
							{
								return 8; //Byte datatypes
							}
						case IEC_Datatype::WORD:
						case IEC_Datatype::INT:
						case IEC_Datatype::UINT:
							{
								return 16; //2 Byte datatypes
							}
						case IEC_Datatype::DWORD:
						case IEC_Datatype::DINT:
						case IEC_Datatype::UDINT:
						case IEC_Datatype::REAL:
							{
								return 32; //4 Byte datatypes
							}
						case IEC_Datatype::LINT:
						case IEC_Datatype::LWORD:
						case IEC_Datatype::ULINT:
						case IEC_Datatype::LREAL:
							{
								return 64; //8 Byte datatypes
							}
						default:
							break;
					}
					return 0;
				}

				//Convert float into the 32-bit binary encoding into std::hexadecimal (IEEE 754)
				//Adapted from http://www.technical-recipes.com/2012/converting-between-binary-and-decimal-representations-of-ieee-754-floating-point-numbers-in-c/
				std::int32_t FloatToSinglePrecisisionHex(float value)
				{
					union
					{
						float input;
						std::int32_t output;
					} data;

					data.input = value;

					std::bitset<sizeof(float) * CHAR_BIT> bits(data.output);
					std::string outputString = bits.to_string<char, std::char_traits<char>, std::allocator<char>>();

					std::bitset<32> set(outputString);
					std::int32_t hex = set.to_ulong();

					return hex;
				}

				//Convert double into the 64-bit binary encoding into std::hexadecimal (IEEE 754)
				//Adapted from http://www.technical-recipes.com/2012/converting-between-binary-and-decimal-representations-of-ieee-754-floating-point-numbers-in-c/
				std::int64_t DoubleToDoublePrecisisionHex(double value)
				{
					union
					{
						double input;
						std::int64_t output;
					} data;

					data.input = value;

					std::bitset<sizeof(double) * CHAR_BIT> bits(data.output);

					std::string outputString = bits.to_string<char, std::char_traits<char>, std::allocator<char>>();

					std::bitset<64> set(outputString);
					std::int64_t hex = set.to_ullong();

					return hex;
				}


				float SinglePrecisisionHexToFloat(const std::string& hexString)
				{
					union
					{
						float output;
						std::uint32_t input;
					} data;
					data.input = HexToInt<std::uint32_t>(hexString);

					return data.output;
				}

				double DoublePrecisisionHexToDouble(const std::string& hexString)
				{
					union
					{
						double output;
						std::uint64_t input;
					} data;
					data.input = HexToInt<std::uint64_t>(hexString);

					return data.output;
				}


				template <typename T>
				std::string ReverseHex(const T& number, const std::uint32_t width)
				{
					std::vector<std::string> splitString;
					std::stringstream originalStr;
					std::stringstream reverseStr;
					originalStr << std::hex << std::uppercase << std::setw(width) << std::setfill('0') << std::right << number;
					std::string tempStr(originalStr.str());

					for (std::uint32_t start = 0; start < tempStr.size(); start += 2)
					{
						splitString.push_back(tempStr.substr(start, 2));
					}

					for (std::vector<std::string>::reverse_iterator i = splitString.rbegin(); i != splitString.rend(); ++i)
					{
						reverseStr << *i;
					}

					return reverseStr.str();
				}

				template<> std::string ReverseHex<std::string>(const std::string& number, const std::uint32_t)
				{
					std::vector<std::string> splitString;
					std::stringstream reverseStr;

					for (std::uint32_t start = 0; start < number.size(); start += 2)
					{
						splitString.push_back(number.substr(start, 2));
					}

					for (std::vector<std::string>::reverse_iterator i = splitString.rbegin(); i != splitString.rend(); ++i)
					{
						reverseStr << *i;
					}

					return reverseStr.str();
				}

				template std::string ReverseHex<std::uint8_t>(const std::uint8_t& number, const std::uint32_t width);
				template std::string ReverseHex<std::uint16_t>(const std::uint16_t& number, const std::uint32_t width);
				template std::string ReverseHex<std::uint32_t>(const std::uint32_t& number, const std::uint32_t width);
				template std::string ReverseHex<unsigned long>(const unsigned long& number, const std::uint32_t width);
				template std::string ReverseHex<unsigned long long>(const unsigned long long& number, const std::uint32_t width);

				template std::string ReverseHex<std::int8_t>(const std::int8_t& number, const std::uint32_t width);
				template std::string ReverseHex<std::int16_t>(const std::int16_t& number, const std::uint32_t width);
				template std::string ReverseHex<std::int32_t>(const std::int32_t& number, const std::uint32_t width);
				template std::string ReverseHex<long>(const long& number, const std::uint32_t width);
				template std::string ReverseHex<long long>(const long long& number, const std::uint32_t width);



				void ConfigurationToAscii(const std::stringstream& inputConfig, std::vector<std::uint8_t>& output)
				{
					//Lowercase
					std::string hexOutputStr = inputConfig.str();
					std::transform(hexOutputStr.begin(), hexOutputStr.end(), hexOutputStr.begin(), ::tolower);
					//Write ascii character
					for (std::uint32_t i = 0; i < hexOutputStr.size(); i += 2)
					{
						std::uint8_t left = AsciiToHex(hexOutputStr[i]);
						std::uint8_t right = AsciiToHex(hexOutputStr[i + 1]);

						//Pack two 4bit chars in one byte
						std::uint8_t complete = (std::uint8_t)(left << 4) | (right);
						output.push_back(complete);
					}
				}

				std::uint8_t AsciiToHex(std::uint8_t input)
				{
					if (input >= 97 && input <= 102) //Convert lowercase hex numbers
					{
						input = input - 87;
					}
					else if (input >= 48 && input <= 57) //Convert Numbers
					{
						input = input - 48;
					}
					return input;
				}

				IEC_Datatype GetIECDataType(const PlkDataType& dataType)
				{
					switch (dataType)
					{
						case PlkDataType::BOOLEAN:
							return IEC_Datatype::BOOL;
						case PlkDataType::INTEGER8:
							return IEC_Datatype::SINT;
						case PlkDataType::INTEGER16:
							return IEC_Datatype::INT;
						case PlkDataType::INTEGER24:
						case PlkDataType::INTEGER32:
							return IEC_Datatype::DINT;
						case PlkDataType::UNSIGNED8:
							return IEC_Datatype::USINT;
						case PlkDataType::UNSIGNED16:
							return IEC_Datatype::UINT;
						case PlkDataType::UNSIGNED32:
							return IEC_Datatype::UDINT;
						case PlkDataType::REAL32:
							return IEC_Datatype::REAL;
						case PlkDataType::REAL64:
							return IEC_Datatype::LREAL;
						case PlkDataType::INTEGER40:
						case PlkDataType::INTEGER48:
						case PlkDataType::INTEGER56:
						case PlkDataType::INTEGER64:
							return IEC_Datatype::LINT;
						case PlkDataType::UNSIGNED24:
						case PlkDataType::UNSIGNED40:
						case PlkDataType::UNSIGNED48:
						case PlkDataType::UNSIGNED56:
						case PlkDataType::UNSIGNED64:
							return IEC_Datatype::ULINT;
						case PlkDataType::UNDEFINED:
						case PlkDataType::VISIBLE_STRING:
						case PlkDataType::OCTET_STRING:
						case PlkDataType::UNICODE_STRING:
						case PlkDataType::TIME_OF_DAY:
						case PlkDataType::TIME_DIFF:
						case PlkDataType::MAC_ADDRESS:
						case PlkDataType::IP_ADDRESS:
						case PlkDataType::NETTIME:
						case PlkDataType::Domain:
						default:
							return IEC_Datatype::UNDEFINED; //error
					}
				}

				PlkDataType GetPlkDataType(const IEC_Datatype& dataType)
				{
					switch (dataType)
					{
						case IEC_Datatype::UNDEFINED:
							return PlkDataType::UNDEFINED;
						case IEC_Datatype::BITSTRING:
						case IEC_Datatype::BOOL:
						case IEC_Datatype::BYTE:
						case IEC_Datatype::_CHAR:
						case IEC_Datatype::USINT:
							return PlkDataType::UNSIGNED8;
						case IEC_Datatype::WORD:
						case IEC_Datatype::UINT:
							return PlkDataType::UNSIGNED16;
						case IEC_Datatype::DWORD:
						case IEC_Datatype::UDINT:
							return PlkDataType::UNSIGNED32;
						case IEC_Datatype::LWORD:
						case IEC_Datatype::ULINT:
							return PlkDataType::UNSIGNED64;
						case IEC_Datatype::SINT:
							return PlkDataType::INTEGER8;
						case IEC_Datatype::INT:
							return PlkDataType::INTEGER16;
						case IEC_Datatype::DINT:
							return PlkDataType::INTEGER32;
						case IEC_Datatype::LINT:
							return PlkDataType::INTEGER64;
						case IEC_Datatype::REAL:
							return PlkDataType::REAL32;
						case IEC_Datatype::LREAL:
							return PlkDataType::REAL64;
						case IEC_Datatype::STRING:
						case IEC_Datatype::WSTRING:
							return PlkDataType::UNDEFINED;
						default:
							break;
					}
					return PlkDataType::UNDEFINED;
				}

				const std::string GetNetDatatypeFromIEC(const IEC_Datatype& iecDataType)
				{
					switch (iecDataType)
					{
						case IEC_Datatype::BITSTRING:
						case IEC_Datatype::BOOL:
						case IEC_Datatype::BYTE:
						case IEC_Datatype::_CHAR:
						case IEC_Datatype::USINT:
							return std::string("byte");
						case IEC_Datatype::DWORD:
						case IEC_Datatype::UDINT:
							return std::string("UInt32");
						case IEC_Datatype::LWORD:
						case IEC_Datatype::ULINT:
							return std::string("UInt64");
						case IEC_Datatype::SINT:
							return std::string("sbyte");
						case IEC_Datatype::INT:
							return std::string("Int16");
						case IEC_Datatype::DINT:
							return std::string("Int32");
						case IEC_Datatype::LINT:
							return std::string("Int64");
						case IEC_Datatype::UINT:
						case IEC_Datatype::WORD:
							return std::string("UInt16");
						case IEC_Datatype::REAL:
							return std::string("float");
						case IEC_Datatype::LREAL:
							return std::string("double");
						case IEC_Datatype::STRING:
						case IEC_Datatype::WSTRING:
						case IEC_Datatype::UNDEFINED:
						default:
							return "";
					}
				}

				bool IsIPAddress(const std::string& ipaddr)
				{
					if (ipaddr.substr(0, 2) == "0x")
					{
						if (ipaddr.size() != 10)
							return false;
						return true;
					}
					else
					{
						std::vector<std::string> ipAddressParts;
						boost::split(ipAddressParts, ipaddr, boost::is_any_of("."));

						if (ipAddressParts.size() != 4)
							return false;
						for (auto& part : ipAddressParts)
						{
							std::int32_t numb;
							std::istringstream(part) >> numb;
							if ((numb < 0) || (numb > 255))
								return false;
						}
						return true;
					}
				}

				AccessType GetAccessTypeFromParameterAccessType(const ParameterAccess& paramAccess)
				{
					switch (paramAccess)
					{
						case ParameterAccess::constant:
							return AccessType::CONST;
						case ParameterAccess::read:
							return AccessType::RO;
						case ParameterAccess::write:
							return AccessType::WO;
						case ParameterAccess::readWrite:
						case ParameterAccess::readWriteInput:
						case ParameterAccess::readWriteOutput:
							return AccessType::RW;
						case ParameterAccess::noAccess:
						case ParameterAccess::undefined:
							return AccessType::UNDEFINED;
						default:
							break;
					}
					return AccessType::UNDEFINED;
				}

				std::string ClearModuleParameterUuid(const std::string& parameterName)
				{
					std::string cleanedName = parameterName;
					std::vector<std::string> strs;
					boost::split(strs, parameterName, boost::is_any_of("_"));
					if (IsValidUuid(strs.back()))
					{
						strs.pop_back();
						cleanedName = boost::algorithm::join(strs, "_");
					}
					return cleanedName;
				}

				bool IsValidUuid(const std::string& maybe_uuid)
				{
					try
					{
						boost::uuids::uuid result = boost::uuids::string_generator()(maybe_uuid);
						return result.version() != boost::uuids::uuid::version_unknown;
					}
					catch (const boost::exception&)
					{
						return false;
					}
				}
			}
		}
	}
}
