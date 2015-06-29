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

public class WrapperTestMainMinGW {

	// Load openCONFIGURATOR core libraries
	static {
		System.loadLibrary("libboost_date_time-mgw48-mt-1_54");
		System.loadLibrary("libboost_system-mgw48-mt-1_54");
		System.loadLibrary("libboost_chrono-mgw48-mt-1_54");
		System.loadLibrary("libboost_filesystem-mgw48-mt-1_54");
		System.loadLibrary("libboost_thread-mgw48-mt-1_54");
		System.loadLibrary("libboost_log-mgw48-mt-1_54");
		System.loadLibrary("libboost_log_setup-mgw48-mt-1_54");
		System.loadLibrary("libopenconfigurator_core_lib");
		System.loadLibrary("libopenconfigurator_core_wrapper_java");
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
		core.CreateNode("test", (short) 1, "CN");

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

		// Create settings for a configuration
		BuildConfigurationSetting mnMapping_all = new BuildConfigurationSetting(
				"GENERATE_MN_MAPPING_FOR_NODES", "all");
		BuildConfigurationSetting nodeAssignment_all = new BuildConfigurationSetting(
				"GENERATE_MN_NODE_ASSIGNMENT_FOR_NODES", "all");
		BuildConfigurationSetting presTimeOut_all = new BuildConfigurationSetting(
				"GENERATE_MN_PRES_TIMEOUT_FOR_NODES", "all");

		BuildConfigurationSetting mnMapping_1 = new BuildConfigurationSetting(
				"GENERATE_MN_MAPPING_FOR_NODEs", "1");
		BuildConfigurationSetting nodeAssignment_1 = new BuildConfigurationSetting(
				"GENERATE_MN_NODE_ASSIGNMENT_FOR_NODEs", "1");
		BuildConfigurationSetting presTimeOut_1 = new BuildConfigurationSetting(
				"GENERATE_MN_PRES_TIMEOUT_FOR_NODES", "1");

		BuildConfigurationSetting mnMapping_all_disabled = new BuildConfigurationSetting(
				"GENERATE_MN_MAPPING_FOR_NODES", "all");
		mnMapping_all_disabled.SetEnabled(false);
		BuildConfigurationSetting nodeAssignment_all_disabled = new BuildConfigurationSetting(
				"GENERATE_MN_NODE_ASSIGNMENT_FOR_NODES", "all");
		nodeAssignment_all_disabled.SetEnabled(false);
		BuildConfigurationSetting presTimeOut_all_disabled = new BuildConfigurationSetting(
				"GENERATE_MN_PRES_TIMEOUT_FOR_NODES", "all");
		presTimeOut_all_disabled.SetEnabled(false);

		// Enable all the settings (are enabled per default)
		mnMapping_all.SetEnabled(true);
		nodeAssignment_all.SetEnabled(true);
		presTimeOut_all.SetEnabled(true);

		// Print setting info
		System.out.println(mnMapping_all.GetDescription());
		System.out.println(nodeAssignment_all.GetDescription());
		System.out.println(presTimeOut_all.GetDescription());

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

		core.CreateObject("test", (short) 1, 0x1600, ObjectType.RECORD,"objectName", PlkDataType.UNSIGNED16,
				AccessType.RW, PDOMapping.RPDO, "1000",
				"Mapping Object");
		core.CreateSubObject("test", (short) 1, 0x1600, 0, ObjectType.DEFTYPE,"objectName",
				PlkDataType.UNSIGNED8, AccessType.RW, PDOMapping.NO, "0", "NrOfEntries");
		core.CreateDomainSubObject("test", (short) 1, 0x1600, 0x01, ObjectType.DEFSTRUCT, "Domain",	"UID_DOM_Index2100_Sub1E");

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
	}

}
