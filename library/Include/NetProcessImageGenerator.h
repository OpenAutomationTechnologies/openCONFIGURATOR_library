/************************************************************************
\file NetProcessImageGenerator.h
\brief Implementation of the Class NetProcessImageGenerator
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
#if !defined NET_PROCESS_IMAGE_GENERATOR_H
#define NET_PROCESS_IMAGE_GENERATOR_H

#include <memory>
#include <string>

#include <boost/date_time.hpp>

#include "ProcessImageGenerator.h"
#include "ManagingNode.h"
#include "Result.h"
#include "IEC_Datatype.h"
#include "Utilities.h"
#include "Constants.h"

namespace IndustrialNetwork
{
	namespace POWERLINK
	{
		namespace Core
		{
			namespace NetworkHandling
			{
				/**
				\brief Calculates the .NET process image for the POWERLINK network
				\author rueckerc, Bernecker+Rainer Industrie Elektronik Ges.m.b.H.
				*/
				class NetProcessImageGenerator : public ProcessImageGenerator
				{
					public:
						static NetProcessImageGenerator& GetInstance();
						virtual ~NetProcessImageGenerator();

						const std::string Generate(std::uint8_t nodeid, std::shared_ptr<IndustrialNetwork::POWERLINK::Core::NetworkHandling::Network> network);

					private:
						NetProcessImageGenerator();
						std::stringstream processImageStream;

						const std::string PrintChannel(const std::string& name, const IndustrialNetwork::POWERLINK::Core::ObjectDictionary::IEC_Datatype& datatype, const std::uint32_t size, const std::uint32_t piOffset, const boost::optional<std::uint32_t>& bitOffset);
						const std::string WriteNETHeader(const std::string& projName, const std::shared_ptr<IndustrialNetwork::POWERLINK::Core::Node::BaseNode>& node);
						const std::string WriteNETOutputSizeHeader(std::uint32_t totalSize);
						const std::string WriteNETInputSizeHeader(std::uint32_t totalSize);
						std::uint32_t WriteNETProcessImage(const IndustrialNetwork::POWERLINK::Core::ObjectDictionary::Direction& dir, const std::shared_ptr<IndustrialNetwork::POWERLINK::Core::Node::BaseNode>& node);
				};
			}
		}
	}
}
#endif
