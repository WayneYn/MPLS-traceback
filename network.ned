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
        string address;
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
        @display("i=device/switch");

    gates:
        output out[];
        input in[];
}

//
// TODO documentation
//
simple router
{
    parameters:
        @display("i=abstract/router");
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
        s1: Switch {
            @display("p=120,165");
        }
        s2: Switch {
            @display("p=647,160");
        }
        s3: Switch {
            @display("p=590,310");
        }
        s4: Switch {
            @display("p=510,383");
        }
        s5: Switch {
            @display("p=647,400");
        }


        r1: router {
            @display("p=215,182");
        }
        r2: router {
            @display("p=334,112");
        }
        r3: router {
            @display("p=334,259");
        }
        r4: router {
            @display("p=440,194");
        }
        r5: router {
            @display("p=583,224");
        }


        h1: Host {
            parameters:
                address = "10.0.0.1";
                @display("p=48,112");
        }
        h2: Host {
            parameters:
                address = "10.0.0.2";
                @display("p=54,224");
        }
        h3: Host {
            parameters:
                address = "10.0.1.1";
                @display("p=240,57");
        }
        h4: Host {
            parameters:
                address = "10.0.2.1";
                @display("p=223,337");
        }
        h5: Host {
            parameters:
                address = "10.0.3.1";
                @display("p=615,77");
        }
        h6: Host {
            parameters:
                address = "10.0.3.2";
                @display("p=747,140");
        }
        h7: Host {
            parameters:
                address = "10.0.4.1";
                @display("p=407,382");
        }
        h8: Host {
            parameters:
                address = "10.0.4.2";
                @display("p=439,478");
        }
        h9: Host {
            parameters:
                address = "10.0.4.3";
                @display("p=599,478");
        }
        h10: Host {
            parameters:
                address = "10.0.4.4";
                @display("p=747,401");
        }
        h11: Host {
            parameters:
                address = "10.0.5.1";
                @display("p=452,54");
        }


    connections:

        r1.out++ --> s1.in++;
        r1.out++ --> r2.in++;
        r1.out++ --> r3.in++;

        r2.out++ --> h3.in++;
        r2.out++ --> r1.in++;
        r2.out++ --> r4.in++;

        r3.out++ --> h4.in++;
        r3.out++ --> r1.in++;
        r3.out++ --> r4.in++;

        r4.out++ --> h11.in++;
        r4.out++ --> r2.in++;
        r4.out++ --> r3.in++;
        r4.out++ --> r5.in++;

        r5.out++ --> r4.in++;
        r5.out++ --> s2.in++;
        r5.out++ --> s3.in++;

        s1.out++ --> h1.in++;
        s1.out++ --> h2.in++;
        s1.out++ --> r1.in++;

        s2.out++ --> h5.in++;
        s2.out++ --> h6.in++;
        s2.out++ --> r5.in++;

        s3.out++ --> s4.in++;
        s3.out++ --> s5.in++;
        s3.out++ --> r5.in++;

        s4.out++ --> h7.in++;
        s4.out++ --> h8.in++;
        s4.out++ --> s3.in++;

        s5.out++ --> h9.in++;
        s5.out++ --> h10.in++;
        s5.out++ --> s3.in++;

        h1.out++ --> s1.in++;
        h2.out++ --> s1.in++;
        h3.out++ --> r2.in++;
        h4.out++ --> r3.in++;
        h5.out++ --> s2.in++;
        h6.out++ --> s2.in++;
        h7.out++ --> s4.in++;
        h8.out++ --> s4.in++;
        h9.out++ --> s5.in++;
        h10.out++ --> s5.in++;
        h11.out++ --> r4.in++;

}

