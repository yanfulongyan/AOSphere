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

package opsphere;
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

simple CoreY
{
    parameters:
        //       int CoreNum = default(4);
        @display("i=block/routing"); // add a default icon
        int FOSRadix;//        int CoreRadix = default( int (sqrt(ServerNum/DownRadix)) );
        int ToRDownRadix;
        int CoreRadix;
        int AgreRadix;
        int ServerNum;
        int NumTx;
        int NumTx_Core;
        int NumTx_ToR2ToR;
        int GroupToRCore;
        double dataRate @unit(bps); //System data rate


    gates:
        input in[];
        output out[];

        output ackout[];
}


simple AgreY
{
    parameters:
        //       int AgreNum = default(4);
        //       int Agre2ToR = default(4);
        @display("i=block/routing"); // add a default icon

//        int AgreRadix = default( int (sqrt(ServerNum/DownRadix)) );
        int FOSRadix;
        int ToRDownRadix;
        int CoreRadix;
        int AgreRadix;
        int ServerNum;
        int NumTx;
        int NumTx_Core;
        int NumTx_ToR2ToR;
        int GroupToR;

    gates:
        input in[];
        output out[];
        output ackout[];
}

module ToRY
{
    parameters:
        //       int ToRNum = default(4);
        //       int ToRServer = default(4);
        @display("i=block/routing"); // add a default icon

        int ToRDownRadix;
        int CoreRadix;
        int AgreRadix;
        int Up2CoreBufferSize;
        int Up2AgreBufferSize;
//        int Down2ServerBufferSize;
        // 		 double BufferSize;
        int ServerNum;
        int NumTx;
        int NumTx_Core;
        int NumTx_ToR2ToR;

    gates:
        input in[];
        output out[];

        input agrein[NumTx];
        output agreout[NumTx];

        input corein[NumTx_Core];
        output coreout[NumTx_Core];
        
        input  Superin[NumTx_ToR2ToR];
        output Superout[NumTx_ToR2ToR];

        input AgreAckIn[NumTx];
        input CoreAckIn[NumTx_Core];
        //input SuperAckIn[SuperNumTx_ToR2ToR];

    submodules:

        HeadProcessor: HeadProcessor {
            @display("p=426,138");
        }
        TxBuffer[NumTx + NumTx_ToR2ToR + NumTx_Core]: TxBuffer {
//        CellIncision[2]:CellIncision;
//        PreambleInsert[5]:PreambleInsert;
//        BurstProcess[2]:BurstProcess;
//        FlowController[2]:FlowController;   

            @display("p=313,63");
        }
    connections:

        for i=0..sizeof(in)-1 {
            in[i] --> HeadProcessor.serverin[i];
            out[i] <-- HeadProcessor.serverout[i];
        }
        // the number of agre modules  
        for i=0..NumTx + NumTx_Core  + NumTx_ToR2ToR -1 {
            HeadProcessor.bufferout[i] --> TxBuffer[i].headprcoessin;
        }
        for i=0..NumTx-1 {
            TxBuffer[i].out --> agreout[i];
            HeadProcessor.OPSin[i] <-- agrein[i];
            TxBuffer[i].ackin <-- AgreAckIn[i];
        }

        for i=0..NumTx_Core-1 {
            TxBuffer[i+NumTx].out --> coreout[i];
            HeadProcessor.OPSin[i+NumTx] <-- corein[i];
            TxBuffer[i+NumTx].ackin <-- CoreAckIn[i];
        }
        
        for i=0..NumTx_ToR2ToR-1 {
            TxBuffer[i+NumTx+ NumTx_Core].out --> Superout[i];
            HeadProcessor.OPSin[i+NumTx+ NumTx_Core] <-- Superin[i];
            //TxBuffer[i+NumTx+ NumTx_ToR2ToR].ackin <-- SuperAckIn[i];
        }
        
}

simple HeadProcessor
{
    parameters:
        int FOSRadix;
        int ServerNum;
        int ToRDownRadix;
        int GroupToR;
        int GroupToRCore;

        int AgreRadix;
        int CoreRadix;
        int NumTx;
        int NumTx_Core;
        int NumTx_ToR2ToR;
        double linkDataRate @unit(bps);		//downlink
        double dataRate @unit(bps);    //uplink
        int CellSize;
        double PreambleInsertdelay;
        double BurstProcessDelay;
        
        int Down2ServerBufferSize;

    gates:
        input serverin[ToRDownRadix];
        output serverout[ToRDownRadix];

        output bufferout[NumTx + NumTx_Core + NumTx_ToR2ToR];

        input OPSin[NumTx + NumTx_Core  + NumTx_ToR2ToR];
        input RadioIn @directIn;
}

