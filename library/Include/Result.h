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

#include "Constants.h"
#include "ErrorCode.h"
#include "IResult.h"

namespace IndustrialNetwork
{
	namespace POWERLINK
	{
		namespace Core
		{
			namespace ErrorHandling
			{
				const std::string kMsgLoggingInitialised[] = {"Logging has been initialised with configuration: \"%s\".", "Das Logging wurde mit folgender Konfiguration initialisiert: \"%s\"."};
				const std::string kMsgDateTimeNotInitialised[] = {"Date / Time variable cannot be initialised.", "Datum / Urzeit Variable ist nicht initialisiert."};

				const std::string kMsgExistingNetwork[] = {"Network with ID \"%s\" already exists.", "Netzwerk mit der ID \"%s\" existiert bereits."};
				const std::string kMsgNonExistingNetwork[] = {"Network with ID \"%s\" does not exists.", "Netzwerk mit der ID \"%s\" existiert nicht."};
				const std::string kMsgNetworkCreated[] = {"Network with ID \"%s\" created.", "Netzwerk mit der ID \"%s\" wurde erstellt."};
				const std::string kMsgNetworkRemoved[] = {"Network with ID \"%s\" removed.", "Netzwerk mit der ID \"%s\" wurde geloescht."};
				const std::string kMsgSupportedSettingIds[] = {"Returned supported configuration setting ids.", "Unterstuetzte Konfigurationsoptionen wurden retourniert."};
				const std::string kMsgNetworkListCleared[] = {"Network list cleared.", "Die Netzwerkliste wurde geloescht."};

				const std::string kMsgExistingNode[] = {"Node with ID \"%d\" already exists.", "Knoten mit der ID \"%d\" existiert bereits."};
				const std::string kMsgNonExistingNode[] = {"Node with ID \"%d\" does not exist.", "Knoten mit der ID \"%d\" existiert nicht."};
				const std::string kMsgNonManagingNode[] = {"Node with ID \"%d\" is not a managing node or redundant managing node.", "Knoten mit der ID \"%d\" ist kein Managing Node order redundanter Managing Node."};
				const std::string kMsgNoManagingNode[] = {"Network has no managing node configured.", "Das Netzwerk hat keinen konfigurierten Managing Node."};
				const std::string kMsgRemoveManagingNode[] = {"Removal of managing node from network is not allowed.", "Der Managing Node darf nicht aus dem Netzwerk geloescht werden."};
				const std::string kMsgNonControlledNode[] = {"Node with ID \"%d\" is not a controlled node.", "Knoten mit der ID \"%d\" ist kein Controlled Node."};
				const std::string kMsgNonRedundantManagingNode[] = {"Node with ID \"%d\" is not a redundant managing node.", "Knoten mit der ID \"%d\" ist kein redundanter Managing Node."};

				const std::string kMsgNodeCreated[] = {"Node with ID \"%d\" created.", "Knoten mit der ID \"%d\" wurde erstellt."};
				const std::string kMsgNodeRemoved[] = {"Node with ID \"%d\" removed.", "Knoten mit der ID \"%d\" wurde geloescht."};
				const std::string kMsgNodeObjectCount[] = {"Node with ID \"%d\" has \"%d\" configuration objects.", "Knoten mit der ID \"%d\" hat \"%d\" Konfigurationsobjekte."};
				const std::string kMsgNodeObjectCountSize[] = {"Node with ID \"%d\" has \"%d\" configuration objects size.", "Knoten mit der ID \"%d\" hat folgende Konfigurationsobjektgroesse \"%d\"."};
				const std::string kMsgManagingNodeDisable[] = {"Managing node of network \"%s\" cannot be disabled.", "Der Managing Node des Netzwerks \"%s\" kann nicht deaktiviert werden."};
				const std::string kMsgNodeDisable[] = {"Node with ID \"%d\" set enabled to \"%s\".", "Bei Knoten mit der ID \"%d\" wurde folgender Status gesetzt \"%s\"."};
				const std::string kMsgMultiplexingNotSupported[] = {"Multiplexing is not supported by controlled node \"%d\".", "Multiplexing wird von Controlled Node \"%d\" nicht unterstuetzt."};
				const std::string kMsgChainingNotSupported[] = {"PRes Chaining is not supported by controlled node \"%d\".", "PRes Chaining wird von Controlled Node \"%d\" nicht unterstuetzt."};
				const std::string kMsgMultiplexCycleAssignInvalid[] = {"Multiplexed cycle \"%d\" assigned to node \"%d\" exceeds multiplexed cycle length (%d).", "Gemultiplexter Zyklus \"%d\" zugewiesen an Knoten \"%d\" ueberschreitet die gemultiplexte Zykluslaenge (%d)."};
				const std::string kMsgMultiplexCycleAlreadyAssigned[] = {"Multiplexed cycle \"%d\" already assigned.", "Gemultiplexter Zyklus \"%d\" wurde bereits zugewiesen."};
				const std::string kMsgInsufficientMappingObjects[] = {"Node with ID \"%d\" has \"%d\" available mapping objects but \"%d\" are requested.", "Knoten mit der ID \"%d\" hat \"%d\" verfuegbare Mappingobjekte aber es werden \"%d\" benoetigt."};

