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
using namespace IndustrialNetwork::POWERLINK::Core::CoreConfiguration;

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
				Result NetworkManagement::GetFeatureDefaultValue(CNFeatureEnum featureID, I& defaultValue)
				{
					CnFeature cnfeature(featureID);
					return cnfeature.GetDefaultValue<I>(defaultValue);
				}
				template Result NetworkManagement::GetFeatureDefaultValue<bool>(CNFeatureEnum featureID, bool& defaultValue);
				template Result NetworkManagement::GetFeatureDefaultValue<std::uint32_t>(CNFeatureEnum featureID, std::uint32_t& defaultValue);

				template<typename I >
				Result NetworkManagement::GetFeatureActualValue(CNFeatureEnum featureID, I& actualValue)
				{
					for (auto& feature : this->cnFeatureList)
					{
						if (feature->GetFeatureId() == featureID)
						{
							return feature->GetActualValue<I>(actualValue);
						}
					}
					boost::format formatter(kMsgFeatureActualValue[static_cast<std::underlying_type<Language>::type>(LoggingConfiguration::GetInstance().GetCurrentLanguage())]);
					formatter
					% PlkFeatureStrings[featureID];
					LOG_ERROR() << formatter.str();
					return Result(ErrorCode::FEATURE_VALUE_NOT_FOUND, formatter.str());
				}
				template Result NetworkManagement::GetFeatureActualValue<bool>(CNFeatureEnum featureID, bool& actualValue);
				template Result NetworkManagement::GetFeatureActualValue<std::uint32_t>(CNFeatureEnum featureID, std::uint32_t& actualValue);

				template<typename I >
				Result NetworkManagement::SetFeatureActualValue(CNFeatureEnum featureID, const I actualValue)
				{
					//alter existing feature
					for (auto& feature : this->cnFeatureList)
					{
						if (feature->GetFeatureId() == featureID)
						{
							return feature->SetActualValue<I>(actualValue);
						}
					}

					//create new and alter actual value
					this->cnFeatureList.push_back(std::make_shared<CnFeature>(featureID));
					for (auto& feature : this->cnFeatureList)
					{
						if (feature->GetFeatureId() == featureID)
						{
							return feature->SetActualValue<I>(actualValue);
						}
					}
					return Result();

				}
				template Result NetworkManagement::SetFeatureActualValue<bool>(CNFeatureEnum featureID, const bool actualValue);
				template Result NetworkManagement::SetFeatureActualValue<std::uint32_t>(CNFeatureEnum featureID, const std::uint32_t actualValue);

				template<typename I>
				Result NetworkManagement::GetFeatureDefaultValue(MNFeatureEnum featureID, I& defaultValue)
				{
					MnFeature mnfeature(featureID);
					return mnfeature.GetDefaultValue<I>(defaultValue);
				}
				template Result NetworkManagement::GetFeatureDefaultValue<bool>(MNFeatureEnum featureID, bool& defaultValue);
				template Result NetworkManagement::GetFeatureDefaultValue<std::uint16_t>(MNFeatureEnum featureID, std::uint16_t& defaultValue);
				template Result NetworkManagement::GetFeatureDefaultValue<std::uint32_t>(MNFeatureEnum featureID, std::uint32_t& defaultValue);

				template<typename I >
				Result NetworkManagement::GetFeatureActualValue(MNFeatureEnum featureID, I& actualValue)
				{
					for (auto& feature : this->mnFeatureList)
					{
						if (feature->GetFeatureId() == featureID)
						{
							return feature->GetActualValue<I>(actualValue);
						}
					}
					boost::format formatter(kMsgFeatureActualValue[static_cast<std::underlying_type<Language>::type>(LoggingConfiguration::GetInstance().GetCurrentLanguage())]);
					formatter
					% PlkFeatureStrings[featureID];
					LOG_ERROR() << formatter.str();
					return Result(ErrorCode::FEATURE_VALUE_NOT_FOUND, formatter.str());
				}
				template Result NetworkManagement::GetFeatureActualValue<bool>(MNFeatureEnum featureID, bool& actualValue);
				template Result NetworkManagement::GetFeatureActualValue<std::uint16_t>(MNFeatureEnum featureID, std::uint16_t& actualValue);
				template Result NetworkManagement::GetFeatureActualValue<std::uint32_t>(MNFeatureEnum featureID, std::uint32_t& actualValue);

				template<typename I >
				Result NetworkManagement::SetFeatureActualValue(MNFeatureEnum featureID, const I actualValue)
				{
					//alter existing feature
					for (auto& feature : this->mnFeatureList)
					{
						if (feature->GetFeatureId() == featureID)
						{
							return feature->SetActualValue<I>(actualValue);
						}
					}

					//create new and alter actual value
					this->mnFeatureList.push_back(std::make_shared<MnFeature>(featureID));
					for (auto& feature : this->mnFeatureList)
					{
						if (feature->GetFeatureId() == featureID)
						{
							return feature->SetActualValue<I>(actualValue);
						}
					}
					return Result();
				}
				template Result NetworkManagement::SetFeatureActualValue<bool>(MNFeatureEnum featureID, const bool actualValue);
				template Result NetworkManagement::SetFeatureActualValue<std::uint16_t>(MNFeatureEnum featureID, const std::uint16_t actualValue);
				template Result NetworkManagement::SetFeatureActualValue<std::uint32_t>(MNFeatureEnum featureID, const std::uint32_t actualValue);

				template<typename I>
				Result NetworkManagement::GetFeatureDefaultValue(GeneralFeatureEnum featureID, I& defaultValue)
				{
					GeneralFeature generalFeature(featureID);
					return generalFeature.GetDefaultValue<I>(defaultValue);
				}
				template Result NetworkManagement::GetFeatureDefaultValue<bool>(GeneralFeatureEnum featureID, bool& defaultValue);
				template Result NetworkManagement::GetFeatureDefaultValue<std::uint16_t>(GeneralFeatureEnum featureID, std::uint16_t& defaultValue);
				template Result NetworkManagement::GetFeatureDefaultValue<std::uint32_t>(GeneralFeatureEnum featureID, std::uint32_t& defaultValue);

				template<typename I >
				Result NetworkManagement::GetFeatureActualValue(GeneralFeatureEnum featureID, I& actualValue)
				{
					for (auto& feature : this->generalFeatureList)
					{
						if (feature->GetFeatureId() == featureID)
						{
							if (!feature->GetUntypedActualValue().empty())
								return feature->GetActualValue<I>(actualValue);
							else if (!feature->GetUntypedDefaultValue().empty())
								return feature->GetDefaultValue<I>(actualValue);
						}
					}
					boost::format formatter(kMsgFeatureActualValue[static_cast<std::underlying_type<Language>::type>(LoggingConfiguration::GetInstance().GetCurrentLanguage())]);
					formatter
					% PlkFeatureStrings[featureID];
					LOG_ERROR() << formatter.str();
					return Result(ErrorCode::FEATURE_VALUE_NOT_FOUND, formatter.str());
				}
				template Result NetworkManagement::GetFeatureActualValue<bool>(GeneralFeatureEnum featureID, bool& actualValue);
				template Result NetworkManagement::GetFeatureActualValue<std::uint16_t>(GeneralFeatureEnum featureID, std::uint16_t& actualValue);
				template Result NetworkManagement::GetFeatureActualValue<std::uint32_t>(GeneralFeatureEnum featureID, std::uint32_t& actualValue);

				template<typename I >
				Result NetworkManagement::SetFeatureActualValue(GeneralFeatureEnum featureID, const I actualValue)
				{
					//alter existing feature
					for (auto& feature : this->generalFeatureList)
					{
						if (feature->GetFeatureId() == featureID)
						{
							return feature->SetActualValue<I>(actualValue);
						}
					}

					//create new and alter actual value
					this->generalFeatureList.push_back(std::make_shared<GeneralFeature>(featureID));
					for (auto& feature : this->generalFeatureList)
					{
						if (feature->GetFeatureId() == featureID)
						{
							return feature->SetActualValue<I>(actualValue);
						}
					}
					return Result();
				}
				template Result NetworkManagement::SetFeatureActualValue<bool>(GeneralFeatureEnum featureID, const bool actualValue);
				template Result NetworkManagement::SetFeatureActualValue<std::uint16_t>(GeneralFeatureEnum featureID, const std::uint16_t actualValue);
				template Result NetworkManagement::SetFeatureActualValue<std::uint32_t>(GeneralFeatureEnum featureID, const std::uint32_t actualValue);

				Result NetworkManagement::SetFeatureUntypedActualValue(CNFeatureEnum featureID, const std::string& actualValue)
				{
					//alter existing feature
					for (auto& feature : this->cnFeatureList)
					{
						if (feature->GetFeatureId() == featureID)
						{
							return feature->SetTypedValues("", actualValue);
						}
					}

					//create new and alter actual value
					this->cnFeatureList.push_back(std::make_shared<CnFeature>(featureID));
					for (auto& feature : this->cnFeatureList)
					{
						if (feature->GetFeatureId() == featureID)
						{
							return feature->SetTypedValues("", actualValue);
						}
					}
					return Result();
				}
				Result NetworkManagement::SetFeatureUntypedActualValue(MNFeatureEnum featureID, const std::string& actualValue)
				{
					//alter existing feature
					for (auto& feature : this->mnFeatureList)
					{
						if (feature->GetFeatureId() == featureID)
						{
							return feature->SetTypedValues("", actualValue);
						}
					}

					//create new and alter actual value
					this->mnFeatureList.push_back(std::make_shared<MnFeature>(featureID));
					for (auto& feature : this->mnFeatureList)
					{
						if (feature->GetFeatureId() == featureID)
						{
							return feature->SetTypedValues("", actualValue);
						}
					}
					return Result();
				}
				Result NetworkManagement::SetFeatureUntypedActualValue(GeneralFeatureEnum featureID, const std::string& actualValue)
				{
					//alter existing feature
					for (auto& feature : this->generalFeatureList)
					{
						if (feature->GetFeatureId() == featureID)
						{
							return feature->SetTypedValues("", actualValue);
						}
					}

					//create new and alter actual value
					this->generalFeatureList.push_back(std::make_shared<GeneralFeature>(featureID));
					for (auto& feature : this->generalFeatureList)
					{
						if (feature->GetFeatureId() == featureID)
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
