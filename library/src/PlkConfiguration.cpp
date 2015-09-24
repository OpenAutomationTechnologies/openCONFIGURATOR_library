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
using namespace IndustrialNetwork::POWERLINK::Core::CoreConfiguration;
using namespace IndustrialNetwork::POWERLINK::Core::Utilities;

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

Result PlkConfiguration::GenerateConfiguration(const std::map<std::uint8_t, std::shared_ptr<BaseNode>>& nodeCollection)
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

	//Distribute the 0x1006 to all nodes
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

	//Distribute 0x1E40 / 0x5 to supporting nodes
	res = DistributeIpDefaultGateway(nodeCollection);
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

	//Distribute SDOCmdLayerTimeout 1301 / 00 to all nodes
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
	if (!res.IsSuccessful())
		return res;

	//Write 0x1C0B / 0x1C0C / 0x1C0D / 0x3 for all nodes if no actual values does exist
	res = DistributeCNLossObjects(nodeCollection);
	if (!res.IsSuccessful())
		return res;

	//Sync all needed MN object actual values with the RMN objects
	return SyncRedundantManagingNodes(nodeCollection);
}

const std::string& PlkConfiguration::GetConfigurationName() const
{
	return this->configurationName;
}

void PlkConfiguration::SetConfigurationName(const std::string& configName)
{
	this->configurationName = configName;
}

