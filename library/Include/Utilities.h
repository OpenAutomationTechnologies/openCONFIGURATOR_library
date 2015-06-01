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

#include <iostream>
#include <sstream>
#include <algorithm>
#include <cstdint>
#include <iomanip>
#include <string>
#include <cctype>

using namespace std;

namespace IndustrialNetwork
{
	namespace POWERLINK
	{
		namespace Core
		{
			namespace Utilities
			{

				/************************************************************************
				\brief Convert a number to hexadecimal representation.

				\param number    Number to convert to hexadecimal representation.
				\param padLength Pad the hex representation with leading '0's up to padLength. I.e. Converting 10 to hex with padLength == 4 -> 000A.
				\param prefix    Prefix to prepend. I.e. "0x", empty string by default.
				\param suffix    Suffix to append. I.e. "h", empty string by default.

				\return A hexadecimal string representation of number.
				************************************************************************/
				template <typename T>
				string IntToHex(const T number, const unsigned int padLength, const string& prefix = "", const string& suffix = "");

				/************************************************************************
				\brief Convert a string in hex-representation to an integral datatype

				\param hexString	String representing an integral number in hexadecimal representation. Prefix "0x" is optional.

				\return The string converted to an integral datatype.
				************************************************************************/
				template <typename T>
				T HexToInt(const string& hexString);

				bool StringToBool(string str);

			}
		}
	}
}

#endif