simple TxBuffer
{
    parameters:
        int BufferSize2OPS;
        int NumTx;
        int NumTx_Core;
        int NumTx_ToR2ToR;

        int SimulatedPacketNum;
        int CellSize;
        int OpticalPacketNum;
        int GroupToR;
        int GroupToRCore;

        double linkDataRate @unit(bps);		//System data rate
    gates:
        output out;
        input headprcoessin;
        input ackin @loose;
}
//simple CellIncision
//{
//    parameters:
//        double CellSize;    
//        double linkDataRate @unit(bps);		//System data rate
//        
//    gates:
//        output out;
//        input headprcoessin;
//        
//        input ackin;
//}
simple ServerY
{
    parameters:
        //        int 64 = default(64);  //??Y
        @display("i=block/server"); // add a default icon  

        int ToRDownRadix;
        int CoreRadix;
        int AgreRadix;

        int ServerNum;
        double load;
		int FOSRadix;
        int meanBurstSize;
        double linkDataRate @unit(bps);		//System data rate

        int SimulatedPacketNum;
        
        int CellSize;
        double PreambleInsertdelay;
        double BurstProcessDelay;
    gates:
        input in;
        output out;
}
//
network CCF
{
    parameters:
        @display("bgb=427,317");
        //  		int ServerNum = default(32);
        // 		int CoreRadix = default(4);
        // 		int AgreRadix = default(4);
        //  		int DownRadix = default(2);
        int ToRDownRadix;
        int CoreRadix;
        int AgreRadix;

        int ServerNum;
        int NumTx;
        int NumTx_Core;
        int NumTx_ToR2ToR;



    types:
        channel c extends ned.DatarateChannel
        {
            datarate = 10Gbps;
            delay = 15ns;
        }
        channel cc extends ned.DatarateChannel
        {
            datarate = 50Gbps;
            delay = 250ns;
        }

    submodules:
        Server[ServerNum]: ServerY {
            parameters:
                @display("i=,gold"); // do not change the icon (first arg of i=) just colorize it
        }

        Agre[AgreRadix*(AgreRadix*AgreRadix+1)]: AgreY {
            parameters:
                @display("i=,blue");// do not change the icon (first arg of i=) just colorize it
        }
        Core[CoreRadix*(CoreRadix*CoreRadix+1)]: CoreY {
            parameters:
                @display("i=,red"); // do not change the icon (first arg of i=) just colorize it
        }
//        Super[SuperRadix*SuperRadix]: SuperY {
//            parameters:
//                @display("i=,red"); // do not change the icon (first arg of i=) just colorize it
//        }
        ToR[AgreRadix*AgreRadix*(AgreRadix*AgreRadix+1)]: ToRY {
            parameters:
                @display("i=,cyan"); // do not change the icon (first arg of i=) just colorize it
        }

    connections:
        for i=0..ServerNum-1 {
            Server[i].out --> c --> ToR[int(i/ToRDownRadix)].in++;
            Server[i].in <-- c <-- ToR[int(i/ToRDownRadix)].out++;
        }

        for k=0..((AgreRadix*AgreRadix+1)-1), for i=0..AgreRadix*AgreRadix-1, for j=0..NumTx-1 {
            ToR[k*AgreRadix*AgreRadix + i].agreout[j] --> cc --> Agre[k*AgreRadix   + int(i/AgreRadix)].in++;
            ToR[k*AgreRadix*AgreRadix + i].agrein[j]  <-- cc <-- Agre[k*AgreRadix   + int(i/AgreRadix)].out++;
            ToR[k*AgreRadix*AgreRadix + i].AgreAckIn[j] <-- cc <-- Agre[k*AgreRadix + int(i/AgreRadix)].ackout++;
        }

        for m=0..(AgreRadix*AgreRadix+1)-1, for i=0..CoreRadix-1, for j=0..CoreRadix-1, for k=0..NumTx_Core-1 {
            // for server num 16
            Core[m*CoreRadix + i].out++ --> cc --> ToR[m*CoreRadix*CoreRadix + i+j*CoreRadix].corein[k];
            Core[m*CoreRadix + i].in++ <-- cc <-- ToR[m*CoreRadix*CoreRadix + i+j*CoreRadix].coreout[k];
            Core[m*CoreRadix + i].ackout++ --> cc --> ToR[m*CoreRadix*CoreRadix + i+j*CoreRadix].CoreAckIn[k];
        }
        
//        for i=0..SuperRadix*SuperRadix-1, for j=0..CoreRadix-1, for k=0..SuperNumTx_ToR2ToR-1 {
//            // for server num 16
//            Super[i].out++ --> cc --> ToR[i+j*SuperRadix*SuperRadix].Superin[k];
//            Super[i].in++ <-- cc <-- ToR[i+j*SuperRadix*SuperRadix].Superout[k];
//            Super[i].ackout++ --> cc --> ToR[i+j*SuperRadix*SuperRadix].SuperAckIn[k];
//        }
        
        for i=0..(AgreRadix*AgreRadix+1)-1, for j=i..AgreRadix*AgreRadix-1, for k=0..NumTx_ToR2ToR-1 {
            
            ToR[i*AgreRadix*AgreRadix + j].Superout[k] --> cc --> ToR[(j+1)*AgreRadix*AgreRadix + i].Superin[k];
            ToR[i*AgreRadix*AgreRadix + j].Superin[k] <-- cc <--  ToR[(j+1)*AgreRadix*AgreRadix + i].Superout[k];          
        }
         
}