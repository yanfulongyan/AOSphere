/*
 * buffer.cc
 *
 *  Created on: Jul 6, 2015
 *      Author: fyan
 */
/*
 * head.cc
 *
 *  Created on: Jul 3, 2015
 *      Author: fyan
 */

#include <stdio.h>
#include <string.h>
#include <omnetpp.h>
#include "pkt_m.h"
#include "Optical_m.h"
using namespace omnetpp;

class TxBuffer : public cSimpleModule
{
    private:
        int BackupQueueopticalNum;
        int TXSend;

        int Down2ServerBufferSize;
        int ToR2OPS_BufferSize;

        int Queue2CoreAveragelost ;
        int Queue2AgreAveragelost ;
        int Queue2ServerAveragelost ;
        int getGroupToRIndex;
        double HeadProcessdelay;
        int GroupToR;
		int GroupToRCore;
        int OpticalPacketNum;

        double slot;
        int NumTx;
        int NumTx_Core;
		int NumTx_ToR2ToR;
        simtime_t t0;

        pkt *local_clock;

        cQueue Queue2OPS[16];
        cQueue BackupQueue[16];
        bool Queue2OPS_busy;
        int Queue2OPSlost;

        int Queue2OPSPacketlost;

        int Received_Packets ;
        int SimulatedPacketNum;

        int SimulatedClock;
        bool finishflag;

        int test;

        bool QueueArrayflag; // 1 is backup queue;

        double linkDataRate;

        int CellSize;
        double BufferTime;

        int BackupQueueIndex;
    protected:

         virtual void initialize();
         virtual void handleMessage(cMessage *msg);
         virtual void finish();
    public:
         TxBuffer();
         virtual ~TxBuffer();
};

Define_Module(TxBuffer);

TxBuffer::TxBuffer(){
    local_clock = NULL;
}
TxBuffer::~TxBuffer(){
    cancelAndDelete(local_clock);
    for(int i=0;i<16;i++){
        while( !Queue2OPS[i].empty() ){
                    delete (pkt *)Queue2OPS[i].pop();
         }

         while( !BackupQueue[i].empty() ){
                    delete (pkt *)BackupQueue[i].pop();
         }
    }
}
void TxBuffer::initialize()
{
    BackupQueueopticalNum = 0;
    linkDataRate = par("linkDataRate");
    ToR2OPS_BufferSize = par("BufferSize2OPS");
    CellSize = par("CellSize");
    BufferTime =  CellSize*8/linkDataRate;
    GroupToR = par("GroupToR");
	GroupToRCore = par("GroupToRCore");
    NumTx = par("NumTx");
    NumTx_Core = par("NumTx_Core");
	NumTx_ToR2ToR = par("NumTx_ToR2ToR");
    OpticalPacketNum = par("OpticalPacketNum");
//    endToEndDelayVec.setName("End-to-End Dealy");
    Queue2CoreAveragelost = 0;
    Queue2AgreAveragelost = 0;
    Queue2ServerAveragelost = 0;

    Queue2OPSlost = 0;
    Queue2OPSPacketlost = 0;
    finishflag = false;
    QueueArrayflag = false;


    HeadProcessdelay = 80e-9;
    slot = 276e-9;
    t0 = 0;
    Received_Packets = 0;
    SimulatedPacketNum =  par("SimulatedPacketNum");
    SimulatedClock = 0;
    test = 0;

    TXSend = 0;
    local_clock = new pkt ("local_clock");
    local_clock->setIndex(0);
    scheduleAt(t0, local_clock);
}

