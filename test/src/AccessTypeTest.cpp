/************************************************************************
\file AccessTypeTest.cpp
\brief Implementation of the class AccessTypeTest
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
#include "AccessTypeTest.h"

using namespace IndustrialNetwork::POWERLINK::Core::Test;

CPPUNIT_TEST_SUITE_REGISTRATION(AccessTypeTest);

AccessTypeTest::AccessTypeTest(void)
{}

AccessTypeTest::~AccessTypeTest(void)
{}

void AccessTypeTest::setUp()
{}

void AccessTypeTest::tearDown()
{}

void AccessTypeTest::testInvalidAccessType()
{
	CPPUNIT_ASSERT_EQUAL(static_cast<int>(IndustrialNetwork::POWERLINK::Core::ObjectDictionary::AccessType::UNDEFINED), 0);
}
void AccessTypeTest::testRWAccessType()
{
	CPPUNIT_ASSERT_EQUAL(static_cast<int>(IndustrialNetwork::POWERLINK::Core::ObjectDictionary::AccessType::RW), 1);
}
void AccessTypeTest::testWOAccessType()
{
	CPPUNIT_ASSERT_EQUAL(static_cast<int>(IndustrialNetwork::POWERLINK::Core::ObjectDictionary::AccessType::WO), 3);
}
void AccessTypeTest::testROAccessType()
{
	CPPUNIT_ASSERT_EQUAL(static_cast<int>(IndustrialNetwork::POWERLINK::Core::ObjectDictionary::AccessType::RO), 5);
}
void AccessTypeTest::testConstAccessType()
{
	CPPUNIT_ASSERT_EQUAL(static_cast<int>(IndustrialNetwork::POWERLINK::Core::ObjectDictionary::AccessType::CONST), 6);
}
void AccessTypeTest::testRWSAccessType()
{
	CPPUNIT_ASSERT_EQUAL(static_cast<int>(IndustrialNetwork::POWERLINK::Core::ObjectDictionary::AccessType::RWS), 2);
}
void AccessTypeTest::testWOSAccessType()
{
	CPPUNIT_ASSERT_EQUAL(static_cast<int>(IndustrialNetwork::POWERLINK::Core::ObjectDictionary::AccessType::WOS), 4);
}