				const std::string kMsgExistingObject[] = {"Object 0x%X already exists on node \"%d\".", "Objekt 0x%X existiert bereits auf dem Knoten \"%d\"."};
				const std::string kMsgExistingSubObject[] = {"Subobject 0x%X/0x%X already exists on node \"%d\".", "Subobjekt 0x%X/0x%X existiert bereits auf dem Knoten \"%d\"."};
				const std::string kMsgObjectCreated[] = {"Object 0x%X created on node \"%d\".", "Objekt 0x%X wurde auf dem Knoten \"%d\" erstellt."};
				const std::string kMsgSubObjectCreated[] = {"Subobject 0x%X/0x%X created on node \"%d\".", "Subobjekt 0x%X/0x%X wurde auf dem Knoten \"%d\" erstellt."};
				const std::string kMsgObjectNoActualValue[] = {"Object 0x%X on node \"%d\" does not have an actual value.", "Objekt 0x%X auf dem Knoten \"%d\" hat keinen konfigurierten Wert."};
				const std::string kMsgSubObjectNoActualValue[] = {"Subobject 0x%X/0x%X on node \"%d\" does not have an actual value.", "Subobjekt 0x%X/0x%X auf dem Knoten \"%d\" hat keinen konfigurierten Wert."};
				const std::string kMsgNonExistingObject[] = {"Object 0x%X does not exist on node \"%d\".", "Objekt 0x%X existiert nicht auf dem Knoten \"%d\"."};
				const std::string kMsgNonExistingSubObject[] = {"Subobject 0x%X/0x%X does not exist on node \"%d\".", "Subobjekt 0x%X/0x%X existiert nicht auf dem Knoten \"%d\"."};
				const std::string kMsgForceObject[] = {"Set force flag on object 0x%X on node \"%d\" to \"%s\".", "Das Force Flag des Objekts 0x%X auf dem Knoten \"%d\" wurde auf \"%s\" gesetzt."};
				const std::string kMsgForceSubObject[] = {"Set force flag on subobject 0x%X/0x%X on node \"%d\" to \"%s\".", "Das Force Flag des Subobjekts 0x%X/0x%X auf dem Knoten \"%d\" wurde auf \"%s\". gesetzt."};
				const std::string kMsgSetObjectActualValue[] = {"Set actual value: \"%s\" on object 0x%X on node \"%d\".", "Es wurde folgender Wert: \"%s\" fuer das Objekt 0x%X auf dem Knoten \"%d\" konfiguriert." };
				const std::string kMsgSetSubObjectActualValue[] = {"Set actual value: \"%s\" on subobject 0x%X/0x%X on node \"%d\".", "Es wurde folgender Wert: \"%s\" fuer das SubObjekt 0x%X/0x%X auf dem Knoten \"%d\". konfiguriert."};
				const std::string kMsgDatatypeMismatch[] = {"Datatype \"%s\" does not match expected datatype \"%s\".", "Der Datentyp \"%s\" stimmt nicht mit dem erwarteten Datentyp \"%s\" ueberein."};
				const std::string kMsgBaseObjectDataTypeMismatch[] = {"Object \"%s\" datatype \"%s\" does not match expected datatype \"%s\".", "Objekt \"%s\" mit Datentyp \"%s\" stimmt nicht mit dem erwarteten Datentyp \"%s\" ueberein."};
				const std::string kMsgActualValueDatatypeError[] = {"Actual value \"%s\" with datatype \"%s\" does not fit the datatype limits or format.", "Der konfigurierte Wert \"%s\" mit Datentyp \"%s\" passt nicht in die Datentypgrenzen oder hat ein falsches Format."};
				const std::string kMsgDefaultValueDatatypeError[] = {"Default value \"%s\" with datatype \"%s\" does not fit the datatype limits or format.", "Der default Wert \"%s\" mit Datentyp \"%s\" passt nicht in die Datentypgrenzen oder hat ein falsches Format."};
				const std::string kMsgHighLimitDatatypeError[] = {"Highlimit \"%s\" exceeds the datatype \"%s\" highlimit.", "Das obere Limit \"%s\" uebersteigt das Datentyplimit \"%s\"."};
				const std::string kMsgLowLimitDatatypeError[] = {"Lowlimit \"%s\" deceeds the datatype \"%s\" lowlimit.", "Das untere Limit \"%s\" unterschreitet das Datentyplimit \"%s\"."};
				const std::string kMsgBaseObjectLowLimitError[] = {"Object \"%s\" with ID \"0x%X\" on node \"%d\" actual value \"%s\" deceeds lowlimit \"%d\".", "Das Objekt \"%s\" mit der ID \"0x%X\" auf dem Knoten \"%d\" mit dem konfigurierten Wert \"%s\" unterschreitet das unter Limit \"%d\"."};
				const std::string kMsgBaseObjectHighLimitSubObjectError[] = {"SubObject \"%s\" \"0x%X/0x%X\" on node \"%d\" with actual value \"%s\" exceeds the highlimit.", "Das Subobjekt \"%s\" \"0x%X/0x%X\" auf dem Knoten \"%d\" mit dem konfigurierten Wert \"%s\" ueberschreitet das obere Limit."};
				const std::string kMsgBaseObjectLowLimitSubObjectError[] = {"SubObject \"%s\" \"0x%X/0x%X\" on node \"%d\" with actual value \"%s\" deceeds the lowlimit.", "Das Subobjekt \"%s\" \"0x%X/0x%X\" auf dem Knoten \"%d\" mit dem konfigurierten Wert \"%s\" unterschreitet das unter Limit."};
				const std::string kMsgBaseObjectHighLimitError[] = {"Object \"%s\" with ID \"0x%X\" on node \"%d\" actual value \"%s\" exceeds highlimit \"%d\".", "Das Objekt \"%s\" mit der ID \"0x%X\" auf dem Knoten \"%d\" mit dem konfigurierten Wert \"%s\" ueberschreitet das obere Limit \"%d\"."};
				const std::string kMsgBaseObjectDataTypeError[] = {"Object \"%s\" with ID \"0x%X\" on node \"%d\" does not have a specified datatype.", "Das Objekt \"%s\" mit der ID \"0x%X\" auf dem Knoten \"%d\" hat keinen festgelegten Datentyp."};
				const std::string kMsgBaseObjectDefaultValue[] = {"Object \"%s\" with ID \"0x%X\" on node \"%d\" does not have a default value.", "Das Objekt \"%s\" mit der ID \"0x%X\" auf dem Knoten \"%d\" hat keinen festgelegten default Wert."};
				const std::string kMsgBaseObjectActualValue[] = {"Object \"%s\" with ID \"0x%X\" on node \"%d\" does not have an actual value.", "Das Objekt \"%s\" mit der ID \"0x%X\" auf dem Knoten \"%d\" hat keinen konfigurierten Wert."};
				const std::string kMsgBaseObjectValueSupport[] = {"Object \"0x%X\" on node \"%d\" does not support actual values.", "Das Object \"0x%X\" auf dem Knoten \"%d\" kann nicht konfiguriert werden."};
				const std::string kMsgBaseSubObjectValueSupport[] = {"Subobject \"0x%X/0x%X\" on node \"%d\" does not support actual values.", "Das Subobjekt \"0x%X/0x%X\" auf dem Knoten \"%d\" kann nicht konfiguriert werden."};
				const std::string kMsgBaseObjectHighLimit[] = {"Object \"%s\" with ID \"0x%X\" on node \"%d\" does not have a high limit.", "Das Object \"0x%X\" auf dem Knoten \"%d\" hat kein oberes Limit."};
				const std::string kMsgBaseObjectLowLimit[] = {"Object \"%s\" with ID \"0x%X\" on node \"%d\" does not have a low limit.", "Das Object \"0x%X\" auf dem Knoten \"%d\" hat kein unteres Limit." };
				const std::string kMsgBaseObjectDoesNotSupportLimits[] = {"Datatype \"%s\" does not support the definition of high or low limit.", "Datentyp \"%s\" unterstuetzt die Konfiguration von einem oberen und unteren Limit nicht."};
				const std::string kMsgBaseObjectDoesNotSupportValues[] = {"Datatype \"%s\" does not support the definition of an actual or default value.", "Datentyp \"%s\" unterstuetzt die Konfiguration von Werten nicht."};

