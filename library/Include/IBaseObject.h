/************************************************************************
\file IBaseObject.h
\brief Implementation of the Class IBaseObject
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
#if !defined IBASE_OBJECT_H
#define IBASE_OBJECT_H

#include <string>

#include <boost/any.hpp>

namespace IndustrialNetwork
{
	/**
	\brief Generic fieldbus related namespace
	\author rueckerc, Bernecker+Rainer Industrie Elektronik Ges.m.b.H.
	*/
	namespace Fieldbus
	{
		/**
		\brief Represents the template class for a Fieldbus object.
		\author rueckerc, Bernecker+Rainer Industrie Elektronik Ges.m.b.H.
		*/
		// T datatype for the object id
		// D datatype for the object datatype
		template<typename T, typename D>
		class IBaseObject
		{

			public:
				IBaseObject(T id, D objectType, const std::string& name) :
					id(id),
					actualValue(boost::any()),
					defaultValue(boost::any()),
					objectType(objectType),
					name(name)
				{}

				virtual ~IBaseObject()
				{}

				const T GetObjectId() const
				{
					return this->id;
				}

				void SetId(const T id)
				{
					this->id = id;
				}

				const std::string& GetName() const
				{
					return this->name;
				}

				void SetName(const std::string& name)
				{
					this->name = name;
				}

				const boost::any& GetActualValue() const
				{
					return this->actualValue;
				}

				void SetActualValue(const boost::any& actualValue)
				{
					this->actualValue = actualValue;
				}

				const boost::any& GetDefaultValue() const
				{
					return this->defaultValue;
				}

				void SetDefaultValue(const boost::any& defaultValue)
				{
					this->defaultValue = defaultValue;
				}

				const D GetObjectType() const
				{
					return this->objectType;
				}

				void SetObjectType(const D& dataType)
				{
					this->objectType = dataType;
				}

			private:
				T id;
				boost::any actualValue;
				boost::any defaultValue;
				D objectType;
				std::string name;
		};
	}
}
#endif
