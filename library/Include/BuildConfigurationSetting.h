/************************************************************************
\file PLKBuildConfigurationSetting.h
\brief Implementation of the Class PLKBuildConfigurationSetting
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
#if !defined PLK_BUILD_CONFIGURATION_SETTING_H
#define PLK_BUILD_CONFIGURATION_SETTING_H

#include <vector>
#include <map>
#include <memory>

#include <boost/format.hpp>

#include "IResult.h"
#include "IBuildConfigurationSetting.h"
#include "BuildConfigurationId.h"
#include "BuildConfigurationSettingBuilder.h"
#include "ManagingNodeMappingBuilder.h"
#include "Result.h"
#include "ErrorCode.h"
#include "Constants.h"
#include "LoggingConfiguration.h"
#include "BaseNode.h"

namespace IndustrialNetwork
{
	namespace POWERLINK
	{
		namespace Core
		{
			namespace Configuration
			{
				/**
				\brief Represents one build configuration setting in the POWERLINK network.
				\author rueckerc, Bernecker+Rainer Industrie Elektronik Ges.m.b.H.
				*/
				class DLLEXPORT BuildConfigurationSetting : public IndustrialNetwork::Fieldbus::IBuildConfigurationSetting
				{
					public:
						BuildConfigurationSetting();
						/**
						\brief Constructor for the BuildConfigurationHandler class.
						\param[in] name Name for the setting.
						\param[in] value String value of the setting. Default value is empty.
						*/
						BuildConfigurationSetting(const std::string& name, const std::string& value = "");
						virtual ~BuildConfigurationSetting();

						

						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result GenerateConfiguration(const std::map<std::uint8_t, std::shared_ptr<IndustrialNetwork::POWERLINK::Core::Node::BaseNode>>& nodeCollection);

					private:
						void InitConfigurationSetting(const std::string& id);
						std::shared_ptr<BuildConfigurationSettingBuilder> configurationBuilder;
				};
			}
		}
	}
}
#endif
