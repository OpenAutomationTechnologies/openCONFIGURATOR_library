/************************************************************************
\file BaseProcessImageObject.h
\brief Implementation of the Class BaseProcessImageObject
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
#if !defined BASE_PROCESS_IMAGE_OBJECT_H
#define BASE_PROCESS_IMAGE_OBJECT_H

#include <cstdint>
#include <string>
#include <vector>

#include <boost/optional.hpp>

#include "Constants.h"
#include "Utilities.h"
#include "IEC_Datatype.h"

namespace IndustrialNetwork
{
	namespace POWERLINK
	{
		namespace Core
		{
			namespace ObjectDictionary
			{
				/**
				\brief Represents a singly entry in the process image of a POWERLINK node.
				\author rueckerc, Bernecker+Rainer Industrie Elektronik Ges.m.b.H.
				*/
				class DLLEXPORT BaseProcessImageObject
				{
					public:
						BaseProcessImageObject(const std::string& name, IEC_Datatype dataType, std::uint32_t piOffset, std::uint32_t size);
						BaseProcessImageObject(const std::string& name, IEC_Datatype dataType, std::uint32_t piOffset, std::uint32_t bitOffset, std::uint32_t size);
						virtual ~BaseProcessImageObject();

						const std::string& GetName() const;
						void SetName(const std::string& name);
						const IEC_Datatype& GetDataType() const;
						std::uint32_t GetSize() const;
						std::uint32_t GetPiOffset() const;
						void SetPIOffset(std::uint32_t piOffset);
						const boost::optional<std::uint32_t>& GetBitOffset() const;

					private:
						std::string name;
						IEC_Datatype dataType;
						std::uint32_t size;
						std::uint32_t piOffset;
						boost::optional<std::uint32_t> bitOffset;
				};
			}
		}
	}
}
#endif
