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
using namespace IndustrialNetwork::POWERLINK::Core::ObjectDictionary;
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
	//Distribute the 0x1006/0x0 to all nodes
	res = DistributeCycleTime(nodeCollection);
	if (!res.IsSuccessful())
		return res;

	res = DistributeMultiplCycleCount(nodeCollection);
	if (!res.IsSuccessful())
		return res;

	res = DistributeAsyncMtu(nodeCollection);
	if (!res.IsSuccessful())
		return res;

	res = DistributePrescaler(nodeCollection);
	if (!res.IsSuccessful())
		return res;

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

Result PlkConfiguration::DistributeCycleTime(const map<uint8_t, shared_ptr<BaseNode>>& nodeCollection)
{
	stringstream cycleTimeStr;

	//Get managing node
	auto& mn = nodeCollection.at(240);
	shared_ptr<SubObject> cycleTimeObject;

	//Get Cycle Time object
	Result res = mn->GetSubObject(0x1006, 0x0, cycleTimeObject);
	if (!res.IsSuccessful())
		return res;

	//If actual value exists
	if (cycleTimeObject->WriteToConfiguration())
	{
		//Convert to cycle time to string
		cycleTimeStr << cycleTimeObject->GetTypedActualValue<uint32_t>();
	}
	//0x1006 / 0x0 has to be written so return error if not
	else
	{
		//Convert to cycle time to string
		cycleTimeStr << cycleTimeObject->GetTypedDefaultValue<uint32_t>();
	}

	for (auto& node : nodeCollection)
	{
		if (node.first != 240)
		{
			//Set every node 0x1006 / 0x0 actual value to cycle time
			res = node.second->SetSubObjectActualValue(0x1006, 0x0, cycleTimeStr.str());
			if (!res.IsSuccessful())
				return res; //If error occurs during set of cycle time return
		}
	}
	return Result();
}

Result PlkConfiguration::DistributeMultiplCycleCount(const map<uint8_t, shared_ptr<BaseNode>>& nodeCollection)
{
	stringstream multiplCycleCountStr;

	//Get managing node
	auto& mn = nodeCollection.at(240);
	shared_ptr<SubObject> multiplCycleCountObject;

	//Get multiplexed cycle count object
	Result res = mn->GetSubObject(0x1F98, 0x07, multiplCycleCountObject);
	if (!res.IsSuccessful())
		return res;

	if (multiplCycleCountObject->WriteToConfiguration())
	{
		//Convert multiplexed cycle count to string
		multiplCycleCountStr << multiplCycleCountObject->GetTypedActualValue<uint16_t>();
	}
	else
	{
		multiplCycleCountStr << multiplCycleCountObject->GetTypedDefaultValue<uint16_t>();
	}

	for (auto& node : nodeCollection)
	{
		if (node.first != 240)
		{
			//Set every node 0x1F98 / 0x7 actual value to multiplexed cycle count
			res = node.second->SetSubObjectActualValue(0x1F98, 0x07, multiplCycleCountStr.str());
			if (!res.IsSuccessful())
				return res; //If error occurs during set of multiplexed cycle count return
		}
	}
	return Result();
}

Result PlkConfiguration::DistributeAsyncMtu(const map<uint8_t, shared_ptr<BaseNode>>& nodeCollection)
{
	stringstream asyncMtuStr;

	//Get managing node
	auto& mn = nodeCollection.at(240);
	shared_ptr<SubObject> asynMtuObject;

	//Get Async MTU object
	Result res = mn->GetSubObject(0x1F98, 0x8, asynMtuObject);
	if (!res.IsSuccessful())
		return res;

	if (asynMtuObject->WriteToConfiguration())
	{
		//Convert Async MTU to string
		asyncMtuStr << asynMtuObject->GetTypedActualValue<uint16_t>();
	}
	else
	{
		asyncMtuStr << asynMtuObject->GetTypedDefaultValue<uint16_t>();
	}

	for (auto& node : nodeCollection)
	{
		if (node.first != 240)
		{
			//Set every node 0x1F98 / 0x8 actual value to async MTU
			res = node.second->SetSubObjectActualValue(0x1F98, 0x8, asyncMtuStr.str());
			if (!res.IsSuccessful())
				return res; //If error occurs during set of cycle time return
		}
	}
	return Result();
}

Result PlkConfiguration::DistributePrescaler(const map<uint8_t, shared_ptr<BaseNode>>& nodeCollection)
{
	stringstream prescalerStr;

	//Get managing node
	auto& mn = nodeCollection.at(240);
	shared_ptr<SubObject> prescalerObject;

	//Get Cycle Time object
	Result res = mn->GetSubObject(0x1F98, 0x9, prescalerObject);
	if (!res.IsSuccessful())
		return res;

	if (prescalerObject->WriteToConfiguration())
	{
		//Convert prescaler to string
		prescalerStr << prescalerObject->GetTypedActualValue<uint16_t>();
	}
	else
	{
		prescalerStr << prescalerObject->GetTypedDefaultValue<uint16_t>();
	}

	for (auto& node : nodeCollection)
	{
		if (node.first != 240)
		{
			//Set every node 0x1F98 / 0x9 actual value to prescaler
			res = node.second->SetSubObjectActualValue(0x1F98, 0x9, prescalerStr.str());
			if (!res.IsSuccessful())
				return res; //If error occurs during set of prescaler return
		}
	}
	return Result();
}


