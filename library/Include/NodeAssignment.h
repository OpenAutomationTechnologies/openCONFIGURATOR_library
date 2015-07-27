/************************************************************************
\file NodeAssignment.h
\brief Implementation of the Class NodeAssignment
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
#if !defined NODE_ASSIGNMENT_H
#define NODE_ASSIGNMENT_H

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
				\brief
				\author rueckerc
				*/
				enum class NodeAssignment : std::uint32_t
				{
					NMT_NODEASSIGN_NODE_EXISTS = 0x00000001,
					NMT_NODEASSIGN_NODE_IS_CN = 0x00000002,
					NMT_NODEASSIGN_START_CN = 0x00000004,
					NMT_NODEASSIGN_MANDATORY_CN = 0x00000008,
					NMT_NODEASSIGN_KEEPALIVE = 0x00000010,
					NMT_NODEASSIGN_SWVERSIONCHECK = 0x00000020,
					NMT_NODEASSIGN_SWUPDATE = 0x00000040,
					NMT_NODEASSIGN_ASYNCONLY_NODE = 0x00000100,
					NMT_NODEASSIGN_MULTIPLEXED_CN = 0x00000200,
					NMT_NODEASSIGN_RT1 = 0x00000400,
					NMT_NODEASSIGN_RT2 = 0x00000800,
					NMT_NODEASSIGN_MN_PRES = 0x00001000,
					NMT_NODEASSIGN_PRES_CHAINING = 0x00004000,
					MNT_NODEASSIGN_VALID = 0x80000000
				};

				inline NodeAssignment operator | (NodeAssignment lhs, NodeAssignment rhs)
				{
					return (NodeAssignment)(static_cast<std::underlying_type<NodeAssignment>::type>(lhs) | static_cast<std::underlying_type<NodeAssignment>::type>(rhs));
				}

				inline NodeAssignment& operator |= (NodeAssignment& lhs, NodeAssignment rhs)
				{
					lhs = (NodeAssignment)(static_cast<std::underlying_type<NodeAssignment>::type>(lhs) | static_cast<std::underlying_type<NodeAssignment>::type>(rhs));
					return lhs;
				}
			}
		}
	}
}
#endif
