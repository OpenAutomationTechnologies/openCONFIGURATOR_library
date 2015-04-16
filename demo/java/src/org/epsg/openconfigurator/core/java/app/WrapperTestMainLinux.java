/* License information to be added */

package org.epsg.openconfigurator.core.java.app;

import org.epsg.openconfigurator.lib.wrapper.ControlledNode;
import org.epsg.openconfigurator.lib.wrapper.Network;
import org.epsg.openconfigurator.lib.wrapper.ProjectManager;
import org.epsg.openconfigurator.lib.wrapper.Result;

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
ProjectManager man = ProjectManager.GetInstance();
            Network net = new Network("test");
            System.out.println(net.GetNetworkId());

            net.SetCycleTime(10000);
			
            ControlledNode nodeT = new ControlledNode((short)10, "TestNode");
            net.AddNode(nodeT);

            ControlledNode node = new ControlledNode((short)1);
            Result res1 = net.GetNode((short)10, node);
            System.out.println(res1.IsSuccessful());

            man.AddNetwork(net.GetNetworkId(), net);
            Network net2 = new Network();
            Result res = man.GetNetwork(net.GetNetworkId(), net2);

            System.out.println(res.IsSuccessful());
            System.out.println(net2.GetNetworkId());
            System.out.println(net2.GetCycleTime());

	}

}
