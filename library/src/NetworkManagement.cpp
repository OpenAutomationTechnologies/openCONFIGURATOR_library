/************************************************************************
\file NodeNetworkManagement.cpp
\brief Implementation of the Class NodeNetworkManagement
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
#include "NetworkManagement.h"

using namespace IndustrialNetwork::POWERLINK::Core::ErrorHandling;

namespace IndustrialNetwork
{
	namespace POWERLINK
	{
		namespace Core
		{
			namespace Node
			{
				NetworkManagement::NetworkManagement() :
					mnFeatureList(std::vector<std::shared_ptr<MnFeature>>()),
					cnFeatureList(std::vector<std::shared_ptr<CnFeature>>()),
					generalFeatureList(std::vector<std::shared_ptr<GeneralFeature>>())

				{}

				NetworkManagement::~NetworkManagement()
				{
					this->mnFeatureList.clear();
					this->cnFeatureList.clear();
					this->generalFeatureList.clear();
				}

				template<typename I>
				Result NetworkManagement::GetFeatureDefaultValue(CNFeatureEnum feature, I& defaultValue)
				{
					CnFeature test(feature);
					return test.GetDefaultValue<I>(defaultValue);
				}
				template Result NetworkManagement::GetFeatureDefaultValue<bool>(CNFeatureEnum feature, bool& defaultValue);
				template Result NetworkManagement::GetFeatureDefaultValue<uint32_t>(CNFeatureEnum feature, uint32_t& defaultValue);

				template<typename I >
				Result NetworkManagement::GetFeatureActualValue(CNFeatureEnum featureid, I& actualValue)
				{
					for (auto& feature : this->cnFeatureList)
					{
						if (feature->GetFeatureId() == featureid)
						{
							return feature->GetActualValue<I>(actualValue);
						}
					}
					return Result(ErrorCode::FEATURE_VALUE_NOT_FOUND);
				}
				template Result NetworkManagement::GetFeatureActualValue<bool>(CNFeatureEnum feature, bool& actualValue);
				template Result NetworkManagement::GetFeatureActualValue<uint32_t>(CNFeatureEnum feature, uint32_t& actualValue);

				template<typename I >
				Result NetworkManagement::SetFeatureActualValue(CNFeatureEnum featureId, const I actualValue)
				{
					//alter existing feature
					for (auto& feature : this->cnFeatureList)
					{
						if (feature->GetFeatureId() == featureId)
						{
							return feature->SetActualValue<I>(actualValue);
						}
					}

					//create new and alter actual value
					this->cnFeatureList.push_back(std::make_shared<CnFeature>(featureId));
					for (auto& feature : this->cnFeatureList)
					{
						if (feature->GetFeatureId() == featureId)
						{
							return feature->SetActualValue<I>(actualValue);
						}
					}
					return Result();

				}
				template Result NetworkManagement::SetFeatureActualValue<bool>(CNFeatureEnum feature, const bool actualValue);
				template Result NetworkManagement::SetFeatureActualValue<uint32_t>(CNFeatureEnum feature, const uint32_t actualValue);

				template<typename I>
				Result NetworkManagement::GetFeatureDefaultValue(MNFeatureEnum feature, I& defaultValue)
				{
					MnFeature test(feature);
					return test.GetDefaultValue<I>(defaultValue);
				}
				template Result NetworkManagement::GetFeatureDefaultValue<bool>(MNFeatureEnum feature, bool& defaultValue);
				template Result NetworkManagement::GetFeatureDefaultValue<uint8_t>(MNFeatureEnum feature, uint8_t& defaultValue);
				template Result NetworkManagement::GetFeatureDefaultValue<uint16_t>(MNFeatureEnum feature, uint16_t& defaultValue);
				template Result NetworkManagement::GetFeatureDefaultValue<uint32_t>(MNFeatureEnum feature, uint32_t& defaultValue);

				template<typename I >
				Result NetworkManagement::GetFeatureActualValue(MNFeatureEnum featureid, I& actualValue)
				{
					for (auto& feature : this->mnFeatureList)
					{
						if (feature->GetFeatureId() == featureid)
						{
							return feature->GetActualValue<I>(actualValue);
						}
					}
					return Result(ErrorCode::FEATURE_VALUE_NOT_FOUND);
				}
				template Result NetworkManagement::GetFeatureActualValue<bool>(MNFeatureEnum feature, bool& actualValue);
				template Result NetworkManagement::GetFeatureActualValue<uint8_t>(MNFeatureEnum feature, uint8_t& actualValue);
				template Result NetworkManagement::GetFeatureActualValue<uint16_t>(MNFeatureEnum feature, uint16_t& actualValue);
				template Result NetworkManagement::GetFeatureActualValue<uint32_t>(MNFeatureEnum feature, uint32_t& actualValue);

				template<typename I >
				Result NetworkManagement::SetFeatureActualValue(MNFeatureEnum featureId, const I actualValue)
				{
					//alter existing feature
					for (auto& feature : this->mnFeatureList)
					{
						if (feature->GetFeatureId() == featureId)
						{
							return feature->SetActualValue<I>(actualValue);
						}
					}

					//create new and alter actual value
					this->mnFeatureList.push_back(std::make_shared<MnFeature>(featureId));
					for (auto& feature : this->mnFeatureList)
					{
						if (feature->GetFeatureId() == featureId)
						{
							return feature->SetActualValue<I>(actualValue);
						}
					}
					return Result();
				}
				template Result NetworkManagement::SetFeatureActualValue<bool>(MNFeatureEnum feature, const bool actualValue);
				template Result NetworkManagement::SetFeatureActualValue<uint8_t>(MNFeatureEnum feature, const uint8_t actualValue);
				template Result NetworkManagement::SetFeatureActualValue<uint16_t>(MNFeatureEnum feature, const uint16_t actualValue);
				template Result NetworkManagement::SetFeatureActualValue<uint32_t>(MNFeatureEnum feature, const uint32_t actualValue);

				template<typename I>
				Result NetworkManagement::GetFeatureDefaultValue(GeneralFeatureEnum feature, I& defaultValue)
				{
					GeneralFeature test(feature);
					return test.GetDefaultValue<I>(defaultValue);
				}
				template Result NetworkManagement::GetFeatureDefaultValue<bool>(GeneralFeatureEnum feature, bool& defaultValue);
				template Result NetworkManagement::GetFeatureDefaultValue<uint8_t>(GeneralFeatureEnum feature, uint8_t& defaultValue);
				template Result NetworkManagement::GetFeatureDefaultValue<uint16_t>(GeneralFeatureEnum feature, uint16_t& defaultValue);
				template Result NetworkManagement::GetFeatureDefaultValue<uint32_t>(GeneralFeatureEnum feature, uint32_t& defaultValue);

				template<typename I >
				Result NetworkManagement::GetFeatureActualValue(GeneralFeatureEnum featureid, I& actualValue)
				{
					for (auto& feature : this->generalFeatureList)
					{
						if (feature->GetFeatureId() == featureid)
						{
							return feature->GetActualValue<I>(actualValue);
						}
					}
					return Result(ErrorCode::FEATURE_VALUE_NOT_FOUND);
				}
				template Result NetworkManagement::GetFeatureActualValue<bool>(GeneralFeatureEnum feature, bool& actualValue);
				template Result NetworkManagement::GetFeatureActualValue<uint8_t>(GeneralFeatureEnum feature, uint8_t& actualValue);
				template Result NetworkManagement::GetFeatureActualValue<uint16_t>(GeneralFeatureEnum feature, uint16_t& actualValue);
				template Result NetworkManagement::GetFeatureActualValue<uint32_t>(GeneralFeatureEnum feature, uint32_t& actualValue);

				template<typename I >
				Result NetworkManagement::SetFeatureActualValue(GeneralFeatureEnum featureId, const I actualValue)
				{
					//alter existing feature
					for (auto& feature : this->generalFeatureList)
					{
						if (feature->GetFeatureId() == featureId)
						{
							return feature->SetActualValue<I>(actualValue);
						}
					}

					//create new and alter actual value
					this->generalFeatureList.push_back(std::make_shared<GeneralFeature>(featureId));
					for (auto& feature : this->generalFeatureList)
					{
						if (feature->GetFeatureId() == featureId)
						{
							return feature->SetActualValue<I>(actualValue);
						}
					}
					return Result();
				}
				template Result NetworkManagement::SetFeatureActualValue<bool>(GeneralFeatureEnum feature, const bool actualValue);
				template Result NetworkManagement::SetFeatureActualValue<uint8_t>(GeneralFeatureEnum feature, const uint8_t actualValue);
				template Result NetworkManagement::SetFeatureActualValue<uint16_t>(GeneralFeatureEnum feature, const uint16_t actualValue);
				template Result NetworkManagement::SetFeatureActualValue<uint32_t>(GeneralFeatureEnum feature, const uint32_t actualValue);

				Result NetworkManagement::SetFeatureUntypedActualValue(CNFeatureEnum featureId, const std::string& actualValue)
				{
					//alter existing feature
					for (auto& feature : this->cnFeatureList)
					{
						if (feature->GetFeatureId() == featureId)
						{
							return feature->SetTypedValues("", actualValue);
						}
					}

					//create new and alter actual value
					this->cnFeatureList.push_back(std::make_shared<CnFeature>(featureId));
					for (auto& feature : this->cnFeatureList)
					{
						if (feature->GetFeatureId() == featureId)
						{
							return feature->SetTypedValues("", actualValue);
						}
					}
					return Result();
				}
				Result NetworkManagement::SetFeatureUntypedActualValue(MNFeatureEnum featureId, const std::string& actualValue)
				{
					//alter existing feature
					for (auto& feature : this->mnFeatureList)
					{
						if (feature->GetFeatureId() == featureId)
						{
							return feature->SetTypedValues("", actualValue);
						}
					}

					//create new and alter actual value
					this->mnFeatureList.push_back(std::make_shared<MnFeature>(featureId));
					for (auto& feature : this->mnFeatureList)
					{
						if (feature->GetFeatureId() == featureId)
						{
							return feature->SetTypedValues("", actualValue);
						}
					}
					return Result();
				}
				Result NetworkManagement::SetFeatureUntypedActualValue(GeneralFeatureEnum featureId, const std::string& actualValue)
				{
					//alter existing feature
					for (auto& feature : this->generalFeatureList)
					{
						if (feature->GetFeatureId() == featureId)
						{
							return feature->SetTypedValues("", actualValue);
						}
					}

					//create new and alter actual value
					this->generalFeatureList.push_back(std::make_shared<GeneralFeature>(featureId));
					for (auto& feature : this->generalFeatureList)
					{
						if (feature->GetFeatureId() == featureId)
						{
							return feature->SetTypedValues("", actualValue);
						}
					}
					return Result();
				}
			}
		}
	}
}