				const std::string kMsgConfigurationSettingNotSupported[] = {"Configuration setting \"%s\" is not supported by the library.", "Konfigurationsoption \"%s\" wird von der Bibliothek nicht unterstuetzt."};
				const std::string kMsgConfigurationNonExisting[] = {"Build configuration \"%s\" does not exist in network \"%s\".", "Ausgewaehlte Erstellungskonfiguration \"%s\" existiert nicht fuer das Netzwerk \"%s\"."};
				const std::string kMsgConfigurationExisting[] = {"Build configuration \"%s\" does already exist in network \"%s\".", "Ausgewaehlte Erstellungskonfiguration \"%s\" existiert bereits fuer das Netzwerk \"%s\"."};
				const std::string kMsgConfigurationAdded[] = {"Build configuration \"%s\" added to network \"%s\".", "Erstellungskonfiguration \"%s\" wurde zu dem Netzwerk \"%s\" hinzugefuegt."};
				const std::string kMsgConfigurationRemoved[] = {"Build configuration \"%s\" removed from network \"%s\".", "Erstellungskonfiguration \"%s\" wurde aus dem Netzwerk \"%s\" entfernt."};
				const std::string kMsgActiveConfigurationRemove[] = {"Active build configuration \"%s\" cannot be removed from network \"%s\".", "Aktive Erstellungskonfiguration \"%s\" kann nicht aus dem Netzwerk \"%s\" entfernt werden."};
				const std::string kMsgConfigurationActive[] = {"Build configuration \"%s\" is active for network \"%s\".",  "Erstellungskonfiguration \"%s\" ist im Netzwerk \"%s\" aktiv."};
				const std::string kMsgConfigurationRenamed[] = {"Build configuration \"%s\" is renamed to \"%s\" in network \"%s\".",  "Erstellungskonfiguration \"%s\" wurde in \"%s\" fuer das Netzwerk \"%s\" umbenannt."};
				const std::string kMsgConfigurationSettingNonExisting[] = {"Build configuration setting \"%s\" does not exist in configuration \"%s\" in network \"%s\".", "Konfigurationsoption \"%s\" existiert nicht in der Erstellungskonfiguration \"%s\" fuer das Netzwerk\"%s\"."};
				const std::string kMsgConfigurationSettingExisting[] = {"Build configuration setting \"%s\" already exist in configuration \"%s\" in network \"%s\".", "Konfigurationsoption \"%s\" existiert bereits in der Erstellungskonfiguration \"%s\" fuer das Netzwerk\"%s\"." };
				const std::string kMsgConfigurationSettingAdded[] = {"Build configuration setting \"%s\" added to configuration \"%s\" in network \"%s\".", "Konfigurationsoption \"%s\" wurde zur Erstellungskonfiguration \"%s\" fuer das Netzwerk\"%s\" hinzugefuegt."};
				const std::string kMsgConfigurationSettingRemoved[] = {"Build configuration setting \"%s\" removed from configuration \"%s\" in network \"%s\".", "Konfigurationsoption \"%s\" wurde von der Erstellungskonfiguration \"%s\" fuer das Netzwerk\"%s\" entfernt."};
				const std::string kMsgConfigurationSettingEnabled[] = {"Build configuration setting \"%s\" in configuration \"%s\" in network \"%s\" set enabled: \"%s\".", "Fuer die Konfigurationsoption \"%s\" in der Erstellungskonfiguration \"%s\" fuer das Netzwerk\"%s\" wurde der Status auf \"%s\" gesetzt."};

