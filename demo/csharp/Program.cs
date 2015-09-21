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
            //Console.ReadLine();

            string loggingConfig = string.Empty;
            using (var streamReader = new StreamReader("boost_log_settings.ini", Encoding.UTF8))
            {
                loggingConfig = streamReader.ReadToEnd();
            }

            // Init logger class with configuration file path
            //Result logres = core.InitLoggingConfiguration(loggingConfig);
            core.InitEclipseLoggingPath("./");
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


            var coll = new SettingsCollection();
            Console.WriteLine(core.GetConfigurationSettings("test", "all", coll).GetErrorType());
            Console.WriteLine(coll.Count);
            //Console.WriteLine(core.SetConfigurationSettingEnabled("test", "all",
            //       "GENERATE_MN_MAPPING_FOR_NODES", false).GetErrorType());

            core.SetActiveConfiguration("test", "all");

            core.CreateConfiguration("test", "none");

            Result cycleTime = core.CreateObject("test", 240, 0x1006, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "400", "1000");
            Console.WriteLine(cycleTime.GetErrorType());
            Console.WriteLine(cycleTime.GetErrorMessage());

            core.CreateObject("test", 240, 0x1300, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "400", "214");

            core.CreateObject("test", 240, 0x1301, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "400", "5000");

            core.CreateObject("test", 240, 0x1F98, ObjectType.RECORD, "objectName", PlkDataType.UNDEFINED, AccessType.UNDEFINED, PDOMapping.UNDEFINED, "", "");
            core.CreateSubObject("test", 240, 0x1F98, 0, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "3", "3");
            core.CreateSubObject("test", 240, 0x1F98, 7, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "10", "");
            core.CreateSubObject("test", 240, 0x1F98, 8, ObjectType.VAR, "testName", PlkDataType.UNSIGNED16, AccessType.RW, PDOMapping.DEFAULT, "1500", "300");
            core.CreateSubObject("test", 240, 0x1F98, 9, ObjectType.VAR, "testName", PlkDataType.UNSIGNED16, AccessType.RW, PDOMapping.DEFAULT, "2", "10");
            core.CreateSubObject("test", 240, 0x1F98, 5, ObjectType.VAR, "testName", PlkDataType.UNSIGNED16, AccessType.RW, PDOMapping.DEFAULT, "", "");

            core.SetSubObjectLimits("test", 240, 0x1f98, 0x9, "", "");

            core.CreateObject("test", 240, 0x1F81, ObjectType.RECORD, "objectName", PlkDataType.UNDEFINED, AccessType.UNDEFINED, PDOMapping.UNDEFINED, "", "");
            core.CreateSubObject("test", 240, 0x1F81, 0, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "3", "3");
            core.CreateSubObject("test", 240, 0x1F81, 1, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 240, 0x1F81, 2, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 240, 0x1F81, 5, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 240, 0x1F81, 10, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 240, 0x1F81, 20, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 240, 0x1F81, 240, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 240, 0x1F81, 241, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "0");

            core.CreateObject("test", 240, 0x1F8B, ObjectType.RECORD, "objectName", PlkDataType.UNDEFINED, AccessType.UNDEFINED, PDOMapping.UNDEFINED, "", "");
            core.CreateSubObject("test", 240, 0x1F8B, 0, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "3", "3");
            core.CreateSubObject("test", 240, 0x1F8B, 1, ObjectType.VAR, "testName", PlkDataType.UNSIGNED16, AccessType.RW, PDOMapping.DEFAULT, "0", "");
            core.CreateSubObject("test", 240, 0x1F8B, 2, ObjectType.VAR, "testName", PlkDataType.UNSIGNED16, AccessType.RW, PDOMapping.DEFAULT, "0", "");
            core.CreateSubObject("test", 240, 0x1F8B, 5, ObjectType.VAR, "testName", PlkDataType.UNSIGNED16, AccessType.RW, PDOMapping.DEFAULT, "0", "");
            core.CreateSubObject("test", 240, 0x1F8B, 10, ObjectType.VAR, "testName", PlkDataType.UNSIGNED16, AccessType.RW, PDOMapping.DEFAULT, "0", "");
            core.CreateSubObject("test", 240, 0x1F8B, 20, ObjectType.VAR, "testName", PlkDataType.UNSIGNED16, AccessType.RW, PDOMapping.DEFAULT, "0", "");
            core.CreateSubObject("test", 240, 0x1F8B, 241, ObjectType.VAR, "testName", PlkDataType.UNSIGNED16, AccessType.RW, PDOMapping.DEFAULT, "0", "");

            core.CreateObject("test", 240, 0x1F8D, ObjectType.RECORD, "objectName", PlkDataType.UNDEFINED, AccessType.UNDEFINED, PDOMapping.UNDEFINED, "", "");
            core.CreateSubObject("test", 240, 0x1F8D, 0, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "3", "3");
            core.CreateSubObject("test", 240, 0x1F8D, 1, ObjectType.VAR, "testName", PlkDataType.UNSIGNED16, AccessType.RW, PDOMapping.DEFAULT, "0", "");
            core.CreateSubObject("test", 240, 0x1F8D, 2, ObjectType.VAR, "testName", PlkDataType.UNSIGNED16, AccessType.RW, PDOMapping.DEFAULT, "0", "");
            core.CreateSubObject("test", 240, 0x1F8D, 5, ObjectType.VAR, "testName", PlkDataType.UNSIGNED16, AccessType.RW, PDOMapping.DEFAULT, "0", "");
            core.CreateSubObject("test", 240, 0x1F8D, 10, ObjectType.VAR, "testName", PlkDataType.UNSIGNED16, AccessType.RW, PDOMapping.DEFAULT, "0", "");
            core.CreateSubObject("test", 240, 0x1F8D, 20, ObjectType.VAR, "testName", PlkDataType.UNSIGNED16, AccessType.RW, PDOMapping.DEFAULT, "0", "");
            core.CreateSubObject("test", 240, 0x1F8D, 0xF1, ObjectType.VAR, "testName", PlkDataType.UNSIGNED16, AccessType.RW, PDOMapping.DEFAULT, "0", "");

            core.CreateObject("test", 240, 0x1F9B, ObjectType.RECORD, "objectName", PlkDataType.UNDEFINED, AccessType.UNDEFINED, PDOMapping.UNDEFINED, "", "");
            core.CreateSubObject("test", 240, 0x1F9B, 0, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "3", "3");
            core.CreateSubObject("test", 240, 0x1F9B, 1, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 240, 0x1F9B, 2, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 240, 0x1F9B, 5, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 240, 0x1F9B, 10, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 240, 0x1F9B, 20, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 240, 0x1F9B, 241, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "0", "0");


            core.CreateObject("test", 240, 0x1F92, ObjectType.RECORD, "objectName", PlkDataType.UNDEFINED, AccessType.UNDEFINED, PDOMapping.UNDEFINED, "", "");
            core.CreateSubObject("test", 240, 0x1F92, 0, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "5", "");
            core.CreateSubObject("test", 240, 0x1F92, 1, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "25000", "");
            core.CreateSubObject("test", 240, 0x1F92, 2, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "25000", "");
            core.CreateSubObject("test", 240, 0x1F92, 5, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "25000", "");
            core.CreateSubObject("test", 240, 0x1F92, 10, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "25000", "");
            core.CreateSubObject("test", 240, 0x1F92, 20, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "25000", "");
            core.CreateSubObject("test", 240, 0x1F92, 241, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "25000", "");

            core.CreateObject("test", 240, 0x1400, ObjectType.RECORD, "objectName", PlkDataType.UNDEFINED, AccessType.UNDEFINED, PDOMapping.UNDEFINED, "", "");
            core.CreateSubObject("test", 240, 0x1400, 0, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "2", "2");
            core.CreateSubObject("test", 240, 0x1400, 1, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 240, 0x1400, 2, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "0", "0");

            core.CreateObject("test", 240, 0x1600, ObjectType.RECORD, "objectName", PlkDataType.UNDEFINED, AccessType.UNDEFINED, PDOMapping.UNDEFINED, "", "");
            core.CreateSubObject("test", 240, 0x1600, 0, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "0", "1");
            core.CreateSubObject("test", 240, 0x1600, 1, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 240, 0x1600, 2, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 240, 0x1600, 3, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 240, 0x1600, 4, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 240, 0x1600, 5, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 240, 0x1600, 6, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 240, 0x1600, 7, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 240, 0x1600, 8, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 240, 0x1600, 9, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 240, 0x1600, 10, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 240, 0x1600, 11, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 240, 0x1600, 12, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 240, 0x1600, 13, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 240, 0x1600, 14, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 240, 0x1600, 15, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");

            core.CreateObject("test", 240, 0x1401, ObjectType.RECORD, "objectName", PlkDataType.UNDEFINED, AccessType.UNDEFINED, PDOMapping.UNDEFINED, "", "");
            core.CreateSubObject("test", 240, 0x1401, 0, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "2", "2");
            core.CreateSubObject("test", 240, 0x1401, 1, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 240, 0x1401, 2, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "0", "0");

            core.CreateObject("test", 240, 0x1601, ObjectType.RECORD, "objectName", PlkDataType.UNDEFINED, AccessType.UNDEFINED, PDOMapping.UNDEFINED, "", "");
            core.CreateSubObject("test", 240, 0x1601, 0, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "0", "1");
            core.CreateSubObject("test", 240, 0x1601, 1, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 240, 0x1601, 2, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 240, 0x1601, 3, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 240, 0x1601, 4, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 240, 0x1601, 5, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 240, 0x1601, 6, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 240, 0x1601, 7, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 240, 0x1601, 8, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 240, 0x1601, 9, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 240, 0x1601, 10, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 240, 0x1601, 11, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 240, 0x1601, 12, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 240, 0x1601, 13, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 240, 0x1601, 14, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 240, 0x1601, 15, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");

            core.CreateObject("test", 240, 0x1800, ObjectType.RECORD, "objectName", PlkDataType.UNDEFINED, AccessType.UNDEFINED, PDOMapping.UNDEFINED, "", "");
            core.CreateSubObject("test", 240, 0x1800, 0, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "2", "2");
            core.CreateSubObject("test", 240, 0x1800, 1, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 240, 0x1800, 2, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "0", "");

            core.CreateObject("test", 240, 0x1A00, ObjectType.RECORD, "objectName", PlkDataType.UNDEFINED, AccessType.UNDEFINED, PDOMapping.UNDEFINED, "", "");
            core.CreateSubObject("test", 240, 0x1A00, 0, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "0", "1");
            core.CreateSubObject("test", 240, 0x1A00, 1, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 240, 0x1A00, 2, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 240, 0x1A00, 3, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 240, 0x1A00, 4, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 240, 0x1A00, 5, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 240, 0x1A00, 6, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 240, 0x1A00, 7, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 240, 0x1A00, 8, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 240, 0x1A00, 9, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 240, 0x1A00, 10, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 240, 0x1A00, 11, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 240, 0x1A00, 12, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 240, 0x1A00, 13, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 240, 0x1A00, 14, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 240, 0x1A00, 15, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");

            core.CreateObject("test", 240, 0x1801, ObjectType.RECORD, "objectName", PlkDataType.UNDEFINED, AccessType.UNDEFINED, PDOMapping.UNDEFINED, "", "");
            core.CreateSubObject("test", 240, 0x1801, 0, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "2", "2");
            core.CreateSubObject("test", 240, 0x1801, 1, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 240, 0x1801, 2, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "0", "");

            core.CreateObject("test", 240, 0x1A01, ObjectType.RECORD, "objectName", PlkDataType.UNDEFINED, AccessType.UNDEFINED, PDOMapping.UNDEFINED, "", "");
            core.CreateSubObject("test", 240, 0x1A01, 0, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "0", "1");
            core.CreateSubObject("test", 240, 0x1A01, 1, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 240, 0x1A01, 2, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 240, 0x1A01, 3, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 240, 0x1A01, 4, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 240, 0x1A01, 5, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 240, 0x1A01, 6, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 240, 0x1A01, 7, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 240, 0x1A01, 8, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 240, 0x1A01, 9, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 240, 0x1A01, 10, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 240, 0x1A01, 11, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 240, 0x1A01, 12, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 240, 0x1A01, 13, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 240, 0x1A01, 14, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 240, 0x1A01, 15, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 240, 0x1A01, 16, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 240, 0x1A01, 17, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 240, 0x1A01, 18, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 240, 0x1A01, 19, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 240, 0x1A01, 20, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");

            core.CreateObject("test", 240, 0x1F26, ObjectType.RECORD, "objectName", PlkDataType.UNDEFINED, AccessType.UNDEFINED, PDOMapping.UNDEFINED, "", "");
            core.CreateSubObject("test", 240, 0x1F26, 0, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "4", "4");
            core.CreateSubObject("test", 240, 0x1F26, 1, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 240, 0x1F26, 5, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 240, 0x1F26, 10, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 240, 0x1F26, 20, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 240, 0x1F26, 241, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "0");

            core.CreateObject("test", 240, 0x1F27, ObjectType.RECORD, "objectName", PlkDataType.UNDEFINED, AccessType.UNDEFINED, PDOMapping.UNDEFINED, "", "");
            core.CreateSubObject("test", 240, 0x1F27, 0, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "4", "4");
            core.CreateSubObject("test", 240, 0x1F27, 1, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 240, 0x1F27, 5, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 240, 0x1F27, 10, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 240, 0x1F27, 20, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 240, 0x1F27, 241, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "0");

            core.CreateObject("test", 240, 0x1C09, ObjectType.RECORD, "objectName", PlkDataType.UNDEFINED, AccessType.UNDEFINED, PDOMapping.UNDEFINED, "", "");
            core.CreateSubObject("test", 240, 0x1C09, 0, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "3", "3");
            core.CreateSubObject("test", 240, 0x1C09, 1, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 240, 0x1C09, 2, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 240, 0x1C09, 5, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 240, 0x1C09, 10, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 240, 0x1C09, 20, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 240, 0x1C09, 240, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 240, 0x1C09, 241, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "0");

            core.SetFeatureValue("test", 240, GeneralFeatureEnum.PDOTPDOChannelObjects, "10");
            core.SetFeatureValue("test", 240, GeneralFeatureEnum.PDORPDOChannelObjects, "10");

            core.CreateNode("test", 241, "RMN");
            core.CreateObject("test", 241, 0x1006, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "400", "1000");
            core.CreateObject("test", 241, 0x1300, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "400", "214");
            core.CreateObject("test", 241, 0x1301, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "400", "5000");

            core.CreateObject("test", 241, 0x1F98, ObjectType.RECORD, "objectName", PlkDataType.UNDEFINED, AccessType.UNDEFINED, PDOMapping.UNDEFINED, "", "");
            core.CreateSubObject("test", 241, 0x1F98, 0, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "3", "3");
            core.CreateSubObject("test", 241, 0x1F98, 7, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "10", "");
            core.CreateSubObject("test", 241, 0x1F98, 8, ObjectType.VAR, "testName", PlkDataType.UNSIGNED16, AccessType.RW, PDOMapping.DEFAULT, "1500", "300");
            core.CreateSubObject("test", 241, 0x1F98, 9, ObjectType.VAR, "testName", PlkDataType.UNSIGNED16, AccessType.RW, PDOMapping.DEFAULT, "2", "10");
            core.CreateSubObject("test", 241, 0x1F98, 5, ObjectType.VAR, "testName", PlkDataType.UNSIGNED16, AccessType.RW, PDOMapping.DEFAULT, "", "");
            core.CreateSubObject("test", 241, 0x1F98, 4, ObjectType.VAR, "testName", PlkDataType.UNSIGNED16, AccessType.RW, PDOMapping.DEFAULT, "36", "");

            core.SetSubObjectLimits("test", 241, 0x1f98, 0x9, "", "");

            core.CreateObject("test", 241, 0x1C09, ObjectType.RECORD, "objectName", PlkDataType.UNDEFINED, AccessType.UNDEFINED, PDOMapping.UNDEFINED, "", "");
            core.CreateSubObject("test", 241, 0x1C09, 0, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "3", "3");
            core.CreateSubObject("test", 241, 0x1C09, 1, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 241, 0x1C09, 2, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 241, 0x1C09, 5, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 241, 0x1C09, 10, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 241, 0x1C09, 20, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 241, 0x1C09, 240, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 241, 0x1C09, 241, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "0");

            core.CreateObject("test", 241, 0x1F81, ObjectType.RECORD, "objectName", PlkDataType.UNDEFINED, AccessType.UNDEFINED, PDOMapping.UNDEFINED, "", "");
            core.CreateSubObject("test", 241, 0x1F81, 0, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "3", "3");
            core.CreateSubObject("test", 241, 0x1F81, 1, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 241, 0x1F81, 2, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 241, 0x1F81, 5, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 241, 0x1F81, 10, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 241, 0x1F81, 20, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 241, 0x1F81, 240, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 241, 0x1F81, 241, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "0");

            core.CreateObject("test", 241, 0x1F8B, ObjectType.RECORD, "objectName", PlkDataType.UNDEFINED, AccessType.UNDEFINED, PDOMapping.UNDEFINED, "", "");
            core.CreateSubObject("test", 241, 0x1F8B, 0, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "3", "3");
            core.CreateSubObject("test", 241, 0x1F8B, 1, ObjectType.VAR, "testName", PlkDataType.UNSIGNED16, AccessType.RW, PDOMapping.DEFAULT, "36", "");
            core.CreateSubObject("test", 241, 0x1F8B, 2, ObjectType.VAR, "testName", PlkDataType.UNSIGNED16, AccessType.RW, PDOMapping.DEFAULT, "36", "");
            core.CreateSubObject("test", 241, 0x1F8B, 5, ObjectType.VAR, "testName", PlkDataType.UNSIGNED16, AccessType.RW, PDOMapping.DEFAULT, "36", "");
            core.CreateSubObject("test", 241, 0x1F8B, 10, ObjectType.VAR, "testName", PlkDataType.UNSIGNED16, AccessType.RW, PDOMapping.DEFAULT, "0", "");
            core.CreateSubObject("test", 241, 0x1F8B, 20, ObjectType.VAR, "testName", PlkDataType.UNSIGNED16, AccessType.RW, PDOMapping.DEFAULT, "36", "");
            core.CreateSubObject("test", 241, 0x1F8B, 241, ObjectType.VAR, "testName", PlkDataType.UNSIGNED16, AccessType.RW, PDOMapping.DEFAULT, "36", "");

            core.CreateObject("test", 241, 0x1F8D, ObjectType.RECORD, "objectName", PlkDataType.UNDEFINED, AccessType.UNDEFINED, PDOMapping.UNDEFINED, "", "");
            core.CreateSubObject("test", 241, 0x1F8D, 0, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "3", "3");
            core.CreateSubObject("test", 241, 0x1F8D, 1, ObjectType.VAR, "testName", PlkDataType.UNSIGNED16, AccessType.RW, PDOMapping.DEFAULT, "36", "");
            core.CreateSubObject("test", 241, 0x1F8D, 2, ObjectType.VAR, "testName", PlkDataType.UNSIGNED16, AccessType.RW, PDOMapping.DEFAULT, "36", "");
            core.CreateSubObject("test", 241, 0x1F8D, 5, ObjectType.VAR, "testName", PlkDataType.UNSIGNED16, AccessType.RW, PDOMapping.DEFAULT, "36", "");
            core.CreateSubObject("test", 241, 0x1F8D, 10, ObjectType.VAR, "testName", PlkDataType.UNSIGNED16, AccessType.RW, PDOMapping.DEFAULT, "0", "");
            core.CreateSubObject("test", 241, 0x1F8D, 20, ObjectType.VAR, "testName", PlkDataType.UNSIGNED16, AccessType.RW, PDOMapping.DEFAULT, "36", "");
            core.CreateSubObject("test", 241, 0x1F8D, 0xF0, ObjectType.VAR, "testName", PlkDataType.UNSIGNED16, AccessType.RW, PDOMapping.DEFAULT, "36", "");
            core.CreateSubObject("test", 241, 0x1F8D, 0xF1, ObjectType.VAR, "testName", PlkDataType.UNSIGNED16, AccessType.RW, PDOMapping.DEFAULT, "36", "");

            core.CreateObject("test", 241, 0x1F9B, ObjectType.RECORD, "objectName", PlkDataType.UNDEFINED, AccessType.UNDEFINED, PDOMapping.UNDEFINED, "", "");
            core.CreateSubObject("test", 241, 0x1F9B, 0, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "3", "3");
            core.CreateSubObject("test", 241, 0x1F9B, 1, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 241, 0x1F9B, 2, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 241, 0x1F98, 3, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 241, 0x1F9B, 5, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 241, 0x1F9B, 10, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 241, 0x1F9B, 20, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 241, 0x1F9B, 241, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "0", "0");


            core.CreateObject("test", 241, 0x1F92, ObjectType.RECORD, "objectName", PlkDataType.UNDEFINED, AccessType.UNDEFINED, PDOMapping.UNDEFINED, "", "");
            core.CreateSubObject("test", 241, 0x1F92, 0, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "5", "");
            core.CreateSubObject("test", 241, 0x1F92, 1, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "25000", "");
            core.CreateSubObject("test", 241, 0x1F92, 2, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "25000", "");
            core.CreateSubObject("test", 241, 0x1F92, 5, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "25000", "");
            core.CreateSubObject("test", 241, 0x1F92, 10, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "25000", "");
            core.CreateSubObject("test", 241, 0x1F92, 20, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "25000", "");
            core.CreateSubObject("test", 241, 0x1F92, 241, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "25000", "");

            core.CreateObject("test", 241, 0x1400, ObjectType.RECORD, "objectName", PlkDataType.UNDEFINED, AccessType.UNDEFINED, PDOMapping.UNDEFINED, "", "");
            core.CreateSubObject("test", 241, 0x1400, 0, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "2", "2");
            core.CreateSubObject("test", 241, 0x1400, 1, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 241, 0x1400, 2, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "0", "0");

            core.CreateObject("test", 241, 0x1600, ObjectType.RECORD, "objectName", PlkDataType.UNDEFINED, AccessType.UNDEFINED, PDOMapping.UNDEFINED, "", "");
            core.CreateSubObject("test", 241, 0x1600, 0, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "0", "1");
            core.CreateSubObject("test", 241, 0x1600, 1, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 241, 0x1600, 2, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 241, 0x1600, 3, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 241, 0x1600, 4, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 241, 0x1600, 5, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 241, 0x1600, 6, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 241, 0x1600, 7, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 241, 0x1600, 8, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 241, 0x1600, 9, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 241, 0x1600, 10, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 241, 0x1600, 11, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 241, 0x1600, 12, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 241, 0x1600, 13, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 241, 0x1600, 14, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 241, 0x1600, 15, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");

            core.CreateObject("test", 241, 0x1401, ObjectType.RECORD, "objectName", PlkDataType.UNDEFINED, AccessType.UNDEFINED, PDOMapping.UNDEFINED, "", "");
            core.CreateSubObject("test", 241, 0x1401, 0, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "2", "2");
            core.CreateSubObject("test", 241, 0x1401, 1, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 241, 0x1401, 2, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "0", "0");

            core.CreateObject("test", 241, 0x1601, ObjectType.RECORD, "objectName", PlkDataType.UNDEFINED, AccessType.UNDEFINED, PDOMapping.UNDEFINED, "", "");
            core.CreateSubObject("test", 241, 0x1601, 0, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "0", "1");
            core.CreateSubObject("test", 241, 0x1601, 1, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 241, 0x1601, 2, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 241, 0x1601, 3, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 241, 0x1601, 4, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 241, 0x1601, 5, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 241, 0x1601, 6, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 241, 0x1601, 7, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 241, 0x1601, 8, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 241, 0x1601, 9, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 241, 0x1601, 10, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 241, 0x1601, 11, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 241, 0x1601, 12, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 241, 0x1601, 13, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 241, 0x1601, 14, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 241, 0x1601, 15, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");

            core.CreateObject("test", 241, 0x1800, ObjectType.RECORD, "objectName", PlkDataType.UNDEFINED, AccessType.UNDEFINED, PDOMapping.UNDEFINED, "", "");
            core.CreateSubObject("test", 241, 0x1800, 0, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "2", "2");
            core.CreateSubObject("test", 241, 0x1800, 1, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 241, 0x1800, 2, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "0", "");

            core.CreateObject("test", 241, 0x1A00, ObjectType.RECORD, "objectName", PlkDataType.UNDEFINED, AccessType.UNDEFINED, PDOMapping.UNDEFINED, "", "");
            core.CreateSubObject("test", 241, 0x1A00, 0, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "0", "1");
            core.CreateSubObject("test", 241, 0x1A00, 1, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 241, 0x1A00, 2, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 241, 0x1A00, 3, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 241, 0x1A00, 4, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 241, 0x1A00, 5, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 241, 0x1A00, 6, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 241, 0x1A00, 7, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 241, 0x1A00, 8, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 241, 0x1A00, 9, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 241, 0x1A00, 10, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 241, 0x1A00, 11, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 241, 0x1A00, 12, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 241, 0x1A00, 13, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 241, 0x1A00, 14, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 241, 0x1A00, 15, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");

            core.CreateObject("test", 241, 0x1801, ObjectType.RECORD, "objectName", PlkDataType.UNDEFINED, AccessType.UNDEFINED, PDOMapping.UNDEFINED, "", "");
            core.CreateSubObject("test", 241, 0x1801, 0, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "2", "2");
            core.CreateSubObject("test", 241, 0x1801, 1, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 241, 0x1801, 2, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "0", "");

            core.CreateObject("test", 241, 0x1A01, ObjectType.RECORD, "objectName", PlkDataType.UNDEFINED, AccessType.UNDEFINED, PDOMapping.UNDEFINED, "", "");
            core.CreateSubObject("test", 241, 0x1A01, 0, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "0", "1");
            core.CreateSubObject("test", 241, 0x1A01, 1, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 241, 0x1A01, 2, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 241, 0x1A01, 3, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 241, 0x1A01, 4, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 241, 0x1A01, 5, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 241, 0x1A01, 6, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 241, 0x1A01, 7, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 241, 0x1A01, 8, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 241, 0x1A01, 9, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 241, 0x1A01, 10, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 241, 0x1A01, 11, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 241, 0x1A01, 12, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 241, 0x1A01, 13, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 241, 0x1A01, 14, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 241, 0x1A01, 15, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 241, 0x1A01, 16, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 241, 0x1A01, 17, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 241, 0x1A01, 18, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 241, 0x1A01, 19, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 241, 0x1A01, 20, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");

            core.CreateObject("test", 241, 0x1F26, ObjectType.RECORD, "objectName", PlkDataType.UNDEFINED, AccessType.UNDEFINED, PDOMapping.UNDEFINED, "", "");
            core.CreateSubObject("test", 241, 0x1F26, 0, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "4", "4");
            core.CreateSubObject("test", 241, 0x1F26, 1, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 241, 0x1F26, 5, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 241, 0x1F26, 10, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 241, 0x1F26, 20, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 241, 0x1F26, 241, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "0");

            core.CreateObject("test", 241, 0x1F27, ObjectType.RECORD, "objectName", PlkDataType.UNDEFINED, AccessType.UNDEFINED, PDOMapping.UNDEFINED, "", "");
            core.CreateSubObject("test", 241, 0x1F27, 0, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "4", "4");
            core.CreateSubObject("test", 241, 0x1F27, 1, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 241, 0x1F27, 5, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 241, 0x1F27, 10, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 241, 0x1F27, 20, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 241, 0x1F27, 241, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "0");

            core.CreateObject("test", 241, 0x1020, ObjectType.RECORD, "objectName", PlkDataType.UNDEFINED, AccessType.UNDEFINED, PDOMapping.UNDEFINED, "", "");
            core.CreateSubObject("test", 241, 0x1020, 0, ObjectType.DEFTYPE, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "2", "2");
            core.CreateSubObject("test", 241, 0x1020, 1, ObjectType.DEFTYPE, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 241, 0x1020, 2, ObjectType.DEFTYPE, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "0");

            core.SetCycleTime("test", 10000); // Set Cycle time

            core.CreateNode("test", 10, "SuperControlledNode");
            core.CreateObject("test", 10, 0x1020, ObjectType.RECORD, "objectName", PlkDataType.UNDEFINED, AccessType.UNDEFINED, PDOMapping.UNDEFINED, "", "");
            core.CreateSubObject("test", 10, 0x1020, 0, ObjectType.DEFTYPE, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "2", "2");
            core.CreateSubObject("test", 10, 0x1020, 1, ObjectType.DEFTYPE, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 10, 0x1020, 2, ObjectType.DEFTYPE, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "0");

            core.CreateObject("test", 10, 0x1C0B, ObjectType.RECORD, "objectName", PlkDataType.UNDEFINED, AccessType.UNDEFINED, PDOMapping.UNDEFINED, "", "");
            core.CreateSubObject("test", 10, 0x1C0B, 3, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "");
            core.CreateObject("test", 10, 0x1C0C, ObjectType.RECORD, "objectName", PlkDataType.UNDEFINED, AccessType.UNDEFINED, PDOMapping.UNDEFINED, "", "");
            core.CreateSubObject("test", 10, 0x1C0C, 3, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "");
            core.CreateObject("test", 10, 0x1C0D, ObjectType.RECORD, "objectName", PlkDataType.UNDEFINED, AccessType.UNDEFINED, PDOMapping.UNDEFINED, "", "");
            core.CreateSubObject("test", 10, 0x1C0D, 3, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "");

            core.CreateObject("test", 10, 0x1F81, ObjectType.RECORD, "objectName", PlkDataType.UNDEFINED, AccessType.UNDEFINED, PDOMapping.UNDEFINED, "", "");
            core.CreateSubObject("test", 10, 0x1F81, 0, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "3", "3");
            core.CreateSubObject("test", 10, 0x1F81, 1, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 10, 0x1F81, 2, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 10, 0x1F81, 5, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 10, 0x1F81, 10, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 10, 0x1F81, 20, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 10, 0x1F81, 240, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 10, 0x1F81, 241, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "0");

            core.CreateObject("test", 10, 0x1400, ObjectType.RECORD, "objectName", PlkDataType.UNDEFINED, AccessType.UNDEFINED, PDOMapping.UNDEFINED, "", "");
            core.CreateSubObject("test", 10, 0x1400, 0, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "2", "2");
            core.CreateSubObject("test", 10, 0x1400, 1, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "0", "");
            core.CreateSubObject("test", 10, 0x1400, 2, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "0", "");

            core.CreateObject("test", 10, 0x1600, ObjectType.RECORD, "objectName", PlkDataType.UNDEFINED, AccessType.UNDEFINED, PDOMapping.UNDEFINED, "", "");
            core.CreateSubObject("test", 10, 0x1600, 0, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 10, 0x1600, 1, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 10, 0x1600, 2, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 10, 0x1600, 3, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 10, 0x1600, 4, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 10, 0x1600, 5, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 10, 0x1600, 6, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 10, 0x1600, 7, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 10, 0x1600, 8, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 10, 0x1600, 9, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 10, 0x1600, 10, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 10, 0x1600, 11, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 10, 0x1600, 12, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");

            core.CreateObject("test", 10, 0x1401, ObjectType.RECORD, "objectName", PlkDataType.UNDEFINED, AccessType.UNDEFINED, PDOMapping.UNDEFINED, "", "");
            core.CreateSubObject("test", 10, 0x1401, 0, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "2", "2");
            core.CreateSubObject("test", 10, 0x1401, 1, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "0", "");
            core.CreateSubObject("test", 10, 0x1401, 2, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "0", "");

            core.CreateObject("test", 10, 0x1601, ObjectType.RECORD, "objectName", PlkDataType.UNDEFINED, AccessType.UNDEFINED, PDOMapping.UNDEFINED, "", "");
            core.CreateSubObject("test", 10, 0x1601, 0, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 10, 0x1601, 1, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 10, 0x1601, 2, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 10, 0x1601, 3, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 10, 0x1601, 4, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 10, 0x1601, 5, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 10, 0x1601, 6, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 10, 0x1601, 7, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 10, 0x1601, 8, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 10, 0x1601, 9, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 10, 0x1601, 10, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 10, 0x1601, 11, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 10, 0x1601, 12, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");

            core.CreateObject("test", 10, 0x1800, ObjectType.RECORD, "objectName", PlkDataType.UNDEFINED, AccessType.UNDEFINED, PDOMapping.UNDEFINED, "", "");
            core.CreateSubObject("test", 10, 0x1800, 0, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "2", "2");
            core.CreateSubObject("test", 10, 0x1800, 1, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "0", "");
            core.CreateSubObject("test", 10, 0x1800, 2, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "0", "");

            core.CreateObject("test", 10, 0x1F8D, ObjectType.RECORD, "objectName", PlkDataType.UNDEFINED, AccessType.UNDEFINED, PDOMapping.UNDEFINED, "", "");
            core.CreateSubObject("test", 10, 0x1F8D, 0, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "3", "3");
            core.CreateSubObject("test", 10, 0x1F8D, 1, ObjectType.VAR, "testName", PlkDataType.UNSIGNED16, AccessType.RW, PDOMapping.DEFAULT, "36", "");
            core.CreateSubObject("test", 10, 0x1F8D, 2, ObjectType.VAR, "testName", PlkDataType.UNSIGNED16, AccessType.RW, PDOMapping.DEFAULT, "36", "");
            core.CreateSubObject("test", 10, 0x1F8D, 5, ObjectType.VAR, "testName", PlkDataType.UNSIGNED16, AccessType.RW, PDOMapping.DEFAULT, "36", "");
            core.CreateSubObject("test", 10, 0x1F8D, 10, ObjectType.VAR, "testName", PlkDataType.UNSIGNED16, AccessType.RW, PDOMapping.DEFAULT, "0", "");
            core.CreateSubObject("test", 10, 0x1F8D, 20, ObjectType.VAR, "testName", PlkDataType.UNSIGNED16, AccessType.RW, PDOMapping.DEFAULT, "36", "");
            core.CreateSubObject("test", 10, 0x1F8D, 0xF0, ObjectType.VAR, "testName", PlkDataType.UNSIGNED16, AccessType.RW, PDOMapping.DEFAULT, "36", "");
            core.CreateSubObject("test", 10, 0x1F8D, 0xF1, ObjectType.VAR, "testName", PlkDataType.UNSIGNED16, AccessType.RW, PDOMapping.DEFAULT, "36", "");

            core.CreateObject("test", 10, 0x1A00, ObjectType.RECORD, "objectName", PlkDataType.UNDEFINED, AccessType.UNDEFINED, PDOMapping.UNDEFINED, "", "");
            core.CreateSubObject("test", 10, 0x1A00, 0, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 10, 0x1A00, 1, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 10, 0x1A00, 2, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 10, 0x1A00, 3, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 10, 0x1A00, 4, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 10, 0x1A00, 5, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 10, 0x1A00, 6, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 10, 0x1A00, 7, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 10, 0x1A00, 8, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 10, 0x1A00, 9, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 10, 0x1A00, 10, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");

            core.CreateObject("test", 10, 0x2104, ObjectType.RECORD, "objectName", PlkDataType.UNDEFINED, AccessType.UNDEFINED, PDOMapping.UNDEFINED, "", "");
            core.CreateSubObject("test", 10, 0x2104, 0, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RO, PDOMapping.DEFAULT, "0", "10");
            core.CreateSubObject("test", 10, 0x2104, 1, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.RPDO, "0", "0x10");
            core.CreateSubObject("test", 10, 0x2104, 2, ObjectType.VAR, "testName", PlkDataType.UNSIGNED16, AccessType.RW, PDOMapping.RPDO, "0", "0x100");
            core.CreateSubObject("test", 10, 0x2104, 3, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.RPDO, "0", "0x1000");
            core.CreateSubObject("test", 10, 0x2104, 4, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.RPDO, "0", "0x10000");
            core.CreateSubObject("test", 10, 0x2104, 5, ObjectType.VAR, "testName", PlkDataType.INTEGER8, AccessType.RW, PDOMapping.RPDO, "0", "0x10");
            core.CreateSubObject("test", 10, 0x2104, 6, ObjectType.VAR, "testName", PlkDataType.INTEGER16, AccessType.RW, PDOMapping.RPDO, "0", "0x100");
            core.CreateSubObject("test", 10, 0x2104, 7, ObjectType.VAR, "testName", PlkDataType.INTEGER32, AccessType.RW, PDOMapping.RPDO, "0", "0x1000");
            core.CreateSubObject("test", 10, 0x2104, 8, ObjectType.VAR, "testName", PlkDataType.INTEGER64, AccessType.RW, PDOMapping.RPDO, "0", "0x10000");

            core.CreateObject("test", 10, 0x2105, ObjectType.RECORD, "objectName", PlkDataType.UNDEFINED, AccessType.UNDEFINED, PDOMapping.UNDEFINED, "", "");
            core.CreateSubObject("test", 10, 0x2105, 0, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RO, PDOMapping.DEFAULT, "0", "10");
            core.CreateSubObject("test", 10, 0x2105, 1, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RO, PDOMapping.TPDO, "0", "0x10");
            core.CreateSubObject("test", 10, 0x2105, 2, ObjectType.VAR, "testName", PlkDataType.UNSIGNED16, AccessType.RO, PDOMapping.TPDO, "0", "0x100");
            core.CreateSubObject("test", 10, 0x2105, 3, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RO, PDOMapping.TPDO, "0", "0x1000");
            core.CreateSubObject("test", 10, 0x2105, 4, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RO, PDOMapping.TPDO, "0", "0x10000");
            core.CreateSubObject("test", 10, 0x2105, 5, ObjectType.VAR, "testName", PlkDataType.INTEGER8, AccessType.RO, PDOMapping.TPDO, "0", "0x10");
            core.CreateSubObject("test", 10, 0x2105, 6, ObjectType.VAR, "testName", PlkDataType.INTEGER16, AccessType.RO, PDOMapping.TPDO, "0", "0x100");
            core.CreateSubObject("test", 10, 0x2105, 7, ObjectType.VAR, "testName", PlkDataType.INTEGER32, AccessType.RO, PDOMapping.TPDO, "0", "0x1000");
            core.CreateSubObject("test", 10, 0x2105, 8, ObjectType.VAR, "testName", PlkDataType.INTEGER64, AccessType.RO, PDOMapping.TPDO, "0", "0x10000");

            core.CreateObject("test", 10, 0x1F9B, ObjectType.RECORD, "objectName", PlkDataType.UNDEFINED, AccessType.UNDEFINED, PDOMapping.UNDEFINED, "", "");
            core.CreateSubObject("test", 10, 0x1F9B, 0, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "3", "3");
            core.CreateSubObject("test", 10, 0x1F9B, 1, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 10, 0x1F9B, 2, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 10, 0x1F9B, 5, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 10, 0x1F9B, 10, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 10, 0x1F9B, 20, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 10, 0x1F9B, 241, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "0", "0");

            core.CreateObject("test", 10, 0x1300, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "400", "5000");

            core.CreateObject("test", 10, 0x1301, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "400", "5000");

            core.CreateObject("test", 10, 0x1006, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "400", "10000");

            core.CreateObject("test", 10, 0x1F98, ObjectType.RECORD, "objectName", PlkDataType.UNDEFINED, AccessType.UNDEFINED, PDOMapping.UNDEFINED, "", "");
            core.CreateSubObject("test", 10, 0x1F98, 0, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "3", "3");
            core.CreateSubObject("test", 10, 0x1F98, 4, ObjectType.VAR, "testName", PlkDataType.UNSIGNED16, AccessType.RW, PDOMapping.DEFAULT, "0", "");
            core.CreateSubObject("test", 10, 0x1F98, 5, ObjectType.VAR, "testName", PlkDataType.UNSIGNED16, AccessType.RW, PDOMapping.DEFAULT, "0", "");
            core.CreateSubObject("test", 10, 0x1F98, 7, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 10, 0x1F98, 8, ObjectType.VAR, "testName", PlkDataType.UNSIGNED16, AccessType.RW, PDOMapping.DEFAULT, "0", "0");

            core.CreateSubObject("test", 10, 0x1F98, 3, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "3", "3");

            core.CreateNode("test", 20, "FieldbusBoy");
            core.CreateObject("test", 20, 0x1020, ObjectType.RECORD, "objectName", PlkDataType.UNDEFINED, AccessType.UNDEFINED, PDOMapping.UNDEFINED, "", "");
            core.CreateSubObject("test", 20, 0x1020, 0, ObjectType.DEFTYPE, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "2", "2");
            core.CreateSubObject("test", 20, 0x1020, 1, ObjectType.DEFTYPE, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 20, 0x1020, 2, ObjectType.DEFTYPE, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "0");

            core.CreateObject("test", 20, 0x1C0B, ObjectType.RECORD, "objectName", PlkDataType.UNDEFINED, AccessType.UNDEFINED, PDOMapping.UNDEFINED, "", "");
            core.CreateSubObject("test", 20, 0x1C0B, 3, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "");
            core.CreateObject("test", 20, 0x1C0C, ObjectType.RECORD, "objectName", PlkDataType.UNDEFINED, AccessType.UNDEFINED, PDOMapping.UNDEFINED, "", "");
            core.CreateSubObject("test", 20, 0x1C0C, 3, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "");
            core.CreateObject("test", 20, 0x1C0D, ObjectType.RECORD, "objectName", PlkDataType.UNDEFINED, AccessType.UNDEFINED, PDOMapping.UNDEFINED, "", "");
            core.CreateSubObject("test", 20, 0x1C0D, 3, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "");

            core.CreateObject("test", 20, 0x1F8B, ObjectType.RECORD, "objectName", PlkDataType.UNDEFINED, AccessType.UNDEFINED, PDOMapping.UNDEFINED, "", "");
            core.CreateSubObject("test", 20, 0x1F8B, 0, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "3", "3");
            core.CreateSubObject("test", 20, 0x1F8B, 1, ObjectType.VAR, "testName", PlkDataType.UNSIGNED16, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 20, 0x1F8B, 2, ObjectType.VAR, "testName", PlkDataType.UNSIGNED16, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 20, 0x1F8B, 5, ObjectType.VAR, "testName", PlkDataType.UNSIGNED16, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 20, 0x1F8B, 10, ObjectType.VAR, "testName", PlkDataType.UNSIGNED16, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 20, 0x1F8B, 20, ObjectType.VAR, "testName", PlkDataType.UNSIGNED16, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 20, 0x1F8B, 0xF0, ObjectType.VAR, "testName", PlkDataType.UNSIGNED16, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 20, 0x1F8B, 0xF1, ObjectType.VAR, "testName", PlkDataType.UNSIGNED16, AccessType.RW, PDOMapping.DEFAULT, "0", "0");

            core.CreateObject("test", 20, 0x1F9B, ObjectType.RECORD, "objectName", PlkDataType.UNDEFINED, AccessType.UNDEFINED, PDOMapping.UNDEFINED, "", "");
            core.CreateSubObject("test", 20, 0x1F9B, 0, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "3", "3");
            core.CreateSubObject("test", 20, 0x1F9B, 1, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 20, 0x1F9B, 2, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 20, 0x1F9B, 5, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 20, 0x1F9B, 10, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 20, 0x1F9B, 20, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 20, 0x1F9B, 241, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "0", "0");

            core.CreateObject("test", 20, 0x1400, ObjectType.RECORD, "objectName", PlkDataType.UNDEFINED, AccessType.UNDEFINED, PDOMapping.UNDEFINED, "", "");
            core.CreateSubObject("test", 20, 0x1400, 0, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "2", "2");
            core.CreateSubObject("test", 20, 0x1400, 1, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "0", "");
            core.CreateSubObject("test", 20, 0x1400, 2, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "0", "");

            core.CreateObject("test", 20, 0x1600, ObjectType.RECORD, "objectName", PlkDataType.UNDEFINED, AccessType.UNDEFINED, PDOMapping.UNDEFINED, "", "");
            core.CreateSubObject("test", 20, 0x1600, 0, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 20, 0x1600, 1, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 20, 0x1600, 2, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 20, 0x1600, 3, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 20, 0x1600, 4, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 20, 0x1600, 5, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 20, 0x1600, 6, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 20, 0x1600, 7, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 20, 0x1600, 8, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 20, 0x1600, 9, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 20, 0x1600, 10, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 20, 0x1600, 11, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 20, 0x1600, 12, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");

            core.CreateObject("test", 20, 0x1300, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "400", "5000");
            core.CreateObject("test", 20, 0x1301, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "400", "5000");

            core.CreateObject("test", 20, 0x1006, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "400", "10000");

            core.CreateObject("test", 20, 0x1F98, ObjectType.RECORD, "objectName", PlkDataType.UNDEFINED, AccessType.UNDEFINED, PDOMapping.UNDEFINED, "", "");
            core.CreateSubObject("test", 20, 0x1F98, 0, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "3", "3");
            core.CreateSubObject("test", 20, 0x1F98, 4, ObjectType.VAR, "testName", PlkDataType.UNSIGNED16, AccessType.RW, PDOMapping.DEFAULT, "0", "");
            core.CreateSubObject("test", 20, 0x1F98, 7, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 20, 0x1F98, 8, ObjectType.VAR, "testName", PlkDataType.UNSIGNED16, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 20, 0x1F98, 9, ObjectType.VAR, "testName", PlkDataType.UNSIGNED16, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 20, 0x1F98, 5, ObjectType.VAR, "testName", PlkDataType.UNSIGNED16, AccessType.RW, PDOMapping.DEFAULT, "", "");

            core.CreateObject("test", 20, 0x1800, ObjectType.RECORD, "objectName", PlkDataType.UNDEFINED, AccessType.UNDEFINED, PDOMapping.UNDEFINED, "", "");
            core.CreateSubObject("test", 20, 0x1800, 0, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "2", "2");
            core.CreateSubObject("test", 20, 0x1800, 1, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "0", "");
            core.CreateSubObject("test", 20, 0x1800, 2, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "0", "");

            core.CreateObject("test", 20, 0x1A00, ObjectType.RECORD, "objectName", PlkDataType.UNDEFINED, AccessType.UNDEFINED, PDOMapping.UNDEFINED, "", "");
            core.CreateSubObject("test", 20, 0x1A00, 0, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 20, 0x1A00, 1, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 20, 0x1A00, 2, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 20, 0x1A00, 3, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 20, 0x1A00, 4, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 20, 0x1A00, 5, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 20, 0x1A00, 6, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 20, 0x1A00, 7, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 20, 0x1A00, 8, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 20, 0x1A00, 9, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 20, 0x1A00, 10, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0");

            core.CreateObject("test", 20, 0x2105, ObjectType.RECORD, "objectName", PlkDataType.UNDEFINED, AccessType.UNDEFINED, PDOMapping.UNDEFINED, "", "");
            core.CreateSubObject("test", 20, 0x2105, 0, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RO, PDOMapping.DEFAULT, "0", "10");
            core.CreateSubObject("test", 20, 0x2105, 1, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RO, PDOMapping.TPDO, "0", "0x10");
            core.CreateSubObject("test", 20, 0x2105, 2, ObjectType.VAR, "testName", PlkDataType.UNSIGNED16, AccessType.RO, PDOMapping.TPDO, "0", "0x100");
            core.CreateSubObject("test", 20, 0x2105, 3, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RO, PDOMapping.TPDO, "0", "0x1000");
            core.CreateSubObject("test", 20, 0x2105, 4, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RO, PDOMapping.TPDO, "0", "0x10000");
            core.CreateSubObject("test", 20, 0x2105, 5, ObjectType.VAR, "testName", PlkDataType.INTEGER8, AccessType.RO, PDOMapping.TPDO, "0", "0x10");
            core.CreateSubObject("test", 20, 0x2105, 6, ObjectType.VAR, "testName", PlkDataType.INTEGER16, AccessType.RO, PDOMapping.TPDO, "0", "0x100");
            core.CreateSubObject("test", 20, 0x2105, 7, ObjectType.VAR, "testName", PlkDataType.INTEGER32, AccessType.RO, PDOMapping.TPDO, "0", "0x1000");
            core.CreateSubObject("test", 20, 0x2105, 8, ObjectType.VAR, "testName", PlkDataType.INTEGER64, AccessType.RO, PDOMapping.TPDO, "0", "0x10000");

            core.CreateObject("test", 20, 0x2104, ObjectType.RECORD, "objectName", PlkDataType.UNDEFINED, AccessType.UNDEFINED, PDOMapping.UNDEFINED, "", "");
            core.CreateSubObject("test", 20, 0x2104, 0, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RO, PDOMapping.DEFAULT, "0", "10");
            core.CreateSubObject("test", 20, 0x2104, 1, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.RPDO, "0", "0x10");
            core.CreateSubObject("test", 20, 0x2104, 2, ObjectType.VAR, "testName", PlkDataType.UNSIGNED16, AccessType.RW, PDOMapping.RPDO, "0", "0x100");
            core.CreateSubObject("test", 20, 0x2104, 3, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.RPDO, "0", "0x1000");
            core.CreateSubObject("test", 20, 0x2104, 4, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.RPDO, "0", "0x10000");
            core.CreateSubObject("test", 20, 0x2104, 5, ObjectType.VAR, "testName", PlkDataType.INTEGER8, AccessType.RW, PDOMapping.RPDO, "0", "0x10");
            core.CreateSubObject("test", 20, 0x2104, 6, ObjectType.VAR, "testName", PlkDataType.INTEGER16, AccessType.RW, PDOMapping.RPDO, "0", "0x100");
            core.CreateSubObject("test", 20, 0x2104, 7, ObjectType.VAR, "testName", PlkDataType.INTEGER32, AccessType.RW, PDOMapping.RPDO, "0", "0x1000");
            core.CreateSubObject("test", 20, 0x2104, 8, ObjectType.VAR, "testName", PlkDataType.INTEGER64, AccessType.RW, PDOMapping.RPDO, "0", "0x10000");

            core.CreateSubObject("test", 20, 0x1F98, 3, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "3", "3");

            core.CreateObject("test", 20, 0x1F8D, ObjectType.RECORD, "objectName", PlkDataType.UNDEFINED, AccessType.UNDEFINED, PDOMapping.UNDEFINED, "", "");
            core.CreateSubObject("test", 20, 0x1F8D, 0, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "3", "3");
            core.CreateSubObject("test",20, 0x1F8D, 1, ObjectType.VAR, "testName", PlkDataType.UNSIGNED16, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 20, 0x1F8D, 2, ObjectType.VAR, "testName", PlkDataType.UNSIGNED16, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 20, 0x1F8D, 5, ObjectType.VAR, "testName", PlkDataType.UNSIGNED16, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 20, 0x1F8D, 10, ObjectType.VAR, "testName", PlkDataType.UNSIGNED16, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 20, 0x1F8D, 20, ObjectType.VAR, "testName", PlkDataType.UNSIGNED16, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 20, 0x1F8D, 0xF0, ObjectType.VAR, "testName", PlkDataType.UNSIGNED16, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 20, 0x1F8D, 0xF1, ObjectType.VAR, "testName", PlkDataType.UNSIGNED16, AccessType.RW, PDOMapping.DEFAULT, "0", "0");

            core.CreateObject("test", 20, 0x1F81, ObjectType.RECORD, "objectName", PlkDataType.UNDEFINED, AccessType.UNDEFINED, PDOMapping.UNDEFINED, "", "");
            core.CreateSubObject("test", 20, 0x1F81, 0, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "3", "3");
            core.CreateSubObject("test", 20, 0x1F81, 1, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 20, 0x1F81, 2, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 20, 0x1F81, 5, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 20, 0x1F81, 10, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 20, 0x1F81, 20, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 20, 0x1F81, 240, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 20, 0x1F81, 241, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "0");

            core.CreateNode("test", 1, "FieldbusGirl");
            core.CreateObject("test", 1, 0x1020, ObjectType.RECORD, "objectName", PlkDataType.UNDEFINED, AccessType.UNDEFINED, PDOMapping.UNDEFINED, "", "");
            core.CreateSubObject("test", 1, 0x1020, 0, ObjectType.DEFTYPE, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "2", "2");
            core.CreateSubObject("test", 1, 0x1020, 1, ObjectType.DEFTYPE, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 1, 0x1020, 2, ObjectType.DEFTYPE, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "0");

            core.CreateObject("test", 1, 0x1C0B, ObjectType.RECORD, "objectName", PlkDataType.UNDEFINED, AccessType.UNDEFINED, PDOMapping.UNDEFINED, "", "");
            core.CreateSubObject("test", 1, 0x1C0B, 3, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "");
            core.CreateObject("test", 1, 0x1C0C, ObjectType.RECORD, "objectName", PlkDataType.UNDEFINED, AccessType.UNDEFINED, PDOMapping.UNDEFINED, "", "");
            core.CreateSubObject("test", 1, 0x1C0C, 3, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "100");
            core.CreateObject("test", 1, 0x1C0D, ObjectType.RECORD, "objectName", PlkDataType.UNDEFINED, AccessType.UNDEFINED, PDOMapping.UNDEFINED, "", "");
            core.CreateSubObject("test", 1, 0x1C0D, 3, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "10");

            core.CreateObject("test", 1, 0x1F9B, ObjectType.RECORD, "objectName", PlkDataType.UNDEFINED, AccessType.UNDEFINED, PDOMapping.UNDEFINED, "", "");
            core.CreateSubObject("test", 1, 0x1F9B, 0, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "3", "3");
            core.CreateSubObject("test", 1, 0x1F9B, 1, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 1, 0x1F9B, 2, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 1, 0x1F9B, 5, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 1, 0x1F9B, 10, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 1, 0x1F9B, 20, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 1, 0x1F9B, 241, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "0", "0");

            core.CreateObject("test", 1, 0x1F8D, ObjectType.RECORD, "objectName", PlkDataType.UNDEFINED, AccessType.UNDEFINED, PDOMapping.UNDEFINED, "", "");
            core.CreateSubObject("test", 1, 0x1F8D, 0, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "3", "3");
            core.CreateSubObject("test", 1, 0x1F8D, 1, ObjectType.VAR, "testName", PlkDataType.UNSIGNED16, AccessType.RW, PDOMapping.DEFAULT, "36", "");
            core.CreateSubObject("test", 1, 0x1F8D, 2, ObjectType.VAR, "testName", PlkDataType.UNSIGNED16, AccessType.RW, PDOMapping.DEFAULT, "36", "");
            core.CreateSubObject("test", 1, 0x1F8D, 5, ObjectType.VAR, "testName", PlkDataType.UNSIGNED16, AccessType.RW, PDOMapping.DEFAULT, "36", "");
            core.CreateSubObject("test", 1, 0x1F8D, 10, ObjectType.VAR, "testName", PlkDataType.UNSIGNED16, AccessType.RW, PDOMapping.DEFAULT, "36", "");
            core.CreateSubObject("test", 1, 0x1F8D, 20, ObjectType.VAR, "testName", PlkDataType.UNSIGNED16, AccessType.RW, PDOMapping.DEFAULT, "36", "");
            core.CreateSubObject("test", 1, 0x1F8D, 0xF0, ObjectType.VAR, "testName", PlkDataType.UNSIGNED16, AccessType.RW, PDOMapping.DEFAULT, "36", "");
            core.CreateSubObject("test", 1, 0x1F8D, 0xF1, ObjectType.VAR, "testName", PlkDataType.UNSIGNED16, AccessType.RW, PDOMapping.DEFAULT, "36", "");

            core.CreateObject("test", 1, 0x1F81, ObjectType.RECORD, "objectName", PlkDataType.UNDEFINED, AccessType.UNDEFINED, PDOMapping.UNDEFINED, "", "");
            core.CreateSubObject("test", 1, 0x1F81, 0, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "3", "3");
            core.CreateSubObject("test", 1, 0x1F81, 1, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 1, 0x1F81, 2, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 1, 0x1F81, 5, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 1, 0x1F81, 10, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 1, 0x1F81, 20, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 1, 0x1F81, 0xF0, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 1, 0x1F81, 0xF1, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "0");

            core.CreateObject("test", 1, 0x1006, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "400", "10000");

            core.CreateObject("test", 1, 0x1300, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "400", "5000");

            core.CreateObject("test", 1, 0x1301, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "400", "5000");

            core.CreateObject("test", 1, 0x1400, ObjectType.RECORD, "objectName", PlkDataType.UNDEFINED, AccessType.UNDEFINED, PDOMapping.UNDEFINED, "", "");
            core.CreateSubObject("test", 1, 0x1400, 0, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "2", "2");
            core.CreateSubObject("test", 1, 0x1400, 1, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "0", "");
            core.CreateSubObject("test", 1, 0x1400, 2, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "0", "");

            core.CreateObject("test", 1, 0x1600, ObjectType.RECORD, "objectName", PlkDataType.UNDEFINED, AccessType.UNDEFINED, PDOMapping.UNDEFINED, "", "");
            core.CreateSubObject("test", 1, 0x1600, 0, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "2", "");
            core.CreateSubObject("test", 1, 0x1600, 1, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0x0120000000002000", "0");
            core.CreateSubObject("test", 1, 0x1600, 2, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0x0040012000012000", "0");

            core.SetObjectActualValue("test", 1, 0x1006, "100000", true);
            //Console.ReadLine();

            core.CreateObject("test", 1, 0x1F98, ObjectType.RECORD, "objectName", PlkDataType.UNDEFINED, AccessType.UNDEFINED, PDOMapping.UNDEFINED, "", "");
            core.CreateSubObject("test", 1, 0x1F98, 0, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "3", "3");
            core.CreateSubObject("test", 1, 0x1F98, 3, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 1, 0x1F98, 4, ObjectType.VAR, "testName", PlkDataType.UNSIGNED16, AccessType.RW, PDOMapping.DEFAULT, "36", "");
            core.CreateSubObject("test", 1, 0x1F98, 7, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 1, 0x1F98, 8, ObjectType.VAR, "testName", PlkDataType.UNSIGNED16, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 1, 0x1F98, 9, ObjectType.VAR, "testName", PlkDataType.UNSIGNED16, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 1, 0x1F98, 5, ObjectType.VAR, "testName", PlkDataType.UNSIGNED16, AccessType.RW, PDOMapping.DEFAULT, "36", "");

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
            core.CreateSubObject("test", 5, 0x1F98, 4, ObjectType.VAR, "testName", PlkDataType.UNSIGNED16, AccessType.RW, PDOMapping.DEFAULT, "36", "");
            core.CreateSubObject("test", 5, 0x1F98, 7, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 5, 0x1F98, 8, ObjectType.VAR, "testName", PlkDataType.UNSIGNED16, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 5, 0x1F98, 9, ObjectType.VAR, "testName", PlkDataType.UNSIGNED16, AccessType.RW, PDOMapping.DEFAULT, "0", "0");

            core.CreateObject("test", 5, 0x1F9B, ObjectType.RECORD, "objectName", PlkDataType.UNDEFINED, AccessType.UNDEFINED, PDOMapping.UNDEFINED, "", "");
            core.CreateSubObject("test", 5, 0x1F9B, 0, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "3", "3");
            core.CreateSubObject("test", 5, 0x1F9B, 1, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 5, 0x1F9B, 2, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 5, 0x1F9B, 5, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 5, 0x1F9B, 10, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 5, 0x1F9B, 20, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "0", "0");

            core.CreateObject("test", 5, 0x1300, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "400", "5000");

            core.CreateObject("test", 5, 0x1301, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "400", "5000");

            core.CreateObject("test", 5, 0x1F8D, ObjectType.RECORD, "objectName", PlkDataType.UNDEFINED, AccessType.UNDEFINED, PDOMapping.UNDEFINED, "", "");
            core.CreateSubObject("test", 5, 0x1F8D, 0, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "3", "3");
            core.CreateSubObject("test", 5, 0x1F8D, 1, ObjectType.VAR, "testName", PlkDataType.UNSIGNED16, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 5, 0x1F8D, 2, ObjectType.VAR, "testName", PlkDataType.UNSIGNED16, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 5, 0x1F8D, 5, ObjectType.VAR, "testName", PlkDataType.UNSIGNED16, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 5, 0x1F8D, 10, ObjectType.VAR, "testName", PlkDataType.UNSIGNED16, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 5, 0x1F8D, 20, ObjectType.VAR, "testName", PlkDataType.UNSIGNED16, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 5, 0x1F8D, 0xF0, ObjectType.VAR, "testName", PlkDataType.UNSIGNED16, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 5, 0x1F8D, 0xF1, ObjectType.VAR, "testName", PlkDataType.UNSIGNED16, AccessType.RW, PDOMapping.DEFAULT, "0", "0");

            core.CreateObject("test", 5, 0x1C0B, ObjectType.RECORD, "objectName", PlkDataType.UNDEFINED, AccessType.UNDEFINED, PDOMapping.UNDEFINED, "", "");
            core.CreateSubObject("test", 5, 0x1C0B, 3, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "20");

            core.CreateObject("test", 5, 0x1F8B, ObjectType.RECORD, "objectName", PlkDataType.UNDEFINED, AccessType.UNDEFINED, PDOMapping.UNDEFINED, "", "");
            core.CreateSubObject("test", 5, 0x1F8B, 0, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "3", "3");
            core.CreateSubObject("test", 5, 0x1F8B, 1, ObjectType.VAR, "testName", PlkDataType.UNSIGNED16, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 5, 0x1F8B, 2, ObjectType.VAR, "testName", PlkDataType.UNSIGNED16, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 5, 0x1F8B, 5, ObjectType.VAR, "testName", PlkDataType.UNSIGNED16, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 5, 0x1F8B, 10, ObjectType.VAR, "testName", PlkDataType.UNSIGNED16, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 5, 0x1F8B, 20, ObjectType.VAR, "testName", PlkDataType.UNSIGNED16, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 5, 0x1F8B, 0xF0, ObjectType.VAR, "testName", PlkDataType.UNSIGNED16, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 5, 0x1F8B, 0xF1, ObjectType.VAR, "testName", PlkDataType.UNSIGNED16, AccessType.RW, PDOMapping.DEFAULT, "0", "0");

            core.CreateObject("test", 5, 0x1F81, ObjectType.RECORD, "objectName", PlkDataType.UNDEFINED, AccessType.UNDEFINED, PDOMapping.UNDEFINED, "", "");
            core.CreateSubObject("test", 5, 0x1F81, 0, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "3", "3");
            core.CreateSubObject("test", 5, 0x1F81, 1, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 5, 0x1F81, 2, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 5, 0x1F81, 5, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 5, 0x1F81, 10, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 5, 0x1F81, 20, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 5, 0x1F81, 0xF0, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 5, 0x1F81, 0xF1, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "0");

            core.CreateObject("test", 5, 0x1006, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "400", "10000");

            core.CreateObject("test", 5, 0x1020, ObjectType.RECORD, "objectName", PlkDataType.UNDEFINED, AccessType.UNDEFINED, PDOMapping.UNDEFINED, "", "");
            core.CreateSubObject("test", 5, 0x1020, 0, ObjectType.DEFTYPE, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "2", "2");
            core.CreateSubObject("test", 5, 0x1020, 1, ObjectType.DEFTYPE, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 5, 0x1020, 2, ObjectType.DEFTYPE, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "0");

            core.CreateSubObject("test", 5, 0x1F98, 3, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
            core.CreateSubObject("test", 5, 0x1F98, 5, ObjectType.VAR, "testName", PlkDataType.UNSIGNED16, AccessType.RW, PDOMapping.DEFAULT, "36", "");

            core.CreateObject("test", 5, 0x2100, ObjectType.RECORD, "objectName", PlkDataType.UNDEFINED, AccessType.UNDEFINED, PDOMapping.UNDEFINED, "", "");
            core.CreateSubObject("test", 5, 0x2100, 0, ObjectType.DEFTYPE, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "0", "10");
            core.CreateSubObject("test", 5, 0x2100, 1, ObjectType.DEFTYPE, "testName", PlkDataType.UNSIGNED16, AccessType.RW, PDOMapping.DEFAULT, "0", "0x100");
            core.CreateSubObject("test", 5, 0x2100, 2, ObjectType.DEFTYPE, "testName", PlkDataType.UNSIGNED24, AccessType.RW, PDOMapping.DEFAULT, "0", "0x100");
            core.CreateSubObject("test", 5, 0x2100, 3, ObjectType.DEFTYPE, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "0x100");
            core.CreateSubObject("test", 5, 0x2100, 4, ObjectType.DEFTYPE, "testName", PlkDataType.UNSIGNED40, AccessType.RW, PDOMapping.DEFAULT, "0", "0x100");
            core.CreateSubObject("test", 5, 0x2100, 6, ObjectType.DEFTYPE, "testName", PlkDataType.UNSIGNED56, AccessType.RW, PDOMapping.NO, "", "100");
            core.CreateSubObject("test", 5, 0x2100, 7, ObjectType.DEFTYPE, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.NO, "0", "0x0010000000202104");

            core.CreateObject("test", 5, 0x2102, ObjectType.RECORD, "objectName", PlkDataType.UNDEFINED, AccessType.UNDEFINED, PDOMapping.UNDEFINED, "", "");
            core.CreateSubObject("test", 5, 0x2102, 0, ObjectType.DEFTYPE, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.NO, "0", "7");
            core.CreateSubObject("test", 5, 0x2102, 1, ObjectType.DEFTYPE, "testName", PlkDataType.INTEGER16, AccessType.RW, PDOMapping.NO, "0", "0x100");
            core.CreateSubObject("test", 5, 0x2102, 2, ObjectType.DEFTYPE, "testName", PlkDataType.INTEGER24, AccessType.RW, PDOMapping.NO, "0", "0x100");
            core.CreateSubObject("test", 5, 0x2102, 3, ObjectType.DEFTYPE, "testName", PlkDataType.INTEGER32, AccessType.RW, PDOMapping.NO, "", "100");
            core.CreateSubObject("test", 5, 0x2102, 4, ObjectType.DEFTYPE, "testName", PlkDataType.INTEGER40, AccessType.RW, PDOMapping.NO, "", "100");
            core.CreateSubObject("test", 5, 0x2102, 5, ObjectType.DEFTYPE, "testName", PlkDataType.INTEGER48, AccessType.RW, PDOMapping.NO, "", "100");
            core.CreateSubObject("test", 5, 0x2102, 6, ObjectType.DEFTYPE, "testName", PlkDataType.INTEGER56, AccessType.RW, PDOMapping.NO, "", "100");
            core.CreateSubObject("test", 5, 0x2102, 7, ObjectType.DEFTYPE, "testName", PlkDataType.INTEGER64, AccessType.RW, PDOMapping.NO, "0", "0x0010000000202104");

            core.CreateObject("test", 5, 0x2110, ObjectType.DEFTYPE, "Name", PlkDataType.UNDEFINED, AccessType.UNDEFINED, PDOMapping.UNDEFINED, "", "");
            core.CreateSubObject("test", 5, 0x2110, 0x00, ObjectType.DEFTYPE, "Name", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.RPDO, "0", "15");
            core.CreateSubObject("test", 5, 0x2110, 0x0d, ObjectType.DEFTYPE, "Name", PlkDataType.UNICODE_STRING, AccessType.RW, PDOMapping.RPDO, "100", "WriteThisString");
            core.CreateSubObject("test", 5, 0x2110, 0x01, ObjectType.DEFTYPE, "Name", PlkDataType.VISIBLE_STRING, AccessType.RW, PDOMapping.RPDO, "100", "WriteThisString");
            core.CreateSubObject("test", 5, 0x2110, 0x02, ObjectType.DEFTYPE, "Name", PlkDataType.OCTET_STRING, AccessType.RW, PDOMapping.RPDO, "100", "WriteThisString");
            core.CreateSubObject("test", 5, 0x2110, 0x03, ObjectType.DEFTYPE, "Name", PlkDataType.IP_ADDRESS, AccessType.RW, PDOMapping.RPDO, "100", "0x64646464");
            core.CreateSubObject("test", 5, 0x2110, 0x0b, ObjectType.DEFTYPE, "Name", PlkDataType.IP_ADDRESS, AccessType.RW, PDOMapping.RPDO, "100", "254.254.254.254");
            core.CreateSubObject("test", 5, 0x2110, 0x0c, ObjectType.DEFTYPE, "Name", PlkDataType.IP_ADDRESS, AccessType.RW, PDOMapping.RPDO, "100", "1.1.1.1");
            core.CreateSubObject("test", 5, 0x2110, 0x04, ObjectType.DEFTYPE, "Name", PlkDataType.BOOLEAN, AccessType.RW, PDOMapping.RPDO, "false", "true");
            core.CreateSubObject("test", 5, 0x2110, 0x05, ObjectType.DEFTYPE, "Name", PlkDataType.MAC_ADDRESS, AccessType.RW, PDOMapping.RPDO, "0", "0x000A086121012");
            core.CreateSubObject("test", 5, 0x2110, 0x06, ObjectType.DEFTYPE, "Name", PlkDataType.REAL32, AccessType.RW, PDOMapping.RPDO, "0", "1267.43233E12");
            core.CreateSubObject("test", 5, 0x2110, 0x07, ObjectType.DEFTYPE, "Name", PlkDataType.REAL32, AccessType.RW, PDOMapping.RPDO, "0", "1.234532432");
            core.CreateSubObject("test", 5, 0x2110, 0x08, ObjectType.DEFTYPE, "Name", PlkDataType.REAL64, AccessType.RW, PDOMapping.RPDO, "0", "-1E4");
            core.CreateSubObject("test", 5, 0x2110, 0x09, ObjectType.DEFTYPE, "Name", PlkDataType.REAL64, AccessType.RW, PDOMapping.RPDO, "0", "12.78e-2");
            core.CreateSubObject("test", 5, 0x2110, 0x0a, ObjectType.DEFTYPE, "Name", PlkDataType.REAL64, AccessType.RW, PDOMapping.RPDO, "6", "-0");
            core.CreateSubObject("test", 5, 0x2110, 0x10, ObjectType.DEFTYPE, "Name", PlkDataType.REAL64, AccessType.RW, PDOMapping.RPDO, "6", "-0");

            core.CreateObject("test", 5, 0x2300, ObjectType.DEFTYPE, "Name", PlkDataType.UNDEFINED, AccessType.UNDEFINED, PDOMapping.UNDEFINED, "", "");
            core.CreateSubObject("test", 5, 0x2300, 0x00, ObjectType.DEFTYPE, "Name", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.RPDO, "0", "5");
            core.CreateSubObject("test", 5, 0x2300, 0x0d, ObjectType.DEFTYPE, "Name", PlkDataType.UNICODE_STRING, AccessType.RW, PDOMapping.RPDO, "100", "WriteThisString");
            core.CreateSubObject("test", 5, 0x2300, 0x01, ObjectType.DEFTYPE, "Name", PlkDataType.VISIBLE_STRING, AccessType.RW, PDOMapping.RPDO, "100", "WriteThisString");
            core.CreateSubObject("test", 5, 0x2300, 0x02, ObjectType.DEFTYPE, "Name", PlkDataType.OCTET_STRING, AccessType.RW, PDOMapping.RPDO, "100", "WriteThisString");
            core.CreateSubObject("test", 5, 0x2300, 0x03, ObjectType.DEFTYPE, "Name", PlkDataType.IP_ADDRESS, AccessType.RW, PDOMapping.RPDO, "100", "0x64646464");
            core.CreateSubObject("test", 5, 0x2300, 0x0b, ObjectType.DEFTYPE, "Name", PlkDataType.IP_ADDRESS, AccessType.RW, PDOMapping.RPDO, "100", "254.254.254.254");
            core.CreateSubObject("test", 5, 0x2300, 0x0c, ObjectType.DEFTYPE, "Name", PlkDataType.IP_ADDRESS, AccessType.RW, PDOMapping.RPDO, "100", "1.1.1.1");
            core.CreateSubObject("test", 5, 0x2300, 0x04, ObjectType.DEFTYPE, "Name", PlkDataType.BOOLEAN, AccessType.RW, PDOMapping.RPDO, "false", "true");
            core.CreateSubObject("test", 5, 0x2300, 0x05, ObjectType.DEFTYPE, "Name", PlkDataType.MAC_ADDRESS, AccessType.RW, PDOMapping.RPDO, "0", "0x000A086121012");
            core.CreateSubObject("test", 5, 0x2300, 0x06, ObjectType.DEFTYPE, "Name", PlkDataType.REAL32, AccessType.RW, PDOMapping.RPDO, "0", "1267432.33E12");
            core.CreateSubObject("test", 5, 0x2300, 0x07, ObjectType.DEFTYPE, "Name", PlkDataType.REAL32, AccessType.RW, PDOMapping.RPDO, "0", "1.234532432");
            core.CreateSubObject("test", 5, 0x2300, 0x08, ObjectType.DEFTYPE, "Name", PlkDataType.REAL64, AccessType.RW, PDOMapping.RPDO, "0", "-1E4");
            core.CreateSubObject("test", 5, 0x2300, 0x09, ObjectType.DEFTYPE, "Name", PlkDataType.REAL64, AccessType.RW, PDOMapping.RPDO, "0", "12.78e-2");
            core.CreateSubObject("test", 5, 0x2300, 0x0a, ObjectType.DEFTYPE, "Name", PlkDataType.REAL64, AccessType.RW, PDOMapping.RPDO, "6", "-0");
            core.CreateSubObject("test", 5, 0x2300, 0x10, ObjectType.DEFTYPE, "Name", PlkDataType.REAL64, AccessType.RW, PDOMapping.RPDO, "6", "-0");

            core.CreateObject("test", 5, 0x2120, ObjectType.DEFTYPE, "Name", PlkDataType.UNDEFINED, AccessType.UNDEFINED, PDOMapping.UNDEFINED, "", "");
            core.CreateSubObject("test", 5, 0x2120, 0x00, ObjectType.DEFTYPE, "Name", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.RPDO, "1", "1");
            core.CreateSubObject("test", 5, 0x2120, 0x01, ObjectType.DEFTYPE, "Name", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.RPDO, "0", "0");


            core.CreateObject("test", 5, 0x2000, ObjectType.RECORD, "objectName", PlkDataType.UNDEFINED, AccessType.UNDEFINED, PDOMapping.UNDEFINED, "", "");
            core.CreateSubObject("test", 5, 0x2000, 0x0b, ObjectType.DEFTYPE, "Name", PlkDataType.IP_ADDRESS, AccessType.RW, PDOMapping.RPDO, "100", "254.254.254.254");

            core.AddNodeAssignment("test", 5, NodeAssignment.NMT_NODEASSIGN_RT1);
            core.AddNodeAssignment("test", 5, NodeAssignment.NMT_NODEASSIGN_RT2);
            core.AddNodeAssignment("test", 5, NodeAssignment.NMT_NODEASSIGN_SWUPDATE);
            core.AddNodeAssignment("test", 5, NodeAssignment.NMT_NODEASSIGN_SWVERSIONCHECK);

            core.AddNodeAssignment("test", 240, NodeAssignment.NMT_NODEASSIGN_MN_PRES);

            AssignmentCollection collection = new AssignmentCollection();
            core.GetNodeAssignment("test", 240, collection);

            foreach (var item in collection)
            {
                Console.WriteLine((NodeAssignment)item);
            }

            uint obsize = 0;
            res = core.GetObjectSize("test", 5, 0x2300, out obsize);
            Console.WriteLine(obsize);
            Console.WriteLine(res.IsSuccessful());

            res = core.GetSubObjectSize("test", 5, 0x2300, 0, out obsize);
            Console.WriteLine(obsize);
            Console.WriteLine(res.IsSuccessful());

            //Console.ReadLine();
            Result test = core.CreateParameter("test", 1, "UID_DOM_Index2100_Sub1E", "UID_DT_Index2100_Sub1E", ParameterAccess.readWrite);
            Console.WriteLine(test.IsSuccessful());
            test = core.CreateStructDatatype("test", 1, "notFound", "Index2100_Sub1E");
            test = core.CreateStructDatatype("test", 1, "UID_DT_Index2100_Sub1E", "Index2100_Sub1E");
            Console.WriteLine(test.IsSuccessful());
            test = core.CreateVarDeclaration("test", 1, "UID_DT_Index2100_Sub1E", "UniqueID_BOOL", "BOOL_NAME", IEC_Datatype.BOOL);
            test = core.CreateVarDeclaration("test", 1, "UID_DT_Index2100_Sub1E", "UniqueID_BYTE", "BYTE_NAME", IEC_Datatype.BYTE);
            test = core.CreateVarDeclaration("test", 1, "UID_DT_Index2100_Sub1E", "UniqueID_INT", "DINT_NAME", IEC_Datatype.DINT);
            test = core.CreateVarDeclaration("test", 1, "UID_DT_Index2100_Sub1E", "UniqueID_SINT", "DWORD_NAME", IEC_Datatype.DWORD);
            test = core.CreateVarDeclaration("test", 1, "UID_DT_Index2100_Sub1E", "UniqueID_SINT", "INT_NAME", IEC_Datatype.INT);
            test = core.CreateVarDeclaration("test", 1, "UID_DT_Index2100_Sub1E", "UniqueID_BOOL", "LINT_NAME", IEC_Datatype.LINT);
            test = core.CreateVarDeclaration("test", 1, "UID_DT_Index2100_Sub1E", "UniqueID_DINT", "LWORD_NAME", IEC_Datatype.LWORD);
            test = core.CreateVarDeclaration("test", 1, "UID_DT_Index2100_Sub1E", "UniqueID_REAL", "SINT_NAME", IEC_Datatype.SINT);
            test = core.CreateVarDeclaration("test", 1, "UID_DT_Index2100_Sub1E", "UniqueID_LREAL", "UDINT_NAME", IEC_Datatype.UDINT);
            test = core.CreateVarDeclaration("test", 1, "UID_DT_Index2100_Sub1E", "UniqueID_UINT", "UINT_NAME", IEC_Datatype.UINT);
            test = core.CreateVarDeclaration("test", 1, "UID_DT_Index2100_Sub1E", "UniqueID_ULINT", "ULINT_NAME", IEC_Datatype.ULINT);
            test = core.CreateVarDeclaration("test", 1, "UID_DT_Index2100_Sub1E", "UniqueID_USINT", "USINT_NAME", IEC_Datatype.USINT);
            test = core.CreateVarDeclaration("test", 1, "UID_DT_Index2100_Sub1E", "UniqueID_CHAR", "CHAR_NAME", IEC_Datatype._CHAR);
            test = core.CreateVarDeclaration("test", 1, "UID_DT_Index2100_Sub1E", "UID_Index2100_Sub1E_StatusInput01", "StatusInput01", IEC_Datatype.BITSTRING, 1);
            test = core.CreateVarDeclaration("test", 1, "UID_DT_Index2100_Sub1E", "UID_Index2100_Sub1E_Bit_Unused_01", "StatusInput01", IEC_Datatype.BITSTRING, 1);
            test = core.CreateVarDeclaration("test", 1, "UID_DT_Index2100_Sub1E", "UID_Index2100_Sub1E_StatusInput02", "StatusInput01", IEC_Datatype.BITSTRING, 1);
            test = core.CreateVarDeclaration("test", 1, "UID_DT_Index2100_Sub1E", "UID_Index2100_Sub1E_Bit_Unused_02", "StatusInput01", IEC_Datatype.BITSTRING, 5);
            Console.WriteLine(test.IsSuccessful());

            test = core.CreateParameter("test", 1, "UID_DOM_Index2101_Sub1E", "UID_DT_Index2101_Sub1E", ParameterAccess.write);
            Console.WriteLine(test.IsSuccessful());
            test = core.CreateStructDatatype("test", 1, "UID_DT_Index2101_Sub1E", "Index2101_Sub1E");
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
            res = core.CreateVarDeclaration("test", 1, "UID_DT_Index2101_Sub1E", "UID_Index2101_Sub1E_Bit_Unused_01", "Bit_Unused_01", IEC_Datatype.BITSTRING, 4);
            Console.WriteLine(test.IsSuccessful());
            test = core.CreateParameter("test", 1, "UID_100", ParameterAccess.read, IEC_Datatype.UDINT);

            test = core.CreateParameter("test", 1, "UID_ArrayDatatype_Param", "UID_DT_ArrayDataType", ParameterAccess.write);
            Console.WriteLine(test.GetErrorMessage());
            test = core.CreateArrayDatatype("test", 1, "UID_DT_ArrayDataType", "arraytypename", 0, 10, IEC_Datatype.INT);
            Console.WriteLine(test.GetErrorMessage());

            uint size;
            res = core.GetDatatypeSize("test", 1, "UID_DT_Index2100_Sub1E", out size);
            Console.WriteLine(res.GetErrorType());
            Console.WriteLine(res.GetErrorMessage());
            Console.WriteLine(size);

            res = core.GetDatatypeSize("test", 1, "UID_DT_Index2101_Sub1E", out size);
            Console.WriteLine(res.GetErrorType());
            Console.WriteLine(res.GetErrorMessage());
            Console.WriteLine(size);

            res = core.GetDatatypeSize("test", 1, "UID_100", out size);
            Console.WriteLine(res.GetErrorType());
            Console.WriteLine(res.GetErrorMessage());
            Console.WriteLine(size);

            res = core.GetDatatypeSize("test", 1, "notFound", out size);
            Console.WriteLine(test.IsSuccessful());
            Console.WriteLine(size);

            core.CreateDynamicChannel("test", 240, PlkDataType.INTEGER8, DynamicChannelAccessType.readOnly, 0xA000, 0xA00F, 4064, 0);
            core.CreateDynamicChannel("test", 240, PlkDataType.UNSIGNED8, DynamicChannelAccessType.readOnly, 0xA040, 0xA04F, 4064, 0);
            core.CreateDynamicChannel("test", 240, PlkDataType.INTEGER16, DynamicChannelAccessType.readOnly, 0xA0C0, 0xA0C7, 2032, 0);
            core.CreateDynamicChannel("test", 240, PlkDataType.UNSIGNED16, DynamicChannelAccessType.readOnly, 0xA100, 0xA107, 2031, 0);
            core.CreateDynamicChannel("test", 240, PlkDataType.INTEGER32, DynamicChannelAccessType.readOnly, 0xA1C0, 0xA1C3, 1016, 0);
            core.CreateDynamicChannel("test", 240, PlkDataType.UNSIGNED32, DynamicChannelAccessType.readOnly, 0xA200, 0xA203, 1016, 0);
            core.CreateDynamicChannel("test", 240, PlkDataType.INTEGER64, DynamicChannelAccessType.readOnly, 0xA400, 0xA401, 508, 0);
            core.CreateDynamicChannel("test", 240, PlkDataType.UNSIGNED64, DynamicChannelAccessType.readOnly, 0xA440, 0xA441, 508, 0);
            core.CreateDynamicChannel("test", 240, PlkDataType.INTEGER8, DynamicChannelAccessType.readWriteOutput, 0xA480, 0xA48F, 4064, 0);
            core.CreateDynamicChannel("test", 240, PlkDataType.UNSIGNED8, DynamicChannelAccessType.readWriteOutput, 0xA4C0, 0xA4CF, 4064, 0);
            core.CreateDynamicChannel("test", 240, PlkDataType.INTEGER16, DynamicChannelAccessType.readWriteOutput, 0xA540, 0xA547, 2032, 0);
            core.CreateDynamicChannel("test", 240, PlkDataType.UNSIGNED16, DynamicChannelAccessType.readWriteOutput, 0xA580, 0xA587, 2031, 0);
            core.CreateDynamicChannel("test", 240, PlkDataType.INTEGER32, DynamicChannelAccessType.readWriteOutput, 0xA640, 0xA643, 1016, 0);
            core.CreateDynamicChannel("test", 240, PlkDataType.UNSIGNED32, DynamicChannelAccessType.readWriteOutput, 0xA680, 0xA683, 1016, 0);
            core.CreateDynamicChannel("test", 240, PlkDataType.INTEGER64, DynamicChannelAccessType.readWriteOutput, 0xA880, 0xA881, 508, 0);
            core.CreateDynamicChannel("test", 240, PlkDataType.UNSIGNED64, DynamicChannelAccessType.readWriteOutput, 0xA8C0, 0xA8C1, 508, 0);

            core.CreateDynamicChannel("test", 241, PlkDataType.INTEGER8, DynamicChannelAccessType.readOnly, 0xA000, 0xA00F, 4064, 0);
            core.CreateDynamicChannel("test", 241, PlkDataType.UNSIGNED8, DynamicChannelAccessType.readOnly, 0xA040, 0xA04F, 4064, 0);
            core.CreateDynamicChannel("test", 241, PlkDataType.INTEGER16, DynamicChannelAccessType.readOnly, 0xA0C0, 0xA0C7, 2032, 0);
            core.CreateDynamicChannel("test", 241, PlkDataType.UNSIGNED16, DynamicChannelAccessType.readOnly, 0xA100, 0xA107, 2031, 0);
            core.CreateDynamicChannel("test", 241, PlkDataType.INTEGER32, DynamicChannelAccessType.readOnly, 0xA1C0, 0xA1C3, 1016, 0);
            core.CreateDynamicChannel("test", 241, PlkDataType.UNSIGNED32, DynamicChannelAccessType.readOnly, 0xA200, 0xA203, 1016, 0);
            core.CreateDynamicChannel("test", 241, PlkDataType.INTEGER64, DynamicChannelAccessType.readOnly, 0xA400, 0xA401, 508, 0);
            core.CreateDynamicChannel("test", 241, PlkDataType.UNSIGNED64, DynamicChannelAccessType.readOnly, 0xA440, 0xA441, 508, 0);
            core.CreateDynamicChannel("test", 241, PlkDataType.INTEGER8, DynamicChannelAccessType.readWriteOutput, 0xA480, 0xA48F, 4064, 0);
            core.CreateDynamicChannel("test", 241, PlkDataType.UNSIGNED8, DynamicChannelAccessType.readWriteOutput, 0xA4C0, 0xA4CF, 4064, 0);
            core.CreateDynamicChannel("test", 241, PlkDataType.INTEGER16, DynamicChannelAccessType.readWriteOutput, 0xA540, 0xA547, 2032, 0);
            core.CreateDynamicChannel("test", 241, PlkDataType.UNSIGNED16, DynamicChannelAccessType.readWriteOutput, 0xA580, 0xA587, 2031, 0);
            core.CreateDynamicChannel("test", 241, PlkDataType.INTEGER32, DynamicChannelAccessType.readWriteOutput, 0xA640, 0xA643, 1016, 0);
            core.CreateDynamicChannel("test", 241, PlkDataType.UNSIGNED32, DynamicChannelAccessType.readWriteOutput, 0xA680, 0xA683, 1016, 0);
            core.CreateDynamicChannel("test", 241, PlkDataType.INTEGER64, DynamicChannelAccessType.readWriteOutput, 0xA880, 0xA881, 508, 0);
            core.CreateDynamicChannel("test", 241, PlkDataType.UNSIGNED64, DynamicChannelAccessType.readWriteOutput, 0xA8C0, 0xA8C1, 508, 0);


            res = core.CreateDomainObject("test", 1, 0x2000, ObjectType.DEFSTRUCT, "domain", PDOMapping.RPDO, "UID_DOM_Index2100_Sub1E");
            Console.WriteLine(res.GetErrorType());
            Console.WriteLine(res.GetErrorMessage());

            res = core.GetObjectSize("test", 1, 0x2000, out obsize);
            Console.WriteLine(obsize);
            Console.WriteLine(res.GetErrorType());
            Console.WriteLine(res.GetErrorMessage());

            res = core.CreateDomainSubObject("test", 1, 0x2000, 0x1, ObjectType.DEFSTRUCT, "domain", PDOMapping.RPDO, "UID_DOM_Index2101_Sub1E");
            Console.WriteLine(res.GetErrorType());
            Console.WriteLine(res.GetErrorMessage());

            res = core.GetSubObjectSize("test", 1, 0x2000, 0x1, out obsize);
            Console.WriteLine(obsize);
            Console.WriteLine(res.IsSuccessful());

            res = core.SetFeatureValue("test", 240, GeneralFeatureEnum.DLLFeatureMN, "true");
            Console.WriteLine(res.GetErrorType());
            Console.WriteLine(res.GetErrorMessage());

            res = core.SetFeatureValue("test", 240, MNFeatureEnum.DLLMNFeatureMultiplex, "true");
            Console.WriteLine(res.GetErrorType());
            Console.WriteLine(res.GetErrorMessage());
            res = core.SetFeatureValue("test", 240, MNFeatureEnum.DLLMNPResChaining, "true");
            Console.WriteLine(res.GetErrorType());
            Console.WriteLine(res.GetErrorMessage());

            res = core.SetFeatureValue("test", 5, CNFeatureEnum.DLLCNPResChaining, "true");
            Console.WriteLine(res.GetErrorType());
            Console.WriteLine(res.GetErrorMessage());
            res = core.SetOperationModeChained("test", 5);
            Console.WriteLine(res.GetErrorType());
            Console.WriteLine(res.GetErrorMessage());

            res = core.SetFeatureValue("test", 20, CNFeatureEnum.DLLCNPResChaining, "true");
            Console.WriteLine(res.GetErrorType());
            Console.WriteLine(res.GetErrorMessage());
            res = core.SetOperationModeChained("test", 20);
            Console.WriteLine(res.GetErrorType());
            Console.WriteLine(res.GetErrorMessage());

            res = core.SetFeatureValue("test", 10, CNFeatureEnum.DLLCNPResChaining, "true");
            Console.WriteLine(res.GetErrorType());
            Console.WriteLine(res.GetErrorMessage());
            res = core.SetOperationModeChained("test", 10);
            Console.WriteLine(res.GetErrorType());
            Console.WriteLine(res.GetErrorMessage());

            res = core.SetFeatureValue("test", 1, CNFeatureEnum.DLLCNFeatureMultiplex, "true");
            Console.WriteLine(res.GetErrorType());
            Console.WriteLine(res.GetErrorMessage());
            res = core.SetOperationModeMultiplexed("test", 1, 5);
            Console.WriteLine(res.GetErrorType());
            Console.WriteLine(res.GetErrorMessage());

            //res = core.MapAllObjects("test", 1, Direction.RX, true);
            //Console.WriteLine(res.GetErrorType());
            //Console.WriteLine(res.GetErrorMessage());
            //Console.ReadLine();

            res = core.MapAllObjects("test", 10, Direction.RX, true);
            Console.WriteLine(res.GetErrorType());
            Console.WriteLine(res.GetErrorMessage());
            Console.ReadLine();

            res = core.MapAllObjects("test", 10, Direction.TX, true);
            Console.WriteLine(res.GetErrorType());
            Console.WriteLine(res.GetErrorMessage());
            Console.ReadLine();

            res = core.MapAllObjects("test", 20, Direction.TX, true);
            Console.WriteLine(res.GetErrorType());
            Console.WriteLine(res.GetErrorMessage());
            Console.ReadLine();

            res = core.MapAllObjects("test", 20, Direction.RX, true);
            Console.WriteLine(res.GetErrorType());
            Console.WriteLine(res.GetErrorMessage());
            Console.ReadLine();

            string tempString;
            ByteCollection binString = new ByteCollection();
            res = core.BuildConfiguration("test", out tempString, binString);
            Console.WriteLine(res.GetErrorType());
            Console.WriteLine(res.GetErrorMessage());
            Console.ReadLine();
            //Console.WriteLine(tempString);

            string processImage;
            res = core.BuildXMLProcessImage("test", 240, out processImage);
            Console.WriteLine(res.GetErrorType());
            Console.WriteLine(res.GetErrorMessage());
            System.IO.File.WriteAllText(@".\XAP_240.xml", processImage);

            string processImageCS;
            res = core.BuildNETProcessImage("test", 240, out processImageCS);
            Console.WriteLine(res.GetErrorType());
            Console.WriteLine(res.GetErrorMessage());
            System.IO.File.WriteAllText(@".\XAP_240.cs", processImageCS);

            string processImageC_240;
            res = core.BuildCProcessImage("test", 240, out processImageC_240);
            Console.WriteLine(res.GetErrorType());
            Console.WriteLine(res.GetErrorMessage());
            System.IO.File.WriteAllText(@".\XAP_240.c", processImageC_240);

            string processImage10;
            res = core.BuildXMLProcessImage("test", 10, out processImage10);
            Console.WriteLine(res.GetErrorType());
            Console.WriteLine(res.GetErrorMessage());
            System.IO.File.WriteAllText(@".\XAP_10.xml", processImage10);

            string processImage1;
            res = core.BuildXMLProcessImage("test", 1, out processImage1);
            Console.WriteLine(res.GetErrorType());
            Console.WriteLine(res.GetErrorMessage());
            System.IO.File.WriteAllText(@".\XAP_1.xml", processImage1);

            string processImage20;
            res = core.BuildXMLProcessImage("test", 20, out processImage20);
            Console.WriteLine(res.GetErrorType());
            Console.WriteLine(res.GetErrorMessage());
            System.IO.File.WriteAllText(@".\XAP_20.xml", processImage20);


            string processImage241;
            res = core.BuildXMLProcessImage("test", 241, out processImage241);
            Console.WriteLine(res.GetErrorType());
            Console.WriteLine(res.GetErrorMessage());
            System.IO.File.WriteAllText(@".\XAP_241.xml", processImage241);

            string processImageNET;
            res = core.BuildNETProcessImage("test", 1, out processImageNET);
            Console.WriteLine(res.GetErrorType());
            Console.WriteLine(res.GetErrorMessage());
            System.IO.File.WriteAllText(@".\ProcessImage.cs", processImageNET);

            string processImageC;
            res = core.BuildCProcessImage("test", 1, out processImageC);
            Console.WriteLine(res.GetErrorType());
            Console.WriteLine(res.GetErrorMessage());
            System.IO.File.WriteAllText(@".\XAP.h", processImageC);

            System.IO.FileStream file = new System.IO.FileStream("mnobd.cdc", System.IO.FileMode.Create, System.IO.FileAccess.Write);
                for (int i = 0; i < binString.Count; i++)
                {
                    file.WriteByte(binString[i]);
                }

            System.IO.File.WriteAllText(@".\mnobd.txt", tempString);

            //core.MapObject("test", 1, 2000, Direction.RX, 1);
           
            //Console.ReadLine();
            string cTime;
            core.GetObjectActualValue("test", 240, 0x1006, out cTime);
            Console.WriteLine(cTime);
            //Console.ReadLine();

            //Console.ReadLine();
            ObjectCollection objColl = new ObjectCollection();
            core.GetObjectsWithActualValue("test", 5, objColl);
            Console.WriteLine(objColl.Count);
            foreach (var var in objColl)
            {
                Console.Write(var.Key.first.ToString("x"));
                Console.Write("/");
                Console.Write(var.Key.second.ToString());
                Console.Write(" : ");
                Console.WriteLine(var.Value.ToString());
            }
            //Console.ReadLine();

            core.SetNodeName("test", 240, "MasterOfDisaster");
            core.SetNodeName("test", 5, "SuperNode");
            //Console.ReadLine();
            
            string assign;
            core.GetSubObjectActualValue("test", 240, 0x1f81, 0x5, out assign);
            Console.WriteLine(assign);
            //Console.ReadLine();
            
            
            res = core.RemoveNode("test", 1);
            Console.WriteLine(res.IsSuccessful());
            core.EnableNode("test", 10, false);
            Console.WriteLine(res.IsSuccessful());

            res = core.BuildConfiguration("test", out tempString, binString);
            Console.WriteLine(tempString);
            Console.WriteLine(res.IsSuccessful());
            //Console.ReadLine();
            core.EnableNode("test", 10, true);
            Console.WriteLine(res.IsSuccessful());

            res = core.BuildConfiguration("test", out tempString, binString);
            Console.WriteLine(tempString);
            Console.WriteLine(res.IsSuccessful());

            //Console.ReadLine();
            res = core.RemoveNetwork("test");
            Console.WriteLine(res.IsSuccessful());
            //Console.ReadLine();

        }
    }
}
