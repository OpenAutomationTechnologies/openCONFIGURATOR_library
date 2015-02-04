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
		\brief
		\author rueckerc
		*/
		template<typename T, typename V, typename D>
		class IBaseObject
		{

			public:
				IBaseObject(T id, V actualValue, V defaultValue, D dataType, std::string name = "") :
					objectIdentifier(id),
					objectActualValue(actualValue),
					objectDefaultValue(defaultValue),
					objectDataType(dataType),
					objectName(name)
				{}

				IBaseObject(T id) :
					objectIdentifier(id),
					objectActualValue(),
					objectDefaultValue(),
					objectDataType(),
					objectName("")
				{}

				IBaseObject(T id, D dataType) :
					objectIdentifier(id),
					objectActualValue(),
					objectDefaultValue(),
					objectDataType(dataType),
					objectName("")
				{}


				virtual ~IBaseObject()
				{}

				const T GetObjectIdentifier() const
				{

					return objectIdentifier;
				}

				void SetObjectIdentifier(T& id)
				{

					objectIdentifier = id;
				}

				const std::string& GetObjectName() const
				{

					return objectName;
				}

				void SetObjectName(std::string& name)
				{

					objectName = name;
				}

				const V GetObjectActualValue() const
				{

					return objectActualValue;
				}
				void SetObjectActualValue(V& actualVal)
				{

					objectActualValue = actualVal;
				}

				const V GetObjectDefaultValue() const
				{

					return objectDefaultValue;
				}

				void SetObjectDefaultValue(V& defaultVal)
				{

					objectDefaultValue = defaultVal;
				}

				const D GetObjectDataType() const
				{

					return objectDataType;
				}

				void SetObjectDataType(D& dataType)
				{

					this->objectDataType = dataType;
				}

			private:
				T objectIdentifier;
				V objectActualValue;
				V objectDefaultValue;
				D objectDataType;
				std::string objectName;


		};

	}

}
#endif