				const std::string kMsgComplexDatatypeNotFoundOnNode[] = {"Complex datatype with uniqueID \"%s\" in node \"%d\" not found.", "Der komplexe Datentyp mit der uniqueID \"%s\" wurde auf dem Knote \"%d\" nicht gefunden."};
				const std::string kMsgComplexDatatypeNotFound[] = {"Complex datatype with uniqueID \"%s\" not found.", "Der komplexe Datentyp mit der uniqueID \"%s\" wurde nicht gefunden."};
				const std::string kMsgComplexDataTypeAlreadyExist[] = {"Complex datatype with uniqueID \"%s\" already exists.", "Der komplexe Datentyp mit der uniqueID \"%s\" existiert bereits."};
				const std::string kMsgComplexDataTypeSizeInvalid[] = {"Complex datatype with uniqueID \"%s\" size calculation failed.", "Die Laenge des komplexen Datentyps mit der uniqueID \"%s\" konnte nicht berechnet werden."};
				const std::string kMsgComplexDataTypeCreated[] = {"Complex datatype with uniqueID \"%s\" created.", "Der komplexe Datentyp mit der uniqueID \"%s\" wurde erstellt."};

				const std::string kMsgStructDatatypeNotFound[] = {"Struct with uniqueID \"%s\" in node \"%d\" not found.", "Der Struct Datentyp mit der uniqueID \"%s\" wurde auf dem Knoten \"%d\" nicht gefunden."};
				const std::string kMsgEnumValueExists[] = {"Enum datatype with uniqueID \"%s\" value \"%s\" already exists.", "Der Enum Datentyp mit der uniqueID  \"%s\" mit Wert \"%s\" existiert bereits."};
				const std::string kMsgVarDeclarationExists[] = {"Var declaration \"%s\" in struct datatype with uniqueID \"%s\" already exists.", "Die VarDeclaration \"%s\" im Struct Datentyp mit der uniqueID \"%s\" existiert bereits."};

				const std::string kMsgParameterCreated[] = {"Parameter with uniqueID \"%s\" created.", "Parameter mit der uniqueID \"%s\" wurde erstellt."};
				const std::string kMsgParameterReturned[] = {"Parameter with uniqueID \"%s\" returned.", "Parameter mit der uniqueID \"%s\" wurde retourniert."};
				const std::string kMsgParameterNotFound[] = {"Parameter with uniqueIDRef \"%s\" not found.", "Parameter mit der uniqueID \"%s\" wurde nicht gefunden."};
				const std::string kMsgParameterAlreadyExist[] = {"Parameter with uniqueID \"%s\" already exists.", "Parameter mit der uniqueID \"%s\" existiert bereits."};
				const std::string kMsgParameterActualValueDoesNotExist[] = {"Parameter with uniqueID \"%s\" does not have an actual value.", "Parameter mit der uniqueID \"%s\" hat keinen konfigurierten Wert."};
				const std::string kMsgParameterDefaultValueDoesNotExist[] = { "Parameter with uniqueID \"%s\" does not have a default value.", "Parameter mit der uniqueID \"%s\" hat keinen default Wert." };
				const std::string kMsgParameterGroupAlreadyExist[] = {"ParameterGroup with uniqueID \"%s\" already exists.", "ParameterGroup mit der uniqueID \"%s\" existiert bereits."};
				const std::string kMsgParameterTemplateAlreadyExist[] = {"ParameterTemplate with uniqueID \"%s\" already exists.", "ParameterTemplate mit der uniqueID \"%s\" existiert bereits."};
				const std::string kMsgParameterGroupNonExisting[] = {"ParameterGroup with uniqueID \"%s\" does not exist.", "ParameterTemplate mit der uniqueID \"%s\" existiert nicht."};
				const std::string kMsgParameterGroupEmpty[] = {"ParameterGroup with uniqueID \"%s\" has no parameter references or included parameter groups.", "ParameterGroup mit der uniqueID \"%s\" hat keine Parameterreferenzen oder inkludierte Parametergruppen."};
				const std::string kMsgParameterTemplateNonExisting[] = {"ParameterTemplate with uniqueID \"%s\" does not exist.", "ParameterTemplate mit der uniqueID \"%s\" existiert nicht."};

