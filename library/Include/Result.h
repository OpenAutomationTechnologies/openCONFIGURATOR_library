/************************************************************************
\file Result.h
\brief Implementation of the Class Result
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
#if !defined RESULT_H
#define RESULT_H

#include <string>

#include "IResult.h"
#include "ErrorCode.h"
#include "Constants.h"

namespace IndustrialNetwork
{
	namespace POWERLINK
	{
		namespace Core
		{
			namespace ErrorHandling
			{
				const std::string kMsgEmptyArgument = "Argument(s) \"%s\" must not be empty.";
				const std::string kMsgLoggingInitialised = "Logging has been initialised with configuration: \"%s\".";

				const std::string kMsgExistingNetwork = "Network with id \"%s\" already exists.";
				const std::string kMsgNonExistingNetwork = "Network with id \"%s\" does not exists.";
				const std::string kMsgNetworkCreated = "Network with id \"%s\" created.";
				const std::string kMsgNetworkRemoved = "Network with id \"%s\" removed.";

				const std::string kMsgExistingNode = "Node with id %d already exists.";
				const std::string kMsgNonExistingNode = "Node with id %d does not exist.";
				const std::string kMsgNonManagingNode = "Node with id %d is not a managing node or redundant managing node.";
				const std::string kMsgNonControlledNode = "Node with id %d is not a controlled node.";
				const std::string kMsgNonRedundantManagingNode = "Node with id %d is not a redundant managing node.";

				const std::string kMsgNodeCreated = "Node with id %d created.";
				const std::string kMsgNodeRemoved = "Node with id %d removed.";
				const std::string kMsgNodeObjectCount = "Node with id %d has %d configuration objects.";
				const std::string kMsgNodeObjectCountSize = "Node with id %d has %d configuration objects size.";
				const std::string kMsgManagingNodeDisable = "Managing node of network \"%s\" cannot be disabled.";
				const std::string kMsgNodeDisable = "Node with Id \"%d\" set enabled to \"%s\".";
				const std::string kMsgMultiplexingNotSupported = "Multiplexing is not supported by controlled node \"%d\"";
				const std::string kMsgChainingNotSupported = "PRes Chaining is not supported by controlled node \"%d\"";
				const std::string kMsgMultiplexCycleAssignInvalid = "Multiplexed cycle \"%d\" assigned to node %d exceeds multiplexed cycle length (%d).";
				const std::string kMsgMultiplexCycleAlreadyAssigned = "Multiplexed cycle \"%d\" already assigned.";

				const std::string kMsgExistingObject = "Object 0x%X already exists on node %d.";
				const std::string kMsgExistingSubObject = "Subobject 0x%X/0x%X already exists on node %d.";
				const std::string kMsgObjectCreated = "Object 0x%X created on node %d.";
				const std::string kMsgSubObjectCreated = "Subobject 0x%X/0x%X created on node %d.";
				const std::string kMsgObjectNoActualValue = "Object 0x%X on node %d does not have an actual value.";
				const std::string kMsgSubObjectNoActualValue = "Subobject 0x%X/0x%X on node %d does not have an actual value.";
				const std::string kMsgNonExistingObject = "Object 0x%X does not exist on node %d.";
				const std::string kMsgNonExistingSubObject = "Subobject 0x%X/0x%X does not exist on node %d.";
				const std::string kMsgForceObject = "Set force flag on object 0x%X on node %d to \"%s\".";
				const std::string kMsgForceSubObject = "Set force flag on subobject 0x%X/0x%X on node %d to \"%s\".";
				const std::string kMsgSetObjectActualValue = "Set actual value: \"%s\" on object 0x%X on node %d.";
				const std::string kMsgSetSubObjectActualValue = "Set actual value: \"%s\" on subobject 0x%X/0x%X on node %d.";
				const std::string kMsgDatatypeMismatch = "Datatype \"%s\" does not match expected datatype \"%s\".";
				const std::string kMsgObjectDatatypeMismatch = "BaseObject \"%s\" datatype \"%s\" does not match expected datatype \"%s\".";
				const std::string kMsgActualValueDatatypeError = "Actual value \"%s\" with datatype \"%s\" does not fit the datatype limits or format.";
				const std::string kMsgDefaultValueDatatypeError = "Default value \"%s\" with datatype \"%s\" does not fit the datatype limits or format.";
				const std::string kMsgHighLimitDatatypeError = "Highlimit \"%s\" with datatype \"%s\" exceeds the datatype limits.";
				const std::string kMsgLowLimitDatatypeError = "Lowlimit \"%s\" with datatype \"%s\" deceeds the datatype limits.";
				const std::string kMsgBaseObjectLowLimitError = "Object \"%s\" with id \"0x%X\" on node \"%d\" actual value \"%s\" deceeds lowlimit \"%d\".";
				const std::string kMsgBaseObjectHighLimitSubObjectError = "Object \"%s\" \"0x%X/0x%X\" on node \"%d\" with actual value \"%s\" exceeds the highlimit.";
				const std::string kMsgBaseObjectLowLimitSubObjectError = "Object \"%s\" \"0x%X/0x%X\" on node \"%d\" with actual value \"%s\" deceeds the lowlimit.";
				const std::string kMsgBaseObjectHighLimitError = "Object \"%s\" with id \"0x%X\" on node \"%d\" actual value \"%s\" exceeds highlimit \"%d\".";
				const std::string kMsgBaseObjectDataTypeError = "Object \"%s\" with id \"0x%X\" on node \"%d\" does not have a specified datatype.";
				const std::string kMsgBaseObjectDefaultValue = "Object \"%s\" with id \"0x%X\" on node \"%d\" does not have a default value.";
				const std::string kMsgBaseObjectActualValue = "Object \"%s\" with id \"0x%X\" on node \"%d\" does not have an actual value.";
				const std::string kMsgBaseObjectValueSupport = "Object \"0x%X\" on node %d does not support actual values.";
				const std::string kMsgBaseSubObjectValueSupport = "Subobject \"0x%X/0x%X\" on node %d does not support actual values.";
				const std::string kMsgBaseObjectHighLimit = "Object \"%s\" with id \"0x%X\" on node \"%d\" does not have a high limit.";
				const std::string kMsgBaseObjectLowLimit = "Object \"%s\" with id \"0x%X\" on node \"%d\" does not have a low limit.";

				const std::string kMsgConfigurationSettingNotSupported = "Configuration setting \"%s\" is not supported by the library.";
				const std::string kMsgConfigurationNonExisting = "Build configuration \"%s\" does not exist in network \"%s\".";
				const std::string kMsgConfigurationExisting = "Build configuration \"%s\" does already exist in network \"%s\".";
				const std::string kMsgConfigurationAdded = "Build configuration \"%s\" added to network \"%s\".";
				const std::string kMsgConfigurationRemoved = "Build configuration \"%s\" removed from network \"%s\".";
				const std::string kMsgConfigurationActive = "Build configuration \"%s\" is active for network \"%s\".";
				const std::string kMsgConfigurationRenamed = "Build configuration \"%s\" is renamed to \"%s\" in network \"%s\".";
				const std::string kMsgConfigurationSettingNonExisting = "Build configuration setting \"%s\" does not exist in configuration \"%s\" in network \"%s\".";
				const std::string kMsgConfigurationSettingExisting = "Build configuration setting \"%s\" already exist in configuration \"%s\" in network \"%s\".";
				const std::string kMsgConfigurationSettingAdded = "Build configuration setting \"%s\" added to configuration \"%s\" in network \"%s\".";
				const std::string kMsgConfigurationSettingRemoved = "Build configuration setting \"%s\" removed from configuration \"%s\" in network \"%s\".";
				const std::string kMsgConfigurationSettingEnabled = "Build configuration setting \"%s\" in configuration \"%s\" in network \"%s\" set enabled: \"%s\".";

				const std::string kMsgComplexDatatypeNotFoundOnNode = "Complex datatype with uniqueID \"%s\" in node %d not found.";
				const std::string kMsgComplexDatatypeNotFound = "Complex datatype with uniqueID \"%s\" not found.";
				const std::string kMsgComplexDataTypeAlreadyExist = "Complex datatype with uniqueID \"%s\" already exists.";
				const std::string kMsgComplexDataTypeSizeInvalid = "Complex datatype with uniqueID \"%s\" size calculation failed.";
				const std::string kMsgComplexDataTypeCreated = "Complex datatype with uniqueID \"%s\" created.";

				const std::string kMsgStructDatatypeNotFound = "Struct with uniqueID \"%s\" in node %d not found.";

				const std::string kMsgParameterCreated = "Parameter with uniqueID \"%s\" created.";
				const std::string kMsgParameterReturned = "Parameter with uniqueID \"%s\" returned.";
				const std::string kMsgParameterNotFound = "Parameter with uniqueIDRef \"%s\" not found.";
				const std::string kMsgParameterAlreadyExist = "Parameter with uniqueID \"%s\" already exists.";
				const std::string kMsgParameterActualValueDoesNotExist = "Parameter with uniqueID \"%s\" does not have an actual value.";
				const std::string kMsgParameterGroupAlreadyExist = "ParameterGroup with uniqueID \"%s\" already exists.";
				const std::string kMsgParameterTemplateAlreadyExist = "ParameterTemplate with uniqueID \"%s\" already exists.";
				const std::string kMsgParameterGroupNonExisting = "ParameterGroup with uniqueID \"%s\" does not exist.";
				const std::string kMsgParameterTemplateNonExisting = "ParameterTemplate with uniqueID \"%s\" does not exist.";				

				const std::string kMsgNodeIdInvalid = "NodeId \"%d\" invalid.";
				const std::string kMsgSubIndexInvalid = "Subindex 0x%X/0x%X invalid for node %d.";
				const std::string kMsgIndexInvalid = "Index 0x%X invalid for node %d.";
				const std::string kMsgUnsupportedAttributeType = "Attributetype \"%d\" not supported for this operation.";
				const std::string kMsgCycleTimeOnMnNotSet = "Cycle time has to be set on the managing node of the POWERLINK network.";
				const std::string kMsgCycleTimeDefaultValue = "Cycle time forced to configuration because default and actual value are equal on the managing node.";

				const std::string kMsgObjectActualValueNotSet = "Object 0x%X on node %d does not have a defined actual value.";
				const std::string kMsgObjectDataTypeMismatch = "Object 0x%X datatype on node %d does not match the datatype to be retrieved.";
				const std::string kMsgSubObjectActualValueNotSet = "Object 0x%X on node %d does not have a defined actual value.";
				const std::string kMsgSubObjectDataTypeMismatch = "Object 0x%X datatype on node %d does not match the datatype to be retrieved.";

				const std::string kMsgForcedValueOverwriteObject = "Overwrite actual value: \"%s\" on object 0x%X on node %d failed because value has been forced.";
				const std::string kMsgForcedValueOverwriteSubObject = "Overwrite actual value: \"%s\" on subobject 0x%X/0x%X on node %d failed because value has been forced.";

				const std::string kMsgWriteManagingNodeObjectCount = "Managing node object count configuration write successful.";
				const std::string kMsgWriteNodeAssignment = "Node assignment configuration write successful.";
				const std::string kMsgWriteRedundantManagingNode = "Redundant Managing Node (%d) configuration write successful.";
				const std::string kMsgWriteManagingNode = "Managing Node (%d) configuration write successful.";
				const std::string kMsgWriteControlledNode = "Controlled Node (%d) configuration write successful.";
				const std::string kMsgWriteMappingObjects = "Mapping objects for Node (%d) write successful.";
				const std::string kMsgWriteMappingObjectsNrOfEntries = "Mapping objects NrOfEntries for Node (%d) write successful.";
				const std::string kMsgWriteMappingObjectsNrOfEntriesZero = "Mapping objects NrOfEntries zero for Node (%d) write successful.";
				const std::string kMsgWriteCommunicationRangeObjects = "Communication range objects for Node (%d) write successful.";
				const std::string kMsgWriteUserDefinedRangeObjects = "User defined range objects for Node (%d) write successful.";

				const std::string kMsgGeneralFeatureDatatypeError = "General feature \"%s\" actual / default value does not match datatype.";
				const std::string kMsgCnFeatureDatatypeError = "CN feature \"%s\" actual / default value does not match datatype.";
				const std::string kMsgMnFeatureDatatypeError = "MN feature \"%s\" actual / default value does not match datatype.";

				const std::string kMsgNodeAssignmentNotSupported = "Node assignement \"%X\" is not supported on node \"%d\".";
				const std::string kMsgNodeAssignmentAlreadyExists = "Node assignement \"%X\" already exists on node \"%d\".";

				const std::string kMsgDynamicChannelNotFound = "Dynamic channel for datatype: \"%s\" and direction: \"%s\" not found on MN.";
				const std::string kMsgNonExistingMappedObject = "Mapped object 0x%X does not exist on node %d.";
				const std::string kMsgNonExistingMappedSubObject = "Mapped subobject 0x%X/0x%X does not exist on node %d.";
				const std::string kMsgAccessTypeForParameterInvalid = "Parameter with uniqueId \"%s\", referenced by mapped index 0x%X/0x%s on node %d, has invalid access type (\"%s\").";
				const std::string kMsgAccessTypeForParameterGroupInvalid = "ParameterGroup with uniqueId \"%s\" must not have multiple access types.";
				const std::string kMsgMappingTypeForPdoInvalid = "(Sub)Index 0x%X/0x%s on node %d cannot be mapped. Wrong PDOMapping attribute \"%s\" for mapping.";
				const std::string kMsgAccessTypeForPdoInvalid = "(Sub)Index 0x%X/0x%s on node %d cannot be mapped. Invalid access type \"%s\" (RPDOs must be writeable, TPDOs readable).";
				const std::string kMsgPdoOffsetInvalid = "Mapping-Object 0x%X/0x%X on node %d: Mapping offset invalid. Actual: %d bits, expected %d bits.";
				const std::string kMsgNoNodesConfigured = "Network does not contain any POWERLINK controlled nodes.";
				const std::string kMsgLowCnPresTimeout = "PResTimeout (%dns) (sub-object 0x1F92/0x%X on MN) of node %d is less than PresMaxLatency (subIndex 0x1F98/0x03 on CN).";
				const std::string kMsgLowCnPresTimeoutDefault = "PResTimeout (%dns) (sub-object 0x1F92/0x%X on MN) of node %d is less than specification default value (25000ns).";
				const std::string kMsgDefaultCnPresTimeout = "PResTimeout (sub-object 0x1F92/0x%X on MN) of node %d does not a have a default value. Error according to DS 301.";

				const std::string kMsgManagingNodeTransmitPres = "Managing node transmits PRes but no controlled node is receiving any data.";
				const std::string kMsgMappingOperationNotSupported = "Mapping operation is not supported for managing or redundant managing node.";

				const std::string kMsgNrOfEntriesInvalid = "Channel 0x%X on node %d: NrOfEntries invalid. Valid mapping entries: %d, Mapping entries enabled: %d.";
				const std::string kMsgDynamicChannelExceeded = "Object \"0x%X/0x%X\" exceeds object range (0x%X - 0x%X) of MN dynamic channel for datatype: \"%s\" and direction: \"%s\".";
				const std::string kMsgChannelExceeded = "No MN %s-Channel available for node \"%s\" (%d).";

				const std::string kMsgCycleTimeMin = "Cycle time \"%d\" is lower than the minimum cycle time \"%d\" of node \"%s\" (%d).";
				const std::string kMsgCycleTimeMax = "Cycle time \"%d\" is higher than the maximum cycle time \"%d\" of node \"%s\" (%d).";
				const std::string kMsgCycleTimeGran = "Cycle time \"%d\" does not fit to the cycle time granularity \"%d\" of node \"%s\" (%d).";
				const std::string kMsgIsochronousMaxPayloadExceeded = "Node %d: No. of %s-bytes mapped (%d) exceeds limit (1490, C_DLL_ISOCHR_MAX_PAYL).";
				const std::string kMsgChainingRmnNotSupported = "PRes Chaining is not supported in combination with Redundant Managing Nodes.";

				/** \addtogroup apireturn
				* @{
				*/
				/**
				\brief Represents the return class for the libary.
				\author rueckerc, Bernecker+Rainer Industrie Elektronik Ges.m.b.H.
				*/
				class DLLEXPORT Result : public IndustrialNetwork::Fieldbus::IResult<IndustrialNetwork::POWERLINK::Core::ErrorHandling::ErrorCode>
				{

					public:
						Result();
						explicit Result(ErrorCode errorCode);
						Result(ErrorCode errorCode, const std::string& errorMessage);

						virtual ~Result();
						/**
						\return bool
						*/
						bool IsSuccessful();
				};
				/** @} */
			}
		}
	}
}
#endif