Result PlkConfiguration::DistributeDateTimeStamps(const std::map<std::uint8_t, std::shared_ptr<BaseNode>>& nodeCollection)
{
	Result res;
	// Get current locale time
	auto now = std::chrono::system_clock::now();

	// Create days since 01/01/1984
	tm timeinfo = tm();
	timeinfo.tm_year = 84; // year: 1984
	timeinfo.tm_mon = 0; // month: january
	timeinfo.tm_mday = 1; // day: 1st
	time_t epoch = mktime(&timeinfo);

	//Create new epoch time (01/01/1984)
	auto epoch_time_point = std::chrono::system_clock::from_time_t (epoch);
	//Days since that epoch
	auto daysSinceEpoch = std::chrono::duration_cast<std::chrono::duration<int, std::ratio<60 * 60 * 24>>> (now - epoch_time_point);

	// Create milliseconds since midnight
	time_t tnow = std::chrono::system_clock::to_time_t(now);
	tm* date = localtime(&tnow); // today
	if (date == NULL)
		return Result(ErrorCode::ARGUMENT_INVALID_NULL);

	date->tm_hour = 0; // set to midnight
	date->tm_min = 0;
	date->tm_sec = 0;
	auto midnight = std::chrono::system_clock::from_time_t(mktime(date));
	auto millisecondsSinceMidnight = std::chrono::duration_cast<std::chrono::milliseconds>(now - midnight);

	std::stringstream dateString;
	dateString << daysSinceEpoch.count(); //write days to std::string

	std::stringstream timeString;
	timeString << millisecondsSinceMidnight.count(); //write milliseconds to std::string

	for (auto& node : nodeCollection)
	{
		if (node.second->IsEnabled() == false)
			continue;

		//Distribute to MN and RMNs
		if (std::dynamic_pointer_cast<ManagingNode>(node.second)) //Set MN date and time objects 1F26 / 1F27
		{
			//Clear to delete pending artifacts
			ClearActualValues(nodeCollection, node.second, 0x1F26);
			ClearActualValues(nodeCollection, node.second, 0x1F27);

			for (auto& nodeIds : nodeCollection)
			{
				if (nodeIds.first == 240) //Avoid distributing values for MN nodeID
					continue;

				if (nodeIds.second->IsEnabled() == false)
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

Result PlkConfiguration::DistributeNodeAssignment(const std::map<std::uint8_t, std::shared_ptr<BaseNode>>& nodeCollection)
{
	std::stringstream nodeAssignmentStr;
	auto& mn = nodeCollection.at(240);
	ClearActualValues(nodeCollection, mn, 0x1F81);

	for (auto& node : nodeCollection)
	{
		if (node.second->IsEnabled() == false)
			continue;

		nodeAssignmentStr << node.second->GetNodeAssignmentValue(); //Retrieve assignment from CN or RMN
		for (auto& mn : nodeCollection)
		{
			//Distribute to MN and RMNs
			if (std::dynamic_pointer_cast<ManagingNode>(mn.second)) //Set MN or RMN node assignment objects
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

Result PlkConfiguration::DistributeCycleTime(const std::map<std::uint8_t, std::shared_ptr<BaseNode>>& nodeCollection)
{
	std::stringstream cycleTimeStr;

	//Get managing node
	auto& mn = nodeCollection.at(240);
	std::shared_ptr<Object> cycleTimeObject;

	//Get Cycle Time object
	Result res = mn->GetObject(0x1006, cycleTimeObject);
	if (!res.IsSuccessful())
		return res;

	//If actual value exists
	if (!cycleTimeObject->GetActualValue().empty())
	{
		//Convert to cycle time to std::string
		cycleTimeStr << cycleTimeObject->GetTypedActualValue<std::uint32_t>();

		//actual == default cycle time force MN object
		if (cycleTimeObject->WriteToConfiguration() == false)
			cycleTimeObject->SetForceToCDC(true);
		else
			cycleTimeObject->SetForceToCDC(false);

	}
	//0x1006 has to be written so return error if not
	else
		return Result(ErrorCode::CYCLE_TIME_NOT_SET, kMsgCycleTimeOnMnNotSet);

	//Warn if cycle time has a default value on MN
	if (cycleTimeObject->HasDefaultValue())
	{
		if (cycleTimeObject->GetTypedActualValue<std::uint32_t>() == cycleTimeObject->GetTypedDefaultValue<std::uint32_t>())
		{
			LOG_WARN() << kMsgCycleTimeDefaultValue;
		}
	}

	for (auto& node : nodeCollection)
	{
		if (node.second->IsEnabled() == false)
			continue;

		if (node.first != 240)
		{
			//Set every node 0x1006 actual value to cycle time
			res = node.second->SetObjectActualValue(0x1006, cycleTimeStr.str());
			if (!res.IsSuccessful())
				return res; //If error occurs during set of cycle time return
		}
	}
	return Result();
}

Result PlkConfiguration::DistributeMultiplCycleCount(const std::map<std::uint8_t, std::shared_ptr<BaseNode>>& nodeCollection)
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
		multiplCycleCountStr << multiplCycleCountObject->GetTypedActualValue<std::uint16_t>();
	}
	else
	{
		multiplCycleCountStr << multiplCycleCountObject->GetTypedDefaultValue<std::uint16_t>();
	}


	for (auto& node : nodeCollection)
	{
		if (node.second->IsEnabled() == false)
			continue;

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

Result PlkConfiguration::DistributeAsyncMtu(const std::map<std::uint8_t, std::shared_ptr<BaseNode>>& nodeCollection)
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
		asyncMtuStr << asynMtuObject->GetTypedActualValue<std::uint16_t>();
	}
	else
	{
		asyncMtuStr << asynMtuObject->GetTypedDefaultValue<std::uint16_t>();
	}

	for (auto& node : nodeCollection)
	{
		if (node.second->IsEnabled() == false)
			continue;

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

Result PlkConfiguration::DistributePrescaler(const std::map<std::uint8_t, std::shared_ptr<BaseNode>>& nodeCollection)
{
	std::stringstream prescalerStr;

	//Get managing node
	auto& mn = nodeCollection.at(240);
	std::shared_ptr<SubObject> prescalerObject;

	//Get Prescaler object
	Result res = mn->GetSubObject(0x1F98, 0x9, prescalerObject, false);
	if (!res.IsSuccessful())
		return Result();

	if (prescalerObject->WriteToConfiguration())
	{
		//Convert prescaler to std::string
		prescalerStr << prescalerObject->GetTypedActualValue<std::uint16_t>();
	}
	else
	{
		prescalerStr << prescalerObject->GetTypedDefaultValue<std::uint16_t>();
	}

	for (auto& node : nodeCollection)
	{
		if (node.second->IsEnabled() == false)
			continue;

		if (node.first != 240)
		{
			std::shared_ptr<SubObject> cnPrescalerObject;
			Result res = node.second->GetSubObject(0x1F98, 0x9, cnPrescalerObject, false);
			if (!res.IsSuccessful())
				continue;

			//Set every node 0x1F98 / 0x9 actual value to prescaler if existing
			res = cnPrescalerObject->SetTypedObjectActualValue(prescalerStr.str());
			if (!res.IsSuccessful())
				return res;
		}
	}
	return Result();
}

Result PlkConfiguration::DistributeMultiplCycleAssign(const std::map<std::uint8_t, std::shared_ptr<BaseNode>>& nodeCollection)
{
	//Get managing node
	auto& mn = nodeCollection.at(240);
	//Check if multiplexing supported
	bool featureMultiplex = false;
	mn->GetNetworkManagement()->GetFeatureActualValue(MNFeatureEnum::DLLMNFeatureMultiplex, featureMultiplex);
	if (featureMultiplex == false)
		return Result();

	for (auto& node : nodeCollection)
	{
		if (node.first == 240)
			continue;

		if (node.second->IsEnabled() == false)
			continue;

		std::shared_ptr<Object> multiplCycleAssignObject;
		Result res = mn->GetObject(0x1F9B, multiplCycleAssignObject);
		if (!res.IsSuccessful())
			return Result();

		for (auto& subObj : multiplCycleAssignObject->GetSubObjectDictionary())
		{
			if (subObj.second->WriteToConfiguration())
			{
				std::stringstream multiplCycleAssignValue;
				multiplCycleAssignValue << subObj.second->GetTypedActualValue<std::uint16_t>();
				res = node.second->SetSubObjectActualValue(0x1F9B, subObj.first, multiplCycleAssignValue.str());
				if (!res.IsSuccessful())
					return Result();
			}
		}
	}
	return Result();
}

Result PlkConfiguration::DistributeSDOSeqLayerTimeout(const std::map<std::uint8_t, std::shared_ptr<BaseNode>>& nodeCollection)
{
	//Get managing node
	auto& mn = nodeCollection.at(240);
	std::shared_ptr<Object> sdoSeqLayerTimeoutObject;

	//Get SDOSeqLayerTimeout object
	Result res = mn->GetObject(0x1300, sdoSeqLayerTimeoutObject);
	if (!res.IsSuccessful())
		return res;

	for (auto& node : nodeCollection)
	{
		if (node.first == 240)
			continue;

		if (node.second->IsEnabled() == false)
			continue;

		if (sdoSeqLayerTimeoutObject->WriteToConfiguration())
		{
			std::stringstream sdoSeqLayerTimeoutStr;
			//Convert SDOSeqLayerTimeout to std::string
			sdoSeqLayerTimeoutStr << sdoSeqLayerTimeoutObject->GetTypedActualValue<std::uint32_t>();
			//Set every node 0x1300 actual value to SDOSeqLayerTimeout
			res = node.second->SetObjectActualValue(0x1300, sdoSeqLayerTimeoutStr.str());
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
	std::shared_ptr<Object> sdoCmdLayerTimeoutObject;

	//Get CmdLayerTimeout object
	Result res = mn->GetObject(0x1301, sdoCmdLayerTimeoutObject, false);
	if (!res.IsSuccessful())
		return Result();

	for (auto& node : nodeCollection)
	{
		if (node.first == 240)
			continue;

		if (node.second->IsEnabled() == false)
			continue;

		if (sdoCmdLayerTimeoutObject->WriteToConfiguration())
		{
			std::stringstream sdoCmdLayerTimeoutStr;
			//Convert CmdLayerTimeout to std::string
			sdoCmdLayerTimeoutStr << sdoCmdLayerTimeoutObject->GetTypedActualValue<std::uint32_t>();
			//Set every node 0x1301 actual value to CmdLayerTimeout
			res = node.second->SetObjectActualValue(0x1301, sdoCmdLayerTimeoutStr.str());
			if (!res.IsSuccessful() && res.GetErrorType() != ErrorCode::OBJECT_DOES_NOT_EXIST)
				return res; //If error occurs during set of CmdLayerTimeout return
		}
	}
	return Result();
}

Result PlkConfiguration::DistributePResTimeOut(const std::map<std::uint8_t, std::shared_ptr<BaseNode>>& nodeCollection)
{
	//Get managing node
	auto& mn = nodeCollection.at(240);
	ClearActualValues(nodeCollection, mn, 0x1F92);

	for (auto& node : nodeCollection)
	{
		if (node.first == 240)
			continue;

		if (node.second->IsEnabled() == false)
			continue;

		std::shared_ptr<SubObject> presMaxLatencyCNObj;
		Result res = node.second->GetSubObject(0x1F98, 0x3, presMaxLatencyCNObj);
		if (!res.IsSuccessful())
			return res;

		std::shared_ptr<SubObject> presTimoutMNObj;
		res = mn->GetSubObject(0x1F92, (std::uint32_t) node.first, presTimoutMNObj);
		if (!res.IsSuccessful())
			return res;

		std::uint32_t presMaxLatencyActualValue = 0;
		if (presMaxLatencyCNObj->WriteToConfiguration())
			presMaxLatencyActualValue = presMaxLatencyCNObj->GetTypedActualValue<std::uint32_t>();
		else if (presMaxLatencyCNObj->HasDefaultValue())
			presMaxLatencyActualValue = presMaxLatencyCNObj->GetTypedDefaultValue<std::uint32_t>();

		std::uint32_t presTimoutActualValue = 0;
		if (presTimoutMNObj->HasActualValue())
			presTimoutActualValue = presTimoutMNObj->GetTypedActualValue<std::uint32_t>();
		else
		{
			presTimoutActualValue = 25000 + presMaxLatencyActualValue; //Default value
			presTimoutMNObj->SetTypedObjectActualValue(IntToHex(presTimoutActualValue, 8, "0x"));
		}

		if (presTimoutActualValue < presMaxLatencyActualValue)
		{
			boost::format formatter(kMsgLowCnPresTimeout);
			formatter
			% (std::uint32_t) presTimoutActualValue
			% (std::uint32_t) node.first
			% (std::uint32_t) node.first;
			LOG_FATAL() << formatter.str();
			return Result(ErrorCode::LOW_CN_PRES_TIMEOUT, formatter.str());
		}

		for (auto& rmn : nodeCollection)
		{
			if (rmn.first == 240)
				continue;

			if (std::dynamic_pointer_cast<ManagingNode>(rmn.second))
			{
				res = rmn.second->SetSubObjectActualValue(0x1F92, node.first, IntToHex(presTimoutActualValue, 8, "0x"));
				if (!res.IsSuccessful())
					return res;
			}
		}
	}
	return Result();
}

Result PlkConfiguration::DistributePReqPayloadLimit(const std::map<std::uint8_t, std::shared_ptr<BaseNode>>& nodeCollection)
{
	//Get managing node
	auto& mn = nodeCollection.at(240);
	ClearActualValues(nodeCollection, mn, 0x1F8B);

	for (auto& node : nodeCollection)
	{
		if (node.first == 240)
			continue;

		if (node.second->IsEnabled() == false)
			continue;

		Result res = node.second->CalculatePReqPayloadLimit();
		if (!res.IsSuccessful())
			return res;

		std::shared_ptr<SubObject> preqActPayloadLimitObj;
		res = node.second->GetSubObject(0x1F98, 0x4, preqActPayloadLimitObj);
		if (!res.IsSuccessful())
			return res;

		std::stringstream preqActPayloadLimitValue;
		if (preqActPayloadLimitObj->HasActualValue())
			preqActPayloadLimitValue << preqActPayloadLimitObj->GetTypedActualValue<std::uint16_t>();
		else
			continue;

		res = mn->SetSubObjectActualValue(0x1F8B, node.first, preqActPayloadLimitValue.str());
		if (!res.IsSuccessful())
			return res;

		for (auto& rmn : nodeCollection)
		{
			if (rmn.first == 240)
				continue;

			if (rmn.second->IsEnabled() == false)
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

Result PlkConfiguration::DistributePResPayloadLimit(const std::map<std::uint8_t, std::shared_ptr<BaseNode>>& nodeCollection)
{
	//Get managing node
	auto& mn = nodeCollection.at(240);
	ClearActualValues(nodeCollection, mn, 0x1F8D);

	for (auto& node : nodeCollection)
	{
		if (node.second->IsEnabled() == false)
			continue;

		Result res = node.second->CalculatePResPayloadLimit();
		if (!res.IsSuccessful())
			return res;

		std::shared_ptr<SubObject> presActPayloadLimitObj;
		res = node.second->GetSubObject(0x1F98, 0x5, presActPayloadLimitObj);
		if (!res.IsSuccessful())
			return res;

		std::stringstream presActPayloadLimitValue;
		if (presActPayloadLimitObj->HasActualValue())
			presActPayloadLimitValue << presActPayloadLimitObj->GetTypedActualValue<std::uint16_t>();
		else
			continue;

		if (node.first != 240) //Skip writing 1F8D / 240 to MN
		{
			res = mn->SetSubObjectActualValue(0x1F8D, node.first, presActPayloadLimitValue.str()); //Write to MN
			if (!res.IsSuccessful())
				return res;
		}

		for (auto& cn : nodeCollection)
		{
			//Always distribute for RMNs
			if (std::dynamic_pointer_cast<ManagingNode>(cn.second))
			{
				res = cn.second->SetSubObjectActualValue(0x1F8D, node.first, presActPayloadLimitValue.str());
				if (!res.IsSuccessful())
					return res;
			}

			bool crossTrafficForNode = false;
			for (auto& object : cn.second->GetObjectDictionary())
			{
				if (object.first >= 0x1400 && object.first < 0x1500)
				{
					std::shared_ptr<SubObject> paramObj;
					//Check corresponding 0x14XX object for origin of Rx Data
					res = object.second->GetSubObject(0x1, paramObj);
					if (!res.IsSuccessful())
						return res; //0x14XX/ 0x1 NodeID_U8 does not exist

					if (!paramObj->WriteToConfiguration())
						continue;

					//Only distribute the PresPayloadLimit for nodes that have cross traffic with the current node
					if (paramObj->GetTypedActualValue<std::uint16_t>() == node.first)
					{
						cn.second->SetSubObjectActualValue(0x1F8D, node.first, presActPayloadLimitValue.str());
						if (!std::dynamic_pointer_cast<ManagingNode>(cn.second))
							cn.second->SetSubObjectActualValue(0x1F81, node.first, IntToHex<std::uint32_t>(node.second->GetNodeAssignmentValue(), 8, "0x")); //Set actual value with assignment
						crossTrafficForNode = true;
					}
				}
			}
			if (crossTrafficForNode == false)
			{
				cn.second->SetSubObjectActualValue(0x1F8D, node.first, "");
				if (!std::dynamic_pointer_cast<ManagingNode>(cn.second))
					cn.second->ForceSubObject(0x1F81, node.first, false, false, "", false);
			}
		}
	}
	return Result();
}

Result PlkConfiguration::DistributeCNLossObjects(const std::map<std::uint8_t, std::shared_ptr<BaseNode>>& nodeCollection)
{
	for (auto& node : nodeCollection)
	{
		if (std::dynamic_pointer_cast<ControlledNode>(node.second))
		{
			if (node.first == 240)
				continue;

			if (node.second->IsEnabled() == false)
				continue;

			std::shared_ptr<SubObject> lossOfObject;
			Result res = node.second->GetSubObject(0x1C0B, 0x3, lossOfObject);
			if (!res.IsSuccessful())
				return res; //Mandatory Object

			if (!lossOfObject->WriteToConfiguration()) //Write default value only if no actualValue exists
			{
				//Set every node 0x1C0B / 0x3 actual value
				res = lossOfObject->SetTypedObjectActualValue(IntToHex((std::uint16_t) 80, 2, "0x"));
				if (!res.IsSuccessful())
					return res; //Mandatory Object
			}

			res = node.second->GetSubObject(0x1C0C, 0x3, lossOfObject, false);
			if (res.IsSuccessful()) //Not mandatory ignore error
			{
				if (!lossOfObject->WriteToConfiguration()) //Write default value only if no actualValue exists
				{
					//Set every node 0x1C0C/ 0x3 actual value
					lossOfObject->SetTypedObjectActualValue(IntToHex((std::uint16_t) 80, 2, "0x"));
				}
			}

			res = node.second->GetSubObject(0x1C0d, 0x3, lossOfObject, false);
			if (res.IsSuccessful()) //Not mandatoy ignore error
			{
				if (!lossOfObject->WriteToConfiguration()) //Write default value only if no actualValue exist
				{
					//Set every node 0x1C0D / 0x3 actual value
					res = lossOfObject->SetTypedObjectActualValue(IntToHex((std::uint16_t) 80, 2, "0x"));
				}
			}
		}
	}
	return Result();
}

Result PlkConfiguration::SyncRedundantManagingNodes(const std::map<std::uint8_t, std::shared_ptr<BaseNode>>& nodeCollection)
{
	//Get managing node
	std::shared_ptr<ManagingNode> mn = std::dynamic_pointer_cast<ManagingNode>(nodeCollection.at(240));
	if (!mn)
		return Result(ErrorCode::NO_MANAGING_NODE_CONFIGURED);

	for (auto& node : nodeCollection)
	{
		if (node.first == 240)
			continue;

		//Distribute RMNs
		std::shared_ptr<ManagingNode> rmn = std::dynamic_pointer_cast<ManagingNode>(node.second);
		if (rmn)
		{
			//Set RMN support on MN and distribute to all RMNs
			mn->SetObjectActualValue(0x1F80, "0x4800");
			ClearActualValues(nodeCollection, rmn, 0x1F92);
			ClearActualValues(nodeCollection, rmn, 0x1F8B);
			ClearActualValues(nodeCollection, rmn, 0x1F8D);
			rmn->ClearMappingObjects();

			for (auto& obj : mn->GetObjectDictionary())
			{
				if (obj.first == 0x1F81) //Skip node assignments
					continue;

				if (obj.second->HasActualValue())
				{
					Result res = node.second->SetObjectActualValue(obj.first, "0x" + obj.second->GetTypedActualValue<std::string>());
					if (!res.IsSuccessful())
						return res;
				}
				for (auto& subObj : obj.second->GetSubObjectDictionary())
				{
					if (subObj.second->HasActualValue())
					{
						Result res = node.second->SetSubObjectActualValue(obj.first, subObj.first, "0x" + subObj.second->GetTypedActualValue<std::string>());
						if (!res.IsSuccessful())
							return res;
					}
				}
			}

			//Sync MN PI
			rmn->GetTransmitProcessImage().clear();
			rmn->GetReceiveProcessImage().clear();
			for (auto& rxPi : mn->GetReceiveProcessImage())
			{
				rmn->GetReceiveProcessImage().push_back(rxPi);
			}
			for (auto& txPi : mn->GetTransmitProcessImage())
			{
				rmn->GetTransmitProcessImage().push_back(txPi);
			}

			//MN transmits PRes RMN should too
			if (std::find(mn->GetNodeAssignment().begin(), mn->GetNodeAssignment().end(), NodeAssignment::NMT_NODEASSIGN_MN_PRES) != mn->GetNodeAssignment().end())
				rmn->AddNodeAssignment(NodeAssignment::NMT_NODEASSIGN_MN_PRES);
			else
				rmn->RemoveNodeAssignment(NodeAssignment::NMT_NODEASSIGN_MN_PRES);

			//Sync RMN List
			rmn->ClearRmnList();
			for (auto id : mn->GetRmnList())
			{
				rmn->AddRmnId(id);
			}
		}
	}
	return Result();
}

Result PlkConfiguration::DistributeIpDefaultGateway(const std::map<std::uint8_t, std::shared_ptr<BaseNode>>& nodeCollection)
{
	//Get managing node
	auto& mn = nodeCollection.at(240);
	std::shared_ptr<SubObject> defaultGatewayObj;

	//Get IpAddrTableObj object
	Result res = mn->GetSubObject(0x1E40, 0x5, defaultGatewayObj);
	if (!res.IsSuccessful())
		return Result();

	for (auto& node : nodeCollection)
	{
		if (node.first == 240)
			continue;

		if (node.second->IsEnabled() == false)
			continue;

		if (defaultGatewayObj->WriteToConfiguration())
		{
			std::shared_ptr<SubObject> cnDefaultGatewayObj;
			res = node.second->GetSubObject(0x1E40, 0x5, cnDefaultGatewayObj, false);
			if (res.GetErrorType() == ErrorCode::OBJECT_DOES_NOT_EXIST)
				return Result();

			//Set every node 0x1E40 / 0x5 actual value to DefaultGateway
			res = node.second->SetSubObjectActualValue(0x1E40, 0x5, "0x" + defaultGatewayObj->GetTypedActualValue<std::string>());
			if (!res.IsSuccessful())
				return Result();
		}
	}
	return Result();
}

Result PlkConfiguration::ClearActualValues(const std::map<std::uint8_t, std::shared_ptr<BaseNode>>& nodeCollection, std::shared_ptr<IndustrialNetwork::POWERLINK::Core::Node::BaseNode> node, std::uint32_t objectId)
{
	std::shared_ptr<Object> obj;
	Result res = node->GetObject(objectId, obj);
	if (!res.IsSuccessful())
		return res;

	for (auto& subObj : obj->GetSubObjectDictionary())
	{
		if (nodeCollection.find((std::uint8_t) subObj.first) != nodeCollection.end())
			continue;

		if (subObj.second->HasActualValue() && subObj.second->GetForceToCDC() == false)
			subObj.second->ClearActualValue();
	}
	return Result();
}
