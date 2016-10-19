/************************************************************************
\file ModuleInterface.cpp
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
#include "Interface.h"

using namespace IndustrialNetwork::POWERLINK::Core::ModularNode;
using namespace IndustrialNetwork::POWERLINK::Core::ErrorHandling;
using namespace IndustrialNetwork::POWERLINK::Core::CoreConfiguration;

Interface::Interface(const std::string& uniqueId, const std::string& type, const ModuleAddressing& moduleAddressing, std::uint32_t maxModules, bool unusedSlots, bool multipleModules) :
	IBaseInterface(uniqueId, type, moduleAddressing),
	unusedSlots(unusedSlots),
	multipleModules(multipleModules),
	maxModules(maxModules),
	rangeList(std::vector<std::shared_ptr<Range>>()),
	moduleCollection(std::map<std::uint32_t, std::shared_ptr<Module>>())
{}

Interface::~Interface()
{
	this->rangeList.clear();
	this->moduleCollection.clear();
}

std::uint32_t Interface::GetModuleCount() const
{
	return (std::uint32_t) this->moduleCollection.size();
}

bool Interface::GetUnusedSlots() const
{
	return this->unusedSlots;
}

bool Interface::GetMultipleModules() const
{
	return this->multipleModules;
}

std::uint32_t Interface::GetMaxModules() const
{
	return this->maxModules;
}

Result Interface::GetModule(const std::string& moduleId, std::uint32_t modulePosition, std::shared_ptr<Module>& moduleRet)
{
	if (this->moduleCollection.find(modulePosition) != this->moduleCollection.end())
	{
		auto module = this->moduleCollection.find(modulePosition)->second;
		if (module->GetModuleId() == moduleId)
		{
			moduleRet = module;
			return Result();
		}
		else
		{
			boost::format formatter(kMsgModuleDoesNotMatch[static_cast<std::underlying_type<Language>::type>(LoggingConfiguration::GetInstance().GetCurrentLanguage())]);
			formatter
			% moduleId
			% module->GetModuleId()
			% modulePosition;
			LOG_ERROR() << formatter.str();
			return Result(ErrorCode::MODULE_ID_DOES_NOT_MATCH, formatter.str());
		}
	}
	boost::format formatter(kMsgModuleDoesNotExists[static_cast<std::underlying_type<Language>::type>(LoggingConfiguration::GetInstance().GetCurrentLanguage())]);
	formatter
	% moduleId
	% modulePosition;
	LOG_ERROR() << formatter.str();
	return Result(ErrorCode::MODULE_DOES_NOT_EXIST, formatter.str());
}

Result Interface::AddRange(const std::shared_ptr<Range>& rangeToAdd)
{
	for (auto& range : this->rangeList)
	{
		if (range->GetName() == rangeToAdd->GetName())
		{
			boost::format formatter(kMsgRangeAlreadyExists[static_cast<std::underlying_type<Language>::type>(LoggingConfiguration::GetInstance().GetCurrentLanguage())]);
			formatter
			% rangeToAdd->GetName();
			LOG_ERROR() << formatter.str();
			return Result(ErrorCode::RANGE_ALREADY_EXISTS, formatter.str());
		}
	}
	this->rangeList.push_back(rangeToAdd);
	return Result();
}

Result Interface::GetRange(const std::string& name, std::shared_ptr<Range>& retRange)
{
	for (auto& range : this->rangeList)
	{
		if (range->GetName() == name)
		{
			retRange = range;
			return Result();
		}
	}
	boost::format formatter(kMsgRangeDoesNotExists[static_cast<std::underlying_type<Language>::type>(LoggingConfiguration::GetInstance().GetCurrentLanguage())]);
	formatter
	% name;
	LOG_ERROR() << formatter.str();
	return Result(ErrorCode::RANGE_DOES_NOT_EXIST, formatter.str());
}

Result Interface::AddModule(std::uint32_t position, const std::shared_ptr<Module>& moduleToAdd)
{
	//ModuleId already exist and multiple modules are not allowed
	for (auto& module : this->moduleCollection)
	{
		if (module.second->GetModuleId() == moduleToAdd->GetModuleId() && this->GetMultipleModules() == false)
		{
			boost::format formatter(kMsgModuleAlreadyExists[static_cast<std::underlying_type<Language>::type>(LoggingConfiguration::GetInstance().GetCurrentLanguage())]);
			formatter
			% moduleToAdd->GetModuleId()
			% position;
			LOG_ERROR() << formatter.str();
			return Result(ErrorCode::MODULE_ALREADY_EXISTS, formatter.str());
		}
	}

	//Module position is already taken
	if (this->moduleCollection.find(position) != this->moduleCollection.end())
	{
		boost::format formatter(kMsgModulePositionOccupied[static_cast<std::underlying_type<Language>::type>(LoggingConfiguration::GetInstance().GetCurrentLanguage())]);
		formatter
		% moduleToAdd->GetModuleId()
		% position;
		LOG_ERROR() << formatter.str();
		return Result(ErrorCode::POSITION_OCCUPIED, formatter.str());
	}

	this->moduleCollection.insert(std::pair<std::uint32_t, std::shared_ptr<Module>>(position, moduleToAdd));
	return Result();
}

Result Interface::RemoveModule(const std::string& moduleId,  std::uint32_t modulePosition)
{
	for (auto it = this->moduleCollection.begin() ; it != this->moduleCollection.end(); ++it)
	{
		if (it->second->GetModuleId() == moduleId && it->first == modulePosition)
		{
			this->moduleCollection.erase(it);
			return Result();
		}
	}
	boost::format formatter(kMsgModuleDoesNotExists[static_cast<std::underlying_type<Language>::type>(LoggingConfiguration::GetInstance().GetCurrentLanguage())]);
	formatter
	% moduleId
	% modulePosition;
	LOG_ERROR() << formatter.str();
	return Result(ErrorCode::MODULE_DOES_NOT_EXIST, formatter.str());
}

bool Interface::IsPositionOccupied(std::uint32_t pos) const
{
	if (this->moduleCollection.find(pos) != this->moduleCollection.end())
		return true;
	return false;
}

Result Interface::ChangeModulePosition(const std::string& moduleId, std::uint32_t oldPos, std::uint32_t newPos)
{
	if (this->moduleCollection.find(oldPos) != this->moduleCollection.end() && this->moduleCollection.find(newPos) == this->moduleCollection.end())
	{
		auto it = this->moduleCollection.find(oldPos);
		it->second->SetPosition(newPos);
		this->moduleCollection.insert(std::pair<std::uint32_t, std::shared_ptr<Module>>(newPos, it->second));

		for (auto& obj : it->second->GetObjectDictionary())
		{
			std::shared_ptr<Range> range;
			Result res = this->GetRange(obj.second->GetRangeSelector().get(), range);
			if (!res.IsSuccessful())
				return res;
			if (range->GetSortMode() == SortMode::INDEX)
			{
				for (auto& subObj : obj.second->GetSubObjectDictionary())
				{
					subObj.second->SetModulePosition(newPos);
				}
			}
		}
		this->moduleCollection.erase(it);
		return Result();
	}
	else if (this->moduleCollection.find(oldPos) != this->moduleCollection.end() && this->moduleCollection.find(newPos) != this->moduleCollection.end())
	{
		auto oldModule = this->moduleCollection.find(oldPos);
		auto newModule = this->moduleCollection.find(newPos);

		std::pair<std::uint32_t, std::shared_ptr<Module>> switchedNewModule(newPos, oldModule->second);
		std::pair<std::uint32_t, std::shared_ptr<Module>> switchedOldModule(oldPos, newModule->second);

		oldModule->second->SetPosition(newPos);
		newModule->second->SetPosition(oldPos);

		this->moduleCollection.erase(oldModule);
		this->moduleCollection.erase(newModule);

		this->moduleCollection.insert(switchedNewModule);
		this->moduleCollection.insert(switchedOldModule);

		for (auto& obj : switchedOldModule.second->GetObjectDictionary())
		{
			std::shared_ptr<Range> range;
			Result res = this->GetRange(obj.second->GetRangeSelector().get(), range);
			if (!res.IsSuccessful())
				return res;
			if (range->GetSortMode() == SortMode::INDEX)
			{
				for (auto& subObj : obj.second->GetSubObjectDictionary())
				{
					subObj.second->SetModulePosition(oldPos);
				}
			}
		}

		for (auto& obj : switchedNewModule.second->GetObjectDictionary())
		{
			std::shared_ptr<Range> range;
			Result res = this->GetRange(obj.second->GetRangeSelector().get(), range);
			if (!res.IsSuccessful())
				return res;
			if (range->GetSortMode() == SortMode::INDEX)
			{
				for (auto& subObj : obj.second->GetSubObjectDictionary())
				{
					subObj.second->SetModulePosition(newPos);
				}
			}
		}
		return Result();
	}
	boost::format formatter(kMsgModuleDoesNotExists[static_cast<std::underlying_type<Language>::type>(LoggingConfiguration::GetInstance().GetCurrentLanguage())]);
	formatter
	% moduleId
	% oldPos;
	LOG_ERROR() << formatter.str();
	return Result(ErrorCode::MODULE_DOES_NOT_EXIST, formatter.str());
}

Result Interface::EnableModule(const std::string& moduleId, std::uint32_t modulePosition, bool enable)
{
	if (this->moduleCollection.find(modulePosition) != this->moduleCollection.end())
	{
		auto module = this->moduleCollection.find(modulePosition)->second;
		if (module->GetModuleId() == moduleId)
		{
			module->SetEnabled(enable);
			return Result();
		}
		else
		{
			boost::format formatter(kMsgModuleDoesNotMatch[static_cast<std::underlying_type<Language>::type>(LoggingConfiguration::GetInstance().GetCurrentLanguage())]);
			formatter
			% moduleId
			% module->GetModuleId()
			% modulePosition;
			LOG_ERROR() << formatter.str();
			return Result(ErrorCode::MODULE_ID_DOES_NOT_MATCH, formatter.str());
		}
	}
	boost::format formatter(kMsgModuleDoesNotExists[static_cast<std::underlying_type<Language>::type>(LoggingConfiguration::GetInstance().GetCurrentLanguage())]);
	formatter
	% moduleId
	% modulePosition;
	LOG_ERROR() << formatter.str();
	return Result(ErrorCode::MODULE_DOES_NOT_EXIST, formatter.str());
}

const std::map<std::uint32_t, std::shared_ptr<Module>>& Interface::GetModuleCollection()
{
	return this->moduleCollection;
}

const std::vector<std::shared_ptr<Range>>& Interface::GetRangeList()
{
	return this->rangeList;
}
