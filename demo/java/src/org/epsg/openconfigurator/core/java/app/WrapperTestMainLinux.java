/* License information to be added */

package org.epsg.openconfigurator.core.java.app;

import org.epsg.openconfigurator.lib.wrapper.BuildConfigurationSetting;
import org.epsg.openconfigurator.lib.wrapper.ControlledNode;
import org.epsg.openconfigurator.lib.wrapper.ManagingNode;
import org.epsg.openconfigurator.lib.wrapper.Network;
import org.epsg.openconfigurator.lib.wrapper.ProjectManager;
import org.epsg.openconfigurator.lib.wrapper.Result;
import org.epsg.openconfigurator.lib.wrapper.SettingsCollection;
import org.epsg.openconfigurator.lib.wrapper.StringCollection;

public class WrapperTestMainLinux {

	// Load openCONFIGURATOR core and dependent libraries
	static {

		System.loadLibrary("boost_date_time");
		System.loadLibrary("boost_system");
		System.loadLibrary("boost_chrono");
		System.loadLibrary("boost_filesystem");
		System.loadLibrary("boost_thread");
		System.loadLibrary("boost_log");
		System.loadLibrary("boost_log_setup");
		System.loadLibrary("openconfigurator_core_lib");
		System.loadLibrary("openconfigurator_core_wrapper_java");
	}

	public static void main(String[] args) {
		// Retrieve main managing class of the library
		ProjectManager man = ProjectManager.GetInstance();
		
		// Init logger class with configuration file path
		man.InitLoggingConfiguration("boost_log_settings.ini");

		// Create an initial network
		Network net = new Network("TestNetwork");
		net.SetCycleTime(10000); // Set Cycle time
		System.out.println(net.GetNetworkId()); // Print UUID to console

		// Add two nodes to the network
		net.AddNode(new ManagingNode("MN"));
		net.AddNode(new ControlledNode((short) 1, "CN"));

		// Add network to the managing class
		man.AddNetwork(net.GetNetworkId(), net);

				// Get a node object from the library
				ControlledNode node = new ControlledNode((short)0);
				Result res = net.GetNode((short) 1, node);
				System.out.println(res.IsSuccessful());
				System.out.println(node.GetNodeName());

		// Get a network from the library
		Network net2 = new Network();
		res = man.GetNetwork(net.GetNetworkId(), net2);
		System.out.println(res.IsSuccessful()); // should be true
		System.out.println(net2.GetNetworkId()); // should be the same
		System.out.println(net2.GetCycleTime()); // should be 10000

		StringCollection support = man.GetSupportedSettingIds();
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
		System.out.println(mnMapping_all.GetInfo());
		System.out.println(nodeAssignment_all.GetInfo());
		System.out.println(presTimeOut_all.GetInfo());

		// Create settings configuration
		/*
		 * <AutoGenerationSettings id="GenerateAll"> <Setting
		 * name="MN_MAPPING_FOR_ALL_NODES" enabled="true"/> <Setting
		 * name="MN_NODE_ASSIGNMENT_FOR_ALL_NODES" enabled="true"/> <Setting
		 * name="MN_PRES_TIMEOUT_FOR_ALL_NODES" enabled="true"/>
		 * </AutoGenerationSettings>
		 */
		net2.AddConfiguration("all");
		net2.AddConfigurationSetting("all", mnMapping_all);
		net2.AddConfigurationSetting("all", nodeAssignment_all);
		net2.AddConfigurationSetting("all", presTimeOut_1);

		// Create specific settings configuration
		/*
		 * <AutoGenerationSettings id="SpecificConfiguration"> <Setting
		 * name="MN_MAPPING_FOR_NODE" value="1" enabled="true"/> <Setting
		 * name="MN_NODE_ASSIGNMENT_FOR_NODE" value="1" enabled="true"/>
		 * <Setting name="MN_PRES_TIMEOUT_FOR_NODE" value="1" enabled="true"/>
		 * </AutoGenerationSettings>
		 */
		net2.AddConfiguration("custom");
		net2.AddConfigurationSetting("custom", mnMapping_1);
		net2.AddConfigurationSetting("custom", nodeAssignment_1);
		net2.AddConfigurationSetting("custom", presTimeOut_all);

		SettingsCollection coll = new SettingsCollection();
		System.out.println(net2.GetConfigurationSettings("all", coll));
		System.out.println(coll.size());
		System.out.println(net2.SetConfigurationSettingEnabled("all",
				"GENERATE_MN_MAPPING_FOR_ALL_NODES", false));

		net2.AddConfiguration("none");
		net2.AddConfigurationSetting("none", mnMapping_all_disabled);
		net2.AddConfigurationSetting("none", nodeAssignment_all_disabled);
		net2.AddConfigurationSetting("none", presTimeOut_all_disabled);

	}

}
