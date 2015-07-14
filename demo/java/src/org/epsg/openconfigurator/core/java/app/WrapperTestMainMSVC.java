package org.epsg.openconfigurator.core.java.app;

import java.io.IOException;
import java.nio.charset.Charset;
import java.nio.file.Files;
import java.nio.file.Paths;

import org.epsg.openconfigurator.lib.wrapper.AccessType;
import org.epsg.openconfigurator.lib.wrapper.BuildConfigurationSetting;
import org.epsg.openconfigurator.lib.wrapper.ControlledNode;
import org.epsg.openconfigurator.lib.wrapper.IEC_Datatype;
import org.epsg.openconfigurator.lib.wrapper.Network;
import org.epsg.openconfigurator.lib.wrapper.ObjectType;
import org.epsg.openconfigurator.lib.wrapper.OpenConfiguratorCore;
import org.epsg.openconfigurator.lib.wrapper.PDOMapping;
import org.epsg.openconfigurator.lib.wrapper.ParameterAccess;
import org.epsg.openconfigurator.lib.wrapper.PlkDataType;
import org.epsg.openconfigurator.lib.wrapper.Result;
import org.epsg.openconfigurator.lib.wrapper.SettingsCollection;
import org.epsg.openconfigurator.lib.wrapper.StringCollection;

public class WrapperTestMainMSVC {

	// Load openCONFIGURATOR core libraries
	static {
		System.loadLibrary("boost_date_time-vc110-mt-1_54");
		System.loadLibrary("boost_system-vc110-mt-1_54");
		System.loadLibrary("boost_chrono-vc110-mt-1_54");
		System.loadLibrary("boost_filesystem-vc110-mt-1_54");
		System.loadLibrary("boost_thread-vc110-mt-1_54");
		System.loadLibrary("boost_log-vc110-mt-1_54");
		System.loadLibrary("boost_log_setup-vc110-mt-1_54");
		System.loadLibrary("openconfigurator_core_lib");
		System.loadLibrary("openconfigurator_core_wrapper_java");
	}

