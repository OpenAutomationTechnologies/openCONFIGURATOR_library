/************************************************************************
\file ModularControlledNode.cpp
\brief Implementation of the Class ModularControlledNode
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
#include "ModularControlledNode.h"

using namespace IndustrialNetwork::POWERLINK::Core::Node;
using namespace IndustrialNetwork::POWERLINK::Core::ModularNode;
using namespace IndustrialNetwork::POWERLINK::Core::ErrorHandling;
using namespace IndustrialNetwork::POWERLINK::Core::ObjectDictionary;
using namespace IndustrialNetwork::POWERLINK::Core::CoreConfiguration;

ModularControlledNode::ModularControlledNode(std::uint8_t nodeID, const std::string& nodeName) : ControlledNode(nodeID, nodeName),
	interfaceList(std::vector<std::shared_ptr<IndustrialNetwork::POWERLINK::Core::ModularNode::Interface>>())
{}

ModularControlledNode::~ModularControlledNode()
{}

Result ModularControlledNode::AddInterface(const std::string& uniqueId, const std::string& type, ModuleAddressing moduleAddressing, std::uint32_t maxModules, bool unusedSlots, bool multipleModules)
{
	for (auto& interf : this->interfaceList)
	{
		if (interf->GetUniqueId() == uniqueId)
		{
			boost::format formatter(kMsgInterfaceAlreadyExists);
			formatter
			% uniqueId;
			LOG_ERROR() << formatter.str();
			return Result(ErrorCode::INTERFACE_ALREADY_EXISTS, formatter.str());
		}
	}
	this->interfaceList.push_back(std::shared_ptr<Interface>(new Interface(uniqueId, type, moduleAddressing, maxModules, unusedSlots, multipleModules)));
	return Result();
}

Result ModularControlledNode::GetModule(const std::string& interfaceId, const std::string& moduleId, std::uint32_t modulePosition, std::shared_ptr<Module>& moduleRet)
{
	for (auto& interf : this->interfaceList)
	{
		if (interf->GetUniqueId() == interfaceId)
		{
			return interf->GetModule(moduleId, modulePosition, moduleRet);
		}
	}
	boost::format formatter(kMsgInterfaceDoesNotExists);
	formatter
	% interfaceId;
	LOG_ERROR() << formatter.str();
	return Result(ErrorCode::INTERFACE_DOES_NOT_EXIST, formatter.str());
}

Result ModularControlledNode::AddModule(const std::string& interfaceId, const std::string& moduleId, const std::string& moduleType, ModuleAddressing addressing, std::uint32_t modulePosition, std::uint32_t moduleAddress, const std::string& moduleName, std::uint16_t minPosition, std::uint16_t maxPosition, std::uint16_t minAddress, std::uint16_t maxAddress, std::uint16_t maxCount)
{
	for (auto& interf : this->interfaceList)
	{
		if (interf->GetUniqueId() == interfaceId)
		{
			return interf->AddModule(modulePosition, std::shared_ptr<Module>(new Module(this->GetNodeId(), moduleId, moduleType, addressing, moduleAddress, modulePosition, moduleName, minPosition, maxPosition, minAddress, maxAddress, maxCount)));
		}
	}
	boost::format formatter(kMsgInterfaceDoesNotExists);
	formatter
	% interfaceId;
	LOG_ERROR() << formatter.str();
	return Result(ErrorCode::INTERFACE_DOES_NOT_EXIST, formatter.str());
}

Result ModularControlledNode::AddRange(const std::string& interfaceId, const std::string& _name, std::uint32_t baseIndex, std::uint32_t maxIndex, std::uint32_t maxSubIndex, std::uint32_t sortStep, const SortMode& sortMode, const SortNumber& sortNumber, const PDOMapping& pdoMapping)
{
	for (auto& interf : this->interfaceList)
	{
		if (interf->GetUniqueId() == interfaceId)
		{
			if (sortNumber == SortNumber::ADDRESS && sortMode == SortMode::SUBINDEX)
			{
				boost::format formatter(kMsgRangeInvalid);
				formatter
				% _name;
				LOG_ERROR() << formatter.str();
				return Result(ErrorCode::RANGE_INVALID, formatter.str());
			}
			std::shared_ptr<Range> range = std::shared_ptr<Range>(new Range(_name, baseIndex, maxIndex, maxSubIndex, sortStep, sortMode, sortNumber, pdoMapping));
			return interf->AddRange(range);
		}
	}
	boost::format formatter(kMsgInterfaceDoesNotExists);
	formatter
	% interfaceId;
	LOG_ERROR() << formatter.str();
	return Result(ErrorCode::INTERFACE_DOES_NOT_EXIST, formatter.str());
}

Result ModularControlledNode::ChangeModuleOrderOnInterface(const std::string& interfaceId, const std::string& moduleId, std::uint32_t oldPos, std::uint32_t newPos)
{
	for (auto& interf : this->interfaceList)
	{
		if (interf->GetUniqueId() == interfaceId)
		{
			Result res = interf->ChangeModulePosition(moduleId, oldPos, newPos);
			if (!res.IsSuccessful())
				return res;
			return this->UpdateControlledNodeOd();
		}
	}
	boost::format formatter(kMsgInterfaceDoesNotExists);
	formatter
	% interfaceId;
	LOG_ERROR() << formatter.str();
	return Result(ErrorCode::INTERFACE_DOES_NOT_EXIST, formatter.str());
}

Result ModularControlledNode::RemoveModule(const std::string& interfaceId, const std::string& moduleId, std::uint32_t position)
{
	for (auto& interf : this->interfaceList)
	{
		if (interf->GetUniqueId() == interfaceId)
		{
			if (interf->GetModuleCollection().find(position) != interf->GetModuleCollection().end())
			{
				for (auto param : interf->GetModuleCollection().find(position)->second->GetParameterNameMapping())
				{
					Result res = this->GetApplicationProcess()->RemoveParameter(param.second);
					if (!res.IsSuccessful())
						return res;
				}
			}
			else
			{
				boost::format formatter(kMsgModuleDoesNotExists);
				formatter
				% moduleId
				% position;
				LOG_ERROR() << formatter.str();
				return Result(ErrorCode::MODULE_DOES_NOT_EXIST, formatter.str());
			}

			std::shared_ptr<Module> module;
			Result res = this->GetModule(interfaceId, moduleId, position, module);
			if (!res.IsSuccessful())
				return res;

			if (module->GetObjectDictionary().size() != 0)
			{
				std::shared_ptr<Range> range;
				res = interf->GetRange(module->GetObjectDictionary().begin()->second->GetRangeSelector().get(), range);
				if (!res.IsSuccessful())
					return res;

				for (auto& obj : module->GetObjectDictionary())
				{
					if (range->GetSortMode() == SortMode::INDEX)
						this->RemoveObjectFromOd(obj.second->GetObjectId());
					if (range->GetSortMode() == SortMode::SUBINDEX)
						this->RemoveSubObjectsFromOd(obj.second->GetObjectId(), module->GetPosition());
				}
			}
			res = interf->RemoveModule(moduleId, position);
			if (!res.IsSuccessful())
				return res;

			return this->UpdateControlledNodeOd();
		}
	}
	boost::format formatter(kMsgInterfaceDoesNotExists);
	formatter
	% interfaceId;
	LOG_ERROR() << formatter.str();
	return Result(ErrorCode::INTERFACE_DOES_NOT_EXIST, formatter.str());
}

Result ModularControlledNode::AddObjectToModule(const std::string& interfaceId, std::uint32_t modulePosition, const std::string& moduleId, const std::string& rangeSelector, std::uint32_t& objectId, std::shared_ptr<IndustrialNetwork::POWERLINK::Core::ObjectDictionary::Object> obj, bool createNew)
{
	for (auto& inter : this->interfaceList)
	{
		if (inter->GetUniqueId() == interfaceId)
		{
			std::shared_ptr<Module> module;
			Result res = inter->GetModule(moduleId, modulePosition, module);
			if (!res.IsSuccessful())
				return res;

			std::shared_ptr<Range> range;
			res = inter->GetRange(rangeSelector, range);
			if (!res.IsSuccessful())
				return res;

			std::uint32_t originalId = objectId;
			res = range->GetNextIndex(objectId, module->GetAddress());
			if (!res.IsSuccessful())
				return res;

			std::shared_ptr<Object> existingObj;
			res = this->GetObject(objectId, existingObj, false);
			if (res.IsSuccessful() && range->GetSortMode() == SortMode::SUBINDEX)
			{
				existingObj->SetId(originalId);
				res = module->AddObject(existingObj);
				if (!res.IsSuccessful())
					return res;
				existingObj->SetId(objectId);
				return Result();
			}
			else
			{
				obj->SetRangeSelector(rangeSelector);

				if (createNew)
				{
					res = module->AddObject(obj);
					if (!res.IsSuccessful())
						return res;
				}

				//Change object Id before adding to nodes od
				obj->SetId(objectId);

				if (range->GetPdoMpapping() != PDOMapping::UNDEFINED)
					obj->SetPDOMapping(range->GetPdoMpapping());

				if (module->IsEnabled())
				{
					res = this->AddObject(obj);
					if (!res.IsSuccessful())
						return res;
				}
				return Result();
			}
		}
	}
	boost::format formatter(kMsgInterfaceDoesNotExists);
	formatter
	% interfaceId;
	LOG_ERROR() << formatter.str();
	return Result(ErrorCode::INTERFACE_DOES_NOT_EXIST, formatter.str());
}

Result ModularControlledNode::AddSubObjectToModule(const std::string& interfaceId, std::uint32_t modulePosition, const std::string& moduleId, std::uint32_t& objectId, std::uint16_t& subObjectId, std::shared_ptr<IndustrialNetwork::POWERLINK::Core::ObjectDictionary::SubObject> subObj, bool createNew)
{
	for (auto& inter : this->interfaceList)
	{
		if (inter->GetUniqueId() == interfaceId)
		{
			std::shared_ptr<Module> module;
			Result res = inter->GetModule(moduleId, modulePosition, module);
			if (!res.IsSuccessful())
				return res;

			std::shared_ptr<Object> object;
			res = module->GetObject(objectId, object);
			if (!res.IsSuccessful())
				return res;

			std::shared_ptr<Range> range;
			res = inter->GetRange(object->GetRangeSelector().get(), range);
			if (!res.IsSuccessful())
				return res;

			std::uint32_t objectCurrentId = object->GetObjectId();
			res = range->GetNextSubIndex(objectCurrentId, subObjectId, module->GetAddress());
			if (!res.IsSuccessful())
				return res;

			subObj->SetId(subObjectId);
			subObj->SetModulePosition(module->GetPosition());

			if (createNew)
			{
				res = module->AddSubObject(objectId, subObj);
				if (!res.IsSuccessful())
					return res;
			}
			objectId = objectCurrentId;

			if (range->GetPdoMpapping() != PDOMapping::UNDEFINED)
				subObj->SetPDOMapping(range->GetPdoMpapping());

			return Result();
		}
	}
	boost::format formatter(kMsgInterfaceDoesNotExists);
	formatter
	% interfaceId;
	LOG_ERROR() << formatter.str();
	return Result(ErrorCode::INTERFACE_DOES_NOT_EXIST, formatter.str());
}

Result ModularControlledNode::EnableModule(const std::string& interfaceId, const std::string& moduleId, std::uint32_t modulePosition, bool enable)
{
	for (auto& interf : this->interfaceList)
	{
		if (interf->GetUniqueId() == interfaceId)
		{
			Result res = interf->EnableModule(moduleId, modulePosition, enable);
			if (!res.IsSuccessful())
				return res;
			return this->UpdateControlledNodeOd();
		}
	}
	boost::format formatter(kMsgInterfaceDoesNotExists);
	formatter
	% interfaceId;
	LOG_ERROR() << formatter.str();
	return Result(ErrorCode::INTERFACE_DOES_NOT_EXIST, formatter.str());
}

Result ModularControlledNode::RemoveObjectFromOd(std::uint32_t objectId)
{
	if (this->GetObjectDictionary().find(objectId) != this->GetObjectDictionary().end())
	{
		auto obj = this->GetObjectDictionary().find(objectId);
		if (obj->second->GetRangeSelector().is_initialized())
			this->GetObjectDictionary().erase(objectId);
		return Result();
	}
	boost::format formatter(kMsgNonExistingObject);
	formatter
	% objectId
	% (std::uint32_t) this->GetNodeId();
	return Result(ErrorCode::OBJECT_DOES_NOT_EXIST, formatter.str());
}

Result ModularControlledNode::RemoveSubObjectsFromOd(std::uint32_t objectId, std::uint32_t subObjectId)
{
	if (this->GetObjectDictionary().find(objectId) != this->GetObjectDictionary().end())
	{
		auto obj = this->GetObjectDictionary().find(objectId);
		if (obj->second->GetSubObjectDictionary().find(subObjectId) != obj->second->GetSubObjectDictionary().end())
		{
			obj->second->GetSubObjectDictionary().erase(obj->second->GetSubObjectDictionary().find(subObjectId));
		}
		return Result();
	}
	boost::format formatter(kMsgNonExistingObject);
	formatter
	% objectId
	% (std::uint32_t) this->GetNodeId();
	return Result(ErrorCode::OBJECT_DOES_NOT_EXIST, formatter.str());
}

Result ModularControlledNode::UpdateControlledNodeOd()
{
	for (auto& inter : this->interfaceList)
	{
		for (auto& range : inter->GetRangeList())
		{
			range->Reset();
		}

		for (auto& module : inter->GetModuleCollection())
		{
			for (auto& obj : module.second->GetObjectDictionary())
			{
				std::shared_ptr<Range> range;
				Result res = inter->GetRange(obj.second->GetRangeSelector().get(), range);
				if (!res.IsSuccessful())
					return res;

				if (range->GetSortMode() == SortMode::SUBINDEX && inter->GetModuleAddressing() == ModuleAddressing::POSITION)
				{
					for (auto subObj = obj.second->GetSubObjectDictionary().begin(); subObj != obj.second->GetSubObjectDictionary().end(); ++subObj)
					{
						bool moduleDisabled = false;

						for (auto& mod : inter->GetModuleCollection())
						{
							if (mod.first == subObj->first && mod.second->IsEnabled() == false)
								moduleDisabled = true;
						}

						//Module has been disabled
						if (moduleDisabled == true)
						{
							if (inter->GetModuleCollection().find(subObj->first) != inter->GetModuleCollection().end())
							{
								std::shared_ptr<Module> mod = inter->GetModuleCollection().at(subObj->first);
								mod->GetDisabledSubindices().clear();

								std::pair<std::uint32_t, std::uint32_t> position = std::pair<std::uint32_t, std::uint32_t>(obj.first, subObj->first);
								mod->GetDisabledSubindices().insert(std::pair<std::pair<std::uint32_t, std::uint32_t>, std::shared_ptr<SubObject>>(position, subObj->second));

								subObj = obj.second->GetSubObjectDictionary().erase(subObj);
							}
						}

						if (subObj == obj.second->GetSubObjectDictionary().end())
							break;
					}
				}

				if (range->GetSortMode() == SortMode::INDEX)
				{
					this->RemoveObjectFromOd(obj.second->GetObjectId());
				}
				//if module is not enable do not add the object to the head node od
				if (module.second->IsEnabled() == false)
					continue;

				std::uint32_t objectId = obj.first;
				if (range->GetSortMode() == SortMode::INDEX)
				{
					res = this->AddObjectToModule(inter->GetUniqueId(), module.first, module.second->GetModuleId(), obj.second->GetRangeSelector().get(), objectId, obj.second, false);
					if (!res.IsSuccessful())
						return res;
				}

				for (auto& subObj : module.second->GetDisabledSubindices())
				{
					if (subObj.first.first == obj.first)
					{
						objectId = obj.first;
						std::uint16_t subObjectId = (std::uint16_t) subObj.second->GetOriginalId();
						res = this->AddSubObjectToModule(inter->GetUniqueId(), module.first, module.second->GetModuleId(), objectId, subObjectId, subObj.second, true);
						if (!res.IsSuccessful())
							return res;
					}
				}

			}
		}
	}
	return Result();
}

Result ModularControlledNode::GetModuleObjectCurrentIndex(const std::string& interfaceId, const std::string& moduleId, std::uint32_t modulePosition, std::uint32_t originalObjectId, std::int32_t originalSubObjectId, std::uint32_t& objectId, std::int32_t& subObjectId)
{
	for (auto& interf : this->interfaceList)
	{
		if (interf->GetUniqueId() == interfaceId)
		{
			std::shared_ptr<Module> module;
			Result res = interf->GetModule(moduleId, modulePosition, module);
			if (!res.IsSuccessful())
				return res;

			auto& moduleOd = module->GetObjectDictionary();
			if (moduleOd.find(originalObjectId) != moduleOd.end())
			{
				if (originalSubObjectId == -1)
				{
					objectId = moduleOd.find(originalObjectId)->second->GetObjectId();
					return Result();
				}
				else
				{
					for (auto& subObj : moduleOd.find(originalObjectId)->second->GetSubObjectDictionary())
					{
						if ((std::int32_t) subObj.second->GetOriginalId() == originalSubObjectId && subObj.second->GetModulePosition() == module->GetPosition())
						{
							subObjectId = (std::int32_t) subObj.first;
							objectId = moduleOd.find(originalObjectId)->second->GetObjectId();
							return Result();
						}
					}
					boost::format formatter(kMsgNonExistingSubObject);
					formatter
					% originalObjectId
					% originalSubObjectId
					% (std::uint32_t) this->GetNodeId();
					return Result(ErrorCode::SUBOBJECT_DOES_NOT_EXIST, formatter.str());
				}
			}
			else
			{
				boost::format formatter(kMsgNonExistingObject);
				formatter
				% originalObjectId
				% (std::uint32_t) this->GetNodeId();
				LOG_ERROR() << formatter.str();
				return Result(ErrorCode::OBJECT_DOES_NOT_EXIST, formatter.str());
			}
		}
	}
	boost::format formatter(kMsgInterfaceDoesNotExists);
	formatter
	% interfaceId;
	LOG_ERROR() << formatter.str();
	return Result(ErrorCode::INTERFACE_DOES_NOT_EXIST, formatter.str());
}

Result ModularControlledNode::GetParameterCurrentName(const std::string& interfaceId, const std::string& moduleId, std::uint32_t modulePosition, const std::string& originalParamName, std::string& parameterName)
{
	for (auto& interf : this->interfaceList)
	{
		if (interf->GetUniqueId() == interfaceId)
		{
			std::shared_ptr<Module> module;
			Result res = interf->GetModule(moduleId, modulePosition, module);
			if (!res.IsSuccessful())
				return res;

			return module->GetMappedParameterName(originalParamName, parameterName);
		}
	}
	boost::format formatter(kMsgInterfaceDoesNotExists);
	formatter
	% interfaceId;
	LOG_ERROR() << formatter.str();
	return Result(ErrorCode::INTERFACE_DOES_NOT_EXIST, formatter.str());
}

Result ModularControlledNode::SetModuleAddress(const std::string& interfaceId, const std::string& moduleId, std::uint32_t modulePosition, std::uint32_t address)
{
	for (auto& interf : this->interfaceList)
	{
		if (interf->GetUniqueId() == interfaceId)
		{
			std::shared_ptr<Module> module;
			Result res = this->GetModule(interfaceId, moduleId, modulePosition, module);
			if (!res.IsSuccessful())
				return res;

			//Adress is already correct
			if (module->GetAddress() == address)
				return Result();

			for (auto& mod : interf->GetModuleCollection())
			{
				if (mod.second->GetAddress() == address)
				{
					boost::format formatter(kMsgModuleAddressOccupied);
					formatter
					% moduleId
					% address;
					LOG_ERROR() << formatter.str();
					return Result(ErrorCode::ADDRESS_OCCUPIED, formatter.str());
				}
			}

			module->SetAddress(address);
			return this->UpdateControlledNodeOd();
		}
	}
	boost::format formatter(kMsgInterfaceDoesNotExists);
	formatter
	% interfaceId;
	LOG_ERROR() << formatter.str();
	return Result(ErrorCode::INTERFACE_DOES_NOT_EXIST, formatter.str());
}
