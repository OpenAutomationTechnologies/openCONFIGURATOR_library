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
            using (var streamReader = new StreamReader("boost_log_settings.ini", Encoding.UTF8))
            {
                loggingConfig = streamReader.ReadToEnd();
            }

            // Init logger class with configuration file path
            Result logres = core.InitLoggingConfiguration(loggingConfig);
            core.CreateNetwork("test");

            // Create an initial network
            core.CreateNetwork("test");

            // Add two nodes to the network
            core.CreateNode("test", 240, "MN");
          

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
            core.CreateConfigurationSetting("test", "custom", "GENERATE_MN_NODE_ASSIGNMENT_FOR_NODES", "");
            core.CreateConfigurationSetting("test", "custom", "GENERATE_MN_PRES_TIMEOUT_FOR_NODES", "1");


            var coll = new SettingsCollection();
            Console.WriteLine(core.GetConfigurationSettings("test", "all", coll).GetErrorType());
            Console.WriteLine(coll.Count);
            Console.WriteLine(core.SetConfigurationSettingEnabled("test", "all",
                    "GENERATE_MN_NODE_ASSIGNMENT_FOR_NODES", false).GetErrorType());

            core.SetActiveConfiguration("test", "custom");

            core.CreateConfiguration("test", "none");

            core.CreateObject("test", 240, 0x01006, ObjectType.RECORD, "objectName", PlkDataType.UNDEFINED, AccessType.UNDEFINED, PDOMapping.UNDEFINED, "", "");
            core.CreateSubObject("test", 240, 0x01006, 0, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "400", "400");

            core.CreateObject("test", 240, 0x1F98, ObjectType.RECORD, "objectName", PlkDataType.UNDEFINED, AccessType.UNDEFINED, PDOMapping.UNDEFINED, "", "");
            core.CreateSubObject("test", 240, 0x1F98, 0, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "3", "3");
            core.CreateSubObject("test", 240, 0x1F98, 7, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "0", "");
            core.CreateSubObject("test", 240, 0x1F98, 8, ObjectType.VAR, "testName", PlkDataType.UNSIGNED16, AccessType.RW, PDOMapping.DEFAULT, "300", "");
            core.CreateSubObject("test", 240, 0x1F98, 9, ObjectType.VAR, "testName", PlkDataType.UNSIGNED16, AccessType.RW, PDOMapping.DEFAULT, "2", "");

            core.CreateObject("test", 240, 0x1F81, ObjectType.RECORD, "objectName", PlkDataType.UNDEFINED, AccessType.UNDEFINED, PDOMapping.UNDEFINED, "", "");
            core.CreateSubObject("test", 240, 0x1F81, 0, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "3", "3");
            core.CreateSubObject("test", 240, 0x1F81, 1, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 240, 0x1F81, 2, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 240, 0x1F81, 5, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 240, 0x1F81, 10, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 240, 0x1F81, 20, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "0");

            core.CreateObject("test", 240, 0x1600, ObjectType.RECORD, "objectName", PlkDataType.UNDEFINED, AccessType.UNDEFINED, PDOMapping.UNDEFINED, "", "");
            core.CreateSubObject("test", 240, 0x1600, 0, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "0", "10");
            core.CreateSubObject("test", 240, 0x1600, 1, ObjectType.VAR, "testName", PlkDataType.UNSIGNED16, AccessType.RW, PDOMapping.DEFAULT, "0", "100");
            core.CreateSubObject("test", 240, 0x1600, 2, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "0x100");
            core.CreateSubObject("test", 240, 0x1600, 3, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0x0010000000202104");

            core.CreateObject("test", 240, 0x1F26, ObjectType.RECORD, "objectName", PlkDataType.UNDEFINED, AccessType.UNDEFINED, PDOMapping.UNDEFINED, "", "");
            core.CreateSubObject("test", 240, 0x1F26, 0, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "4", "4");
            core.CreateSubObject("test", 240, 0x1F26, 1, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 240, 0x1F26, 5, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 240, 0x1F26, 10, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 240, 0x1F26, 20, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "0");

            core.CreateObject("test", 240, 0x1F27, ObjectType.RECORD, "objectName", PlkDataType.UNDEFINED, AccessType.UNDEFINED, PDOMapping.UNDEFINED, "", "");
            core.CreateSubObject("test", 240, 0x1F27, 0, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "4", "4");
            core.CreateSubObject("test", 240, 0x1F27, 1, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 240, 0x1F27, 5, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 240, 0x1F27, 10, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 240, 0x1F27, 20, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "0");

            core.SetCycleTime("test", 10000); // Set Cycle time
            
            core.CreateNode("test", 10, "Redundant_MN");
            core.CreateObject("test", 10, 0x1020, ObjectType.RECORD, "objectName", PlkDataType.UNDEFINED, AccessType.UNDEFINED, PDOMapping.UNDEFINED, "", "");
            core.CreateSubObject("test", 10, 0x1020, 0, ObjectType.DEFTYPE, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "2", "2");
            core.CreateSubObject("test", 10, 0x1020, 1, ObjectType.DEFTYPE, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 10, 0x1020, 2, ObjectType.DEFTYPE, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "0");

            core.CreateObject("test", 10, 0x01006, ObjectType.RECORD, "objectName", PlkDataType.UNDEFINED, AccessType.UNDEFINED, PDOMapping.UNDEFINED, "", "");
            core.CreateSubObject("test", 10, 0x01006, 0, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "400", "400");

            core.CreateObject("test", 10, 0x1F98, ObjectType.RECORD, "objectName", PlkDataType.UNDEFINED, AccessType.UNDEFINED, PDOMapping.UNDEFINED, "", "");
            core.CreateSubObject("test", 10, 0x1F98, 0, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "3", "3");
            core.CreateSubObject("test", 10, 0x1F98, 7, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 10, 0x1F98, 8, ObjectType.VAR, "testName", PlkDataType.UNSIGNED16, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 10, 0x1F98, 9, ObjectType.VAR, "testName", PlkDataType.UNSIGNED16, AccessType.RW, PDOMapping.DEFAULT, "0", "0");


            core.CreateNode("test", 20, "Redundant_MN");
            core.CreateObject("test", 20, 0x1020, ObjectType.RECORD, "objectName", PlkDataType.UNDEFINED, AccessType.UNDEFINED, PDOMapping.UNDEFINED, "", "");
            core.CreateSubObject("test", 20, 0x1020, 0, ObjectType.DEFTYPE, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "2", "2");
            core.CreateSubObject("test", 20, 0x1020, 1, ObjectType.DEFTYPE, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 20, 0x1020, 2, ObjectType.DEFTYPE, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "0");

            core.CreateObject("test", 20, 0x01006, ObjectType.RECORD, "objectName", PlkDataType.UNDEFINED, AccessType.UNDEFINED, PDOMapping.UNDEFINED, "", "");
            core.CreateSubObject("test", 20, 0x01006, 0, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "400", "400");

            core.CreateObject("test", 20, 0x1F98, ObjectType.RECORD, "objectName", PlkDataType.UNDEFINED, AccessType.UNDEFINED, PDOMapping.UNDEFINED, "", "");
            core.CreateSubObject("test", 20, 0x1F98, 0, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "3", "3");
            core.CreateSubObject("test", 20, 0x1F98, 7, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 20, 0x1F98, 8, ObjectType.VAR, "testName", PlkDataType.UNSIGNED16, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 20, 0x1F98, 9, ObjectType.VAR, "testName", PlkDataType.UNSIGNED16, AccessType.RW, PDOMapping.DEFAULT, "0", "0");

            
            core.CreateNode("test", 1, "CN");
            core.CreateObject("test", 1, 0x1020, ObjectType.RECORD, "objectName", PlkDataType.UNDEFINED, AccessType.UNDEFINED, PDOMapping.UNDEFINED, "", "");
            core.CreateSubObject("test", 1, 0x1020, 0, ObjectType.DEFTYPE, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "2", "2");
            core.CreateSubObject("test", 1, 0x1020, 1, ObjectType.DEFTYPE, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 1, 0x1020, 2, ObjectType.DEFTYPE, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "0");

            core.CreateObject("test", 1, 0x01006, ObjectType.RECORD, "objectName", PlkDataType.UNDEFINED, AccessType.UNDEFINED, PDOMapping.UNDEFINED, "", "");
            core.CreateSubObject("test", 1, 0x01006, 0, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "400", "400");

            core.CreateObject("test", 1, 0x1F98, ObjectType.RECORD, "objectName", PlkDataType.UNDEFINED, AccessType.UNDEFINED, PDOMapping.UNDEFINED, "", "");
            core.CreateSubObject("test", 1, 0x1F98, 0, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "3", "3");
            core.CreateSubObject("test", 1, 0x1F98, 7, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 1, 0x1F98, 8, ObjectType.VAR, "testName", PlkDataType.UNSIGNED16, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 1, 0x1F98, 9, ObjectType.VAR, "testName", PlkDataType.UNSIGNED16, AccessType.RW, PDOMapping.DEFAULT, "0", "0");

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

            core.CreateNode("test", 5, "node");

            core.CreateObject("test", 5, 0x1F98, ObjectType.RECORD, "objectName", PlkDataType.UNDEFINED, AccessType.UNDEFINED, PDOMapping.UNDEFINED, "", "");
            core.CreateSubObject("test", 5, 0x1F98, 0, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "3", "3");
            core.CreateSubObject("test", 5, 0x1F98, 7, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 5, 0x1F98, 8, ObjectType.VAR, "testName", PlkDataType.UNSIGNED16, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 5, 0x1F98, 9, ObjectType.VAR, "testName", PlkDataType.UNSIGNED16, AccessType.RW, PDOMapping.DEFAULT, "0", "0");

            core.CreateObject("test", 5, 0x01006, ObjectType.RECORD, "objectName", PlkDataType.UNDEFINED, AccessType.UNDEFINED, PDOMapping.UNDEFINED, "", "");
            core.CreateSubObject("test", 5, 0x01006, 0, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "400", "400");

            core.CreateObject("test", 5, 0x1020, ObjectType.RECORD, "objectName", PlkDataType.UNDEFINED, AccessType.UNDEFINED, PDOMapping.UNDEFINED, "", "");
            core.CreateSubObject("test", 5, 0x1020, 0, ObjectType.DEFTYPE, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "2", "2");
            core.CreateSubObject("test", 5, 0x1020, 1, ObjectType.DEFTYPE, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 5, 0x1020, 2, ObjectType.DEFTYPE, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "0");

            core.CreateObject("test", 5, 0x1600, ObjectType.RECORD, "objectName", PlkDataType.UNDEFINED, AccessType.UNDEFINED, PDOMapping.UNDEFINED, "", "");
            core.CreateSubObject("test", 5, 0x1600, 0, ObjectType.DEFTYPE, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "0", "10");
            core.CreateSubObject("test", 5, 0x1600, 1, ObjectType.DEFTYPE, "testName", PlkDataType.UNSIGNED16, AccessType.RW, PDOMapping.DEFAULT, "0", "0x100");
            core.CreateSubObject("test", 5, 0x1600, 2, ObjectType.DEFTYPE, "testName", PlkDataType.UNSIGNED24, AccessType.RW, PDOMapping.DEFAULT, "0", "0x100");
            core.CreateSubObject("test", 5, 0x1600, 3, ObjectType.DEFTYPE, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "0x100");
            core.CreateSubObject("test", 5, 0x1600, 4, ObjectType.DEFTYPE, "testName", PlkDataType.UNSIGNED40, AccessType.RW, PDOMapping.DEFAULT, "0", "0x100");
            core.CreateSubObject("test", 5, 0x1600, 6, ObjectType.DEFTYPE, "testName", PlkDataType.UNSIGNED56, AccessType.RW, PDOMapping.NO, "", "100");
            core.CreateSubObject("test", 5, 0x1600, 7, ObjectType.DEFTYPE, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.NO, "0", "0x0010000000202104");

            core.CreateObject("test", 5, 0x1602, ObjectType.RECORD, "objectName", PlkDataType.UNDEFINED, AccessType.UNDEFINED, PDOMapping.UNDEFINED, "", "");
            core.CreateSubObject("test", 5, 0x1602, 0, ObjectType.DEFTYPE, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.NO, "0", "0x10");
            core.CreateSubObject("test", 5, 0x1602, 1, ObjectType.DEFTYPE, "testName", PlkDataType.INTEGER16, AccessType.RW, PDOMapping.NO, "0", "0x100");
            core.CreateSubObject("test", 5, 0x1602, 2, ObjectType.DEFTYPE, "testName", PlkDataType.INTEGER24, AccessType.RW, PDOMapping.NO, "0", "0x100");
            core.CreateSubObject("test", 5, 0x1602, 3, ObjectType.DEFTYPE, "testName", PlkDataType.INTEGER32, AccessType.RW, PDOMapping.NO, "", "100");
            core.CreateSubObject("test", 5, 0x1602, 4, ObjectType.DEFTYPE, "testName", PlkDataType.INTEGER40, AccessType.RW, PDOMapping.NO, "", "100");
            core.CreateSubObject("test", 5, 0x1602, 5, ObjectType.DEFTYPE, "testName", PlkDataType.INTEGER48, AccessType.RW, PDOMapping.NO, "", "100");
            core.CreateSubObject("test", 5, 0x1602, 6, ObjectType.DEFTYPE, "testName", PlkDataType.INTEGER56, AccessType.RW, PDOMapping.NO, "", "100");
            core.CreateSubObject("test", 5, 0x1602, 7, ObjectType.DEFTYPE, "testName", PlkDataType.INTEGER64, AccessType.RW, PDOMapping.NO, "0", "0x0010000000202104");

            core.CreateObject("test", 5, 0x1610, ObjectType.DEFTYPE, "Name", PlkDataType.UNDEFINED, AccessType.UNDEFINED, PDOMapping.UNDEFINED, "", "");
            core.CreateSubObject("test", 5, 0x1610, 0x00, ObjectType.DEFTYPE, "Name", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.RPDO, "0", "15");
            core.CreateSubObject("test", 5, 0x1610, 0x0d, ObjectType.DEFTYPE, "Name", PlkDataType.UNICODE_STRING, AccessType.RW, PDOMapping.RPDO, "100", "WriteThisString");
            core.CreateSubObject("test", 5, 0x1610, 0x01, ObjectType.DEFTYPE, "Name", PlkDataType.VISIBLE_STRING, AccessType.RW, PDOMapping.RPDO, "100", "WriteThisString");
            core.CreateSubObject("test", 5, 0x1610, 0x02, ObjectType.DEFTYPE, "Name", PlkDataType.OCTET_STRING, AccessType.RW, PDOMapping.RPDO, "100", "WriteThisString");
            core.CreateSubObject("test", 5, 0x1610, 0x03, ObjectType.DEFTYPE, "Name", PlkDataType.IP_ADDRESS, AccessType.RW, PDOMapping.RPDO, "100", "0x64646464");
            core.CreateSubObject("test", 5, 0x1610, 0x0b, ObjectType.DEFTYPE, "Name", PlkDataType.IP_ADDRESS, AccessType.RW, PDOMapping.RPDO, "100", "254.254.254.254");
            core.CreateSubObject("test", 5, 0x1610, 0x0c, ObjectType.DEFTYPE, "Name", PlkDataType.IP_ADDRESS, AccessType.RW, PDOMapping.RPDO, "100", "1.1.1.1");
            core.CreateSubObject("test", 5, 0x1610, 0x04, ObjectType.DEFTYPE, "Name", PlkDataType.BOOLEAN, AccessType.RW, PDOMapping.RPDO, "false", "true");
            core.CreateSubObject("test", 5, 0x1610, 0x05, ObjectType.DEFTYPE, "Name", PlkDataType.MAC_ADDRESS, AccessType.RW, PDOMapping.RPDO, "0", "0x000A086121012");
            core.CreateSubObject("test", 5, 0x1610, 0x06, ObjectType.DEFTYPE, "Name", PlkDataType.REAL32, AccessType.RW, PDOMapping.RPDO, "0", "1267.43233E12");
            core.CreateSubObject("test", 5, 0x1610, 0x07, ObjectType.DEFTYPE, "Name", PlkDataType.REAL32, AccessType.RW, PDOMapping.RPDO, "0", "1.234532432");
            core.CreateSubObject("test", 5, 0x1610, 0x08, ObjectType.DEFTYPE, "Name", PlkDataType.REAL64, AccessType.RW, PDOMapping.RPDO, "0", "-1E4");
            core.CreateSubObject("test", 5, 0x1610, 0x09, ObjectType.DEFTYPE, "Name", PlkDataType.REAL64, AccessType.RW, PDOMapping.RPDO, "0", "12.78e-2");
            core.CreateSubObject("test", 5, 0x1610, 0x0a, ObjectType.DEFTYPE, "Name", PlkDataType.REAL64, AccessType.RW, PDOMapping.RPDO, "6", "-0");
            core.CreateSubObject("test", 5, 0x1610, 0x10, ObjectType.DEFTYPE, "Name", PlkDataType.REAL64, AccessType.RW, PDOMapping.RPDO, "6", "-0");

            core.CreateObject("test", 5, 0x1A00, ObjectType.DEFTYPE, "Name", PlkDataType.UNDEFINED, AccessType.UNDEFINED, PDOMapping.UNDEFINED, "", "");
            core.CreateSubObject("test", 5, 0x1A00, 0x00, ObjectType.DEFTYPE, "Name", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.RPDO, "0", "5");
            core.CreateSubObject("test", 5, 0x1A00, 0x0d, ObjectType.DEFTYPE, "Name", PlkDataType.UNICODE_STRING, AccessType.RW, PDOMapping.RPDO, "100", "WriteThisString");
            core.CreateSubObject("test", 5, 0x1A00, 0x01, ObjectType.DEFTYPE, "Name", PlkDataType.VISIBLE_STRING, AccessType.RW, PDOMapping.RPDO, "100", "WriteThisString");
            core.CreateSubObject("test", 5, 0x1A00, 0x02, ObjectType.DEFTYPE, "Name", PlkDataType.OCTET_STRING, AccessType.RW, PDOMapping.RPDO, "100", "WriteThisString");
            core.CreateSubObject("test", 5, 0x1A00, 0x03, ObjectType.DEFTYPE, "Name", PlkDataType.IP_ADDRESS, AccessType.RW, PDOMapping.RPDO, "100", "0x64646464");
            core.CreateSubObject("test", 5, 0x1A00, 0x0b, ObjectType.DEFTYPE, "Name", PlkDataType.IP_ADDRESS, AccessType.RW, PDOMapping.RPDO, "100", "254.254.254.254");
            core.CreateSubObject("test", 5, 0x1A00, 0x0c, ObjectType.DEFTYPE, "Name", PlkDataType.IP_ADDRESS, AccessType.RW, PDOMapping.RPDO, "100", "1.1.1.1");
            core.CreateSubObject("test", 5, 0x1A00, 0x04, ObjectType.DEFTYPE, "Name", PlkDataType.BOOLEAN, AccessType.RW, PDOMapping.RPDO, "false", "true");
            core.CreateSubObject("test", 5, 0x1A00, 0x05, ObjectType.DEFTYPE, "Name", PlkDataType.MAC_ADDRESS, AccessType.RW, PDOMapping.RPDO, "0", "0x000A086121012");
            core.CreateSubObject("test", 5, 0x1A00, 0x06, ObjectType.DEFTYPE, "Name", PlkDataType.REAL32, AccessType.RW, PDOMapping.RPDO, "0", "1267432.33E12");
            core.CreateSubObject("test", 5, 0x1A00, 0x07, ObjectType.DEFTYPE, "Name", PlkDataType.REAL32, AccessType.RW, PDOMapping.RPDO, "0", "1.234532432");
            core.CreateSubObject("test", 5, 0x1A00, 0x08, ObjectType.DEFTYPE, "Name", PlkDataType.REAL64, AccessType.RW, PDOMapping.RPDO, "0", "-1E4");
            core.CreateSubObject("test", 5, 0x1A00, 0x09, ObjectType.DEFTYPE, "Name", PlkDataType.REAL64, AccessType.RW, PDOMapping.RPDO, "0", "12.78e-2");
            core.CreateSubObject("test", 5, 0x1A00, 0x0a, ObjectType.DEFTYPE, "Name", PlkDataType.REAL64, AccessType.RW, PDOMapping.RPDO, "6", "-0");
            core.CreateSubObject("test", 5, 0x1A00, 0x10, ObjectType.DEFTYPE, "Name", PlkDataType.REAL64, AccessType.RW, PDOMapping.RPDO, "6", "-0");

            core.CreateObject("test", 5, 0x1400, ObjectType.DEFTYPE, "Name", PlkDataType.UNDEFINED, AccessType.UNDEFINED, PDOMapping.UNDEFINED, "", "");
            core.CreateSubObject("test", 5, 0x1400, 0x00, ObjectType.DEFTYPE, "Name", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.RPDO, "0", "15");
            core.CreateSubObject("test", 5, 0x1400, 0x01, ObjectType.DEFTYPE, "Name", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.RPDO, "100", "5");


            core.CreateObject("test", 5, 0x2000, ObjectType.RECORD, "objectName", PlkDataType.UNDEFINED, AccessType.UNDEFINED, PDOMapping.UNDEFINED, "", "");
            core.CreateSubObject("test", 5, 0x2000, 0x0b, ObjectType.DEFTYPE, "Name", PlkDataType.IP_ADDRESS, AccessType.RW, PDOMapping.RPDO, "100", "254.254.254.254");


            uint obsize = 0;
            res = core.GetObjectSize("test", 1, 0x1601, out obsize);
            Console.WriteLine(obsize);
            Console.WriteLine(res.IsSuccessful());

            res = core.GetSubObjectSize("test", 1, 0x1601, 0, out obsize);
            Console.WriteLine(obsize);
            Console.WriteLine(res.IsSuccessful());

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

            res = core.CreateDomainObject("test", 1, 0x2000, ObjectType.DEFSTRUCT, "domain", "UID_DOM_Index2101_Sub1E");
            Console.WriteLine(res.GetErrorType());

            res = core.GetObjectSize("test", 1, 0x2000, out obsize);
            Console.WriteLine(obsize);
            Console.WriteLine(res.IsSuccessful());

            res = core.CreateDomainSubObject("test", 1, 0x2000, 0x1, ObjectType.DEFSTRUCT, "domain", "UID_DOM_Index2101_Sub1E");
            Console.WriteLine(res.GetErrorType());

            res = core.GetSubObjectSize("test", 1, 0x2000, 0x1, out obsize);
            Console.WriteLine(obsize);
            Console.WriteLine(res.IsSuccessful());

            string tempString;
            res = core.BuildConfiguration("test", out tempString);
            Console.WriteLine(res.IsSuccessful());

            res = core.RemoveNode("test", 1);
            Console.WriteLine(res.IsSuccessful());

            res = core.BuildConfiguration("test", out tempString);
            Console.WriteLine(res.IsSuccessful());
        }
    }
}