				const std::string kMsgNodeIdInvalid[] = {"NodeId \"%d\" invalid.", "Knotennummer \"%d\" ist ungueltig."};
				const std::string kMsgSubIndexInvalid[] = {"Subindex 0x%X/0x%X invalid for node \"%d\".", "Subindex 0x%X/0x%X ist auf dem Knoten \"%d\" ungueltig."};
				const std::string kMsgIndexInvalid[] = {"Index 0x%X invalid for node \"%d\".", "Index 0x%X ist auf dem Knoten \"%d\" ungueltig."};
				const std::string kMsgUnsupportedAttributeType[] = {"Attribute type \"%d\" not supported for this operation.", "Attributetyp \"%d\" unterstuetzt folgende Operation nicht."};
				const std::string kMsgCycleTimeOnMnNotSet[] = {"Cycle time has to be set on the managing node of the POWERLINK network.", "Die Zykluszeit muss auf dem Managing Node des POWERLINK Netzwerks konfiguriert sein."};
				const std::string kMsgCycleTimeDefaultValue[] = {"Cycle time forced to configuration because default and actual value are equal on the managing node.", "Die Zykluszeit wurde in die Konfiguration geschrieben obwohl sich der default und der konfigurierte Wert auf dem Managing Node nicht unterscheiden."};

				const std::string kMsgForcedValueOverwriteObject[] = {"Overwrite actual value: \"%s\" on object 0x%X on node \"%d\" failed because value has been forced.", "Der Wert: \"%s\" kann nicht auf das Objekt 0x%X auf dem Knoten \"%d\" geschrieben werden weil der Wert erzwungen wurde."};
				const std::string kMsgForcedValueOverwriteSubObject[] = {"Overwrite actual value: \"%s\" on subobject 0x%X/0x%X on node \"%d\" failed because value has been forced.", "Der Wert: \"%s\" kann nicht auf das Subobjekt 0x%X/0x%X  auf dem Knoten \"%d\" geschrieben werden weil der Wert erzwungen wurde."};

				const std::string kMsgWriteManagingNodeObjectCount[] = {"Managing node object count configuration write successful.", "Managing Node Objektanzahl wurde erfolgreich in die Konfiguration geschrieben."};
				const std::string kMsgWriteNodeAssignment[] = {"Node assignment configuration write successful.", "Node Assignment wurde erfolgreich in die Konfiguration geschrieben."};
				const std::string kMsgWriteRedundantManagingNode[] = {"Redundant Managing Node (%d) configuration write successful.", "Redundant Managing Node(%d) Werte wurden erfolgreich in die Konfiguration geschrieben."};
				const std::string kMsgWriteManagingNode[] = {"Managing Node (%d) configuration write successful.", "Managing Node(%d) Werte wurden erfolgreich in die Konfiguration geschrieben."};
				const std::string kMsgWriteControlledNode[] = {"Controlled Node (%d) configuration write successful.", "Controlled Node(%d) Werte wurden erfolgreich in die Konfiguration geschrieben."};
				const std::string kMsgWriteMappingObjects[] = {"Mapping objects for Node (%d) write successful.", "Mappingobjekte fuer den Knoten (%d) wurden erfolgreich in die Konfiguration geschrieben."};
				const std::string kMsgWriteMappingObjectsNrOfEntries[] = {"Mapping objects NrOfEntries for Node (%d) write successful.", "Mappingobjekte NrOfEntries fuer den Knoten (%d) wurden erfolgreich in die Konfiguration geschrieben."};
				const std::string kMsgWriteMappingObjectsNrOfEntriesZero[] = {"Mapping objects NrOfEntries zero for Node (%d) write successful.", "Mappingobjekte NrOfEntries Null fuer den Knoten (%d) wurden erfolgreich in die Konfiguration geschrieben."};
				const std::string kMsgWriteCommunicationRangeObjects[] = {"Communication range objects for Node (%d) write successful.", "Kommunikationsobjektbereich fuer den Knoten (%d) wurden erfolgreich in die Konfiguration geschrieben."};
				const std::string kMsgWriteUserDefinedRangeObjects[] = {"User defined range objects for Node (%d) write successful.", "Benutzerobjektbereich fuer den Knoten (%d) wurden erfolgreich in die Konfiguration geschrieben."};

