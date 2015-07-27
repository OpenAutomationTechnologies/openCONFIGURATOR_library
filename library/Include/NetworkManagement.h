/************************************************************************
\file NetworkManagement.h
\brief Implementation of the Class NetworkManagement
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
#if !defined NETWORK_MANAGEMENT_H
#define NETWORK_MANAGEMENT_H

#include <vector>
#include <memory>

#include "PlkFeatureEnum.h"
#include "PlkFeature.h"
#include "GeneralFeature.h"
#include "CnFeature.h"
#include "MnFeature.h"
#include "Result.h"
#include "Constants.h"

namespace IndustrialNetwork
{
	namespace POWERLINK
	{
		namespace Core
		{
			namespace Node
			{
				/**
				\brief
				\author rueckerc
				*/
				class DLLEXPORT NetworkManagement
				{

					public:
						NetworkManagement();
						virtual ~NetworkManagement();

						template<typename I >
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result GetFeatureDefaultValue(CNFeatureEnum feature, I& defaultValue);

						template<typename I >
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result GetFeatureDefaultValue(MNFeatureEnum feature, I& defaultValue);

						template<typename I >
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result GetFeatureDefaultValue(GeneralFeatureEnum feature, I& defaultValue);

						template<typename I >
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result GetFeatureActualValue(CNFeatureEnum feature, I& actualValue);

						template<typename I >
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result GetFeatureActualValue(MNFeatureEnum feature, I& actualValue);

						template<typename I >
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result GetFeatureActualValue(GeneralFeatureEnum feature, I& actualValue);

						template<typename I >
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result SetFeatureActualValue(CNFeatureEnum feature, const I actualValue);

						template<typename I >
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result SetFeatureActualValue(MNFeatureEnum feature, const I actualValue);

						template<typename I >
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result SetFeatureActualValue(GeneralFeatureEnum feature, const I actualValue);

						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result SetFeatureUntypedActualValue(CNFeatureEnum feature, const std::string& actualValue);
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result SetFeatureUntypedActualValue(MNFeatureEnum feature, const std::string& actualValue);
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result SetFeatureUntypedActualValue(GeneralFeatureEnum feature, const std::string& actualValue);

					private:
						std::vector<std::shared_ptr<MnFeature>> mnFeatureList;
						std::vector<std::shared_ptr<CnFeature>> cnFeatureList;
						std::vector<std::shared_ptr<GeneralFeature>> generalFeatureList;
				};
			}
		}
	}
}
#endif
