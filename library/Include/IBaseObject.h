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

namespace IndustrialNetwork
{
	namespace Fieldbus
	{
		/**
		\brief Represents the template class for the Fieldbus objects.
		\author rueckerc, Bernecker+Rainer Industrie Elektronik Ges.m.b.H.
		*/
		// T datatype for the object id
		// V datatype for the actual/default value
		// D datatype for the object datatype
		template<typename T, typename V, typename D>
		class IBaseObject
		{

			public:
				IBaseObject() :
					id(),
					actualValue(),
					defaultValue(),
					dataType(),
					name("")

				{}

				IBaseObject(T id, V defaultValue, D dataType, std::string name = "") :
					id(id),
					actualValue(),
					defaultValue(defaultValue),
					dataType(dataType),
					name(name)
				{}

				IBaseObject(T id) :
					id(id),
					actualValue(),
					defaultValue(),
					dataType(),
					name("")
				{}

				IBaseObject(T id, D dataType, std::string name = "") :
					id(id),
					actualValue(),
					defaultValue(),
					dataType(dataType),
					name(name)
				{}


				virtual ~IBaseObject()
				{}

				const T GetId() const
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

				void SetName(std::string& name)
				{
					this->name = name;
				}

				const V GetActualValue() const
				{
					return this->actualValue;
				}

				void SetActualValue(const V& actualValue)
				{
					this->actualValue = actualValue;
				}

				const V GetDefaultValue() const
				{
					return this->defaultValue;
				}

				void SetDefaultValue(const V& defaultValue)
				{
					this->defaultValue = defaultValue;
				}

				const D GetDataType() const
				{
					return this->dataType;
				}

				void SetDataType(const D& dataType)
				{

					this->dataType = dataType;
				}

			private:
				T id;
				V actualValue;
				V defaultValue;
				D dataType;
				std::string name;


		};

	}

}
#endif
