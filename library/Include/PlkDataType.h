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
					UNDEFINED = 0x0000,
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

				inline const std::string GetPlkDataTypeName(PlkDataType dt)
				{
					switch (dt)
					{
						case PlkDataType::UNDEFINED:
							return "UNDEFINED";
						case PlkDataType::BOOLEAN:
							return "BOOLEAN";
						case PlkDataType::INTEGER8:
							return "INTEGER8";
						case PlkDataType::INTEGER16:
							return "INTEGER16";
						case PlkDataType::INTEGER32:
							return "INTEGER32";
						case PlkDataType::UNSIGNED8:
							return "UNSIGNED8";
						case PlkDataType::UNSIGNED16:
							return "UNSIGNED16";
						case PlkDataType::UNSIGNED32:
							return "UNSIGNED32";
						case PlkDataType::REAL32:
							return "REAL32";
						case PlkDataType::VISIBLE_STRING:
							return "VISIBLE_STRING";
						case PlkDataType::OCTET_STRING:
							return "OCTET_STRING";
						case PlkDataType::UNICODE_STRING:
							return "UNICODE_STRING";
						case PlkDataType::TIME_OF_DAY:
							return "TIME_OF_DAY";
						case PlkDataType::TIME_DIFF:
							return "TIME_DIFF";
						case PlkDataType::Domain:
							return "Domain";
						case PlkDataType::INTEGER24:
							return "INTEGER24";
						case PlkDataType::REAL64:
							return "REAL64";
						case PlkDataType::INTEGER40:
							return "INTEGER40";
						case PlkDataType::INTEGER48:
							return "INTEGER48";
						case PlkDataType::INTEGER56:
							return "INTEGER56";
						case PlkDataType::INTEGER64:
							return "INTEGER64";
						case PlkDataType::UNSIGNED24:
							return "UNSIGNED24";
						case PlkDataType::UNSIGNED40:
							return "UNSIGNED40";
						case PlkDataType::UNSIGNED48:
							return "UNSIGNED48";
						case PlkDataType::UNSIGNED56:
							return "UNSIGNED56";
						case PlkDataType::UNSIGNED64:
							return "UNSIGNED64";
						case PlkDataType::MAC_ADDRESS:
							return "MAC_ADDRESS";
						case PlkDataType::IP_ADDRESS:
							return "IP_ADDRESS";
						case PlkDataType::NETTIME:
							return "NETTIME";
						default:
							return "";
					}
				}
			}
		}
	}
}
#endif