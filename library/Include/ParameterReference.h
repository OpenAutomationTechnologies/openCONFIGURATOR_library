/************************************************************************
\file ParameterReference.h
\brief Implementation of the Class Parameter
\author rueckerc, Bernecker+Rainer Industrie Elektronik Ges.m.b.H.
\date 05-January-2016 12:00:00
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
#if !defined PARAMETER_REFERENCE_H
#define PARAMETER_REFERENCE_H

#include <memory>
#include <boost/dynamic_bitset.hpp>

#include "ComplexDataType.h"
#include "PlkDataType.h"
#include "ParameterAccess.h"
#include "Parameter.h"
#include "IEC_Datatype.h"
#include "Utilities.h"
#include "IParameterGroupEntry.h"

namespace IndustrialNetwork
{
	namespace POWERLINK
	{
		namespace Core
		{
			namespace ObjectDictionary
			{
				/**
				\brief Represents a parameter reference.
				\author rueckerc, Bernecker+Rainer Industrie Elektronik Ges.m.b.H.
				*/
				class ParameterReference : public IParameterGroupEntry
				{

					public:
						ParameterReference(const std::string& uniqueId, const std::shared_ptr<Parameter>& param, const std::string& actualValue = "", std::uint16_t bitOffset = 0);
						virtual ~ParameterReference();
						const std::string& GetActualValue();
						const boost::dynamic_bitset<>& GetActualValueBitSet();
						const std::shared_ptr<Parameter>& GetReferencedParameter();
						std::uint32_t GetBitSize();

					private:
						std::string actualValue;
						std::shared_ptr<Parameter> referencedParameter;
				};
			}
		}
	}
}
#endif