void TxBuffer::handleMessage(cMessage *msg)
{

    pkt *pkt_temp =  check_and_cast<pkt*>(msg);

    if(pkt_temp->getIndex() == 1){

  //      EV<<"one packet arrived";
        Received_Packets++;
 //       EV<<"Received_Packets is "<<Received_Packets<<"\n";
 //       EV<<"SimulatedPacketNum is "<<SimulatedPacketNum<<"\n";
        int CurrentToR = pkt_temp->getHop(pkt_temp->getHopCountNow());
        int TxIndex = pkt_temp->getTxIndex();
        int QueueOfOPSIndex = (NumTx+1)*CurrentToR + TxIndex;

//        EV<<"CurrentToR is"<<CurrentToR <<"QueueOfOPSIndex is" <<QueueOfOPSIndex <<"TxIndex is" <<TxIndex;
//        EV<<"Package size is "<< pkt_temp->getPacketSize();
//        EV<< "received a packet from" <<pkt_temp->getSrc_Server() <<" to "<<pkt_temp->getDst_Server()<<"\n";

        cModule *targetModule = getParentModule();
        int tt = targetModule->getIndex();
        int xxxx = 0;
        if(tt == 1 && getIndex() ==4 ){
            xxxx = 1;
        }
        xxxx = xxxx +1;

        int OccupyTX = 0;
        getGroupToRIndex = pkt_temp->getGroupToRIndex();

        if(getIndex()<NumTx){
            for(int i=0;i<GroupToR;i++){
                OccupyTX += Queue2OPS[i].length();
            }
        }
        else if(getIndex()>=NumTx && getIndex()<NumTx + NumTx_Core){
            for(int i=0;i<GroupToRCore;i++){
                OccupyTX += Queue2OPS[i].length();
            }
        }
        /*else{
            for(int i=0;i<GroupToRSuper;i++){
                OccupyTX += Queue2OPS[i].length();
            }
        }*/

        if(pkt_temp->getPacketSize()<=64){
  //          EV<<"Package size is 300";
            if( OccupyTX >=ToR2OPS_BufferSize ){
                Queue2OPSlost++;
                Queue2OPSPacketlost++;
                delete msg;
            }
            else{
                 if(pkt_temp->getHopCountNow() == 0){ pkt_temp->setFlag_last(1);} Queue2OPS[getGroupToRIndex].insert(pkt_temp); pkt_temp->setPacketNetworkStartTime(simTime()  ) ;

            }
        }
        else if(pkt_temp->getPacketSize()<=128){
   //         EV<<"Package size is 600";
            if( OccupyTX >=ToR2OPS_BufferSize-1 ){
                 Queue2OPSlost = Queue2OPSlost + 2;
                 Queue2OPSPacketlost++;
                 delete msg;
            }
            else{
                 pkt_temp->setPacketSize(64);
                 pkt *pkt_temp_1 = pkt_temp->dup();
                 Queue2OPS[getGroupToRIndex].insert(pkt_temp_1);
                 if(pkt_temp->getHopCountNow() == 0){ pkt_temp->setFlag_last(1);} Queue2OPS[getGroupToRIndex].insert(pkt_temp); pkt_temp->setPacketNetworkStartTime(simTime()  ) ;

            }
        }
        else if(pkt_temp->getPacketSize()<=192){
   //         EV<<"Package size is 900";
            if( OccupyTX >=ToR2OPS_BufferSize-2 ){
                 Queue2OPSlost = Queue2OPSlost + 3;
                 Queue2OPSPacketlost++;
                 delete msg;
            }
            else{
                pkt_temp->setPacketSize(64);
                pkt *pkt_temp_1 = pkt_temp->dup();
                pkt *pkt_temp_2 = pkt_temp->dup();

                Queue2OPS[getGroupToRIndex].insert(pkt_temp_1);
                Queue2OPS[getGroupToRIndex].insert(pkt_temp_2);
                if(pkt_temp->getHopCountNow() == 0){ pkt_temp->setFlag_last(1);} Queue2OPS[getGroupToRIndex].insert(pkt_temp); pkt_temp->setPacketNetworkStartTime(simTime()  ) ;
            }
        }
        else if(pkt_temp->getPacketSize()<=256){

            if( OccupyTX >=ToR2OPS_BufferSize-3 ){
                   Queue2OPSlost = Queue2OPSlost + 4;
                   Queue2OPSPacketlost++;
                   delete msg;
              }
            else{
                pkt_temp->setPacketSize(64);
                pkt *pkt_temp_1 = pkt_temp->dup();
                pkt *pkt_temp_2 = pkt_temp->dup();
                pkt *pkt_temp_3 = pkt_temp->dup();

                Queue2OPS[getGroupToRIndex].insert(pkt_temp_1);
                Queue2OPS[getGroupToRIndex].insert(pkt_temp_2);
                Queue2OPS[getGroupToRIndex].insert(pkt_temp_3);
            if(pkt_temp->getHopCountNow() == 0){ pkt_temp->setFlag_last(1);} Queue2OPS[getGroupToRIndex].insert(pkt_temp); pkt_temp->setPacketNetworkStartTime(simTime()  ) ;
            }
        }
        else if(pkt_temp->getPacketSize()<=320){

            if( OccupyTX >=ToR2OPS_BufferSize-4 ){
                   Queue2OPSlost = Queue2OPSlost + 5;
                   Queue2OPSPacketlost++;
                   delete msg;
              }
            else{
                pkt_temp->setPacketSize(64);
                pkt *pkt_temp_1 = pkt_temp->dup();
                pkt *pkt_temp_2 = pkt_temp->dup();
                pkt *pkt_temp_3 = pkt_temp->dup();
                pkt *pkt_temp_4 = pkt_temp->dup();

            Queue2OPS[getGroupToRIndex].insert(pkt_temp_1);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_2);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_3);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_4);
            if(pkt_temp->getHopCountNow() == 0){ pkt_temp->setFlag_last(1);} Queue2OPS[getGroupToRIndex].insert(pkt_temp); pkt_temp->setPacketNetworkStartTime(simTime()  ) ;
            }
        }
        else if(pkt_temp->getPacketSize()<=384){

            if( OccupyTX >=ToR2OPS_BufferSize-5 ){
                   Queue2OPSlost = Queue2OPSlost + 6;
                   Queue2OPSPacketlost++;
                   delete msg;
              }
            else{
                pkt_temp->setPacketSize(64);
                pkt *pkt_temp_1 = pkt_temp->dup();
                pkt *pkt_temp_2 = pkt_temp->dup();
                pkt *pkt_temp_3 = pkt_temp->dup();
                pkt *pkt_temp_4 = pkt_temp->dup();
                pkt *pkt_temp_5 = pkt_temp->dup();

            Queue2OPS[getGroupToRIndex].insert(pkt_temp_1);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_2);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_3);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_4);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_5);
            if(pkt_temp->getHopCountNow() == 0){ pkt_temp->setFlag_last(1);} Queue2OPS[getGroupToRIndex].insert(pkt_temp); pkt_temp->setPacketNetworkStartTime(simTime()  ) ;
            }
        }
        else if(pkt_temp->getPacketSize()<=448){

            if( OccupyTX >=ToR2OPS_BufferSize-6 ){
                   Queue2OPSlost = Queue2OPSlost + 7;
                   Queue2OPSPacketlost++;
                   delete msg;
              }
            else{
                pkt_temp->setPacketSize(64);
                pkt *pkt_temp_1 = pkt_temp->dup();
                pkt *pkt_temp_2 = pkt_temp->dup();
                pkt *pkt_temp_3 = pkt_temp->dup();
                pkt *pkt_temp_4 = pkt_temp->dup();
                pkt *pkt_temp_5 = pkt_temp->dup();
                pkt *pkt_temp_6 = pkt_temp->dup();

            Queue2OPS[getGroupToRIndex].insert(pkt_temp_1);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_2);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_3);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_4);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_5);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_6);
            if(pkt_temp->getHopCountNow() == 0){ pkt_temp->setFlag_last(1);} Queue2OPS[getGroupToRIndex].insert(pkt_temp); pkt_temp->setPacketNetworkStartTime(simTime()  ) ;
            }
        }
        else if(pkt_temp->getPacketSize()<=512){

            if( OccupyTX >=ToR2OPS_BufferSize-7 ){
                   Queue2OPSlost = Queue2OPSlost + 8;
                   Queue2OPSPacketlost++;
                   delete msg;
              }
            else{
                pkt_temp->setPacketSize(64);
                pkt *pkt_temp_1 = pkt_temp->dup();
                pkt *pkt_temp_2 = pkt_temp->dup();
                pkt *pkt_temp_3 = pkt_temp->dup();
                pkt *pkt_temp_4 = pkt_temp->dup();
                pkt *pkt_temp_5 = pkt_temp->dup();
                pkt *pkt_temp_6 = pkt_temp->dup();
                pkt *pkt_temp_7 = pkt_temp->dup();

            Queue2OPS[getGroupToRIndex].insert(pkt_temp_1);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_2);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_3);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_4);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_5);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_6);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_7);
            if(pkt_temp->getHopCountNow() == 0){ pkt_temp->setFlag_last(1);} Queue2OPS[getGroupToRIndex].insert(pkt_temp); pkt_temp->setPacketNetworkStartTime(simTime()  ) ;
            }
        }
        else if(pkt_temp->getPacketSize()<=576){

            if( OccupyTX >=ToR2OPS_BufferSize-8 ){
                   Queue2OPSlost = Queue2OPSlost + 9;
                   Queue2OPSPacketlost++;
                   delete msg;
              }
            else{
                pkt_temp->setPacketSize(64);
                pkt *pkt_temp_1 = pkt_temp->dup();
                pkt *pkt_temp_2 = pkt_temp->dup();
                pkt *pkt_temp_3 = pkt_temp->dup();
                pkt *pkt_temp_4 = pkt_temp->dup();
                pkt *pkt_temp_5 = pkt_temp->dup();
                pkt *pkt_temp_6 = pkt_temp->dup();
                pkt *pkt_temp_7 = pkt_temp->dup();
                pkt *pkt_temp_8 = pkt_temp->dup();

            Queue2OPS[getGroupToRIndex].insert(pkt_temp_1);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_2);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_3);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_4);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_5);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_6);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_7);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_8);
            if(pkt_temp->getHopCountNow() == 0){ pkt_temp->setFlag_last(1);} Queue2OPS[getGroupToRIndex].insert(pkt_temp); pkt_temp->setPacketNetworkStartTime(simTime()  ) ;
            }
        }
        else if(pkt_temp->getPacketSize()<=640){

            if( OccupyTX >=ToR2OPS_BufferSize-9 ){
                   Queue2OPSlost = Queue2OPSlost + 10;
                   Queue2OPSPacketlost++;
                   delete msg;
              }
            else{
                pkt_temp->setPacketSize(64);
                pkt *pkt_temp_1 = pkt_temp->dup();
                pkt *pkt_temp_2 = pkt_temp->dup();
                pkt *pkt_temp_3 = pkt_temp->dup();
                pkt *pkt_temp_4 = pkt_temp->dup();
                pkt *pkt_temp_5 = pkt_temp->dup();
                pkt *pkt_temp_6 = pkt_temp->dup();
                pkt *pkt_temp_7 = pkt_temp->dup();
                pkt *pkt_temp_8 = pkt_temp->dup();
                pkt *pkt_temp_9 = pkt_temp->dup();

            Queue2OPS[getGroupToRIndex].insert(pkt_temp_1);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_2);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_3);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_4);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_5);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_6);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_7);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_8);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_9);
            if(pkt_temp->getHopCountNow() == 0){ pkt_temp->setFlag_last(1);} Queue2OPS[getGroupToRIndex].insert(pkt_temp); pkt_temp->setPacketNetworkStartTime(simTime()  ) ;
            }
        }
        else if(pkt_temp->getPacketSize()<=704){

            if( OccupyTX >=ToR2OPS_BufferSize-10 ){
                   Queue2OPSlost = Queue2OPSlost + 11;
                   Queue2OPSPacketlost++;
                   delete msg;
              }
            else{
                pkt_temp->setPacketSize(64);
                pkt *pkt_temp_1 = pkt_temp->dup();
                pkt *pkt_temp_2 = pkt_temp->dup();
                pkt *pkt_temp_3 = pkt_temp->dup();
                pkt *pkt_temp_4 = pkt_temp->dup();
                pkt *pkt_temp_5 = pkt_temp->dup();
                pkt *pkt_temp_6 = pkt_temp->dup();
                pkt *pkt_temp_7 = pkt_temp->dup();
                pkt *pkt_temp_8 = pkt_temp->dup();
                pkt *pkt_temp_9 = pkt_temp->dup();
                pkt *pkt_temp_10 = pkt_temp->dup();

            Queue2OPS[getGroupToRIndex].insert(pkt_temp_1);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_2);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_3);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_4);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_5);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_6);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_7);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_8);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_9);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_10);
            if(pkt_temp->getHopCountNow() == 0){ pkt_temp->setFlag_last(1);} Queue2OPS[getGroupToRIndex].insert(pkt_temp); pkt_temp->setPacketNetworkStartTime(simTime()  ) ;
            }
        }
        else if(pkt_temp->getPacketSize()<=768){

            if( OccupyTX >=ToR2OPS_BufferSize-11 ){
                   Queue2OPSlost = Queue2OPSlost + 12;
                   Queue2OPSPacketlost++;
                   delete msg;
              }
            else{
                pkt_temp->setPacketSize(64);
                pkt *pkt_temp_1 = pkt_temp->dup();
                pkt *pkt_temp_2 = pkt_temp->dup();
                pkt *pkt_temp_3 = pkt_temp->dup();
                pkt *pkt_temp_4 = pkt_temp->dup();
                pkt *pkt_temp_5 = pkt_temp->dup();
                pkt *pkt_temp_6 = pkt_temp->dup();
                pkt *pkt_temp_7 = pkt_temp->dup();
                pkt *pkt_temp_8 = pkt_temp->dup();
                pkt *pkt_temp_9 = pkt_temp->dup();
                pkt *pkt_temp_10 = pkt_temp->dup();
                pkt *pkt_temp_11 = pkt_temp->dup();

            Queue2OPS[getGroupToRIndex].insert(pkt_temp_1);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_2);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_3);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_4);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_5);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_6);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_7);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_8);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_9);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_10);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_11);
            if(pkt_temp->getHopCountNow() == 0){ pkt_temp->setFlag_last(1);} Queue2OPS[getGroupToRIndex].insert(pkt_temp); pkt_temp->setPacketNetworkStartTime(simTime()  ) ;
            }
        }


        else if(pkt_temp->getPacketSize()<=832){

            if( OccupyTX >=ToR2OPS_BufferSize-12 ){
                   Queue2OPSlost = Queue2OPSlost + 13;
                   Queue2OPSPacketlost++;
                   delete msg;
              }
            else{
                pkt_temp->setPacketSize(64);
                pkt *pkt_temp_1 = pkt_temp->dup();
                pkt *pkt_temp_2 = pkt_temp->dup();
                pkt *pkt_temp_3 = pkt_temp->dup();
                pkt *pkt_temp_4 = pkt_temp->dup();
                pkt *pkt_temp_5 = pkt_temp->dup();
                pkt *pkt_temp_6 = pkt_temp->dup();
                pkt *pkt_temp_7 = pkt_temp->dup();
                pkt *pkt_temp_8 = pkt_temp->dup();
                pkt *pkt_temp_9 = pkt_temp->dup();
                pkt *pkt_temp_10 = pkt_temp->dup();
                pkt *pkt_temp_11 = pkt_temp->dup();
                pkt *pkt_temp_12 = pkt_temp->dup();

            Queue2OPS[getGroupToRIndex].insert(pkt_temp_1);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_2);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_3);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_4);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_5);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_6);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_7);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_8);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_9);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_10);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_11);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_12);
            if(pkt_temp->getHopCountNow() == 0){ pkt_temp->setFlag_last(1);} Queue2OPS[getGroupToRIndex].insert(pkt_temp); pkt_temp->setPacketNetworkStartTime(simTime()  ) ;
            }
        }
        else if(pkt_temp->getPacketSize()<=896){

            if( OccupyTX >=ToR2OPS_BufferSize-13 ){
                   Queue2OPSlost = Queue2OPSlost + 14;
                   Queue2OPSPacketlost++;
                   delete msg;
              }
            else{
                pkt_temp->setPacketSize(64);
                pkt *pkt_temp_1 = pkt_temp->dup();
                pkt *pkt_temp_2 = pkt_temp->dup();
                pkt *pkt_temp_3 = pkt_temp->dup();
                pkt *pkt_temp_4 = pkt_temp->dup();
                pkt *pkt_temp_5 = pkt_temp->dup();
                pkt *pkt_temp_6 = pkt_temp->dup();
                pkt *pkt_temp_7 = pkt_temp->dup();
                pkt *pkt_temp_8 = pkt_temp->dup();
                pkt *pkt_temp_9 = pkt_temp->dup();
                pkt *pkt_temp_10 = pkt_temp->dup();
                pkt *pkt_temp_11 = pkt_temp->dup();
                pkt *pkt_temp_12 = pkt_temp->dup();
                pkt *pkt_temp_13 = pkt_temp->dup();

            Queue2OPS[getGroupToRIndex].insert(pkt_temp_1);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_2);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_3);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_4);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_5);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_6);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_7);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_8);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_9);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_10);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_11);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_12);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_13);
            if(pkt_temp->getHopCountNow() == 0){ pkt_temp->setFlag_last(1);} Queue2OPS[getGroupToRIndex].insert(pkt_temp); pkt_temp->setPacketNetworkStartTime(simTime()  ) ;
            }
        }
        else if(pkt_temp->getPacketSize()<=960){

            if( OccupyTX >=ToR2OPS_BufferSize-14 ){
                   Queue2OPSlost = Queue2OPSlost + 15;
                   Queue2OPSPacketlost++;
                   delete msg;
              }
            else{
                pkt_temp->setPacketSize(64);
                pkt *pkt_temp_1 = pkt_temp->dup();
                pkt *pkt_temp_2 = pkt_temp->dup();
                pkt *pkt_temp_3 = pkt_temp->dup();
                pkt *pkt_temp_4 = pkt_temp->dup();
                pkt *pkt_temp_5 = pkt_temp->dup();
                pkt *pkt_temp_6 = pkt_temp->dup();
                pkt *pkt_temp_7 = pkt_temp->dup();
                pkt *pkt_temp_8 = pkt_temp->dup();
                pkt *pkt_temp_9 = pkt_temp->dup();
                pkt *pkt_temp_10 = pkt_temp->dup();
                pkt *pkt_temp_11 = pkt_temp->dup();
                pkt *pkt_temp_12 = pkt_temp->dup();
                pkt *pkt_temp_13 = pkt_temp->dup();
                pkt *pkt_temp_14 = pkt_temp->dup();

            Queue2OPS[getGroupToRIndex].insert(pkt_temp_1);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_2);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_3);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_4);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_5);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_6);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_7);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_8);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_9);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_10);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_11);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_12);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_13);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_14);
            if(pkt_temp->getHopCountNow() == 0){ pkt_temp->setFlag_last(1);} Queue2OPS[getGroupToRIndex].insert(pkt_temp); pkt_temp->setPacketNetworkStartTime(simTime()  ) ;
            }
        }
        else if(pkt_temp->getPacketSize()<=1024){

            if( OccupyTX >=ToR2OPS_BufferSize-15 ){
                   Queue2OPSlost = Queue2OPSlost + 16;
                   Queue2OPSPacketlost++;
                   delete msg;
              }
            else{
                pkt_temp->setPacketSize(64);
                pkt *pkt_temp_1 = pkt_temp->dup();
                pkt *pkt_temp_2 = pkt_temp->dup();
                pkt *pkt_temp_3 = pkt_temp->dup();
                pkt *pkt_temp_4 = pkt_temp->dup();
                pkt *pkt_temp_5 = pkt_temp->dup();
                pkt *pkt_temp_6 = pkt_temp->dup();
                pkt *pkt_temp_7 = pkt_temp->dup();
                pkt *pkt_temp_8 = pkt_temp->dup();
                pkt *pkt_temp_9 = pkt_temp->dup();
                pkt *pkt_temp_10 = pkt_temp->dup();
                pkt *pkt_temp_11 = pkt_temp->dup();
                pkt *pkt_temp_12 = pkt_temp->dup();
                pkt *pkt_temp_13 = pkt_temp->dup();
                pkt *pkt_temp_14 = pkt_temp->dup();
                pkt *pkt_temp_15 = pkt_temp->dup();

            Queue2OPS[getGroupToRIndex].insert(pkt_temp_1);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_2);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_3);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_4);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_5);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_6);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_7);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_8);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_9);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_10);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_11);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_12);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_13);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_14);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_15);
            if(pkt_temp->getHopCountNow() == 0){ pkt_temp->setFlag_last(1);} Queue2OPS[getGroupToRIndex].insert(pkt_temp); pkt_temp->setPacketNetworkStartTime(simTime()  ) ;
            }
        }
        else if(pkt_temp->getPacketSize()<=1088){

            if( OccupyTX >=ToR2OPS_BufferSize-16 ){
                   Queue2OPSlost = Queue2OPSlost + 17;
                   Queue2OPSPacketlost++;
                   delete msg;
              }
            else{
                pkt_temp->setPacketSize(64);
                pkt *pkt_temp_1 = pkt_temp->dup();
                pkt *pkt_temp_2 = pkt_temp->dup();
                pkt *pkt_temp_3 = pkt_temp->dup();
                pkt *pkt_temp_4 = pkt_temp->dup();
                pkt *pkt_temp_5 = pkt_temp->dup();
                pkt *pkt_temp_6 = pkt_temp->dup();
                pkt *pkt_temp_7 = pkt_temp->dup();
                pkt *pkt_temp_8 = pkt_temp->dup();
                pkt *pkt_temp_9 = pkt_temp->dup();
                pkt *pkt_temp_10 = pkt_temp->dup();
                pkt *pkt_temp_11 = pkt_temp->dup();
                pkt *pkt_temp_12 = pkt_temp->dup();
                pkt *pkt_temp_13 = pkt_temp->dup();
                pkt *pkt_temp_14 = pkt_temp->dup();
                pkt *pkt_temp_15 = pkt_temp->dup();
                pkt *pkt_temp_16 = pkt_temp->dup();

            Queue2OPS[getGroupToRIndex].insert(pkt_temp_1);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_2);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_3);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_4);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_5);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_6);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_7);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_8);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_9);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_10);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_11);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_12);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_13);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_14);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_15);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_16);
            if(pkt_temp->getHopCountNow() == 0){ pkt_temp->setFlag_last(1);} Queue2OPS[getGroupToRIndex].insert(pkt_temp); pkt_temp->setPacketNetworkStartTime(simTime()  ) ;
            }
        }
        else if(pkt_temp->getPacketSize()<=1152){

            if( OccupyTX >=ToR2OPS_BufferSize-17 ){
                   Queue2OPSlost = Queue2OPSlost + 18;
                   Queue2OPSPacketlost++;
                   delete msg;
              }
            else{
                pkt_temp->setPacketSize(64);
                pkt *pkt_temp_1 = pkt_temp->dup();
                pkt *pkt_temp_2 = pkt_temp->dup();
                pkt *pkt_temp_3 = pkt_temp->dup();
                pkt *pkt_temp_4 = pkt_temp->dup();
                pkt *pkt_temp_5 = pkt_temp->dup();
                pkt *pkt_temp_6 = pkt_temp->dup();
                pkt *pkt_temp_7 = pkt_temp->dup();
                pkt *pkt_temp_8 = pkt_temp->dup();
                pkt *pkt_temp_9 = pkt_temp->dup();
                pkt *pkt_temp_10 = pkt_temp->dup();
                pkt *pkt_temp_11 = pkt_temp->dup();
                pkt *pkt_temp_12 = pkt_temp->dup();
                pkt *pkt_temp_13 = pkt_temp->dup();
                pkt *pkt_temp_14 = pkt_temp->dup();
                pkt *pkt_temp_15 = pkt_temp->dup();
                pkt *pkt_temp_16 = pkt_temp->dup();
                pkt *pkt_temp_17 = pkt_temp->dup();

            Queue2OPS[getGroupToRIndex].insert(pkt_temp_1);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_2);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_3);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_4);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_5);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_6);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_7);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_8);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_9);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_10);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_11);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_12);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_13);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_14);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_15);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_16);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_17);
            if(pkt_temp->getHopCountNow() == 0){ pkt_temp->setFlag_last(1);} Queue2OPS[getGroupToRIndex].insert(pkt_temp); pkt_temp->setPacketNetworkStartTime(simTime()  ) ;
            }
        }
        else if(pkt_temp->getPacketSize()<=1216){

            if( OccupyTX >=ToR2OPS_BufferSize-18 ){
                   Queue2OPSlost = Queue2OPSlost + 19;
                   Queue2OPSPacketlost++;
                   delete msg;
              }
            else{
                pkt_temp->setPacketSize(64);
                pkt *pkt_temp_1 = pkt_temp->dup();
                pkt *pkt_temp_2 = pkt_temp->dup();
                pkt *pkt_temp_3 = pkt_temp->dup();
                pkt *pkt_temp_4 = pkt_temp->dup();
                pkt *pkt_temp_5 = pkt_temp->dup();
                pkt *pkt_temp_6 = pkt_temp->dup();
                pkt *pkt_temp_7 = pkt_temp->dup();
                pkt *pkt_temp_8 = pkt_temp->dup();
                pkt *pkt_temp_9 = pkt_temp->dup();
                pkt *pkt_temp_10 = pkt_temp->dup();
                pkt *pkt_temp_11 = pkt_temp->dup();
                pkt *pkt_temp_12 = pkt_temp->dup();
                pkt *pkt_temp_13 = pkt_temp->dup();
                pkt *pkt_temp_14 = pkt_temp->dup();
                pkt *pkt_temp_15 = pkt_temp->dup();
                pkt *pkt_temp_16 = pkt_temp->dup();
                pkt *pkt_temp_17 = pkt_temp->dup();
                pkt *pkt_temp_18 = pkt_temp->dup();

            Queue2OPS[getGroupToRIndex].insert(pkt_temp_1);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_2);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_3);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_4);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_5);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_6);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_7);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_8);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_9);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_10);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_11);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_12);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_13);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_14);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_15);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_16);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_17);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_18);
            if(pkt_temp->getHopCountNow() == 0){ pkt_temp->setFlag_last(1);} Queue2OPS[getGroupToRIndex].insert(pkt_temp); pkt_temp->setPacketNetworkStartTime(simTime()  ) ;
            }
        }
        else if(pkt_temp->getPacketSize()<=1280){

            if( OccupyTX >=ToR2OPS_BufferSize-19 ){
                   Queue2OPSlost = Queue2OPSlost + 20;
                   Queue2OPSPacketlost++;
                   delete msg;
              }
            else{
                pkt_temp->setPacketSize(64);
                pkt *pkt_temp_1 = pkt_temp->dup();
                pkt *pkt_temp_2 = pkt_temp->dup();
                pkt *pkt_temp_3 = pkt_temp->dup();
                pkt *pkt_temp_4 = pkt_temp->dup();
                pkt *pkt_temp_5 = pkt_temp->dup();
                pkt *pkt_temp_6 = pkt_temp->dup();
                pkt *pkt_temp_7 = pkt_temp->dup();
                pkt *pkt_temp_8 = pkt_temp->dup();
                pkt *pkt_temp_9 = pkt_temp->dup();
                pkt *pkt_temp_10 = pkt_temp->dup();
                pkt *pkt_temp_11 = pkt_temp->dup();
                pkt *pkt_temp_12 = pkt_temp->dup();
                pkt *pkt_temp_13 = pkt_temp->dup();
                pkt *pkt_temp_14 = pkt_temp->dup();
                pkt *pkt_temp_15 = pkt_temp->dup();
                pkt *pkt_temp_16 = pkt_temp->dup();
                pkt *pkt_temp_17 = pkt_temp->dup();
                pkt *pkt_temp_18 = pkt_temp->dup();
                pkt *pkt_temp_19 = pkt_temp->dup();

            Queue2OPS[getGroupToRIndex].insert(pkt_temp_1);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_2);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_3);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_4);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_5);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_6);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_7);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_8);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_9);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_10);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_11);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_12);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_13);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_14);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_15);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_16);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_17);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_18);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_19);
            if(pkt_temp->getHopCountNow() == 0){ pkt_temp->setFlag_last(1);} Queue2OPS[getGroupToRIndex].insert(pkt_temp); pkt_temp->setPacketNetworkStartTime(simTime()  ) ;
            }
        }
        else if(pkt_temp->getPacketSize()<=1344){

            if( OccupyTX >=ToR2OPS_BufferSize-20 ){
                   Queue2OPSlost = Queue2OPSlost + 21;
                   Queue2OPSPacketlost++;
                   delete msg;
              }
            else{
                pkt_temp->setPacketSize(64);
                pkt *pkt_temp_1 = pkt_temp->dup();
                pkt *pkt_temp_2 = pkt_temp->dup();
                pkt *pkt_temp_3 = pkt_temp->dup();
                pkt *pkt_temp_4 = pkt_temp->dup();
                pkt *pkt_temp_5 = pkt_temp->dup();
                pkt *pkt_temp_6 = pkt_temp->dup();
                pkt *pkt_temp_7 = pkt_temp->dup();
                pkt *pkt_temp_8 = pkt_temp->dup();
                pkt *pkt_temp_9 = pkt_temp->dup();
                pkt *pkt_temp_10 = pkt_temp->dup();
                pkt *pkt_temp_11 = pkt_temp->dup();
                pkt *pkt_temp_12 = pkt_temp->dup();
                pkt *pkt_temp_13 = pkt_temp->dup();
                pkt *pkt_temp_14 = pkt_temp->dup();
                pkt *pkt_temp_15 = pkt_temp->dup();
                pkt *pkt_temp_16 = pkt_temp->dup();
                pkt *pkt_temp_17 = pkt_temp->dup();
                pkt *pkt_temp_18 = pkt_temp->dup();
                pkt *pkt_temp_19 = pkt_temp->dup();
                pkt *pkt_temp_20 = pkt_temp->dup();

            Queue2OPS[getGroupToRIndex].insert(pkt_temp_1);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_2);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_3);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_4);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_5);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_6);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_7);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_8);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_9);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_10);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_11);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_12);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_13);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_14);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_15);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_16);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_17);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_18);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_19);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_20);
            if(pkt_temp->getHopCountNow() == 0){ pkt_temp->setFlag_last(1);} Queue2OPS[getGroupToRIndex].insert(pkt_temp); pkt_temp->setPacketNetworkStartTime(simTime()  ) ;
            }
        }
        else if(pkt_temp->getPacketSize()<=1408){

            if( OccupyTX >=ToR2OPS_BufferSize-21 ){
                   Queue2OPSlost = Queue2OPSlost + 22;
                   Queue2OPSPacketlost++;
                   delete msg;
              }
            else{
                pkt_temp->setPacketSize(64);
                pkt *pkt_temp_1 = pkt_temp->dup();
                pkt *pkt_temp_2 = pkt_temp->dup();
                pkt *pkt_temp_3 = pkt_temp->dup();
                pkt *pkt_temp_4 = pkt_temp->dup();
                pkt *pkt_temp_5 = pkt_temp->dup();
                pkt *pkt_temp_6 = pkt_temp->dup();
                pkt *pkt_temp_7 = pkt_temp->dup();
                pkt *pkt_temp_8 = pkt_temp->dup();
                pkt *pkt_temp_9 = pkt_temp->dup();
                pkt *pkt_temp_10 = pkt_temp->dup();
                pkt *pkt_temp_11 = pkt_temp->dup();
                pkt *pkt_temp_12 = pkt_temp->dup();
                pkt *pkt_temp_13 = pkt_temp->dup();
                pkt *pkt_temp_14 = pkt_temp->dup();
                pkt *pkt_temp_15 = pkt_temp->dup();
                pkt *pkt_temp_16 = pkt_temp->dup();
                pkt *pkt_temp_17 = pkt_temp->dup();
                pkt *pkt_temp_18 = pkt_temp->dup();
                pkt *pkt_temp_19 = pkt_temp->dup();
                pkt *pkt_temp_20 = pkt_temp->dup();
                pkt *pkt_temp_21 = pkt_temp->dup();

            Queue2OPS[getGroupToRIndex].insert(pkt_temp_1);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_2);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_3);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_4);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_5);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_6);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_7);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_8);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_9);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_10);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_11);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_12);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_13);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_14);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_15);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_16);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_17);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_18);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_19);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_20);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_21);
            if(pkt_temp->getHopCountNow() == 0){ pkt_temp->setFlag_last(1);} Queue2OPS[getGroupToRIndex].insert(pkt_temp); pkt_temp->setPacketNetworkStartTime(simTime()  ) ;
            }
        }
        else if(pkt_temp->getPacketSize()<=1472){

            if( OccupyTX >=ToR2OPS_BufferSize-22 ){
                   Queue2OPSlost = Queue2OPSlost + 23;
                   Queue2OPSPacketlost++;
                   delete msg;
              }
            else{
                pkt_temp->setPacketSize(64);

                pkt *pkt_temp_1 = pkt_temp->dup();
                pkt *pkt_temp_2 = pkt_temp->dup();
                pkt *pkt_temp_3 = pkt_temp->dup();
                pkt *pkt_temp_4 = pkt_temp->dup();
                pkt *pkt_temp_5 = pkt_temp->dup();
                pkt *pkt_temp_6 = pkt_temp->dup();
                pkt *pkt_temp_7 = pkt_temp->dup();
                pkt *pkt_temp_8 = pkt_temp->dup();
                pkt *pkt_temp_9 = pkt_temp->dup();
                pkt *pkt_temp_10 = pkt_temp->dup();
                pkt *pkt_temp_11 = pkt_temp->dup();
                pkt *pkt_temp_12 = pkt_temp->dup();
                pkt *pkt_temp_13 = pkt_temp->dup();
                pkt *pkt_temp_14 = pkt_temp->dup();
                pkt *pkt_temp_15 = pkt_temp->dup();
                pkt *pkt_temp_16 = pkt_temp->dup();
                pkt *pkt_temp_17 = pkt_temp->dup();
                pkt *pkt_temp_18 = pkt_temp->dup();
                pkt *pkt_temp_19 = pkt_temp->dup();
                pkt *pkt_temp_20 = pkt_temp->dup();
                pkt *pkt_temp_21 = pkt_temp->dup();
                pkt *pkt_temp_22 = pkt_temp->dup();

                Queue2OPS[getGroupToRIndex].insert(pkt_temp_1);
                Queue2OPS[getGroupToRIndex].insert(pkt_temp_2);
                Queue2OPS[getGroupToRIndex].insert(pkt_temp_3);
                Queue2OPS[getGroupToRIndex].insert(pkt_temp_4);
                Queue2OPS[getGroupToRIndex].insert(pkt_temp_5);
                Queue2OPS[getGroupToRIndex].insert(pkt_temp_6);
                Queue2OPS[getGroupToRIndex].insert(pkt_temp_7);
                Queue2OPS[getGroupToRIndex].insert(pkt_temp_8);
                Queue2OPS[getGroupToRIndex].insert(pkt_temp_9);
                Queue2OPS[getGroupToRIndex].insert(pkt_temp_10);
                Queue2OPS[getGroupToRIndex].insert(pkt_temp_11);
                Queue2OPS[getGroupToRIndex].insert(pkt_temp_12);
                Queue2OPS[getGroupToRIndex].insert(pkt_temp_13);
                Queue2OPS[getGroupToRIndex].insert(pkt_temp_14);
                Queue2OPS[getGroupToRIndex].insert(pkt_temp_15);
                Queue2OPS[getGroupToRIndex].insert(pkt_temp_16);
                Queue2OPS[getGroupToRIndex].insert(pkt_temp_17);
                Queue2OPS[getGroupToRIndex].insert(pkt_temp_18);
                Queue2OPS[getGroupToRIndex].insert(pkt_temp_19);
                Queue2OPS[getGroupToRIndex].insert(pkt_temp_20);
                Queue2OPS[getGroupToRIndex].insert(pkt_temp_21);
                Queue2OPS[getGroupToRIndex].insert(pkt_temp_22);
                if(pkt_temp->getHopCountNow() == 0){ pkt_temp->setFlag_last(1);} Queue2OPS[getGroupToRIndex].insert(pkt_temp); pkt_temp->setPacketNetworkStartTime(simTime()  ) ;
            }
        }
        else if(pkt_temp->getPacketSize()<=1536){

            if( OccupyTX >=ToR2OPS_BufferSize-23 ){
                   Queue2OPSlost = Queue2OPSlost + 24;
                   Queue2OPSPacketlost++;
                   delete msg;
              }
            else{
                pkt_temp->setPacketSize(64);
                pkt *pkt_temp_1 = pkt_temp->dup();
                pkt *pkt_temp_2 = pkt_temp->dup();
                pkt *pkt_temp_3 = pkt_temp->dup();
                pkt *pkt_temp_4 = pkt_temp->dup();
                pkt *pkt_temp_5 = pkt_temp->dup();
                pkt *pkt_temp_6 = pkt_temp->dup();
                pkt *pkt_temp_7 = pkt_temp->dup();
                pkt *pkt_temp_8 = pkt_temp->dup();
                pkt *pkt_temp_9 = pkt_temp->dup();
                pkt *pkt_temp_10 = pkt_temp->dup();
                pkt *pkt_temp_11 = pkt_temp->dup();
                pkt *pkt_temp_12 = pkt_temp->dup();
                pkt *pkt_temp_13 = pkt_temp->dup();
                pkt *pkt_temp_14 = pkt_temp->dup();
                pkt *pkt_temp_15 = pkt_temp->dup();
                pkt *pkt_temp_16 = pkt_temp->dup();
                pkt *pkt_temp_17 = pkt_temp->dup();
                pkt *pkt_temp_18 = pkt_temp->dup();
                pkt *pkt_temp_19 = pkt_temp->dup();
                pkt *pkt_temp_20 = pkt_temp->dup();
                pkt *pkt_temp_21 = pkt_temp->dup();
                pkt *pkt_temp_22 = pkt_temp->dup();
                pkt *pkt_temp_23 = pkt_temp->dup();

            Queue2OPS[getGroupToRIndex].insert(pkt_temp_1);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_2);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_3);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_4);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_5);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_6);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_7);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_8);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_9);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_10);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_11);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_12);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_13);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_14);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_15);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_16);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_17);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_18);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_19);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_20);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_21);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_22);
            Queue2OPS[getGroupToRIndex].insert(pkt_temp_23);
            if(pkt_temp->getHopCountNow() == 0){ pkt_temp->setFlag_last(1);} Queue2OPS[getGroupToRIndex].insert(pkt_temp); pkt_temp->setPacketNetworkStartTime(simTime()  ) ;
            }
        }
        else {
            delete msg;
        }

        if(finishflag == true){
        	if(getIndex()<NumTx){
				for(int i=0;i<GroupToR;i++){			
		            if( (Queue2OPS[i].length() != 0) ){
		 //               EV<<"Queue2OPS length before the last send is"<<Queue2OPS.length()<<"\n";
		                pkt *pkt_send = check_and_cast<pkt*>(Queue2OPS[i].pop());

		                if(pkt_temp->getHopCountNow() == 1)
		                    msg->setTimestamp(simTime());
		                int countNow = pkt_send->getHopCountNow();
		  //              EV<< "countNow is" <<countNow;
		                pkt_send->setHopCountNow(countNow + 1);

                        sendDelayed(pkt_send,slot,"out");
		  //              EV<<"Queue2OPS length after the last send is"<<Queue2OPS.length()<<"\n";
		                test++;
		                while( (BackupQueue[i].length() != 0) ){
							delete BackupQueue[i].pop();
						}
		               // wait(slot);
		            }
		        }
		     }
             else if(getIndex()>=NumTx && getIndex()<NumTx+NumTx_Core){
                    for(int i=0;i<GroupToRCore;i++){
                        if( (Queue2OPS[i].length() != 0) ){
             //               EV<<"Queue2OPS length before the last send is"<<Queue2OPS.length()<<"\n";
                            pkt *pkt_send = check_and_cast<pkt*>(Queue2OPS[i].pop());

                            if(pkt_temp->getHopCountNow() == 1)
                                msg->setTimestamp(simTime());
                            int countNow = pkt_send->getHopCountNow();
              //              EV<< "countNow is" <<countNow;
                            pkt_send->setHopCountNow(countNow + 1);

                            sendDelayed(pkt_send,slot,"out");
              //              EV<<"Queue2OPS length after the last send is"<<Queue2OPS.length()<<"\n";
                            test++;
                            while( (BackupQueue[i].length() != 0) ){
                                delete BackupQueue[i].pop();
                            }
                        }
                    }

              }
        	/*else{
                for(int i=0;i<GroupToRSuper;i++){
                    if( (Queue2OPS[i].length() != 0) ){
         //               EV<<"Queue2OPS length before the last send is"<<Queue2OPS.length()<<"\n";
                        pkt *pkt_send = check_and_cast<pkt*>(Queue2OPS[i].pop());

                        if(pkt_temp->getHopCountNow() == 1)
                            msg->setTimestamp(simTime());
                        int countNow = pkt_send->getHopCountNow();
          //              EV<< "countNow is" <<countNow;
                        pkt_send->setHopCountNow(countNow + 1);

                        sendDelayed(pkt_send,slot,"out");
          //              EV<<"Queue2OPS length after the last send is"<<Queue2OPS.length()<<"\n";
                        test++;
                        while( (BackupQueue[i].length() != 0) ){
                            delete BackupQueue[i].pop();
                        }
                       // wait(slot);
                    }
                }
             }*/
        }
    }
    else if(pkt_temp->getIndex() == 0){

//        if(Received_Packets < SimulatedPacketNum){
        if(finishflag != true){
            int maxQueueIndex = 0;
            int maxQueuelength = -1;
            if(getIndex()<NumTx){
                for(int i=0;i<GroupToR;i++){
                    if( Queue2OPS[i].length() > maxQueuelength ){
                        maxQueuelength = Queue2OPS[i].length();
                        maxQueueIndex = i;
                    }
                }
            }
            else if(getIndex()>=NumTx && getIndex()<NumTx+NumTx_Core){
                for(int i=0;i<GroupToRCore;i++){
                    if( Queue2OPS[i].length() > maxQueuelength ){
                        maxQueuelength = Queue2OPS[i].length();
                        maxQueueIndex = i;
                    }
                }
            }
            else{
                 maxQueuelength = Queue2OPS[0].length();
                 maxQueueIndex = 0;

            }

            if(QueueArrayflag == true){
                Optical *pkt_optical_temp_send = new Optical();
                pkt_optical_temp_send->setKind(1000);
                for(int i=0;i<BackupQueueopticalNum;i++){
                          if(BackupQueue[BackupQueueIndex].length()>0){
                              switch(i){
                              case 0:
                               {
                                   pkt *msgresend = check_and_cast<pkt*>(BackupQueue[BackupQueueIndex].pop());
                                   pkt *msgresend_copy = msgresend->dup();

                                   BackupQueue[BackupQueueIndex].insert(msgresend);

                                   int countNow = msgresend_copy->getHopCountNow();
                                   pkt_optical_temp_send->setNextToR( msgresend_copy->getHop(countNow + 2));
                                   pkt_optical_temp_send->setPreviousToR(msgresend_copy->getHop(countNow ));
                                   pkt_optical_temp_send->setTxIndex(msgresend_copy->getTxIndex());
                                   pkt_optical_temp_send->setGroupToRIndex( msgresend_copy->getGroupToRIndex() );
                                   msgresend_copy->setHopCountNow(countNow + 1);


                                   pkt_optical_temp_send->setPkt1(msgresend_copy);
                                   pkt_optical_temp_send->insertMessage(msgresend_copy);
                                   pkt_optical_temp_send->setOptical_num(1);
                               }
                                   break;
                               case 1:
                                   {
                                   pkt *msgresend1 = check_and_cast<pkt*>(BackupQueue[BackupQueueIndex].pop());
                                   pkt *msgresend1_copy = msgresend1->dup();

                                   BackupQueue[BackupQueueIndex].insert(msgresend1);

                                   int countNow1 = msgresend1_copy->getHopCountNow();
                                   pkt_optical_temp_send->setNextToR( msgresend1_copy->getHop(countNow1 + 2));
                                   pkt_optical_temp_send->setPreviousToR(msgresend1_copy->getHop(countNow1 ));
                                   pkt_optical_temp_send->setTxIndex(msgresend1_copy->getTxIndex());
                                   pkt_optical_temp_send->setGroupToRIndex( msgresend1_copy->getGroupToRIndex() );
                                   msgresend1_copy->setHopCountNow(countNow1 + 1);



                                   pkt_optical_temp_send->setPkt2(msgresend1_copy);
                                   pkt_optical_temp_send->insertMessage(msgresend1_copy);
                                   pkt_optical_temp_send->setOptical_num(2);
                                   }
                                   break;
                               case 2:
                               {
                                   pkt *msgresend2 = check_and_cast<pkt*>(BackupQueue[BackupQueueIndex].pop());
                                   pkt *msgresend2_copy = msgresend2->dup();

                                   BackupQueue[BackupQueueIndex].insert(msgresend2);

                                   int countNow2 = msgresend2_copy->getHopCountNow();
                                   pkt_optical_temp_send->setNextToR( msgresend2_copy->getHop(countNow2 + 2));
                                   pkt_optical_temp_send->setPreviousToR(msgresend2_copy->getHop(countNow2 ));
                                   pkt_optical_temp_send->setTxIndex(msgresend2_copy->getTxIndex());
                                   pkt_optical_temp_send->setGroupToRIndex( msgresend2_copy->getGroupToRIndex() );
                                   msgresend2_copy->setHopCountNow(countNow2 + 1);

                                   pkt_optical_temp_send->setPkt3(msgresend2_copy);
                                   pkt_optical_temp_send->insertMessage(msgresend2_copy);
                                   pkt_optical_temp_send->setOptical_num(3);
                               }
                                   break;
                               case 3:
                               {
                                   pkt *msgresend3 = check_and_cast<pkt*>(BackupQueue[BackupQueueIndex].pop());
                                   pkt *msgresend3_copy = msgresend3->dup();

                                   BackupQueue[BackupQueueIndex].insert(msgresend3);

                                   int countNow3 = msgresend3_copy->getHopCountNow();
                                   pkt_optical_temp_send->setNextToR( msgresend3_copy->getHop(countNow3 + 2));
                                   pkt_optical_temp_send->setPreviousToR(msgresend3_copy->getHop(countNow3 ));
                                   pkt_optical_temp_send->setTxIndex(msgresend3_copy->getTxIndex());
                                   pkt_optical_temp_send->setGroupToRIndex( msgresend3_copy->getGroupToRIndex() );
                                   msgresend3_copy->setHopCountNow(countNow3 + 1);

                                   pkt_optical_temp_send->setPkt4(msgresend3_copy);
                                   pkt_optical_temp_send->insertMessage(msgresend3_copy);
                                   pkt_optical_temp_send->setOptical_num(4);
                               }
                                   break;
                               case 4:
                               {
                                   pkt *msgresend4 = check_and_cast<pkt*>(BackupQueue[BackupQueueIndex].pop());
                                   pkt *msgresend4_copy = msgresend4->dup();

                                   BackupQueue[BackupQueueIndex].insert(msgresend4);

                                   int countNow4 = msgresend4_copy->getHopCountNow();
                                   pkt_optical_temp_send->setNextToR( msgresend4_copy->getHop(countNow4 + 2));
                                   pkt_optical_temp_send->setPreviousToR(msgresend4_copy->getHop(countNow4 ));
                                   pkt_optical_temp_send->setTxIndex(msgresend4_copy->getTxIndex());
                                   pkt_optical_temp_send->setGroupToRIndex( msgresend4_copy->getGroupToRIndex() );
                                   msgresend4_copy->setHopCountNow(countNow4 + 1);

                                   pkt_optical_temp_send->setPkt5(msgresend4_copy);
                                   pkt_optical_temp_send->insertMessage(msgresend4_copy);
                                   pkt_optical_temp_send->setOptical_num(5);
                               }
                                   break;
                               case 5:
                               {
                                   pkt *msgresend5 = check_and_cast<pkt*>(BackupQueue[BackupQueueIndex].pop());
                                   pkt *msgresend5_copy = msgresend5->dup();

                                   BackupQueue[BackupQueueIndex].insert(msgresend5);

                                   int countNow5 = msgresend5_copy->getHopCountNow();
                                   pkt_optical_temp_send->setNextToR( msgresend5_copy->getHop(countNow5 + 2));
                                   pkt_optical_temp_send->setPreviousToR(msgresend5_copy->getHop(countNow5 ));
                                   pkt_optical_temp_send->setTxIndex(msgresend5_copy->getTxIndex());
                                   pkt_optical_temp_send->setGroupToRIndex( msgresend5_copy->getGroupToRIndex() );
                                   msgresend5_copy->setHopCountNow(countNow5 + 1);

                                   pkt_optical_temp_send->setPkt6(msgresend5_copy);
                                   pkt_optical_temp_send->insertMessage(msgresend5_copy);
                                   pkt_optical_temp_send->setOptical_num(6);
                               }
                                   break;
							   case 6:
                               {
                                   pkt *msgresend6 = check_and_cast<pkt*>(BackupQueue[BackupQueueIndex].pop());
                                   pkt *msgresend6_copy = msgresend6->dup();

                                   BackupQueue[BackupQueueIndex].insert(msgresend6);

                                   int countNow6 = msgresend6_copy->getHopCountNow();
                                   pkt_optical_temp_send->setNextToR( msgresend6_copy->getHop(countNow6 + 2));
                                   pkt_optical_temp_send->setPreviousToR(msgresend6_copy->getHop(countNow6 ));
                                   pkt_optical_temp_send->setTxIndex(msgresend6_copy->getTxIndex());
                                   pkt_optical_temp_send->setGroupToRIndex( msgresend6_copy->getGroupToRIndex() );
                                   msgresend6_copy->setHopCountNow(countNow6 + 1);

                                   pkt_optical_temp_send->setPkt7(msgresend6_copy);
                                   pkt_optical_temp_send->insertMessage(msgresend6_copy);
                                   pkt_optical_temp_send->setOptical_num(7);
                               }
                                   break;
							   case 7:
                               {
                                   pkt *msgresend7 = check_and_cast<pkt*>(BackupQueue[BackupQueueIndex].pop());
                                   pkt *msgresend7_copy = msgresend7->dup();

                                   BackupQueue[BackupQueueIndex].insert(msgresend7);

                                   int countNow7 = msgresend7_copy->getHopCountNow();
                                   pkt_optical_temp_send->setNextToR( msgresend7_copy->getHop(countNow7 + 2));
                                   pkt_optical_temp_send->setPreviousToR(msgresend7_copy->getHop(countNow7 ));
                                   pkt_optical_temp_send->setTxIndex(msgresend7_copy->getTxIndex());
                                   pkt_optical_temp_send->setGroupToRIndex( msgresend7_copy->getGroupToRIndex() );
                                   msgresend7_copy->setHopCountNow(countNow7 + 1);

                                   pkt_optical_temp_send->setPkt8(msgresend7_copy);
                                   pkt_optical_temp_send->insertMessage(msgresend7_copy);
                                   pkt_optical_temp_send->setOptical_num(8);
                               }
                                   break;
							   case 8:
                               {
                                   pkt *msgresend8 = check_and_cast<pkt*>(BackupQueue[BackupQueueIndex].pop());
                                   pkt *msgresend8_copy = msgresend8->dup();

                                   BackupQueue[BackupQueueIndex].insert(msgresend8);

                                   int countNow8 = msgresend8_copy->getHopCountNow();
                                   pkt_optical_temp_send->setNextToR( msgresend8_copy->getHop(countNow8 + 2));
                                   pkt_optical_temp_send->setPreviousToR(msgresend8_copy->getHop(countNow8 ));
                                   pkt_optical_temp_send->setTxIndex(msgresend8_copy->getTxIndex());
                                   pkt_optical_temp_send->setGroupToRIndex( msgresend8_copy->getGroupToRIndex() );
                                   msgresend8_copy->setHopCountNow(countNow8 + 1);

                                   pkt_optical_temp_send->setPkt9(msgresend8_copy);
                                   pkt_optical_temp_send->insertMessage(msgresend8_copy);
                                   pkt_optical_temp_send->setOptical_num(9);
                               }
                                   break;
							   case 9:
                               {
                                   pkt *msgresend9 = check_and_cast<pkt*>(BackupQueue[BackupQueueIndex].pop());
                                   pkt *msgresend9_copy = msgresend9->dup();

                                   BackupQueue[BackupQueueIndex].insert(msgresend9);

                                   int countNow9 = msgresend9_copy->getHopCountNow();
                                   pkt_optical_temp_send->setNextToR( msgresend9_copy->getHop(countNow9 + 2));
                                   pkt_optical_temp_send->setPreviousToR(msgresend9_copy->getHop(countNow9 ));
                                   pkt_optical_temp_send->setTxIndex(msgresend9_copy->getTxIndex());
                                   pkt_optical_temp_send->setGroupToRIndex( msgresend9_copy->getGroupToRIndex() );
                                   msgresend9_copy->setHopCountNow(countNow9 + 1);

                                   pkt_optical_temp_send->setPkt10(msgresend9_copy);
                                   pkt_optical_temp_send->insertMessage(msgresend9_copy);
                                   pkt_optical_temp_send->setOptical_num(10);
                               }
                                   break;
							   case 10:
                               {
                                   pkt *msgresend10 = check_and_cast<pkt*>(BackupQueue[BackupQueueIndex].pop());
                                   pkt *msgresend10_copy = msgresend10->dup();

                                   BackupQueue[BackupQueueIndex].insert(msgresend10);

                                   int countNow10 = msgresend10_copy->getHopCountNow();
                                   pkt_optical_temp_send->setNextToR( msgresend10_copy->getHop(countNow10 + 2));
                                   pkt_optical_temp_send->setPreviousToR(msgresend10_copy->getHop(countNow10 ));
                                   pkt_optical_temp_send->setTxIndex(msgresend10_copy->getTxIndex());
                                   pkt_optical_temp_send->setGroupToRIndex( msgresend10_copy->getGroupToRIndex() );
                                   msgresend10_copy->setHopCountNow(countNow10 + 1);

                                   pkt_optical_temp_send->setPkt11(msgresend10_copy);
                                   pkt_optical_temp_send->insertMessage(msgresend10_copy);
                                   pkt_optical_temp_send->setOptical_num(11);
                               }
                                   break;
							   case 11:
                               {
                                   pkt *msgresend11 = check_and_cast<pkt*>(BackupQueue[BackupQueueIndex].pop());
                                   pkt *msgresend11_copy = msgresend11->dup();

                                   BackupQueue[BackupQueueIndex].insert(msgresend11);

                                   int countNow11 = msgresend11_copy->getHopCountNow();
                                   pkt_optical_temp_send->setNextToR( msgresend11_copy->getHop(countNow11 + 2));
                                   pkt_optical_temp_send->setPreviousToR(msgresend11_copy->getHop(countNow11 ));
                                   pkt_optical_temp_send->setTxIndex(msgresend11_copy->getTxIndex());
                                   pkt_optical_temp_send->setGroupToRIndex( msgresend11_copy->getGroupToRIndex() );
                                   msgresend11_copy->setHopCountNow(countNow11 + 1);

                                   pkt_optical_temp_send->setPkt12(msgresend11_copy);
                                   pkt_optical_temp_send->insertMessage(msgresend11_copy);
                                   pkt_optical_temp_send->setOptical_num(12);
                               }
                                   break;
							   case 12:
                               {
                                   pkt *msgresend12 = check_and_cast<pkt*>(BackupQueue[BackupQueueIndex].pop());
                                   pkt *msgresend12_copy = msgresend12->dup();

                                   BackupQueue[BackupQueueIndex].insert(msgresend12);

                                   int countNow12 = msgresend12_copy->getHopCountNow();
                                   pkt_optical_temp_send->setNextToR( msgresend12_copy->getHop(countNow12 + 2));
                                   pkt_optical_temp_send->setPreviousToR(msgresend12_copy->getHop(countNow12 ));
                                   pkt_optical_temp_send->setTxIndex(msgresend12_copy->getTxIndex());
                                   pkt_optical_temp_send->setGroupToRIndex( msgresend12_copy->getGroupToRIndex() );
                                   msgresend12_copy->setHopCountNow(countNow12 + 1);

                                   pkt_optical_temp_send->setPkt13(msgresend12_copy);
                                   pkt_optical_temp_send->insertMessage(msgresend12_copy);
                                   pkt_optical_temp_send->setOptical_num(13);
                               }
                                   break;
							   case 13:
                               {
                                   pkt *msgresend13 = check_and_cast<pkt*>(BackupQueue[BackupQueueIndex].pop());
                                   pkt *msgresend13_copy = msgresend13->dup();

                                   BackupQueue[BackupQueueIndex].insert(msgresend13);

                                   int countNow13 = msgresend13_copy->getHopCountNow();
                                   pkt_optical_temp_send->setNextToR( msgresend13_copy->getHop(countNow13 + 2));
                                   pkt_optical_temp_send->setPreviousToR(msgresend13_copy->getHop(countNow13 ));
                                   pkt_optical_temp_send->setTxIndex(msgresend13_copy->getTxIndex());
                                   pkt_optical_temp_send->setGroupToRIndex( msgresend13_copy->getGroupToRIndex() );
                                   msgresend13_copy->setHopCountNow(countNow13 + 1);

                                   pkt_optical_temp_send->setPkt14(msgresend13_copy);
                                   pkt_optical_temp_send->insertMessage(msgresend13_copy);
                                   pkt_optical_temp_send->setOptical_num(14);
                               }
                                   break;									   
							   case 14:
                               {
                                   pkt *msgresend14 = check_and_cast<pkt*>(BackupQueue[BackupQueueIndex].pop());
                                   pkt *msgresend14_copy = msgresend14->dup();

                                   BackupQueue[BackupQueueIndex].insert(msgresend14);

                                   int countNow14 = msgresend14_copy->getHopCountNow();
                                   pkt_optical_temp_send->setNextToR( msgresend14_copy->getHop(countNow14 + 2));
                                   pkt_optical_temp_send->setPreviousToR(msgresend14_copy->getHop(countNow14 ));
                                   pkt_optical_temp_send->setTxIndex(msgresend14_copy->getTxIndex());
                                   pkt_optical_temp_send->setGroupToRIndex( msgresend14_copy->getGroupToRIndex() );
                                   msgresend14_copy->setHopCountNow(countNow14 + 1);

                                   pkt_optical_temp_send->setPkt15(msgresend14_copy);
                                   pkt_optical_temp_send->insertMessage(msgresend14_copy);
                                   pkt_optical_temp_send->setOptical_num(15);
                               }
                                   break;	
							   case 15:
                               {
                                   pkt *msgresend15 = check_and_cast<pkt*>(BackupQueue[BackupQueueIndex].pop());
                                   pkt *msgresend15_copy = msgresend15->dup();

                                   BackupQueue[BackupQueueIndex].insert(msgresend15);

                                   int countNow15 = msgresend15_copy->getHopCountNow();
                                   pkt_optical_temp_send->setNextToR( msgresend15_copy->getHop(countNow15 + 2));
                                   pkt_optical_temp_send->setPreviousToR(msgresend15_copy->getHop(countNow15 ));
                                   pkt_optical_temp_send->setTxIndex(msgresend15_copy->getTxIndex());
                                   pkt_optical_temp_send->setGroupToRIndex( msgresend15_copy->getGroupToRIndex() );
                                   msgresend15_copy->setHopCountNow(countNow15 + 1);

                                   pkt_optical_temp_send->setPkt16(msgresend15_copy);
                                   pkt_optical_temp_send->insertMessage(msgresend15_copy);
                                   pkt_optical_temp_send->setOptical_num(16);
                               }
                                   break;									   
							   case 16:
                               {
                                   pkt *msgresend16 = check_and_cast<pkt*>(BackupQueue[BackupQueueIndex].pop());
                                   pkt *msgresend16_copy = msgresend16->dup();

                                   BackupQueue[BackupQueueIndex].insert(msgresend16);

                                   int countNow16 = msgresend16_copy->getHopCountNow();
                                   pkt_optical_temp_send->setNextToR( msgresend16_copy->getHop(countNow16 + 2));
                                   pkt_optical_temp_send->setPreviousToR(msgresend16_copy->getHop(countNow16 ));
                                   pkt_optical_temp_send->setTxIndex(msgresend16_copy->getTxIndex());
                                   pkt_optical_temp_send->setGroupToRIndex( msgresend16_copy->getGroupToRIndex() );
                                   msgresend16_copy->setHopCountNow(countNow16 + 1);

                                   pkt_optical_temp_send->setPkt17(msgresend16_copy);
                                   pkt_optical_temp_send->insertMessage(msgresend16_copy);
                                   pkt_optical_temp_send->setOptical_num(17);
                               }
                                   break;	
							   case 17:
                               {
                                   pkt *msgresend17 = check_and_cast<pkt*>(BackupQueue[BackupQueueIndex].pop());
                                   pkt *msgresend17_copy = msgresend17->dup();

                                   BackupQueue[BackupQueueIndex].insert(msgresend17);

                                   int countNow17 = msgresend17_copy->getHopCountNow();
                                   pkt_optical_temp_send->setNextToR( msgresend17_copy->getHop(countNow17 + 2));
                                   pkt_optical_temp_send->setPreviousToR(msgresend17_copy->getHop(countNow17 ));
                                   pkt_optical_temp_send->setTxIndex(msgresend17_copy->getTxIndex());
                                   pkt_optical_temp_send->setGroupToRIndex( msgresend17_copy->getGroupToRIndex() );
                                   msgresend17_copy->setHopCountNow(countNow17 + 1);

                                   pkt_optical_temp_send->setPkt18(msgresend17_copy);
                                   pkt_optical_temp_send->insertMessage(msgresend17_copy);
                                   pkt_optical_temp_send->setOptical_num(18);
                               }
                                   break;
							   case 18:
                               {
                                   pkt *msgresend18 = check_and_cast<pkt*>(BackupQueue[BackupQueueIndex].pop());
                                   pkt *msgresend18_copy = msgresend18->dup();

                                   BackupQueue[BackupQueueIndex].insert(msgresend18);

                                   int countNow18 = msgresend18_copy->getHopCountNow();
                                   pkt_optical_temp_send->setNextToR( msgresend18_copy->getHop(countNow18 + 2));
                                   pkt_optical_temp_send->setPreviousToR(msgresend18_copy->getHop(countNow18 ));
                                   pkt_optical_temp_send->setTxIndex(msgresend18_copy->getTxIndex());
                                   pkt_optical_temp_send->setGroupToRIndex( msgresend18_copy->getGroupToRIndex() );
                                   msgresend18_copy->setHopCountNow(countNow18 + 1);

                                   pkt_optical_temp_send->setPkt19(msgresend18_copy);
                                   pkt_optical_temp_send->insertMessage(msgresend18_copy);
                                   pkt_optical_temp_send->setOptical_num(19);
                               }
                                   break;
							   case 19:
                               {
                                   pkt *msgresend19 = check_and_cast<pkt*>(BackupQueue[BackupQueueIndex].pop());
                                   pkt *msgresend19_copy = msgresend19->dup();

                                   BackupQueue[BackupQueueIndex].insert(msgresend19);

                                   int countNow19 = msgresend19_copy->getHopCountNow();
                                   pkt_optical_temp_send->setNextToR( msgresend19_copy->getHop(countNow19 + 2));
                                   pkt_optical_temp_send->setPreviousToR(msgresend19_copy->getHop(countNow19 ));
                                   pkt_optical_temp_send->setTxIndex(msgresend19_copy->getTxIndex());
                                   pkt_optical_temp_send->setGroupToRIndex( msgresend19_copy->getGroupToRIndex() );
                                   msgresend19_copy->setHopCountNow(countNow19 + 1);

                                   pkt_optical_temp_send->setPkt20(msgresend19_copy);
                                   pkt_optical_temp_send->insertMessage(msgresend19_copy);
                                   pkt_optical_temp_send->setOptical_num(20);
                               }
                                   break;
							   case 20:
                               {
                                   pkt *msgresend20 = check_and_cast<pkt*>(BackupQueue[BackupQueueIndex].pop());
                                   pkt *msgresend20_copy = msgresend20->dup();

                                   BackupQueue[BackupQueueIndex].insert(msgresend20);

                                   int countNow20 = msgresend20_copy->getHopCountNow();
                                   pkt_optical_temp_send->setNextToR( msgresend20_copy->getHop(countNow20 + 2));
                                   pkt_optical_temp_send->setPreviousToR(msgresend20_copy->getHop(countNow20 ));
                                   pkt_optical_temp_send->setTxIndex(msgresend20_copy->getTxIndex());
                                   pkt_optical_temp_send->setGroupToRIndex( msgresend20_copy->getGroupToRIndex() );
                                   msgresend20_copy->setHopCountNow(countNow20 + 1);

                                   pkt_optical_temp_send->setPkt21(msgresend20_copy);
                                   pkt_optical_temp_send->insertMessage(msgresend20_copy);
                                   pkt_optical_temp_send->setOptical_num(21);
                               }
                                   break;									   
							   case 21:
                               {
                                   pkt *msgresend21 = check_and_cast<pkt*>(BackupQueue[BackupQueueIndex].pop());
                                   pkt *msgresend21_copy = msgresend21->dup();

                                   BackupQueue[BackupQueueIndex].insert(msgresend21);

                                   int countNow21 = msgresend21_copy->getHopCountNow();
                                   pkt_optical_temp_send->setNextToR( msgresend21_copy->getHop(countNow21 + 2));
                                   pkt_optical_temp_send->setPreviousToR(msgresend21_copy->getHop(countNow21 ));
                                   pkt_optical_temp_send->setTxIndex(msgresend21_copy->getTxIndex());
                                   pkt_optical_temp_send->setGroupToRIndex( msgresend21_copy->getGroupToRIndex() );
                                   msgresend21_copy->setHopCountNow(countNow21 + 1);

                                   pkt_optical_temp_send->setPkt22(msgresend21_copy);
                                   pkt_optical_temp_send->insertMessage(msgresend21_copy);
                                   pkt_optical_temp_send->setOptical_num(22);
                               }
                                   break;
							   case 22:
                               {
                                   pkt *msgresend22 = check_and_cast<pkt*>(BackupQueue[BackupQueueIndex].pop());
                                   pkt *msgresend22_copy = msgresend22->dup();

                                   BackupQueue[BackupQueueIndex].insert(msgresend22);

                                   int countNow22 = msgresend22_copy->getHopCountNow();
                                   pkt_optical_temp_send->setNextToR( msgresend22_copy->getHop(countNow22 + 2));
                                   pkt_optical_temp_send->setPreviousToR(msgresend22_copy->getHop(countNow22 ));
                                   pkt_optical_temp_send->setTxIndex(msgresend22_copy->getTxIndex());
                                   pkt_optical_temp_send->setGroupToRIndex( msgresend22_copy->getGroupToRIndex() );
                                   msgresend22_copy->setHopCountNow(countNow22 + 1);

                                   pkt_optical_temp_send->setPkt23(msgresend22_copy);
                                   pkt_optical_temp_send->insertMessage(msgresend22_copy);
                                   pkt_optical_temp_send->setOptical_num(23);
                               }
                                   break;		
							   case 23:
                               {
                                   pkt *msgresend23 = check_and_cast<pkt*>(BackupQueue[BackupQueueIndex].pop());
                                   pkt *msgresend23_copy = msgresend23->dup();

                                   BackupQueue[BackupQueueIndex].insert(msgresend23);

                                   int countNow23 = msgresend23_copy->getHopCountNow();
                                   pkt_optical_temp_send->setNextToR( msgresend23_copy->getHop(countNow23 + 2));
                                   pkt_optical_temp_send->setPreviousToR(msgresend23_copy->getHop(countNow23 ));
                                   pkt_optical_temp_send->setTxIndex(msgresend23_copy->getTxIndex());
                                   pkt_optical_temp_send->setGroupToRIndex( msgresend23_copy->getGroupToRIndex() );
                                   msgresend23_copy->setHopCountNow(countNow23 + 1);

                                   pkt_optical_temp_send->setPkt24(msgresend23_copy);
                                   pkt_optical_temp_send->insertMessage(msgresend23_copy);
                                   pkt_optical_temp_send->setOptical_num(24);
                               }
                                   break;
							   case 24:
                               {
                                   pkt *msgresend24 = check_and_cast<pkt*>(BackupQueue[BackupQueueIndex].pop());
                                   pkt *msgresend24_copy = msgresend24->dup();

                                   BackupQueue[BackupQueueIndex].insert(msgresend24);

                                   int countNow24 = msgresend24_copy->getHopCountNow();
                                   pkt_optical_temp_send->setNextToR( msgresend24_copy->getHop(countNow24 + 2));
                                   pkt_optical_temp_send->setPreviousToR(msgresend24_copy->getHop(countNow24 ));
                                   pkt_optical_temp_send->setTxIndex(msgresend24_copy->getTxIndex());
                                   pkt_optical_temp_send->setGroupToRIndex( msgresend24_copy->getGroupToRIndex() );
                                   msgresend24_copy->setHopCountNow(countNow24 + 1);

                                   pkt_optical_temp_send->setPkt25(msgresend24_copy);
                                   pkt_optical_temp_send->insertMessage(msgresend24_copy);
                                   pkt_optical_temp_send->setOptical_num(25);
                               }
                                   break;	   
                          }
               			 }
                    }


                TXSend++;
                QueueArrayflag = false;
                send(pkt_optical_temp_send,"out");
//                    EV<<"Send Backup Queue"<<"\n"<<"Agre_Queue2OPS "<<getIndex()<<"  Buffer size is"<<Queue2OPS.length()<<"\n" ;
            }
            else if(QueueArrayflag == false){
                if(maxQueuelength > 0){
                    Optical *pkt_optical_temp_send = new Optical();
                    pkt_optical_temp_send->setKind(1000);
                    for(int i=0;i<OpticalPacketNum;i++){
                            if(Queue2OPS[maxQueueIndex].length()>0){
                               switch(i){
                                   case 0:
                                   {
                                       pkt *pkt_temp_send = check_and_cast<pkt*>(Queue2OPS[maxQueueIndex].pop());
                                       int countNow0f = pkt_temp_send->getHopCountNow();

                                       pkt *pkt_temp_copy = pkt_temp_send->dup();
                                       BackupQueue[maxQueueIndex].insert(pkt_temp_copy);
                                       pkt_optical_temp_send->setNextToR( pkt_temp_send->getHop(countNow0f + 2));
                                       pkt_optical_temp_send->setPreviousToR(pkt_temp_send->getHop(countNow0f ));
                                       pkt_optical_temp_send->setTxIndex(pkt_temp_send->getTxIndex());
                                       pkt_optical_temp_send->setGroupToRIndex( pkt_temp_send->getGroupToRIndex() );


                                       pkt_temp_send->setHopCountNow(countNow0f + 1);

                                       pkt_optical_temp_send->setPkt1(pkt_temp_send);
                                       pkt_optical_temp_send->insertMessage(pkt_temp_send);
                                       pkt_optical_temp_send->setOptical_num(1);
                                   }
                                       break;
                                   case 1:
                                   {
                                       pkt *pkt_temp1_send = check_and_cast<pkt*>(Queue2OPS[maxQueueIndex].pop());
                                       int countNow1f = pkt_temp1_send->getHopCountNow();

                                       pkt *pkt_temp1_copy = pkt_temp1_send->dup();
                                       BackupQueue[maxQueueIndex].insert(pkt_temp1_copy);
                                       pkt_optical_temp_send->setNextToR( pkt_temp1_send->getHop(countNow1f + 2));
                                       pkt_optical_temp_send->setPreviousToR(pkt_temp1_send->getHop(countNow1f ));
                                       pkt_optical_temp_send->setTxIndex(pkt_temp1_send->getTxIndex());
                                       pkt_optical_temp_send->setGroupToRIndex( pkt_temp1_send->getGroupToRIndex() );

                                       pkt_temp1_send->setHopCountNow(countNow1f + 1);
                                       pkt_optical_temp_send->setPkt2(pkt_temp1_send);
                                       pkt_optical_temp_send->insertMessage(pkt_temp1_send);
                                       pkt_optical_temp_send->setOptical_num(2);
                                   }
                                       break;
                                   case 2:
                                   {
                                       pkt *pkt_temp2_send = check_and_cast<pkt*>(Queue2OPS[maxQueueIndex].pop());
                                       int countNow2f = pkt_temp2_send->getHopCountNow();

                                       pkt *pkt_temp2_copy = pkt_temp2_send->dup();
                                       BackupQueue[maxQueueIndex].insert(pkt_temp2_copy);
                                       pkt_optical_temp_send->setNextToR( pkt_temp2_send->getHop(countNow2f + 2));
                                       pkt_optical_temp_send->setPreviousToR(pkt_temp2_send->getHop(countNow2f ));
                                       pkt_optical_temp_send->setTxIndex(pkt_temp2_send->getTxIndex());
                                       pkt_optical_temp_send->setGroupToRIndex( pkt_temp2_send->getGroupToRIndex() );

                                       pkt_temp2_send->setHopCountNow(countNow2f + 1);
                                       pkt_optical_temp_send->setPkt3(pkt_temp2_send);
                                       pkt_optical_temp_send->insertMessage(pkt_temp2_send);
                                       pkt_optical_temp_send->setOptical_num(3);
                                   }
                                       break;
                                   case 3:
                                   {
                                       pkt *pkt_temp3_send = check_and_cast<pkt*>(Queue2OPS[maxQueueIndex].pop());
                                       int countNow3f = pkt_temp3_send->getHopCountNow();

                                       pkt *pkt_temp3_copy = pkt_temp3_send->dup();
                                       BackupQueue[maxQueueIndex].insert(pkt_temp3_copy);
                                       pkt_optical_temp_send->setNextToR( pkt_temp3_send->getHop(countNow3f + 2));
                                       pkt_optical_temp_send->setPreviousToR(pkt_temp3_send->getHop(countNow3f ));
                                       pkt_optical_temp_send->setTxIndex(pkt_temp3_send->getTxIndex());
                                       pkt_optical_temp_send->setGroupToRIndex( pkt_temp3_send->getGroupToRIndex() );

                                       pkt_temp3_send->setHopCountNow(countNow3f + 1);
                                       pkt_optical_temp_send->setPkt4(pkt_temp3_send);
                                       pkt_optical_temp_send->insertMessage(pkt_temp3_send);
                                       pkt_optical_temp_send->setOptical_num(4);
                                   }
                                       break;
                                   case 4:
                                   {
                                       pkt *pkt_temp4_send = check_and_cast<pkt*>(Queue2OPS[maxQueueIndex].pop());
                                       int countNow4f = pkt_temp4_send->getHopCountNow();

                                       pkt *pkt_temp4_copy = pkt_temp4_send->dup();
                                       BackupQueue[maxQueueIndex].insert(pkt_temp4_copy);
                                       pkt_optical_temp_send->setNextToR( pkt_temp4_send->getHop(countNow4f + 2));
                                       pkt_optical_temp_send->setPreviousToR(pkt_temp4_send->getHop(countNow4f ));
                                       pkt_optical_temp_send->setTxIndex(pkt_temp4_send->getTxIndex());
                                       pkt_optical_temp_send->setGroupToRIndex( pkt_temp4_send->getGroupToRIndex() );

                                       pkt_temp4_send->setHopCountNow(countNow4f + 1);
                                       pkt_optical_temp_send->setPkt5(pkt_temp4_send);
                                       pkt_optical_temp_send->insertMessage(pkt_temp4_send);
                                       pkt_optical_temp_send->setOptical_num(5);
                                   }
                                       break;
                                   case 5:
                                   {
                                       pkt *pkt_temp5_send = check_and_cast<pkt*>(Queue2OPS[maxQueueIndex].pop());
                                       int countNow4f = pkt_temp5_send->getHopCountNow();

                                       pkt *pkt_temp5_copy = pkt_temp5_send->dup();
                                       BackupQueue[maxQueueIndex].insert(pkt_temp5_copy);
                                       pkt_optical_temp_send->setNextToR( pkt_temp5_send->getHop(countNow4f + 2));
                                       pkt_optical_temp_send->setPreviousToR(pkt_temp5_send->getHop(countNow4f ));
                                       pkt_optical_temp_send->setTxIndex(pkt_temp5_send->getTxIndex());
                                       pkt_optical_temp_send->setGroupToRIndex( pkt_temp5_send->getGroupToRIndex() );

                                       pkt_temp5_send->setHopCountNow(countNow4f + 1);
                                       pkt_optical_temp_send->setPkt6(pkt_temp5_send);
                                       pkt_optical_temp_send->insertMessage(pkt_temp5_send);
                                       pkt_optical_temp_send->setOptical_num(6);
                                   }
                                       break;									   
                                   case 6:
                                   {
                                       pkt *pkt_temp6_send = check_and_cast<pkt*>(Queue2OPS[maxQueueIndex].pop());
                                       int countNow4f = pkt_temp6_send->getHopCountNow();

                                       pkt *pkt_temp6_copy = pkt_temp6_send->dup();
                                       BackupQueue[maxQueueIndex].insert(pkt_temp6_copy);
                                       pkt_optical_temp_send->setNextToR( pkt_temp6_send->getHop(countNow4f + 2));
                                       pkt_optical_temp_send->setPreviousToR(pkt_temp6_send->getHop(countNow4f ));
                                       pkt_optical_temp_send->setTxIndex(pkt_temp6_send->getTxIndex());
                                       pkt_optical_temp_send->setGroupToRIndex( pkt_temp6_send->getGroupToRIndex() );

                                       pkt_temp6_send->setHopCountNow(countNow4f + 1);
                                       pkt_optical_temp_send->setPkt7(pkt_temp6_send);
                                       pkt_optical_temp_send->insertMessage(pkt_temp6_send);
                                       pkt_optical_temp_send->setOptical_num(7);
                                   }
                                       break;									   
                                   case 7:
                                   {
                                       pkt *pkt_temp7_send = check_and_cast<pkt*>(Queue2OPS[maxQueueIndex].pop());
                                       int countNow4f = pkt_temp7_send->getHopCountNow();

                                       pkt *pkt_temp7_copy = pkt_temp7_send->dup();
                                       BackupQueue[maxQueueIndex].insert(pkt_temp7_copy);
                                       pkt_optical_temp_send->setNextToR( pkt_temp7_send->getHop(countNow4f + 2));
                                       pkt_optical_temp_send->setPreviousToR(pkt_temp7_send->getHop(countNow4f ));
                                       pkt_optical_temp_send->setTxIndex(pkt_temp7_send->getTxIndex());
                                       pkt_optical_temp_send->setGroupToRIndex( pkt_temp7_send->getGroupToRIndex() );

                                       pkt_temp7_send->setHopCountNow(countNow4f + 1);
                                       pkt_optical_temp_send->setPkt8(pkt_temp7_send);
                                       pkt_optical_temp_send->insertMessage(pkt_temp7_send);
                                       pkt_optical_temp_send->setOptical_num(8);
                                   }
                                       break;									   
                                   case 8:
                                   {
                                       pkt *pkt_temp8_send = check_and_cast<pkt*>(Queue2OPS[maxQueueIndex].pop());
                                       int countNow4f = pkt_temp8_send->getHopCountNow();

                                       pkt *pkt_temp8_copy = pkt_temp8_send->dup();
                                       BackupQueue[maxQueueIndex].insert(pkt_temp8_copy);
                                       pkt_optical_temp_send->setNextToR( pkt_temp8_send->getHop(countNow4f + 2));
                                       pkt_optical_temp_send->setPreviousToR(pkt_temp8_send->getHop(countNow4f ));
                                       pkt_optical_temp_send->setTxIndex(pkt_temp8_send->getTxIndex());
                                       pkt_optical_temp_send->setGroupToRIndex( pkt_temp8_send->getGroupToRIndex() );

                                       pkt_temp8_send->setHopCountNow(countNow4f + 1);
                                       pkt_optical_temp_send->setPkt9(pkt_temp8_send);
                                       pkt_optical_temp_send->insertMessage(pkt_temp8_send);
                                       pkt_optical_temp_send->setOptical_num(9);
                                   }
                                       break;									   
                                   case 9:
                                   {
                                       pkt *pkt_temp9_send = check_and_cast<pkt*>(Queue2OPS[maxQueueIndex].pop());
                                       int countNow4f = pkt_temp9_send->getHopCountNow();

                                       pkt *pkt_temp9_copy = pkt_temp9_send->dup();
                                       BackupQueue[maxQueueIndex].insert(pkt_temp9_copy);
                                       pkt_optical_temp_send->setNextToR( pkt_temp9_send->getHop(countNow4f + 2));
                                       pkt_optical_temp_send->setPreviousToR(pkt_temp9_send->getHop(countNow4f ));
                                       pkt_optical_temp_send->setTxIndex(pkt_temp9_send->getTxIndex());
                                       pkt_optical_temp_send->setGroupToRIndex( pkt_temp9_send->getGroupToRIndex() );

                                       pkt_temp9_send->setHopCountNow(countNow4f + 1);
                                       pkt_optical_temp_send->setPkt10(pkt_temp9_send);
                                       pkt_optical_temp_send->insertMessage(pkt_temp9_send);
                                       pkt_optical_temp_send->setOptical_num(10);
                                   }
                                       break;									   
                                   case 10:
                                   {
                                       pkt *pkt_temp10_send = check_and_cast<pkt*>(Queue2OPS[maxQueueIndex].pop());
                                       int countNow4f = pkt_temp10_send->getHopCountNow();

                                       pkt *pkt_temp10_copy = pkt_temp10_send->dup();
                                       BackupQueue[maxQueueIndex].insert(pkt_temp10_copy);
                                       pkt_optical_temp_send->setNextToR( pkt_temp10_send->getHop(countNow4f + 2));
                                       pkt_optical_temp_send->setPreviousToR(pkt_temp10_send->getHop(countNow4f ));
                                       pkt_optical_temp_send->setTxIndex(pkt_temp10_send->getTxIndex());
                                       pkt_optical_temp_send->setGroupToRIndex( pkt_temp10_send->getGroupToRIndex() );

                                       pkt_temp10_send->setHopCountNow(countNow4f + 1);
                                       pkt_optical_temp_send->setPkt11(pkt_temp10_send);
                                       pkt_optical_temp_send->insertMessage(pkt_temp10_send);
                                       pkt_optical_temp_send->setOptical_num(11);
                                   }
                                       break;									   
                                   case 11:
                                   {
                                       pkt *pkt_temp11_send = check_and_cast<pkt*>(Queue2OPS[maxQueueIndex].pop());
                                       int countNow4f = pkt_temp11_send->getHopCountNow();

                                       pkt *pkt_temp11_copy = pkt_temp11_send->dup();
                                       BackupQueue[maxQueueIndex].insert(pkt_temp11_copy);
                                       pkt_optical_temp_send->setNextToR( pkt_temp11_send->getHop(countNow4f + 2));
                                       pkt_optical_temp_send->setPreviousToR(pkt_temp11_send->getHop(countNow4f ));
                                       pkt_optical_temp_send->setTxIndex(pkt_temp11_send->getTxIndex());
                                       pkt_optical_temp_send->setGroupToRIndex( pkt_temp11_send->getGroupToRIndex() );

                                       pkt_temp11_send->setHopCountNow(countNow4f + 1);
                                       pkt_optical_temp_send->setPkt12(pkt_temp11_send);
                                       pkt_optical_temp_send->insertMessage(pkt_temp11_send);
                                       pkt_optical_temp_send->setOptical_num(12);
                                   }
                                       break;									   
                                   case 12:
                                   {
                                       pkt *pkt_temp12_send = check_and_cast<pkt*>(Queue2OPS[maxQueueIndex].pop());
                                       int countNow4f = pkt_temp12_send->getHopCountNow();

                                       pkt *pkt_temp12_copy = pkt_temp12_send->dup();
                                       BackupQueue[maxQueueIndex].insert(pkt_temp12_copy);
                                       pkt_optical_temp_send->setNextToR( pkt_temp12_send->getHop(countNow4f + 2));
                                       pkt_optical_temp_send->setPreviousToR(pkt_temp12_send->getHop(countNow4f ));
                                       pkt_optical_temp_send->setTxIndex(pkt_temp12_send->getTxIndex());
                                       pkt_optical_temp_send->setGroupToRIndex( pkt_temp12_send->getGroupToRIndex() );

                                       pkt_temp12_send->setHopCountNow(countNow4f + 1);
                                       pkt_optical_temp_send->setPkt13(pkt_temp12_send);
                                       pkt_optical_temp_send->insertMessage(pkt_temp12_send);
                                       pkt_optical_temp_send->setOptical_num(13);
                                   }
                                       break;
                                   case 13:
                                   {
                                       pkt *pkt_temp13_send = check_and_cast<pkt*>(Queue2OPS[maxQueueIndex].pop());
                                       int countNow4f = pkt_temp13_send->getHopCountNow();

                                       pkt *pkt_temp13_copy = pkt_temp13_send->dup();
                                       BackupQueue[maxQueueIndex].insert(pkt_temp13_copy);
                                       pkt_optical_temp_send->setNextToR( pkt_temp13_send->getHop(countNow4f + 2));
                                       pkt_optical_temp_send->setPreviousToR(pkt_temp13_send->getHop(countNow4f ));
                                       pkt_optical_temp_send->setTxIndex(pkt_temp13_send->getTxIndex());
                                       pkt_optical_temp_send->setGroupToRIndex( pkt_temp13_send->getGroupToRIndex() );

                                       pkt_temp13_send->setHopCountNow(countNow4f + 1);
                                       pkt_optical_temp_send->setPkt14(pkt_temp13_send);
                                       pkt_optical_temp_send->insertMessage(pkt_temp13_send);
                                       pkt_optical_temp_send->setOptical_num(14);
                                   }
                                       break;
                                   case 14:
                                   {
                                       pkt *pkt_temp14_send = check_and_cast<pkt*>(Queue2OPS[maxQueueIndex].pop());
                                       int countNow4f = pkt_temp14_send->getHopCountNow();

                                       pkt *pkt_temp14_copy = pkt_temp14_send->dup();
                                       BackupQueue[maxQueueIndex].insert(pkt_temp14_copy);
                                       pkt_optical_temp_send->setNextToR( pkt_temp14_send->getHop(countNow4f + 2));
                                       pkt_optical_temp_send->setPreviousToR(pkt_temp14_send->getHop(countNow4f ));
                                       pkt_optical_temp_send->setTxIndex(pkt_temp14_send->getTxIndex());
                                       pkt_optical_temp_send->setGroupToRIndex( pkt_temp14_send->getGroupToRIndex() );

                                       pkt_temp14_send->setHopCountNow(countNow4f + 1);
                                       pkt_optical_temp_send->setPkt15(pkt_temp14_send);
                                       pkt_optical_temp_send->insertMessage(pkt_temp14_send);
                                       pkt_optical_temp_send->setOptical_num(15);
                                   }
                                       break;
                                   case 15:
                                   {
                                       pkt *pkt_temp15_send = check_and_cast<pkt*>(Queue2OPS[maxQueueIndex].pop());
                                       int countNow4f = pkt_temp15_send->getHopCountNow();

                                       pkt *pkt_temp15_copy = pkt_temp15_send->dup();
                                       BackupQueue[maxQueueIndex].insert(pkt_temp15_copy);
                                       pkt_optical_temp_send->setNextToR( pkt_temp15_send->getHop(countNow4f + 2));
                                       pkt_optical_temp_send->setPreviousToR(pkt_temp15_send->getHop(countNow4f ));
                                       pkt_optical_temp_send->setTxIndex(pkt_temp15_send->getTxIndex());
                                       pkt_optical_temp_send->setGroupToRIndex( pkt_temp15_send->getGroupToRIndex() );

                                       pkt_temp15_send->setHopCountNow(countNow4f + 1);
                                       pkt_optical_temp_send->setPkt16(pkt_temp15_send);
                                       pkt_optical_temp_send->insertMessage(pkt_temp15_send);
                                       pkt_optical_temp_send->setOptical_num(16);
                                   }
                                       break;									   
                                   case 16:
                                   {
                                       pkt *pkt_temp16_send = check_and_cast<pkt*>(Queue2OPS[maxQueueIndex].pop());
                                       int countNow4f = pkt_temp16_send->getHopCountNow();

                                       pkt *pkt_temp16_copy = pkt_temp16_send->dup();
                                       BackupQueue[maxQueueIndex].insert(pkt_temp16_copy);
                                       pkt_optical_temp_send->setNextToR( pkt_temp16_send->getHop(countNow4f + 2));
                                       pkt_optical_temp_send->setPreviousToR(pkt_temp16_send->getHop(countNow4f ));
                                       pkt_optical_temp_send->setTxIndex(pkt_temp16_send->getTxIndex());
                                       pkt_optical_temp_send->setGroupToRIndex( pkt_temp16_send->getGroupToRIndex() );

                                       pkt_temp16_send->setHopCountNow(countNow4f + 1);
                                       pkt_optical_temp_send->setPkt17(pkt_temp16_send);
                                       pkt_optical_temp_send->insertMessage(pkt_temp16_send);
                                       pkt_optical_temp_send->setOptical_num(17);
                                   }
                                       break;	
                                   case 17:
                                   {
                                       pkt *pkt_temp17_send = check_and_cast<pkt*>(Queue2OPS[maxQueueIndex].pop());
                                       int countNow4f = pkt_temp17_send->getHopCountNow();

                                       pkt *pkt_temp17_copy = pkt_temp17_send->dup();
                                       BackupQueue[maxQueueIndex].insert(pkt_temp17_copy);
                                       pkt_optical_temp_send->setNextToR( pkt_temp17_send->getHop(countNow4f + 2));
                                       pkt_optical_temp_send->setPreviousToR(pkt_temp17_send->getHop(countNow4f ));
                                       pkt_optical_temp_send->setTxIndex(pkt_temp17_send->getTxIndex());
                                       pkt_optical_temp_send->setGroupToRIndex( pkt_temp17_send->getGroupToRIndex() );

                                       pkt_temp17_send->setHopCountNow(countNow4f + 1);
                                       pkt_optical_temp_send->setPkt18(pkt_temp17_send);
                                       pkt_optical_temp_send->insertMessage(pkt_temp17_send);
                                       pkt_optical_temp_send->setOptical_num(18);
                                   }
                                       break;
                                   case 18:
                                   {
                                       pkt *pkt_temp18_send = check_and_cast<pkt*>(Queue2OPS[maxQueueIndex].pop());
                                       int countNow4f = pkt_temp18_send->getHopCountNow();

                                       pkt *pkt_temp18_copy = pkt_temp18_send->dup();
                                       BackupQueue[maxQueueIndex].insert(pkt_temp18_copy);
                                       pkt_optical_temp_send->setNextToR( pkt_temp18_send->getHop(countNow4f + 2));
                                       pkt_optical_temp_send->setPreviousToR(pkt_temp18_send->getHop(countNow4f ));
                                       pkt_optical_temp_send->setTxIndex(pkt_temp18_send->getTxIndex());
                                       pkt_optical_temp_send->setGroupToRIndex( pkt_temp18_send->getGroupToRIndex() );

                                       pkt_temp18_send->setHopCountNow(countNow4f + 1);
                                       pkt_optical_temp_send->setPkt19(pkt_temp18_send);
                                       pkt_optical_temp_send->insertMessage(pkt_temp18_send);
                                       pkt_optical_temp_send->setOptical_num(19);
                                   }
                                       break;
                                   case 19:
                                   {
                                       pkt *pkt_temp19_send = check_and_cast<pkt*>(Queue2OPS[maxQueueIndex].pop());
                                       int countNow4f = pkt_temp19_send->getHopCountNow();

                                       pkt *pkt_temp19_copy = pkt_temp19_send->dup();
                                       BackupQueue[maxQueueIndex].insert(pkt_temp19_copy);
                                       pkt_optical_temp_send->setNextToR( pkt_temp19_send->getHop(countNow4f + 2));
                                       pkt_optical_temp_send->setPreviousToR(pkt_temp19_send->getHop(countNow4f ));
                                       pkt_optical_temp_send->setTxIndex(pkt_temp19_send->getTxIndex());
                                       pkt_optical_temp_send->setGroupToRIndex( pkt_temp19_send->getGroupToRIndex() );

                                       pkt_temp19_send->setHopCountNow(countNow4f + 1);
                                       pkt_optical_temp_send->setPkt20(pkt_temp19_send);
                                       pkt_optical_temp_send->insertMessage(pkt_temp19_send);
                                       pkt_optical_temp_send->setOptical_num(20);
                                   }
                                       break;
                                   case 20:
                                   {
                                       pkt *pkt_temp20_send = check_and_cast<pkt*>(Queue2OPS[maxQueueIndex].pop());
                                       int countNow4f = pkt_temp20_send->getHopCountNow();

                                       pkt *pkt_temp20_copy = pkt_temp20_send->dup();
                                       BackupQueue[maxQueueIndex].insert(pkt_temp20_copy);
                                       pkt_optical_temp_send->setNextToR( pkt_temp20_send->getHop(countNow4f + 2));
                                       pkt_optical_temp_send->setPreviousToR(pkt_temp20_send->getHop(countNow4f ));
                                       pkt_optical_temp_send->setTxIndex(pkt_temp20_send->getTxIndex());
                                       pkt_optical_temp_send->setGroupToRIndex( pkt_temp20_send->getGroupToRIndex() );

                                       pkt_temp20_send->setHopCountNow(countNow4f + 1);
                                       pkt_optical_temp_send->setPkt21(pkt_temp20_send);
                                       pkt_optical_temp_send->insertMessage(pkt_temp20_send);
                                       pkt_optical_temp_send->setOptical_num(21);
                                   }
                                       break;
                                   case 21:
                                   {
                                       pkt *pkt_temp21_send = check_and_cast<pkt*>(Queue2OPS[maxQueueIndex].pop());
                                       int countNow4f = pkt_temp21_send->getHopCountNow();

                                       pkt *pkt_temp21_copy = pkt_temp21_send->dup();
                                       BackupQueue[maxQueueIndex].insert(pkt_temp21_copy);
                                       pkt_optical_temp_send->setNextToR( pkt_temp21_send->getHop(countNow4f + 2));
                                       pkt_optical_temp_send->setPreviousToR(pkt_temp21_send->getHop(countNow4f ));
                                       pkt_optical_temp_send->setTxIndex(pkt_temp21_send->getTxIndex());
                                       pkt_optical_temp_send->setGroupToRIndex( pkt_temp21_send->getGroupToRIndex() );

                                       pkt_temp21_send->setHopCountNow(countNow4f + 1);
                                       pkt_optical_temp_send->setPkt22(pkt_temp21_send);
                                       pkt_optical_temp_send->insertMessage(pkt_temp21_send);
                                       pkt_optical_temp_send->setOptical_num(22);
                                   }
                                       break;
                                   case 22:
                                   {
                                       pkt *pkt_temp22_send = check_and_cast<pkt*>(Queue2OPS[maxQueueIndex].pop());
                                       int countNow4f = pkt_temp22_send->getHopCountNow();

                                       pkt *pkt_temp22_copy = pkt_temp22_send->dup();
                                       BackupQueue[maxQueueIndex].insert(pkt_temp22_copy);
                                       pkt_optical_temp_send->setNextToR( pkt_temp22_send->getHop(countNow4f + 2));
                                       pkt_optical_temp_send->setPreviousToR(pkt_temp22_send->getHop(countNow4f ));
                                       pkt_optical_temp_send->setTxIndex(pkt_temp22_send->getTxIndex());
                                       pkt_optical_temp_send->setGroupToRIndex( pkt_temp22_send->getGroupToRIndex() );

                                       pkt_temp22_send->setHopCountNow(countNow4f + 1);
                                       pkt_optical_temp_send->setPkt23(pkt_temp22_send);
                                       pkt_optical_temp_send->insertMessage(pkt_temp22_send);
                                       pkt_optical_temp_send->setOptical_num(23);
                                   }
                                       break;
                                   case 23:
                                   {
                                       pkt *pkt_temp23_send = check_and_cast<pkt*>(Queue2OPS[maxQueueIndex].pop());
                                       int countNow4f = pkt_temp23_send->getHopCountNow();

                                       pkt *pkt_temp23_copy = pkt_temp23_send->dup();
                                       BackupQueue[maxQueueIndex].insert(pkt_temp23_copy);
                                       pkt_optical_temp_send->setNextToR( pkt_temp23_send->getHop(countNow4f + 2));
                                       pkt_optical_temp_send->setPreviousToR(pkt_temp23_send->getHop(countNow4f ));
                                       pkt_optical_temp_send->setTxIndex(pkt_temp23_send->getTxIndex());
                                       pkt_optical_temp_send->setGroupToRIndex( pkt_temp23_send->getGroupToRIndex() );

                                       pkt_temp23_send->setHopCountNow(countNow4f + 1);
                                       pkt_optical_temp_send->setPkt24(pkt_temp23_send);
                                       pkt_optical_temp_send->insertMessage(pkt_temp23_send);
                                       pkt_optical_temp_send->setOptical_num(24);
                                   }
                                       break;	
                                   case 24:
                                   {
                                       pkt *pkt_temp24_send = check_and_cast<pkt*>(Queue2OPS[maxQueueIndex].pop());
                                       int countNow4f = pkt_temp24_send->getHopCountNow();

                                       pkt *pkt_temp24_copy = pkt_temp24_send->dup();
                                       BackupQueue[maxQueueIndex].insert(pkt_temp24_copy);
                                       pkt_optical_temp_send->setNextToR( pkt_temp24_send->getHop(countNow4f + 2));
                                       pkt_optical_temp_send->setPreviousToR(pkt_temp24_send->getHop(countNow4f ));
                                       pkt_optical_temp_send->setTxIndex(pkt_temp24_send->getTxIndex());
                                       pkt_optical_temp_send->setGroupToRIndex( pkt_temp24_send->getGroupToRIndex() );

                                       pkt_temp24_send->setHopCountNow(countNow4f + 1);
                                       pkt_optical_temp_send->setPkt25(pkt_temp24_send);
                                       pkt_optical_temp_send->insertMessage(pkt_temp24_send);
                                       pkt_optical_temp_send->setOptical_num(25);
                                   }
                                       break;
									   
                               }
                            }
                    }

                    TXSend++;
                    gate("out")->getIndex();
                    send(pkt_optical_temp_send,"out");
                }
			}
            scheduleAt(simTime() + slot, local_clock);
            SimulatedClock ++;
        }
        else
        {
    //        EV<<"I am run out of msg, delete local_clock"<<"\n";
            cancelEvent(local_clock);
        }


    }
    else if(pkt_temp->getIndex() == 5){ // finish msg from headprocess
        finishflag = true;
//        EV<<"finishflag now is"<<finishflag<<"\n";
        delete msg;
    }
    else if(pkt_temp->getIndex() == 8){ // the ACK

            if(pkt_temp->getRetransmission() == 0){
                int Num = pkt_temp->getOptNum();
                int TORIndex = pkt_temp->getGroupToRIndex();
                for(int i=0;i<Num;i++){
                    if(BackupQueue[TORIndex].length()>0)
                        delete BackupQueue[TORIndex].pop();
                }
            }
            else{
                QueueArrayflag = true;
                BackupQueueIndex = pkt_temp->getGroupToRIndex();
                BackupQueueopticalNum = pkt_temp->getOptNum();
//                EV<<"ACK = 1"<<"\n"<<"Agre_Queue2OPS"<<getIndex()<<"  Buffer size is"<<Queue2OPS.length()<<"\n" ;
//                pkt *msgresend = check_and_cast<pkt*>(BackupQueue.pop());
//                BackupQueue.insert(msgresend);
            }
            delete msg;
    }
    else if(pkt_temp->getIndex() == 10){
            if(pkt_temp->getRetransmission() == 0){
                int Num = pkt_temp->getOptNum();
                int TORIndex = pkt_temp->getGroupToRIndex();
                for(int i=0;i<Num;i++){
                    if(BackupQueue[TORIndex].length()>0)
                        delete BackupQueue[TORIndex].pop();
                }
            }
            else{
                QueueArrayflag = true;
                BackupQueueIndex = pkt_temp->getGroupToRIndex();
                BackupQueueopticalNum = pkt_temp->getOptNum();
//                pkt *msgresend = check_and_cast<pkt*>(BackupQueue.pop());
//                BackupQueue.insert(msgresend);
            }
            delete msg;
    }

}

void TxBuffer::finish()
{
    recordScalar("Packet-lost-----Queue2OPS", Queue2OPSPacketlost);
    recordScalar("Cell-lost-From--Queue2OPS", Queue2OPSlost);
}





