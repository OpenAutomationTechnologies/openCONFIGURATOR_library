/************************************************************************
\file BaseObjectTest.cpp
\brief Implementation of the Class BaseObjectTest
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
#include "BaseObjectTest.h"

using namespace IndustrialNetwork::POWERLINK::Core::Test;

CPPUNIT_TEST_SUITE_REGISTRATION(BaseObjectTest);

BaseObjectTest::BaseObjectTest(void) :
	objectToTest(1000, PlkDataType::BOOLEAN)
{}

BaseObjectTest::~BaseObjectTest(void)
{}

void BaseObjectTest::setUp() {}

void BaseObjectTest::tearDown() {}

void BaseObjectTest::testEqualOperator(void)
{
	BaseObject equalTest(1000, PlkDataType::BOOLEAN);
	BaseObject newEqualTestObject(1000, PlkDataType::BOOLEAN);
	CPPUNIT_ASSERT_EQUAL(true, (newEqualTestObject == equalTest));
}
void BaseObjectTest::testGetDefaultValue()
{
	uint32_t value = 10000;
	objectToTest.SetObjectDefaultValue(value);
	CPPUNIT_ASSERT_EQUAL(objectToTest.GetObjectDefaultValue(), (unsigned int) 10000);
}
void BaseObjectTest::testSetDefaultValue()
{
	uint32_t value = 100;
	objectToTest.SetObjectDefaultValue(value);
	CPPUNIT_ASSERT_EQUAL(objectToTest.GetObjectDefaultValue(), (unsigned int) 100);
}
void BaseObjectTest::testForcedToCDC()
{
	objectToTest.SetForceToCDC(true);
	CPPUNIT_ASSERT_EQUAL(objectToTest.GetForceToCDC(), true);

}
void BaseObjectTest::testSetForceToCDC()
{
	objectToTest.SetForceToCDC(false);
	CPPUNIT_ASSERT_EQUAL(objectToTest.GetForceToCDC(), false);
}
void BaseObjectTest::testGetHighLimit()
{
	objectToTest.SetHighLimit(1000);
	CPPUNIT_ASSERT_EQUAL(objectToTest.GetHighLimit().get(), (unsigned int) 1000);

}
void BaseObjectTest::testSetHighLimit()
{

}
void BaseObjectTest::testGetUniqueIdRef()
{

}
void BaseObjectTest::testSetUniqueIdRef()
{

}
void BaseObjectTest::testGetAccessType()
{

}
void BaseObjectTest::testSetAccessType()
{

}
void BaseObjectTest::testGetObjectType()
{

}
void BaseObjectTest::testSetObjectType()
{

}
void BaseObjectTest::testGetPDOMapping()
{

}
void BaseObjectTest::testSetPDOMapping()
{

}
void BaseObjectTest::testGetDataType()
{

}
void BaseObjectTest::testSetDataType()
{

}