/************************************************************************
\file ConfigurationGenerator.cpp
\brief Implementation of the Class ConfigurationGenerator
\author rueckerc, Bernecker+Rainer Industrie Elektronik Ges.m.b.H.
\date 24-Feb-2015 13:06:38
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
#include "ConfigurationGenerator.h"

using namespace std;
using namespace IndustrialNetwork::POWERLINK::Core::NetworkHandling;
using namespace IndustrialNetwork::POWERLINK::Core::ErrorHandling;
using namespace IndustrialNetwork::POWERLINK::Core::Node;
using namespace IndustrialNetwork::POWERLINK::Core::ObjectDictionary;
using namespace IndustrialNetwork::POWERLINK::Core::CoreConfiguration;
using namespace IndustrialNetwork::POWERLINK::Core::Utilities;

ConfigurationGenerator::ConfigurationGenerator()
{}

ConfigurationGenerator::~ConfigurationGenerator()
{}

ConfigurationGenerator& ConfigurationGenerator::GetInstance()
{
	static ConfigurationGenerator instance;
	return instance;
}

Result ConfigurationGenerator::GenerateNetworkConfiguration(const shared_ptr<Network>& net, stringstream& configurationOutput, stringstream& hexOutput)
{
	//Write the active managing node object count including Assignments and 1F22 objects
	Result res = WriteManagingNodeObjectCount(net, configurationOutput, hexOutput);
	if (!res.IsSuccessful())
		return res;

	//Write the node assignments
	res = WriteNodeAssignement(net, configurationOutput, hexOutput, false, true);
	if (!res.IsSuccessful())
		return res;

	//Write the managing node configuration
	res = WriteManagingNodeConfiguration(net, configurationOutput, hexOutput);
	if (!res.IsSuccessful())
		return res;

	map<uint8_t, shared_ptr<BaseNode>> nodes;
	res = net->GetNodes(nodes);
	if (!res.IsSuccessful())
		return res;

	//Write CN configuration including RMNs
	for (auto& node : nodes)
	{
		if (node.first == 240) //skip MN already written
			continue;

		//Write controlled node or RMN configuration
		res = WriteControlledNodeConfiguration(net, node.second, configurationOutput, hexOutput);
		if (!res.IsSuccessful())
			return res;
	}

	//Write the node reassignment
	res = WriteNodeAssignement(net, configurationOutput, hexOutput, true, true);
	return res;
}

Result ConfigurationGenerator::WriteManagingNodeObjectCount(const shared_ptr<Network>& net, stringstream& configurationOutput, stringstream& hexOutput)
{
	//Write Managing Node Object Count
	shared_ptr<ManagingNode> mn;
	Result res = net->GetManagingNode(mn);
	if (res.IsSuccessful())
	{
		configurationOutput << setfill('0') << setw(8) << hex << uppercase << mn->GetConfigurationObjectCount() << endl;
		hexOutput << setfill('0') << setw(8) << hex  << uppercase << left << ReverseHex(mn->GetConfigurationObjectCount());
	}
	return res;
}

Result ConfigurationGenerator::WriteNodeAssignement(const shared_ptr<Network>& net, stringstream& configurationOutput, stringstream& hexOutput, bool writeNodeValid, bool writeComments)
{
	map<uint8_t, shared_ptr<BaseNode>> nodes;
	Result res = net->GetNodes(nodes);
	if (!res.IsSuccessful())
		return res;

	for (auto& node : nodes)
	{
		//Determine if Node is RMN
		if (dynamic_pointer_cast<ManagingNode>(node.second))
		{
			//Skip if node is RMN and write Reassignment or node is AMN
			if ((writeNodeValid && node.second->GetNodeIdentifier() != 240)
			        || node.second->GetNodeIdentifier() == 240) //Do not write Node Reassignment for RMNscontinue;
				continue;
		}

		//Write Assignment with no node valid bit therefore remove the bit first
		if (!writeNodeValid)
			node.second->RemoveNodeAssignment(NodeAssignment::MNT_NODEASSIGN_VALID);

		//Write comments according to writeComments flag
		if (!writeNodeValid && writeComments)
			configurationOutput << "//// NodeId Assignment" << endl;
		else if (writeNodeValid && writeComments)
			configurationOutput << "//// NodeId Reassignment" << endl;


		configurationOutput << hex << uppercase << 0x1F81;
		configurationOutput << "\t";
		configurationOutput << hex << uppercase << setw(2) << setfill('0') << (uint32_t) node.first;
		configurationOutput << "\t";
		configurationOutput << hex << uppercase << setw(8) << setfill('0') << 0x4;
		configurationOutput << "\t";
		configurationOutput << hex << uppercase << setw(8) << setfill('0') << node.second->GetNodeAssignmentValue() << endl;

		hexOutput << hex << uppercase << ReverseHex(0x1F81);
		hexOutput << hex << uppercase << setw(2) << setfill('0') << left << ReverseHex((uint32_t) node.first);
		hexOutput << hex << uppercase << setw(8) << setfill('0') << left << 0x4;
		hexOutput << hex << uppercase << setw(8) << setfill('0') << left << ReverseHex(node.second->GetNodeAssignmentValue());

		//If valid bit has been remove add it again
		if (!writeNodeValid)
			node.second->AddNodeAssignement(NodeAssignment::MNT_NODEASSIGN_VALID);
	}

	configurationOutput << endl;

	return res;
}

Result ConfigurationGenerator::WriteManagingNodeConfiguration(const shared_ptr<Network>& net, stringstream& configurationOutput, stringstream& hexOutput)
{
	//Write Managing Node Object Count
	shared_ptr<ManagingNode> mn;
	Result res = net->GetManagingNode(mn);
	if (!res.IsSuccessful())
		return res;

	//Reset Mapping objects
	res = WriteMappingNrOfEntriesZero(mn, configurationOutput, hexOutput);
	if (!res.IsSuccessful())
		return res;

	//Write Communication Profile area
	res = WriteCommunicationProfileArea(mn, configurationOutput, hexOutput);
	if (!res.IsSuccessful())
		return res;

	//Write Mapping objects
	res = WriteMappingObjects(mn, configurationOutput, hexOutput);
	if (!res.IsSuccessful())
		return res;

	//Write manufacturer specific objects
	res = WriteManufacturerSpecificProfileArea(mn, configurationOutput, hexOutput);
	if (!res.IsSuccessful())
		return res;

	//Write mapping number of entries
	res = WriteMappingNrOfEntries(mn, configurationOutput, hexOutput);
	return res;
}

Result ConfigurationGenerator::WriteRedundantManagingNodeConfiguration(const shared_ptr<Network>& net, const shared_ptr<BaseNode>& node, stringstream& configurationOutput, stringstream& hexOutput)
{
	auto rmn = dynamic_pointer_cast<ManagingNode>(node); //This should not fail because it can either be MN or CN

	//Write the 1F22 object for RMN
	configurationOutput << "////Configuration Data for CN: " << node->GetName() << "(" << dec << (uint32_t) node->GetNodeIdentifier() << ")" << endl;
	configurationOutput << hex << uppercase << 0x1F22;
	configurationOutput << "\t";
	configurationOutput << hex << uppercase << setw(2) << setfill('0') << (uint32_t) node->GetNodeIdentifier();
	configurationOutput << "\t";
	configurationOutput << hex << uppercase << setw(8) << setfill('0') << rmn->GetConfigurationObjectSize() << endl;
	configurationOutput << hex << uppercase << setw(8) << setfill('0') << rmn->GetConfigurationObjectCount() << endl;

	hexOutput << hex << uppercase << ReverseHex(0x1F22);
	hexOutput << hex << uppercase << setw(2) << setfill('0') << left << ReverseHex((uint32_t) node->GetNodeIdentifier());
	hexOutput << hex << uppercase << setw(8) << setfill('0') << left << ReverseHex(rmn->GetConfigurationObjectSize());
	hexOutput << hex << uppercase << setw(8) << setfill('0') << left << ReverseHex(rmn->GetConfigurationObjectCount());

	Result res = WriteNodeAssignement(net, configurationOutput, hexOutput, false, false);
	if (!res.IsSuccessful())
		return res;

	res = WriteMappingNrOfEntriesZero(rmn, configurationOutput, hexOutput);
	if (!res.IsSuccessful())
		return res;

	res = WriteCommunicationProfileArea(rmn, configurationOutput, hexOutput);
	if (!res.IsSuccessful())
		return res;

	res = WriteMappingObjects(rmn, configurationOutput, hexOutput);
	if (!res.IsSuccessful())
		return res;

	res = WriteManufacturerSpecificProfileArea(rmn, configurationOutput, hexOutput);
	if (!res.IsSuccessful())
		return res;

	res = WriteMappingNrOfEntries(rmn, configurationOutput, hexOutput);
	if (!res.IsSuccessful())
		return res;

	res = WriteNodeAssignement(net, configurationOutput, hexOutput, true, false);
	return res;
}

Result ConfigurationGenerator::WriteControlledNodeConfiguration(const shared_ptr<Network>& net, const shared_ptr<BaseNode>& node, stringstream& configurationOutput, stringstream& hexOutput)
{
	auto cn = dynamic_pointer_cast<ControlledNode>(node);
	if (!cn) //If cast fails this is an RMN
		return WriteRedundantManagingNodeConfiguration(net, node, configurationOutput, hexOutput);

	configurationOutput << "////Configuration Data for CN: " << node->GetName() << "(" << dec << (uint32_t) node->GetNodeIdentifier() << ")" << endl;
	configurationOutput << hex << uppercase << "1F22";
	configurationOutput << "\t";
	configurationOutput << hex << uppercase << setw(2) << setfill('0') << (uint32_t) node->GetNodeIdentifier();
	configurationOutput << "\t";
	configurationOutput << hex << uppercase << setw(8) << setfill('0') << cn->GetConfigurationObjectSize() << endl;
	configurationOutput << hex << uppercase << setw(8) << setfill('0') << cn->GetConfigurationObjectCount() << endl;

	hexOutput << hex << uppercase << ReverseHex(0x1F22);
	hexOutput << hex << uppercase << setw(2) << setfill('0') << left << ReverseHex((uint32_t) node->GetNodeIdentifier());
	hexOutput << hex << uppercase << setw(8) << setfill('0') << left << ReverseHex(cn->GetConfigurationObjectSize());
	hexOutput << hex << uppercase << setw(8) << setfill('0') << left << ReverseHex(cn->GetConfigurationObjectCount());

	Result res = WriteMappingNrOfEntriesZero(node, configurationOutput, hexOutput);
	if (!res.IsSuccessful())
		return res;

	res = WriteCommunicationProfileArea(node, configurationOutput, hexOutput);
	if (!res.IsSuccessful())
		return res;

	res = WriteManufacturerSpecificProfileArea(node, configurationOutput, hexOutput);
	if (!res.IsSuccessful())
		return res;

	res = WriteMappingObjects(node, configurationOutput, hexOutput);
	if (!res.IsSuccessful())
		return res;

	res = WriteMappingNrOfEntries(node, configurationOutput, hexOutput);
	configurationOutput << endl;

	return res;
}

Result ConfigurationGenerator::WriteMappingNrOfEntriesZero(const shared_ptr<BaseNode>& node, stringstream& configurationOutput, stringstream& hexOutput)
{
	for (auto& object : node->GetObjectDictionary())
	{
		if ((object.first >= 0x1600 && object.first < 0x1700) || (object.first >= 0x1A00 && object.first < 0x1B00))
		{
			auto subobject = object.second->GetSubObjectCollection().find((uint32_t) 0);
			if (subobject != object.second->GetSubObjectCollection().end())
			{
				if (subobject->second->WriteToConfiguration())
				{
					configurationOutput << hex << uppercase << object.first;
					configurationOutput << "\t";
					configurationOutput << hex << uppercase << setw(2) << setfill('0') << subobject->first;
					configurationOutput << "\t";
					configurationOutput << hex << uppercase << setw(8) << setfill('0') << (subobject->second->GetBitSize() / 8);
					configurationOutput << "\t";
					configurationOutput << hex << uppercase << setw(2) << setfill('0') << 0 << endl;

					hexOutput << hex << uppercase << ReverseHex(object.first);
					hexOutput << hex << uppercase << setw(2) << setfill('0') << left << ReverseHex(subobject->first);
					hexOutput << hex << uppercase << setw(8) << setfill('0') << left << ReverseHex(subobject->second->GetBitSize() / 8);
					hexOutput << hex << uppercase << setw(2) << setfill('0') << left << 0;

				}
			}
		}
	}
	return Result();
}

Result ConfigurationGenerator::WriteMappingObjects(const shared_ptr<BaseNode>& node, stringstream& configurationOutput, stringstream& hexOutput)
{
	for (auto& object : node->GetObjectDictionary())
	{
		if ((object.first >= 0x1800 && object.first < 0x1900)
		        || (object.first >= 0x1400 && object.first < 0x1500))
		{
			for (auto& subobject : object.second->GetSubObjectCollection())
			{
				if (subobject.second->WriteToConfiguration() && subobject.first != 0)
				{
					configurationOutput << hex << uppercase << object.first;
					configurationOutput << "\t";
					configurationOutput << hex << uppercase << setw(2) << setfill('0') << subobject.first;
					configurationOutput << "\t";
					configurationOutput << hex << uppercase << setw(8) << setfill('0') << (subobject.second->GetBitSize() / 8);
					configurationOutput << "\t";
					configurationOutput << subobject.second->GetTypedActualValue<string>() << endl;

					hexOutput << hex << uppercase << ReverseHex(object.first);
					hexOutput << hex << uppercase << setw(2) << setfill('0') << left << ReverseHex(subobject.first);
					hexOutput << hex << uppercase << setw(8) << setfill('0') << left << ReverseHex(subobject.second->GetBitSize() / 8);
					hexOutput << ReverseHex(subobject.second->GetTypedActualValue<string>());
				}
			}
		}
		else if ((object.first >= 0x1600 && object.first < 0x1700)
		         || (object.first >= 0x1A00 && object.first < 0x1B00))
		{
			uint16_t numberOfIndicesToWrite = 0;
			auto& subobject = object.second->GetSubObjectCollection().at((uint8_t) 0);
			if (subobject->WriteToConfiguration())
				numberOfIndicesToWrite = subobject->GetTypedActualValue<uint16_t>();

			uint16_t count = 0;
			for (auto& subobject : object.second->GetSubObjectCollection())
			{
				if (subobject.second->WriteToConfiguration() && subobject.first != 0 && count < numberOfIndicesToWrite)
				{
					configurationOutput << hex << uppercase << object.first;
					configurationOutput << "\t";
					configurationOutput << hex << uppercase << setw(2) << setfill('0') << subobject.first;
					configurationOutput << "\t";
					configurationOutput << hex << uppercase << setw(8) << setfill('0') << (subobject.second->GetBitSize() / 8);
					configurationOutput << "\t";
					configurationOutput << subobject.second->GetTypedActualValue<string>() << endl;

					hexOutput << hex << uppercase << ReverseHex(object.first);
					hexOutput << hex << uppercase << setw(2) << setfill('0') << left << ReverseHex(subobject.first);
					hexOutput << hex << uppercase << setw(8) << setfill('0') << left << ReverseHex(subobject.second->GetBitSize() / 8);
					hexOutput << ReverseHex(subobject.second->GetTypedActualValue<string>());

					count++;
				}
			}
		}
	}
	return Result();
}

Result ConfigurationGenerator::WriteMappingNrOfEntries(const shared_ptr<BaseNode>& node, stringstream& configurationOutput, stringstream& hexOutput)
{
	for (auto& object : node->GetObjectDictionary())
	{
		if ((object.first >= 0x1600 && object.first < 0x1700) || (object.first >= 0x1A00 && object.first < 0x1B00))
		{
			auto subobject = object.second->GetSubObjectCollection().find((uint32_t) 0);
			if (subobject != object.second->GetSubObjectCollection().end())
			{
				if (subobject->second->WriteToConfiguration())
				{
					configurationOutput << hex << uppercase << object.first;
					configurationOutput << "\t";
					configurationOutput << hex << uppercase << setw(2) << setfill('0') << subobject->first;
					configurationOutput << "\t";
					configurationOutput << hex << uppercase << setw(8) << setfill('0') << (subobject->second->GetBitSize() / 8);
					configurationOutput << "\t";
					configurationOutput << hex << uppercase << subobject->second->GetTypedActualValue<string>() << endl;

					hexOutput << hex << uppercase << ReverseHex(object.first);
					hexOutput << hex << uppercase << setw(2) << setfill('0') << left << ReverseHex(subobject->first);
					hexOutput << hex << uppercase << setw(8) << setfill('0') << left << ReverseHex(subobject->second->GetBitSize() / 8);
					hexOutput << hex << uppercase << ReverseHex(subobject->second->GetTypedActualValue<string>());
				}
			}
		}
	}
	return Result();
}

Result ConfigurationGenerator::WriteCommunicationProfileArea(const shared_ptr<BaseNode>& node, stringstream& configurationOutput, stringstream& hexOutput)
{
	for (auto& object : node->GetObjectDictionary())
	{
		if ((object.first < 0x1600 || object.first > 0x16FF)
		        && (object.first < 0x1A00 || object.first > 0x1AFF)
		        && (object.first < 0x1400 || object.first > 0x14FF)
		        && (object.first < 0x1800 || object.first > 0x18FF)
		        && object.first < 0x2000
		        && object.first != 0x1F81)
		{
			for (auto& subobject : object.second->GetSubObjectCollection())
			{
				if (subobject.second->WriteToConfiguration())
				{
					configurationOutput << hex << uppercase << object.first;
					configurationOutput << "\t";
					configurationOutput << hex << uppercase << setw(2) << setfill('0') << subobject.first;
					configurationOutput << "\t";
					configurationOutput << hex << uppercase << setw(8) << setfill('0') << (subobject.second->GetBitSize() / 8);
					configurationOutput << "\t";
					configurationOutput << subobject.second->GetTypedActualValue<string>() << endl;

					hexOutput << hex << uppercase << ReverseHex(object.first);
					hexOutput << hex << uppercase << setw(2) << setfill('0') << left << ReverseHex(subobject.first);
					hexOutput << hex << uppercase << setw(8) << setfill('0') << left << ReverseHex(subobject.second->GetBitSize() / 8);
					hexOutput << ReverseHex(subobject.second->GetTypedActualValue<string>());
				}
			}
		}
	}
	return Result();
}

Result ConfigurationGenerator::WriteManufacturerSpecificProfileArea(const shared_ptr<BaseNode>& node, stringstream& configurationOutput, stringstream& hexOutput)
{
	for (auto& object : node->GetObjectDictionary())
	{
		if (object.first >= 0x2000 && object.first <= 0x5FFF)
		{
			for (auto& subobject : object.second->GetSubObjectCollection())
			{
				if (subobject.second->WriteToConfiguration())
				{
					configurationOutput << hex << uppercase << object.first;
					configurationOutput << "\t";
					configurationOutput << hex << uppercase << setw(2) << setfill('0') << subobject.first;
					configurationOutput << "\t";
					configurationOutput << hex << uppercase << setw(8) << setfill('0') << (subobject.second->GetBitSize() / 8);
					configurationOutput << "\t";
					configurationOutput << subobject.second->GetTypedActualValue<string>() << endl;

					hexOutput << hex << uppercase << ReverseHex(object.first);
					hexOutput << hex << uppercase << setw(2) << setfill('0') << left << ReverseHex(subobject.first);
					hexOutput << hex << uppercase << setw(8) << setfill('0') << left << ReverseHex(subobject.second->GetBitSize() / 8);
					hexOutput << ReverseHex(subobject.second->GetTypedActualValue<string>());
				}
			}
		}
	}
	return Result();
}