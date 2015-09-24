/************************************************************************
\file ManagingNode.h
\brief Implementation of the Class ManagingNode
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
#if !defined MANAGING_NODE_H
#define MANAGING_NODE_H

#include <vector>
#include <memory>
#include <algorithm>

#include <boost/format.hpp>

#include "BaseNode.h"
#include "Constants.h"
#include "DynamicChannel.h"
#include "PlkDataType.h"
#include "Result.h"
#include "LoggingConfiguration.h"
#include "Utilities.h"
#include "Parameter.h"
#include "ArrayDataType.h"
#include "StructDataType.h"
#include "EnumDataType.h"
#include "Direction.h"
#include "ControlledNode.h"

namespace IndustrialNetwork
{
	namespace POWERLINK
	{
		namespace Core
		{
			namespace Node
			{
				/**
				\brief Represents a managing node in the POWERLINK network.
				\author rueckerc, Bernecker+Rainer Industrie Elektronik Ges.m.b.H.
				*/
				class DLLEXPORT ManagingNode : public IndustrialNetwork::POWERLINK::Core::Node::BaseNode
				{
					public:
						ManagingNode(std::uint8_t nodeID = 240, const std::string& nodeName = "");
						virtual ~ManagingNode();

						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result AddNodeAssignment(const NodeAssignment& assign);
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result RemoveNodeAssignment(const NodeAssignment& assign);
						std::uint32_t GetNodeAssignmentValue();

						void AddDynamicChannel(std::shared_ptr<DynamicChannel>& channelRef);
						const std::vector<std::shared_ptr<DynamicChannel>>& GetDynamicChannels();
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result GetDynamicChannel(IndustrialNetwork::POWERLINK::Core::ObjectDictionary::PlkDataType dataType, IndustrialNetwork::POWERLINK::Core::ObjectDictionary::Direction dir, std::shared_ptr<DynamicChannel>& returnChannel);

						std::uint32_t GetConfigurationObjectCount();
						std::uint32_t GetConfigurationObjectSize();

						std::uint16_t GetRmnCount() const;
						void AddRmnId(std::uint16_t nodeId);
						void RemoveRmnId(std::uint16_t nodeId);
						void ClearRmnList();
						const std::vector<std::uint16_t>& GetRmnList();

						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result MapObject(std::uint32_t index, IndustrialNetwork::POWERLINK::Core::ObjectDictionary::Direction dir,  std::uint16_t channelNr = 0, std::uint32_t position = 0, std::uint16_t fromNode = 0, bool updateNrOfEntries = false);
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result MapSubObject(std::uint32_t index, std::uint16_t subindex, IndustrialNetwork::POWERLINK::Core::ObjectDictionary::Direction dir, std::uint16_t channelNr = 0, std::uint32_t position = 0, std::uint16_t fromNode = 0, bool updateNrOfEntries = false);
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result MapAllRxObjects(std::uint16_t channelNr, bool updateNrOfEntries);
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result MapAllTxObjects(std::uint16_t channelNr, bool updateNrOfEntries);

						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result SetMultiplexedCycle(const std::uint8_t nodeID, const std::uint8_t multiplexedCycle);
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result ResetMultiplexedCycle(const std::uint8_t nodeID);

						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result CalculatePReqPayloadLimit();
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result CalculatePResPayloadLimit();

						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result UpdateProcessImage(const std::map<std::uint8_t, std::shared_ptr<BaseNode>>& nodeCollection, IndustrialNetwork::POWERLINK::Core::ObjectDictionary::Direction dir);

						void ClearMappingObjects();

					private:
						std::vector<std::shared_ptr<DynamicChannel>> dynamicChannelList;
						std::vector<std::uint16_t> rmnList;

						bool MultiplexedCycleAlreadyAssigned(std::uint8_t multiplexedCycle);
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result CheckProcessDataMapping(const std::shared_ptr<BaseNode>& node, const std::shared_ptr<IndustrialNetwork::POWERLINK::Core::ObjectDictionary::BaseProcessDataMapping>& mapping, IndustrialNetwork::POWERLINK::Core::ObjectDictionary::Direction dir);
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result UpdateProcessDataMapping(const std::map<uint8_t, std::shared_ptr<BaseNode>>& nodeCollection, IndustrialNetwork::POWERLINK::Core::ObjectDictionary::Direction dir);


				};
			}
		}
	}
}
#endif
