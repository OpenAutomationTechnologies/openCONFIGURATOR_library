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

using namespace IndustrialNetwork::POWERLINK::Core::Configuration;
using namespace IndustrialNetwork::POWERLINK::Core::ObjectDictionary;
using namespace IndustrialNetwork::POWERLINK::Core::ErrorHandling;
using namespace IndustrialNetwork::POWERLINK::Core::Node;

PlkConfiguration::PlkConfiguration() :
	IBuildConfiguration<BuildConfigurationSetting>(),
	configurationName("")
{}

PlkConfiguration::PlkConfiguration(const std::string& name) :
	IBuildConfiguration<BuildConfigurationSetting>(),
	configurationName(name)
{}

PlkConfiguration::~PlkConfiguration()
{}

Result PlkConfiguration::GenerateConfiguration(const std::map<uint8_t, std::shared_ptr<BaseNode>>& nodeCollection)
{
	Result res;
	//Generate autogeneration settings
	for (auto& config_setting : this->GetBuildConfigurationSettings())
	{
		if (config_setting->IsEnabled()) //Generate only if enabled
		{
			//Call settings builder
			res = config_setting->GenerateConfiguration(nodeCollection);
			if (!res.IsSuccessful())
				return res;
		}
	}

	//Distribute the 0x1020/0x1 and 0x2 to all CNs and RMNs
	//Distribute the 0x1f26/NodeID and 0x1F27/NodeID to MN and RMNs
	res = DistributeDateTimeStamps(nodeCollection);
	if (!res.IsSuccessful())
		return res;

	//Distribute the 0x1006/0x0 to all nodes
	res = DistributeCycleTime(nodeCollection);
	if (!res.IsSuccessful())
		return res;

	//Distribute the 0x1F98 / 0x7 to all nodes
	res = DistributeMultiplCycleCount(nodeCollection);
	if (!res.IsSuccessful())
		return res;

	//Distribute the 0x1F98 / 0x8 to all nodes
	res = DistributeAsyncMtu(nodeCollection);
	if (!res.IsSuccessful())
		return res;

	//Distribute the 0x1F98 / 0x9 to all nodes
	res = DistributePrescaler(nodeCollection);
	if (!res.IsSuccessful())
		return res;

	//Distribute the CN node assignments to MN and RMNs
	res = DistributeNodeAssignment(nodeCollection);
	if (!res.IsSuccessful())
		return res;

	//Distribute MultiplexCycleAssign 1F9B / NodeID to all nodes
	res = DistributeMultiplCycleAssign(nodeCollection);
	if (!res.IsSuccessful())
		return res;

	//Distribute SDOSeqLayerTimeout 1300 / 00 to all nodes
	res = DistributeSDOSeqLayerTimeout(nodeCollection);
	if (!res.IsSuccessful())
		return res;

	//Distribute SDOCmdLayerTimeout 1300 / 00 to all nodes
	res = DistributeSDOCmdLayerTimeout(nodeCollection);
	if (!res.IsSuccessful())
		return res;

	//Distribute CNPresTimeout 1F98 / 03 to MN 1F92 / nodeID and RMN 1F92 / nodeID
	res = DistributePResTimeOut(nodeCollection);
	if (!res.IsSuccessful())
		return res;

	//Distribute PReqPayloadLimit 1F98 / 04 to MN 1F8B / nodeID and RMN 1F8B / nodeID
	res = DistributePReqPayloadLimit(nodeCollection);
	if (!res.IsSuccessful())
		return res;

	//Distribute PResPayloadLimit 1F98 / 05 to all node 1F8D / nodeID
	res = DistributePResPayloadLimit(nodeCollection);
	return res;
}

const std::string& PlkConfiguration::GetConfigurationName()
{
	return this->configurationName;
}

void PlkConfiguration::SetConfigurationName(const std::string& configName)
{
	this->configurationName = configName;
}

