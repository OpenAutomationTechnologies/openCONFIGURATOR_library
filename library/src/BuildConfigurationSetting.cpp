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

using namespace std;
using namespace IndustrialNetwork::POWERLINK::Core::Configuration;
using namespace IndustrialNetwork::POWERLINK::Core::ErrorHandling;

BuildConfigurationSetting::BuildConfigurationSetting()
	:  IndustrialNetwork::Fieldbus::IBuildConfigurationSetting()
{}

BuildConfigurationSetting::BuildConfigurationSetting(string id, string value)
	:  IndustrialNetwork::Fieldbus::IBuildConfigurationSetting(id, value)
{
	InitConfigurationSetting(id);
}

BuildConfigurationSetting::~BuildConfigurationSetting(void)
{}

void BuildConfigurationSetting::InitConfigurationSetting(string id)
{
	if (id == "GENERATE_MN_MAPPING_FOR_NODES")
	{
		this->SetDescription(BuildConfigurationIdDescription[BuildConfigurationId::GENERATE_MN_MAPPING_FOR_NODES]);
	}
	else if (id == "GENERATE_MN_NODE_ASSIGNMENT_FOR_NODES")
	{
		this->SetDescription(BuildConfigurationIdDescription[BuildConfigurationId::GENERATE_MN_NODE_ASSIGNMENT_FOR_NODES]);
	}
	else if (id == "GENERATE_MN_PRES_TIMEOUT_FOR_NODES")
	{
		this->SetDescription(BuildConfigurationIdDescription[BuildConfigurationId::GENERATE_MN_MAPPING_FOR_NODES]);
	}
}