				const std::string kMsgGeneralFeatureDatatypeError[] = {"General feature \"%s\" actual / default value does not match datatype.", "General Feature \"%s\" aktueller oder default Wert stimmt nicht mit dem Datentyp ueberein."};
				const std::string kMsgCnFeatureDatatypeError[] = {"CN feature \"%s\" actual / default value does not match datatype.", "CN Feature \"%s\" aktueller oder default Wert stimmt nicht mit dem Datentyp ueberein."};
				const std::string kMsgMnFeatureDatatypeError[] = {"MN feature \"%s\" actual / default value does not match datatype.", "MN Feature \"%s\" aktueller oder default Wert stimmt nicht mit dem Datentyp ueberein."};
				const std::string kMsgFeatureActualValue[] = {"Feature \"%s\" does not have an actual value.", "Feature \"%s\" hat keinen konfigurierten Wert."};

				const std::string kMsgNodeAssignmentNotSupported[] = {"Node assignment \"%s\" is not supported on node \"%d\".", "Node assignment \"%s\" wird von Knoten \"%d\" nicht unterstuetzt."};
				const std::string kMsgNodeAssignmentAlreadyExists[] = {"Node assignment \"%s\" already exists on node \"%d\".", "Node assignment \"%s\" existiert bereits auf dem Knoten \"%d\"."};

				const std::string kMsgDynamicChannelNotFound[] = {"Dynamic channel for datatype: \"%s\" and direction: \"%s\" not found on the Managing Node.", "Dynamic channel fuer Datentyp: \"%s\" und Richtung: \"%s\" wurde am Managing Node nicht gefunden." };
				const std::string kMsgNonExistingMappedObject[] = {"Mapped object 0x%X does not exist on node \"%d\".", "Das gemappte Objekt 0x%X existiert nicht auf dem Knoten \"%d\"."};
				const std::string kMsgNonExistingMappedSubObject[] = {"Mapped subobject 0x%X/0x%X does not exist on node \"%d\".", "Das gemappte Subobjekt 0x%X/0x%X existiert nicht auf dem Knoten \"%d\"."};
				const std::string kMsgMappedObjectSizeInvalid[] = {"Mapped subobject 0x%X/0x%X has size \"%d\" but \"%d\" is expected on node \"%d\".", "Das gemappte Subobjekt 0x%X/0x%X hat eine Laenge von \"%d\" aber \"%d\" wird auf dem Knoten \"%d\" erwartet."};
				const std::string kMsgAccessTypeForParameterInvalid[] = {"Parameter with uniqueID \"%s\", referenced by mapped index 0x%X/0x%X on node \"%d\", has invalid access type (\"%s\").", "Parameter mit uniqueID \"%s\", referenziert vom Mappingobject 0x%X/0x%X auf dem Knoten \"%d\", hat einen ungueltigen Zugriffstyp (\"%s\")."};
				const std::string kMsgAccessTypeForParameterGroupInvalid[] = {"ParameterGroup with uniqueID \"%s\" must not have multiple access types.", "ParameterGroup mit der uniqueID \"%s\" darf nicht mehrere Zugriffstypen haben."};
				const std::string kMsgMappingTypeForPdoInvalid[] = {"(Sub)Index 0x%X/0x%X on node \"%d\" cannot be mapped. Wrong PDOMapping attribute \"%s\" for mapping.", "(Sub)Index 0x%X/0x%X auf dem Knoten \"%d\" kann nicht gemappt werden. Falsches PDOMapping Attribute \"%s\" fuer das Mapping."};
				const std::string kMsgMappingSizeZero[] = { "(Sub)Index 0x%X/0x%X on node \"%d\" cannot be mapped. Data size is not defined or zero.", "(Sub)Index 0x%X/0x%X auf dem Knoten \"%d\" kann nicht gemappt werden. Die Datengroesse ist undefiniert oder null." };
				const std::string kMsgAccessTypeForPdoInvalid[] = {"(Sub)Index 0x%X/0x%X on node \"%d\" cannot be mapped. Invalid access type \"%s\" (RPDOs must be writeable, TPDOs readable).", "(Sub)Index 0x%X/0x%X auf dem Knoten \"%d\" kann nicht gemappt werden. Falscher Zugriffstyp \"%s\" (RPDOs muessen schreibbar sein, TPDOs muessen lesbar sein)."};
				const std::string kMsgPdoOffsetInvalid[] = {"Mapping-Object 0x%X/0x%X on node \"%d\": Mapping offset invalid. Actual: \"%d\" bits, expected \"%d\" bits.", "Mappingobjekt 0x%X/0x%X auf dem Knoten \"%d\": Mappingoffset ist ungueltig. Konfiguriert: \"%d\" bits, erwartet \"%d\" bits."};
				const std::string kMsgNoNodesConfigured[] = {"Network does not contain any POWERLINK controlled nodes.", "Das Netzwerk enthaelt keine POWERLINK Knoten."};
				const std::string kMsgLowCnPresTimeout[] = {"PResTimeout (%dns) (sub-object 0x1F92/0x%X on MN) of node \"%d\" is less than PresMaxLatency (sub-object 0x1F98/0x03 on CN).", "PResTimeout (%dns) (Subobjekt 0x1F92/0x%X auf dem MN) auf dem Knoten \"%d\" ist kleiner als PresMaxLatency (Subobjekt 0x1F98/0x03 auf dem CN)."};
				const std::string kMsgLowCnPresTimeoutDefault[] = {"PResTimeout (%dns) (sub-object 0x1F92/0x%X on MN) of node \"%d\" is less than DS301 default value (25000ns).", "PResTimeout (%dns) (Subobjekt 0x1F92/0x%X on MN) auf dem Knoten \"%d\" ist kleiner als der default Wert aus der DS301 (25000ns)."};
				const std::string kMsgDefaultCnPresTimeout[] = {"PResTimeout (sub-object 0x1F92/0x%X on MN) of node \"%d\" does not a have a default value. Error according to DS 301.", "PResTimeout (Subobjekt 0x1F92/0x%X on MN) auf dem Knoten \"%d\" hat keine default Wert. Dieser muss laut DS 301 vorhanden sein."};
				const std::string kMsgParameterValueInvalid[] = {"Parameter actual or default value \"%s\" with datatype \"%s\" does not fit the datatype limits or format.", "Parameter konfigurierter oder default Wert \"%s\" mit dem Datentyp \"%s\" passt nicht in die Datentypgrenzen oder hat ein falsches Format."};
				const std::string kMsgParameterMinMaxValueInvalid[] = {"Parameter minimum (\"%s\") or maximum value (\"%s\") with datatype \"%s\" does not fit the datatype limits or format.", "Parameter Minimalwert (\"%s\") oder Maximalwert (\"%s\") mit dem Datentyp \"%s\" passt nicht in die Datentypgrenzen oder hat ein falsches Format."};
				const std::string kMsgParameterDataTypeInvalid[] = {"Parameter (\"%s\") does not have a datatype.", "Parameter (\"%s\") hat keinen definierten Datentyp."};