Result PlkConfiguration::DistributeDateTimeStamps(const std::map<uint8_t, std::shared_ptr<BaseNode>>& nodeCollection)
{
	Result res;
	// Get current locale time
	auto now = std::chrono::system_clock::now();

	// Create days since 01/01/1984
	tm timeinfo = tm();
	timeinfo.tm_year = 84;   // year: 1984
	timeinfo.tm_mon = 0;      // month: january
	timeinfo.tm_mday = 1;     // day: 1st
	time_t epoch = mktime(&timeinfo);

	//Create new epoch time (01/01/1984)
	auto epoch_time_point = std::chrono::system_clock::from_time_t (epoch);
	//Days since that epoch
	auto daysSinceEpoch = std::chrono::duration_cast<std::chrono::duration<int, std::ratio<60 * 60 * 24>>> (now - epoch_time_point);

	// Create milliseconds since midnight
	time_t tnow = std::chrono::system_clock::to_time_t(now);
	tm* date = localtime(&tnow); // today
	date->tm_hour = 0; // set to midnight
	date->tm_min = 0;
	date->tm_sec = 0;
	auto midnight = std::chrono::system_clock::from_time_t(mktime(date));
	auto millisecondsSinceMidnight = std::chrono::duration_cast<std::chrono::milliseconds>(now - midnight);

	std::stringstream dateString;
	dateString << daysSinceEpoch.count(); //write days to std::string

	std::stringstream timeString;
	timeString << millisecondsSinceMidnight.count(); //write milliseconds to std::string

	for (auto& node :  nodeCollection)
	{
		//Distribute to MN and RMNs
		if (std::dynamic_pointer_cast<ManagingNode>(node.second)) //Set MN date and time objects 1F26 / 1F27
		{
			for (auto& nodeIds :  nodeCollection)
			{
				if (nodeIds.first == 240) //Avoid distributing values for MN nodeID
					continue;

				res = node.second->SetSubObjectActualValue(0x1F26, nodeIds.first, dateString.str());
				if (!res.IsSuccessful())
					return res;
				res = node.second->SetSubObjectActualValue(0x1F27, nodeIds.first, timeString.str());
				if (!res.IsSuccessful())
					return res;

				if (node.first != 240) //write 1020 values to RMNs
				{
					res = node.second->SetSubObjectActualValue(0x1020, 0x1, dateString.str());
					if (!res.IsSuccessful())
						return res;
					res = node.second->SetSubObjectActualValue(0x1020, 0x2, timeString.str());
					if (!res.IsSuccessful())
						return res;
				}
			}
		}
		else // Node is CN
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

Result PlkConfiguration::DistributeNodeAssignment(const std::map<uint8_t, std::shared_ptr<BaseNode>>& nodeCollection)
{
	std::stringstream nodeAssignmentStr;
	for (auto& node : nodeCollection)
	{
		if (node.first == 240) //Dont distribute assignement for MN
			continue;

		nodeAssignmentStr << node.second->GetNodeAssignmentValue(); //Retrieve assignment from CN or RMN
		for (auto& mn :  nodeCollection)
		{
			//Distribute to MN and RMNs
			if (std::dynamic_pointer_cast<ManagingNode>(mn.second)) //Set MN or RMN node assignement objects
			{
				Result res = mn.second->SetSubObjectActualValue(0x1F81, node.first, nodeAssignmentStr.str()); //Set actual value with assignment
				if (!res.IsSuccessful())
					return res;
			}
		}
		nodeAssignmentStr.str(std::string()); //reset std::stringstream
	}
	return Result();
}

Result PlkConfiguration::DistributeCycleTime(const std::map<uint8_t, std::shared_ptr<BaseNode>>& nodeCollection)
{
	std::stringstream cycleTimeStr;

	//Get managing node
	auto& mn = nodeCollection.at(240);
	std::shared_ptr<SubObject> cycleTimeObject;

	//Get Cycle Time object
	Result res = mn->GetSubObject(0x1006, 0x0, cycleTimeObject);
	if (!res.IsSuccessful())
		return res;

	//If actual value exists
	if (cycleTimeObject->WriteToConfiguration())
	{
		//Convert to cycle time to std::string
		cycleTimeStr << cycleTimeObject->GetTypedActualValue<uint32_t>();
	}
	//0x1006 / 0x0 has to be written so return error if not
	else
		return Result(ErrorCode::CYCLE_TIME_NOT_SET, kMsgCycleTimeOnMnNotSet);

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

Result PlkConfiguration::DistributeMultiplCycleCount(const std::map<uint8_t, std::shared_ptr<BaseNode>>& nodeCollection)
{
	std::stringstream multiplCycleCountStr;

	//Get managing node
	auto& mn = nodeCollection.at(240);
	std::shared_ptr<SubObject> multiplCycleCountObject;

	//Get multiplexed cycle count object
	Result res = mn->GetSubObject(0x1F98, 0x07, multiplCycleCountObject);
	if (!res.IsSuccessful())
		return res;

	if (multiplCycleCountObject->WriteToConfiguration())
	{
		//Convert multiplexed cycle count to std::string
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

Result PlkConfiguration::DistributeAsyncMtu(const std::map<uint8_t, std::shared_ptr<BaseNode>>& nodeCollection)
{
	std::stringstream asyncMtuStr;

	//Get managing node
	auto& mn = nodeCollection.at(240);
	std::shared_ptr<SubObject> asynMtuObject;

	//Get Async MTU object
	Result res = mn->GetSubObject(0x1F98, 0x8, asynMtuObject);
	if (!res.IsSuccessful())
		return res;

	if (asynMtuObject->WriteToConfiguration())
	{
		//Convert Async MTU to std::string
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
				return res; //If error occurs during set of async MTU return
		}
	}
	return Result();
}

Result PlkConfiguration::DistributePrescaler(const std::map<uint8_t, std::shared_ptr<BaseNode>>& nodeCollection)
{
	std::stringstream prescalerStr;

	//Get managing node
	auto& mn = nodeCollection.at(240);
	std::shared_ptr<SubObject> prescalerObject;

	//Get Cycle Time object
	Result res = mn->GetSubObject(0x1F98, 0x9, prescalerObject);
	if (!res.IsSuccessful())
		return res;

	if (prescalerObject->WriteToConfiguration())
	{
		//Convert prescaler to std::string
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

Result PlkConfiguration::DistributeMultiplCycleAssign(const std::map<uint8_t, std::shared_ptr<BaseNode>>& nodeCollection)
{
	//Get managing node
	auto& mn = nodeCollection.at(240);
	for (auto& node : nodeCollection)
	{
		if (node.first == 240)
			continue;

		std::shared_ptr<Object> multiplCycleAssignObject;
		Result res = mn->GetObject(0x1F9B, multiplCycleAssignObject);
		if (!res.IsSuccessful())
			return res;

		for (auto& subObj : multiplCycleAssignObject->GetSubObjectCollection())
		{
			if (subObj.second->WriteToConfiguration())
			{
				std::stringstream multiplCycleAssignValue;
				multiplCycleAssignValue << subObj.second->GetTypedActualValue<uint16_t>();
				res = node.second->SetSubObjectActualValue(0x1F9B, subObj.first, multiplCycleAssignValue.str());
				if (!res.IsSuccessful())
					return res;
			}
		}
	}
	return Result();
}

Result PlkConfiguration::DistributeSDOSeqLayerTimeout(const std::map<uint8_t, std::shared_ptr<BaseNode>>& nodeCollection)
{
	//Get managing node
	auto& mn = nodeCollection.at(240);
	std::shared_ptr<SubObject> sdoSeqLayerTimeoutObject;

	//Get Async MTU object
	Result res = mn->GetSubObject(0x1300, 0x0, sdoSeqLayerTimeoutObject);
	if (!res.IsSuccessful())
		return res;

	for (auto& node : nodeCollection)
	{
		if (node.first == 240)
			continue;

		if (sdoSeqLayerTimeoutObject->WriteToConfiguration())
		{
			std::stringstream sdoSeqLayerTimeoutStr;
			//Convert SDOSeqLayerTimeout to std::string
			sdoSeqLayerTimeoutStr << sdoSeqLayerTimeoutObject->GetTypedActualValue<uint32_t>();
			//Set every node 0x1300 / 0x0 actual value to SDOSeqLayerTimeout
			res = node.second->SetSubObjectActualValue(0x1300, 0x0, sdoSeqLayerTimeoutStr.str());
			if (!res.IsSuccessful())
				return res; //If error occurs during set of SDOSeqLayerTimeout return
		}
	}
	return Result();
}

Result PlkConfiguration::DistributeSDOCmdLayerTimeout(const std::map<uint8_t, std::shared_ptr<BaseNode>>& nodeCollection)
{
	//Get managing node
	auto& mn = nodeCollection.at(240);
	std::shared_ptr<SubObject> sdoCmdLayerTimeoutObject;

	//Get Async MTU object
	Result res = mn->GetSubObject(0x1301, 0x0, sdoCmdLayerTimeoutObject);
	if (!res.IsSuccessful())
		return res;

	for (auto& node : nodeCollection)
	{
		if (node.first == 240)
			continue;

		if (sdoCmdLayerTimeoutObject->WriteToConfiguration())
		{
			std::stringstream sdoCmdLayerTimeoutStr;
			//Convert CmdLayerTimeout to std::string
			sdoCmdLayerTimeoutStr << sdoCmdLayerTimeoutObject->GetTypedActualValue<uint32_t>();
			//Set every node 0x1300 / 0x0 actual value to CmdLayerTimeout
			res = node.second->SetSubObjectActualValue(0x1301, 0x0, sdoCmdLayerTimeoutStr.str());
			if (!res.IsSuccessful())
				return res; //If error occurs during set of CmdLayerTimeout return
		}
	}
	return Result();
}

Result PlkConfiguration::DistributePResTimeOut(const std::map<uint8_t, std::shared_ptr<BaseNode>>& nodeCollection)
{
	//Get managing node
	auto& mn = nodeCollection.at(240);

	for (auto& node : nodeCollection)
	{
		if (node.first == 240)
			continue;

		std::shared_ptr<SubObject> presTimoutCNObj;
		Result res = node.second->GetSubObject(0x1F98, 0x3, presTimoutCNObj);
		if (!res.IsSuccessful())
			return res;

		std::stringstream presTimoutActualValue;
		if (presTimoutCNObj->WriteToConfiguration())
			presTimoutActualValue << (presTimoutCNObj->GetTypedActualValue<uint32_t>());
		else
			continue;

		res = mn->SetSubObjectActualValue(0x1F92, node.first, presTimoutActualValue.str());
		if (!res.IsSuccessful())
			return res;

		for (auto& rmn : nodeCollection)
		{
			if (rmn.first == 240)
				continue;

			if (std::dynamic_pointer_cast<ManagingNode>(rmn.second))
			{
				res = rmn.second->SetSubObjectActualValue(0x1F92, node.first, presTimoutActualValue.str());
				if (!res.IsSuccessful())
					return res;
			}
		}
	}
	return Result();
}

Result PlkConfiguration::DistributePReqPayloadLimit(const std::map<uint8_t, std::shared_ptr<BaseNode>>& nodeCollection)
{
	//Get managing node
	auto& mn = nodeCollection.at(240);

	for (auto& node : nodeCollection)
	{
		if (node.first == 240)
			continue;

		std::shared_ptr<SubObject> preqActPayloadLimitObj;
		Result res = node.second->GetSubObject(0x1F98, 0x4, preqActPayloadLimitObj);
		if (!res.IsSuccessful())
			return res;

		std::stringstream preqActPayloadLimitValue;
		if (preqActPayloadLimitObj->WriteToConfiguration())
			preqActPayloadLimitValue << preqActPayloadLimitObj->GetTypedActualValue<uint32_t>();
		else
			continue;

		res = mn->SetSubObjectActualValue(0x1F8B, node.first, preqActPayloadLimitValue.str());
		if (!res.IsSuccessful())
			return res;

		for (auto& rmn : nodeCollection)
		{
			if (rmn.first == 240)
				continue;

			if (std::dynamic_pointer_cast<ManagingNode>(rmn.second))
			{
				res = rmn.second->SetSubObjectActualValue(0x1F8B, node.first, preqActPayloadLimitValue.str());
				if (!res.IsSuccessful())
					return res;
			}
		}
	}
	return Result();
}

Result PlkConfiguration::DistributePResPayloadLimit(const std::map<uint8_t, std::shared_ptr<BaseNode>>& nodeCollection)
{
	//Get managing node
	auto& mn = nodeCollection.at(240);

	for (auto& node : nodeCollection)
	{
		if (node.first == 240)
			continue;

		std::shared_ptr<SubObject> presActPayloadLimitObj;
		Result res = node.second->GetSubObject(0x1F98, 0x5, presActPayloadLimitObj);
		if (!res.IsSuccessful())
			return res;

		std::stringstream presActPayloadLimitValue;
		if (presActPayloadLimitObj->WriteToConfiguration())
			presActPayloadLimitValue << presActPayloadLimitObj->GetTypedActualValue<uint16_t>();
		else
			continue;

		res = mn->SetSubObjectActualValue(0x1F8D, node.first, presActPayloadLimitValue.str());
		if (!res.IsSuccessful())
			return res;

		for (auto& cn : nodeCollection)
		{
			if (cn.first == 240 || node.first == cn.first)
				continue;

			res = cn.second->SetSubObjectActualValue(0x1F8D, node.first, presActPayloadLimitValue.str());
			if (!res.IsSuccessful())
				return res;

		}
	}
	return Result();
}







