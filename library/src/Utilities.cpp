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

namespace IndustrialNetwork
{
	namespace POWERLINK
	{
		namespace Core
		{
			namespace Utilities
			{

				template <typename T>
				string IntToHex(const T number, const unsigned int padLength, const string& prefix, const string& suffix)
				{
					ostringstream hexStream;
					hexStream << setfill('0')
					          << setw(padLength)
					          << hex
					          << uppercase
					          << number
					          << suffix;
					return (prefix + hexStream.str());
				}
				template string IntToHex<unsigned short>(const unsigned short number, const unsigned int padLength, const string& prefix, const string& suffix);
				template string IntToHex<unsigned int>(const unsigned int number, const unsigned int padLength, const string& prefix, const string& suffix);
				template string IntToHex<unsigned long long>(const unsigned long long number, const unsigned int padLength, const string& prefix, const string& suffix);

				template <typename T>
				T HexToInt(const string& hexString)
				{
					// Strip prefix if necessary
					string valueStr = (hexString.substr(0, 2) == "0x")
					                  ? hexString.substr(2)
					                  : hexString;
					stringstream stream;
					T value = 0;
					stream << hex << valueStr;
					stream >> value;
					return value;
				}

				template unsigned char HexToInt<unsigned char>(const string& hexString);
				template unsigned short HexToInt<unsigned short>(const string& hexString);
				template unsigned int HexToInt<unsigned int>(const string& hexString);
				template unsigned long long HexToInt<unsigned long long>(const string& hexString);

				template signed char HexToInt<signed char>(const string& hexString);
				template short HexToInt<short>(const string& hexString);
				template int HexToInt<int>(const string& hexString);
				template long long HexToInt<long long>(const string& hexString);

				bool StringToBool(string str)
				{
					transform(str.begin(), str.end(), str.begin(), ::tolower);
					istringstream is(str);
					bool b;
					is >> boolalpha >> b;
					return b;
				}
			}
		}
	}
}
