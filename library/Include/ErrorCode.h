/************************************************************************
\file ErrorCode.h
\brief Implementation of the Enumeration ErrorCode
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
#if !defined ERROR_CODE_H
#define ERROR_CODE_H

#include <cstdint>

#include "Constants.h"

namespace IndustrialNetwork
{
	namespace POWERLINK
	{
		namespace Core
		{
			/**
			\brief Error handling related namespace
			\author rueckerc, Bernecker+Rainer Industrie Elektronik Ges.m.b.H.
			*/
			namespace ErrorHandling
			{
				/**
				\brief Represents the error codes returned by the openCONFIGURATOR core.
				\author rueckerc, Bernecker+Rainer Industrie Elektronik Ges.m.b.H.
				*/
				/** \addtogroup apireturn
				* @{
				*/
				DLLEXPORT enum class ErrorCode : std::uint8_t
				{

					SUCCESS = 0,//!< Operation completed successfully.
					FILE_WRITE_FAILED, //!< Cannot write to file.
					FILE_READ_FAILED, //!< Cannot read from file
					SUBOBJECT_INVALID, //!< Invalid subindex encountered during operation.
					OBJECT_INVALID, //!< Invalid index encountered during operation.
					NODEID_INVALID, //!< Invalid node id encountered during operation.
					ATTRIBUTEVALUE_INVALID, //!< Invalid attribute value encountered during operation.
					ATTRIBUTEVALUE_NOT_IN_RANGE, //!< Attribute value not in valid range.
					UNSUPPORTED_ATTRIBUTETYPE, //!< Attribute type not supported by operation.
					NO_NODES_CONFIGURED, //!< Network does not contain any nodes.
					NO_CONTROLLED_NODES_CONFIGURED, //!< Network does not contain any controlled nodes.
					NO_MANAGING_NODE_CONFIGURED, //!< Network does not contain a managing node.
					OD_EMPTY, //!< Object dictionary is empty.
					OBJECT_CONTAINS_NO_SUBOBJECTS, //!< %Index does not contain subindices.
					NODE_DOES_NOT_EXIST, //!< %Node does not exist in the network.
					OBJECT_DOES_NOT_EXIST, //!< %Index does not exist on %Node.
					SUBOBJECT_DOES_NOT_EXIST, //!< %SubIndex does not exist in %Index.
					NODE_EXISTS, //!< %Node already exists in the network.
					OBJECT_EXISTS, //!< %Index already exists on a %Node.
					SUBOBJECT_EXISTS, //!< %SubIndex already exists for an %Index.
					TPDO_CHANNEL_COUNT_EXCEEDED, //!< Max. no. of TPDO-Channels exceeded.
					NODE_CONFIGURATION_ERROR, //!< %Node configuration error occured.
					MAPPED_OBJECT_DOES_NOT_EXIST, //!< A mapped %Index does not exist on %Node.
					MAPPED_SUBOBJECT_DOES_NOT_EXIST, //!< A mapped %SubIndex does not exist on %Node.
					INSUFFICIENT_MAPPING_OBJECTS, //!< Insufficient mapping objects on the %Node.
					PARAMETER_NOT_FOUND, //!< %Parameter not found on a %Node.
					STRUCT_DATATYPE_NOT_FOUND, //!< Struct datatype not found on a %Node.
					SIMPLE_DATATYPE_NOT_FOUND, //!< Simple datatype not found on the %Node.
					VALUE_NOT_WITHIN_RANGE, //!< Value not within a valid range.
					CHANNEL_PAYLOAD_LIMIT_EXCEEDED, //!< Max. Channel-Payload exceeded.
					CHANNEL_OBJECT_LIMIT_EXCEEDED, //!< Max. no. of Channel-Objects exceeded.
					PDO_DATATYPE_INVALID, //!< PDO datatype invalid.
					UNSUPPORTED_PI_GEN_LANGUAGE, //!< Process image generation not supported for given programming language.
					MAX_PI_SIZE_EXCEEDED, //!< Maximum size of process image exceeded.
					MULTIPLEXING_NOT_SUPPORTED, //!< Multiplexing not supported.
					MULTIPLEX_CYCLE_ASSIGN_INVALID, //!< Invalid multiplexed cycle assigned to %Node.
					OBJECT_LIMITS_INVALID, //!< High-/Lowlimit of the object are invalid.
					LOW_CN_PRES_TIMEOUT, //!< CNPResTimeout (0x1F92) is too low.
					CROSS_TRAFFIC_STATION_LIMIT_EXCEEDED, //!< CN exceeds the no. of allowed cross-traffic stations (device description entry D_PDO_RPDOChannels_U16).
					ARGUMENT_INVALID_NULL, //!< Invalid function-argument (NULL).
					ARGUMENT_INVALID_EMPTY, //!< Invalid function-argument (empty).
					UNHANDLED_EXCEPTION, //!< Unhandled exception occurred during operation.
					NO_DEFAULT_OR_ACTUAL_VALUE, //!< No default or actual value defined for object.
					FEATURE_VALUE_NOT_FOUND, //!< CN-/MN- or GeneralFeature not found in a node's XDD.
					PLKDATATYPE_SIZE_UNDEFINED, //!< The given POWERLINK data type has no defined size.
					OBJECT_SIZE_MAPPED_INVALID, //!< Mapped object size invalid.
					MAPPING_TYPE_FOR_PDO_INVALID, //!< Mapped object has an invalid/incompatible PDOmapping.
					ACCESS_TYPE_FOR_PARAMETER_INVALID, //!< A mapped object's referenced %Parameter has an invalid/incompatible accessType.
					ACCESS_TYPE_FOR_PDO_INVALID, //!< A mapped object has an invalid/incompatible accessType.
					PDO_OFFSET_INVALID, //!< A mapped object has an invalid offset (non-contigous).
					NO_PROJECT_LOADED, //!< No project loaded.
					MAPPING_INVALID, //!< An enabled mapping entry is empty.
					PARAMETER_VALUE_NOT_SET, //!< A specific parameter value is not set.
					PARAMETER_VALUE_INVALID, //!< A value is invalid for a specific parameter.
					PATH_EXISTS, //!< A path already exists in the project configuration.
					PATH_DOES_NOT_EXIST, //!< A path does not exists in the project configuration.
					VIEW_SETTING_EXISTS, //!< A view setting exists in the project configuration.
					VIEW_SETTING_DOES_NOT_EXIST, //!< A view setting does not exist in the project configuration.
					VIEW_SETTINGS_DOES_NOT_EXIST, //!< A view settings group does not exist in the project configuration.
					BUILD_SETTING_EXISTS, //!< An auto generation setting already exists in the project configuration.
					BUILD_SETTING_DOES_NOT_EXIST, //!< An auto generation setting does not exist in the project configuration.
					BUILD_CONFIGURATION_EXISTS, //!< An auto generation settings group does already exist in the project configuration.
					BUILD_CONFIGURATION_DOES_NOT_EXIST, //!< An auto generation settings group does not exist in the project configuration.
					BUILD_CONFIGURATION_IS_ACTIVE, //!< The build configuration group is set active in the project configuration.
					NETWORK_EXISTS, //!< The network configuration already exists in the library.
					NETWORK_DOES_NOT_EXIST, //!< The network configuration already not exist in the library.
					DATATYPE_MISMATCH, //!< The conversion of a requested value into the destined datatype is not possible.
					COMPLEX_DATATYPE_NOT_FOUND, //!< A complex datatype does not exist for a node.
					VAR_DECLARATION_EXISTS, //!< A VarDeclaration with the same uniqueId already exists.
					ENUM_VALUE_EXISTS, //!< An Enum Value with the same name already exists.
					FORCED_VALUE_OVERWRITE, //!< A forced value must not be overwritten by the library.
					CYCLE_TIME_NOT_SET, //!< An actual value for cycle time must be configured.
					OBJECT_ACTUAL_VALUE_EXCEEDS_HIGHLIMIT, //!< An actual value exceeds its high limit.
					OBJECT_ACTUAL_VALUE_DECEEDS_LOWLIMIT, //!< An actual value deceeds its low limit.
					CHAINING_NOT_SUPPORTED, //!< Chaining not supported.
					OBJECT_HAS_NO_ACTUAL_VALUE, //!< The object does not have a defined actual value.
					SUB_OBJECT_HAS_NO_ACTUAL_VALUE, //!< The sub object does not have a defined actual value.
					OBJECT_HAS_NO_DEFAULT_VALUE, //!< The object does not have a defined default value.
					SUB_OBJECT_HAS_NO_DEFAULT_VALUE, //!< The sub object does not have a defined default value.
					OBJECT_HAS_NO_DATATYPE, //!< An object has no defined data type.
					DATATYPE_DOES_NOT_SUPPORT_LIMITS, //!< A data type that does not support the definition of high and low limit.
					OBJECT_TYPE_DOES_NOT_SUPPORT_VALUES, //!< Only objects with object type VAR may have values.
					ACTUAL_VALUE_INVALID, //!< Actual value exceeds datatype limits.
					DEFAULT_VALUE_INVALID, //!< Default value exceeds datatype limits.
					HIGH_LIMIT_INVALID, //!< High limit exceeds datatype limits.
					LOW_LIMIT_INVALID, //!< Low limit exceeds datatype limits.
					GENERAL_FEATURE_VALUE_INVALID, //!< General feature value invalid for datatype.
					CN_FEATURE_VALUE_INVALID, //!< CN Feature value invalid for datatype.
					MN_FEATURE_VALUE_INVALID, //!< MN Feature value invalid for datatype.
					NODE_ASSIGNMENT_NOT_SUPPORTED, //!< Node assignment not supported by node.
					NODE_ASSIGNMENT_EXISTS, //!< Node assignment is already set.
					MANAGING_NODE_DISABLE_INVALID, //!<  The managing node cannot be disabled.
					NODE_IS_NOT_MANAGING_NODE, //!< The node is not a managing node.
					NODE_IS_NOT_CONTROLLED_NODE, //!< The node is not a controlled node.
					NODE_IS_NOT_REDUNDANT_MANAGING_NODE, //!< The node is not a redundant managing node.
					DYNAMIC_CHANNEL_NOT_FOUND, //!< Dynamic Channel not found.
					PARAMETER_ACCESS_INVALID, //!< Parameter access invalid.
					OBJECT_ACCESS_DOES_NOT_SUPPORT_VALUES, //!< Object access does not allow values
					CYCLE_TIME_ERROR, //!< Cycle time does not meet criteria of the network
					PARAMETER_HAS_NO_DATATYPE, //!< Parameter has no IEC datatype
					PARAMETER_HAS_NO_ACTUAL_VALUE, //!< Parameter does not have an actual value
					PARAMETER_HAS_NO_DEFAULT_VALUE, //!< Parameter has no default value
					PARAMETER_EXISTS, //<! Parameter already exists
					PARAMETER_GROUP_EXISTS, //<! Parameter group already exists
					PARAMETER_TEMPLATE_EXISTS, //<! Parameter template already exists
					PARAMETER_GROUP_DOES_NOT_EXIST, //<! Parameter group does not exist
					PARAMETER_TEMPLATE_DOES_NOT_EXIST, //<! Parameter template does not exist
					COMPLEX_DATATYPE_ALREADY_EXIST //!< A complex datatype already exists on a node.
				};
				/** @} */
			}
		}
	}
}
#endif
