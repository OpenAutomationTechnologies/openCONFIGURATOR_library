/************************************************************************
\file IEC_Datatype.h
\brief Implementation of the Enumeration IEC_Datatype
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
#if !defined IEC_DATATYPE_H
#define IEC_DATATYPE_H

#include <cstdint>

namespace IndustrialNetwork
{
	namespace POWERLINK
	{
		namespace Core
		{
			namespace ObjectDictionary
			{
				/**
				\brief Represents an IEC data type.
				\author rueckerc, Bernecker+Rainer Industrie Elektronik Ges.m.b.H.
				*/
				enum class IEC_Datatype : std::int8_t
				{
					UNDEFINED = -1, //!< Undefined
					BITSTRING = 0, //!< Bit string (1 bit)
					BOOL, //!< Bool (1 bit)
					BYTE, //!< Byte (8 bit)
					_CHAR, //!< Char (8 bit)
					WORD, //!< Word (16 bit)
					DWORD, //!< Dword (32 bit)
					LWORD, //!< Lword (64 bit)
					SINT, //!< Signed short integer (1 byte)
					INT, //!< Signed integer (2 byte)
					DINT, //!< Double integer (4 byte)
					LINT, //!< Long integer (8 byte)
					USINT, //!< Unsigned short integer (1 byte)
					UINT, //!< Unsigned integer (2 byte)
					UDINT, //!< Unsigned double integer (4 byte)
					ULINT, //!< Unsigned long integer (8 byte)
					REAL, //!< REAL (4 byte)
					LREAL, //!< LREAL (8 byte)
					STRING, //!< STRING
					WSTRING //!< WSTRING to hold multi byte string
				};

				static const std::string IECDatatypeValues[] =
				{
					"BITSTRING",
					"BOOL",
					"BYTE",
					"CHAR",
					"WORD",
					"DWORD",
					"LWORD",
					"SINT",
					"INT",
					"DINT",
					"LINT",
					"USINT",
					"UINT",
					"UDINT",
					"ULINT",
					"REAL",
					"LREAL",
					"STRING",
					"WSTRING"
				};
			}
		}
	}
}
#endif
