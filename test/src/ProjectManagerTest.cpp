/************************************************************************
\file ProjectManagerTest.cpp
\brief Implementation of the Class ProjectManagerTest
\author rueckerc, Bernecker+Rainer Industrie Elektronik Ges.m.b.H.
\date 04-Feb-2015 09:38:12
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
#include <iostream>
#include "ProjectManagerTest.h"

using namespace IndustrialNetwork::POWERLINK::Core::Test;
using namespace IndustrialNetwork::POWERLINK::Core::NetworkHandling;
using namespace IndustrialNetwork::POWERLINK::Core::ErrorHandling;
using namespace IndustrialNetwork::POWERLINK::Core::Configuration;

CPPUNIT_TEST_SUITE_REGISTRATION(ProjectManagerTest);

ProjectManagerTest::ProjectManagerTest(void)
{}

ProjectManagerTest::~ProjectManagerTest(void)
{}

void ProjectManagerTest::setUp()
{
	shared_ptr<Network> testNetwork = make_shared<Network>("testNetwork");
	this->test_uuid = testNetwork.get()->GetNetworkId();
	shared_ptr<Network> secondTestNework = make_shared<Network>("secondTestNework");
	this->test_uuid = secondTestNework.get()->GetNetworkId();

	Result res = ProjectManager::GetInstance().AddNetwork(testNetwork.get()->GetNetworkId(), testNetwork);
	CPPUNIT_ASSERT_EQUAL(true, res.IsSuccessful());
	res = ProjectManager::GetInstance().AddNetwork(secondTestNework.get()->GetNetworkId(), secondTestNework);
	CPPUNIT_ASSERT_EQUAL(true, res.IsSuccessful());
}

void ProjectManagerTest::tearDown()
{
	Result res = ProjectManager::GetInstance().ClearNetworkList();
	CPPUNIT_ASSERT_EQUAL(true, res.IsSuccessful());
}

void ProjectManagerTest::testAddNetwork()
{
	std::map<string, shared_ptr<Network>> networkList;
	Result res = ProjectManager::GetInstance().GetNetworks(networkList);
	CPPUNIT_ASSERT_EQUAL(true, res.IsSuccessful());
	auto size = networkList.size();
	CPPUNIT_ASSERT_EQUAL((unsigned int) size, (unsigned int) 2);
}

void ProjectManagerTest::testGetNetwork(void)
{
	shared_ptr<Network> networkConf;
	Result res = ProjectManager::GetInstance().GetNetwork(test_uuid, networkConf);
	CPPUNIT_ASSERT_EQUAL(true, res.IsSuccessful());
	CPPUNIT_ASSERT_EQUAL(test_uuid, networkConf.get()->GetNetworkId());
}

void ProjectManagerTest::testRemoveNetwork()
{
	Result res = ProjectManager::GetInstance().RemoveNetwork("testNetwork");
	CPPUNIT_ASSERT_EQUAL(true, res.IsSuccessful());

	std::map<string, shared_ptr<Network>> networkList;
	res = ProjectManager::GetInstance().GetNetworks(networkList);
	CPPUNIT_ASSERT_EQUAL(true, res.IsSuccessful());
	auto size = networkList.size();
	CPPUNIT_ASSERT_EQUAL((unsigned int) size, (unsigned int) 1);
}

void ProjectManagerTest::testGetNetworks(void)
{
	std::map<string, shared_ptr<Network>> networkList;
	Result res = ProjectManager::GetInstance().GetNetworks(networkList);
	CPPUNIT_ASSERT_EQUAL(true, res.IsSuccessful());
	auto size = networkList.size();
	CPPUNIT_ASSERT_EQUAL((unsigned int) size, (unsigned int) 2);
}