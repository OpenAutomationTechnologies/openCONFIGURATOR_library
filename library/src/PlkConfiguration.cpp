/************************************************************************
\file PlkConfiguration.cpp
\brief Implementation of the Class PlkConfiguration
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
#include "PlkConfiguration.h"

using namespace std;
using namespace IndustrialNetwork::POWERLINK::Core::Configuration;
using namespace IndustrialNetwork::POWERLINK::Core::ErrorHandling;
using namespace IndustrialNetwork::POWERLINK::Core::Node;

PlkConfiguration::PlkConfiguration() :
	IBuildConfiguration<BuildConfigurationSetting>(),
	configurationName("")
{}

PlkConfiguration::PlkConfiguration(const string& name) :
	IBuildConfiguration<BuildConfigurationSetting>(),
	configurationName(name)
{}

PlkConfiguration::~PlkConfiguration()
{}

Result PlkConfiguration::GenerateConfiguration(const map<uint8_t, shared_ptr<IndustrialNetwork::POWERLINK::Core::Node::BaseNode>>& nodeCollection)
{
	Result res;
	for (auto& config_setting : this->GetBuildConfigurationSettings())
	{
		if (config_setting->IsEnabled())
		{
			res = config_setting->GenerateConfiguration(nodeCollection);
		}
	}

	res = DistributeNodeAssignment(nodeCollection);
	if (!res.IsSuccessful())
		return res;
	res = DistributeDateTimeStamps(nodeCollection);
	return res;
}

const string& PlkConfiguration::GetConfigurationName()
{
	return this->configurationName;
}

void PlkConfiguration::SetConfigurationName(const string& configName)
{
	this->configurationName = configName;
}

Result PlkConfiguration::DistributeDateTimeStamps(const map<uint8_t, shared_ptr<BaseNode>>& nodeCollection)
{
	Result res;
	// Get current locale time
	auto now = chrono::system_clock::now();

	// Create days since 01/01/1984
	tm timeinfo = tm();
	timeinfo.tm_year = 84;   // year: 1984
	timeinfo.tm_mon = 0;      // month: january
	timeinfo.tm_mday = 1;     // day: 1st
	time_t epoch = mktime(&timeinfo);

	auto epoch_time_point = chrono::system_clock::from_time_t (epoch);
	auto daysSinceEpoch = chrono::duration_cast<chrono::duration<int, ratio<60 * 60 * 24>>> (now - epoch_time_point);

	// Create milliseconds since midnight
	time_t tnow = chrono::system_clock::to_time_t(now);
	tm* date = localtime(&tnow); // today
	date->tm_hour = 0; // set to midnight
	date->tm_min = 0;
	date->tm_sec = 0;
	auto midnight = chrono::system_clock::from_time_t(mktime(date));
	auto millisecondsSinceMidnight = chrono::duration_cast<chrono::milliseconds>(now - midnight);

	cout << hex << daysSinceEpoch.count() << endl;
	cout << hex << millisecondsSinceMidnight.count() << endl;

	stringstream dateString;
	dateString << daysSinceEpoch.count();

	stringstream timeString;
	timeString << millisecondsSinceMidnight.count();

	for (auto& node :  nodeCollection)
	{
		if (node.first == 240) //Set MN date and time objects 1F26 / 1F27
		{
			for (auto& nodeIds :  nodeCollection)
			{
				if (node.first != nodeIds.first)
				{
					res = node.second->SetSubObjectActualValue(0x1F26, nodeIds.first, dateString.str());
					if (!res.IsSuccessful())
						return res;
					res = node.second->SetSubObjectActualValue(0x1F27, nodeIds.first, timeString.str());
					if (!res.IsSuccessful())
						return res;
				}
			}
		}
		else
		{
			//Set CN date and time objects 1020/01 and 1020/02
			res = node.second->SetSubObjectActualValue(0x1020, 0x1, dateString.str());
			if (!res.IsSuccessful())
				return res;
			res = node.second->SetSubObjectActualValue(0x1020, 0x2, timeString.str());
			if (!res.IsSuccessful())
				return res;
		}

	}
	return res;
}

Result PlkConfiguration::DistributeNodeAssignment(const map<uint8_t, shared_ptr<BaseNode>>& nodeCollection)
{
	stringstream nodeAssignmentStr;
	auto& mn = nodeCollection.at(240);

	for (auto& node : nodeCollection)
	{
		if (node.first != 240)
		{
			nodeAssignmentStr << node.second->GetNodeAssignmentValue();
			mn->SetSubObjectActualValue(0x1F81, node.second->GetNodeIdentifier(), nodeAssignmentStr.str());
			nodeAssignmentStr.str(string());
		}
	}


	return Result();
}