	public static void main(String[] args) {
		// Retrieve main managing class of the library
		OpenConfiguratorCore core = OpenConfiguratorCore.GetInstance();

		// Init logger class with configuration file path
		try {
			byte[] encoded = Files.readAllBytes(Paths
					.get("boost_log_settings.ini"));

			String config = new String(encoded, Charset.defaultCharset());
			core.InitLoggingConfiguration(config);
		} catch (IOException e) {
			e.printStackTrace();
		}
		core.CreateNetwork("test");

		// Create an initial network
		Network net = new Network();
		core.CreateNetwork("test");
		core.SetCycleTime("test", 10000); // Set Cycle time
		core.GetNetwork("test", net);
		System.out.println(net.GetNetworkId()); // Print UUID to console

		// Add two nodes to the network
		core.CreateNode("test", (short) 240, "MN");

		// Get a node object from the library
		ControlledNode node = new ControlledNode((short)0);
		Result res = core.GetControlledNode("test", (short) 1, node);
		System.out.println(res.IsSuccessful());
		System.out.println(node.GetName());

		// Get a network from the library
		Network net2 = new Network();
		res = core.GetNetwork("test", net2);
		System.out.println(res.IsSuccessful()); // should be true
		System.out.println(net2.GetNetworkId()); // should be the same
		System.out.println(net2.GetCycleTime()); // should be 10000

		StringCollection support = new StringCollection();
		res = core.GetSupportedSettingIds(support);
		for (int i = 0; i < support.size(); i++) {
			System.out.println(support.get(i));

		}

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

		SettingsCollection coll = new SettingsCollection();
		System.out.println(core.GetConfigurationSettings("test", "all", coll).GetErrorType());
		System.out.println(coll.size());
		System.out.println(core.SetConfigurationSettingEnabled("test", "all",
				"GENERATE_MN_MAPPING_FOR_ALL_NODES", false).GetErrorType());

		core.CreateConfiguration("test", "none");
		
		core.CreateObject("test", (short) 240, 0x1F81, ObjectType.RECORD, "objectName", PlkDataType.UNDEFINED, AccessType.UNDEFINED, PDOMapping.UNDEFINED, "", "");
        core.CreateSubObject("test", (short) 240, 0x1F81, 0x0, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "3", "3");
        core.CreateSubObject("test", (short) 240, 0x1F81, (short)1, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
        core.CreateSubObject("test", (short) 240, 0x1F81, (short)2, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
        core.CreateSubObject("test", (short) 240, 0x1F81, (short) 5, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
        core.CreateSubObject("test", (short) 240, 0x1F81, (short) 10, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
        core.CreateSubObject("test", (short) 240, 0x1F81, (short)20, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "0");

        core.CreateObject("test", (short) 240, 0x1600, ObjectType.RECORD, "objectName", PlkDataType.UNDEFINED, AccessType.UNDEFINED, PDOMapping.UNDEFINED, "", "");
        core.CreateSubObject("test", (short) 240, 0x1600, 0x0, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "0", "10");
        core.CreateSubObject("test", (short) 240, 0x1600, 0x1, ObjectType.VAR, "testName", PlkDataType.UNSIGNED16, AccessType.RW, PDOMapping.DEFAULT, "0", "100");
        core.CreateSubObject("test", (short) 240, 0x1600, 0x2, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "100");
        core.CreateSubObject("test", (short) 240, 0x1600, 0x3, ObjectType.VAR, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.DEFAULT, "0", "0x0010000000202104");

        core.CreateObject("test", (short) 240, 0x1F26, ObjectType.RECORD, "objectName", PlkDataType.UNDEFINED, AccessType.UNDEFINED, PDOMapping.UNDEFINED, "", "");
        core.CreateSubObject("test", (short) 240, 0x1F26, (short)0, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "4", "4");
        core.CreateSubObject("test", (short) 240, 0x1F26, (short)1, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
        core.CreateSubObject("test", (short) 240, 0x1F26, (short) 5, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
        core.CreateSubObject("test", (short) 240, 0x1F26, (short) 10, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
        core.CreateSubObject("test", (short) 240, 0x1F26, (short)20, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "0");

        core.CreateObject("test", (short) 240, 0x1F27, ObjectType.RECORD, "objectName", PlkDataType.UNDEFINED, AccessType.UNDEFINED, PDOMapping.UNDEFINED, "", "");
        core.CreateSubObject("test", (short) 240, 0x1F27, (short)0, ObjectType.VAR, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "4", "4");
        core.CreateSubObject("test", (short) 240, 0x1F27, (short)1, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
        core.CreateSubObject("test", (short) 240, 0x1F27, (short)5, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
        core.CreateSubObject("test", (short) 240, 0x1F27, (short)10, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
        core.CreateSubObject("test", (short) 240, 0x1F27, (short)20, ObjectType.VAR, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
        
        core.CreateNode("test", (short) 10, "Redundant_MN");
		
        core.CreateObject("test", (short) 10, 0x1020, ObjectType.RECORD, "objectName", PlkDataType.UNDEFINED, AccessType.UNDEFINED, PDOMapping.UNDEFINED, "", "");
        core.CreateSubObject("test", (short) 10, 0x1020, (short)0, ObjectType.DEFTYPE, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "2", "2");
        core.CreateSubObject("test", (short) 10, 0x1020, (short)1, ObjectType.DEFTYPE, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
        core.CreateSubObject("test", (short) 10, 0x1020, (short)2, ObjectType.DEFTYPE, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "0");

        core.CreateNode("test", (short)20, "Redundant_MN");
        core.CreateObject("test", (short)20, 0x1020, ObjectType.RECORD, "objectName", PlkDataType.UNDEFINED, AccessType.UNDEFINED, PDOMapping.UNDEFINED, "", "");
        core.CreateSubObject("test", (short)20, 0x1020, (short)0, ObjectType.DEFTYPE, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "2", "2");
        core.CreateSubObject("test",(short) 20, 0x1020, (short)1, ObjectType.DEFTYPE, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
        core.CreateSubObject("test", (short)20, 0x1020, (short)2, ObjectType.DEFTYPE, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
        
        core.CreateNode("test", (short)1, "CN");
		
        core.CreateObject("test", (short)1, 0x1020, ObjectType.RECORD, "objectName", PlkDataType.UNDEFINED, AccessType.UNDEFINED, PDOMapping.UNDEFINED, "", "");
        core.CreateSubObject("test", (short)1, 0x1020, (short)0, ObjectType.DEFTYPE, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "2", "2");
        core.CreateSubObject("test", (short)1, 0x1020, 1, ObjectType.DEFTYPE, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
        core.CreateSubObject("test", (short)1, 0x1020, 2, ObjectType.DEFTYPE, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "0");

        core.CreateNode("test", (short) 5, "Redundant_MN");

        core.CreateObject("test", (short) 5, 0x1020, ObjectType.RECORD, "objectName", PlkDataType.UNDEFINED, AccessType.UNDEFINED, PDOMapping.UNDEFINED, "", "");
        core.CreateSubObject("test", (short) 5, 0x1020, 0, ObjectType.DEFTYPE, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "2", "2");
        core.CreateSubObject("test", (short) 5, 0x1020, 1, ObjectType.DEFTYPE, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "0");
        core.CreateSubObject("test", (short) 5, 0x1020, 2, ObjectType.DEFTYPE, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "0");

        core.CreateObject("test", (short) 5, 0x1600, ObjectType.RECORD, "objectName", PlkDataType.UNDEFINED, AccessType.UNDEFINED, PDOMapping.UNDEFINED, "", "");
        core.CreateSubObject("test", (short) 5, 0x1600, 0, ObjectType.DEFTYPE, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.DEFAULT, "0", "10");
        core.CreateSubObject("test", (short) 5, 0x1600, 1, ObjectType.DEFTYPE, "testName", PlkDataType.UNSIGNED16, AccessType.RW, PDOMapping.DEFAULT, "0", "100");
        core.CreateSubObject("test", (short) 5, 0x1600, 2, ObjectType.DEFTYPE, "testName", PlkDataType.UNSIGNED24, AccessType.RW, PDOMapping.DEFAULT, "0", "100");
        core.CreateSubObject("test", (short) 5, 0x1600, 3, ObjectType.DEFTYPE, "testName", PlkDataType.UNSIGNED32, AccessType.RW, PDOMapping.DEFAULT, "0", "100");
        core.CreateSubObject("test", (short) 5, 0x1600, 4, ObjectType.DEFTYPE, "testName", PlkDataType.UNSIGNED40, AccessType.RW, PDOMapping.DEFAULT, "0", "100");
        core.CreateSubObject("test", (short) 5, 0x1600, 6, ObjectType.DEFTYPE, "testName", PlkDataType.UNSIGNED56, AccessType.RW, PDOMapping.NO, "", "100");
        core.CreateSubObject("test", (short) 5, 0x1600, 7, ObjectType.DEFTYPE, "testName", PlkDataType.UNSIGNED64, AccessType.RW, PDOMapping.NO, "0", "0x0010000000202104");

        core.CreateObject("test", (short) 5, 0x1602, ObjectType.RECORD, "objectName", PlkDataType.UNDEFINED, AccessType.UNDEFINED, PDOMapping.UNDEFINED, "", "");
        core.CreateSubObject("test", (short) 5, 0x1602, 0, ObjectType.DEFTYPE, "testName", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.NO, "0", "10");
        core.CreateSubObject("test", (short) 5, 0x1602, 1, ObjectType.DEFTYPE, "testName", PlkDataType.INTEGER16, AccessType.RW, PDOMapping.NO, "0", "100");
        core.CreateSubObject("test", (short) 5, 0x1602, 2, ObjectType.DEFTYPE, "testName", PlkDataType.INTEGER24, AccessType.RW, PDOMapping.NO, "0", "100");
        core.CreateSubObject("test", (short) 5, 0x1602, 3, ObjectType.DEFTYPE, "testName", PlkDataType.INTEGER32, AccessType.RW, PDOMapping.NO, "", "100");
        core.CreateSubObject("test", (short) 5, 0x1602, 4, ObjectType.DEFTYPE, "testName", PlkDataType.INTEGER40, AccessType.RW, PDOMapping.NO, "", "100");
        core.CreateSubObject("test", (short) 5, 0x1602, 5, ObjectType.DEFTYPE, "testName", PlkDataType.INTEGER48, AccessType.RW, PDOMapping.NO, "", "100");
        core.CreateSubObject("test", (short) 5, 0x1602, 6, ObjectType.DEFTYPE, "testName", PlkDataType.INTEGER56, AccessType.RW, PDOMapping.NO, "", "100");
        core.CreateSubObject("test", (short) 5, 0x1602, 7, ObjectType.DEFTYPE, "testName", PlkDataType.INTEGER64, AccessType.RW, PDOMapping.NO, "0", "0x0010000000202104");

        core.CreateObject("test", (short) 5, 0x1610, ObjectType.DEFTYPE, "Name", PlkDataType.UNDEFINED, AccessType.UNDEFINED, PDOMapping.UNDEFINED, "", "");
        core.CreateSubObject("test", (short) 5, 0x1610, 0x00, ObjectType.DEFTYPE, "Name", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.RPDO, "0", "15");
        core.CreateSubObject("test", (short) 5, 0x1610, 0x0d, ObjectType.DEFTYPE, "Name", PlkDataType.UNICODE_STRING, AccessType.RW, PDOMapping.RPDO, "100", "WriteThisString");
        core.CreateSubObject("test", (short) 5, 0x1610, 0x01, ObjectType.DEFTYPE, "Name", PlkDataType.VISIBLE_STRING, AccessType.RW, PDOMapping.RPDO, "100", "WriteThisString");
        core.CreateSubObject("test", (short) 5, 0x1610, 0x02, ObjectType.DEFTYPE, "Name", PlkDataType.OCTET_STRING, AccessType.RW, PDOMapping.RPDO, "100", "WriteThisString");
        core.CreateSubObject("test", (short) 5, 0x1610, 0x03, ObjectType.DEFTYPE, "Name", PlkDataType.IP_ADDRESS, AccessType.RW, PDOMapping.RPDO, "100", "0x64646464");
        core.CreateSubObject("test", (short) 5, 0x1610, 0x0b, ObjectType.DEFTYPE, "Name", PlkDataType.IP_ADDRESS, AccessType.RW, PDOMapping.RPDO, "100", "254.254.254.254");
        core.CreateSubObject("test", (short) 5, 0x1610, 0x0c, ObjectType.DEFTYPE, "Name", PlkDataType.IP_ADDRESS, AccessType.RW, PDOMapping.RPDO, "100", "1.1.1.1");
        core.CreateSubObject("test", (short) 5, 0x1610, 0x04, ObjectType.DEFTYPE, "Name", PlkDataType.BOOLEAN, AccessType.RW, PDOMapping.RPDO, "false", "true");
        core.CreateSubObject("test", (short) 5, 0x1610, 0x05, ObjectType.DEFTYPE, "Name", PlkDataType.MAC_ADDRESS, AccessType.RW, PDOMapping.RPDO, "0", "0x000A086121012");
        core.CreateSubObject("test", (short) 5, 0x1610, 0x06, ObjectType.DEFTYPE, "Name", PlkDataType.REAL32, AccessType.RW, PDOMapping.RPDO, "0", "1267.43233E12");
        core.CreateSubObject("test", (short) 5, 0x1610, 0x07, ObjectType.DEFTYPE, "Name", PlkDataType.REAL32, AccessType.RW, PDOMapping.RPDO, "0", "1.234532432");
        core.CreateSubObject("test", (short) 5, 0x1610, 0x08, ObjectType.DEFTYPE, "Name", PlkDataType.REAL64, AccessType.RW, PDOMapping.RPDO, "0", "-1E4");
        core.CreateSubObject("test", (short) 5, 0x1610, 0x09, ObjectType.DEFTYPE, "Name", PlkDataType.REAL64, AccessType.RW, PDOMapping.RPDO, "0", "12.78e-2");
        core.CreateSubObject("test", (short) 5, 0x1610, 0x0a, ObjectType.DEFTYPE, "Name", PlkDataType.REAL64, AccessType.RW, PDOMapping.RPDO, "6", "-0");
        core.CreateSubObject("test", (short) 5, 0x1610, 0x10, ObjectType.DEFTYPE, "Name", PlkDataType.REAL64, AccessType.RW, PDOMapping.RPDO, "6", "-0");

        core.CreateObject("test", (short) 5, 0x1A00, ObjectType.DEFTYPE, "Name", PlkDataType.UNDEFINED, AccessType.UNDEFINED, PDOMapping.UNDEFINED, "", "");
        core.CreateSubObject("test", (short) 5, 0x1A00, 0x00, ObjectType.DEFTYPE, "Name", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.RPDO, "0", "5");
        core.CreateSubObject("test", (short) 5, 0x1A00, 0x0d, ObjectType.DEFTYPE, "Name", PlkDataType.UNICODE_STRING, AccessType.RW, PDOMapping.RPDO, "100", "WriteThisString");
        core.CreateSubObject("test", (short) 5, 0x1A00, 0x01, ObjectType.DEFTYPE, "Name", PlkDataType.VISIBLE_STRING, AccessType.RW, PDOMapping.RPDO, "100", "WriteThisString");
        core.CreateSubObject("test", (short) 5, 0x1A00, 0x02, ObjectType.DEFTYPE, "Name", PlkDataType.OCTET_STRING, AccessType.RW, PDOMapping.RPDO, "100", "WriteThisString");
        core.CreateSubObject("test", (short) 5, 0x1A00, 0x03, ObjectType.DEFTYPE, "Name", PlkDataType.IP_ADDRESS, AccessType.RW, PDOMapping.RPDO, "100", "0x64646464");
        core.CreateSubObject("test", (short) 5, 0x1A00, 0x0b, ObjectType.DEFTYPE, "Name", PlkDataType.IP_ADDRESS, AccessType.RW, PDOMapping.RPDO, "100", "254.254.254.254");
        core.CreateSubObject("test", (short) 5, 0x1A00, 0x0c, ObjectType.DEFTYPE, "Name", PlkDataType.IP_ADDRESS, AccessType.RW, PDOMapping.RPDO, "100", "1.1.1.1");
        core.CreateSubObject("test", (short) 5, 0x1A00, 0x04, ObjectType.DEFTYPE, "Name", PlkDataType.BOOLEAN, AccessType.RW, PDOMapping.RPDO, "false", "true");
        core.CreateSubObject("test", (short) 5, 0x1A00, 0x05, ObjectType.DEFTYPE, "Name", PlkDataType.MAC_ADDRESS, AccessType.RW, PDOMapping.RPDO, "0", "0x000A086121012");
        core.CreateSubObject("test", (short) 5, 0x1A00, 0x06, ObjectType.DEFTYPE, "Name", PlkDataType.REAL32, AccessType.RW, PDOMapping.RPDO, "0", "1267.43233E12");
        core.CreateSubObject("test", (short) 5, 0x1A00, 0x07, ObjectType.DEFTYPE, "Name", PlkDataType.REAL32, AccessType.RW, PDOMapping.RPDO, "0", "1.234532432");
        core.CreateSubObject("test", (short) 5, 0x1A00, 0x08, ObjectType.DEFTYPE, "Name", PlkDataType.REAL64, AccessType.RW, PDOMapping.RPDO, "0", "-1E4");
        core.CreateSubObject("test", (short) 5, 0x1A00, 0x09, ObjectType.DEFTYPE, "Name", PlkDataType.REAL64, AccessType.RW, PDOMapping.RPDO, "0", "12.78e-2");
        core.CreateSubObject("test", (short) 5, 0x1A00, 0x0a, ObjectType.DEFTYPE, "Name", PlkDataType.REAL64, AccessType.RW, PDOMapping.RPDO, "6", "-0");
        core.CreateSubObject("test", (short) 5, 0x1A00, 0x10, ObjectType.DEFTYPE, "Name", PlkDataType.REAL64, AccessType.RW, PDOMapping.RPDO, "6", "-0");

        core.CreateObject("test", (short) 5, 0x1400, ObjectType.DEFTYPE, "Name", PlkDataType.UNDEFINED, AccessType.UNDEFINED, PDOMapping.UNDEFINED, "", "");
        core.CreateSubObject("test", (short) 5, 0x1400, 0x00, ObjectType.DEFTYPE, "Name", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.RPDO, "0", "15");
        core.CreateSubObject("test", (short) 5, 0x1400, 0x01, ObjectType.DEFTYPE, "Name", PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.RPDO, "100", "5");

        core.CreateObject("test", (short) 5, 0x2000, ObjectType.RECORD, "objectName", PlkDataType.UNDEFINED, AccessType.UNDEFINED, PDOMapping.UNDEFINED, "", "");
        core.CreateSubObject("test", (short) 5, 0x2000, 0x0b, ObjectType.DEFTYPE, "Name", PlkDataType.IP_ADDRESS, AccessType.RW, PDOMapping.RPDO, "100", "254.254.254.254");

		core.CreateObject("test", (short) 1, 0x2000, ObjectType.RECORD,"objectName", PlkDataType.UNSIGNED16,
				AccessType.RW, PDOMapping.RPDO, "1000",
				"Mapping Object");
		core.CreateSubObject("test", (short) 1, 0x2000, 0x0, ObjectType.DEFTYPE,"objectName",
				PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.NO, "0", "NrOfEntries");
		core.CreateDomainSubObject("test", (short) 1, 0x2000, 0x1, ObjectType.DEFSTRUCT, "Domain",	"UID_DOM_Index2100_Sub1E");

        Result test = core.CreateParameter("test", (short) 1, "UID_DOM_Index2100_Sub1E", ParameterAccess.read);
        System.out.println(test.IsSuccessful());
        test = core.CreateStructDatatype("test", (short) 1, "notFound", "UID_DT_Index2100_Sub1E", "Index2100_Sub1E");
        test = core.CreateStructDatatype("test", (short)1, "UID_DOM_Index2100_Sub1E", "UID_DT_Index2100_Sub1E", "Index2100_Sub1E");
        System.out.println(test.IsSuccessful());
        test = core.CreateVarDeclaration("test",(short) 1, "notFound", "Index2100_Sub1E_OK", "NetworkStatus", IEC_Datatype.USINT);
        test = core.CreateVarDeclaration("test", (short)1, "UID_DT_Index2100_Sub1E", "Index2100_Sub1E_OK", "NetworkStatus", IEC_Datatype.USINT);
        test = core.CreateVarDeclaration("test", (short)1, "UID_DT_Index2100_Sub1E", "UID_Index2100_Sub1E_StatusInput01", "StatusInput01", IEC_Datatype.BITSTRING, 1);
        test = core.CreateVarDeclaration("test", (short)1, "UID_DT_Index2100_Sub1E", "UID_Index2100_Sub1E_Bit_Unused_01", "StatusInput01", IEC_Datatype.BITSTRING, 1);
        test = core.CreateVarDeclaration("test", (short)1, "UID_DT_Index2100_Sub1E", "UID_Index2100_Sub1E_StatusInput02", "StatusInput01", IEC_Datatype.BITSTRING, 1);
        test = core.CreateVarDeclaration("test", (short)1, "UID_DT_Index2100_Sub1E", "UID_Index2100_Sub1E_Bit_Unused_02", "StatusInput01", IEC_Datatype.BITSTRING, 5);
        System.out.println(test.IsSuccessful());

        test = core.CreateParameter("test", (short)1, "UID_DOM_Index2101_Sub1E", ParameterAccess.read);
        System.out.println(test.IsSuccessful());
        test = core.CreateStructDatatype("test", (short)1, "UID_DOM_Index2101_Sub1E", "UID_DT_Index2101_Sub1E", "Index2101_Sub1E");
        System.out.println(test.IsSuccessful());
        test = core.CreateVarDeclaration("test", (short)1, "UID_DT_Index2101_Sub1E", "Index2101_Sub1E_OK", "NetworkStatus", IEC_Datatype.USINT);
        test = core.CreateVarDeclaration("test", (short)1, "UID_DT_Index2101_Sub1E", "UID_Index2101_Sub1E_DigitalInput01", "DigitalInput01", IEC_Datatype.BITSTRING, 1);
        test = core.CreateVarDeclaration("test", (short)1, "UID_DT_Index2101_Sub1E", "UID_Index2101_Sub1E_DigitalInput02", "DigitalInput02", IEC_Datatype.BITSTRING, 1);
        test = core.CreateVarDeclaration("test",(short) 1, "UID_DT_Index2101_Sub1E", "UID_Index2101_Sub1E_DigitalInput03", "DigitalInput03", IEC_Datatype.BITSTRING, 1);
        test = core.CreateVarDeclaration("test", (short)1, "UID_DT_Index2101_Sub1E", "UID_Index2101_Sub1E_DigitalInput04", "DigitalInput04", IEC_Datatype.BITSTRING, 1);
        test = core.CreateVarDeclaration("test", (short)1, "UID_DT_Index2101_Sub1E", "UID_Index2101_Sub1E_DigitalInput05", "DigitalInput05", IEC_Datatype.BITSTRING, 1);
        test = core.CreateVarDeclaration("test", (short)1, "UID_DT_Index2101_Sub1E", "UID_Index2101_Sub1E_DigitalInput06", "DigitalInput06", IEC_Datatype.BITSTRING, 1);
        test = core.CreateVarDeclaration("test",(short) 1, "UID_DT_Index2101_Sub1E", "UID_Index2101_Sub1E_DigitalInput07", "DigitalInput07", IEC_Datatype.BITSTRING, 1);
        test = core.CreateVarDeclaration("test", (short)1, "UID_DT_Index2101_Sub1E", "UID_Index2101_Sub1E_DigitalInput08", "DigitalInput08", IEC_Datatype.BITSTRING, 1);
        test = core.CreateVarDeclaration("test",(short) 1, "UID_DT_Index2101_Sub1E", "UID_Index2101_Sub1E_DigitalInput09", "DigitalInput09", IEC_Datatype.BITSTRING, 1);
        test = core.CreateVarDeclaration("test", (short)1, "UID_DT_Index2101_Sub1E", "UID_Index2101_Sub1E_DigitalInput10", "DigitalInput10", IEC_Datatype.BITSTRING, 1);
        test = core.CreateVarDeclaration("test",(short) 1, "UID_DT_Index2101_Sub1E", "UID_Index2101_Sub1E_DigitalInput11", "DigitalInput11", IEC_Datatype.BITSTRING, 1);
        test = core.CreateVarDeclaration("test", (short)1, "UID_DT_Index2101_Sub1E", "UID_Index2101_Sub1E_DigitalInput12", "DigitalInput12", IEC_Datatype.BITSTRING, 1);
        test = core.CreateVarDeclaration("test",(short) 1, "UID_DT_Index2101_Sub1E", "UID_Index2101_Sub1E_Bit_Unused_01", "Bit_Unused_01", IEC_Datatype.BITSTRING, 4);
        System.out.println(test.IsSuccessful());
        test = core.CreateParameter("test", (short)1, "UID_100", ParameterAccess.read, IEC_Datatype.UDINT);

        long[] size = new long[1];
        test = core.GetDatatypeSize("test", (short)1, "UID_DT_Index2100_Sub1E", size);
        System.out.println(test.IsSuccessful());
        System.out.println(size[0]);

        test = core.GetDatatypeSize("test", (short)1, "UID_DT_Index2101_Sub1E", size);
        System.out.println(test.IsSuccessful());
        System.out.println(size[0]);

        test = core.GetDatatypeSize("test",(short)1, "UID_100", size);
        System.out.println(test.IsSuccessful());
        System.out.println(size[0]);

        test = core.GetDatatypeSize("test",(short) 1, "notFound", size);
        System.out.println(test.IsSuccessful());
        System.out.println(size[0]);
		
        String[] configOutput = new String[1];
        res = core.BuildConfiguration("test", configOutput);
	}

}
