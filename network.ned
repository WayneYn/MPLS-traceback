//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
//
//
// TODO documentation
//
simple Host
{
    parameters:
        int nodeIndex;
        string IP;
        volatile double sendIaTime @unit(s) = default(exponential(1s));
        @display("i=device/pc2");

    gates:
        output out[];
        input in[];
}

//
// TODO documentation
//
simple Switch
{
    parameters:
        int nodeIndex;
        @display("i=device/switch");

    gates:
        output out[];
        input in[];
}

//
// TODO documentation
//
simple Router
{
    parameters:
        int nodeIndex;
        @display("i=old/srouter");

    gates:
        output out[];
        input in[];
}

//
// TODO documentation
//
network Network
{
    @display("bgb=876,543");
    submodules:
        r1: Router {
            parameters:
                nodeIndex = 0;
                @display("p=215,182");
        }
        r2: Router {
            parameters:
                nodeIndex = 1;
                @display("p=334,112");
        }
        r3: Router {
            parameters:
                nodeIndex = 2;
                @display("p=334,259");
        }
        r4: Router {
            parameters:
                nodeIndex = 3;
                @display("p=440,194");
        }
        r5: Router {
            parameters:
                nodeIndex = 4;
                @display("p=583,224");
        }
        
        s1: Switch {
            parameters:
                nodeIndex = 5;
                @display("p=120,165");
        }
        s2: Switch {
            parameters:
                nodeIndex = 6;
                @display("p=647,160");
        }
        s3: Switch {
            parameters:
                nodeIndex = 7;
                @display("p=590,310");
        }
        s4: Switch {
            parameters:
                nodeIndex = 8;
                @display("p=510,383");
        }
        s5: Switch {
            parameters:
                nodeIndex = 9;
                @display("p=647,400");
        }

        h1: Host {
            parameters:
                nodeIndex = 10;
                IP = "10.0.0.1";
                @display("p=48,112");
        }
        h2: Host {
            parameters:
                nodeIndex = 11;
                IP = "10.0.0.2";
                @display("p=54,224");
        }
        h3: Host {
            parameters:
                nodeIndex = 12;
                IP = "10.0.1.1";
                @display("p=240,57");
        }
        h4: Host {
            parameters:
                nodeIndex = 13;
                IP = "10.0.2.1";
                @display("p=223,337");
        }
        h5: Host {
            parameters:
                nodeIndex = 14;
                IP = "10.0.3.1";
                @display("p=615,77");
        }
        h6: Host {
            parameters:
                nodeIndex = 15;
                IP = "10.0.3.2";
                @display("p=747,140");
        }
        h7: Host {
            parameters:
                nodeIndex = 16;
                IP = "10.0.4.1";
                @display("p=407,382");
        }
        h8: Host {
            parameters:
                nodeIndex = 17;
                IP = "10.0.4.2";
                @display("p=439,478");
        }
        h9: Host {
            parameters:
                nodeIndex = 18;
                IP = "10.0.4.3";
                @display("p=599,478");
        }
        h10: Host {
            parameters:
                nodeIndex = 19;
                IP = "10.0.4.4";
                @display("p=747,401");
        }
        h11: Host {
            parameters:
                nodeIndex = 20;
                IP = "10.0.5.1";
                @display("p=452,54");
        }


    connections:

        h1.out++ --> s1.in++;
        s1.out++ --> h1.in++;
        
        s1.out++ --> h2.in++;
        h2.out++ --> s1.in++;
        
        s1.out++ --> r1.in++;
        r1.out++ --> s1.in++;
        
        r1.out++ --> r2.in++;
        r2.out++ --> r1.in++;
        
        r2.out++ --> r4.in++;
        r4.out++ --> r2.in++;
        
        r1.out++ --> r3.in++;
        r3.out++ --> r1.in++;
        
        r3.out++ --> r4.in++;
        r4.out++ --> r3.in++;
        
        r4.out++ --> r5.in++;
        r5.out++ --> r4.in++;
        
        r2.out++ --> h3.in++;
        h3.out++ --> r2.in++;
        
        r3.out++ --> h4.in++;
        h4.out++ --> r3.in++;
        
        r4.out++ --> h11.in++;
        h11.out++ --> r4.in++;
        
        r5.out++ --> s2.in++;
        s2.out++ --> r5.in++;
        
        s2.out++ --> h5.in++;
        h5.out++ --> s2.in++;
        
        s2.out++ --> h6.in++;
        h6.out++ --> s2.in++;
        
        r5.out++ --> s3.in++;
        s3.out++ --> r5.in++;
        
        s3.out++ --> s4.in++;
        s4.out++ --> s3.in++;
        
        s3.out++ --> s5.in++;
        s5.out++ --> s3.in++;
        
        s4.out++ --> h7.in++;
        h7.out++ --> s4.in++;
        
        s4.out++ --> h8.in++;
        h8.out++ --> s4.in++;
        
        s5.out++ --> h9.in++;
        h9.out++ --> s5.in++;
        
        h10.out++ --> s5.in++;
        s5.out++ --> h10.in++;
}

