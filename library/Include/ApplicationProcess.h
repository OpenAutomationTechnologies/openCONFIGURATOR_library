/************************************************************************
\file ApplicationProcess.h
\brief Implementation of the Class ApplicationProcess
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
#if !defined APPLICATION_PROCESS_H
#define APPLICATION_PROCESS_H

#include <vector>
#include <memory>

#include <boost/format.hpp>

#include "Parameter.h"
#include "ParameterGroup.h"
#include "ParameterTemplate.h"
#include "Constants.h"
#include "Result.h"
#include "LoggingConfiguration.h"
#include "ComplexDataType.h"
#include "StructDataType.h"

namespace IndustrialNetwork
{
	namespace POWERLINK
	{
		namespace Core
		{
			namespace ObjectDictionary
			{
				/**
				\brief Represents application process of a node in the POWERLINK network
				\author rueckerc, Bernecker+Rainer Industrie Elektronik Ges.m.b.H.
				*/
				class DLLEXPORT ApplicationProcess
				{

					public:
						ApplicationProcess();
						virtual ~ApplicationProcess();
						const std::vector<std::shared_ptr<Parameter>>& GetParameterList();
						const std::vector<std::shared_ptr<ComplexDataType>>& GetDataTypeList();
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result AddComplexDataType(std::shared_ptr<ComplexDataType>& dt);
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result GetComplexDataType(const std::string& uniqueId, std::shared_ptr<ComplexDataType>& dt);

						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result AddParameter(std::shared_ptr<Parameter>& param);
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result AddParameterGroup(std::shared_ptr<ParameterGroup>& paramGrp);
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result AddParameterTemplate(std::shared_ptr<ParameterTemplate>& paramTempl);

						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result GetParameter(const std::string& uniqueId, std::shared_ptr<Parameter>& returnParam);
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result GetParameterGroup(const std::string& uniqueId, std::shared_ptr<ParameterGroup>& returnParam);
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result GetParameterTemplate(const std::string& uniqueId, std::shared_ptr<ParameterTemplate>& returnParam);

						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result SetParameterActualValue(const std::string& uniqueId, const std::string& actualValue);
						IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result GetParameterActualValue(const std::string& uniqueId, std::string& actualValue);
						std::uint32_t GetBitSize(const std::string& uniqueIdRef);

					private:
						std::vector<std::shared_ptr<ComplexDataType>> datatypeList;
						std::vector<std::shared_ptr<Parameter>> parameterList;
						std::vector<std::shared_ptr<ParameterGroup>> parameterGroupList;
						std::vector<std::shared_ptr<ParameterTemplate>> parameterTemplateList;

				};
			}
		}
	}
}
#endif
