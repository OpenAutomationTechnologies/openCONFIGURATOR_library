/************************************************************************
\file PLKBuildConfigurationSetting.cpp
\brief Implementation of the Class PLKBuildConfigurationSetting
\author rueckerc, Bernecker+Rainer Industrie Elektronik Ges.m.b.H.
\date 27-Apr-2015 09:38:09
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
#include "BuildConfigurationSetting.h"

using namespace IndustrialNetwork::POWERLINK::Core::Configuration;
using namespace IndustrialNetwork::POWERLINK::Core::ErrorHandling;
using namespace IndustrialNetwork::POWERLINK::Core::CoreConfiguration;
using namespace IndustrialNetwork::POWERLINK::Core::Node;

BuildConfigurationSetting::BuildConfigurationSetting()
	: IndustrialNetwork::Fieldbus::IBuildConfigurationSetting(),
	  configurationBuilder(std::shared_ptr<BuildConfigurationSettingBuilder>())
{}

BuildConfigurationSetting::BuildConfigurationSetting(const std::string& id, const std::string& value)
	: IndustrialNetwork::Fieldbus::IBuildConfigurationSetting(id, value),
	  configurationBuilder(std::shared_ptr<BuildConfigurationSettingBuilder>())
{
	InitConfigurationSetting(id);
}

BuildConfigurationSetting::~BuildConfigurationSetting(void)
{}

void BuildConfigurationSetting::InitConfigurationSetting(const std::string& id)
{
	if (id == "GENERATE_MN_MAPPING_FOR_NODES")
	{
		this->SetDescription(BuildConfigurationIdDescription[BuildConfigurationId::GENERATE_MN_MAPPING_FOR_NODES]);
		this->configurationBuilder = std::shared_ptr<BuildConfigurationSettingBuilder>(new ManagingNodeMappingBuilder());
	}
	else if (id == "IGNORE_INVALID_MAPPING_OBJECT_REFERENCES")
	{
		this->SetDescription(BuildConfigurationIdDescription[BuildConfigurationId::IGNORE_INVALID_MAPPING_OBJECT_REFERENCES]);
		this->configurationBuilder.reset();
	}
	else if (id == "IGNORE_INVALID_MAPPING_OBJECT_OFFSETS")
	{
		this->SetDescription(BuildConfigurationIdDescription[BuildConfigurationId::IGNORE_INVALID_MAPPING_OBJECT_OFFSETS]);
		this->configurationBuilder.reset();
	}
	// add other supported settings here
	else
	{
		this->SetEnabled(false);
		boost::format formatter(kMsgConfigurationSettingNotSupported[static_cast<std::underlying_type<Language>::type>(LoggingConfiguration::GetInstance().GetCurrentLanguage())]);
		formatter
		% id;
		LOG_ERROR() << formatter.str();
	}
}

Result BuildConfigurationSetting::GenerateConfiguration(const std::map<std::uint8_t, std::shared_ptr<BaseNode>>& nodeCollection)
{
	if (this->configurationBuilder.get())
		return this->configurationBuilder->GenerateConfiguration(this->GetValue(), nodeCollection);
	else
		return Result();
}

