/************************************************************************
\file FeatureFlags.h
\brief Implementation of the Class FeatureFlags
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
#if !defined FEATURE_FLAGS_H
#define FEATURE_FLAGS_H

#include <cstdint>
#include <type_traits>

namespace IndustrialNetwork
{
	namespace POWERLINK
	{
		namespace Core
		{
			namespace Node
			{
				/**
				\brief Represents a feature flag of a POWERLINK node (0x1F82).
				\author rueckerc, Bernecker+Rainer Industrie Elektronik Ges.m.b.H.
				*/
				enum class FeatureFlags : std::uint32_t
				{
					Isochronous = 0x0,
					SDO_by_UDP_IP = 0x1,
					SDO_by_ASnd = 0x2,
					SDO_by_PDO = 0x4,
					NMT_Info_Services = 0x8,
					Extended_NMT_State_Commands = 0x10,
					Dynamic_PDO_Mapping = 0x20,
					NMT_Services_by_UDP_IP = 0x40,
					Configuration_Manager = 0x80,
					Multiplexed_Access = 0x100,
					Node_ID_setup_by_SW = 0x200,
					MN_Basic_Ethernet_Mode = 0x400,
					Routing_Type_1_Support = 0x800,
					Routing_Type_2_Support = 0x1000,
					SDO_Read_Write_All_by_Index = 0x2000,
					SDO_Read_Write_Multiple_Parameter_by_Index = 0x4000,
					Multiple_ASend_Support = 0x8000,
					Ring_Redundancy = 0x10000,
					PResChaining = 0x20000,
					Multiple_PReq_PRes = 0x40000,
					Dynamic_Node_Allocation = 0x80000
				};

				inline FeatureFlags operator | (FeatureFlags lhs, FeatureFlags rhs)
				{
					return (FeatureFlags)(static_cast<std::underlying_type<FeatureFlags>::type>(lhs) | static_cast<std::underlying_type<FeatureFlags>::type>(rhs));
				}

				inline FeatureFlags& operator |= (FeatureFlags& lhs, FeatureFlags rhs)
				{
					lhs = (FeatureFlags)(static_cast<std::underlying_type<FeatureFlags>::type>(lhs) | static_cast<std::underlying_type<FeatureFlags>::type>(rhs));
					return lhs;
				}
			}
		}
	}
}
#endif