				const std::string kMsgManagingNodeTransmitPres[] = {"Managing node transmits PRes but no controlled node is receiving any data.", "Der Managing Node schickt einen PRes aber kein Knoten empfaengt Daten davon."};
				const std::string kMsgMappingOperationNotSupported[] = {"Mapping operation is not supported for managing or redundant managing node.", "Mappingmanipulation wird fuer Managing und redundante Managing Nodes nicht unterstuetzt."};
				const std::string kMsgMappingObjectInvalid[] = {"Mapping-Object 0x%X/0x%X on node \"%d\": Enabled mapping entry does not have a valid object value.", "Mappingobjekt 0x%X/0x%X auf dem Knoten \"%d\": Aktivierter Mappingeintrag hat keinen gueltigen Wert."};

				const std::string kMsgNrOfEntriesInvalid[] = {"Channel 0x%X on node \"%d\": NrOfEntries invalid. Valid mapping entries: \"%d\", Mapping entries enabled: \"%d\".", "Kanal 0x%X auf dem Knoten \"%d\": NrOfEntries ungueltig. Gueltige Mappingeintraege: \"%d\", Aktivierte Mappingeintraege: \"%d\"."};
				const std::string kMsgDynamicChannelExceeded[] = {"Object \"0x%X/0x%X\" exceeds object range (0x%X - 0x%X) of MN dynamic channel for datatype: \"%s\" and direction: \"%s\".", "Objekt \"0x%X/0x%X\" uebersteigt Objektbereich (0x%X - 0x%X) vom MN Dynamic Channel fuer Datentyp: \"%s\" und Richtung: \"%s\"." };
				const std::string kMsgChannelExceeded[] = {"No MN %s-Channel available for node \"%s\" (%d).", "Kein MN %s-Kanal verfuegbar fuer den Knoten \"%s\" (%d)."};

