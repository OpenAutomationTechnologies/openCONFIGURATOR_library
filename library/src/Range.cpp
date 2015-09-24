/************************************************************************
\file Range.cpp
\brief Implementation of the Class Range
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
#include "Range.h"

using namespace IndustrialNetwork::POWERLINK::Core::ModularNode;
using namespace IndustrialNetwork::POWERLINK::Core::ObjectDictionary;
using namespace IndustrialNetwork::POWERLINK::Core::ErrorHandling;
using namespace IndustrialNetwork::POWERLINK::Core::CoreConfiguration;

Range::Range(const std::string& name, std::uint32_t baseIndex, std::uint32_t maxIndex, std::uint32_t maxSubIndex, std::uint32_t sortStep, SortMode sortMode, SortNumber sortNumber, PDOMapping pdoMapping) :
	name(name),
	baseIndex(baseIndex),
	maxIndex(maxIndex),
	maxSubIndex(maxSubIndex),
	sortStep(sortStep),
	sortMode(sortMode),
	sortNumber(sortNumber),
	pdoMapping(pdoMapping),
	currentIndex(baseIndex),
	currentSubIndex(0),
	takenIndices(std::map<std::uint32_t, std::int64_t>()),
	takenAddresses(std::set<std::uint32_t>())
{}

Range::~Range()
{}

const std::string& Range::GetName() const
{
	return this->name;
}

std::uint32_t Range::GetBaseIndex() const
{
	return this->baseIndex;
}

std::uint32_t Range::GetMaxIndex() const
{
	return this->maxIndex;
}

std::uint32_t Range::GetMaxSubIndex() const
{
	return this->maxSubIndex;
}

const SortMode& Range::GetSortMode() const
{
	return this->sortMode;
}

const SortNumber& Range::GetSortNumber() const
{
	return this->sortNumber;
}

const PDOMapping& Range::GetPdoMpapping() const
{
	return this->pdoMapping;
}

Result Range::GetNextIndex(std::uint32_t& index, std::uint32_t address)
{
	bool newModule = false;
	if (this->takenAddresses.find(address) == this->takenAddresses.end())
		newModule = true;

	if (this->sortMode == SortMode::INDEX && this->sortNumber == SortNumber::CONTINUOUS)
	{
		//first object
		if (this->currentIndex == 0)
		{
			this->currentIndex = this->baseIndex + index;
			this->takenIndices.insert(std::pair<std::uint32_t, std::int64_t>(this->currentIndex, -1));
			this->takenAddresses.insert(address);
		}
		else
		{
			if (newModule)
				this->currentIndex = this->currentIndex + index + this->sortStep;
			else
				this->currentIndex = this->currentIndex + index;

			if (this->currentIndex > this->maxIndex)
			{
				boost::format formatter(kMsgRangeHasNoFreeObjects);
				formatter
				% this->GetName();
				return Result(ErrorCode::RANGE_HAS_NO_FREE_OBJECTS, formatter.str());
			}

			this->takenIndices.insert(std::pair<std::uint32_t, std::int64_t>(this->currentIndex, -1));
			if (newModule)
				this->takenAddresses.insert(address);
		}
	}
	else if (this->sortMode == SortMode::INDEX && this->sortNumber == SortNumber::ADDRESS)
	{
		this->currentIndex = this->baseIndex + index + (address - 1);
		if (this->takenIndices.find(this->currentIndex) != this->takenIndices.end())
		{
			boost::format formatter(kMsgRangeIndexAlreadyTaken);
			formatter
			% this->currentIndex
			% this->GetName();
			return Result(ErrorCode::RANGE_INDEX_ALREADY_TAKEN);
		}

		this->takenIndices.insert(std::pair<std::uint32_t, std::int64_t>(this->currentIndex, -1));
		if (newModule)
			this->takenAddresses.insert(address);
	}

	index = this->currentIndex;
	return Result();
}

Result Range::GetNextSubIndex(std::uint32_t& index, std::uint16_t& subindex, std::uint32_t address)
{
	bool newModule = false;
	if (this->takenAddresses.find(address) == this->takenAddresses.end())
		newModule = true;

	if (this->sortMode == SortMode::INDEX)
		return Result();

	if (this->sortMode == SortMode::SUBINDEX && this->sortNumber == SortNumber::CONTINUOUS)
	{
		if (this->currentSubIndex == 0)
		{
			this->currentSubIndex = subindex + address;
			this->takenIndices.insert(std::pair<std::uint32_t, std::int64_t>(this->currentIndex, this->currentSubIndex));
			this->takenAddresses.insert(address);
		}
		else
		{
			if (newModule)
				this->currentSubIndex = subindex + (address - 1) + (std::uint16_t) this->sortStep;
			else
				this->currentSubIndex = subindex + address;

			if (this->currentSubIndex > this->maxSubIndex)
			{
				this->currentIndex++;
				if (this->currentIndex > this->maxIndex)
				{
					boost::format formatter(kMsgRangeHasNoFreeObjects);
					formatter
					% this->GetName();
					return Result(ErrorCode::RANGE_HAS_NO_FREE_OBJECTS, formatter.str());
				}

				this->currentSubIndex = this->currentSubIndex - (std::uint8_t) this->maxSubIndex;
			}

			this->takenIndices.insert(std::pair<std::uint32_t, std::int64_t>(this->currentIndex, this->currentSubIndex));
			if (newModule)
				this->takenAddresses.insert(address);
		}
		index = this->currentIndex;
		subindex = (std::uint16_t) this->currentSubIndex;
		return Result();
	}
	boost::format formatter(kMsgRangeInvalid);
	formatter
	% name;
	LOG_ERROR() << formatter.str();
	return Result(ErrorCode::RANGE_INVALID);
}

void Range::Reset()
{
	this->takenAddresses.clear();
	this->takenIndices.clear();
	this->currentIndex = baseIndex;
	this->currentSubIndex = 0;
}