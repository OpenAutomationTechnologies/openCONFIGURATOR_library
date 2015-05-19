/************************************************************************
\file Network.h
\brief Implementation of the Class Network
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
#if !defined NETWORK_H
#define NETWORK_H

#include <memory>
#include "Result.h"
#include "BaseNode.h"
#include "Module.h"
#include "NodeType.h"
#include "PlkConfiguration.h"
#include "Exports.h"
#include "ControlledNode.h"
#include "ManagingNode.h"
#include "PlkConfiguration.h"
#include "BuildConfigurationId.h"
#include "BuildConfigurationSetting.h"

namespace IndustrialNetwork
{
	namespace POWERLINK
	{
		namespace Core
		{
			namespace NetworkHandling
			{
				/**
				\brief
				\author rueckerc
				*/
				class DLLEXPORT Network
				{

					public:
						Network();
						Network(const std::string networkId);
						virtual ~Network();

						/**
						/brief Adds a controlled node (or redundant managing node) to the network.
						/param[in] controlled node reference.
						/return Result
						*/
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result AddNode(IndustrialNetwork::POWERLINK::Core::Node::ControlledNode& node);

						/**
						/brief Adds a managing node to the network.
						/param[in] managing node reference.
						/return Result
						*/
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result AddNode(IndustrialNetwork::POWERLINK::Core::Node::ManagingNode& node);

						/**
						/brief Removes a node to the network.
						/param[in] node reference.
						/return Result
						*/
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result RemoveNode(const std::uint8_t nodeID);

						/**
						/brief Replace an existing node in the network.
						/param[in] node id of node to be replaced.
						/param[in] node reference.
						/return Result
						*/
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result ReplaceNode(const std::uint8_t nodeID, IndustrialNetwork::POWERLINK::Core::Node::BaseNode& node);

						/**
						/brief Retrieve an existing node in the network.
						/param[in] node id of node to be retrieved.
						/param[in] node reference.
						/return Result
						*/
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result GetNode(const std::uint8_t nodeID, IndustrialNetwork::POWERLINK::Core::Node::BaseNode& node);

						/**
						/brief Retrieve the current managing node from the network.
						/param[in] node reference.
						/return Result
						*/
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result GetManagingNode(IndustrialNetwork::POWERLINK::Core::Node::BaseNode& node);

						/**
						/brief Retrieve all the nodes in the network.
						/param[in] node vector reference.
						/return Result
						*/
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result GetNodes(std::vector<std::shared_ptr<IndustrialNetwork::POWERLINK::Core::Node::BaseNode>>& nodeCollection);

						/**
						/brief Retrieve all exising node ids in the network.
						/param[in] node id vector reference.
						/return Result
						*/
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result GetAvailableNodeIds(std::vector<std::uint8_t>& nodeIdCollection);

						const std::string GetNetworkId();
						std::uint32_t GetCycleTime();
						std::uint32_t GetAsyncMTU();
						std::uint32_t GetMultiplexedCycleLength();
						std::uint32_t GetPrescaler();

						void SetCycleTime(const std::uint32_t cycleTime);
						void SetAsyncMTU(const std::uint32_t asyncMTU);
						void SetMultiplexedCycleLength(const std::uint32_t multiCycleLength);
						void SetPrescaler(const std::uint32_t prescaler);

						/**
						\brief Enables/Disables a configuration setting handled by the class.
						\param[in] configID string identifier.
						\param[in] name of the setting.
						\retval true The setting was enabled/disabled.
						\retval false The setting does not exist in the configuration.
						*/
						bool SetConfigurationSettingEnabled(const std::string  configID, const std::string  settingID, bool enabled);

						/**
						\brief Adds a configuration setting to the handler class.
						\param[in] configID string identifier.
						\param[in] setting BuildConfigurationSetting to be added.
						\retval true The setting was added to the handler.
						\retval false The setting already exist in the configuration.
						*/
						bool AddConfigurationSetting(const std::string configID, IndustrialNetwork::POWERLINK::Core::Configuration::BuildConfigurationSetting setting);

						/**
						\brief Remove a configuration setting handled by the class.
						\param[in] configID string identifier.
						\param[in] id BuildConfigurationSettingId of the setting.
						\retval true The setting was removed from the handler.
						\retval false The setting does not exist in the configuration.
						*/
						bool RemoveConfigurationSetting(const std::string configID, const std::string name);

						/**
						\brief Add a configuration to the handler.
						\param[in] configID string identifier. Must be unique.
						\retval true The configuration has been created.
						\retval false The configuration already exists.
						*/
						bool AddConfiguration(const std::string configID);

						/**
						\brief Remove a configuration from the handler.
						\param[in] configID string identifier.
						\retval true The configuration has been deleted.
						\retval false The configuration does not exist.
						*/
						bool RemoveConfiguration(const std::string configID);

						/**
						\brief Alter a configuration name.
						\param[in] oldConfigID configuration identifier.
						\param[in] newConfigId new name.
						\retval true The configuration has been altered.
						\retval false The configuration does not exist.
						*/
						bool ReplaceConfigurationName(const std::string oldConfigId, const std::string newConfigId);

						/**
						\brief Get the settings from a configuration.
						\param[in] configID string identifier.
						\param[in] std::vector<BuildConfigurationSetting>& to return the collection.
						\retval true The configuration exists and the returned reference is valid.
						\retval false The configuration does not exist.
						*/
						bool GetConfigurationSettings(const std::string configID, std::vector<IndustrialNetwork::POWERLINK::Core::Configuration::BuildConfigurationSetting>&);

						/**
						\brief Get the active configuration.
						\retval std::string active configuration.
						*/
						const std::string& GetActiveConfiguration();

						/**
						\brief Set the active configuration.
						\retval true The active configuration is set.
						\retval false The active configuration is not set.
						*/
						bool SetActiveConfiguration(const std::string configID);

					private:
						std::string networkId;
						std::uint32_t cycleTime;
						std::uint32_t asyncMTU;
						std::uint32_t multiplexedCycleLength;
						std::uint32_t prescaler;
						std::unordered_map<std::uint8_t, std::shared_ptr<IndustrialNetwork::POWERLINK::Core::Node::BaseNode>> nodeCollection;
						std::vector<IndustrialNetwork::POWERLINK::Core::Configuration::PlkConfiguration> buildConfigurations;
						std::string activeConfiguration;
				};

			}

		}

	}

}
#endif
