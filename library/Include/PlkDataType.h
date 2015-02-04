/************************************************************************
\file PlkDataType.h
\brief Implementation of the Enumeration PlkDataType
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
#if !defined PLK_DATATYPE_H
#define PLK_DATATYPE_H

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
				\brief Powerlink Data Types
				See EPSG DS 301, v1.1.0, v1.2.0
				\author rueckerc
				*/
				enum class PlkDataType : std::uint32_t
				{
					BOOLEAN = 0x0001,
					INTEGER8 = 0x0002,
					INTEGER16 = 0x0003,
					INTEGER32 = 0x0004,
					UNSIGNED8 = 0x0005,
					UNSIGNED16 = 0x0006,
					UNSIGNED32 = 0x0007,
					REAL32 = 0x0008,
					VISIBLE_STRING = 0x0009,
					OCTET_STRING = 0x000A,
					UNICODE_STRING = 0x000B,
					TIME_OF_DAY = 0x000C,
					TIME_DIFF = 0x000D,
					Domain = 0x000F,
					INTEGER24 = 0x0010,
					REAL64 = 0x0011,
					INTEGER40 = 0x0012,
					INTEGER48 = 0x0013,
					INTEGER56 = 0x0014,
					INTEGER64 = 0x0015,
					UNSIGNED24 = 0x0016,
					UNSIGNED40 = 0x0018,
					UNSIGNED48 = 0x0019,
					UNSIGNED56 = 0x001A,
					UNSIGNED64 = 0x001B,
					MAC_ADDRESS = 0x0401,
					IP_ADDRESS = 0x0402,
					NETTIME = 0x0403
				};

			}

		}

	}

}
#endif