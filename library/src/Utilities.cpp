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
				std::string IntToHex(const T number, const unsigned int padLength, const std::string& prefix, const std::string& suffix)
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
				template std::string IntToHex<unsigned short>(const unsigned short number, const unsigned int padLength, const std::string& prefix, const std::string& suffix);
				template std::string IntToHex<unsigned int>(const unsigned int number, const unsigned int padLength, const std::string& prefix, const std::string& suffix);
				template std::string IntToHex<unsigned long long>(const unsigned long long number, const unsigned int padLength, const std::string& prefix, const std::string& suffix);

				template <typename T>
				T HexToInt(const std::string& hexString)
				{
					std::stringstream stream;
					stream.exceptions(std::ios::failbit); 

					T value = 0;
					if (hexString.substr(0, 2) == "0x")
					{
						// Strip prefix if necessary
						std::string valueStr = (hexString.substr(0, 2) == "0x")
						                       ? hexString.substr(2)
						                       : hexString;

						stream << std::hex << valueStr;
						stream >> value;
					}
					else
					{
						stream << std::dec << hexString;
						stream >> value;
					}
					return value;
				}

				template std::uint8_t HexToInt<std::uint8_t >(const std::string& hexString);
				template std::uint16_t  HexToInt<uint16_t>(const std::string& hexString);
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
					std::istringstream is(boolString);
					bool b;
					is >> std::boolalpha >> b;
					return b;
				}

				uint32_t GetIECDataTypeBitSize(const IEC_Datatype dataType)
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
				int32_t FloatToSinglePrecisisionHex(float value)
				{
					union
					{
						float input;
						int   output;
					} data;

					data.input = value;

					std::bitset<sizeof(float) * CHAR_BIT>   bits(data.output);
					std::string outputString = bits.to_string<char, std::char_traits<char>, std::allocator<char>>();

					std::bitset<32> set(outputString);
					int32_t hex = set.to_ulong();

					return hex;
				}

				//Convert double into the 64-bit binary encoding into std::hexadecimal (IEEE 754)
				//Adapted from http://www.technical-recipes.com/2012/converting-between-binary-and-decimal-representations-of-ieee-754-floating-point-numbers-in-c/
				int64_t DoubleToDoublePrecisisionHex(double value)
				{
					union
					{
						double input;
						long long output;
					} data;

					data.input = value;

					std::bitset<sizeof(double) * CHAR_BIT>   bits(data.output);

					std::string outputString = bits.to_string<char, std::char_traits<char>, std::allocator<char>>();

					std::bitset<64> set(outputString);
					int64_t hex = set.to_ullong();

					return hex;
				}

				template <typename T>
				std::string ReverseHex(const T& number)
				{
					std::vector<std::string> splitString;
					std::stringstream originalStr;
					std::stringstream reverseStr;
					originalStr << std::hex << std::uppercase << number;
					std::string tempStr(originalStr.str());

					for (uint32_t start = 0; start < tempStr.size(); start += 2)
					{
						splitString.push_back(tempStr.substr(start, start + 2));
					}

					for (std::vector<std::string>::reverse_iterator i = splitString.rbegin();
					        i != splitString.rend(); ++i)
					{
						reverseStr << *i;
					}

					return reverseStr.str();
				}

				template<> std::string ReverseHex<std::string>(const std::string& number)
				{
					std::vector<std::string> splitString;
					std::stringstream reverseStr;

					for (uint32_t start = 0; start < number.size(); start += 2)
					{
						splitString.push_back(number.substr(start, start + 2));
					}

					for (std::vector<std::string>::reverse_iterator i = splitString.rbegin();
					        i != splitString.rend(); ++i)
					{
						reverseStr << *i;
					}

					return reverseStr.str();
				}

				template std::string ReverseHex<std::uint8_t>(const std::uint8_t& number);
				template std::string ReverseHex<std::uint16_t>(const std::uint16_t& number);
				template std::string ReverseHex<std::uint32_t>(const std::uint32_t& number);
				template std::string ReverseHex<unsigned long>(const unsigned long& number);
				template std::string ReverseHex<unsigned long long>(const unsigned long long& number);

				template std::string ReverseHex<std::int8_t>(const std::int8_t& number);
				template std::string ReverseHex<std::int16_t>(const std::int16_t& number);
				template std::string ReverseHex<std::int32_t>(const std::int32_t& number);
				template std::string ReverseHex<long>(const long& number);
				template std::string ReverseHex<long long>(const long long& number);
			}
		}
	}
}
