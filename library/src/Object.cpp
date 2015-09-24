/************************************************************************
\file Index.cpp
\brief Implementation of the Class Index
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
#include "Object.h"

using namespace IndustrialNetwork::POWERLINK::Core::Utilities;
using namespace IndustrialNetwork::POWERLINK::Core::ErrorHandling;
using namespace IndustrialNetwork::POWERLINK::Core::CoreConfiguration;

namespace IndustrialNetwork
{
	namespace POWERLINK
	{
		namespace Core
		{
			namespace ObjectDictionary
			{
				Object::Object(std::uint32_t id, ObjectType objectType, const std::string& name, std::uint8_t containingNode) : BaseObject(id, objectType, name, containingNode),
					subIndexCollection(std::map<uint32_t, std::shared_ptr<SubObject>>())
				{}

				Object::Object(std::uint32_t id, ObjectType objectType, const std::string& name, std::uint8_t containingNode, PlkDataType dataType, AccessType accessType, PDOMapping pdoMapping) : BaseObject(id, objectType, name, containingNode, dataType, accessType, pdoMapping),
					subIndexCollection(std::map<uint32_t, std::shared_ptr<SubObject>>())
				{}

				Object::Object(std::uint32_t id, ObjectType objectType, const std::string& name, std::uint8_t containingNode, const std::string& uniqueIdRef) : BaseObject(id, objectType, name, containingNode, uniqueIdRef),
					subIndexCollection(std::map<std::uint32_t, std::shared_ptr<SubObject>>())
				{}

				Object::~Object()
				{}

				Result Object::AddSubobject(std::shared_ptr<SubObject>& ref)
				{
					if (this->subIndexCollection.find(ref->GetObjectId()) != this->subIndexCollection.end())
					{
						//SubObject does already exists
						boost::format formatter(kMsgExistingSubObject);
						formatter
						% this->GetObjectId()
						% ref->GetObjectId()
						% (std::uint32_t) this->GetContainingNode();
						LOG_ERROR() << formatter.str();
						return Result(ErrorCode::SUBOBJECT_EXISTS, formatter.str());
					}

					this->subIndexCollection.insert(std::pair<std::uint32_t, std::shared_ptr<SubObject>>(ref->GetObjectId(), ref));
					//Log info subobject created
					boost::format formatter(kMsgSubObjectCreated);
					formatter
					% this->GetObjectId()
					% ref->GetObjectId()
					% (std::uint32_t) this->GetContainingNode();
					LOG_INFO() << formatter.str();
					return Result();
				}

				Result Object::GetSubObject(std::uint32_t subObjectId, std::shared_ptr<SubObject>& ref, bool enableLog)
				{
					auto iter = this->subIndexCollection.find(subObjectId);
					if (iter == this->subIndexCollection.end())
					{
						//Subobject does not exist
						boost::format formatter(kMsgNonExistingSubObject);
						formatter
						% this->GetObjectId()
						% subObjectId
						% (std::uint32_t) this->GetContainingNode();
						if (enableLog)
						{
							LOG_ERROR() << formatter.str();
						}
						return Result(ErrorCode::SUBOBJECT_DOES_NOT_EXIST, formatter.str());
					}
					ref = iter->second;
					return Result();
				}

				std::map<std::uint32_t, std::shared_ptr<IndustrialNetwork::POWERLINK::Core::ObjectDictionary::SubObject>>& Object::GetSubObjectDictionary()
				{
					return this->subIndexCollection;
				}
			}
		}
	}
}