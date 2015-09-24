/************************************************************************
\file DynamicChannel.h
\brief Implementation of the Class DynamicChannel
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
#if !defined DYNAMIC_CHANNEL_H
#define DYNAMIC_CHANNEL_H

#include <string>
#include <cstdint>

#include <boost/optional.hpp>

#include "DynamicChannelAccessType.h"
#include "PlkDataType.h"
#include "Constants.h"

namespace IndustrialNetwork
{
	namespace POWERLINK
	{
		namespace Core
		{
			namespace Node
			{
				/**
				\brief Represents a dynamic channel of a POWERLINK node. (EPSG DS 301 1.2.1 chapter 6.8.2)
				\author rueckerc, Bernecker+Rainer Industrie Elektronik Ges.m.b.H.
				*/
				class DLLEXPORT DynamicChannel
				{

					public:
						DynamicChannel(IndustrialNetwork::POWERLINK::Core::ObjectDictionary::PlkDataType dataType, IndustrialNetwork::POWERLINK::Core::ObjectDictionary::DynamicChannelAccessType accessType, std::uint32_t startIndex, std::uint32_t endIndex, std::uint32_t maxNumber, std::uint32_t addressOffset, std::uint8_t bitAlignment = 0);
						virtual ~DynamicChannel();

						const IndustrialNetwork::POWERLINK::Core::ObjectDictionary::PlkDataType& GetDataType() const;
						const IndustrialNetwork::POWERLINK::Core::ObjectDictionary::DynamicChannelAccessType& GetAccessType() const;
						std::uint32_t GetStartIndex() const;
						std::uint32_t GetEndIndex() const;
						std::uint32_t GetMaxNumber() const;
						std::uint32_t GetAddressOffset() const;
						const boost::optional<std::uint32_t>& GetBitAlignment() const;

					private:
						IndustrialNetwork::POWERLINK::Core::ObjectDictionary::PlkDataType dataType;
						IndustrialNetwork::POWERLINK::Core::ObjectDictionary::DynamicChannelAccessType accessType;
						std::uint32_t startIndex;
						std::uint32_t endIndex;
						std::uint32_t maxNumber;
						std::uint32_t addressOffset;
						boost::optional<std::uint32_t> bitAlignment;
				};
			}
		}
	}
}
#endif
