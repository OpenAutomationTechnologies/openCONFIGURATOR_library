/************************************************************************
\file SubIndex.cpp
\brief Implementation of the Class SubIndex
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
#include "SubObject.h"

using namespace std;
using namespace IndustrialNetwork::POWERLINK::Core::Utilities;

namespace IndustrialNetwork
{
	namespace POWERLINK
	{
		namespace Core
		{
			namespace ObjectDictionary
			{

				SubObject::SubObject(uint32_t id, PlkDataType type) : BaseObject(id, type)
				{}
				SubObject::SubObject(std::uint32_t id, std::string defaultValue, PlkDataType type, AccessType accessType, ObjectType objectType, PDOMapping pdoMapping, std::string name) : BaseObject(id, defaultValue, type, accessType, objectType, pdoMapping, name)
				{}
				SubObject::SubObject(std::uint32_t id, std::string defaultValue, PlkDataType type, AccessType accessType, ObjectType objectType, PDOMapping pdoMapping, std::uint32_t highlimit, std::uint32_t lowLimit, std::string uniqueIdRef, std::string name): BaseObject(id, defaultValue, type, accessType, objectType, pdoMapping, highlimit, lowLimit, uniqueIdRef, name)
				{}

				SubObject::~SubObject()
				{}

				template<>
				string SubObject::GetTypedActualValue<string>()
				{
					//return original stored value
					return this->GetActualValue();
				}

				template<>
				uint32_t SubObject::GetTypedActualValue<unsigned int>()
				{
					//Check for correct data type for returning a numeric value
					return HexToInt<unsigned int>(this->GetActualValue());
				}
			}
		}
	}
}