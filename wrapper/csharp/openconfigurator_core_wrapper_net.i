/*################################################################################
#
# Project: openCONFIGURATOR core library .NET wrapper interface
#
# (c) Bernecker + Rainer Industrie-Elektronik Ges.m.b.H.
#     B&R Strasse 1, A-5142 Eggelsberg
#     www.br-automation.com
#
#
# License:
#
#   Redistribution and use in source and binary forms, with or without
#   modification, are permitted provided that the following conditions
#   are met:
#
#   1. Redistributions of source code must retain the above copyright
#      notice, this list of conditions and the following disclaimer.
#
#   2. Redistributions in binary form must reproduce the above copyright
#      notice, this list of conditions and the following disclaimer in the
#      documentation and/or other materials provided with the distribution.
#
#   3. Neither the name of the copyright holders nor the names of its
#      contributors may be used to endorse or promote products derived
#      from this software without prior written permission. For written
#      permission, please contact office@br-automation.com.
#
#   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
#   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
#   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
#   FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
#   COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
#   INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
#   BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
#   LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
#   CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
#   LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
#   ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
#   POSSIBILITY OF SUCH DAMAGE.
#
#   Severability Clause:
#
#       If a provision of this License is or becomes illegal, invalid or
#       unenforceable in any jurisdiction, that shall not affect:
#       1. the validity or enforceability in that jurisdiction of any other
#          provision of this License; or
#       2. the validity or enforceability in other jurisdictions of that or
#          any other provision of this License.
#
################################################################################*/
%module openconfigurator_core_wrapper_net

#define DLLEXPORT
#define __attribute__(x)
#if defined(_WIN32) && defined(_MSC_VER)
	 #undef DllExport
	 #define DllExport __declspec(dllexport)
#endif

%include typemaps.i
%include std_except.i
%include std_pair.i
%include std_string.i
%include std_map.i
%include std_vector.i
%include "enums.swg"
%include std_string_ref.i
%include stdint.i

%ignore GetReceiveMapping();
%ignore GetTransmitMapping();
%ignore GetMappingObject();
%ignore SetMappingObject(const std::shared_ptr<BaseProcessDataMapping>& mappingObject);
%ignore GetNetworkManagement();
%ignore PlkFeatureDefaultValues;
%ignore DirectionTypeValues;
%ignore GetPlkDataTypeName();
%ignore IECDatatypeValues;
%ignore PlkFeatureStrings;
%ignore SetComplexDataType(std::shared_ptr<Parameter>& parameter);
%ignore SetComplexDataType(std::shared_ptr<ParameterGroup>& parameterGrp);
%ignore GetReferencedParameter();
%ignore GetReferencedParameterGroup();
%ignore ParameterAccessValues;
%ignore PDOMappingValues;
%ignore AccessTypeValues;

%apply unsigned int& OUTPUT { unsigned int& size };
%apply unsigned int& OUTPUT { unsigned int& size };
%apply unsigned int& OUTPUT { unsigned int& cycleTime };
%apply unsigned short& OUTPUT { unsigned short& asyncMtu };
%apply unsigned short& OUTPUT { unsigned short& multiplexedCycleLength };
%apply unsigned short& OUTPUT { unsigned short& prescaler };
%apply unsigned int& OUTPUT { unsigned int& asyncSlotTimeout };
%apply unsigned short& OUTPUT { unsigned short& asndMaxNr };
%apply unsigned int& OUTPUT { unsigned int& presTimeout };
%apply unsigned int& OUTPUT { unsigned int& lossOfSocTolerance };
%apply unsigned int& OUTPUT { unsigned int& waitNotActive };
%apply unsigned int& OUTPUT { unsigned int& priority };
%apply unsigned int& OUTPUT { unsigned int& gapSize };

%{

#include "BaseProcessDataMapping.h"
#include "DynamicChannelAccessType.h"
#include "ErrorCode.h"
#include "IEC_Datatype.h"
#include "ParameterAccess.h"
#include "ObjectType.h"
#include "PDOMapping.h"
#include "PlkDataType.h"
#include "NodeAssignment.h"
#include "DynamicChannel.h"
#include "Result.h"
#include "Network.h"
#include "Direction.h"
#include "PlkConfiguration.h"
#include "BuildConfigurationSetting.h"
#include "BaseNode.h"
#include "BaseObject.h"
#include "Object.h"
#include "SubObject.h"
#include "ControlledNode.h"
#include "ManagingNode.h"
#include "PlkFeatureEnum.h"
#include "PlkOperationMode.h"
#include "OpenConfiguratorCore.h"

%}

%include "ErrorCode.h"
%include "IResult.h"
%template(intResult) IndustrialNetwork::Fieldbus::IResult<IndustrialNetwork::POWERLINK::Core::ErrorHandling::ErrorCode>;
%include "Result.h"

%include "IEC_Datatype.h"
%include "ParameterAccess.h"
%include "AccessType.h"
%include "DynamicChannelAccessType.h"
%include "ObjectType.h"
%include "PDOMapping.h"
%include "PlkDataType.h"

%typemap(csbase) IndustrialNetwork ::POWERLINK::Core::Node::NodeAssignment "uint"
%include "NodeAssignment.h"

%include "DynamicChannel.h"
%include "Network.h"
%include "Direction.h"

%include "IFieldbusNode.h"
%template(intFieldBusNode) IndustrialNetwork::Fieldbus::IFieldbusNode<unsigned char, unsigned int, IndustrialNetwork::POWERLINK::Core::ObjectDictionary::Object>;
%include "BaseNode.h"

%include "IBuildConfigurationSetting.h"
%include "BuildConfigurationSetting.h"
%include "IBuildConfiguration.h"
%template(intBuildConfiguration) IndustrialNetwork::Fieldbus::IBuildConfiguration<IndustrialNetwork::POWERLINK::Core::Configuration::BuildConfigurationSetting>;
%include "PlkConfiguration.h"

%include "IBaseObject.h"
%template(intBaseObject) IndustrialNetwork::Fieldbus::IBaseObject<unsigned int, IndustrialNetwork::POWERLINK::Core::ObjectDictionary::ObjectType>;
%include "BaseObject.h"

%include "Object.h"
%include "SubObject.h"
%include "ControlledNode.h"
%include "ManagingNode.h"
%include "PlkFeatureEnum.h"
%include "PlkOperationMode.h"

%include "BaseProcessDataMapping.h"
%include "BaseProcessImageObject.h"

%include "OpenConfiguratorCore.h"


namespace std {
   %template(StringCollection) std::vector<std::string>;
   %template(ConfigurationCollection) std::vector<IndustrialNetwork::POWERLINK::Core::Configuration::PlkConfiguration>;
   %template(SettingsCollection) std::vector<IndustrialNetwork::POWERLINK::Core::Configuration::BuildConfigurationSetting>;
   %template(ByteCollection) std::vector<unsigned char>;
   %template(ObjectPair) std::pair<unsigned int, int>;
   %template(ObjectCollection) std::map<std::pair<unsigned int, int> , std::string>;
   %template(AssignmentCollection) std::vector<unsigned int>;
}
