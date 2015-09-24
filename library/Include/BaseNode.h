/************************************************************************
\file BaseNode.h
\brief Implementation of the Class BaseNode
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
#if !defined BASE_NODE_H
#define BASE_NODE_H

#include <vector>
#include <cstdint>
#include <memory>

#include <boost/filesystem.hpp>
#include <boost/format.hpp>

#include "Constants.h"
#include "ApplicationProcess.h"
#include "NetworkManagement.h"
#include "DynamicChannel.h"
#include "Object.h"
#include "IFieldbusNode.h"
#include "BaseProcessImageObject.h"
#include "BaseProcessDataMapping.h"
#include "NodeAssignment.h"
#include "Result.h"
#include "LoggingConfiguration.h"
#include "Utilities.h"
#include "Direction.h"

namespace IndustrialNetwork
{
	namespace POWERLINK
	{
		namespace Core
		{
			/**
			\brief %Node related namespace
			\author rueckerc, Bernecker+Rainer Industrie Elektronik Ges.m.b.H.
			*/
			namespace Node
			{
				/**
				\brief Represents the common base class for all POWERLINK nodes.
				\author rueckerc, Bernecker+Rainer Industrie Elektronik Ges.m.b.H.
				*/
				class DLLEXPORT BaseNode : public IndustrialNetwork::Fieldbus::IFieldbusNode<std::uint8_t, std::uint32_t, IndustrialNetwork::POWERLINK::Core::ObjectDictionary::Object>
				{

					public:
						BaseNode(std::uint8_t nodeId, const std::string& nodeName = "");
						virtual ~BaseNode();

						const std::string& GetName() const;
						void SetName(const std::string& name);
						std::uint8_t GetNodeId() const;
						void SetNodeId(std::uint8_t nodeId);
						std::map<std::uint32_t, std::shared_ptr<IndustrialNetwork::POWERLINK::Core::ObjectDictionary::Object>>& GetObjectDictionary();

						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result AddObject(std::shared_ptr<IndustrialNetwork::POWERLINK::Core::ObjectDictionary::Object>& objRef);
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result AddSubObject(std::uint32_t objectId, std::shared_ptr<IndustrialNetwork::POWERLINK::Core::ObjectDictionary::SubObject>& subObjRef);

						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result ForceObject(std::uint32_t objectId, bool force, bool validateOnly = false, const std::string& actualValue = "", bool enableLog = true);
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result SetObjectActualValue(std::uint32_t objectId, const std::string& actualValue);
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result GetObject(std::uint32_t objectId, std::shared_ptr<IndustrialNetwork::POWERLINK::Core::ObjectDictionary::Object>& objRef, bool enableLog = true);

						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result ForceSubObject(std::uint32_t objectId, std::uint32_t subObjectId, bool force, bool validateOnly = false, const std::string& actualValue = "", bool enableLog = true);
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result SetSubObjectActualValue(std::uint32_t objectId, std::uint32_t subObjectId, const std::string& actualValue);
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result GetSubObject(std::uint32_t objectId, std::uint32_t subObjectId, std::shared_ptr<IndustrialNetwork::POWERLINK::Core::ObjectDictionary::SubObject>& subObjRef, bool enableLog = true);

						virtual IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result AddNodeAssignment(const NodeAssignment& assign) = 0;
						virtual IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result RemoveNodeAssignment(const NodeAssignment& assign) = 0;
						virtual std::uint32_t GetNodeAssignmentValue() = 0;

						std::vector<IndustrialNetwork::POWERLINK::Core::Node::NodeAssignment>& GetNodeAssignment();
						std::shared_ptr<IndustrialNetwork::POWERLINK::Core::Node::NetworkManagement>& GetNetworkManagement();

						std::shared_ptr<IndustrialNetwork::POWERLINK::Core::ObjectDictionary::ApplicationProcess>& GetApplicationProcess();

						virtual std::uint32_t GetConfigurationObjectCount() = 0;
						virtual std::uint32_t GetConfigurationObjectSize() = 0;

						bool IsEnabled() const;
						void SetEnabled(bool enabled);

						virtual IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result CalculatePReqPayloadLimit() = 0;
						virtual IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result CalculatePResPayloadLimit() = 0;

						std::vector<std::shared_ptr<IndustrialNetwork::POWERLINK::Core::ObjectDictionary::BaseProcessDataMapping>>& GetTransmitMapping();
						std::vector<std::shared_ptr<IndustrialNetwork::POWERLINK::Core::ObjectDictionary::BaseProcessDataMapping>>& GetReceiveMapping();

						std::vector<std::shared_ptr<IndustrialNetwork::POWERLINK::Core::ObjectDictionary::BaseProcessImageObject>>& GetTransmitProcessImage();
						std::vector<std::shared_ptr<IndustrialNetwork::POWERLINK::Core::ObjectDictionary::BaseProcessImageObject>>& GetReceiveProcessImage();

						std::uint32_t GetTransmitProcessImageSize() const;
						std::uint32_t GetReceiveProcessImageSize() const;

						void ChangeMappingChannelNodeId(const std::uint8_t nodeId, const std::uint8_t newNodeId);
						void ClearMappingChannelforNode(const std::uint8_t nodeId);

					private:
						std::uint8_t nodeId;
						std::string name;
						bool enabled;
						std::map<std::uint32_t, std::shared_ptr<IndustrialNetwork::POWERLINK::Core::ObjectDictionary::Object>> objectDictionary;
						std::shared_ptr<IndustrialNetwork::POWERLINK::Core::ObjectDictionary::ApplicationProcess> applicationProcess;

						std::vector<IndustrialNetwork::POWERLINK::Core::Node::NodeAssignment> nodeAssignment;
						std::shared_ptr<IndustrialNetwork::POWERLINK::Core::Node::NetworkManagement> networkManagement;
						std::vector<std::shared_ptr<DynamicChannel>> dynamicChannelList;
						std::vector<std::shared_ptr<IndustrialNetwork::POWERLINK::Core::ObjectDictionary::BaseProcessDataMapping>> transmitMapping;
						std::vector<std::shared_ptr<IndustrialNetwork::POWERLINK::Core::ObjectDictionary::BaseProcessDataMapping>> receiveMapping;

						std::vector<std::shared_ptr<IndustrialNetwork::POWERLINK::Core::ObjectDictionary::BaseProcessImageObject>> transmitProcessImage;
						std::vector<std::shared_ptr<IndustrialNetwork::POWERLINK::Core::ObjectDictionary::BaseProcessImageObject>> receiveProcessImage;

				};
			}
		}
	}
}
#endif
