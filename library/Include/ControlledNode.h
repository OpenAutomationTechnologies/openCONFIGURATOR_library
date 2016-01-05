/************************************************************************
\file ControlledNode.h
\brief Implementation of the Class ControlledNode
\author rueckerc, Bernecker+Rainer Industrie Elektronik Ges.m.b.H.
\date 04-Feb-2015 09:38:08
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
#if !defined CONTROLLED_NODE_H
#define CONTROLLED_NODE_H

#include <boost/format.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/range/algorithm_ext.hpp>

#include "BaseNode.h"
#include "BaseObject.h"
#include "PlkOperationMode.h"
#include "BaseProcessDataMapping.h"
#include "Result.h"
#include "BaseObject.h"
#include "Constants.h"
#include "Direction.h"
#include "Module.h"
#include "Range.h"
#include "Constants.h"
#include "LoggingConfiguration.h"
#include "Utilities.h"
#include "Parameter.h"
#include "ArrayDataType.h"
#include "StructDataType.h"
#include "EnumDataType.h"
#include "Direction.h"

namespace IndustrialNetwork
{
	namespace POWERLINK
	{
		namespace Core
		{
			namespace Node
			{
				/**
				\brief Representes the Controlled Node in the POWERLINK network.
				\author rueckerc, Bernecker+Rainer Industrie Elektronik Ges.m.b.H.
				*/
				class DLLEXPORT ControlledNode : public IndustrialNetwork::POWERLINK::Core::Node::BaseNode
				{

					public:
						ControlledNode(std::uint8_t nodeID, const std::string& nodeName = "");
						virtual ~ControlledNode();

						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result AddNodeAssignment(NodeAssignment);
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result RemoveNodeAssignment(NodeAssignment);
						std::uint32_t GetNodeAssignmentValue();

						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result SetOperationMode(IndustrialNetwork::POWERLINK::Core::Node::PlkOperationMode operationMode);
						IndustrialNetwork::POWERLINK::Core::Node::PlkOperationMode GetOperationMode();

						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result MapObject(std::uint32_t index, const IndustrialNetwork::POWERLINK::Core::ObjectDictionary::Direction dir, std::uint16_t channelNr = 0, std::uint32_t position = 0, std::uint16_t fromNode = 0, bool updateNrOfEntries = false);
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result MapSubObject(std::uint32_t index, std::uint16_t subindex, const IndustrialNetwork::POWERLINK::Core::ObjectDictionary::Direction dir,  std::uint16_t channelNr = 0, uint32_t position = 0, std::uint16_t fromNode = 0, bool updateNrOfEntries = false);
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result MapAllRxObjects(std::uint16_t channelNr, bool updateNrOfEntries);
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result MapAllTxObjects(std::uint16_t channelNr, bool updateNrOfEntries);
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result MoveMappingObject(const IndustrialNetwork::POWERLINK::Core::ObjectDictionary::Direction dir, std::uint16_t channelNr, std::uint16_t oldPosition, std::uint16_t newPosition);

						std::uint32_t GetConfigurationObjectCount();
						std::uint32_t GetConfigurationObjectSize();

						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result CalculatePReqPayloadLimit();
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result CalculatePResPayloadLimit();

						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result UpdateProcessImage(IndustrialNetwork::POWERLINK::Core::ObjectDictionary::Direction dir);

						void SetNodeDataPresMnOffset(std::uint32_t offset);
						std::uint32_t GetNodeDataPresMnOffset();
						std::uint32_t GetNodeDataPresMnCurrentOffset();

						bool ReceivesPResMN();

					private:
						IndustrialNetwork::POWERLINK::Core::Node::PlkOperationMode operationMode;

						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result GetDataObjectFromMapping(const std::shared_ptr<IndustrialNetwork::POWERLINK::Core::ObjectDictionary::BaseProcessDataMapping>& mapping, std::shared_ptr<IndustrialNetwork::POWERLINK::Core::ObjectDictionary::BaseObject>& dataObject, std::string& retName);
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result UpdateProcessDataMapping(IndustrialNetwork::POWERLINK::Core::ObjectDictionary::Direction dir);
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result CheckProcessDataMapping(const std::shared_ptr<IndustrialNetwork::POWERLINK::Core::ObjectDictionary::BaseProcessDataMapping>& mapping, std::uint32_t expectedOffset, IndustrialNetwork::POWERLINK::Core::ObjectDictionary::Direction dir);
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result MapBaseObject(const std::shared_ptr<IndustrialNetwork::POWERLINK::Core::ObjectDictionary::BaseObject>& obj, std::uint32_t index, std::uint16_t subindex, const IndustrialNetwork::POWERLINK::Core::ObjectDictionary::Direction dir, bool updateNrOfEntries, std::uint16_t channelNr = 0, std::uint32_t position = 0, std::uint16_t fromNode = 0);
						void ProcessComplexDatatype(const std::shared_ptr<IndustrialNetwork::POWERLINK::Core::ObjectDictionary::Parameter>& obj, const std::string& dataName, IndustrialNetwork::POWERLINK::Core::ObjectDictionary::Direction dir, std::uint32_t& piOffset, std::uint32_t& bitOffset, std::uint32_t domainCount);
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result ProcessParameterGroup(const std::shared_ptr<IndustrialNetwork::POWERLINK::Core::ObjectDictionary::ParameterGroup>& grp, const std::string& dataName, IndustrialNetwork::POWERLINK::Core::ObjectDictionary::Direction dir, std::uint32_t& piOffset, std::uint32_t domainCount);
						void ProcessComplexDatatype(const std::shared_ptr<IndustrialNetwork::POWERLINK::Core::ObjectDictionary::StructDataType>& obj, const std::string& dataName, IndustrialNetwork::POWERLINK::Core::ObjectDictionary::Direction dir, std::uint32_t& piOffset, std::uint32_t domainCount);
						void ProcessComplexDatatype(const std::shared_ptr<IndustrialNetwork::POWERLINK::Core::ObjectDictionary::ArrayDataType>& obj, const std::string& dataName, IndustrialNetwork::POWERLINK::Core::ObjectDictionary::Direction dir, std::uint32_t& piOffset, std::uint32_t domainCount);
						//void ProcessComplexDatatype(const std::shared_ptr<IndustrialNetwork::POWERLINK::Core::ObjectDictionary::EnumDataType>& obj, const std::string& dataName, IndustrialNetwork::POWERLINK::Core::ObjectDictionary::Direction dir, std::uint32_t& piOffset, std::uint32_t domainCount);
						std::uint32_t nodeDataPresMnOffset;
						std::uint32_t nodeDataPresMnCurrentOffset;
						bool receivesPResMN;

				};
			}
		}
	}
}
#endif
