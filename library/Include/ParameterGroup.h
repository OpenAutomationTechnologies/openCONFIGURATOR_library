/************************************************************************
\file ParameterGroup.h
\brief Implementation of the Class ParameterGroup
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
#if !defined PARAMETER_GROUP_H
#define PARAMETER_GROUP_H

#include <memory>
#include <sstream>
#include <algorithm>
#include <boost/dynamic_bitset.hpp>

#include "ParameterReference.h"
#include "Parameter.h"
#include "Utilities.h"
#include "Result.h"
#include "LoggingConfiguration.h"
#include "PlkDataType.h"
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
				\brief Represents parameter group.
				\author rueckerc, Bernecker+Rainer Industrie Elektronik Ges.m.b.H.
				*/
				class ParameterGroup : public IParameterGroupEntry
				{

					public:
						ParameterGroup(const std::string& uniqueId, std::uint16_t bitOffset = 0);
						ParameterGroup(const std::string& uniqueId, const std::string& conditionalUniqueIdRef, const std::string& conditionalValue, const std::shared_ptr<Parameter>& param, std::uint16_t bitOffset = 0);
						virtual ~ParameterGroup();

						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result AddParameterGroupEntry(const std::shared_ptr<IParameterGroupEntry>& paramGrp);
						const boost::dynamic_bitset<>& GetActualValueBitSet(std::uint32_t bitSetSize);

						std::uint32_t GetBitSize();

						bool CheckParameterGroupCondition();
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result GetParameterGroupAccess(ParameterAccess& access);
						const std::vector<std::shared_ptr<IParameterGroupEntry>>& GetParameterGroupEntries();

					private:
						std::string conditionalUniqueIdRef;
						std::string conditionalValue;

						std::vector<std::shared_ptr<IParameterGroupEntry>> parameterGroupEntries;
						boost::dynamic_bitset<> bitSet;
						std::shared_ptr<Parameter> conditionalParameter;
				};
			}
		}
	}
}
#endif
