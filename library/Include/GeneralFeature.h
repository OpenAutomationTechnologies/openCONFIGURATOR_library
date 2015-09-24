/************************************************************************
\file GeneralFeature.h
\brief Implementation of the Class GeneralFeature
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
#if !defined GENERAL_FEATURE_H
#define GENERAL_FEATURE_H

#include <string>

#include <boost/any.hpp>
#include <boost/format.hpp>

#include "PlkFeature.h"
#include "PlkFeatureEnum.h"
#include "Result.h"
#include "Utilities.h"
#include "LoggingConfiguration.h"

namespace IndustrialNetwork
{
	namespace POWERLINK
	{
		namespace Core
		{
			namespace Node
			{
				/**
				\brief Represents a general feature of a POWERLINK node.
				\author rueckerc, Bernecker+Rainer Industrie Elektronik Ges.m.b.H.
				*/
				class GeneralFeature : public PlkFeature<GeneralFeatureEnum>
				{
					public:
						explicit GeneralFeature(GeneralFeatureEnum type);
						virtual ~GeneralFeature();

						const std::string& GetName() const;
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result SetTypedValues(const std::string& defaultValue, const std::string& actualValue);

						template<class T>
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result GetDefaultValue(T& value);

						template<class T>
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result GetActualValue(T& value);

						template<class T>
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result SetActualValue(const T actualValue);
				};
			}
		}
	}
}
#endif
