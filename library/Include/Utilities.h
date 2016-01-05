/************************************************************************
\file Utilities.h
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
#ifndef UTILITIES_H
#define UTILITIES_H

#include <sstream>
#include <algorithm>
#include <cstdint>
#include <iomanip>
#include <string>
#include <limits.h>
#include <math.h>
#include <bitset>
#include <vector>

#include <boost/algorithm/string.hpp>

#include "IEC_Datatype.h"
#include "PlkDataType.h"
#include "AccessType.h"
#include "ParameterAccess.h"

namespace IndustrialNetwork
{
	namespace POWERLINK
	{
		namespace Core
		{
			/**
			\brief %Utilities related namespace
			\author rueckerc, Bernecker+Rainer Industrie Elektronik Ges.m.b.H.
			*/
			namespace Utilities
			{

				/************************************************************************
				\brief Convert a number to std::hexadecimal representation.

				\param number    Number to convert to std::hexadecimal representation.
				\param padLength Pad the std::hex representation with leading '0's up to padLength. I.e. Converting 10 to std::hex with padLength == 4 -> 000A.
				\param prefix    Prefix to prepend. I.e. "0x", empty std::string by default.
				\param suffix    Suffix to append. I.e. "h", empty std::string by default.

				\return A std::hexadecimal std::string representation of number.
				************************************************************************/
				template <typename T>
				const std::string IntToHex(const T number, const std::uint32_t padLength, const std::string& prefix = "", const std::string& suffix = "");

				/************************************************************************
				\brief Convert a std::string in std::hex-representation to an integral datatype

				\param std::hexString	String representing an integral number in std::hexadecimal representation. Prefix "0x" is optional.

				\return The std::string converted to an integral datatype.
				************************************************************************/
				template <typename T>
				T HexToInt(const std::string& hexString);

				bool StringToBool(const std::string& str);

				std::uint32_t GetIECDataTypeBitSize(const IndustrialNetwork::POWERLINK::Core::ObjectDictionary::IEC_Datatype dataType);

				IndustrialNetwork::POWERLINK::Core::ObjectDictionary::IEC_Datatype GetIECDataType(const IndustrialNetwork::POWERLINK::Core::ObjectDictionary::PlkDataType dataType);
				IndustrialNetwork::POWERLINK::Core::ObjectDictionary::PlkDataType GetPlkDataType(const IndustrialNetwork::POWERLINK::Core::ObjectDictionary::IEC_Datatype dataType);

				int32_t FloatToSinglePrecisisionHex(float value);
				int64_t DoubleToDoublePrecisisionHex(double value);
				float SinglePrecisisionHexToFloat(const std::string& hexString);
				double DoublePrecisisionHexToDouble(const std::string& hexString);

				template <typename T>
				std::string ReverseHex(const T& number, const std::uint32_t width = 0);

				std::uint8_t AsciiToHex(std::uint8_t input);
				void ConfigurationToAscii(const std::stringstream& inputConfig, std::vector<std::uint8_t>& output);
				const std::string GetNetDatatypeFromIEC(IndustrialNetwork::POWERLINK::Core::ObjectDictionary::IEC_Datatype iecDataType);
				IndustrialNetwork::POWERLINK::Core::ObjectDictionary::AccessType GetAccessTypeFromParameterAccessType(IndustrialNetwork::POWERLINK::Core::ObjectDictionary::ParameterAccess paramAccess);
				bool IsIPAddress(const std::string& ipaddr);
			}
		}
	}
}

#endif