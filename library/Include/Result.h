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
				const std::string kMsgNodeCreated = "Node with id %d created.";
				const std::string kMsgNodeRemoved = "Node with id %d removed.";
				const std::string kMsgExistingObject = "Object 0x%X already exists on node %d.";
				const std::string kMsgExistingSubObject = "Subobject 0x%X/0x%X already exists on node %d.";
				const std::string kMsgObjectCreated = "Object 0x%X created on node %d.";
				const std::string kMsgSubObjectCreated = "Subobject 0x%X/0x%X  created on node %d.";
				const std::string kMsgNonExistingObject = "Object 0x%X does not exist on node %d.";
				const std::string kMsgNonExistingSubObject = "Subobject 0x%X/0x%X does not exist on node %d.";
				const std::string kMsgForceObject = "Set force flage on object 0x%X on node %d to '%s'.";
				const std::string kMsgForceSubObject = "Set force flage on subobject 0x%X/0x%X on node %d to '%s'.";
				const std::string kMsgSetObjectActualValue = "Set actual value: %s on object 0x%X on node %d.";
				const std::string kMsgSetSubObjectActualValue = "Set actual value: %s on subobject 0x%X/0x%X on node %d.";
				const std::string kMsgDatatypeMismatch = "Datatype '%s' does not match expected datatype '%s'.";
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
				const std::string kMsgComplexDatatypeNotFound = "Complex datatype with uniqueID '%s' in node %d not found.";
				const std::string kMsgStructDatatypeNotFound = "Struct with uniqueID '%s' in node %d not found.";
				const std::string kMsgParameterNotFound = "Parameter with uniqueID '%s' not found in node %d.";

				const std::string kMsgNodeIdInvalid = "NodeId '%d' invalid.";
				const std::string kMsgSubIndexInvalid = "Subindex 0x%X/0x%X invalid for node %d.";
				const std::string kMsgIndexInvalid = "Index 0x%X invalid for node %d.";
				const std::string kMsgUnsupportedAttributeType = "Attributetype '%d' not supported for this operation.";

				const std::string kMsgNoNodesConfigured = "Configuration does not contain any POWERLINK-Nodes.";
				const std::string kMsgNoControlledNodesConfigured = "Configuration does not contain any POWERLINK-CNs.";
				const std::string kMsgNoManagingNodeConfigured = "Configuration does not contain any POWERLINK-MN.";
				const std::string kMsgObjectDictoryEmpty = "Object dictionary of node %d is empty.";
				const std::string kMsgIndexContainsNoSubIndices = "Index 0x%X of node %d contains no subIndices.";
				const std::string kMsgNullArgument = "Argument(s) %s must not be NULL.";
				const std::string kMsgAttributeValueInvalid = "Value '%s' invalid for attribute type '%d' of index 0x%X/0x%X, node %d. Expected value: '%s'.";
				const std::string kMsgAttributeValueNotInRange = "Value '%d' invalid for attribute type '%d' of index 0x%X/0x%X, node %d. Expected value in range: [%d-%d].";
				const std::string kMsgExternalSystemCallFailed = "External system call '%s' failed with returncode %d";
				const std::string kMsgTPDOChannelCountExceeded = "No. of TPDO-Channels on node %d exceeded. Allowed: %d, existing: %d.";
				const std::string kMsgNonExistingMappedIndex = "Mapping-Object 0x%X/0x%X: Mapped index 0x%X does not exist on node %d.";
				const std::string kMsgNonExistingMappedSubIndex = "Mapping-Object 0x%X/0x%X: Mapped subIndex 0x%X/0x%X does not exist on node %d.";
				const std::string kMsgInsufficientMappingObjects = "Insufficient number of mapping objects within index 0x%X on node %d. Expected: %d, existing %d";
				const std::string kMsgParameterIndexNotFound = "Parameter with uniqueID '%s' not found in node %d referenced by (Sub)Index 0x%X/0x%s.";				
				const std::string kMsgSimpleDatatypeNotFound = "Simple datatype '%s' referenced by (Sub)Index 0x%X/0x%s in node %d not found in DatatypeList of node.";
				const std::string kMsgValueNotWithinRange = "Value %d of object '%s' on node %d out of range (%d - %d).";
				const std::string kMsgChannelPayloadLimitExceeded = "Node %d, channel 0x%X: No. of bytes mapped (%d) exceeds limit (1490, C_DLL_ISOCHR_MAX_PAYL)";
				const std::string kMsgChannelObjectLimitExceeded = "Node %d, channel 0x%X: No. of channel objects (%d) exceeds limit (254).";
				const std::string kMsgIsochronousMaxPayloadExceeded = "Node %d: No. of %s-bytes mapped (%d) exceeds limit (1490, C_DLL_ISOCHR_MAX_PAYL).";
				const std::string kMsgPdoDatatypeInvalid = "Node %d, (Sub)Index 0x%X/0x%s: Mapping of datatype '%s' not supported. Supported data types: Integer8(16/32/64), Unsigned8(16/32/64).";
				const std::string kMsgMaxPiSizeExceeded = "Max. process image size exceeded: Actual: %d bytes, expected: max. %d bytes.";
				const std::string kMsgMultiplexingNotSupported = "Multiplexing is not supported by the managing node.";
				const std::string kMsgUnsupportedPiLanguage = "Process-Image generation for language %d not supported.";
				const std::string kMsgMultiplexCycleAssignInvalid = "Multiplexed cycle '%d' assigned to node %d exceeds multiplexed cycle length (%d).";
				const std::string kMsgObjectLimitsInvalid = "Invalid object limits (high limit '%d' < low limit '%d')";
				const std::string kMsgNodeIndexDescription = "Node %d, Index 0x%X: ";
				const std::string kMsgNodeSubIndexDescription = "Node %d, (Sub)Index 0x%X/0x%X: ";
				const std::string kMsgLowCnPresTimeout = "PResTimeout (%dns) (subIndex 0x1F92/0x%X on MN) of node %d is less than PresMaxLatency (subIndex 0x1F98/0x03 on CN).";
				const std::string kMsgCrossTrafficStationLimitExceeded = "Node %d: No. of cross traffic stations (%d) exceeds limit (%d).";
				const std::string kMsgPDOTPDOChannelCountExceeded = "Node 240: No. of TPDO-Channels exceeded. Allowed: %d (MN-Feature D_PDO_TPDOChannels_U16), existing: %d.";
				const std::string kMsgNoActualOrDefaultValue = "Object 0x%X/0x%X on node %d has neither a default value nor an actual value.";
				const std::string kMsgPowerlinkDatatypeSizeNotDefined = "POWERLINK-Datatype '0x%X' has no defined size.";
				const std::string kMsgFeatureValueNotFound = "%s-Feature '%s' not defined in XDD of node %d.";
				const std::string kMsgObjectSizeMappedInvalid = "Mapping-Object 0x%X/0x%X on node %d: Mapped data-length invalid. Actual: %d bits, expected %d bits.";
				const std::string kMsgMappingTypeForPdoInvalid = "Mapping-Object 0x%X/0x%X: (Sub)Index 0x%X/0x%s on node %d cannot be mapped. Mismatching PDOMapping ('%s' Mapping-Object, mapped object '%s').";
				const std::string kMsgAccessTypeForParameterInvalid = "Mapping-Object 0x%X/0x%X: Parameter with uniqueId '%s', referenced by mapped index 0x%X/0x%s on node %d, has invalid access type ('%s').";
				const std::string kMsgAccessTypeForPdoInvalid = "Mapping-Object 0x%X/0x%X: (Sub)Index 0x%X/0x%s on node %d cannot be mapped. Invalid access type '%s' (RPDOs must be writeable, TPDOs readable).";
				const std::string kMsgPdoOffsetInvalid = "Mapping-Object 0x%X/0x%X on node %d: Mapping offset invalid. Actual: %d bits, expected %d bits.";

				const std::string kMsgParameterValueInvalid = "Node %d: For parameter '%d' the given value '%s' is invalid.";
				const std::string kMsgParameterValueNotSet = "Node %d: Parameter '%d' value is not set.";
				const std::string kMsgMappingInvalid = "Mapping-Object 0x%X/0x%X on node %d: Mapping is enabled (NumberOfEntries != 0) but mapping entry is zero. Expected valid mapping entry.";

				/**
				\class Result
				\brief
				\author rueckerc
				*/
				class DLLEXPORT Result : public IndustrialNetwork::Fieldbus::IResult<IndustrialNetwork::POWERLINK::Core::ErrorHandling::ErrorCode>
				{

					public:
						Result();
						Result(ErrorCode errorCode, std::string errorMessage);
						Result(ErrorCode errorCode);

						virtual ~Result();
						/**
						\return bool
						*/
						bool IsSuccessful();

				};

			}

		}

	}

}
#endif
