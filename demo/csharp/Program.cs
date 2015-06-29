/************************************************************************
\file Program.cs
\brief Implementation of the C# test class
\author rueckerc, Bernecker+Rainer Industrie Elektronik Ges.m.b.H.
\date 16-Apr-2015 10:00:00
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

using System;
using System.IO;
using System.Text;

namespace openconfigurator_core_net_app
{
    class Program
    {
        static void Main()
        {
            Console.ReadLine();
            // Retrieve main managing class of the library
            var core = OpenConfiguratorCore.GetInstance();
            Console.ReadLine();

            string loggingConfig = string.Empty;
            using (StreamReader streamReader = new StreamReader("boost_log_settings.ini", Encoding.UTF8))
            {
                loggingConfig = streamReader.ReadToEnd();
            }
            // Init logger class with configuration file path
            Result logres = core.InitLoggingConfiguration(loggingConfig);
            core.CreateNetwork("test");

            // Create an initial network
            var net = new Network();
            core.CreateNetwork("test");
            core.SetCycleTime("test", 10000); // Set Cycle time
            core.GetNetwork("test", net);
            Console.WriteLine(net.GetNetworkId()); // Print UUID to console

            // Add two nodes to the network
            core.CreateNode("test", 240, "MN");
            core.CreateNode("test", 241, "Redundant_MN");
            core.CreateNode("test", 242, "Redundant_MN");
            core.CreateNode("test", 242, "already_existing");
            core.CreateNode("test", 1, "CN");

            // Get a node object from the library
            var node = new ControlledNode(0);
            var res = core.GetControlledNode("test", 1, node);
            Console.WriteLine(res.IsSuccessful());
            Console.WriteLine(node.GetName());

            // Get a network from the library
            var net2 = new Network();
            res = core.GetNetwork("test", net2);
            Console.WriteLine(res.IsSuccessful()); // should be true
            Console.WriteLine(net2.GetNetworkId()); // should be the same
            Console.WriteLine(net2.GetCycleTime()); // should be 10000

            var support = new StringCollection();
            res = core.GetSupportedSettingIds(support);
            Console.WriteLine(res.IsSuccessful());
            foreach (string t in support)
            {
                Console.WriteLine(t);
            }

            // Create settings for a configuration
            var mnMappingAll = new BuildConfigurationSetting(
                    "GENERATE_MN_MAPPING_FOR_NODES", "all");
            var nodeAssignmentAll = new BuildConfigurationSetting(
                    "GENERATE_MN_NODE_ASSIGNMENT_FOR_NODES", "all");
            var presTimeOutAll = new BuildConfigurationSetting(
                    "GENERATE_MN_PRES_TIMEOUT_FOR_NODES", "all");

            // Enable all the settings (are enabled per default)
            mnMappingAll.SetEnabled(true);
            nodeAssignmentAll.SetEnabled(true);
            presTimeOutAll.SetEnabled(true);

            // Print setting info
            Console.WriteLine(mnMappingAll.GetDescription());
            Console.WriteLine(nodeAssignmentAll.GetDescription());
            Console.WriteLine(presTimeOutAll.GetDescription());

            // Create settings configuration
            /*
             * <AutoGenerationSettings id="GenerateAll"> <Setting
             * name="MN_MAPPING_FOR_ALL_NODES" enabled="true"/> <Setting
             * name="MN_NODE_ASSIGNMENT_FOR_ALL_NODES" enabled="true"/> <Setting
             * name="MN_PRES_TIMEOUT_FOR_ALL_NODES" enabled="true"/>
             * </AutoGenerationSettings>
             */
            core.CreateConfiguration("test", "all");
            core.CreateConfigurationSetting("test", "all", "GENERATE_MN_MAPPING_FOR_NODES", "");
            core.CreateConfigurationSetting("test", "all", "GENERATE_MN_NODE_ASSIGNMENT_FOR_NODES", "");
            core.CreateConfigurationSetting("test", "all", "GENERATE_MN_PRES_TIMEOUT_FOR_NODES", "");

            // Create specific settings configuration
            /*
             * <AutoGenerationSettings id="SpecificConfiguration"> <Setting
             * name="MN_MAPPING_FOR_NODE" value="1" enabled="true"/> <Setting
             * name="MN_NODE_ASSIGNMENT_FOR_NODE" value="1" enabled="true"/>
             * <Setting name="MN_PRES_TIMEOUT_FOR_NODE" value="1" enabled="true"/>
             * </AutoGenerationSettings>
             */
            core.CreateConfiguration("test", "custom");
            core.CreateConfigurationSetting("test", "custom", "GENERATE_MN_MAPPING_FOR_NODES", "1");
            core.CreateConfigurationSetting("test", "custom", "GENERATE_MN_NODE_ASSIGNMENT_FOR_NODES", "1");
            core.CreateConfigurationSetting("test", "custom", "GENERATE_MN_PRES_TIMEOUT_FOR_NODES", "1");

            var coll = new SettingsCollection();
            Console.WriteLine(core.GetConfigurationSettings("test", "all", coll).GetErrorType());
            Console.WriteLine(coll.Count);
            Console.WriteLine(core.SetConfigurationSettingEnabled("test", "all",
                    "GENERATE_MN_MAPPING_FOR_NODES", false).GetErrorType());

            core.CreateConfiguration("test", "none");

            core.CreateObject("test", 1, 0x1600, PlkDataType.UNSIGNED16, AccessType.RW, ObjectType.RECORD,
                PDOMapping.RPDO, "1000", "Mapping Object");
            core.CreateSubObject("test", 1, 0x1600, 0, PlkDataType.UNSIGNED8, AccessType.RW, ObjectType.DEFTYPE, PDOMapping.NO, "0", "NrOfEntries");

            Console.ReadLine();
            Result test = core.CreateParameter("test", 1, "UID_DOM_Index2100_Sub1E", ParameterAccess.read);
            Console.WriteLine(test.IsSuccessful());
            test = core.CreateStructDatatype("test", 1, "notFound", "UID_DT_Index2100_Sub1E", "Index2100_Sub1E");
            test = core.CreateStructDatatype("test", 1, "UID_DOM_Index2100_Sub1E", "UID_DT_Index2100_Sub1E", "Index2100_Sub1E");
            Console.WriteLine(test.IsSuccessful());
            test = core.CreateVarDeclaration("test", 1, "notFound", "Index2100_Sub1E_OK", "NetworkStatus", IEC_Datatype.USINT);
            test = core.CreateVarDeclaration("test", 1, "UID_DT_Index2100_Sub1E", "Index2100_Sub1E_OK", "NetworkStatus", IEC_Datatype.USINT);
            test = core.CreateVarDeclaration("test", 1, "UID_DT_Index2100_Sub1E", "UID_Index2100_Sub1E_StatusInput01", "StatusInput01", IEC_Datatype.BITSTRING, 1);
            test = core.CreateVarDeclaration("test", 1, "UID_DT_Index2100_Sub1E", "UID_Index2100_Sub1E_Bit_Unused_01", "StatusInput01", IEC_Datatype.BITSTRING, 1);
            test = core.CreateVarDeclaration("test", 1, "UID_DT_Index2100_Sub1E", "UID_Index2100_Sub1E_StatusInput02", "StatusInput01", IEC_Datatype.BITSTRING, 1);
            test = core.CreateVarDeclaration("test", 1, "UID_DT_Index2100_Sub1E", "UID_Index2100_Sub1E_Bit_Unused_02", "StatusInput01", IEC_Datatype.BITSTRING, 5);
            Console.WriteLine(test.IsSuccessful());

            test = core.CreateParameter("test", 1, "UID_DOM_Index2101_Sub1E", ParameterAccess.read);
            Console.WriteLine(test.IsSuccessful());
            test = core.CreateStructDatatype("test", 1, "UID_DOM_Index2101_Sub1E", "UID_DT_Index2101_Sub1E", "Index2101_Sub1E");
            Console.WriteLine(test.IsSuccessful());
            test = core.CreateVarDeclaration("test", 1, "UID_DT_Index2101_Sub1E", "Index2101_Sub1E_OK", "NetworkStatus", IEC_Datatype.USINT);
            test = core.CreateVarDeclaration("test", 1, "UID_DT_Index2101_Sub1E", "UID_Index2101_Sub1E_DigitalInput01", "DigitalInput01", IEC_Datatype.BITSTRING, 1);
            test = core.CreateVarDeclaration("test", 1, "UID_DT_Index2101_Sub1E", "UID_Index2101_Sub1E_DigitalInput02", "DigitalInput02", IEC_Datatype.BITSTRING, 1);
            test = core.CreateVarDeclaration("test", 1, "UID_DT_Index2101_Sub1E", "UID_Index2101_Sub1E_DigitalInput03", "DigitalInput03", IEC_Datatype.BITSTRING, 1);
            test = core.CreateVarDeclaration("test", 1, "UID_DT_Index2101_Sub1E", "UID_Index2101_Sub1E_DigitalInput04", "DigitalInput04", IEC_Datatype.BITSTRING, 1);
            test = core.CreateVarDeclaration("test", 1, "UID_DT_Index2101_Sub1E", "UID_Index2101_Sub1E_DigitalInput05", "DigitalInput05", IEC_Datatype.BITSTRING, 1);
            test = core.CreateVarDeclaration("test", 1, "UID_DT_Index2101_Sub1E", "UID_Index2101_Sub1E_DigitalInput06", "DigitalInput06", IEC_Datatype.BITSTRING, 1);
            test = core.CreateVarDeclaration("test", 1, "UID_DT_Index2101_Sub1E", "UID_Index2101_Sub1E_DigitalInput07", "DigitalInput07", IEC_Datatype.BITSTRING, 1);
            test = core.CreateVarDeclaration("test", 1, "UID_DT_Index2101_Sub1E", "UID_Index2101_Sub1E_DigitalInput08", "DigitalInput08", IEC_Datatype.BITSTRING, 1);
            test = core.CreateVarDeclaration("test", 1, "UID_DT_Index2101_Sub1E", "UID_Index2101_Sub1E_DigitalInput09", "DigitalInput09", IEC_Datatype.BITSTRING, 1);
            test = core.CreateVarDeclaration("test", 1, "UID_DT_Index2101_Sub1E", "UID_Index2101_Sub1E_DigitalInput10", "DigitalInput10", IEC_Datatype.BITSTRING, 1);
            test = core.CreateVarDeclaration("test", 1, "UID_DT_Index2101_Sub1E", "UID_Index2101_Sub1E_DigitalInput11", "DigitalInput11", IEC_Datatype.BITSTRING, 1);
            test = core.CreateVarDeclaration("test", 1, "UID_DT_Index2101_Sub1E", "UID_Index2101_Sub1E_DigitalInput12", "DigitalInput12", IEC_Datatype.BITSTRING, 1);
            test = core.CreateVarDeclaration("test", 1, "UID_DT_Index2101_Sub1E", "UID_Index2101_Sub1E_Bit_Unused_01", "Bit_Unused_01", IEC_Datatype.BITSTRING, 4);
            Console.WriteLine(test.IsSuccessful());
            test = core.CreateParameter("test", 1, "UID_100", ParameterAccess.read, IEC_Datatype.UDINT);

            uint size;
            test = core.GetDatatypeSize("test", 1, "UID_DT_Index2100_Sub1E", out size);
            Console.WriteLine(test.IsSuccessful());
            Console.WriteLine(size);

            test = core.GetDatatypeSize("test", 1, "UID_DT_Index2101_Sub1E", out size);
            Console.WriteLine(test.IsSuccessful());
            Console.WriteLine(size);

            test = core.GetDatatypeSize("test", 1, "UID_100", out size);
            Console.WriteLine(test.IsSuccessful());
            Console.WriteLine(size);

            test = core.GetDatatypeSize("test", 1, "notFound", out size);
            Console.WriteLine(test.IsSuccessful());
            Console.WriteLine(size);

        }
    }
}