				const std::string kMsgCycleTimeMin[] = {"Cycle time \"%d\" is lower than the minimum cycle time \"%d\" of node \"%s\" (%d).", "Zykluszeit \"%d\" ist kleiner als die minimale Zykluszeit \"%d\" auf dem Knoten \"%s\" (%d)."};
				const std::string kMsgCycleTimeMax[] = {"Cycle time \"%d\" is higher than the maximum cycle time \"%d\" of node \"%s\" (%d).", "Zykluszeit\"%d\" ist groesser als die maximale Zykluszeit  \"%d\" auf dem Knoten \"%s\" (%d)."};
				const std::string kMsgCycleTimeGran[] = {"Cycle time \"%d\" does not fit to the cycle time granularity \"%d\" of node \"%s\" (%d).", "Zykluszeit \"%d\" passt nicht zur Zykluszeitgranularitaet \"%d\" auf dem Knoten \"%s\" (%d)."};
				const std::string kMsgIsochronousMaxPayloadExceeded[] = {"Node \"%d\": No. of %s-bytes mapped (%d) exceeds limit (1490, C_DLL_ISOCHR_MAX_PAYL).", "Knoten \"%d\": Anzahl der gemappten %s-bytes (%d) ueberschreitet das Limit (1490, C_DLL_ISOCHR_MAX_PAYL)."};
				const std::string kMsgChainingRmnNotSupported[] = {"PRes Chaining is not supported in combination with Redundant Managing Nodes.", "PRes Chaining wird in Kombination mit Redundant Managing Nodes nicht unterstuetzt."};
				const std::string kMsgChainingMnNotSupported[] = {"PRes Chaining is not supported by the managing node.", "PRes Chaining wird vom Managing Node nicht unterstuetzt."};
				const std::string kMsgMultiplexingMnNotSupported[] = {"Multiplexing is not supported by the managing node.", "Multiplexing wird vom Managing Node nicht unterstuetzt."};

				const std::string kMsgNodeIsNotAModularControlledNode[] = {"Node with ID \"%d\" is not a modular head node.", "Knoten mit der ID \"%d\" ist keine modulare Kopfstation."};
				const std::string kMsgInterfaceDoesNotExists[] = {"Interface \"%s\" does not exist.", "Interface \"%s\" existiert nicht."};
				const std::string kMsgInterfaceAlreadyExists[] = {"Interface \"%s\" already exists.", "Interface \"%s\" existiert brereits."};
				const std::string kMsgRangeAlreadyExists[] = {"Range \"%s\" already exists.", "Range \"%s\" existiert bereits."};
				const std::string kMsgRangeDoesNotExists[] = {"Range \"%s\" does not exist.", "Range \"%s\" existiert nicht."};
				const std::string kMsgRangeInvalid[] = {"Range \"%s\" cannot be added because it has invalid attributes.", "Range \"%s\" kann nicht hinzugefuegt werden weil sie ungueltige Attribute besitzt."};
				const std::string kMsgRangeHasNoFreeObjects[] = {"Range \"%s\" has no free objects.", "Range \"%s\" hat keine unbenutzten Objekte."};
				const std::string kMsgRangeIndexAlreadyTaken[] = {"The index 0x%X is already taken in range \"%s\".", "Der Index 0x%X wird in der Range \"%s\" bereits verwendet."};
				const std::string kMsgModuleDoesNotExists[] = {"Module \"%s\" does not exist on position \"%d\".", "Modul \"%s\" existiert nicht an der Position \"%d\"."};
				const std::string kMsgModuleAlreadyExists[] = {"Module \"%s\" already exists on position \"%d\".", "Modul \"%s\" existiert bereits an der Position \"%d\"."};
				const std::string kMsgModuleDoesNotMatch[] = {"Module \"%s\" does not match module \"%s\" on position \"%d\".", "Modul \"%s\" stimmt nicht mit dem Modul \"%s\" auf der Position \"%d\" ueberein."};
				const std::string kMsgModulePositionOccupied[] = {"Module \"%s\" cannot be added to the position \"%d\" because it is already occupied.", "Modul \"%s\" kann nicht auf die Position \"%d\" gesetzt werden, weil diese bereits besetzt ist."};
				const std::string kMsgModuleAddressOccupied[] = {"Address of Module \"%s\" cannot be changed to \"%d\" because it is already occupied.", "Die Adresse des Moduls \"%s\" kann nicht auf \"%d\" gesetzt werden, weil diese bereits besetzt ist."};
				const std::string kMsgInterfaceDoesNotSupportManualAddressing[] = { "Module Interface \"%s\" of \"%s\" (on node \"%s\" (%d)) does not support manual addressing.", "Das Modulinterface \"%s\" von \"%s\" (auf dem Knoten \"%s\" (%d)) unterstuetzt die manuelle Adressierung nicht." };
				const std::string kMsgInterfaceDoesNotHaveAnyRange[] = { "Module Interface \"%s\" on node \"%s\" (%d) does not have any referenced index ranges.", "Das Modulinterface \"%s\" auf dem Knoten \"%s\" (%d) hat keine zugewiesenen Index Ranges." };

				const std::string kMsgInvalidProcessImage[] = {"Invalid process image ignored : ", "Ungueltiges Prozessabbild wurde ignoriert: "};
				const std::string kMsgInvalidMappingOffset[] = {"Invalid mapping offset ignored : ", "Ungueltiger Mappingoffset wurde ignoriert: "};
				const std::string kMsgInvalidMappingRecalculated[] = {" Mapping value has been recalculated.", " Der Mappingeintrag wurde erneut berechnet."};
				const std::string kMsgInvalidMappingReference[] = {"Invalid mapping object reference ignored: ", "Ungueltiger Mappingobjektreferenz wurde ignoriert: "};

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
						explicit Result(const ErrorCode& errorCode);
						Result(const ErrorCode& errorCode, const std::string& errorMessage);

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
