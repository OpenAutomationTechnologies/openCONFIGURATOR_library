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
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace openconfigurator_core_net_app
{
    class Program
    {
        static void Main(string[] args)
        {
            var man = ProjectManager.GetInstance();
            var net = new Network("test");
            Console.WriteLine(net.GetNetworkId());

            net.SetCycleTime(10000);

            var nodeT = new ControlledNode(10, "Testnode");
            net.AddNode(nodeT);

            var node = new ControlledNode(1);
            var res1 = net.GetNode(10, node);
            Console.WriteLine(res1.IsSuccessful());

            man.AddNetwork("test", net);
            var net2 = new Network();
            var res = man.GetNetwork(net.GetNetworkId(), net2);

            Console.WriteLine(res.IsSuccessful());
            Console.WriteLine(net2.GetNetworkId());
            Console.WriteLine(net2.GetCycleTime());

        }
    }
}
