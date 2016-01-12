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
				const std::string kMsgEmptyArgument = "Argument(s) '%s' must not be empty.";
				const std::string kMsgLoggingInitialised = "Logging has been initialised with configuration: '%s'.";

				const std::string kMsgExistingNetwork = "Network with id '%s' already exists.";
				const std::string kMsgNonExistingNetwork = "Network with id '%s' does not exists.";
				const std::string kMsgNetworkCreated = "Network with id '%s' created.";
				const std::string kMsgNetworkRemoved = "Network with id '%s' removed.";

				const std::string kMsgExistingNode = "Node with id %d already exists.";
				const std::string kMsgNonExistingNode = "Node with id %d does not exist.";
				const std::string kMsgNonManagingNode = "Node with id %d is not a managing node or redundant managing node.";
				const std::string kMsgNonControlledNode = "Node with id %d is not a controlled node.";
				const std::string kMsgNonRedundantManagingNode = "Node with id %d is not a redundant managing node.";

				const std::string kMsgNodeCreated = "Node with id %d created.";
				const std::string kMsgNodeRemoved = "Node with id %d removed.";
				const std::string kMsgNodeObjectCount = "Node with id %d has %d configuration objects.";
				const std::string kMsgNodeObjectCountSize = "Node with id %d has %d configuration objects size.";
				const std::string kMsgManagingNodeDisable = "Managing node of network '%s' cannot be disabled.";
				const std::string kMsgNodeDisable = "Node with Id '%d' set enabled to '%s'.";
				const std::string kMsgMultiplexingNotSupported = "Multiplexing is not supported by controlled node '%d'";
				const std::string kMsgChainingNotSupported = "PRes Chaining is not supported by controlled node '%d'";
				const std::string kMsgMultiplexCycleAssignInvalid = "Multiplexed cycle '%d' assigned to node %d exceeds multiplexed cycle length (%d).";
				const std::string kMsgMultiplexCycleAlreadyAssigned = "Multiplexed cycle '%d' already assigned.";

				const std::string kMsgExistingObject = "Object 0x%X already exists on node %d.";
				const std::string kMsgExistingSubObject = "Subobject 0x%X/0x%X already exists on node %d.";
				const std::string kMsgObjectCreated = "Object 0x%X created on node %d.";
				const std::string kMsgSubObjectCreated = "Subobject 0x%X/0x%X created on node %d.";
				const std::string kMsgObjectNoActualValue = "Object 0x%X on node %d does not have an actual value.";
				const std::string kMsgSubObjectNoActualValue = "Subobject 0x%X/0x%X on node %d does not have an actual value.";
				const std::string kMsgNonExistingObject = "Object 0x%X does not exist on node %d.";
				const std::string kMsgNonExistingSubObject = "Subobject 0x%X/0x%X does not exist on node %d.";
				const std::string kMsgForceObject = "Set force flag on object 0x%X on node %d to '%s'.";
				const std::string kMsgForceSubObject = "Set force flag on subobject 0x%X/0x%X on node %d to '%s'.";
				const std::string kMsgSetObjectActualValue = "Set actual value: %s on object 0x%X on node %d.";
				const std::string kMsgSetSubObjectActualValue = "Set actual value: %s on subobject 0x%X/0x%X on node %d.";
				const std::string kMsgDatatypeMismatch = "Datatype '%s' does not match expected datatype '%s'.";
				const std::string kMsgObjectDatatypeMismatch = "BaseObject '%s' datatype '%s' does not match expected datatype '%s'.";
				const std::string kMsgActualValueFormatError = "Actual value '%s' has invalid format for datatype with datatype '%d'.";
				const std::string kMsgActualValueDatatypeError = "Actual value '%s' with datatype '%d' does exceed the datatype limits.";
				const std::string kMsgDefaultValueDatatypeError = "Default value '%s' with datatype '%d' does exceed the datatype limits.";
				const std::string kMsgHighLimitDatatypeError = "Highlimit '%s' with datatype '%d' does exceed the datatype limits.";
				const std::string kMsgLowLimitDatatypeError = "Lowlimit '%s' with datatype '%d' does exceed the datatype limits.";
				const std::string kMsgBaseObjectLowLimitError = "BaseObject '%s' with id '0x%X' on node '%d' deceeds lowlimit '%d'.";
				const std::string kMsgBaseObjectHighLimitSubObjectError = "Object '%s' '0x%X/0x%X' on node '%d' with actual value '%s' exceeds the highlimit.";
				const std::string kMsgBaseObjectLowLimitSubObjectError = "Object '%s' '0x%X/0x%X' on node '%d' with actual value '%s' deceeds the lowlimit.";
				const std::string kMsgBaseObjectHighLimitError = "BaseObject '%s' with id '0x%X' on node '%d' exceeds highlimit '%d'.";
				const std::string kMsgBaseObjectDataTypeError = "BaseObject '%s' with id '0x%X' on node '%d' does not have a specified datatype.";
				const std::string kMsgBaseObjectDefaultValue = "BaseObject '%s' with id '0x%X' on node '%d' does not have a default value.";
				const std::string kMsgBaseObjectActualValue = "BaseObject '%s' with id '0x%X' on node '%d' does not have an actual value.";
				const std::string kMsgBaseObjectValueSupport = "Object '0x%X' on node %d does not support actual values.";
				const std::string kMsgBaseSubObjectValueSupport = "Subobject '0x%X/0x%X' on node %d does not support actual values.";
				const std::string kMsgBaseObjectHighLimit = "BaseObject '%s' with id '0x%X' on node '%d' does not have a high limit.";
				const std::string kMsgBaseObjectLowLimit = "BaseObject '%s' with id '0x%X' on node '%d' does not have a low limit.";

				const std::string kMsgConfigurationSettingNotSupported = "Configuration setting '%s' is not supported by the library.";
				const std::string kMsgConfigurationNonExisting = "Build configuration '%s' does not exist in network '%s'.";
				const std::string kMsgConfigurationExisting = "Build configuration '%s' does already exist in network '%s'.";
				const std::string kMsgConfigurationAdded = "Build configuration '%s' added to network '%s'.";
				const std::string kMsgConfigurationRemoved = "Build configuration '%s' removed from network '%s'.";
				const std::string kMsgConfigurationActive = "Build configuration '%s' is active for network '%s'.";
				const std::string kMsgConfigurationRenamed = "Build configuration '%s' is renamed to '%s' in network '%s'.";
				const std::string kMsgConfigurationSettingNonExisting = "Build configuration setting '%s' does not exist in configuration '%s' in network '%s'.";
				const std::string kMsgConfigurationSettingExisting = "Build configuration setting '%s' already exist in configuration '%s' in network '%s'.";
				const std::string kMsgConfigurationSettingAdded = "Build configuration setting '%s' added to configuration '%s' in network '%s'.";
				const std::string kMsgConfigurationSettingRemoved = "Build configuration setting '%s' removed from configuration '%s' in network '%s'.";
				const std::string kMsgConfigurationSettingEnabled = "Build configuration setting '%s' in configuration '%s' in network '%s' set enabled: '%s'.";

				const std::string kMsgComplexDatatypeNotFoundOnNode = "Complex datatype with uniqueID '%s' in node %d not found.";
				const std::string kMsgComplexDatatypeNotFound = "Complex data type with uniqueID '%s' not found.";
				const std::string kMsgComplexDataTypeAlreadyExist = "Complex data type with uniqueID '%s' already exists.";
				const std::string kMsgComplexDataTypeSizeInvalid = "Complex data type with uniqueID '%s' size calculation failed.";
				const std::string kMsgComplexDataTypeCreated = "Complex data type with uniqueID '%s' created.";

				const std::string kMsgStructDatatypeNotFound = "Struct with uniqueID '%s' in node %d not found.";

				const std::string kMsgParameterCreated = "Parameter with uniqueID '%s' created.";
				const std::string kMsgParameterReturned = "Parameter with uniqueID '%s' returned.";
				const std::string kMsgParameterNotFound = "Parameter which references datatype with uniqueIDRef '%s' not found on node %d.";
				const std::string kMsgParameterAlreadyExist = "Parameter with uniqueID '%s' already exists.";

				const std::string kMsgNodeIdInvalid = "NodeId '%d' invalid.";
				const std::string kMsgSubIndexInvalid = "Subindex 0x%X/0x%X invalid for node %d.";
				const std::string kMsgIndexInvalid = "Index 0x%X invalid for node %d.";
				const std::string kMsgUnsupportedAttributeType = "Attributetype '%d' not supported for this operation.";
				const std::string kMsgCycleTimeOnMnNotSet = "Cycle time has to be set on the managing node of the POWERLINK network.";
				const std::string kMsgCycleTimeDefaultValue = "Cycle time forced to configuration because default and actual value are equal on the managing node.";

				const std::string kMsgObjectActualValueNotSet = "Object 0x%X on node %d does not have a defined actual value.";
				const std::string kMsgObjectDataTypeMismatch = "Object 0x%X datatype on node %d does not match the datatype to be retrieved.";
				const std::string kMsgSubObjectActualValueNotSet = "Object 0x%X on node %d does not have a defined actual value.";
				const std::string kMsgSubObjectDataTypeMismatch = "Object 0x%X datatype on node %d does not match the datatype to be retrieved.";

				const std::string kMsgForcedValueOverwriteObject = "Overwrite actual value: %s on object 0x%X on node %d failed because value has been forced.";
				const std::string kMsgForcedValueOverwriteSubObject = "Overwrite actual value: %s on subobject 0x%X/0x%X on node %d failed because value has been forced.";

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

				const std::string kMsgGeneralFeatureDatatypeError = "General feature '%s' actual / default value does not match datatype.";
				const std::string kMsgCnFeatureDatatypeError = "CN feature '%s' actual / default value does not match datatype.";
				const std::string kMsgMnFeatureDatatypeError = "MN feature '%s' actual / default value does not match datatype.";

				const std::string kMsgNodeAssignmentNotSupported = "Node assignement '%X' is not supported on node '%d'.";
				const std::string kMsgNodeAssignmentAlreadyExists = "Node assignement '%X' already exists on node '%d'.";

				const std::string kMsgDynamicChannelNotFound = "Dynamic channel for datatype: '%s' and direction: '%s' not found on MN.";
				const std::string kMsgNonExistingMappedObject = "Mapped object 0x%X does not exist on node %d.";
				const std::string kMsgNonExistingMappedSubObject = "Mapped subobject 0x%X/0x%X does not exist on node %d.";
				const std::string kMsgAccessTypeForParameterInvalid = "Parameter with uniqueId '%s', referenced by mapped index 0x%X/0x%s on node %d, has invalid access type ('%s').";
				const std::string kMsgMappingTypeForPdoInvalid = "(Sub)Index 0x%X/0x%s on node %d cannot be mapped. Wrong PDOMapping attribute '%s' for mapping.";
				const std::string kMsgAccessTypeForPdoInvalid = "(Sub)Index 0x%X/0x%s on node %d cannot be mapped. Invalid access type '%s' (RPDOs must be writeable, TPDOs readable).";
				const std::string kMsgPdoOffsetInvalid = "Mapping-Object 0x%X/0x%X on node %d: Mapping offset invalid. Actual: %d bits, expected %d bits.";
				const std::string kMsgNoNodesConfigured = "Network does not contain any POWERLINK controlled nodes.";
				const std::string kMsgLowCnPresTimeout = "PResTimeout (%dns) (subIndex 0x1F92/0x%X on MN) of node %d is less than PresMaxLatency (subIndex 0x1F98/0x03 on CN).";

				const std::string kMsgManagingNodeTransmitPres = "Managing node transmits PRes but no controlled node is receiving any data.";
				const std::string kMsgMappingOperationNotSupported = "Mapping operation is not supported for managing or redundant managing node.";

				const std::string kMsgNrOfEntriesInvalid = "Channel 0x%X on node %d: NrOfEntries invalid. Valid mapping entries: %d, Mapping entries enabled: %d.";

				//Unused
				/*
				const std::string kMsgNoControlledNodesConfigured = "Configuration does not contain any POWERLINK-CNs.";
				const std::string kMsgObjectDictoryEmpty = "Object dictionary of node %d is empty.";
				const std::string kMsgIndexContainsNoSubIndices = "Index 0x%X of node %d contains no subIndices.";
				const std::string kMsgNullArgument = "Argument(s) %s must not be NULL.";
				const std::string kMsgAttributeValueInvalid = "Value '%s' invalid for attribute type '%d' of index 0x%X/0x%X, node %d. Expected value: '%s'.";
				const std::string kMsgAttributeValueNotInRange = "Value '%d' invalid for attribute type '%d' of index 0x%X/0x%X, node %d. Expected value in range: [%d-%d].";
				const std::string kMsgTPDOChannelCountExceeded = "No. of TPDO-Channels on node %d exceeded. Allowed: %d, existing: %d.";
				const std::string kMsgInsufficientMappingObjects = "Insufficient number of mapping objects within index 0x%X on node %d. Expected: %d, existing %d";
				const std::string kMsgParameterIndexNotFound = "Parameter with uniqueID '%s' not found in node %d referenced by (Sub)Index 0x%X/0x%s.";
				const std::string kMsgSimpleDatatypeNotFound = "Simple datatype '%s' referenced by (Sub)Index 0x%X/0x%s in node %d not found in DatatypeList of node.";
				const std::string kMsgValueNotWithinRange = "Value %d of object '%s' on node %d out of range (%d - %d).";
				const std::string kMsgChannelPayloadLimitExceeded = "Node %d, channel 0x%X: No. of bytes mapped (%d) exceeds limit (1490, C_DLL_ISOCHR_MAX_PAYL)";
				const std::string kMsgChannelObjectLimitExceeded = "Node %d, channel 0x%X: No. of channel objects (%d) exceeds limit (254).";
				const std::string kMsgIsochronousMaxPayloadExceeded = "Node %d: No. of %s-bytes mapped (%d) exceeds limit (1490, C_DLL_ISOCHR_MAX_PAYL).";
				const std::string kMsgPdoDatatypeInvalid = "Node %d, (Sub)Index 0x%X/0x%s: Mapping of datatype '%s' not supported. Supported data types: Integer8(16/32/64), Unsigned8(16/32/64).";
				const std::string kMsgMaxPiSizeExceeded = "Max. process image size exceeded: Actual: %d bytes, expected: max. %d bytes.";
				const std::string kMsgObjectLimitsInvalid = "Invalid object limits (high limit '%d' < low limit '%d')";
				const std::string kMsgNodeIndexDescription = "Node %d, Index 0x%X: ";
				const std::string kMsgNodeSubIndexDescription = "Node %d, (Sub)Index 0x%X/0x%X: ";
				const std::string kMsgCrossTrafficStationLimitExceeded = "Node %d: No. of cross traffic stations (%d) exceeds limit (%d).";
				const std::string kMsgPDOTPDOChannelCountExceeded = "Node 240: No. of TPDO-Channels exceeded. Allowed: %d (MN-Feature D_PDO_TPDOChannels_U16), existing: %d.";
				const std::string kMsgNoActualOrDefaultValue = "Object 0x%X/0x%X on node %d has neither a default value nor an actual value.";
				const std::string kMsgPowerlinkDatatypeSizeNotDefined = "POWERLINK-Datatype '0x%X' has no defined size.";
				const std::string kMsgFeatureValueNotFound = "%s-Feature '%s' not defined in XDD of node %d.";
				const std::string kMsgObjectSizeMappedInvalid = "Mapping-Object 0x%X/0x%X on node %d: Mapped data-length invalid. Actual: %d bits, expected %d bits.";
				const std::string kMsgParameterValueInvalid = "Node %d: For parameter '%d' the given value '%s' is invalid.";
				const std::string kMsgParameterValueNotSet = "Node %d: Parameter '%d' value is not set.";
				const std::string kMsgMappingInvalid = "Mapping-Object 0x%X/0x%X on node %d: Mapping is enabled (NumberOfEntries != 0) but mapping entry is zero. Expected valid mapping entry.";*/

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
