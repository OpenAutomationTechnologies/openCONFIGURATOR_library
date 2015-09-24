/************************************************************************
\file Object.h
\brief Implementation of the Class Object
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
#if !defined OBJECT_H
#define OBJECT_H

#include <map>
#include <memory>

#include <boost/format.hpp>

#include "Result.h"
#include "BaseObject.h"
#include "SubObject.h"
#include "Utilities.h"
#include "LoggingConfiguration.h"

namespace IndustrialNetwork
{
	namespace POWERLINK
	{
		namespace Core
		{
			namespace ObjectDictionary
			{
				/**
				\brief Represents an Object in the nodes object dictionary.
				\author rueckerc, Bernecker+Rainer Industrie Elektronik Ges.m.b.H.
				*/
				class DLLEXPORT Object : public IndustrialNetwork::POWERLINK::Core::ObjectDictionary::BaseObject
				{

					public:
						Object(std::uint32_t id, ObjectType objectType, const std::string& name, std::uint8_t containingNode);
						Object(std::uint32_t id, ObjectType objectType, const std::string&, std::uint8_t containingNode, PlkDataType dataType, AccessType accessType, PDOMapping pdoMapping);
						Object(std::uint32_t id, ObjectType objectType, const std::string&, std::uint8_t containingNode, const std::string& uniqueIdRef);

						virtual ~Object();
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result AddSubobject(std::shared_ptr<IndustrialNetwork::POWERLINK::Core::ObjectDictionary::SubObject>& ref);
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result GetSubObject(std::uint32_t subObjectId, std::shared_ptr<IndustrialNetwork::POWERLINK::Core::ObjectDictionary::SubObject>& ref, bool enableLog = true);

						std::map<std::uint32_t, std::shared_ptr<IndustrialNetwork::POWERLINK::Core::ObjectDictionary::SubObject>>& GetSubObjectDictionary();

					private:
						std::map<std::uint32_t, std::shared_ptr<IndustrialNetwork::POWERLINK::Core::ObjectDictionary::SubObject>> subIndexCollection;


				};
			}
		}
	}
}
#endif
