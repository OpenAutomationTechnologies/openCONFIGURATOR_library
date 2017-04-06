/************************************************************************
\file ManagingNodeMappingBuilder.h
\brief Implementation of the Class ManagingNodeMappingBuilder
\author rueckerc, Bernecker+Rainer Industrie Elektronik Ges.m.b.H.
\date 28-Apr-2015 14:40:00
************************************************************************/

/*------------------------------------------------------------------------------
Copyright (c) 2014, Bernecker+Rainer Industrie-Elektronik Ges.m.b.H. (B&R)
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
#if !defined MANAGING_NODE_MAPPING_BUILDER_H
#define MANAGING_NODE_MAPPING_BUILDER_H

#include <map>
#include <cstdint>
#include <memory>
#include <sstream>

#include "BuildConfigurationSettingBuilder.h"
#include "Result.h"
#include "ErrorCode.h"
#include "Constants.h"
#include "LoggingConfiguration.h"
#include "BaseNode.h"
#include "ManagingNode.h"
#include "ControlledNode.h"
#include "Direction.h"
#include "Utilities.h"

namespace IndustrialNetwork
{
	namespace POWERLINK
	{
		namespace Core
		{
			namespace Configuration
			{
				/**
				\brief Builds the managing node mapping.
				\author rueckerc, Bernecker+Rainer Industrie Elektronik Ges.m.b.H.
				*/
				class ManagingNodeMappingBuilder : public IndustrialNetwork::POWERLINK::Core::Configuration::BuildConfigurationSettingBuilder
				{
					public:
						ManagingNodeMappingBuilder();
						virtual ~ManagingNodeMappingBuilder();

						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result GenerateConfiguration(const std::string& value, const std::map<std::uint8_t, std::shared_ptr<IndustrialNetwork::POWERLINK::Core::Node::BaseNode>>& nodeCollection);

					private:
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result GenerateMnMapping(const std::string& value, IndustrialNetwork::POWERLINK::Core::ObjectDictionary::Direction dir, const std::map<std::uint8_t, std::shared_ptr<IndustrialNetwork::POWERLINK::Core::Node::BaseNode>>& nodeCollection);
						const std::string GetMappingString(std::uint32_t bitSize, std::uint32_t bitOffset, std::uint32_t subindex, std::uint32_t index);
						bool GenerateForNode(const std::string& value, std::uint16_t nodeId);
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result FindMappedObject(const std::shared_ptr<IndustrialNetwork::POWERLINK::Core::Node::BaseNode>& node, std::uint32_t dataIndex, std::uint16_t dataSubindex, std::shared_ptr<IndustrialNetwork::POWERLINK::Core::ObjectDictionary::BaseObject>& foundObject);
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result WriteMappingToForNode(std::uint16_t nodeId, IndustrialNetwork::POWERLINK::Core::ObjectDictionary::Direction dir, const std::string& actualMappingValue, const std::shared_ptr<IndustrialNetwork::POWERLINK::Core::Node::ManagingNode>& mn);
						std::uint32_t CalculateOffset(IndustrialNetwork::POWERLINK::Core::ObjectDictionary::PlkDataType dataType, IndustrialNetwork::POWERLINK::Core::ObjectDictionary::Direction dir);

						std::shared_ptr<std::map<IndustrialNetwork::POWERLINK::Core::ObjectDictionary::PlkDataType, std::uint32_t>> inputOffsets;
						std::shared_ptr<std::map<IndustrialNetwork::POWERLINK::Core::ObjectDictionary::PlkDataType, std::uint32_t>> outputOffsets;
				};
			}
		}
	}
}
#endif
