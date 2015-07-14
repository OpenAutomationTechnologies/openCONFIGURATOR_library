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

ConfigurationGenerator::ConfigurationGenerator()
{}

ConfigurationGenerator::~ConfigurationGenerator()
{}

ConfigurationGenerator& ConfigurationGenerator::GetInstance()
{
	static ConfigurationGenerator instance;
	return instance;
}

Result ConfigurationGenerator::GenerateNetworkConfiguration(const shared_ptr<Network>& net, stringstream& configurationOutput)
{
	Result res = WriteManagingNodeObjectCount(net, configurationOutput);
	if (!res.IsSuccessful())
		return res;
	res = WriteNodeAssignement(net, configurationOutput, false);
	if (!res.IsSuccessful())
		return res;
	res = WriteManagingNodeConfiguration(net, configurationOutput);
	if (!res.IsSuccessful())
		return res;
	//TODO: Check if redundant managing node exists and change behaviour

	map<uint8_t, shared_ptr<BaseNode>> nodes;
	res = net->GetNodes(nodes);
	if (res.IsSuccessful())
	{
		for (auto& node : nodes)
		{
			res = WriteControlledNodeConfiguration(node.second, configurationOutput);
			if (!res.IsSuccessful())
				return res;
		}
	}
	res = WriteNodeAssignement(net, configurationOutput, true);
	if (!res.IsSuccessful())
		return res;

	LOG_INFO() << configurationOutput.str();

	return res;
}

Result ConfigurationGenerator::WriteManagingNodeObjectCount(const shared_ptr<Network>& net, stringstream& configurationOutput)
{
	//Write Managing Node Object Count
	shared_ptr<ManagingNode> mn;
	Result res = net->GetManagingNode(mn);
	if (res.IsSuccessful())
	{
		configurationOutput << setfill('0') << setw(8) << hex << uppercase << mn->GetConfigurationObjectCount() << endl;
	}
	return res;
}

Result ConfigurationGenerator::WriteNodeAssignement(const shared_ptr<Network>& net, stringstream& configurationOutput, bool writeNodeValid)
{
	map<uint8_t, shared_ptr<BaseNode>> nodes;
	Result res = net->GetNodes(nodes);
	if (res.IsSuccessful())
	{
		for (auto& node : nodes)
		{
			if (node.first != 240)
			{
				if (!writeNodeValid)
					node.second->RemoveNodeAssignment(NodeAssignment::MNT_NODEASSIGN_VALID);

				if (writeNodeValid)
					configurationOutput << "//// NodeId Reassignment" << endl;
				else
					configurationOutput << "//// NodeId Assignment" << endl;

				configurationOutput << hex << uppercase << 0x1F81;
				configurationOutput << "\t";
				configurationOutput << hex << uppercase << setw(2) << setfill('0') << (uint32_t) node.first;
				configurationOutput << "\t";
				configurationOutput << hex << uppercase << setw(8) << setfill('0') << 0x4;
				configurationOutput << "\t";
				configurationOutput << hex << uppercase << setw(8) << setfill('0') << node.second->GetNodeAssignmentValue() << endl;

				if (!writeNodeValid)
					node.second->AddNodeAssignement(NodeAssignment::MNT_NODEASSIGN_VALID);


			}
		}
	}
	configurationOutput << endl;

	return res;
}

Result ConfigurationGenerator::WriteManagingNodeConfiguration(const shared_ptr<Network>& net, stringstream& configurationOutput)
{
	//Write Managing Node Object Count
	shared_ptr<ManagingNode> mn;
	Result res = net->GetManagingNode(mn);
	if (res.IsSuccessful())
	{
		WriteMappingNrOfEntriesZero(mn, configurationOutput);
		WriteCommunicationProfileArea(mn, configurationOutput);
		WriteMappingObjects(mn, configurationOutput);
		WriteManufacturerSpecificProfileArea(mn, configurationOutput);
		WriteMappingNrOfEntries(mn, configurationOutput);
	}
	return res;
}

Result ConfigurationGenerator::WriteControlledNodeConfiguration(const shared_ptr<BaseNode>& node, stringstream& configurationOutput)
{
	auto cn = dynamic_pointer_cast<ControlledNode>(node);
	if (cn.use_count() == 0)
		return Result();

	configurationOutput << "////Configuration Data for CN: " << node->GetName() << "(" << dec << (uint32_t) node->GetNodeIdentifier() << ")"<< endl;
	configurationOutput << hex << uppercase << "1F22";
	configurationOutput << "\t";
	configurationOutput << hex << uppercase << setw(2) << setfill('0') << (uint32_t) node->GetNodeIdentifier();
	configurationOutput << "\t";
	configurationOutput << hex << uppercase << setw(8) << setfill('0') << cn->GetConfigurationObjectSize() << endl;
	configurationOutput << hex << uppercase << setw(8) << setfill('0') << cn->GetConfigurationObjectCount() << endl;

	WriteMappingNrOfEntriesZero(node, configurationOutput);
	WriteCommunicationProfileArea(node, configurationOutput);
	WriteManufacturerSpecificProfileArea(node, configurationOutput);
	WriteMappingObjects(node, configurationOutput);
	WriteMappingNrOfEntries(node, configurationOutput);

	configurationOutput << endl;

	return Result();
}

Result ConfigurationGenerator::WriteMappingNrOfEntriesZero(const shared_ptr<BaseNode>& node, stringstream& configurationOutput)
{
	for (auto& object : node->GetObjectDictionary())
	{
		if ((object.first >= 0x1600 && object.first <= 0x16FF) || (object.first >= 0x1A00 && object.first <= 0x1AFF))
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
				}
			}
		}
	}
	return Result();
}

Result ConfigurationGenerator::WriteMappingObjects(const shared_ptr<BaseNode>& node, stringstream& configurationOutput)
{
	for (auto& object : node->GetObjectDictionary())
	{
		if ((object.first >= 0x1600 && object.first <= 0x16FF) || (object.first >= 0x1A00 && object.first <= 0x1AFF))
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

					count++;
				}
			}
		}
	}
	return Result();
}

Result ConfigurationGenerator::WriteMappingNrOfEntries(const shared_ptr<BaseNode>& node, stringstream& configurationOutput)
{
	for (auto& object : node->GetObjectDictionary())
	{
		if ((object.first >= 0x1600 && object.first <= 0x16FF) || (object.first >= 0x1A00 && object.first <= 0x1AFF))
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
				}
			}
		}
	}
	return Result();
}

Result ConfigurationGenerator::WriteCommunicationProfileArea(const shared_ptr<BaseNode>& node, stringstream& configurationOutput)
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
				}
			}
		}
	}
	return Result();
}

Result ConfigurationGenerator::WriteManufacturerSpecificProfileArea(const shared_ptr<BaseNode>& node, stringstream& configurationOutput)
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
				}
			}
		}
	}
	return Result();
}