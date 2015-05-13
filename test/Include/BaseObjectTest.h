/************************************************************************
\file BaseObjectTest.h
\brief BaseObjectTest class
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
#pragma once
#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>
#include "BaseObject.h"

using namespace std;
using namespace IndustrialNetwork::POWERLINK::Core::ObjectDictionary;

namespace IndustrialNetwork
{
	namespace POWERLINK
	{
		namespace Core
		{
			namespace Test
			{
				class BaseObjectTest :
					public CppUnit::TestFixture
				{

						CPPUNIT_TEST_SUITE(BaseObjectTest);
						CPPUNIT_TEST(testEqualOperator);
						CPPUNIT_TEST(testGetDefaultValue);
						CPPUNIT_TEST(testSetDefaultValue);
						CPPUNIT_TEST(testForcedToCDC);
						CPPUNIT_TEST(testSetForceToCDC);
						CPPUNIT_TEST(testGetHighLimit);
						CPPUNIT_TEST(testSetHighLimit);
						CPPUNIT_TEST(testGetUniqueIdRef);
						CPPUNIT_TEST(testSetUniqueIdRef);
						CPPUNIT_TEST(testGetAccessType);
						CPPUNIT_TEST(testSetAccessType);
						CPPUNIT_TEST(testGetObjectType);
						CPPUNIT_TEST(testSetObjectType);
						CPPUNIT_TEST(testGetPDOMapping);
						CPPUNIT_TEST(testSetPDOMapping);
						CPPUNIT_TEST(testGetDataType);
						CPPUNIT_TEST(testSetDataType);
						CPPUNIT_TEST_SUITE_END();

					private:
						BaseObject objectToTest;

					public:
						BaseObjectTest(void);
						~BaseObjectTest(void);
						void setUp();
						void tearDown();

						void testEqualOperator();
						void testGetDefaultValue();
						void testSetDefaultValue();
						void testForcedToCDC();
						void testSetForceToCDC();
						void testGetHighLimit();
						void testSetHighLimit();
						void testGetUniqueIdRef();
						void testSetUniqueIdRef();
						void testGetAccessType();
						void testSetAccessType();
						void testGetObjectType();
						void testSetObjectType();
						void testGetPDOMapping();
						void testSetPDOMapping();
						void testGetDataType();
						void testSetDataType();

				};
			}
		}
	}
}