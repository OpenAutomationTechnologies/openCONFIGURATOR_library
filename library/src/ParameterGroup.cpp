/************************************************************************
\file ParameterGroup.cpp
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
#include "ParameterGroup.h"

using namespace IndustrialNetwork::POWERLINK::Core::ObjectDictionary;
using namespace IndustrialNetwork::POWERLINK::Core::Utilities;
using namespace IndustrialNetwork::POWERLINK::Core::ErrorHandling;
using namespace IndustrialNetwork::POWERLINK::Core::CoreConfiguration;

ParameterGroup::ParameterGroup(const std::string& uniqueId, std::uint16_t bitOffset) : IParameterGroupEntry(uniqueId, bitOffset),
	conditionalUniqueIdRef(""),
	conditionalValue(""),
	parameterGroupEntries(std::vector<std::shared_ptr<IParameterGroupEntry>>()),
	bitSet(boost::dynamic_bitset<>()),
	conditionalParameter(std::shared_ptr<Parameter>())
{}

ParameterGroup::ParameterGroup(const std::string& uniqueId, const std::string& conditionalUniqueIdRef, const std::string& conditionalValue, const std::shared_ptr<Parameter>& param, std::uint16_t bitOffset) : IParameterGroupEntry(uniqueId, bitOffset),
	conditionalUniqueIdRef(conditionalUniqueIdRef),
	conditionalValue(conditionalValue),
	parameterGroupEntries(std::vector<std::shared_ptr<IParameterGroupEntry>>()),
	bitSet(boost::dynamic_bitset<>()),
	conditionalParameter(param)
{}

ParameterGroup::~ParameterGroup()
{}

const std::vector<std::shared_ptr<IParameterGroupEntry>>& ParameterGroup::GetParameterGroupEntries()
{
	std::sort(parameterGroupEntries.begin(), parameterGroupEntries.end(), [](const std::shared_ptr<IParameterGroupEntry>& l, const std::shared_ptr<IParameterGroupEntry>& r)
	{
		return (l->GetBitOffset() < r->GetBitOffset());
	});
	return this->parameterGroupEntries;
}

std::uint32_t ParameterGroup::GetBitSize()
{
	std::uint32_t size = 0;
	std::sort(parameterGroupEntries.begin(), parameterGroupEntries.end(), [](const std::shared_ptr<IParameterGroupEntry>& l, const std::shared_ptr<IParameterGroupEntry>& r)
	{
		return (l->GetBitOffset() < r->GetBitOffset());
	});
	for (auto& paramGrpEntry : this->parameterGroupEntries)
	{
		auto paramGrp = std::dynamic_pointer_cast<ParameterGroup>(paramGrpEntry);
		auto paramRef = std::dynamic_pointer_cast<ParameterReference>(paramGrpEntry);
		if (paramGrp)
		{
			if (paramGrp->CheckParameterGroupCondition())
			{
				if (size < paramGrp->GetBitOffset())
					size += (paramGrp->GetBitOffset() - size);
				size += paramGrp->GetBitSize();
			}
		}
		else if (paramRef)
		{
			if (size < paramRef->GetBitOffset())
				size += (paramRef->GetBitOffset() - size);
			size += paramRef->GetBitSize();
		}
	}
	//Byte Align
	if (size % 8 != 0)
		size += (size % 8);
	return size;
}

Result ParameterGroup::AddParameterGroupEntry(const std::shared_ptr<IParameterGroupEntry>& paramEntry)
{
	this->parameterGroupEntries.push_back(paramEntry);
	std::sort(parameterGroupEntries.begin(), parameterGroupEntries.end(), [](const std::shared_ptr<IParameterGroupEntry>& l, const std::shared_ptr<IParameterGroupEntry>& r)
	{
		return (l->GetBitOffset() < r->GetBitOffset());
	});
	return Result();
}

const boost::dynamic_bitset<>& ParameterGroup::GetActualValueBitSet(std::uint32_t bitSetSize)
{
	this->bitSet.clear();
	this->bitSet.resize(bitSetSize);
	std::sort(parameterGroupEntries.begin(), parameterGroupEntries.end(), [](const std::shared_ptr<IParameterGroupEntry>& l, const std::shared_ptr<IParameterGroupEntry>& r)
	{
		return (l->GetBitOffset() < r->GetBitOffset());
	});
	for (auto& paramGrpEntry : this->parameterGroupEntries)
	{
		auto paramGrp = std::dynamic_pointer_cast<ParameterGroup>(paramGrpEntry);
		auto paramRef = std::dynamic_pointer_cast<ParameterReference>(paramGrpEntry);
		if (paramGrp)
		{
			if (paramGrp->CheckParameterGroupCondition())
			{
				//if (paramGrp->GetBitOffset() > bitSet.size())
				//	bitSet.resize(paramGrp->GetBitOffset());

				auto paramGrpBitSet = paramGrp->GetActualValueBitSet(paramGrp->GetBitSize());
				//std::uint32_t start = paramGrp->GetBitOffset() + bitSet.size();
				//size_t totalSize = paramGrpBitSet.size() + bitSet.size();
				//bitSet.resize(totalSize);
				for (boost::dynamic_bitset<>::size_type i = 0; i < paramGrpBitSet.size(); i++)
				{
					bitSet[paramGrp->GetBitOffset() + i] = paramGrpBitSet[i];
				}
				//LOG_FATAL() << paramGrp->GetUniqueId() << " : " << paramGrp->GetBitOffset() << " : " << bitSet;
			}
		}
		else if (paramRef)
		{
			//if (paramRef->GetBitOffset() > bitSet.size())
			//	bitSet.resize(paramRef->GetBitOffset());

			auto paramRefBitSet = paramRef->GetActualValueBitSet();
			//std::uint32_t start = paramRef->GetBitOffset() + bitSet.size();
			//size_t totalSize = paramRefBitSet.size() + bitSet.size();
			//bitSet.resize(totalSize);
			for (size_t i = 0; i < paramRefBitSet.size(); i++)
			{
				bitSet[paramRef->GetBitOffset() + i] = paramRefBitSet[i];
			}
			//LOG_FATAL() << paramRef->GetUniqueId() << " : " << paramRef->GetBitOffset() << " : " << bitSet;
		}
	}
	return this->bitSet;
}

Result ParameterGroup::GetParameterGroupAccess(ParameterAccess& access)
{
	std::vector<ParameterAccess> accessVector;
	for (auto& paramGrpEntry : this->parameterGroupEntries)
	{
		auto paramGrp = std::dynamic_pointer_cast<ParameterGroup>(paramGrpEntry);
		auto paramRef = std::dynamic_pointer_cast<ParameterReference>(paramGrpEntry);
		if (paramGrp)
		{
			if (paramGrp->CheckParameterGroupCondition())
			{
				ParameterAccess tempAccess;
				Result res = paramGrp->GetParameterGroupAccess(tempAccess);
				if (!res.IsSuccessful())
					continue;
				accessVector.push_back(tempAccess);
			}
		}
		else if (paramRef)
			accessVector.push_back(paramRef->GetReferencedParameter()->GetParameterAccess());
	}
	if (accessVector.size() == 0)
		return Result(ErrorCode::ARGUMENT_INVALID_EMPTY);

	if (std::adjacent_find(accessVector.begin(), accessVector.end(), std::not_equal_to<ParameterAccess>()) == accessVector.end())
	{
		access = accessVector.at(0);
		return Result();
	}
	else
	{
		//Parameter access is not equal throughout the group
		boost::format formatter(kMsgAccessTypeForParameterGroupInvalid);
		formatter
		% this->GetUniqueId();
		LOG_ERROR() << formatter.str();
		return Result(ErrorCode::PARAMETER_ACCESS_INVALID, formatter.str());
	}
}

bool ParameterGroup::CheckParameterGroupCondition()
{
	//if conditon is met return true
	if (this->conditionalParameter->HasActualValue())
	{
		if (this->conditionalParameter->GetTypedParameterActualValue<std::string>() == this->conditionalValue)
			return true;
	}
	else if (this->conditionalParameter->HasDefaultValue())
	{
		if (this->conditionalParameter->GetTypedParameterDefaultValue<std::string>() == this->conditionalValue)
			return true;
	}
	return false;
}

