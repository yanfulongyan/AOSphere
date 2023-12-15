/*
 * head.cc
 *
 *  Created on: Jul 3, 2015
 *      Author: fyan
 */

#include<stdio.h>
#include <string.h>
#include <omnetpp.h>
#include "pkt_m.h"
#include "Optical_m.h"
using namespace omnetpp;
int count2ToR[166400];
cQueue Queue2Server[166400];
bool Queue2Server_busy[166400];
int Queue2Serverlost[166400];
bool msgfromserver2headprocessorfinish[166400];


class HeadProcessor : public cSimpleModule
{
    private:
        int ToRDownRadix;
        int NumTx_ToR2ToR;
        int FOSRadix;
        simtime_t HeadProcessorfinishtime;
        int totalserverNum;
        int ToRserverNum;
        int ClusterToRNum;
        int AllToRNum;
        int GroupToR ;
        int AgreRadix;
        int CoreRadix;

        int Queue2ServerAveragelost;

        simtime_t Networktime_latency;
        simtime_t ToR2ToR_Latency;
        simtime_t finishtime_Agre;
        simtime_t finishtime_Core;
        simtime_t finishtime;

        int Down2ServerBufferSize;

        double slot;
        double slot_ToR;

        int NumTx;
        int NumTx_Core;

		int GroupToRCore;

        int TxIndex;
        int GroupToRIndex;
        int numReceived_Total;
        int numReceived_ToR;
        int PnumReceived_ToR;
        int FinishserverNum;
        int coutserver;

        double DelayOfProcessor;

        double TxBufferTime;
        double PreambleInsertdelay;
        double BurstProcessDelay;

        simtime_t AverageNetworkLatency;
        simtime_t PAverageNetworkLatency;
        simtime_t PToR2ToR_Latency;

        simtime_t interval_0;

        double UplinkdataRate;
        double DownlinkDataRate;
        int CellSize;
        int CellNum;

        int tt0;
        int receivedPackets;
        int xx;
        int caseofcellsize;

        bool FlagToRFlowControl;

        pkt * local_pkt_ToR;
    protected:

         virtual void initialize();
         virtual void handleMessage(cMessage *msg);
         virtual void finish();
    public:
         HeadProcessor();
         virtual ~HeadProcessor();
};

Define_Module(HeadProcessor);

HeadProcessor::HeadProcessor(){
    local_pkt_ToR = NULL;

}

HeadProcessor::~HeadProcessor(){

 //   cancelAndDelete(local_pkt_ToR);
    cancelAndDelete(local_pkt_ToR);
    if(getIndex()==0){
        for(int i=0;i<166400;i++){
            while(!Queue2Server[i].empty())
                delete (pkt *)Queue2Server[i].pop();
        }
    }
}
void HeadProcessor::initialize()
{

    ToRDownRadix = par("ToRDownRadix");
    FOSRadix = par("FOSRadix");
    xx = 0;
    GroupToRIndex = 0;
    HeadProcessorfinishtime = 0;
    CellSize = par("CellSize");
    totalserverNum = par("ServerNum");
    ToRserverNum = par("ToRDownRadix");
    AllToRNum = totalserverNum/ToRserverNum;
//    ClusterToRNum = sqrt(AllToRNum);
    ClusterToRNum = par("AgreRadix");
    GroupToR = par("GroupToR");
    AgreRadix = par("AgreRadix");
    CoreRadix = par("CoreRadix");
    NumTx = par("NumTx");
    NumTx_ToR2ToR = par("NumTx_ToR2ToR");
    NumTx_Core = par("NumTx_Core");
    GroupToRCore = par("GroupToRCore");
    Down2ServerBufferSize = par("Down2ServerBufferSize");
    UplinkdataRate = par("dataRate");
    DownlinkDataRate = par("linkDataRate");

    if(getIndex()==0){
        for(int i=0;i<166400;i++){
            msgfromserver2headprocessorfinish[i] = false;
            count2ToR[i] = 0;
            Queue2Server_busy[i]=0;
            Queue2Serverlost[i]=0;
            count2ToR[i] = 0;
        }
        FinishserverNum = 0;
    }

    receivedPackets = 0;
    HeadProcessorfinishtime = 0;
    numReceived_Total = 0;
    numReceived_ToR = 0;
    PnumReceived_ToR = 0;
    Networktime_latency = 0;
    ToR2ToR_Latency = 0;

    AverageNetworkLatency = 0;
    PAverageNetworkLatency = 0;
    coutserver = 0;


    slot = 48e-9;
    slot_ToR = 51.2e-9;

    DelayOfProcessor = 80e-9;

    Queue2ServerAveragelost = 0;

    caseofcellsize = 0;

    CellNum = 0;
    PreambleInsertdelay = par("PreambleInsertdelay");
    BurstProcessDelay = par("BurstProcessDelay");
    TxBufferTime = CellSize*8/DownlinkDataRate;
    finishtime = 0;
    finishtime_Agre = 0;
    finishtime_Core = 0;
}


void HeadProcessor::handleMessage(cMessage *msg)
{

    if(msg->getKind() == 1000){

        Optical *Optical_pkt_temp =  check_and_cast<Optical*>(msg);

        int xoptnum1 = Optical_pkt_temp->getOptical_num();
        cModule *targetModule = getParentModule();
        int tt = targetModule->getIndex();
        ///////////////////////////////////////
        //cModule *targetModule = getParentModule();
        //int tt = getIndex();
        //////////////////////////////////////////////////////////////////////////////////

        //pkt *pkt_temp0, *pkt_temp1, *pkt_temp2, *pkt_temp3, *pkt_temp4;
		pkt *pkt_temp0, *pkt_temp1, *pkt_temp2, *pkt_temp3, *pkt_temp4,*pkt_temp5, *pkt_temp6, *pkt_temp7, *pkt_temp8, *pkt_temp9;
        pkt *pkt_temp10, *pkt_temp11, *pkt_temp12, *pkt_temp13, *pkt_temp14,*pkt_temp15, *pkt_temp16, *pkt_temp17, *pkt_temp18, *pkt_temp19;
        pkt *pkt_temp20, *pkt_temp21, *pkt_temp22, *pkt_temp23, *pkt_temp24;
		
        //int countNow = 0;
        //countNow =  Optical_pkt_temp->getPkt1()->getHopCountNow();
     /*   int xGroupToRIndex = pkt_temp->getGroupToRIndex();
        int xTXIndex = pkt_temp->getTxIndex() ;
        int xBufferindex = xTXIndex*NumTx+xGroupToRIndex;*/


        for(int i=0;i<xoptnum1;i++){
            switch(i){
                 case 0:
                     Optical_pkt_temp->removeMessage(Optical_pkt_temp->getPkt1());
                     pkt_temp0 =  check_and_cast<pkt*>(Optical_pkt_temp->getPkt1());
                     if(  ( pkt_temp0->getHop(pkt_temp0->getHopCountNow() + 1)  >= AllToRNum ) && ( pkt_temp0->getHop(pkt_temp0->getHopCountNow() + 1 ) <  AllToRNum +  (FOSRadix*(FOSRadix*FOSRadix + 1 ) ) )  ){
                             int Next_ToR = pkt_temp0->getHop(pkt_temp0->getHopCountNow() + 2);
                             int TxIndex_0 = ( (Next_ToR%(FOSRadix*FOSRadix)) % AgreRadix)/GroupToR;
                             GroupToRIndex = ( (Next_ToR%(FOSRadix*FOSRadix)) % AgreRadix)%GroupToR;
                             pkt_temp0->setTxIndex(TxIndex_0);
                             pkt_temp0->setGroupToRIndex(GroupToRIndex);
                             sendDelayed(pkt_temp0,DelayOfProcessor,"bufferout",TxIndex_0);
                             //finishtime_Agre = gate("bufferout",TxIndex_0)->getTransmissionChannel()->getTransmissionFinishTime();
                     }
                     else if(   (pkt_temp0->getHop(pkt_temp0->getHopCountNow() + 1)  >= AllToRNum + (FOSRadix*(FOSRadix*FOSRadix + 1 )) ) && ( pkt_temp0->getHop(pkt_temp0->getHopCountNow() + 1 ) <  AllToRNum + 2* (FOSRadix*(FOSRadix*FOSRadix + 1 )))   ){
                         int Next_ToR = pkt_temp0->getHop(pkt_temp0->getHopCountNow() + 2);
                         int TxIndex_0 =  ( (Next_ToR%(FOSRadix*FOSRadix)) / CoreRadix)/GroupToRCore;       // go to the direct connected ToR
                             pkt_temp0->setTxIndex(TxIndex_0);
                             GroupToRIndex = ( (Next_ToR%(FOSRadix*FOSRadix)) / CoreRadix)%GroupToRCore;
                             pkt_temp0->setGroupToRIndex(GroupToRIndex);
                             sendDelayed(pkt_temp0,DelayOfProcessor,"bufferout",NumTx+TxIndex_0);
                             //finishtime_Core = gate("bufferout",NumTx+TxIndex_0)->getTransmissionChannel()->getTransmissionFinishTime();
                     }
                     else if(  (pkt_temp0->getHop(pkt_temp0->getHopCountNow() + 1)   >= 0  ) && (pkt_temp0->getHop(pkt_temp0->getHopCountNow() + 1)   <  AllToRNum  )   ){
                             int xx = uniform(0,100);
                             int TxIndex_2 = (xx*NumTx_ToR2ToR)/100;       // go to the direct connected ToR
                             pkt_temp0->setTxIndex(TxIndex_2);
                             GroupToRIndex = 0;
                             pkt_temp0->setGroupToRIndex(GroupToRIndex);
                             sendDelayed(pkt_temp0,DelayOfProcessor,"bufferout",NumTx+NumTx_Core+TxIndex_2);
                             //finishtime_Core = gate("bufferout",NumTx+TxIndex_0)->getTransmissionChannel()->getTransmissionFinishTime();
                     }
                     else if(pkt_temp0->getHop(pkt_temp0->getHopCountNow() + 1)  == -100){
                         if(pkt_temp0->getFlag_last() == 1){
                             PnumReceived_ToR++;
                             Networktime_latency = simTime() - pkt_temp0->getTimestamp();
                             PToR2ToR_Latency = PToR2ToR_Latency + Networktime_latency;
                             //PAverageNetworkLatency = ToR2ToR_Latency/PnumReceived_ToR;
                         }
                             numReceived_ToR++;
                             Networktime_latency = simTime() - pkt_temp0->getTimestamp();
                             ToR2ToR_Latency =  ToR2ToR_Latency + Networktime_latency;
                             Networktime_latency = 0;
                             //AverageNetworkLatency = ToR2ToR_Latency/numReceived_ToR;
                             numReceived_Total++;
                         int DestServer = pkt_temp0->getDst_Server();
                         if(Queue2Server_busy[DestServer] == false){
                             Queue2Server_busy[DestServer] = true;
                             sendDelayed(pkt_temp0,DelayOfProcessor,"serverout",DestServer%ToRserverNum);
                             finishtime = gate("serverout",DestServer%ToRserverNum)->getTransmissionChannel()->getTransmissionFinishTime();

                             pkt *pkt2l = new pkt("local_pkt_ToR");
                             pkt2l->setDst_Server(DestServer);
                             pkt2l->setIndex(2);
                             pkt2l->setSchedulingPriority(0);
                             scheduleAt(simTime()+slot_ToR + DelayOfProcessor,pkt2l);
                             pkt_temp0->setFlag_intraToR(1);
                         }
                         else{
                             int temporyOccupy = 0;
                             for(int i = (DestServer/ToRserverNum)*ToRserverNum;i< (DestServer/ToRserverNum)*ToRserverNum + ToRserverNum ;i++){
                                 temporyOccupy +=  Queue2Server[i].length();
                             }
                             if(temporyOccupy >= Down2ServerBufferSize*ToRDownRadix ){
                                 Queue2Serverlost[DestServer] = Queue2Serverlost[DestServer] + 1;
                                 delete pkt_temp0;
                             }
                             else{
                                 pkt_temp0->setPacketSize(64);
                                 Queue2Server[DestServer].insert(pkt_temp0);
                             }
                         }
                     }
                     break;
                 case 1:
                     Optical_pkt_temp->removeMessage(Optical_pkt_temp->getPkt2());
                     pkt_temp1 =  check_and_cast<pkt*>(Optical_pkt_temp->getPkt2());
                     if(  ( pkt_temp1->getHop(pkt_temp1->getHopCountNow() + 1)  >= AllToRNum ) && ( pkt_temp1->getHop(pkt_temp1->getHopCountNow() + 1 ) <  AllToRNum +  (FOSRadix*(FOSRadix*FOSRadix + 1 ) ) )  ){
                             int Next_ToR = pkt_temp1->getHop(pkt_temp1->getHopCountNow() + 2);
                             int TxIndex_0 = ( (Next_ToR%(FOSRadix*FOSRadix)) % AgreRadix)/GroupToR;
                             GroupToRIndex = ( (Next_ToR%(FOSRadix*FOSRadix)) % AgreRadix)%GroupToR;
                             pkt_temp1->setTxIndex(TxIndex_0);
                             pkt_temp1->setGroupToRIndex(GroupToRIndex);
                             sendDelayed(pkt_temp1,DelayOfProcessor,"bufferout",TxIndex_0);
                             //finishtime_Agre = gate("bufferout",TxIndex_0)->getTransmissionChannel()->getTransmissionFinishTime();
                     }
                     else if(   (pkt_temp1->getHop(pkt_temp1->getHopCountNow() + 1)  >= AllToRNum +  (FOSRadix*(FOSRadix*FOSRadix + 1 ) ) ) && ( pkt_temp1->getHop(pkt_temp1->getHopCountNow() + 1 ) <  AllToRNum + 2* (FOSRadix*(FOSRadix*FOSRadix + 1 )))   ){
                         int Next_ToR = pkt_temp1->getHop(pkt_temp1->getHopCountNow() + 2);
                         int TxIndex_0 =  ( (Next_ToR%(FOSRadix*FOSRadix)) / CoreRadix)/GroupToRCore;       // go to the direct connected ToR
                             pkt_temp1->setTxIndex(TxIndex_0);
                             GroupToRIndex = ( (Next_ToR%(FOSRadix*FOSRadix)) / CoreRadix)%GroupToRCore;
                             pkt_temp1->setGroupToRIndex(GroupToRIndex);
                             sendDelayed(pkt_temp1,DelayOfProcessor,"bufferout",NumTx+TxIndex_0);
                             //finishtime_Core = gate("bufferout",NumTx+TxIndex_0)->getTransmissionChannel()->getTransmissionFinishTime();
                     }
                     else if( (pkt_temp1->getHop(pkt_temp1->getHopCountNow() + 1)   >= 0  ) &&  (pkt_temp1->getHop(pkt_temp1->getHopCountNow() + 1)   <  AllToRNum  )   ){
                             int xx = uniform(0,100);
                             int TxIndex_2 = (xx*NumTx_ToR2ToR)/100;       // go to the direct connected ToR
                             pkt_temp1->setTxIndex(TxIndex_2);
                             GroupToRIndex = 0;
                             pkt_temp1->setGroupToRIndex(GroupToRIndex);
                             sendDelayed(pkt_temp1,DelayOfProcessor,"bufferout",NumTx+NumTx_Core+TxIndex_2);
                             //finishtime_Core = gate("bufferout",NumTx+TxIndex_0)->getTransmissionChannel()->getTransmissionFinishTime();
                     }
                     else if(pkt_temp1->getHop(pkt_temp1->getHopCountNow() + 1)  == -100){

                         if(pkt_temp1->getFlag_last() == 1){
                             PnumReceived_ToR++;
                             Networktime_latency = simTime() - pkt_temp1->getTimestamp();
                             PToR2ToR_Latency = PToR2ToR_Latency + Networktime_latency;
                             //PAverageNetworkLatency = ToR2ToR_Latency/PnumReceived_ToR;
                         }
                             numReceived_ToR++;
                             Networktime_latency = simTime() - pkt_temp1->getTimestamp();
                             ToR2ToR_Latency =  ToR2ToR_Latency + Networktime_latency;
                             Networktime_latency = 0;
                             //AverageNetworkLatency = ToR2ToR_Latency/numReceived_ToR;
                             numReceived_Total++;
                         int DestServer = pkt_temp1->getDst_Server();
                         if(Queue2Server_busy[DestServer] == false){
                             Queue2Server_busy[DestServer] = true;
                             sendDelayed(pkt_temp1,DelayOfProcessor,"serverout",DestServer%ToRserverNum);
                             //finishtime = gate("serverout",DestServer%ToRserverNum)->getTransmissionChannel()->getTransmissionFinishTime();

                             pkt *pkt2l1 = new pkt("local_pkt_ToR");
                             pkt2l1->setDst_Server(DestServer);
                             pkt2l1->setIndex(2);
                             pkt2l1->setSchedulingPriority(0);
                             scheduleAt(simTime()+slot_ToR + DelayOfProcessor,pkt2l1);
                             pkt_temp1->setFlag_intraToR(1);
                         }
                         else{
                             int temporyOccupy = 0;
                             for(int i = (DestServer/ToRserverNum)*ToRserverNum;i< (DestServer/ToRserverNum)*ToRserverNum + ToRserverNum ;i++){
                                 temporyOccupy +=  Queue2Server[i].length();
                             }
                             if(temporyOccupy >= Down2ServerBufferSize*ToRDownRadix ){
                                 Queue2Serverlost[DestServer] = Queue2Serverlost[DestServer] + 1;
                                 delete pkt_temp1;
                             }
                             else{

                                 pkt_temp1->setPacketSize(64);

                                 Queue2Server[DestServer].insert(pkt_temp1);

                             }
                         }
                     }
                     break;
                 case 2:
                     Optical_pkt_temp->removeMessage(Optical_pkt_temp->getPkt3());
                     pkt_temp2 =  check_and_cast<pkt*>(Optical_pkt_temp->getPkt3());
                     if(  ( pkt_temp2->getHop(pkt_temp2->getHopCountNow() + 1)  >= AllToRNum ) && ( pkt_temp2->getHop(pkt_temp2->getHopCountNow() + 1 ) < AllToRNum +  (FOSRadix*(FOSRadix*FOSRadix + 1 ) ) )  ){
                             int Next_ToR = pkt_temp2->getHop(pkt_temp2->getHopCountNow() + 2);
                             int TxIndex_0 = ( (Next_ToR%(FOSRadix*FOSRadix)) % AgreRadix)/GroupToR;
                             GroupToRIndex = ( (Next_ToR%(FOSRadix*FOSRadix)) % AgreRadix)%GroupToR;
                             pkt_temp2->setTxIndex(TxIndex_0);
                             pkt_temp2->setGroupToRIndex(GroupToRIndex);
                             sendDelayed(pkt_temp2,DelayOfProcessor,"bufferout",TxIndex_0);
                             //finishtime_Agre = gate("bufferout",TxIndex_0)->getTransmissionChannel()->getTransmissionFinishTime();
                     }
                     else if(   (pkt_temp2->getHop(pkt_temp2->getHopCountNow() + 1)  >= AllToRNum +  (FOSRadix*(FOSRadix*FOSRadix + 1 ) ) ) && ( pkt_temp2->getHop(pkt_temp2->getHopCountNow() + 1 ) <  AllToRNum + 2* (FOSRadix*(FOSRadix*FOSRadix + 1 )))   ){
                         int Next_ToR = pkt_temp2->getHop(pkt_temp2->getHopCountNow() + 2);
                         int TxIndex_0 =  ( (Next_ToR%(FOSRadix*FOSRadix)) / CoreRadix)/GroupToRCore;       // go to the direct connected ToR
                             pkt_temp2->setTxIndex(TxIndex_0);
                             GroupToRIndex = ( (Next_ToR%(FOSRadix*FOSRadix)) / CoreRadix)%GroupToRCore;
                             pkt_temp2->setGroupToRIndex(GroupToRIndex);
                             sendDelayed(pkt_temp2,DelayOfProcessor,"bufferout",NumTx+TxIndex_0);
                             //finishtime_Core = gate("bufferout",NumTx+TxIndex_0)->getTransmissionChannel()->getTransmissionFinishTime();
                     }
                     else if(  (pkt_temp2->getHop(pkt_temp2->getHopCountNow() + 1)   >= 0  ) && (pkt_temp2->getHop(pkt_temp2->getHopCountNow() + 1)   <  AllToRNum  )   ){
                             int xx = uniform(0,100);
                             int TxIndex_2 = (xx*NumTx_ToR2ToR)/100;       // go to the direct connected ToR
                             pkt_temp2->setTxIndex(TxIndex_2);
                             GroupToRIndex = 0;
                             pkt_temp2->setGroupToRIndex(GroupToRIndex);
                             sendDelayed(pkt_temp2,DelayOfProcessor,"bufferout",NumTx+NumTx_Core+TxIndex_2);
                     }
                     else if(pkt_temp2->getHop(pkt_temp2->getHopCountNow() + 1)  == -100){
                         if(pkt_temp2->getFlag_last() == 1){
                             PnumReceived_ToR++;
                             Networktime_latency = simTime() - pkt_temp2->getTimestamp();
                             PToR2ToR_Latency = PToR2ToR_Latency + Networktime_latency;
                             //PAverageNetworkLatency = ToR2ToR_Latency/PnumReceived_ToR;
                         }
                             numReceived_ToR++;
                             Networktime_latency = simTime() - pkt_temp2->getTimestamp();
                             ToR2ToR_Latency =  ToR2ToR_Latency + Networktime_latency;
                             Networktime_latency = 0;
                             //AverageNetworkLatency = ToR2ToR_Latency/numReceived_ToR;
                             numReceived_Total++;
                         int DestServer = pkt_temp2->getDst_Server();
                         if(Queue2Server_busy[DestServer] == false){
                             Queue2Server_busy[DestServer] = true;
                             sendDelayed(pkt_temp2,DelayOfProcessor,"serverout",DestServer%ToRserverNum);
                             finishtime = gate("serverout",DestServer%ToRserverNum)->getTransmissionChannel()->getTransmissionFinishTime();

                             pkt *pkt2l2 = new pkt("local_pkt_ToR");
                             pkt2l2->setDst_Server(DestServer);
                             pkt2l2->setIndex(2);
                             pkt2l2->setSchedulingPriority(0);
                             scheduleAt(simTime()+slot_ToR + DelayOfProcessor,pkt2l2);
                             pkt_temp2->setFlag_intraToR(1);
                         }
                         else{
                             int temporyOccupy = 0;
                             for(int i = (DestServer/ToRserverNum)*ToRserverNum;i< (DestServer/ToRserverNum)*ToRserverNum + ToRserverNum ;i++){
                                 temporyOccupy +=  Queue2Server[i].length();
                             }
                             if(temporyOccupy >= Down2ServerBufferSize*ToRDownRadix ){
                                 Queue2Serverlost[DestServer] = Queue2Serverlost[DestServer] + 1;
                                 delete pkt_temp2;
                             }
                             else{

                                 pkt_temp2->setPacketSize(64);

                                 Queue2Server[DestServer].insert(pkt_temp2);

                             }
                         }
                     }
                     break;
                 case 3:
                     Optical_pkt_temp->removeMessage(Optical_pkt_temp->getPkt4());
                     pkt_temp3 =  check_and_cast<pkt*>(Optical_pkt_temp->getPkt4());
                     if(  ( pkt_temp3->getHop(pkt_temp3->getHopCountNow() + 1)  >= AllToRNum ) && ( pkt_temp3->getHop(pkt_temp3->getHopCountNow() + 1 ) < AllToRNum +  (FOSRadix*(FOSRadix*FOSRadix + 1 ) ) )  ){
                             int Next_ToR = pkt_temp3->getHop(pkt_temp3->getHopCountNow() + 2);
                             int TxIndex_0 = ( (Next_ToR%(FOSRadix*FOSRadix)) % AgreRadix)/GroupToR;
                             GroupToRIndex = ( (Next_ToR%(FOSRadix*FOSRadix)) % AgreRadix)%GroupToR;
                             pkt_temp3->setTxIndex(TxIndex_0);
                             pkt_temp3->setGroupToRIndex(GroupToRIndex);
                             sendDelayed(pkt_temp3,DelayOfProcessor,"bufferout",TxIndex_0);
                             //finishtime_Agre = gate("bufferout",TxIndex_0)->getTransmissionChannel()->getTransmissionFinishTime();
                     }
                     else if(   (pkt_temp3->getHop(pkt_temp3->getHopCountNow() + 1)  >= AllToRNum +  (FOSRadix*(FOSRadix*FOSRadix + 1 ) ) ) && ( pkt_temp3->getHop(pkt_temp3->getHopCountNow() + 1 ) <  AllToRNum + 2* (FOSRadix*(FOSRadix*FOSRadix + 1 )))   ){
                         int Next_ToR = pkt_temp3->getHop(pkt_temp3->getHopCountNow() + 2);
                         int TxIndex_0 =  ( (Next_ToR%(FOSRadix*FOSRadix)) / CoreRadix)/GroupToRCore;       // go to the direct connected ToR
                             pkt_temp3->setTxIndex(TxIndex_0);
                             GroupToRIndex = ( (Next_ToR%(FOSRadix*FOSRadix)) / CoreRadix)%GroupToRCore;
                             pkt_temp3->setGroupToRIndex(GroupToRIndex);
                             sendDelayed(pkt_temp3,DelayOfProcessor,"bufferout",NumTx+TxIndex_0);
                             //finishtime_Core = gate("bufferout",NumTx+TxIndex_0)->getTransmissionChannel()->getTransmissionFinishTime();
                     }
                     else if( (pkt_temp3->getHop(pkt_temp3->getHopCountNow() + 1)   >= 0  ) &&   (pkt_temp3->getHop(pkt_temp3->getHopCountNow() + 1)   <  AllToRNum  )   ){
                             int xx = uniform(0,100);
                             int TxIndex_2 = (xx*NumTx_ToR2ToR)/100;       // go to the direct connected ToR
                             pkt_temp3->setTxIndex(TxIndex_2);
                             GroupToRIndex = 0;
                             pkt_temp3->setGroupToRIndex(GroupToRIndex);
                             sendDelayed(pkt_temp3,DelayOfProcessor,"bufferout",NumTx+NumTx_Core+TxIndex_2);
                             //finishtime_Core = gate("bufferout",NumTx+TxIndex_0)->getTransmissionChannel()->getTransmissionFinishTime();
                     }
                     else if(pkt_temp3->getHop(pkt_temp3->getHopCountNow() + 1)  == -100){
                         if(pkt_temp3->getFlag_last() == 1){
                             PnumReceived_ToR++;
                             Networktime_latency = simTime() - pkt_temp3->getTimestamp();
                             PToR2ToR_Latency = PToR2ToR_Latency + Networktime_latency;
                             //PAverageNetworkLatency = ToR2ToR_Latency/PnumReceived_ToR;
                         }
                             numReceived_ToR++;
                             Networktime_latency = simTime() - pkt_temp3->getTimestamp();
                             ToR2ToR_Latency =  ToR2ToR_Latency + Networktime_latency;
                             Networktime_latency = 0;
                             //AverageNetworkLatency = ToR2ToR_Latency/numReceived_ToR;
                             numReceived_Total++;
                         int DestServer = pkt_temp3->getDst_Server();
                         if(Queue2Server_busy[DestServer] == false){
                             Queue2Server_busy[DestServer] = true;
                             sendDelayed(pkt_temp3,DelayOfProcessor,"serverout",DestServer%ToRserverNum);
                             finishtime = gate("serverout",DestServer%ToRserverNum)->getTransmissionChannel()->getTransmissionFinishTime();

                             pkt *pkt2l3 = new pkt("local_pkt_ToR");
                             pkt2l3->setDst_Server(DestServer);
                             pkt2l3->setIndex(2);
                             pkt2l3->setSchedulingPriority(0);
                             scheduleAt(simTime()+slot_ToR + DelayOfProcessor,pkt2l3);
                             pkt_temp3->setFlag_intraToR(1);
                         }
                         else{
                             int temporyOccupy = 0;
                             for(int i = (DestServer/ToRserverNum)*ToRserverNum;i< (DestServer/ToRserverNum)*ToRserverNum + ToRserverNum ;i++){
                                 temporyOccupy +=  Queue2Server[i].length();
                             }
                             if(temporyOccupy >= Down2ServerBufferSize*ToRDownRadix ){
                                 Queue2Serverlost[DestServer] = Queue2Serverlost[DestServer] + 1;
                                 delete pkt_temp3;
                             }
                             else{

                                 pkt_temp3->setPacketSize(64);

                                 Queue2Server[DestServer].insert(pkt_temp3);

                             }
                         }
                     }
                     break;
                 case 4:
                     Optical_pkt_temp->removeMessage(Optical_pkt_temp->getPkt5());
                     pkt_temp4 =  check_and_cast<pkt*>(Optical_pkt_temp->getPkt5());
                     if(  ( pkt_temp4->getHop(pkt_temp4->getHopCountNow() + 1)  >= AllToRNum ) && ( pkt_temp4->getHop(pkt_temp4->getHopCountNow() + 1 ) < AllToRNum +  (FOSRadix*(FOSRadix*FOSRadix + 1 ) ) )  ){
                             int Next_ToR = pkt_temp4->getHop(pkt_temp4->getHopCountNow() + 2);
                             int TxIndex_0 = ( (Next_ToR%(FOSRadix*FOSRadix)) % AgreRadix)/GroupToR;
                             GroupToRIndex = ( (Next_ToR%(FOSRadix*FOSRadix)) % AgreRadix)%GroupToR;
                             pkt_temp4->setTxIndex(TxIndex_0);
                             pkt_temp4->setGroupToRIndex(GroupToRIndex);
                             sendDelayed(pkt_temp4,DelayOfProcessor,"bufferout",TxIndex_0);
                             //finishtime_Agre = gate("bufferout",TxIndex_0)->getTransmissionChannel()->getTransmissionFinishTime();
                     }
                     else if(   (pkt_temp4->getHop(pkt_temp4->getHopCountNow() + 1)  >= AllToRNum +  (FOSRadix*(FOSRadix*FOSRadix + 1 ) ) ) && ( pkt_temp4->getHop(pkt_temp4->getHopCountNow() + 1 ) <  AllToRNum + 2* (FOSRadix*(FOSRadix*FOSRadix + 1 )))   ){
                         int Next_ToR = pkt_temp4->getHop(pkt_temp4->getHopCountNow() + 2);
                         int TxIndex_0 =  ( (Next_ToR%(FOSRadix*FOSRadix)) / CoreRadix)/GroupToRCore;       // go to the direct connected ToR
                             pkt_temp4->setTxIndex(TxIndex_0);
                             GroupToRIndex = ( (Next_ToR%(FOSRadix*FOSRadix)) / CoreRadix)%GroupToRCore;
                             pkt_temp4->setGroupToRIndex(GroupToRIndex);
                             sendDelayed(pkt_temp4,DelayOfProcessor,"bufferout",NumTx+TxIndex_0);
                             //finishtime_Core = gate("bufferout",NumTx+TxIndex_0)->getTransmissionChannel()->getTransmissionFinishTime();
                     }
                     else if( (pkt_temp4->getHop(pkt_temp4->getHopCountNow() + 1)   >= 0  ) &&   (pkt_temp4->getHop(pkt_temp4->getHopCountNow() + 1)   <  AllToRNum  )   ){
                             int xx = uniform(0,100);
                             int TxIndex_2 = (xx*NumTx_ToR2ToR)/100;       // go to the direct connected ToR
                             pkt_temp4->setTxIndex(TxIndex_2);
                             GroupToRIndex = 0;
                             pkt_temp4->setGroupToRIndex(GroupToRIndex);
                             sendDelayed(pkt_temp4,DelayOfProcessor,"bufferout",NumTx+NumTx_Core+TxIndex_2);
                             //finishtime_Core = gate("bufferout",NumTx+TxIndex_0)->getTransmissionChannel()->getTransmissionFinishTime();
                     }
                     else if(pkt_temp4->getHop(pkt_temp4->getHopCountNow() + 1)  == -100){
                         if(pkt_temp4->getFlag_last() == 1){
                             PnumReceived_ToR++;
                             Networktime_latency = simTime() - pkt_temp4->getTimestamp();
                             PToR2ToR_Latency = PToR2ToR_Latency + Networktime_latency;
                             //PAverageNetworkLatency = ToR2ToR_Latency/PnumReceived_ToR;
                         }
                             numReceived_ToR++;
                             Networktime_latency = simTime() - pkt_temp4->getTimestamp();
                             ToR2ToR_Latency =  ToR2ToR_Latency + Networktime_latency;
                             Networktime_latency = 0;
                             //AverageNetworkLatency = ToR2ToR_Latency/numReceived_ToR;
                             numReceived_Total++;
                         int DestServer = pkt_temp4->getDst_Server();
                         if(Queue2Server_busy[DestServer] == false){
                             Queue2Server_busy[DestServer] = true;
                             sendDelayed(pkt_temp4,DelayOfProcessor,"serverout",DestServer%ToRserverNum);
                             finishtime = gate("serverout",DestServer%ToRserverNum)->getTransmissionChannel()->getTransmissionFinishTime();

                             pkt *pkt2l4 = new pkt("local_pkt_ToR");
                             pkt2l4->setDst_Server(DestServer);
                             pkt2l4->setIndex(2);
                             pkt2l4->setSchedulingPriority(0);
                             scheduleAt(simTime()+slot_ToR + DelayOfProcessor,pkt2l4);
                             pkt_temp4->setFlag_intraToR(1);
                         }
                         else{
                             int temporyOccupy = 0;
                             for(int i = (DestServer/ToRserverNum)*ToRserverNum;i< (DestServer/ToRserverNum)*ToRserverNum + ToRserverNum ;i++){
                                 temporyOccupy +=  Queue2Server[i].length();
                             }
                             if(temporyOccupy >= Down2ServerBufferSize*ToRDownRadix ){
                                 Queue2Serverlost[DestServer] = Queue2Serverlost[DestServer] + 1;
                                 delete pkt_temp4;
                             }
                             else{

                                 pkt_temp4->setPacketSize(64);
                                 Queue2Server[DestServer].insert(pkt_temp4);
                             }
                         }
                     }
                     break;
                 case 5:
                     Optical_pkt_temp->removeMessage(Optical_pkt_temp->getPkt6());
                     pkt_temp5 =  check_and_cast<pkt*>(Optical_pkt_temp->getPkt6());
                     if(  ( pkt_temp5->getHop(pkt_temp5->getHopCountNow() + 1)  >= AllToRNum ) && ( pkt_temp5->getHop(pkt_temp5->getHopCountNow() + 1 ) < AllToRNum +  (FOSRadix*(FOSRadix*FOSRadix + 1 ) ) )  ){
                             int Next_ToR = pkt_temp5->getHop(pkt_temp5->getHopCountNow() + 2);
                             int TxIndex_0 = ( (Next_ToR%(FOSRadix*FOSRadix)) % AgreRadix)/GroupToR;
                             GroupToRIndex = ( (Next_ToR%(FOSRadix*FOSRadix)) % AgreRadix)%GroupToR;
                             pkt_temp5->setTxIndex(TxIndex_0);
                             pkt_temp5->setGroupToRIndex(GroupToRIndex);
                             sendDelayed(pkt_temp5,DelayOfProcessor,"bufferout",TxIndex_0);
                             //finishtime_Agre = gate("bufferout",TxIndex_0)->getTransmissionChannel()->getTransmissionFinishTime();
                     }
                     else if(   (pkt_temp5->getHop(pkt_temp5->getHopCountNow() + 1)  >= AllToRNum +  (FOSRadix*(FOSRadix*FOSRadix + 1 ) ) ) && ( pkt_temp5->getHop(pkt_temp5->getHopCountNow() + 1 ) <  AllToRNum + 2* (FOSRadix*(FOSRadix*FOSRadix + 1 )))   ){
                         int Next_ToR = pkt_temp5->getHop(pkt_temp5->getHopCountNow() + 2);
                         int TxIndex_0 =  ( (Next_ToR%(FOSRadix*FOSRadix)) / CoreRadix)/GroupToRCore;       // go to the direct connected ToR
                             pkt_temp5->setTxIndex(TxIndex_0);
                             GroupToRIndex = ( (Next_ToR%(FOSRadix*FOSRadix)) / CoreRadix)%GroupToRCore;
                             pkt_temp5->setGroupToRIndex(GroupToRIndex);
                             sendDelayed(pkt_temp5,DelayOfProcessor,"bufferout",NumTx+TxIndex_0);
                             //finishtime_Core = gate("bufferout",NumTx+TxIndex_0)->getTransmissionChannel()->getTransmissionFinishTime();
                     }
                     else if( (pkt_temp5->getHop(pkt_temp5->getHopCountNow() + 1)   >= 0  ) &&   (pkt_temp5->getHop(pkt_temp5->getHopCountNow() + 1)   <  AllToRNum  )   ){
                             int xx = uniform(0,100);
                             int TxIndex_2 = (xx*NumTx_ToR2ToR)/100;       // go to the direct connected ToR
                             pkt_temp5->setTxIndex(TxIndex_2);
                             GroupToRIndex = 0;
                             pkt_temp5->setGroupToRIndex(GroupToRIndex);
                             sendDelayed(pkt_temp5,DelayOfProcessor,"bufferout",NumTx+NumTx_Core+TxIndex_2);
                             //finishtime_Core = gate("bufferout",NumTx+TxIndex_0)->getTransmissionChannel()->getTransmissionFinishTime();
                     }
                     else if(pkt_temp5->getHop(pkt_temp5->getHopCountNow() + 1)  == -100){
                         if(pkt_temp5->getFlag_last() == 1){
                             PnumReceived_ToR++;
                             Networktime_latency = simTime() - pkt_temp5->getTimestamp();
                             PToR2ToR_Latency = PToR2ToR_Latency + Networktime_latency;
                             //PAverageNetworkLatency = ToR2ToR_Latency/PnumReceived_ToR;
                         }
                             numReceived_ToR++;
                             Networktime_latency = simTime() - pkt_temp5->getTimestamp();
                             ToR2ToR_Latency =  ToR2ToR_Latency + Networktime_latency;
                             Networktime_latency = 0;
                             //AverageNetworkLatency = ToR2ToR_Latency/numReceived_ToR;
                             numReceived_Total++;
                         int DestServer = pkt_temp5->getDst_Server();
                         if(Queue2Server_busy[DestServer] == false){
                             Queue2Server_busy[DestServer] = true;
                             sendDelayed(pkt_temp5,DelayOfProcessor,"serverout",DestServer%ToRserverNum);
                             finishtime = gate("serverout",DestServer%ToRserverNum)->getTransmissionChannel()->getTransmissionFinishTime();

                             pkt *pkt2l5 = new pkt("local_pkt_ToR");
                             pkt2l5->setDst_Server(DestServer);
                             pkt2l5->setIndex(2);
                             pkt2l5->setSchedulingPriority(0);
                             scheduleAt(simTime()+slot_ToR + DelayOfProcessor,pkt2l5);
                             pkt_temp5->setFlag_intraToR(1);
                         }
                         else{
                             int temporyOccupy = 0;
                             for(int i = (DestServer/ToRserverNum)*ToRserverNum;i< (DestServer/ToRserverNum)*ToRserverNum + ToRserverNum ;i++){
                                 temporyOccupy +=  Queue2Server[i].length();
                             }
                             if(temporyOccupy >= Down2ServerBufferSize*ToRDownRadix ){
                                 Queue2Serverlost[DestServer] = Queue2Serverlost[DestServer] + 1;
                                 delete pkt_temp5;
                             }
                             else{

                                 pkt_temp5->setPacketSize(64);
                                 Queue2Server[DestServer].insert(pkt_temp5);
                             }
                         }
                     }
                     break;
                 case 6:
                     Optical_pkt_temp->removeMessage(Optical_pkt_temp->getPkt7());
                     pkt_temp6 =  check_and_cast<pkt*>(Optical_pkt_temp->getPkt7());
                     if(  ( pkt_temp6->getHop(pkt_temp6->getHopCountNow() + 1)  >= AllToRNum ) && ( pkt_temp6->getHop(pkt_temp6->getHopCountNow() + 1 ) < AllToRNum +  (FOSRadix*(FOSRadix*FOSRadix + 1 ) ) )  ){
                             int Next_ToR = pkt_temp6->getHop(pkt_temp6->getHopCountNow() + 2);
                             int TxIndex_0 = ( (Next_ToR%(FOSRadix*FOSRadix)) % AgreRadix)/GroupToR;
                             GroupToRIndex = ( (Next_ToR%(FOSRadix*FOSRadix)) % AgreRadix)%GroupToR;
                             pkt_temp6->setTxIndex(TxIndex_0);
                             pkt_temp6->setGroupToRIndex(GroupToRIndex);
                             sendDelayed(pkt_temp6,DelayOfProcessor,"bufferout",TxIndex_0);
                             //finishtime_Agre = gate("bufferout",TxIndex_0)->getTransmissionChannel()->getTransmissionFinishTime();
                     }
                     else if(   (pkt_temp6->getHop(pkt_temp6->getHopCountNow() + 1)  >= AllToRNum +  (FOSRadix*(FOSRadix*FOSRadix + 1 ) ) ) && ( pkt_temp6->getHop(pkt_temp6->getHopCountNow() + 1 ) <  AllToRNum + 2* (FOSRadix*(FOSRadix*FOSRadix + 1 )))   ){
                         int Next_ToR = pkt_temp6->getHop(pkt_temp6->getHopCountNow() + 2);
                         int TxIndex_0 =  ( (Next_ToR%(FOSRadix*FOSRadix)) / CoreRadix)/GroupToRCore;       // go to the direct connected ToR
                             pkt_temp6->setTxIndex(TxIndex_0);
                             GroupToRIndex = ( (Next_ToR%(FOSRadix*FOSRadix)) / CoreRadix)%GroupToRCore;
                             pkt_temp6->setGroupToRIndex(GroupToRIndex);
                             sendDelayed(pkt_temp6,DelayOfProcessor,"bufferout",NumTx+TxIndex_0);
                             //finishtime_Core = gate("bufferout",NumTx+TxIndex_0)->getTransmissionChannel()->getTransmissionFinishTime();
                     }
                     else if( (pkt_temp6->getHop(pkt_temp6->getHopCountNow() + 1)   >= 0  ) &&   (pkt_temp6->getHop(pkt_temp6->getHopCountNow() + 1)   <  AllToRNum  )   ){
                             int xx = uniform(0,100);
                             int TxIndex_2 = (xx*NumTx_ToR2ToR)/100;       // go to the direct connected ToR
                             pkt_temp6->setTxIndex(TxIndex_2);
                             GroupToRIndex = 0;
                             pkt_temp6->setGroupToRIndex(GroupToRIndex);
                             sendDelayed(pkt_temp6,DelayOfProcessor,"bufferout",NumTx+NumTx_Core+TxIndex_2);
                             //finishtime_Core = gate("bufferout",NumTx+TxIndex_0)->getTransmissionChannel()->getTransmissionFinishTime();
                     }
                     else if(pkt_temp6->getHop(pkt_temp6->getHopCountNow() + 1)  == -100){
                         if(pkt_temp6->getFlag_last() == 1){
                             PnumReceived_ToR++;
                             Networktime_latency = simTime() - pkt_temp6->getTimestamp();
                             PToR2ToR_Latency = PToR2ToR_Latency + Networktime_latency;
                             //PAverageNetworkLatency = ToR2ToR_Latency/PnumReceived_ToR;
                         }
                             numReceived_ToR++;
                             Networktime_latency = simTime() - pkt_temp6->getTimestamp();
                             ToR2ToR_Latency =  ToR2ToR_Latency + Networktime_latency;
                             Networktime_latency = 0;
                             //AverageNetworkLatency = ToR2ToR_Latency/numReceived_ToR;
                             numReceived_Total++;
                         int DestServer = pkt_temp6->getDst_Server();
                         if(Queue2Server_busy[DestServer] == false){
                             Queue2Server_busy[DestServer] = true;
                             sendDelayed(pkt_temp6,DelayOfProcessor,"serverout",DestServer%ToRserverNum);
                             finishtime = gate("serverout",DestServer%ToRserverNum)->getTransmissionChannel()->getTransmissionFinishTime();

                             pkt *pkt2l6 = new pkt("local_pkt_ToR");
                             pkt2l6->setDst_Server(DestServer);
                             pkt2l6->setIndex(2);
                             pkt2l6->setSchedulingPriority(0);
                             scheduleAt(simTime()+slot_ToR + DelayOfProcessor,pkt2l6);
                             pkt_temp6->setFlag_intraToR(1);
                         }
                         else{
                             int temporyOccupy = 0;
                             for(int i = (DestServer/ToRserverNum)*ToRserverNum;i< (DestServer/ToRserverNum)*ToRserverNum + ToRserverNum ;i++){
                                 temporyOccupy +=  Queue2Server[i].length();
                             }
                             if(temporyOccupy >= Down2ServerBufferSize*ToRDownRadix ){
                                 Queue2Serverlost[DestServer] = Queue2Serverlost[DestServer] + 1;
                                 delete pkt_temp6;
                             }
                             else{

                                 pkt_temp6->setPacketSize(64);
                                 Queue2Server[DestServer].insert(pkt_temp6);
                             }
                         }
                     }
                     break;
                 case 7:
                     Optical_pkt_temp->removeMessage(Optical_pkt_temp->getPkt8());
                     pkt_temp7 =  check_and_cast<pkt*>(Optical_pkt_temp->getPkt8());
                     if(  ( pkt_temp7->getHop(pkt_temp7->getHopCountNow() + 1)  >= AllToRNum ) && ( pkt_temp7->getHop(pkt_temp7->getHopCountNow() + 1 ) < AllToRNum +  (FOSRadix*(FOSRadix*FOSRadix + 1 ) ) )  ){
                             int Next_ToR = pkt_temp7->getHop(pkt_temp7->getHopCountNow() + 2);
                             int TxIndex_0 = ( (Next_ToR%(FOSRadix*FOSRadix)) % AgreRadix)/GroupToR;
                             GroupToRIndex = ( (Next_ToR%(FOSRadix*FOSRadix)) % AgreRadix)%GroupToR;
                             pkt_temp7->setTxIndex(TxIndex_0);
                             pkt_temp7->setGroupToRIndex(GroupToRIndex);
                             sendDelayed(pkt_temp7,DelayOfProcessor,"bufferout",TxIndex_0);
                             //finishtime_Agre = gate("bufferout",TxIndex_0)->getTransmissionChannel()->getTransmissionFinishTime();
                     }
                     else if(   (pkt_temp7->getHop(pkt_temp7->getHopCountNow() + 1)  >= AllToRNum +  (FOSRadix*(FOSRadix*FOSRadix + 1 ) ) ) && ( pkt_temp7->getHop(pkt_temp7->getHopCountNow() + 1 ) <  AllToRNum + 2* (FOSRadix*(FOSRadix*FOSRadix + 1 )))   ){
                         int Next_ToR = pkt_temp7->getHop(pkt_temp7->getHopCountNow() + 2);
                         int TxIndex_0 =  ( (Next_ToR%(FOSRadix*FOSRadix)) / CoreRadix)/GroupToRCore;       // go to the direct connected ToR
                             pkt_temp7->setTxIndex(TxIndex_0);
                             GroupToRIndex = ( (Next_ToR%(FOSRadix*FOSRadix)) / CoreRadix)%GroupToRCore;
                             pkt_temp7->setGroupToRIndex(GroupToRIndex);
                             sendDelayed(pkt_temp7,DelayOfProcessor,"bufferout",NumTx+TxIndex_0);
                             //finishtime_Core = gate("bufferout",NumTx+TxIndex_0)->getTransmissionChannel()->getTransmissionFinishTime();
                     }
                     else if( (pkt_temp7->getHop(pkt_temp7->getHopCountNow() + 1)   >= 0  ) &&   (pkt_temp7->getHop(pkt_temp7->getHopCountNow() + 1)   <  AllToRNum  )   ){
                             int xx = uniform(0,100);
                             int TxIndex_2 = (xx*NumTx_ToR2ToR)/100;       // go to the direct connected ToR
                             pkt_temp7->setTxIndex(TxIndex_2);
                             GroupToRIndex = 0;
                             pkt_temp7->setGroupToRIndex(GroupToRIndex);
                             sendDelayed(pkt_temp7,DelayOfProcessor,"bufferout",NumTx+NumTx_Core+TxIndex_2);
                             //finishtime_Core = gate("bufferout",NumTx+TxIndex_0)->getTransmissionChannel()->getTransmissionFinishTime();
                     }
                     else if(pkt_temp7->getHop(pkt_temp7->getHopCountNow() + 1)  == -100){
                         if(pkt_temp7->getFlag_last() == 1){
                             PnumReceived_ToR++;
                             Networktime_latency = simTime() - pkt_temp7->getTimestamp();
                             PToR2ToR_Latency = PToR2ToR_Latency + Networktime_latency;
                             //PAverageNetworkLatency = ToR2ToR_Latency/PnumReceived_ToR;
                         }
                             numReceived_ToR++;
                             Networktime_latency = simTime() - pkt_temp7->getTimestamp();
                             ToR2ToR_Latency =  ToR2ToR_Latency + Networktime_latency;
                             Networktime_latency = 0;
                             //AverageNetworkLatency = ToR2ToR_Latency/numReceived_ToR;
                             numReceived_Total++;
                         int DestServer = pkt_temp7->getDst_Server();
                         if(Queue2Server_busy[DestServer] == false){
                             Queue2Server_busy[DestServer] = true;
                             sendDelayed(pkt_temp7,DelayOfProcessor,"serverout",DestServer%ToRserverNum);
                             finishtime = gate("serverout",DestServer%ToRserverNum)->getTransmissionChannel()->getTransmissionFinishTime();

                             pkt *pkt2l7 = new pkt("local_pkt_ToR");
                             pkt2l7->setDst_Server(DestServer);
                             pkt2l7->setIndex(2);
                             pkt2l7->setSchedulingPriority(0);
                             scheduleAt(simTime()+slot_ToR + DelayOfProcessor,pkt2l7);
                             pkt_temp7->setFlag_intraToR(1);
                         }
                         else{
                             int temporyOccupy = 0;
                             for(int i = (DestServer/ToRserverNum)*ToRserverNum;i< (DestServer/ToRserverNum)*ToRserverNum + ToRserverNum ;i++){
                                 temporyOccupy +=  Queue2Server[i].length();
                             }
                             if(temporyOccupy >= Down2ServerBufferSize*ToRDownRadix ){
                                 Queue2Serverlost[DestServer] = Queue2Serverlost[DestServer] + 1;
                                 delete pkt_temp7;
                             }
                             else{

                                 pkt_temp7->setPacketSize(64);
                                 Queue2Server[DestServer].insert(pkt_temp7);
                             }
                         }
                     }
                     break;
                 case 8:
                     Optical_pkt_temp->removeMessage(Optical_pkt_temp->getPkt9());
                     pkt_temp8 =  check_and_cast<pkt*>(Optical_pkt_temp->getPkt9());
                     if(  ( pkt_temp8->getHop(pkt_temp8->getHopCountNow() + 1)  >= AllToRNum ) && ( pkt_temp8->getHop(pkt_temp8->getHopCountNow() + 1 ) < AllToRNum +  (FOSRadix*(FOSRadix*FOSRadix + 1 ) ) )  ){
                             int Next_ToR = pkt_temp8->getHop(pkt_temp8->getHopCountNow() + 2);
                             int TxIndex_0 = ( (Next_ToR%(FOSRadix*FOSRadix)) % AgreRadix)/GroupToR;
                             GroupToRIndex = ( (Next_ToR%(FOSRadix*FOSRadix)) % AgreRadix)%GroupToR;
                             pkt_temp8->setTxIndex(TxIndex_0);
                             pkt_temp8->setGroupToRIndex(GroupToRIndex);
                             sendDelayed(pkt_temp8,DelayOfProcessor,"bufferout",TxIndex_0);
                             //finishtime_Agre = gate("bufferout",TxIndex_0)->getTransmissionChannel()->getTransmissionFinishTime();
                     }
                     else if(   (pkt_temp8->getHop(pkt_temp8->getHopCountNow() + 1)  >= AllToRNum +  (FOSRadix*(FOSRadix*FOSRadix + 1 ) ) ) && ( pkt_temp8->getHop(pkt_temp8->getHopCountNow() + 1 ) <  AllToRNum + 2* (FOSRadix*(FOSRadix*FOSRadix + 1 )))   ){
                         int Next_ToR = pkt_temp8->getHop(pkt_temp8->getHopCountNow() + 2);
                         int TxIndex_0 =  ( (Next_ToR%(FOSRadix*FOSRadix)) / CoreRadix)/GroupToRCore;       // go to the direct connected ToR
                             pkt_temp8->setTxIndex(TxIndex_0);
                             GroupToRIndex = ( (Next_ToR%(FOSRadix*FOSRadix)) / CoreRadix)%GroupToRCore;
                             pkt_temp8->setGroupToRIndex(GroupToRIndex);
                             sendDelayed(pkt_temp8,DelayOfProcessor,"bufferout",NumTx+TxIndex_0);
                             //finishtime_Core = gate("bufferout",NumTx+TxIndex_0)->getTransmissionChannel()->getTransmissionFinishTime();
                     }
                     else if( (pkt_temp8->getHop(pkt_temp8->getHopCountNow() + 1)   >= 0  ) &&   (pkt_temp8->getHop(pkt_temp8->getHopCountNow() + 1)   <  AllToRNum  )   ){
                             int xx = uniform(0,100);
                             int TxIndex_2 = (xx*NumTx_ToR2ToR)/100;       // go to the direct connected ToR
                             pkt_temp8->setTxIndex(TxIndex_2);
                             GroupToRIndex = 0;
                             pkt_temp8->setGroupToRIndex(GroupToRIndex);
                             sendDelayed(pkt_temp8,DelayOfProcessor,"bufferout",NumTx+NumTx_Core+TxIndex_2);
                             //finishtime_Core = gate("bufferout",NumTx+TxIndex_0)->getTransmissionChannel()->getTransmissionFinishTime();
                     }
                     else if(pkt_temp8->getHop(pkt_temp8->getHopCountNow() + 1)  == -100){
                         if(pkt_temp8->getFlag_last() == 1){
                             PnumReceived_ToR++;
                             Networktime_latency = simTime() - pkt_temp8->getTimestamp();
                             PToR2ToR_Latency = PToR2ToR_Latency + Networktime_latency;
                             //PAverageNetworkLatency = ToR2ToR_Latency/PnumReceived_ToR;
                         }
                             numReceived_ToR++;
                             Networktime_latency = simTime() - pkt_temp8->getTimestamp();
                             ToR2ToR_Latency =  ToR2ToR_Latency + Networktime_latency;
                             Networktime_latency = 0;
                             //AverageNetworkLatency = ToR2ToR_Latency/numReceived_ToR;
                             numReceived_Total++;
                         int DestServer = pkt_temp8->getDst_Server();
                         if(Queue2Server_busy[DestServer] == false){
                             Queue2Server_busy[DestServer] = true;
                             sendDelayed(pkt_temp8,DelayOfProcessor,"serverout",DestServer%ToRserverNum);
                             finishtime = gate("serverout",DestServer%ToRserverNum)->getTransmissionChannel()->getTransmissionFinishTime();

                             pkt *pkt2l8 = new pkt("local_pkt_ToR");
                             pkt2l8->setDst_Server(DestServer);
                             pkt2l8->setIndex(2);
                             pkt2l8->setSchedulingPriority(0);
                             scheduleAt(simTime()+slot_ToR + DelayOfProcessor,pkt2l8);
                             pkt_temp8->setFlag_intraToR(1);
                         }
                         else{
                             int temporyOccupy = 0;
                             for(int i = (DestServer/ToRserverNum)*ToRserverNum;i< (DestServer/ToRserverNum)*ToRserverNum + ToRserverNum ;i++){
                                 temporyOccupy +=  Queue2Server[i].length();
                             }
                             if(temporyOccupy >= Down2ServerBufferSize*ToRDownRadix ){
                                 Queue2Serverlost[DestServer] = Queue2Serverlost[DestServer] + 1;
                                 delete pkt_temp8;
                             }
                             else{

                                 pkt_temp8->setPacketSize(64);
                                 Queue2Server[DestServer].insert(pkt_temp8);
                             }
                         }
                     }
                     break;					 
                 case 9:
                     Optical_pkt_temp->removeMessage(Optical_pkt_temp->getPkt10());
                     pkt_temp9 =  check_and_cast<pkt*>(Optical_pkt_temp->getPkt10());
                     if(  ( pkt_temp9->getHop(pkt_temp9->getHopCountNow() + 1)  >= AllToRNum ) && ( pkt_temp9->getHop(pkt_temp9->getHopCountNow() + 1 ) < AllToRNum +  (FOSRadix*(FOSRadix*FOSRadix + 1 ) ) )  ){
                             int Next_ToR = pkt_temp9->getHop(pkt_temp9->getHopCountNow() + 2);
                             int TxIndex_0 = ( (Next_ToR%(FOSRadix*FOSRadix)) % AgreRadix)/GroupToR;
                             GroupToRIndex = ( (Next_ToR%(FOSRadix*FOSRadix)) % AgreRadix)%GroupToR;
                             pkt_temp9->setTxIndex(TxIndex_0);
                             pkt_temp9->setGroupToRIndex(GroupToRIndex);
                             sendDelayed(pkt_temp9,DelayOfProcessor,"bufferout",TxIndex_0);
                             //finishtime_Agre = gate("bufferout",TxIndex_0)->getTransmissionChannel()->getTransmissionFinishTime();
                     }
                     else if(   (pkt_temp9->getHop(pkt_temp9->getHopCountNow() + 1)  >= AllToRNum +  (FOSRadix*(FOSRadix*FOSRadix + 1 ) ) ) && ( pkt_temp9->getHop(pkt_temp9->getHopCountNow() + 1 ) <  AllToRNum + 2* (FOSRadix*(FOSRadix*FOSRadix + 1 )))   ){
                         int Next_ToR = pkt_temp9->getHop(pkt_temp9->getHopCountNow() + 2);
                         int TxIndex_0 =  ( (Next_ToR%(FOSRadix*FOSRadix)) / CoreRadix)/GroupToRCore;       // go to the direct connected ToR
                             pkt_temp9->setTxIndex(TxIndex_0);
                             GroupToRIndex = ( (Next_ToR%(FOSRadix*FOSRadix)) / CoreRadix)%GroupToRCore;
                             pkt_temp9->setGroupToRIndex(GroupToRIndex);
                             sendDelayed(pkt_temp9,DelayOfProcessor,"bufferout",NumTx+TxIndex_0);
                             //finishtime_Core = gate("bufferout",NumTx+TxIndex_0)->getTransmissionChannel()->getTransmissionFinishTime();
                     }
                     else if( (pkt_temp9->getHop(pkt_temp9->getHopCountNow() + 1)   >= 0  ) &&   (pkt_temp9->getHop(pkt_temp9->getHopCountNow() + 1)   <  AllToRNum  )   ){
                             int xx = uniform(0,100);
                             int TxIndex_2 = (xx*NumTx_ToR2ToR)/100;       // go to the direct connected ToR
                             pkt_temp9->setTxIndex(TxIndex_2);
                             GroupToRIndex = 0;
                             pkt_temp9->setGroupToRIndex(GroupToRIndex);
                             sendDelayed(pkt_temp9,DelayOfProcessor,"bufferout",NumTx+NumTx_Core+TxIndex_2);
                             //finishtime_Core = gate("bufferout",NumTx+TxIndex_0)->getTransmissionChannel()->getTransmissionFinishTime();
                     }
                     else if(pkt_temp9->getHop(pkt_temp9->getHopCountNow() + 1)  == -100){
                         if(pkt_temp9->getFlag_last() == 1){
                             PnumReceived_ToR++;
                             Networktime_latency = simTime() - pkt_temp9->getTimestamp();
                             PToR2ToR_Latency = PToR2ToR_Latency + Networktime_latency;
                             //PAverageNetworkLatency = ToR2ToR_Latency/PnumReceived_ToR;
                         }
                             numReceived_ToR++;
                             Networktime_latency = simTime() - pkt_temp9->getTimestamp();
                             ToR2ToR_Latency =  ToR2ToR_Latency + Networktime_latency;
                             Networktime_latency = 0;
                             //AverageNetworkLatency = ToR2ToR_Latency/numReceived_ToR;
                             numReceived_Total++;
                         int DestServer = pkt_temp9->getDst_Server();
                         if(Queue2Server_busy[DestServer] == false){
                             Queue2Server_busy[DestServer] = true;
                             sendDelayed(pkt_temp9,DelayOfProcessor,"serverout",DestServer%ToRserverNum);
                             finishtime = gate("serverout",DestServer%ToRserverNum)->getTransmissionChannel()->getTransmissionFinishTime();

                             pkt *pkt2l9 = new pkt("local_pkt_ToR");
                             pkt2l9->setDst_Server(DestServer);
                             pkt2l9->setIndex(2);
                             pkt2l9->setSchedulingPriority(0);
                             scheduleAt(simTime()+slot_ToR + DelayOfProcessor,pkt2l9);
                             pkt_temp9->setFlag_intraToR(1);
                         }
                         else{
                             int temporyOccupy = 0;
                             for(int i = (DestServer/ToRserverNum)*ToRserverNum;i< (DestServer/ToRserverNum)*ToRserverNum + ToRserverNum ;i++){
                                 temporyOccupy +=  Queue2Server[i].length();
                             }
                             if(temporyOccupy >= Down2ServerBufferSize*ToRDownRadix ){
                                 Queue2Serverlost[DestServer] = Queue2Serverlost[DestServer] + 1;
                                 delete pkt_temp9;
                             }
                             else{

                                 pkt_temp9->setPacketSize(64);
                                 Queue2Server[DestServer].insert(pkt_temp9);
                             }
                         }
                     }
                     break;					 
                 case 10:
                     Optical_pkt_temp->removeMessage(Optical_pkt_temp->getPkt11());
                     pkt_temp10 =  check_and_cast<pkt*>(Optical_pkt_temp->getPkt11());
                     if(  ( pkt_temp10->getHop(pkt_temp10->getHopCountNow() + 1)  >= AllToRNum ) && ( pkt_temp10->getHop(pkt_temp10->getHopCountNow() + 1 ) < AllToRNum +  (FOSRadix*(FOSRadix*FOSRadix + 1 ) ) )  ){
                             int Next_ToR = pkt_temp10->getHop(pkt_temp10->getHopCountNow() + 2);
                             int TxIndex_0 = ( (Next_ToR%(FOSRadix*FOSRadix)) % AgreRadix)/GroupToR;
                             GroupToRIndex = ( (Next_ToR%(FOSRadix*FOSRadix)) % AgreRadix)%GroupToR;
                             pkt_temp10->setTxIndex(TxIndex_0);
                             pkt_temp10->setGroupToRIndex(GroupToRIndex);
                             sendDelayed(pkt_temp10,DelayOfProcessor,"bufferout",TxIndex_0);
                             //finishtime_Agre = gate("bufferout",TxIndex_0)->getTransmissionChannel()->getTransmissionFinishTime();
                     }
                     else if(   (pkt_temp10->getHop(pkt_temp10->getHopCountNow() + 1)  >= AllToRNum +  (FOSRadix*(FOSRadix*FOSRadix + 1 ) ) ) && ( pkt_temp10->getHop(pkt_temp10->getHopCountNow() + 1 ) <  AllToRNum + 2* (FOSRadix*(FOSRadix*FOSRadix + 1 )))   ){
                         int Next_ToR = pkt_temp10->getHop(pkt_temp10->getHopCountNow() + 2);
                         int TxIndex_0 =  ( (Next_ToR%(FOSRadix*FOSRadix)) / CoreRadix)/GroupToRCore;       // go to the direct connected ToR
                             pkt_temp10->setTxIndex(TxIndex_0);
                             GroupToRIndex = ( (Next_ToR%(FOSRadix*FOSRadix)) / CoreRadix)%GroupToRCore;
                             pkt_temp10->setGroupToRIndex(GroupToRIndex);
                             sendDelayed(pkt_temp10,DelayOfProcessor,"bufferout",NumTx+TxIndex_0);
                             //finishtime_Core = gate("bufferout",NumTx+TxIndex_0)->getTransmissionChannel()->getTransmissionFinishTime();
                     }
                     else if( (pkt_temp10->getHop(pkt_temp10->getHopCountNow() + 1)   >= 0  ) &&   (pkt_temp10->getHop(pkt_temp10->getHopCountNow() + 1)   <  AllToRNum  )   ){
                             int xx = uniform(0,100);
                             int TxIndex_2 = (xx*NumTx_ToR2ToR)/100;       // go to the direct connected ToR
                             pkt_temp10->setTxIndex(TxIndex_2);
                             GroupToRIndex = 0;
                             pkt_temp10->setGroupToRIndex(GroupToRIndex);
                             sendDelayed(pkt_temp10,DelayOfProcessor,"bufferout",NumTx+NumTx_Core+TxIndex_2);
                             //finishtime_Core = gate("bufferout",NumTx+TxIndex_0)->getTransmissionChannel()->getTransmissionFinishTime();
                     }
                     else if(pkt_temp10->getHop(pkt_temp10->getHopCountNow() + 1)  == -100){
                         if(pkt_temp10->getFlag_last() == 1){
                             PnumReceived_ToR++;
                             Networktime_latency = simTime() - pkt_temp10->getTimestamp();
                             PToR2ToR_Latency = PToR2ToR_Latency + Networktime_latency;
                             //PAverageNetworkLatency = ToR2ToR_Latency/PnumReceived_ToR;
                         }
                             numReceived_ToR++;
                             Networktime_latency = simTime() - pkt_temp10->getTimestamp();
                             ToR2ToR_Latency =  ToR2ToR_Latency + Networktime_latency;
                             Networktime_latency = 0;
                             //AverageNetworkLatency = ToR2ToR_Latency/numReceived_ToR;
                             numReceived_Total++;
                         int DestServer = pkt_temp10->getDst_Server();
                         if(Queue2Server_busy[DestServer] == false){
                             Queue2Server_busy[DestServer] = true;
                             sendDelayed(pkt_temp10,DelayOfProcessor,"serverout",DestServer%ToRserverNum);
                             finishtime = gate("serverout",DestServer%ToRserverNum)->getTransmissionChannel()->getTransmissionFinishTime();

                             pkt *pkt2l10 = new pkt("local_pkt_ToR");
                             pkt2l10->setDst_Server(DestServer);
                             pkt2l10->setIndex(2);
                             pkt2l10->setSchedulingPriority(0);
                             scheduleAt(simTime()+slot_ToR + DelayOfProcessor,pkt2l10);
                             pkt_temp10->setFlag_intraToR(1);
                         }
                         else{
                             int temporyOccupy = 0;
                             for(int i = (DestServer/ToRserverNum)*ToRserverNum;i< (DestServer/ToRserverNum)*ToRserverNum + ToRserverNum ;i++){
                                 temporyOccupy +=  Queue2Server[i].length();
                             }
                             if(temporyOccupy >= Down2ServerBufferSize*ToRDownRadix ){
                                 Queue2Serverlost[DestServer] = Queue2Serverlost[DestServer] + 1;
                                 delete pkt_temp10;
                             }
                             else{

                                 pkt_temp10->setPacketSize(64);
                                 Queue2Server[DestServer].insert(pkt_temp10);
                             }
                         }
                     }
                     break;
                 case 11:
                     Optical_pkt_temp->removeMessage(Optical_pkt_temp->getPkt12());
                     pkt_temp11 =  check_and_cast<pkt*>(Optical_pkt_temp->getPkt12());
                     if(  ( pkt_temp11->getHop(pkt_temp11->getHopCountNow() + 1)  >= AllToRNum ) && ( pkt_temp11->getHop(pkt_temp11->getHopCountNow() + 1 ) < AllToRNum +  (FOSRadix*(FOSRadix*FOSRadix + 1 ) ) )  ){
                             int Next_ToR = pkt_temp11->getHop(pkt_temp11->getHopCountNow() + 2);
                             int TxIndex_0 = ( (Next_ToR%(FOSRadix*FOSRadix)) % AgreRadix)/GroupToR;
                             GroupToRIndex = ( (Next_ToR%(FOSRadix*FOSRadix)) % AgreRadix)%GroupToR;
                             pkt_temp11->setTxIndex(TxIndex_0);
                             pkt_temp11->setGroupToRIndex(GroupToRIndex);
                             sendDelayed(pkt_temp11,DelayOfProcessor,"bufferout",TxIndex_0);
                             //finishtime_Agre = gate("bufferout",TxIndex_0)->getTransmissionChannel()->getTransmissionFinishTime();
                     }
                     else if(   (pkt_temp11->getHop(pkt_temp11->getHopCountNow() + 1)  >= AllToRNum +  (FOSRadix*(FOSRadix*FOSRadix + 1 ) ) ) && ( pkt_temp11->getHop(pkt_temp11->getHopCountNow() + 1 ) <  AllToRNum + 2* (FOSRadix*(FOSRadix*FOSRadix + 1 )))   ){
                         int Next_ToR = pkt_temp11->getHop(pkt_temp11->getHopCountNow() + 2);
                         int TxIndex_0 =  ( (Next_ToR%(FOSRadix*FOSRadix)) / CoreRadix)/GroupToRCore;       // go to the direct connected ToR
                             pkt_temp11->setTxIndex(TxIndex_0);
                             GroupToRIndex = ( (Next_ToR%(FOSRadix*FOSRadix)) / CoreRadix)%GroupToRCore;
                             pkt_temp11->setGroupToRIndex(GroupToRIndex);
                             sendDelayed(pkt_temp11,DelayOfProcessor,"bufferout",NumTx+TxIndex_0);
                             //finishtime_Core = gate("bufferout",NumTx+TxIndex_0)->getTransmissionChannel()->getTransmissionFinishTime();
                     }
                     else if( (pkt_temp11->getHop(pkt_temp11->getHopCountNow() + 1)   >= 0  ) &&   (pkt_temp11->getHop(pkt_temp11->getHopCountNow() + 1)   <  AllToRNum  )   ){
                             int xx = uniform(0,100);
                             int TxIndex_2 = (xx*NumTx_ToR2ToR)/100;       // go to the direct connected ToR
                             pkt_temp11->setTxIndex(TxIndex_2);
                             GroupToRIndex = 0;
                             pkt_temp11->setGroupToRIndex(GroupToRIndex);
                             sendDelayed(pkt_temp11,DelayOfProcessor,"bufferout",NumTx+NumTx_Core+TxIndex_2);
                             //finishtime_Core = gate("bufferout",NumTx+TxIndex_0)->getTransmissionChannel()->getTransmissionFinishTime();
                     }
                     else if(pkt_temp11->getHop(pkt_temp11->getHopCountNow() + 1)  == -100){
                         if(pkt_temp11->getFlag_last() == 1){
                             PnumReceived_ToR++;
                             Networktime_latency = simTime() - pkt_temp11->getTimestamp();
                             PToR2ToR_Latency = PToR2ToR_Latency + Networktime_latency;
                             //PAverageNetworkLatency = ToR2ToR_Latency/PnumReceived_ToR;
                         }
                             numReceived_ToR++;
                             Networktime_latency = simTime() - pkt_temp11->getTimestamp();
                             ToR2ToR_Latency =  ToR2ToR_Latency + Networktime_latency;
                             Networktime_latency = 0;
                             //AverageNetworkLatency = ToR2ToR_Latency/numReceived_ToR;
                             numReceived_Total++;
                         int DestServer = pkt_temp11->getDst_Server();
                         if(Queue2Server_busy[DestServer] == false){
                             Queue2Server_busy[DestServer] = true;
                             sendDelayed(pkt_temp11,DelayOfProcessor,"serverout",DestServer%ToRserverNum);
                             finishtime = gate("serverout",DestServer%ToRserverNum)->getTransmissionChannel()->getTransmissionFinishTime();

                             pkt *pkt2l11 = new pkt("local_pkt_ToR");
                             pkt2l11->setDst_Server(DestServer);
                             pkt2l11->setIndex(2);
                             pkt2l11->setSchedulingPriority(0);
                             scheduleAt(simTime()+slot_ToR + DelayOfProcessor,pkt2l11);
                             pkt_temp11->setFlag_intraToR(1);
                         }
                         else{
                             int temporyOccupy = 0;
                             for(int i = (DestServer/ToRserverNum)*ToRserverNum;i< (DestServer/ToRserverNum)*ToRserverNum + ToRserverNum ;i++){
                                 temporyOccupy +=  Queue2Server[i].length();
                             }
                             if(temporyOccupy >= Down2ServerBufferSize*ToRDownRadix ){
                                 Queue2Serverlost[DestServer] = Queue2Serverlost[DestServer] + 1;
                                 delete pkt_temp11;
                             }
                             else{

                                 pkt_temp11->setPacketSize(64);
                                 Queue2Server[DestServer].insert(pkt_temp11);
                             }
                         }
                     }
                     break;
                 case 12:
                     Optical_pkt_temp->removeMessage(Optical_pkt_temp->getPkt13());
                     pkt_temp12 =  check_and_cast<pkt*>(Optical_pkt_temp->getPkt13());
                     if(  ( pkt_temp12->getHop(pkt_temp12->getHopCountNow() + 1)  >= AllToRNum ) && ( pkt_temp12->getHop(pkt_temp12->getHopCountNow() + 1 ) < AllToRNum +  (FOSRadix*(FOSRadix*FOSRadix + 1 ) ) )  ){
                             int Next_ToR = pkt_temp12->getHop(pkt_temp12->getHopCountNow() + 2);
                             int TxIndex_0 = ( (Next_ToR%(FOSRadix*FOSRadix)) % AgreRadix)/GroupToR;
                             GroupToRIndex = ( (Next_ToR%(FOSRadix*FOSRadix)) % AgreRadix)%GroupToR;
                             pkt_temp12->setTxIndex(TxIndex_0);
                             pkt_temp12->setGroupToRIndex(GroupToRIndex);
                             sendDelayed(pkt_temp12,DelayOfProcessor,"bufferout",TxIndex_0);
                             //finishtime_Agre = gate("bufferout",TxIndex_0)->getTransmissionChannel()->getTransmissionFinishTime();
                     }
                     else if(   (pkt_temp12->getHop(pkt_temp12->getHopCountNow() + 1)  >= AllToRNum +  (FOSRadix*(FOSRadix*FOSRadix + 1 ) ) ) && ( pkt_temp12->getHop(pkt_temp12->getHopCountNow() + 1 ) <  AllToRNum + 2* (FOSRadix*(FOSRadix*FOSRadix + 1 )))   ){
                         int Next_ToR = pkt_temp12->getHop(pkt_temp12->getHopCountNow() + 2);
                         int TxIndex_0 =  ( (Next_ToR%(FOSRadix*FOSRadix)) / CoreRadix)/GroupToRCore;       // go to the direct connected ToR
                             pkt_temp12->setTxIndex(TxIndex_0);
                             GroupToRIndex = ( (Next_ToR%(FOSRadix*FOSRadix)) / CoreRadix)%GroupToRCore;
                             pkt_temp12->setGroupToRIndex(GroupToRIndex);
                             sendDelayed(pkt_temp12,DelayOfProcessor,"bufferout",NumTx+TxIndex_0);
                             //finishtime_Core = gate("bufferout",NumTx+TxIndex_0)->getTransmissionChannel()->getTransmissionFinishTime();
                     }
                     else if(  (pkt_temp12->getHop(pkt_temp12->getHopCountNow() + 1)   >= 0  ) &&  (pkt_temp12->getHop(pkt_temp12->getHopCountNow() + 1)   <  AllToRNum  )   ){
                             int xx = uniform(0,100);
                             int TxIndex_2 = (xx*NumTx_ToR2ToR)/100;       // go to the direct connected ToR
                             pkt_temp12->setTxIndex(TxIndex_2);
                             GroupToRIndex = 0;
                             pkt_temp12->setGroupToRIndex(GroupToRIndex);
                             sendDelayed(pkt_temp12,DelayOfProcessor,"bufferout",NumTx+NumTx_Core+TxIndex_2);
                             //finishtime_Core = gate("bufferout",NumTx+TxIndex_0)->getTransmissionChannel()->getTransmissionFinishTime();
                     }
                     else if(pkt_temp12->getHop(pkt_temp12->getHopCountNow() + 1)  == -100){
                         if(pkt_temp12->getFlag_last() == 1){
                             PnumReceived_ToR++;
                             Networktime_latency = simTime() - pkt_temp12->getTimestamp();
                             PToR2ToR_Latency = PToR2ToR_Latency + Networktime_latency;
                             //PAverageNetworkLatency = ToR2ToR_Latency/PnumReceived_ToR;
                         }
                             numReceived_ToR++;
                             Networktime_latency = simTime() - pkt_temp12->getTimestamp();
                             ToR2ToR_Latency =  ToR2ToR_Latency + Networktime_latency;
                             Networktime_latency = 0;
                             //AverageNetworkLatency = ToR2ToR_Latency/numReceived_ToR;
                             numReceived_Total++;
                         int DestServer = pkt_temp12->getDst_Server();
                         if(Queue2Server_busy[DestServer] == false){
                             Queue2Server_busy[DestServer] = true;
                             sendDelayed(pkt_temp12,DelayOfProcessor,"serverout",DestServer%ToRserverNum);
                             finishtime = gate("serverout",DestServer%ToRserverNum)->getTransmissionChannel()->getTransmissionFinishTime();

                             pkt *pkt2l12 = new pkt("local_pkt_ToR");
                             pkt2l12->setDst_Server(DestServer);
                             pkt2l12->setIndex(2);
                             pkt2l12->setSchedulingPriority(0);
                             scheduleAt(simTime()+slot_ToR + DelayOfProcessor,pkt2l12);
                             pkt_temp12->setFlag_intraToR(1);
                         }
                         else{
                             int temporyOccupy = 0;
                             for(int i = (DestServer/ToRserverNum)*ToRserverNum;i< (DestServer/ToRserverNum)*ToRserverNum + ToRserverNum ;i++){
                                 temporyOccupy +=  Queue2Server[i].length();
                             }
                             if(temporyOccupy >= Down2ServerBufferSize*ToRDownRadix ){
                                 Queue2Serverlost[DestServer] = Queue2Serverlost[DestServer] + 1;
                                 delete pkt_temp12;
                             }
                             else{

                                 pkt_temp12->setPacketSize(64);
                                 Queue2Server[DestServer].insert(pkt_temp12);
                             }
                         }
                     }
                     break;
                 case 13:
                     Optical_pkt_temp->removeMessage(Optical_pkt_temp->getPkt14());
                     pkt_temp13 =  check_and_cast<pkt*>(Optical_pkt_temp->getPkt14());
                     if(  ( pkt_temp13->getHop(pkt_temp13->getHopCountNow() + 1)  >= AllToRNum ) && ( pkt_temp13->getHop(pkt_temp13->getHopCountNow() + 1 ) < AllToRNum +  (FOSRadix*(FOSRadix*FOSRadix + 1 ) ) )  ){
                             int Next_ToR = pkt_temp13->getHop(pkt_temp13->getHopCountNow() + 2);
                             int TxIndex_0 = ( (Next_ToR%(FOSRadix*FOSRadix)) % AgreRadix)/GroupToR;
                             GroupToRIndex = ( (Next_ToR%(FOSRadix*FOSRadix)) % AgreRadix)%GroupToR;
                             pkt_temp13->setTxIndex(TxIndex_0);
                             pkt_temp13->setGroupToRIndex(GroupToRIndex);
                             sendDelayed(pkt_temp13,DelayOfProcessor,"bufferout",TxIndex_0);
                             //finishtime_Agre = gate("bufferout",TxIndex_0)->getTransmissionChannel()->getTransmissionFinishTime();
                     }
                     else if(   (pkt_temp13->getHop(pkt_temp13->getHopCountNow() + 1)  >= AllToRNum +  (FOSRadix*(FOSRadix*FOSRadix + 1 ) ) ) && ( pkt_temp13->getHop(pkt_temp13->getHopCountNow() + 1 ) <  AllToRNum + 2* (FOSRadix*(FOSRadix*FOSRadix + 1 )))   ){
                         int Next_ToR = pkt_temp13->getHop(pkt_temp13->getHopCountNow() + 2);
                         int TxIndex_0 =  ( (Next_ToR%(FOSRadix*FOSRadix)) / CoreRadix)/GroupToRCore;       // go to the direct connected ToR
                             pkt_temp13->setTxIndex(TxIndex_0);
                             GroupToRIndex = ( (Next_ToR%(FOSRadix*FOSRadix)) / CoreRadix)%GroupToRCore;
                             pkt_temp13->setGroupToRIndex(GroupToRIndex);
                             sendDelayed(pkt_temp13,DelayOfProcessor,"bufferout",NumTx+TxIndex_0);
                             //finishtime_Core = gate("bufferout",NumTx+TxIndex_0)->getTransmissionChannel()->getTransmissionFinishTime();
                     }
                     else if( (pkt_temp13->getHop(pkt_temp13->getHopCountNow() + 1)   >= 0  ) &&   (pkt_temp13->getHop(pkt_temp13->getHopCountNow() + 1)   <  AllToRNum  )   ){
                             int xx = uniform(0,100);
                             int TxIndex_2 = (xx*NumTx_ToR2ToR)/100;       // go to the direct connected ToR
                             pkt_temp13->setTxIndex(TxIndex_2);
                             GroupToRIndex = 0;
                             pkt_temp13->setGroupToRIndex(GroupToRIndex);
                             sendDelayed(pkt_temp13,DelayOfProcessor,"bufferout",NumTx+NumTx_Core+TxIndex_2);
                             //finishtime_Core = gate("bufferout",NumTx+TxIndex_0)->getTransmissionChannel()->getTransmissionFinishTime();
                     }
                     else if(pkt_temp13->getHop(pkt_temp13->getHopCountNow() + 1)  == -100){
                         if(pkt_temp13->getFlag_last() == 1){
                             PnumReceived_ToR++;
                             Networktime_latency = simTime() - pkt_temp13->getTimestamp();
                             PToR2ToR_Latency = PToR2ToR_Latency + Networktime_latency;
                             //PAverageNetworkLatency = ToR2ToR_Latency/PnumReceived_ToR;
                         }
                             numReceived_ToR++;
                             Networktime_latency = simTime() - pkt_temp13->getTimestamp();
                             ToR2ToR_Latency =  ToR2ToR_Latency + Networktime_latency;
                             Networktime_latency = 0;
                             //AverageNetworkLatency = ToR2ToR_Latency/numReceived_ToR;
                             numReceived_Total++;
                         int DestServer = pkt_temp13->getDst_Server();
                         if(Queue2Server_busy[DestServer] == false){
                             Queue2Server_busy[DestServer] = true;
                             sendDelayed(pkt_temp13,DelayOfProcessor,"serverout",DestServer%ToRserverNum);
                             finishtime = gate("serverout",DestServer%ToRserverNum)->getTransmissionChannel()->getTransmissionFinishTime();

                             pkt *pkt2l13 = new pkt("local_pkt_ToR");
                             pkt2l13->setDst_Server(DestServer);
                             pkt2l13->setIndex(2);
                             pkt2l13->setSchedulingPriority(0);
                             scheduleAt(simTime()+slot_ToR + DelayOfProcessor,pkt2l13);
                             pkt_temp13->setFlag_intraToR(1);
                         }
                         else{
                             int temporyOccupy = 0;
                             for(int i = (DestServer/ToRserverNum)*ToRserverNum;i< (DestServer/ToRserverNum)*ToRserverNum + ToRserverNum ;i++){
                                 temporyOccupy +=  Queue2Server[i].length();
                             }
                             if(temporyOccupy >= Down2ServerBufferSize*ToRDownRadix ){
                                 Queue2Serverlost[DestServer] = Queue2Serverlost[DestServer] + 1;
                                 delete pkt_temp13;
                             }
                             else{

                                 pkt_temp13->setPacketSize(64);
                                 Queue2Server[DestServer].insert(pkt_temp13);
                             }
                         }
                     }
                     break;					 
                 case 14:
                     Optical_pkt_temp->removeMessage(Optical_pkt_temp->getPkt15());
                     pkt_temp14 =  check_and_cast<pkt*>(Optical_pkt_temp->getPkt15());
                     if(  ( pkt_temp14->getHop(pkt_temp14->getHopCountNow() + 1)  >= AllToRNum ) && ( pkt_temp14->getHop(pkt_temp14->getHopCountNow() + 1 ) < AllToRNum +  (FOSRadix*(FOSRadix*FOSRadix + 1 ) ) )  ){
                             int Next_ToR = pkt_temp14->getHop(pkt_temp14->getHopCountNow() + 2);
                             int TxIndex_0 = ( (Next_ToR%(FOSRadix*FOSRadix)) % AgreRadix)/GroupToR;
                             GroupToRIndex = ( (Next_ToR%(FOSRadix*FOSRadix)) % AgreRadix)%GroupToR;
                             pkt_temp14->setTxIndex(TxIndex_0);
                             pkt_temp14->setGroupToRIndex(GroupToRIndex);
                             sendDelayed(pkt_temp14,DelayOfProcessor,"bufferout",TxIndex_0);
                             //finishtime_Agre = gate("bufferout",TxIndex_0)->getTransmissionChannel()->getTransmissionFinishTime();
                     }
                     else if(   (pkt_temp14->getHop(pkt_temp14->getHopCountNow() + 1)  >= AllToRNum +  (FOSRadix*(FOSRadix*FOSRadix + 1 ) ) ) && ( pkt_temp14->getHop(pkt_temp14->getHopCountNow() + 1 ) <  AllToRNum + 2* (FOSRadix*(FOSRadix*FOSRadix + 1 )))   ){
                         int Next_ToR = pkt_temp14->getHop(pkt_temp14->getHopCountNow() + 2);
                         int TxIndex_0 =  ( (Next_ToR%(FOSRadix*FOSRadix)) / CoreRadix)/GroupToRCore;       // go to the direct connected ToR
                             pkt_temp14->setTxIndex(TxIndex_0);
                             GroupToRIndex = ( (Next_ToR%(FOSRadix*FOSRadix)) / CoreRadix)%GroupToRCore;
                             pkt_temp14->setGroupToRIndex(GroupToRIndex);
                             sendDelayed(pkt_temp14,DelayOfProcessor,"bufferout",NumTx+TxIndex_0);
                             //finishtime_Core = gate("bufferout",NumTx+TxIndex_0)->getTransmissionChannel()->getTransmissionFinishTime();
                     }
                     else if( (pkt_temp14->getHop(pkt_temp14->getHopCountNow() + 1)   >= 0  ) &&   (pkt_temp14->getHop(pkt_temp14->getHopCountNow() + 1)   <  AllToRNum  )   ){
                             int xx = uniform(0,100);
                             int TxIndex_2 = (xx*NumTx_ToR2ToR)/100;       // go to the direct connected ToR
                             pkt_temp14->setTxIndex(TxIndex_2);
                             GroupToRIndex = 0;
                             pkt_temp14->setGroupToRIndex(GroupToRIndex);
                             sendDelayed(pkt_temp14,DelayOfProcessor,"bufferout",NumTx+NumTx_Core+TxIndex_2);
                             //finishtime_Core = gate("bufferout",NumTx+TxIndex_0)->getTransmissionChannel()->getTransmissionFinishTime();
                     }
                     else if(pkt_temp14->getHop(pkt_temp14->getHopCountNow() + 1)  == -100){
                         if(pkt_temp14->getFlag_last() == 1){
                             PnumReceived_ToR++;
                             Networktime_latency = simTime() - pkt_temp14->getTimestamp();
                             PToR2ToR_Latency = PToR2ToR_Latency + Networktime_latency;
                             //PAverageNetworkLatency = ToR2ToR_Latency/PnumReceived_ToR;
                         }
                             numReceived_ToR++;
                             Networktime_latency = simTime() - pkt_temp14->getTimestamp();
                             ToR2ToR_Latency =  ToR2ToR_Latency + Networktime_latency;
                             Networktime_latency = 0;
                             //AverageNetworkLatency = ToR2ToR_Latency/numReceived_ToR;
                             numReceived_Total++;
                         int DestServer = pkt_temp14->getDst_Server();
                         if(Queue2Server_busy[DestServer] == false){
                             Queue2Server_busy[DestServer] = true;
                             sendDelayed(pkt_temp14,DelayOfProcessor,"serverout",DestServer%ToRserverNum);
                             finishtime = gate("serverout",DestServer%ToRserverNum)->getTransmissionChannel()->getTransmissionFinishTime();

                             pkt *pkt2l14 = new pkt("local_pkt_ToR");
                             pkt2l14->setDst_Server(DestServer);
                             pkt2l14->setIndex(2);
                             pkt2l14->setSchedulingPriority(0);
                             scheduleAt(simTime()+slot_ToR + DelayOfProcessor,pkt2l14);
                             pkt_temp14->setFlag_intraToR(1);
                         }
                         else{
                             int temporyOccupy = 0;
                             for(int i = (DestServer/ToRserverNum)*ToRserverNum;i< (DestServer/ToRserverNum)*ToRserverNum + ToRserverNum ;i++){
                                 temporyOccupy +=  Queue2Server[i].length();
                             }
                             if(temporyOccupy >= Down2ServerBufferSize*ToRDownRadix ){
                                 Queue2Serverlost[DestServer] = Queue2Serverlost[DestServer] + 1;
                                 delete pkt_temp14;
                             }
                             else{

                                 pkt_temp14->setPacketSize(64);
                                 Queue2Server[DestServer].insert(pkt_temp14);
                             }
                         }
                     }
                     break;					 
                 case 15:
                     Optical_pkt_temp->removeMessage(Optical_pkt_temp->getPkt16());
                     pkt_temp15 =  check_and_cast<pkt*>(Optical_pkt_temp->getPkt16());
                     if(  ( pkt_temp15->getHop(pkt_temp15->getHopCountNow() + 1)  >= AllToRNum ) && ( pkt_temp15->getHop(pkt_temp15->getHopCountNow() + 1 ) < AllToRNum +  (FOSRadix*(FOSRadix*FOSRadix + 1 ) ) )  ){
                             int Next_ToR = pkt_temp15->getHop(pkt_temp15->getHopCountNow() + 2);
                             int TxIndex_0 = ( (Next_ToR%(FOSRadix*FOSRadix)) % AgreRadix)/GroupToR;
                             GroupToRIndex = ( (Next_ToR%(FOSRadix*FOSRadix)) % AgreRadix)%GroupToR;
                             pkt_temp15->setTxIndex(TxIndex_0);
                             pkt_temp15->setGroupToRIndex(GroupToRIndex);
                             sendDelayed(pkt_temp15,DelayOfProcessor,"bufferout",TxIndex_0);
                             //finishtime_Agre = gate("bufferout",TxIndex_0)->getTransmissionChannel()->getTransmissionFinishTime();
                     }
                     else if(   (pkt_temp15->getHop(pkt_temp15->getHopCountNow() + 1)  >= AllToRNum +  (FOSRadix*(FOSRadix*FOSRadix + 1 ) ) ) && ( pkt_temp15->getHop(pkt_temp15->getHopCountNow() + 1 ) <  AllToRNum + 2* (FOSRadix*(FOSRadix*FOSRadix + 1 )))   ){
                         int Next_ToR = pkt_temp15->getHop(pkt_temp15->getHopCountNow() + 2);
                         int TxIndex_0 =  ( (Next_ToR%(FOSRadix*FOSRadix)) / CoreRadix)/GroupToRCore;       // go to the direct connected ToR
                             pkt_temp15->setTxIndex(TxIndex_0);
                             GroupToRIndex = ( (Next_ToR%(FOSRadix*FOSRadix)) / CoreRadix)%GroupToRCore;
                             pkt_temp15->setGroupToRIndex(GroupToRIndex);
                             sendDelayed(pkt_temp15,DelayOfProcessor,"bufferout",NumTx+TxIndex_0);
                             //finishtime_Core = gate("bufferout",NumTx+TxIndex_0)->getTransmissionChannel()->getTransmissionFinishTime();
                     }
                     else if( (pkt_temp15->getHop(pkt_temp15->getHopCountNow() + 1)   >= 0  ) &&   (pkt_temp15->getHop(pkt_temp15->getHopCountNow() + 1)   <  AllToRNum  )   ){
                             int xx = uniform(0,100);
                             int TxIndex_2 = (xx*NumTx_ToR2ToR)/100;       // go to the direct connected ToR
                             pkt_temp15->setTxIndex(TxIndex_2);
                             GroupToRIndex = 0;
                             pkt_temp15->setGroupToRIndex(GroupToRIndex);
                             sendDelayed(pkt_temp15,DelayOfProcessor,"bufferout",NumTx+NumTx_Core+TxIndex_2);
                             //finishtime_Core = gate("bufferout",NumTx+TxIndex_0)->getTransmissionChannel()->getTransmissionFinishTime();
                     }
                     else if(pkt_temp15->getHop(pkt_temp15->getHopCountNow() + 1)  == -100){
                         if(pkt_temp15->getFlag_last() == 1){
                             PnumReceived_ToR++;
                             Networktime_latency = simTime() - pkt_temp15->getTimestamp();
                             PToR2ToR_Latency = PToR2ToR_Latency + Networktime_latency;
                             //PAverageNetworkLatency = ToR2ToR_Latency/PnumReceived_ToR;
                         }
                             numReceived_ToR++;
                             Networktime_latency = simTime() - pkt_temp15->getTimestamp();
                             ToR2ToR_Latency =  ToR2ToR_Latency + Networktime_latency;
                             Networktime_latency = 0;
                             //AverageNetworkLatency = ToR2ToR_Latency/numReceived_ToR;
                             numReceived_Total++;
                         int DestServer = pkt_temp15->getDst_Server();
                         if(Queue2Server_busy[DestServer] == false){
                             Queue2Server_busy[DestServer] = true;
                             sendDelayed(pkt_temp15,DelayOfProcessor,"serverout",DestServer%ToRserverNum);
                             finishtime = gate("serverout",DestServer%ToRserverNum)->getTransmissionChannel()->getTransmissionFinishTime();

                             pkt *pkt2l15 = new pkt("local_pkt_ToR");
                             pkt2l15->setDst_Server(DestServer);
                             pkt2l15->setIndex(2);
                             pkt2l15->setSchedulingPriority(0);
                             scheduleAt(simTime()+slot_ToR + DelayOfProcessor,pkt2l15);
                             pkt_temp15->setFlag_intraToR(1);
                         }
                         else{
                             int temporyOccupy = 0;
                             for(int i = (DestServer/ToRserverNum)*ToRserverNum;i< (DestServer/ToRserverNum)*ToRserverNum + ToRserverNum ;i++){
                                 temporyOccupy +=  Queue2Server[i].length();
                             }
                             if(temporyOccupy >= Down2ServerBufferSize*ToRDownRadix ){
                                 Queue2Serverlost[DestServer] = Queue2Serverlost[DestServer] + 1;
                                 delete pkt_temp15;
                             }
                             else{

                                 pkt_temp15->setPacketSize(64);
                                 Queue2Server[DestServer].insert(pkt_temp15);
                             }
                         }
                     }
                     break;					 
                 case 16:
                     Optical_pkt_temp->removeMessage(Optical_pkt_temp->getPkt17());
                     pkt_temp16 =  check_and_cast<pkt*>(Optical_pkt_temp->getPkt17());
                     if(  ( pkt_temp16->getHop(pkt_temp16->getHopCountNow() + 1)  >= AllToRNum ) && ( pkt_temp16->getHop(pkt_temp16->getHopCountNow() + 1 ) < AllToRNum +  (FOSRadix*(FOSRadix*FOSRadix + 1 ) ) )  ){
                             int Next_ToR = pkt_temp16->getHop(pkt_temp16->getHopCountNow() + 2);
                             int TxIndex_0 = ( (Next_ToR%(FOSRadix*FOSRadix)) % AgreRadix)/GroupToR;
                             GroupToRIndex = ( (Next_ToR%(FOSRadix*FOSRadix)) % AgreRadix)%GroupToR;
                             pkt_temp16->setTxIndex(TxIndex_0);
                             pkt_temp16->setGroupToRIndex(GroupToRIndex);
                             sendDelayed(pkt_temp16,DelayOfProcessor,"bufferout",TxIndex_0);
                             //finishtime_Agre = gate("bufferout",TxIndex_0)->getTransmissionChannel()->getTransmissionFinishTime();
                     }
                     else if(   (pkt_temp16->getHop(pkt_temp16->getHopCountNow() + 1)  >= AllToRNum +  (FOSRadix*(FOSRadix*FOSRadix + 1 ) ) ) && ( pkt_temp16->getHop(pkt_temp16->getHopCountNow() + 1 ) <  AllToRNum + 2* (FOSRadix*(FOSRadix*FOSRadix + 1 )))   ){
                         int Next_ToR = pkt_temp16->getHop(pkt_temp16->getHopCountNow() + 2);
                         int TxIndex_0 =  ( (Next_ToR%(FOSRadix*FOSRadix)) / CoreRadix)/GroupToRCore;       // go to the direct connected ToR
                             pkt_temp16->setTxIndex(TxIndex_0);
                             GroupToRIndex = ( (Next_ToR%(FOSRadix*FOSRadix)) / CoreRadix)%GroupToRCore;
                             pkt_temp16->setGroupToRIndex(GroupToRIndex);
                             sendDelayed(pkt_temp16,DelayOfProcessor,"bufferout",NumTx+TxIndex_0);
                             //finishtime_Core = gate("bufferout",NumTx+TxIndex_0)->getTransmissionChannel()->getTransmissionFinishTime();
                     }
                     else if(  (pkt_temp16->getHop(pkt_temp16->getHopCountNow() + 1)   >= 0  ) &&  (pkt_temp16->getHop(pkt_temp16->getHopCountNow() + 1)   <  AllToRNum  )   ){
                             int xx = uniform(0,100);
                             int TxIndex_2 = (xx*NumTx_ToR2ToR)/100;       // go to the direct connected ToR
                             pkt_temp16->setTxIndex(TxIndex_2);
                             GroupToRIndex = 0;
                             pkt_temp16->setGroupToRIndex(GroupToRIndex);
                             sendDelayed(pkt_temp16,DelayOfProcessor,"bufferout",NumTx+NumTx_Core+TxIndex_2);
                             //finishtime_Core = gate("bufferout",NumTx+TxIndex_0)->getTransmissionChannel()->getTransmissionFinishTime();
                     }
                     else if(pkt_temp16->getHop(pkt_temp16->getHopCountNow() + 1)  == -100){
                         if(pkt_temp16->getFlag_last() == 1){
                             PnumReceived_ToR++;
                             Networktime_latency = simTime() - pkt_temp16->getTimestamp();
                             PToR2ToR_Latency = PToR2ToR_Latency + Networktime_latency;
                             //PAverageNetworkLatency = ToR2ToR_Latency/PnumReceived_ToR;
                         }
                             numReceived_ToR++;
                             Networktime_latency = simTime() - pkt_temp16->getTimestamp();
                             ToR2ToR_Latency =  ToR2ToR_Latency + Networktime_latency;
                             Networktime_latency = 0;
                             //AverageNetworkLatency = ToR2ToR_Latency/numReceived_ToR;
                             numReceived_Total++;
                         int DestServer = pkt_temp16->getDst_Server();
                         if(Queue2Server_busy[DestServer] == false){
                             Queue2Server_busy[DestServer] = true;
                             sendDelayed(pkt_temp16,DelayOfProcessor,"serverout",DestServer%ToRserverNum);
                             finishtime = gate("serverout",DestServer%ToRserverNum)->getTransmissionChannel()->getTransmissionFinishTime();

                             pkt *pkt2l16 = new pkt("local_pkt_ToR");
                             pkt2l16->setDst_Server(DestServer);
                             pkt2l16->setIndex(2);
                             pkt2l16->setSchedulingPriority(0);
                             scheduleAt(simTime()+slot_ToR + DelayOfProcessor,pkt2l16);
                             pkt_temp16->setFlag_intraToR(1);
                         }
                         else{
                             int temporyOccupy = 0;
                             for(int i = (DestServer/ToRserverNum)*ToRserverNum;i< (DestServer/ToRserverNum)*ToRserverNum + ToRserverNum ;i++){
                                 temporyOccupy +=  Queue2Server[i].length();
                             }
                             if(temporyOccupy >= Down2ServerBufferSize*ToRDownRadix ){
                                 Queue2Serverlost[DestServer] = Queue2Serverlost[DestServer] + 1;
                                 delete pkt_temp16;
                             }
                             else{

                                 pkt_temp16->setPacketSize(64);
                                 Queue2Server[DestServer].insert(pkt_temp16);
                             }
                         }
                     }
                     break;					 
                 case 17:
                     Optical_pkt_temp->removeMessage(Optical_pkt_temp->getPkt18());
                     pkt_temp17 =  check_and_cast<pkt*>(Optical_pkt_temp->getPkt18());
                     if(  ( pkt_temp17->getHop(pkt_temp17->getHopCountNow() + 1)  >= AllToRNum ) && ( pkt_temp17->getHop(pkt_temp17->getHopCountNow() + 1 ) < AllToRNum +  (FOSRadix*(FOSRadix*FOSRadix + 1 ) ) )  ){
                             int Next_ToR = pkt_temp17->getHop(pkt_temp17->getHopCountNow() + 2);
                             int TxIndex_0 = ( (Next_ToR%(FOSRadix*FOSRadix)) % AgreRadix)/GroupToR;
                             GroupToRIndex = ( (Next_ToR%(FOSRadix*FOSRadix)) % AgreRadix)%GroupToR;
                             pkt_temp17->setTxIndex(TxIndex_0);
                             pkt_temp17->setGroupToRIndex(GroupToRIndex);
                             sendDelayed(pkt_temp17,DelayOfProcessor,"bufferout",TxIndex_0);
                             //finishtime_Agre = gate("bufferout",TxIndex_0)->getTransmissionChannel()->getTransmissionFinishTime();
                     }
                     else if(   (pkt_temp17->getHop(pkt_temp17->getHopCountNow() + 1)  >= AllToRNum +  (FOSRadix*(FOSRadix*FOSRadix + 1 ) ) ) && ( pkt_temp17->getHop(pkt_temp17->getHopCountNow() + 1 ) <  AllToRNum + 2* (FOSRadix*(FOSRadix*FOSRadix + 1 )))   ){
                         int Next_ToR = pkt_temp17->getHop(pkt_temp17->getHopCountNow() + 2);
                         int TxIndex_0 =  ( (Next_ToR%(FOSRadix*FOSRadix)) / CoreRadix)/GroupToRCore;       // go to the direct connected ToR
                             pkt_temp17->setTxIndex(TxIndex_0);
                             GroupToRIndex = ( (Next_ToR%(FOSRadix*FOSRadix)) / CoreRadix)%GroupToRCore;
                             pkt_temp17->setGroupToRIndex(GroupToRIndex);
                             sendDelayed(pkt_temp17,DelayOfProcessor,"bufferout",NumTx+TxIndex_0);
                             //finishtime_Core = gate("bufferout",NumTx+TxIndex_0)->getTransmissionChannel()->getTransmissionFinishTime();
                     }
                     else if(  (pkt_temp17->getHop(pkt_temp17->getHopCountNow() + 1)   >= 0  ) &&  (pkt_temp17->getHop(pkt_temp17->getHopCountNow() + 1)   <  AllToRNum  )   ){
                             int xx = uniform(0,100);
                             int TxIndex_2 = (xx*NumTx_ToR2ToR)/100;       // go to the direct connected ToR
                             pkt_temp17->setTxIndex(TxIndex_2);
                             GroupToRIndex = 0;
                             pkt_temp17->setGroupToRIndex(GroupToRIndex);
                             sendDelayed(pkt_temp17,DelayOfProcessor,"bufferout",NumTx+NumTx_Core+TxIndex_2);
                             //finishtime_Core = gate("bufferout",NumTx+TxIndex_0)->getTransmissionChannel()->getTransmissionFinishTime();
                     }
                     else if(pkt_temp17->getHop(pkt_temp17->getHopCountNow() + 1)  == -100){
                         if(pkt_temp17->getFlag_last() == 1){
                             PnumReceived_ToR++;
                             Networktime_latency = simTime() - pkt_temp17->getTimestamp();
                             PToR2ToR_Latency = PToR2ToR_Latency + Networktime_latency;
                             //PAverageNetworkLatency = ToR2ToR_Latency/PnumReceived_ToR;
                         }
                             numReceived_ToR++;
                             Networktime_latency = simTime() - pkt_temp17->getTimestamp();
                             ToR2ToR_Latency =  ToR2ToR_Latency + Networktime_latency;
                             Networktime_latency = 0;
                             //AverageNetworkLatency = ToR2ToR_Latency/numReceived_ToR;
                             numReceived_Total++;
                         int DestServer = pkt_temp17->getDst_Server();
                         if(Queue2Server_busy[DestServer] == false){
                             Queue2Server_busy[DestServer] = true;
                             sendDelayed(pkt_temp17,DelayOfProcessor,"serverout",DestServer%ToRserverNum);
                             finishtime = gate("serverout",DestServer%ToRserverNum)->getTransmissionChannel()->getTransmissionFinishTime();

                             pkt *pkt2l17 = new pkt("local_pkt_ToR");
                             pkt2l17->setDst_Server(DestServer);
                             pkt2l17->setIndex(2);
                             pkt2l17->setSchedulingPriority(0);
                             scheduleAt(simTime()+slot_ToR + DelayOfProcessor,pkt2l17);
                             pkt_temp17->setFlag_intraToR(1);
                         }
                         else{
                             int temporyOccupy = 0;
                             for(int i = (DestServer/ToRserverNum)*ToRserverNum;i< (DestServer/ToRserverNum)*ToRserverNum + ToRserverNum ;i++){
                                 temporyOccupy +=  Queue2Server[i].length();
                             }
                             if(temporyOccupy >= Down2ServerBufferSize*ToRDownRadix ){
                                 Queue2Serverlost[DestServer] = Queue2Serverlost[DestServer] + 1;
                                 delete pkt_temp17;
                             }
                             else{

                                 pkt_temp17->setPacketSize(64);
                                 Queue2Server[DestServer].insert(pkt_temp17);
                             }
                         }
                     }
                     break;					 
                 case 18:
                     Optical_pkt_temp->removeMessage(Optical_pkt_temp->getPkt19());
                     pkt_temp18 =  check_and_cast<pkt*>(Optical_pkt_temp->getPkt19());
                     if(  ( pkt_temp18->getHop(pkt_temp18->getHopCountNow() + 1)  >= AllToRNum ) && ( pkt_temp18->getHop(pkt_temp18->getHopCountNow() + 1 ) < AllToRNum +  (FOSRadix*(FOSRadix*FOSRadix + 1 ) ) )  ){
                             int Next_ToR = pkt_temp18->getHop(pkt_temp18->getHopCountNow() + 2);
                             int TxIndex_0 = ( (Next_ToR%(FOSRadix*FOSRadix)) % AgreRadix)/GroupToR;
                             GroupToRIndex = ( (Next_ToR%(FOSRadix*FOSRadix)) % AgreRadix)%GroupToR;
                             pkt_temp18->setTxIndex(TxIndex_0);
                             pkt_temp18->setGroupToRIndex(GroupToRIndex);
                             sendDelayed(pkt_temp18,DelayOfProcessor,"bufferout",TxIndex_0);
                             //finishtime_Agre = gate("bufferout",TxIndex_0)->getTransmissionChannel()->getTransmissionFinishTime();
                     }
                     else if(   (pkt_temp18->getHop(pkt_temp18->getHopCountNow() + 1)  >= AllToRNum +  (FOSRadix*(FOSRadix*FOSRadix + 1 ) ) ) && ( pkt_temp18->getHop(pkt_temp18->getHopCountNow() + 1 ) <  AllToRNum + 2* (FOSRadix*(FOSRadix*FOSRadix + 1 )))   ){
                         int Next_ToR = pkt_temp18->getHop(pkt_temp18->getHopCountNow() + 2);
                         int TxIndex_0 =  ( (Next_ToR%(FOSRadix*FOSRadix)) / CoreRadix)/GroupToRCore;       // go to the direct connected ToR
                             pkt_temp18->setTxIndex(TxIndex_0);
                             GroupToRIndex = ( (Next_ToR%(FOSRadix*FOSRadix)) / CoreRadix)%GroupToRCore;
                             pkt_temp18->setGroupToRIndex(GroupToRIndex);
                             sendDelayed(pkt_temp18,DelayOfProcessor,"bufferout",NumTx+TxIndex_0);
                             //finishtime_Core = gate("bufferout",NumTx+TxIndex_0)->getTransmissionChannel()->getTransmissionFinishTime();
                     }
                     else if(  (pkt_temp18->getHop(pkt_temp18->getHopCountNow() + 1)   >= 0  ) &&  (pkt_temp18->getHop(pkt_temp18->getHopCountNow() + 1)   <  AllToRNum  )   ){
                             int xx = uniform(0,100);
                             int TxIndex_2 = (xx*NumTx_ToR2ToR)/100;       // go to the direct connected ToR
                             pkt_temp18->setTxIndex(TxIndex_2);
                             GroupToRIndex = 0;
                             pkt_temp18->setGroupToRIndex(GroupToRIndex);
                             sendDelayed(pkt_temp18,DelayOfProcessor,"bufferout",NumTx+NumTx_Core+TxIndex_2);
                             //finishtime_Core = gate("bufferout",NumTx+TxIndex_0)->getTransmissionChannel()->getTransmissionFinishTime();
                     }
                     else if(pkt_temp18->getHop(pkt_temp18->getHopCountNow() + 1)  == -100){
                         if(pkt_temp18->getFlag_last() == 1){
                             PnumReceived_ToR++;
                             Networktime_latency = simTime() - pkt_temp18->getTimestamp();
                             PToR2ToR_Latency = PToR2ToR_Latency + Networktime_latency;
                             //PAverageNetworkLatency = ToR2ToR_Latency/PnumReceived_ToR;
                         }
                             numReceived_ToR++;
                             Networktime_latency = simTime() - pkt_temp18->getTimestamp();
                             ToR2ToR_Latency =  ToR2ToR_Latency + Networktime_latency;
                             Networktime_latency = 0;
                             //AverageNetworkLatency = ToR2ToR_Latency/numReceived_ToR;
                             numReceived_Total++;
                         int DestServer = pkt_temp18->getDst_Server();
                         if(Queue2Server_busy[DestServer] == false){
                             Queue2Server_busy[DestServer] = true;
                             sendDelayed(pkt_temp18,DelayOfProcessor,"serverout",DestServer%ToRserverNum);
                             finishtime = gate("serverout",DestServer%ToRserverNum)->getTransmissionChannel()->getTransmissionFinishTime();

                             pkt *pkt2l18 = new pkt("local_pkt_ToR");
                             pkt2l18->setDst_Server(DestServer);
                             pkt2l18->setIndex(2);
                             pkt2l18->setSchedulingPriority(0);
                             scheduleAt(simTime()+slot_ToR + DelayOfProcessor,pkt2l18);
                             pkt_temp18->setFlag_intraToR(1);
                         }
                         else{
                             int temporyOccupy = 0;
                             for(int i = (DestServer/ToRserverNum)*ToRserverNum;i< (DestServer/ToRserverNum)*ToRserverNum + ToRserverNum ;i++){
                                 temporyOccupy +=  Queue2Server[i].length();
                             }
                             if(temporyOccupy >= Down2ServerBufferSize*ToRDownRadix ){
                                 Queue2Serverlost[DestServer] = Queue2Serverlost[DestServer] + 1;
                                 delete pkt_temp18;
                             }
                             else{

                                 pkt_temp18->setPacketSize(64);
                                 Queue2Server[DestServer].insert(pkt_temp18);
                             }
                         }
                     }
                     break;					 
                 case 19:
                     Optical_pkt_temp->removeMessage(Optical_pkt_temp->getPkt20());
                     pkt_temp19 =  check_and_cast<pkt*>(Optical_pkt_temp->getPkt20());
                     if(  ( pkt_temp19->getHop(pkt_temp19->getHopCountNow() + 1)  >= AllToRNum ) && ( pkt_temp19->getHop(pkt_temp19->getHopCountNow() + 1 ) < AllToRNum +  (FOSRadix*(FOSRadix*FOSRadix + 1 ) ) )  ){
                             int Next_ToR = pkt_temp19->getHop(pkt_temp19->getHopCountNow() + 2);
                             int TxIndex_0 = ( (Next_ToR%(FOSRadix*FOSRadix)) % AgreRadix)/GroupToR;
                             GroupToRIndex = ( (Next_ToR%(FOSRadix*FOSRadix)) % AgreRadix)%GroupToR;
                             pkt_temp19->setTxIndex(TxIndex_0);
                             pkt_temp19->setGroupToRIndex(GroupToRIndex);
                             sendDelayed(pkt_temp19,DelayOfProcessor,"bufferout",TxIndex_0);
                             //finishtime_Agre = gate("bufferout",TxIndex_0)->getTransmissionChannel()->getTransmissionFinishTime();
                     }
                     else if(   (pkt_temp19->getHop(pkt_temp19->getHopCountNow() + 1)  >= AllToRNum +  (FOSRadix*(FOSRadix*FOSRadix + 1 ) ) ) && ( pkt_temp19->getHop(pkt_temp19->getHopCountNow() + 1 ) <  AllToRNum + 2* (FOSRadix*(FOSRadix*FOSRadix + 1 )))   ){
                         int Next_ToR = pkt_temp19->getHop(pkt_temp19->getHopCountNow() + 2);
                         int TxIndex_0 =  ( (Next_ToR%(FOSRadix*FOSRadix)) / CoreRadix)/GroupToRCore;       // go to the direct connected ToR
                             pkt_temp19->setTxIndex(TxIndex_0);
                             GroupToRIndex = ( (Next_ToR%(FOSRadix*FOSRadix)) / CoreRadix)%GroupToRCore;
                             pkt_temp19->setGroupToRIndex(GroupToRIndex);
                             sendDelayed(pkt_temp19,DelayOfProcessor,"bufferout",NumTx+TxIndex_0);
                             //finishtime_Core = gate("bufferout",NumTx+TxIndex_0)->getTransmissionChannel()->getTransmissionFinishTime();
                     }
                     else if(  (pkt_temp19->getHop(pkt_temp19->getHopCountNow() + 1)   >= 0  ) &&  (pkt_temp19->getHop(pkt_temp19->getHopCountNow() + 1)   <  AllToRNum  )   ){
                             int xx = uniform(0,100);
                             int TxIndex_2 = (xx*NumTx_ToR2ToR)/100;       // go to the direct connected ToR
                             pkt_temp19->setTxIndex(TxIndex_2);
                             GroupToRIndex = 0;
                             pkt_temp19->setGroupToRIndex(GroupToRIndex);
                             sendDelayed(pkt_temp19,DelayOfProcessor,"bufferout",NumTx+NumTx_Core+TxIndex_2);
                             //finishtime_Core = gate("bufferout",NumTx+TxIndex_0)->getTransmissionChannel()->getTransmissionFinishTime();
                     }
                     else if(pkt_temp19->getHop(pkt_temp19->getHopCountNow() + 1)  == -100){
                         if(pkt_temp19->getFlag_last() == 1){
                             PnumReceived_ToR++;
                             Networktime_latency = simTime() - pkt_temp19->getTimestamp();
                             PToR2ToR_Latency = PToR2ToR_Latency + Networktime_latency;
                             //PAverageNetworkLatency = ToR2ToR_Latency/PnumReceived_ToR;
                         }
                             numReceived_ToR++;
                             Networktime_latency = simTime() - pkt_temp19->getTimestamp();
                             ToR2ToR_Latency =  ToR2ToR_Latency + Networktime_latency;
                             Networktime_latency = 0;
                             //AverageNetworkLatency = ToR2ToR_Latency/numReceived_ToR;
                             numReceived_Total++;
                         int DestServer = pkt_temp19->getDst_Server();
                         if(Queue2Server_busy[DestServer] == false){
                             Queue2Server_busy[DestServer] = true;
                             sendDelayed(pkt_temp19,DelayOfProcessor,"serverout",DestServer%ToRserverNum);
                             finishtime = gate("serverout",DestServer%ToRserverNum)->getTransmissionChannel()->getTransmissionFinishTime();

                             pkt *pkt2l19 = new pkt("local_pkt_ToR");
                             pkt2l19->setDst_Server(DestServer);
                             pkt2l19->setIndex(2);
                             pkt2l19->setSchedulingPriority(0);
                             scheduleAt(simTime()+slot_ToR + DelayOfProcessor,pkt2l19);
                             pkt_temp19->setFlag_intraToR(1);
                         }
                         else{
                             int temporyOccupy = 0;
                             for(int i = (DestServer/ToRserverNum)*ToRserverNum;i< (DestServer/ToRserverNum)*ToRserverNum + ToRserverNum ;i++){
                                 temporyOccupy +=  Queue2Server[i].length();
                             }
                             if(temporyOccupy >= Down2ServerBufferSize*ToRDownRadix ){
                                 Queue2Serverlost[DestServer] = Queue2Serverlost[DestServer] + 1;
                                 delete pkt_temp19;
                             }
                             else{

                                 pkt_temp19->setPacketSize(64);
                                 Queue2Server[DestServer].insert(pkt_temp19);
                             }
                         }
                     }
                     break;					 
                 case 20:
                     Optical_pkt_temp->removeMessage(Optical_pkt_temp->getPkt21());
                     pkt_temp20 =  check_and_cast<pkt*>(Optical_pkt_temp->getPkt21());
                     if(  ( pkt_temp20->getHop(pkt_temp20->getHopCountNow() + 1)  >= AllToRNum ) && ( pkt_temp20->getHop(pkt_temp20->getHopCountNow() + 1 ) < AllToRNum +  (FOSRadix*(FOSRadix*FOSRadix + 1 ) ) )  ){
                             int Next_ToR = pkt_temp20->getHop(pkt_temp20->getHopCountNow() + 2);
                             int TxIndex_0 = ( (Next_ToR%(FOSRadix*FOSRadix)) % AgreRadix)/GroupToR;
                             GroupToRIndex = ( (Next_ToR%(FOSRadix*FOSRadix)) % AgreRadix)%GroupToR;
                             pkt_temp20->setTxIndex(TxIndex_0);
                             pkt_temp20->setGroupToRIndex(GroupToRIndex);
                             sendDelayed(pkt_temp20,DelayOfProcessor,"bufferout",TxIndex_0);
                             //finishtime_Agre = gate("bufferout",TxIndex_0)->getTransmissionChannel()->getTransmissionFinishTime();
                     }
                     else if(   (pkt_temp20->getHop(pkt_temp20->getHopCountNow() + 1)  >= AllToRNum +  (FOSRadix*(FOSRadix*FOSRadix + 1 ) ) ) && ( pkt_temp20->getHop(pkt_temp20->getHopCountNow() + 1 ) <  AllToRNum + 2* (FOSRadix*(FOSRadix*FOSRadix + 1 )))   ){
                         int Next_ToR = pkt_temp20->getHop(pkt_temp20->getHopCountNow() + 2);
                         int TxIndex_0 =  ( (Next_ToR%(FOSRadix*FOSRadix)) / CoreRadix)/GroupToRCore;       // go to the direct connected ToR
                             pkt_temp20->setTxIndex(TxIndex_0);
                             GroupToRIndex = ( (Next_ToR%(FOSRadix*FOSRadix)) / CoreRadix)%GroupToRCore;
                             pkt_temp20->setGroupToRIndex(GroupToRIndex);
                             sendDelayed(pkt_temp20,DelayOfProcessor,"bufferout",NumTx+TxIndex_0);
                             //finishtime_Core = gate("bufferout",NumTx+TxIndex_0)->getTransmissionChannel()->getTransmissionFinishTime();
                     }
                     else if( (pkt_temp20->getHop(pkt_temp20->getHopCountNow() + 1)   >= 0  ) &&   (pkt_temp20->getHop(pkt_temp20->getHopCountNow() + 1)   <  AllToRNum  )   ){
                             int xx = uniform(0,100);
                             int TxIndex_2 = (xx*NumTx_ToR2ToR)/100;       // go to the direct connected ToR
                             pkt_temp20->setTxIndex(TxIndex_2);
                             GroupToRIndex = 0;
                             pkt_temp20->setGroupToRIndex(GroupToRIndex);
                             sendDelayed(pkt_temp20,DelayOfProcessor,"bufferout",NumTx+NumTx_Core+TxIndex_2);
                             //finishtime_Core = gate("bufferout",NumTx+TxIndex_0)->getTransmissionChannel()->getTransmissionFinishTime();
                     }
                     else if(pkt_temp20->getHop(pkt_temp20->getHopCountNow() + 1)  == -100){
                         if(pkt_temp20->getFlag_last() == 1){
                             PnumReceived_ToR++;
                             Networktime_latency = simTime() - pkt_temp20->getTimestamp();
                             PToR2ToR_Latency = PToR2ToR_Latency + Networktime_latency;
                             //PAverageNetworkLatency = ToR2ToR_Latency/PnumReceived_ToR;
                         }
                             numReceived_ToR++;
                             Networktime_latency = simTime() - pkt_temp20->getTimestamp();
                             ToR2ToR_Latency =  ToR2ToR_Latency + Networktime_latency;
                             Networktime_latency = 0;
                             //AverageNetworkLatency = ToR2ToR_Latency/numReceived_ToR;
                             numReceived_Total++;
                         int DestServer = pkt_temp20->getDst_Server();
                         if(Queue2Server_busy[DestServer] == false){
                             Queue2Server_busy[DestServer] = true;
                             sendDelayed(pkt_temp20,DelayOfProcessor,"serverout",DestServer%ToRserverNum);
                             finishtime = gate("serverout",DestServer%ToRserverNum)->getTransmissionChannel()->getTransmissionFinishTime();

                             pkt *pkt2l20 = new pkt("local_pkt_ToR");
                             pkt2l20->setDst_Server(DestServer);
                             pkt2l20->setIndex(2);
                             pkt2l20->setSchedulingPriority(0);
                             scheduleAt(simTime()+slot_ToR + DelayOfProcessor,pkt2l20);
                             pkt_temp20->setFlag_intraToR(1);
                         }
                         else{
                             int temporyOccupy = 0;
                             for(int i = (DestServer/ToRserverNum)*ToRserverNum;i< (DestServer/ToRserverNum)*ToRserverNum + ToRserverNum ;i++){
                                 temporyOccupy +=  Queue2Server[i].length();
                             }
                             if(temporyOccupy >= Down2ServerBufferSize*ToRDownRadix ){
                                 Queue2Serverlost[DestServer] = Queue2Serverlost[DestServer] + 1;
                                 delete pkt_temp20;
                             }
                             else{

                                 pkt_temp20->setPacketSize(64);
                                 Queue2Server[DestServer].insert(pkt_temp20);
                             }
                         }
                     }
                     break;					 
                 case 21:
                     Optical_pkt_temp->removeMessage(Optical_pkt_temp->getPkt22());
                     pkt_temp21 =  check_and_cast<pkt*>(Optical_pkt_temp->getPkt22());
                     if(  ( pkt_temp21->getHop(pkt_temp21->getHopCountNow() + 1)  >= AllToRNum ) && ( pkt_temp21->getHop(pkt_temp21->getHopCountNow() + 1 ) < AllToRNum +  (FOSRadix*(FOSRadix*FOSRadix + 1 ) ) )  ){
                             int Next_ToR = pkt_temp21->getHop(pkt_temp21->getHopCountNow() + 2);
                             int TxIndex_0 = ( (Next_ToR%(FOSRadix*FOSRadix)) % AgreRadix)/GroupToR;
                             GroupToRIndex = ( (Next_ToR%(FOSRadix*FOSRadix)) % AgreRadix)%GroupToR;
                             pkt_temp21->setTxIndex(TxIndex_0);
                             pkt_temp21->setGroupToRIndex(GroupToRIndex);
                             sendDelayed(pkt_temp21,DelayOfProcessor,"bufferout",TxIndex_0);
                             //finishtime_Agre = gate("bufferout",TxIndex_0)->getTransmissionChannel()->getTransmissionFinishTime();
                     }
                     else if(   (pkt_temp21->getHop(pkt_temp21->getHopCountNow() + 1)  >= AllToRNum +  (FOSRadix*(FOSRadix*FOSRadix + 1 ) ) ) && ( pkt_temp21->getHop(pkt_temp21->getHopCountNow() + 1 ) <  AllToRNum + 2* (FOSRadix*(FOSRadix*FOSRadix + 1 )))   ){
                         int Next_ToR = pkt_temp21->getHop(pkt_temp21->getHopCountNow() + 2);
                         int TxIndex_0 =  ( (Next_ToR%(FOSRadix*FOSRadix)) / CoreRadix)/GroupToRCore;       // go to the direct connected ToR
                             pkt_temp21->setTxIndex(TxIndex_0);
                             GroupToRIndex = ( (Next_ToR%(FOSRadix*FOSRadix)) / CoreRadix)%GroupToRCore;
                             pkt_temp21->setGroupToRIndex(GroupToRIndex);
                             sendDelayed(pkt_temp21,DelayOfProcessor,"bufferout",NumTx+TxIndex_0);
                             //finishtime_Core = gate("bufferout",NumTx+TxIndex_0)->getTransmissionChannel()->getTransmissionFinishTime();
                     }
                     else if(  (pkt_temp21->getHop(pkt_temp21->getHopCountNow() + 1)   >= 0  ) &&  (pkt_temp21->getHop(pkt_temp21->getHopCountNow() + 1)   <  AllToRNum  )   ){
                             int xx = uniform(0,100);
                             int TxIndex_2 = (xx*NumTx_ToR2ToR)/100;       // go to the direct connected ToR
                             pkt_temp21->setTxIndex(TxIndex_2);
                             GroupToRIndex = 0;
                             pkt_temp21->setGroupToRIndex(GroupToRIndex);
                             sendDelayed(pkt_temp21,DelayOfProcessor,"bufferout",NumTx+NumTx_Core+TxIndex_2);
                             //finishtime_Core = gate("bufferout",NumTx+TxIndex_0)->getTransmissionChannel()->getTransmissionFinishTime();
                     }
                     else if(pkt_temp21->getHop(pkt_temp21->getHopCountNow() + 1)  == -100){
                         if(pkt_temp21->getFlag_last() == 1){
                             PnumReceived_ToR++;
                             Networktime_latency = simTime() - pkt_temp21->getTimestamp();
                             PToR2ToR_Latency = PToR2ToR_Latency + Networktime_latency;
                             //PAverageNetworkLatency = ToR2ToR_Latency/PnumReceived_ToR;
                         }
                             numReceived_ToR++;
                             Networktime_latency = simTime() - pkt_temp21->getTimestamp();
                             ToR2ToR_Latency =  ToR2ToR_Latency + Networktime_latency;
                             Networktime_latency = 0;
                             //AverageNetworkLatency = ToR2ToR_Latency/numReceived_ToR;
                             numReceived_Total++;
                         int DestServer = pkt_temp21->getDst_Server();
                         if(Queue2Server_busy[DestServer] == false){
                             Queue2Server_busy[DestServer] = true;
                             sendDelayed(pkt_temp21,DelayOfProcessor,"serverout",DestServer%ToRserverNum);
                             finishtime = gate("serverout",DestServer%ToRserverNum)->getTransmissionChannel()->getTransmissionFinishTime();

                             pkt *pkt2l21 = new pkt("local_pkt_ToR");
                             pkt2l21->setDst_Server(DestServer);
                             pkt2l21->setIndex(2);
                             pkt2l21->setSchedulingPriority(0);
                             scheduleAt(simTime()+slot_ToR + DelayOfProcessor,pkt2l21);
                             pkt_temp21->setFlag_intraToR(1);
                         }
                         else{
                             int temporyOccupy = 0;
                             for(int i = (DestServer/ToRserverNum)*ToRserverNum;i< (DestServer/ToRserverNum)*ToRserverNum + ToRserverNum ;i++){
                                 temporyOccupy +=  Queue2Server[i].length();
                             }
                             if(temporyOccupy >= Down2ServerBufferSize*ToRDownRadix ){
                                 Queue2Serverlost[DestServer] = Queue2Serverlost[DestServer] + 1;
                                 delete pkt_temp21;
                             }
                             else{

                                 pkt_temp21->setPacketSize(64);
                                 Queue2Server[DestServer].insert(pkt_temp21);
                             }
                         }
                     }
                     break;					 
                 case 22:
                     Optical_pkt_temp->removeMessage(Optical_pkt_temp->getPkt23());
                     pkt_temp22 =  check_and_cast<pkt*>(Optical_pkt_temp->getPkt23());
                     if(  ( pkt_temp22->getHop(pkt_temp22->getHopCountNow() + 1)  >= AllToRNum ) && ( pkt_temp22->getHop(pkt_temp22->getHopCountNow() + 1 ) < AllToRNum +  (FOSRadix*(FOSRadix*FOSRadix + 1 ) ) )  ){
                             int Next_ToR = pkt_temp22->getHop(pkt_temp22->getHopCountNow() + 2);
                             int TxIndex_0 = ( (Next_ToR%(FOSRadix*FOSRadix)) % AgreRadix)/GroupToR;
                             GroupToRIndex = ( (Next_ToR%(FOSRadix*FOSRadix)) % AgreRadix)%GroupToR;
                             pkt_temp22->setTxIndex(TxIndex_0);
                             pkt_temp22->setGroupToRIndex(GroupToRIndex);
                             sendDelayed(pkt_temp22,DelayOfProcessor,"bufferout",TxIndex_0);
                             //finishtime_Agre = gate("bufferout",TxIndex_0)->getTransmissionChannel()->getTransmissionFinishTime();
                     }
                     else if(   (pkt_temp22->getHop(pkt_temp22->getHopCountNow() + 1)  >= AllToRNum +  (FOSRadix*(FOSRadix*FOSRadix + 1 ) ) ) && ( pkt_temp22->getHop(pkt_temp22->getHopCountNow() + 1 ) <  AllToRNum + 2* (FOSRadix*(FOSRadix*FOSRadix + 1 )))   ){
                         int Next_ToR = pkt_temp22->getHop(pkt_temp22->getHopCountNow() + 2);
                         int TxIndex_0 =  ( (Next_ToR%(FOSRadix*FOSRadix)) / CoreRadix)/GroupToRCore;       // go to the direct connected ToR
                             pkt_temp22->setTxIndex(TxIndex_0);
                             GroupToRIndex = ( (Next_ToR%(FOSRadix*FOSRadix)) / CoreRadix)%GroupToRCore;
                             pkt_temp22->setGroupToRIndex(GroupToRIndex);
                             sendDelayed(pkt_temp22,DelayOfProcessor,"bufferout",NumTx+TxIndex_0);
                             //finishtime_Core = gate("bufferout",NumTx+TxIndex_0)->getTransmissionChannel()->getTransmissionFinishTime();
                     }
                     else if( (pkt_temp22->getHop(pkt_temp22->getHopCountNow() + 1)   >= 0  ) &&   (pkt_temp22->getHop(pkt_temp22->getHopCountNow() + 1)   <  AllToRNum  )   ){
                             int xx = uniform(0,100);
                             int TxIndex_2 = (xx*NumTx_ToR2ToR)/100;       // go to the direct connected ToR
                             pkt_temp22->setTxIndex(TxIndex_2);
                             GroupToRIndex = 0;
                             pkt_temp22->setGroupToRIndex(GroupToRIndex);
                             sendDelayed(pkt_temp22,DelayOfProcessor,"bufferout",NumTx+NumTx_Core+TxIndex_2);
                             //finishtime_Core = gate("bufferout",NumTx+TxIndex_0)->getTransmissionChannel()->getTransmissionFinishTime();
                     }
                     else if(pkt_temp22->getHop(pkt_temp22->getHopCountNow() + 1)  == -100){
                         if(pkt_temp22->getFlag_last() == 1){
                             PnumReceived_ToR++;
                             Networktime_latency = simTime() - pkt_temp22->getTimestamp();
                             PToR2ToR_Latency = PToR2ToR_Latency + Networktime_latency;
                             //PAverageNetworkLatency = ToR2ToR_Latency/PnumReceived_ToR;
                         }
                             numReceived_ToR++;
                             Networktime_latency = simTime() - pkt_temp22->getTimestamp();
                             ToR2ToR_Latency =  ToR2ToR_Latency + Networktime_latency;
                             Networktime_latency = 0;
                             //AverageNetworkLatency = ToR2ToR_Latency/numReceived_ToR;
                             numReceived_Total++;
                         int DestServer = pkt_temp22->getDst_Server();
                         if(Queue2Server_busy[DestServer] == false){
                             Queue2Server_busy[DestServer] = true;
                             sendDelayed(pkt_temp22,DelayOfProcessor,"serverout",DestServer%ToRserverNum);
                             finishtime = gate("serverout",DestServer%ToRserverNum)->getTransmissionChannel()->getTransmissionFinishTime();

                             pkt *pkt2l22 = new pkt("local_pkt_ToR");
                             pkt2l22->setDst_Server(DestServer);
                             pkt2l22->setIndex(2);
                             pkt2l22->setSchedulingPriority(0);
                             scheduleAt(simTime()+slot_ToR + DelayOfProcessor,pkt2l22);
                             pkt_temp22->setFlag_intraToR(1);
                         }
                         else{
                             int temporyOccupy = 0;
                             for(int i = (DestServer/ToRserverNum)*ToRserverNum;i< (DestServer/ToRserverNum)*ToRserverNum + ToRserverNum ;i++){
                                 temporyOccupy +=  Queue2Server[i].length();
                             }
                             if(temporyOccupy >= Down2ServerBufferSize*ToRDownRadix ){
                                 Queue2Serverlost[DestServer] = Queue2Serverlost[DestServer] + 1;
                                 delete pkt_temp22;
                             }
                             else{

                                 pkt_temp22->setPacketSize(64);
                                 Queue2Server[DestServer].insert(pkt_temp22);
                             }
                         }
                     }
                     break;					 
                 case 23:
                     Optical_pkt_temp->removeMessage(Optical_pkt_temp->getPkt24());
                     pkt_temp23 =  check_and_cast<pkt*>(Optical_pkt_temp->getPkt24());
                     if(  ( pkt_temp23->getHop(pkt_temp23->getHopCountNow() + 1)  >= AllToRNum ) && ( pkt_temp23->getHop(pkt_temp23->getHopCountNow() + 1 ) < AllToRNum +  (FOSRadix*(FOSRadix*FOSRadix + 1 ) ) )  ){
                             int Next_ToR = pkt_temp23->getHop(pkt_temp23->getHopCountNow() + 2);
                             int TxIndex_0 = ( (Next_ToR%(FOSRadix*FOSRadix)) % AgreRadix)/GroupToR;
                             GroupToRIndex = ( (Next_ToR%(FOSRadix*FOSRadix)) % AgreRadix)%GroupToR;
                             pkt_temp23->setTxIndex(TxIndex_0);
                             pkt_temp23->setGroupToRIndex(GroupToRIndex);
                             sendDelayed(pkt_temp23,DelayOfProcessor,"bufferout",TxIndex_0);
                             //finishtime_Agre = gate("bufferout",TxIndex_0)->getTransmissionChannel()->getTransmissionFinishTime();
                     }
                     else if(   (pkt_temp23->getHop(pkt_temp23->getHopCountNow() + 1)  >= AllToRNum +  (FOSRadix*(FOSRadix*FOSRadix + 1 ) ) ) && ( pkt_temp23->getHop(pkt_temp23->getHopCountNow() + 1 ) <  AllToRNum + 2* (FOSRadix*(FOSRadix*FOSRadix + 1 )))   ){
                         int Next_ToR = pkt_temp23->getHop(pkt_temp23->getHopCountNow() + 2);
                         int TxIndex_0 =  ( (Next_ToR%(FOSRadix*FOSRadix)) / CoreRadix)/GroupToRCore;       // go to the direct connected ToR
                             pkt_temp23->setTxIndex(TxIndex_0);
                             GroupToRIndex = ( (Next_ToR%(FOSRadix*FOSRadix)) / CoreRadix)%GroupToRCore;
                             pkt_temp23->setGroupToRIndex(GroupToRIndex);
                             sendDelayed(pkt_temp23,DelayOfProcessor,"bufferout",NumTx+TxIndex_0);
                             //finishtime_Core = gate("bufferout",NumTx+TxIndex_0)->getTransmissionChannel()->getTransmissionFinishTime();
                     }
                     else if( (pkt_temp23->getHop(pkt_temp23->getHopCountNow() + 1)   >= 0  ) &&   (pkt_temp23->getHop(pkt_temp23->getHopCountNow() + 1)   <  AllToRNum  )   ){
                             int xx = uniform(0,100);
                             int TxIndex_2 = (xx*NumTx_ToR2ToR)/100;       // go to the direct connected ToR
                             pkt_temp23->setTxIndex(TxIndex_2);
                             GroupToRIndex = 0;
                             pkt_temp23->setGroupToRIndex(GroupToRIndex);
                             sendDelayed(pkt_temp23,DelayOfProcessor,"bufferout",NumTx+NumTx_Core+TxIndex_2);
                             //finishtime_Core = gate("bufferout",NumTx+TxIndex_0)->getTransmissionChannel()->getTransmissionFinishTime();
                     }
                     else if(pkt_temp23->getHop(pkt_temp23->getHopCountNow() + 1)  == -100){
                         if(pkt_temp23->getFlag_last() == 1){
                             PnumReceived_ToR++;
                             Networktime_latency = simTime() - pkt_temp23->getTimestamp();
                             PToR2ToR_Latency = PToR2ToR_Latency + Networktime_latency;
                             //PAverageNetworkLatency = ToR2ToR_Latency/PnumReceived_ToR;
                         }
                             numReceived_ToR++;
                             Networktime_latency = simTime() - pkt_temp23->getTimestamp();
                             ToR2ToR_Latency =  ToR2ToR_Latency + Networktime_latency;
                             Networktime_latency = 0;
                             //AverageNetworkLatency = ToR2ToR_Latency/numReceived_ToR;
                             numReceived_Total++;
                         int DestServer = pkt_temp23->getDst_Server();
                         if(Queue2Server_busy[DestServer] == false){
                             Queue2Server_busy[DestServer] = true;
                             sendDelayed(pkt_temp23,DelayOfProcessor,"serverout",DestServer%ToRserverNum);
                             finishtime = gate("serverout",DestServer%ToRserverNum)->getTransmissionChannel()->getTransmissionFinishTime();

                             pkt *pkt2l23 = new pkt("local_pkt_ToR");
                             pkt2l23->setDst_Server(DestServer);
                             pkt2l23->setIndex(2);
                             pkt2l23->setSchedulingPriority(0);
                             scheduleAt(simTime()+slot_ToR + DelayOfProcessor,pkt2l23);
                             pkt_temp23->setFlag_intraToR(1);
                         }
                         else{
                             int temporyOccupy = 0;
                             for(int i = (DestServer/ToRserverNum)*ToRserverNum;i< (DestServer/ToRserverNum)*ToRserverNum + ToRserverNum ;i++){
                                 temporyOccupy +=  Queue2Server[i].length();
                             }
                             if(temporyOccupy >= Down2ServerBufferSize*ToRDownRadix ){
                                 Queue2Serverlost[DestServer] = Queue2Serverlost[DestServer] + 1;
                                 delete pkt_temp23;
                             }
                             else{

                                 pkt_temp23->setPacketSize(64);
                                 Queue2Server[DestServer].insert(pkt_temp23);
                             }
                         }
                     }
                     break;					 
                 case 24:
                     Optical_pkt_temp->removeMessage(Optical_pkt_temp->getPkt25());
                     pkt_temp24 =  check_and_cast<pkt*>(Optical_pkt_temp->getPkt25());
                     if(  ( pkt_temp24->getHop(pkt_temp24->getHopCountNow() + 1)  >= AllToRNum ) && ( pkt_temp24->getHop(pkt_temp24->getHopCountNow() + 1 ) < AllToRNum +  (FOSRadix*(FOSRadix*FOSRadix + 1 ) ) )  ){
                             int Next_ToR = pkt_temp24->getHop(pkt_temp24->getHopCountNow() + 2);
                             int TxIndex_0 = ( (Next_ToR%(FOSRadix*FOSRadix)) % AgreRadix)/GroupToR;
                             GroupToRIndex = ( (Next_ToR%(FOSRadix*FOSRadix)) % AgreRadix)%GroupToR;
                             pkt_temp24->setTxIndex(TxIndex_0);
                             pkt_temp24->setGroupToRIndex(GroupToRIndex);
                             sendDelayed(pkt_temp24,DelayOfProcessor,"bufferout",TxIndex_0);
                             //finishtime_Agre = gate("bufferout",TxIndex_0)->getTransmissionChannel()->getTransmissionFinishTime();
                     }
                     else if(   (pkt_temp24->getHop(pkt_temp24->getHopCountNow() + 1)  >= AllToRNum +  (FOSRadix*(FOSRadix*FOSRadix + 1 ) ) ) && ( pkt_temp24->getHop(pkt_temp24->getHopCountNow() + 1 ) <  AllToRNum + 2* (FOSRadix*(FOSRadix*FOSRadix + 1 )))   ){
                         int Next_ToR = pkt_temp24->getHop(pkt_temp24->getHopCountNow() + 2);
                         int TxIndex_0 =  ( (Next_ToR%(FOSRadix*FOSRadix)) / CoreRadix)/GroupToRCore;       // go to the direct connected ToR
                             pkt_temp24->setTxIndex(TxIndex_0);
                             GroupToRIndex = ( (Next_ToR%(FOSRadix*FOSRadix)) / CoreRadix)%GroupToRCore;
                             pkt_temp24->setGroupToRIndex(GroupToRIndex);
                             sendDelayed(pkt_temp24,DelayOfProcessor,"bufferout",NumTx+TxIndex_0);
                             //finishtime_Core = gate("bufferout",NumTx+TxIndex_0)->getTransmissionChannel()->getTransmissionFinishTime();
                     }
                     else if( (pkt_temp24->getHop(pkt_temp24->getHopCountNow() + 1)   >= 0  ) &&   (pkt_temp24->getHop(pkt_temp24->getHopCountNow() + 1)   <  AllToRNum  )   ){
                             int xx = uniform(0,100);
                             int TxIndex_2 = (xx*NumTx_ToR2ToR)/100;       // go to the direct connected ToR
                             pkt_temp24->setTxIndex(TxIndex_2);
                             GroupToRIndex = 0;
                             pkt_temp24->setGroupToRIndex(GroupToRIndex);
                             sendDelayed(pkt_temp24,DelayOfProcessor,"bufferout",NumTx+NumTx_Core+TxIndex_2);
                             //finishtime_Core = gate("bufferout",NumTx+TxIndex_0)->getTransmissionChannel()->getTransmissionFinishTime();
                     }
                     else if(pkt_temp24->getHop(pkt_temp24->getHopCountNow() + 1)  == -100){
                         if(pkt_temp24->getFlag_last() == 1){
                             PnumReceived_ToR++;
                             Networktime_latency = simTime() - pkt_temp24->getTimestamp();
                             PToR2ToR_Latency = PToR2ToR_Latency + Networktime_latency;
                             //PAverageNetworkLatency = ToR2ToR_Latency/PnumReceived_ToR;
                         }
                             numReceived_ToR++;
                             Networktime_latency = simTime() - pkt_temp24->getTimestamp();
                             ToR2ToR_Latency =  ToR2ToR_Latency + Networktime_latency;
                             Networktime_latency = 0;
                             //AverageNetworkLatency = ToR2ToR_Latency/numReceived_ToR;
                             numReceived_Total++;
                         int DestServer = pkt_temp24->getDst_Server();
                         if(Queue2Server_busy[DestServer] == false){
                             Queue2Server_busy[DestServer] = true;
                             sendDelayed(pkt_temp24,DelayOfProcessor,"serverout",DestServer%ToRserverNum);
                             finishtime = gate("serverout",DestServer%ToRserverNum)->getTransmissionChannel()->getTransmissionFinishTime();

                             pkt *pkt2l24 = new pkt("local_pkt_ToR");
                             pkt2l24->setDst_Server(DestServer);
                             pkt2l24->setIndex(2);
                             pkt2l24->setSchedulingPriority(0);
                             scheduleAt(simTime()+slot_ToR + DelayOfProcessor,pkt2l24);
                             pkt_temp24->setFlag_intraToR(1);
                         }
                         else{
                             int temporyOccupy = 0;
                             for(int i = (DestServer/ToRserverNum)*ToRserverNum;i< (DestServer/ToRserverNum)*ToRserverNum + ToRserverNum ;i++){
                                 temporyOccupy +=  Queue2Server[i].length();
                             }
                             if(temporyOccupy >= Down2ServerBufferSize*ToRDownRadix ){
                                 Queue2Serverlost[DestServer] = Queue2Serverlost[DestServer] + 1;
                                 delete pkt_temp24;
                             }
                             else{

                                 pkt_temp24->setPacketSize(64);
                                 Queue2Server[DestServer].insert(pkt_temp24);
                             }
                         }
                     }
                     break;					 
	 
            }
        }
        delete msg;

    }
	else{
	    xx++;
			    pkt *pkt_temp =  check_and_cast<pkt*>(msg);

			    if(pkt_temp->getIndex() == 1){

			            if(pkt_temp->getHopCountNow() == 0){
			                  receivedPackets++;
			                        int DstToR = pkt_temp->getDst_ToR();
			                        caseofcellsize = pkt_temp->getPacketSize()/64 + 1;
			                        if(pkt_temp->getPacketSize()%64==0)
			                              caseofcellsize -=1;
			                        switch(caseofcellsize){
			                            case 1:
			                            {
			                                count2ToR[DstToR] += 1;
			                                break;
			                            }
			                            case 2:
			                            {
			                                count2ToR[DstToR] += 2;
			                                break;
			                            }
			                            case 3:
			                            {   count2ToR[DstToR] += 3;
			                                break;
			                            }
			                            case 4:
			                            {    count2ToR[DstToR] += 4;
			                                 break;
			                            }
			                            case 5:
			                            {
			                                count2ToR[DstToR] += 5;
			                                break;
			                            }
			                            case 6:
			                            {    count2ToR[DstToR] += 6;
			                                break;
			                            }
			                            case 7:
			                            {
			                               count2ToR[DstToR] += 7;
			                               break;
			                            }
			                            case 8:
			                              {
			                                  count2ToR[DstToR] += 8;
			                                  break;
			                              }
			                              case 9:
			                              {
			                                  count2ToR[DstToR] += 9;
			                                  break;
			                              }
			                              case 10:
			                              {   count2ToR[DstToR] += 10;
			                                  break;
			                              }
			                              case 11:
			                              {    count2ToR[DstToR] += 11;
			                                   break;
			                              }
			                              case 12:
			                              {
			                                  count2ToR[DstToR] += 12;
			                                  break;
			                              }
			                              case 13:
			                              {    count2ToR[DstToR] += 13;
			                                  break;
			                              }
			                              case 14:
			                              {
			                                 count2ToR[DstToR] += 14;
			                                 break;
			                              }
			                              case 15:
			                             {
			                                count2ToR[DstToR] += 15;
			                                break;
			                             }
			                            case 16:
			                             {
			                                count2ToR[DstToR] += 16;
			                                break;
			                             }
			                            case 17:
			                            {   count2ToR[DstToR] += 17;
			                                break;
			                            }
			                            case 18:
			                            {    count2ToR[DstToR] += 18;
			                                 break;
			                            }
			                            case 19:
			                            {
			                                count2ToR[DstToR] += 19;
			                                break;
			                            }
			                            case 20:
			                            {    count2ToR[DstToR] += 20;
			                                break;
			                            }
			                            case 21:
			                            {
			                               count2ToR[DstToR] += 21;
			                               break;
			                            }
			                            case 22:
			                              {
			                                  count2ToR[DstToR] += 22;
			                                  break;
			                              }
			                              case 23:
			                              {
			                                  count2ToR[DstToR] += 23;
			                                  break;
			                              }
			                              case 24:
			                              {   count2ToR[DstToR] += 24;
			                                  break;
			                              }
			                              case 25:
			                              {    count2ToR[DstToR] += 25;
			                                   break;
			                              }
			                              default:
			                                   break;
			                        }
			            }
		                EV<< "HP received a packet from" <<pkt_temp->getSrc_Server() <<" to "<<pkt_temp->getDst_Server()<<"\n";
		                EV<<"next hop is"<<pkt_temp->getHop(pkt_temp->getHopCountNow() + 1)<<"\n";
                     if(  ( pkt_temp->getHop(pkt_temp->getHopCountNow() + 1)  >= AllToRNum ) && ( pkt_temp->getHop(pkt_temp->getHopCountNow() + 1 ) < AllToRNum +  (FOSRadix*(FOSRadix*FOSRadix + 1 ) ) )  ){
                             int Next_ToR = pkt_temp->getHop(pkt_temp->getHopCountNow() + 2);
                             int TxIndex_0 = ( (Next_ToR%(FOSRadix*FOSRadix)) % AgreRadix)/GroupToR;
                             GroupToRIndex = ( (Next_ToR%(FOSRadix*FOSRadix)) % AgreRadix)%GroupToR;
                             pkt_temp->setTxIndex(TxIndex_0);
                             pkt_temp->setGroupToRIndex(GroupToRIndex);
                             sendDelayed(pkt_temp,DelayOfProcessor,"bufferout",TxIndex_0);
                             //finishtime_Agre = gate("bufferout",TxIndex_0)->getTransmissionChannel()->getTransmissionFinishTime();
                     }
                     else if(   (pkt_temp->getHop(pkt_temp->getHopCountNow() + 1)  >= AllToRNum +  (FOSRadix*(FOSRadix*FOSRadix + 1 ) ) ) && ( pkt_temp->getHop(pkt_temp->getHopCountNow() + 1 ) <  AllToRNum + 2* (FOSRadix*(FOSRadix*FOSRadix + 1 )))   ){
                         int Next_ToR = pkt_temp->getHop(pkt_temp->getHopCountNow() + 2);
                         int TxIndex_0 =  ( (Next_ToR%(FOSRadix*FOSRadix)) / CoreRadix)/GroupToRCore;       // go to the direct connected ToR
                             pkt_temp->setTxIndex(TxIndex_0);
                             GroupToRIndex = ( (Next_ToR%(FOSRadix*FOSRadix)) / CoreRadix)%GroupToRCore;
                             pkt_temp->setGroupToRIndex(GroupToRIndex);
                             sendDelayed(pkt_temp,DelayOfProcessor,"bufferout",NumTx+TxIndex_0);
                             //finishtime_Core = gate("bufferout",NumTx+TxIndex_0)->getTransmissionChannel()->getTransmissionFinishTime();
                     }
                     else if( (pkt_temp->getHop(pkt_temp->getHopCountNow() + 1)   >= 0)   &&  (pkt_temp->getHop(pkt_temp->getHopCountNow() + 1)   <  AllToRNum  )   ){
                             int xx = uniform(0,100);
                             int TxIndex_2 = (xx*NumTx_ToR2ToR)/100;       // go to the direct connected ToR
                             pkt_temp->setTxIndex(TxIndex_2);
                             GroupToRIndex = 0;
                             pkt_temp->setGroupToRIndex(GroupToRIndex);
                             sendDelayed(pkt_temp,DelayOfProcessor,"bufferout",NumTx+NumTx_Core+TxIndex_2);
                             //finishtime_Core = gate("bufferout",NumTx+TxIndex_0)->getTransmissionChannel()->getTransmissionFinishTime();
                     }
			        else if( pkt_temp->getHop(pkt_temp->getHopCountNow() + 1)  == -100){

			            if(pkt_temp->getHopCountNow() > 1){
			                numReceived_ToR++;
			 //               Networktime_latency = simTime() - pkt_temp->getTimestamp() + TxBufferTime + PreambleInsertdelay + BurstProcessDelay;
			                Networktime_latency = simTime() - pkt_temp->getTimestamp();

			                ToR2ToR_Latency =  ToR2ToR_Latency + Networktime_latency;
			                Networktime_latency = 0;
			                //AverageNetworkLatency = ToR2ToR_Latency/numReceived_ToR;

			                numReceived_Total++;
			            }
			            else{
			                caseofcellsize = pkt_temp->getPacketSize()/64 + 1;
			                if(pkt_temp->getPacketSize()%64==0)
			                      caseofcellsize -=1;
			                numReceived_Total = numReceived_Total + caseofcellsize;
			            }


			            if(pkt_temp->getHopCountNow() > 1){
			                CellNum =1;
			            }
			            else{
			                CellNum = pkt_temp->getPacketSize()/64 + 1;
			                if(pkt_temp->getPacketSize()%64==0)
			                    CellNum -=1;
			            }
			//            delete msg;
			            int DestServer = pkt_temp->getDst_Server();
			            EV<< "received a packet from" <<pkt_temp->getSrc_Server() <<" to "<<pkt_temp->getDst_Server()<<"\n";

			            if(Queue2Server_busy[DestServer] == false){

			                Queue2Server_busy[DestServer] = true;

	                        if(pkt_temp->getHopCountNow() == 0){
	                            pkt_temp->setFlag_last(1);
	                        }
	                        if(pkt_temp->getHopCountNow() == 0  && pkt_temp->getFlag_last() == 1){
	                            PnumReceived_ToR++;
	                            Networktime_latency = simTime() - pkt_temp->getTimestamp();
	                            PToR2ToR_Latency = PToR2ToR_Latency + Networktime_latency;
	                            //PAverageNetworkLatency = ToR2ToR_Latency/PnumReceived_ToR;
	                        }

			                sendDelayed(msg,DelayOfProcessor,"serverout",DestServer%ToRserverNum);
			                finishtime = gate("serverout",DestServer%ToRserverNum)->getTransmissionChannel()->getTransmissionFinishTime();

			                pkt *pkt2l = new pkt("local_pkt_ToR");
			                pkt2l->setDst_Server(DestServer);
			                pkt2l->setIndex(2);
			                pkt2l->setSchedulingPriority(0);
			                scheduleAt(simTime()+slot_ToR*CellNum + DelayOfProcessor,pkt2l);
			                pkt_temp->setFlag_intraToR(CellNum);
			            }
			            else{


	                        int temporyOccupy = 0;
	                        int startServer = (DestServer/ToRserverNum)*ToRserverNum;
	                        int endServer = (DestServer/ToRserverNum)*ToRserverNum + ToRserverNum;
                           /* if(xx==2){
                                 EV<<"startServer is "<<startServer<<"\n";
                                 EV<<" endServer is "<< endServer<<"\n";
                                 EV<<"Queue2Server[i].length() is "<<Queue2Server[20].length()<<"\n";
                                 EV<<" Queue2Server[i].length() is "<< Queue2Server[21].length()<<"\n";

                                 delete msg;
                                 return;
                             }*/

	                        for(int i =startServer ;i< endServer ;i++){
	                            temporyOccupy +=  Queue2Server[i].length();
	                        }


			                if(temporyOccupy >= Down2ServerBufferSize*ToRDownRadix  +1-CellNum){
			                    Queue2Serverlost[DestServer] = Queue2Serverlost[DestServer] + CellNum;
			                    delete msg;
			                }
			                else{
			                    switch(CellNum){
			                          case 1:
			                          {
			                              pkt_temp->setPacketSize(64);
	                                       if(pkt_temp->getHopCountNow() == 0) { pkt_temp->setFlag_last(1);}  Queue2Server[DestServer].insert(pkt_temp);
			                              break;
			                          }
			                          case 2:
			                          {
			                              pkt_temp->setPacketSize(64);
			                            pkt *pkt_temp_1 = pkt_temp->dup();
			                            Queue2Server[DestServer].insert(pkt_temp_1);
	                                       if(pkt_temp->getHopCountNow() == 0) { pkt_temp->setFlag_last(1);}  Queue2Server[DestServer].insert(pkt_temp);
			                              break;
			                          }
			                          case 3:
			                          {
			                              pkt_temp->setPacketSize(64);
			                          pkt *pkt_temp_1 = pkt_temp->dup();
			                          pkt *pkt_temp_2 = pkt_temp->dup();
			                          Queue2Server[DestServer].insert(pkt_temp_1);
			                          Queue2Server[DestServer].insert(pkt_temp_2);
	                                     if(pkt_temp->getHopCountNow() == 0) { pkt_temp->setFlag_last(1);}  Queue2Server[DestServer].insert(pkt_temp);
			                              break;
			                          }
			                          case 4:
			                          {
			                              pkt_temp->setPacketSize(64);
			                          pkt *pkt_temp_1 = pkt_temp->dup();
			                          pkt *pkt_temp_2 = pkt_temp->dup();
			                          pkt *pkt_temp_3 = pkt_temp->dup();

			                          Queue2Server[DestServer].insert(pkt_temp_1);
			                          Queue2Server[DestServer].insert(pkt_temp_2);
			                          Queue2Server[DestServer].insert(pkt_temp_3);
	                                     if(pkt_temp->getHopCountNow() == 0) { pkt_temp->setFlag_last(1);}  Queue2Server[DestServer].insert(pkt_temp);
			                               break;
			                          }
			                          case 5:
			                          {
			                              pkt_temp->setPacketSize(64);
			                            pkt *pkt_temp_1 = pkt_temp->dup();
			                            pkt *pkt_temp_2 = pkt_temp->dup();
			                            pkt *pkt_temp_3 = pkt_temp->dup();
			                            pkt *pkt_temp_4 = pkt_temp->dup();
			                            Queue2Server[DestServer].insert(pkt_temp_1);
			                            Queue2Server[DestServer].insert(pkt_temp_2);
			                            Queue2Server[DestServer].insert(pkt_temp_3);
			                            Queue2Server[DestServer].insert(pkt_temp_4);
	                                       if(pkt_temp->getHopCountNow() == 0) { pkt_temp->setFlag_last(1);}  Queue2Server[DestServer].insert(pkt_temp);
			                              break;
			                          }
			                          case 6:
			                          {    pkt_temp->setPacketSize(64);
			                          pkt *pkt_temp_1 = pkt_temp->dup();
			                          pkt *pkt_temp_2 = pkt_temp->dup();
			                          pkt *pkt_temp_3 = pkt_temp->dup();
			                          pkt *pkt_temp_4 = pkt_temp->dup();
			                          pkt *pkt_temp_5 = pkt_temp->dup();
			                          Queue2Server[DestServer].insert(pkt_temp_1);
			                          Queue2Server[DestServer].insert(pkt_temp_2);
			                          Queue2Server[DestServer].insert(pkt_temp_3);
			                          Queue2Server[DestServer].insert(pkt_temp_4);
			                          Queue2Server[DestServer].insert(pkt_temp_5);
	                                     if(pkt_temp->getHopCountNow() == 0) { pkt_temp->setFlag_last(1);}  Queue2Server[DestServer].insert(pkt_temp);
			                              break;
			                          }
			                          case 7:
			                          {
			                              pkt_temp->setPacketSize(64);
			                            pkt *pkt_temp_1 = pkt_temp->dup();
			                            pkt *pkt_temp_2 = pkt_temp->dup();
			                            pkt *pkt_temp_3 = pkt_temp->dup();
			                            pkt *pkt_temp_4 = pkt_temp->dup();
			                            pkt *pkt_temp_5 = pkt_temp->dup();
			                            pkt *pkt_temp_6 = pkt_temp->dup();
			                            Queue2Server[DestServer].insert(pkt_temp_1);
			                            Queue2Server[DestServer].insert(pkt_temp_2);
			                            Queue2Server[DestServer].insert(pkt_temp_3);
			                            Queue2Server[DestServer].insert(pkt_temp_4);
			                            Queue2Server[DestServer].insert(pkt_temp_5);
			                            Queue2Server[DestServer].insert(pkt_temp_6);
	                                       if(pkt_temp->getHopCountNow() == 0) { pkt_temp->setFlag_last(1);}  Queue2Server[DestServer].insert(pkt_temp);
			                             break;
			                          }
			                          case 8:
			                            {
			                                pkt_temp->setPacketSize(64);
			                                 pkt *pkt_temp_1 = pkt_temp->dup();
			                                 pkt *pkt_temp_2 = pkt_temp->dup();
			                                 pkt *pkt_temp_3 = pkt_temp->dup();
			                                 pkt *pkt_temp_4 = pkt_temp->dup();
			                                 pkt *pkt_temp_5 = pkt_temp->dup();
			                                 pkt *pkt_temp_6 = pkt_temp->dup();
			                                 pkt *pkt_temp_7 = pkt_temp->dup();
			                                 Queue2Server[DestServer].insert(pkt_temp_1);
			                                 Queue2Server[DestServer].insert(pkt_temp_2);
			                                 Queue2Server[DestServer].insert(pkt_temp_3);
			                                 Queue2Server[DestServer].insert(pkt_temp_4);
			                                 Queue2Server[DestServer].insert(pkt_temp_5);
			                                 Queue2Server[DestServer].insert(pkt_temp_6);
			                                 Queue2Server[DestServer].insert(pkt_temp_7);
		                                      if(pkt_temp->getHopCountNow() == 0) { pkt_temp->setFlag_last(1);}  Queue2Server[DestServer].insert(pkt_temp);
			                                break;
			                            }
			                            case 9:
			                            {
			                                pkt_temp->setPacketSize(64);
			                                pkt *pkt_temp_1 = pkt_temp->dup();
			                                pkt *pkt_temp_2 = pkt_temp->dup();
			                                pkt *pkt_temp_3 = pkt_temp->dup();
			                                pkt *pkt_temp_4 = pkt_temp->dup();
			                                pkt *pkt_temp_5 = pkt_temp->dup();
			                                pkt *pkt_temp_6 = pkt_temp->dup();
			                                pkt *pkt_temp_7 = pkt_temp->dup();
			                                pkt *pkt_temp_8 = pkt_temp->dup();
			                                Queue2Server[DestServer].insert(pkt_temp_1);
			                                Queue2Server[DestServer].insert(pkt_temp_2);
			                                Queue2Server[DestServer].insert(pkt_temp_3);
			                                Queue2Server[DestServer].insert(pkt_temp_4);
			                                Queue2Server[DestServer].insert(pkt_temp_5);
			                                Queue2Server[DestServer].insert(pkt_temp_6);
			                                Queue2Server[DestServer].insert(pkt_temp_7);
			                                Queue2Server[DestServer].insert(pkt_temp_8);
		                                       if(pkt_temp->getHopCountNow() == 0) { pkt_temp->setFlag_last(1);}  Queue2Server[DestServer].insert(pkt_temp);
			                                break;
			                            }
			                            case 10:
			                            {    pkt_temp->setPacketSize(64);
			                            pkt *pkt_temp_1 = pkt_temp->dup();
			                            pkt *pkt_temp_2 = pkt_temp->dup();
			                            pkt *pkt_temp_3 = pkt_temp->dup();
			                            pkt *pkt_temp_4 = pkt_temp->dup();
			                            pkt *pkt_temp_5 = pkt_temp->dup();
			                            pkt *pkt_temp_6 = pkt_temp->dup();
			                            pkt *pkt_temp_7 = pkt_temp->dup();
			                            pkt *pkt_temp_8 = pkt_temp->dup();
			                            pkt *pkt_temp_9 = pkt_temp->dup();
			                            Queue2Server[DestServer].insert(pkt_temp_1);
			                            Queue2Server[DestServer].insert(pkt_temp_2);
			                            Queue2Server[DestServer].insert(pkt_temp_3);
			                            Queue2Server[DestServer].insert(pkt_temp_4);
			                            Queue2Server[DestServer].insert(pkt_temp_5);
			                            Queue2Server[DestServer].insert(pkt_temp_6);
			                            Queue2Server[DestServer].insert(pkt_temp_7);
			                            Queue2Server[DestServer].insert(pkt_temp_8);
			                            Queue2Server[DestServer].insert(pkt_temp_9);
	                                       if(pkt_temp->getHopCountNow() == 0) { pkt_temp->setFlag_last(1);}  Queue2Server[DestServer].insert(pkt_temp);
			                                break;
			                            }
			                            case 11:
			                            {     pkt_temp->setPacketSize(64);
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
			                            Queue2Server[DestServer].insert(pkt_temp_1);
			                            Queue2Server[DestServer].insert(pkt_temp_2);
			                            Queue2Server[DestServer].insert(pkt_temp_3);
			                            Queue2Server[DestServer].insert(pkt_temp_4);
			                            Queue2Server[DestServer].insert(pkt_temp_5);
			                            Queue2Server[DestServer].insert(pkt_temp_6);
			                            Queue2Server[DestServer].insert(pkt_temp_7);
			                            Queue2Server[DestServer].insert(pkt_temp_8);
			                            Queue2Server[DestServer].insert(pkt_temp_9);
			                            Queue2Server[DestServer].insert(pkt_temp_10);
	                                       if(pkt_temp->getHopCountNow() == 0) { pkt_temp->setFlag_last(1);}  Queue2Server[DestServer].insert(pkt_temp);
			                                 break;
			                            }
			                            case 12:
			                            {
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
			                                Queue2Server[DestServer].insert(pkt_temp_1);
			                                Queue2Server[DestServer].insert(pkt_temp_2);
			                                Queue2Server[DestServer].insert(pkt_temp_3);
			                                Queue2Server[DestServer].insert(pkt_temp_4);
			                                Queue2Server[DestServer].insert(pkt_temp_5);
			                                Queue2Server[DestServer].insert(pkt_temp_6);
			                                Queue2Server[DestServer].insert(pkt_temp_7);
			                                Queue2Server[DestServer].insert(pkt_temp_8);
			                                Queue2Server[DestServer].insert(pkt_temp_9);
			                                Queue2Server[DestServer].insert(pkt_temp_10);
			                                Queue2Server[DestServer].insert(pkt_temp_11);
		                                       if(pkt_temp->getHopCountNow() == 0) { pkt_temp->setFlag_last(1);}  Queue2Server[DestServer].insert(pkt_temp);
			                                break;
			                            }
			                            case 13:
			                            {       pkt_temp->setPacketSize(64);
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
			                            Queue2Server[DestServer].insert(pkt_temp_1);
			                            Queue2Server[DestServer].insert(pkt_temp_2);
			                            Queue2Server[DestServer].insert(pkt_temp_3);
			                            Queue2Server[DestServer].insert(pkt_temp_4);
			                            Queue2Server[DestServer].insert(pkt_temp_5);
			                            Queue2Server[DestServer].insert(pkt_temp_6);
			                            Queue2Server[DestServer].insert(pkt_temp_7);
			                            Queue2Server[DestServer].insert(pkt_temp_8);
			                            Queue2Server[DestServer].insert(pkt_temp_9);
			                            Queue2Server[DestServer].insert(pkt_temp_10);
			                            Queue2Server[DestServer].insert(pkt_temp_11);
			                            Queue2Server[DestServer].insert(pkt_temp_12);
	                                       if(pkt_temp->getHopCountNow() == 0) { pkt_temp->setFlag_last(1);}  Queue2Server[DestServer].insert(pkt_temp);
			                                break;
			                            }
			                            case 14:
			                            {
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
			                              Queue2Server[DestServer].insert(pkt_temp_1);
			                              Queue2Server[DestServer].insert(pkt_temp_2);
			                              Queue2Server[DestServer].insert(pkt_temp_3);
			                              Queue2Server[DestServer].insert(pkt_temp_4);
			                              Queue2Server[DestServer].insert(pkt_temp_5);
			                              Queue2Server[DestServer].insert(pkt_temp_6);
			                              Queue2Server[DestServer].insert(pkt_temp_7);
			                              Queue2Server[DestServer].insert(pkt_temp_8);
			                              Queue2Server[DestServer].insert(pkt_temp_9);
			                              Queue2Server[DestServer].insert(pkt_temp_10);
			                              Queue2Server[DestServer].insert(pkt_temp_11);
			                              Queue2Server[DestServer].insert(pkt_temp_12);
			                              Queue2Server[DestServer].insert(pkt_temp_13);
		                                     if(pkt_temp->getHopCountNow() == 0) { pkt_temp->setFlag_last(1);}  Queue2Server[DestServer].insert(pkt_temp);
			                               break;
			                            }
			                            case 15:
			                           {
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
			                              Queue2Server[DestServer].insert(pkt_temp_1);
			                              Queue2Server[DestServer].insert(pkt_temp_2);
			                              Queue2Server[DestServer].insert(pkt_temp_3);
			                              Queue2Server[DestServer].insert(pkt_temp_4);
			                              Queue2Server[DestServer].insert(pkt_temp_5);
			                              Queue2Server[DestServer].insert(pkt_temp_6);
			                              Queue2Server[DestServer].insert(pkt_temp_7);
			                              Queue2Server[DestServer].insert(pkt_temp_8);
			                              Queue2Server[DestServer].insert(pkt_temp_9);
			                              Queue2Server[DestServer].insert(pkt_temp_10);
			                              Queue2Server[DestServer].insert(pkt_temp_11);
			                              Queue2Server[DestServer].insert(pkt_temp_12);
			                              Queue2Server[DestServer].insert(pkt_temp_13);
			                              Queue2Server[DestServer].insert(pkt_temp_14);
		                                     if(pkt_temp->getHopCountNow() == 0) { pkt_temp->setFlag_last(1);}  Queue2Server[DestServer].insert(pkt_temp);
			                              break;
			                           }
			                          case 16:
			                           {
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

			                               Queue2Server[DestServer].insert(pkt_temp_1);
			                               Queue2Server[DestServer].insert(pkt_temp_2);
			                               Queue2Server[DestServer].insert(pkt_temp_3);
			                               Queue2Server[DestServer].insert(pkt_temp_4);
			                               Queue2Server[DestServer].insert(pkt_temp_5);
			                               Queue2Server[DestServer].insert(pkt_temp_6);
			                               Queue2Server[DestServer].insert(pkt_temp_7);
			                               Queue2Server[DestServer].insert(pkt_temp_8);
			                               Queue2Server[DestServer].insert(pkt_temp_9);
			                               Queue2Server[DestServer].insert(pkt_temp_10);
			                               Queue2Server[DestServer].insert(pkt_temp_11);
			                               Queue2Server[DestServer].insert(pkt_temp_12);
			                               Queue2Server[DestServer].insert(pkt_temp_13);
			                               Queue2Server[DestServer].insert(pkt_temp_14);
			                               Queue2Server[DestServer].insert(pkt_temp_15);
		                                    if(pkt_temp->getHopCountNow() == 0) { pkt_temp->setFlag_last(1);}  Queue2Server[DestServer].insert(pkt_temp);
			                              break;
			                           }
			                          case 17:
			                          {
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
			                              Queue2Server[DestServer].insert(pkt_temp_1);
			                              Queue2Server[DestServer].insert(pkt_temp_2);
			                              Queue2Server[DestServer].insert(pkt_temp_3);
			                              Queue2Server[DestServer].insert(pkt_temp_4);
			                              Queue2Server[DestServer].insert(pkt_temp_5);
			                              Queue2Server[DestServer].insert(pkt_temp_6);
			                              Queue2Server[DestServer].insert(pkt_temp_7);
			                              Queue2Server[DestServer].insert(pkt_temp_8);
			                              Queue2Server[DestServer].insert(pkt_temp_9);
			                              Queue2Server[DestServer].insert(pkt_temp_10);
			                              Queue2Server[DestServer].insert(pkt_temp_11);
			                              Queue2Server[DestServer].insert(pkt_temp_12);
			                              Queue2Server[DestServer].insert(pkt_temp_13);
			                              Queue2Server[DestServer].insert(pkt_temp_14);
			                              Queue2Server[DestServer].insert(pkt_temp_15);
			                              Queue2Server[DestServer].insert(pkt_temp_16);
		                                     if(pkt_temp->getHopCountNow() == 0) { pkt_temp->setFlag_last(1);}  Queue2Server[DestServer].insert(pkt_temp);
			                              break;
			                          }
			                          case 18:
			                          {
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
			                             Queue2Server[DestServer].insert(pkt_temp_1);
			                             Queue2Server[DestServer].insert(pkt_temp_2);
			                             Queue2Server[DestServer].insert(pkt_temp_3);
			                             Queue2Server[DestServer].insert(pkt_temp_4);
			                             Queue2Server[DestServer].insert(pkt_temp_5);
			                             Queue2Server[DestServer].insert(pkt_temp_6);
			                             Queue2Server[DestServer].insert(pkt_temp_7);
			                             Queue2Server[DestServer].insert(pkt_temp_8);
			                             Queue2Server[DestServer].insert(pkt_temp_9);
			                             Queue2Server[DestServer].insert(pkt_temp_10);
			                             Queue2Server[DestServer].insert(pkt_temp_11);
			                             Queue2Server[DestServer].insert(pkt_temp_12);
			                             Queue2Server[DestServer].insert(pkt_temp_13);
			                             Queue2Server[DestServer].insert(pkt_temp_14);
			                             Queue2Server[DestServer].insert(pkt_temp_15);
			                             Queue2Server[DestServer].insert(pkt_temp_16);
			                             Queue2Server[DestServer].insert(pkt_temp_17);
	                                      if(pkt_temp->getHopCountNow() == 0) { pkt_temp->setFlag_last(1);}  Queue2Server[DestServer].insert(pkt_temp);
			                               break;
			                          }
			                          case 19:
			                          {
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
			                           Queue2Server[DestServer].insert(pkt_temp_1);
			                           Queue2Server[DestServer].insert(pkt_temp_2);
			                           Queue2Server[DestServer].insert(pkt_temp_3);
			                           Queue2Server[DestServer].insert(pkt_temp_4);
			                           Queue2Server[DestServer].insert(pkt_temp_5);
			                           Queue2Server[DestServer].insert(pkt_temp_6);
			                           Queue2Server[DestServer].insert(pkt_temp_7);
			                           Queue2Server[DestServer].insert(pkt_temp_8);
			                           Queue2Server[DestServer].insert(pkt_temp_9);
			                           Queue2Server[DestServer].insert(pkt_temp_10);
			                           Queue2Server[DestServer].insert(pkt_temp_11);
			                           Queue2Server[DestServer].insert(pkt_temp_12);
			                           Queue2Server[DestServer].insert(pkt_temp_13);
			                           Queue2Server[DestServer].insert(pkt_temp_14);
			                           Queue2Server[DestServer].insert(pkt_temp_15);
			                           Queue2Server[DestServer].insert(pkt_temp_16);
			                           Queue2Server[DestServer].insert(pkt_temp_17);
			                           Queue2Server[DestServer].insert(pkt_temp_18);
	                                   if(pkt_temp->getHopCountNow() == 0) { pkt_temp->setFlag_last(1);}  Queue2Server[DestServer].insert(pkt_temp);
			                              break;
			                          }
			                          case 20:
			                          {
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
			                          Queue2Server[DestServer].insert(pkt_temp_1);
			                          Queue2Server[DestServer].insert(pkt_temp_2);
			                          Queue2Server[DestServer].insert(pkt_temp_3);
			                          Queue2Server[DestServer].insert(pkt_temp_4);
			                          Queue2Server[DestServer].insert(pkt_temp_5);
			                          Queue2Server[DestServer].insert(pkt_temp_6);
			                          Queue2Server[DestServer].insert(pkt_temp_7);
			                          Queue2Server[DestServer].insert(pkt_temp_8);
			                          Queue2Server[DestServer].insert(pkt_temp_9);
			                          Queue2Server[DestServer].insert(pkt_temp_10);
			                          Queue2Server[DestServer].insert(pkt_temp_11);
			                          Queue2Server[DestServer].insert(pkt_temp_12);
			                          Queue2Server[DestServer].insert(pkt_temp_13);
			                          Queue2Server[DestServer].insert(pkt_temp_14);
			                          Queue2Server[DestServer].insert(pkt_temp_15);
			                          Queue2Server[DestServer].insert(pkt_temp_16);
			                          Queue2Server[DestServer].insert(pkt_temp_17);
			                          Queue2Server[DestServer].insert(pkt_temp_18);
			                          Queue2Server[DestServer].insert(pkt_temp_19);
	                                  if(pkt_temp->getHopCountNow() == 0) { pkt_temp->setFlag_last(1);}  Queue2Server[DestServer].insert(pkt_temp);
			                              break;
			                          }
			                          case 21:
			                          {
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
			                          Queue2Server[DestServer].insert(pkt_temp_1);
			                          Queue2Server[DestServer].insert(pkt_temp_2);
			                          Queue2Server[DestServer].insert(pkt_temp_3);
			                          Queue2Server[DestServer].insert(pkt_temp_4);
			                          Queue2Server[DestServer].insert(pkt_temp_5);
			                          Queue2Server[DestServer].insert(pkt_temp_6);
			                          Queue2Server[DestServer].insert(pkt_temp_7);
			                          Queue2Server[DestServer].insert(pkt_temp_8);
			                          Queue2Server[DestServer].insert(pkt_temp_9);
			                          Queue2Server[DestServer].insert(pkt_temp_10);
			                          Queue2Server[DestServer].insert(pkt_temp_11);
			                          Queue2Server[DestServer].insert(pkt_temp_12);
			                          Queue2Server[DestServer].insert(pkt_temp_13);
			                          Queue2Server[DestServer].insert(pkt_temp_14);
			                          Queue2Server[DestServer].insert(pkt_temp_15);
			                          Queue2Server[DestServer].insert(pkt_temp_16);
			                          Queue2Server[DestServer].insert(pkt_temp_17);
			                          Queue2Server[DestServer].insert(pkt_temp_18);
			                          Queue2Server[DestServer].insert(pkt_temp_19);
			                          Queue2Server[DestServer].insert(pkt_temp_20);
	                                  if(pkt_temp->getHopCountNow() == 0) { pkt_temp->setFlag_last(1);}  Queue2Server[DestServer].insert(pkt_temp);
			                             break;
			                          }
			                          case 22:
			                            {
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
			                            Queue2Server[DestServer].insert(pkt_temp_1);
			                            Queue2Server[DestServer].insert(pkt_temp_2);
			                            Queue2Server[DestServer].insert(pkt_temp_3);
			                            Queue2Server[DestServer].insert(pkt_temp_4);
			                            Queue2Server[DestServer].insert(pkt_temp_5);
			                            Queue2Server[DestServer].insert(pkt_temp_6);
			                            Queue2Server[DestServer].insert(pkt_temp_7);
			                            Queue2Server[DestServer].insert(pkt_temp_8);
			                            Queue2Server[DestServer].insert(pkt_temp_9);
			                            Queue2Server[DestServer].insert(pkt_temp_10);
			                            Queue2Server[DestServer].insert(pkt_temp_11);
			                            Queue2Server[DestServer].insert(pkt_temp_12);
			                            Queue2Server[DestServer].insert(pkt_temp_13);
			                            Queue2Server[DestServer].insert(pkt_temp_14);
			                            Queue2Server[DestServer].insert(pkt_temp_15);
			                            Queue2Server[DestServer].insert(pkt_temp_16);
			                            Queue2Server[DestServer].insert(pkt_temp_17);
			                            Queue2Server[DestServer].insert(pkt_temp_18);
			                            Queue2Server[DestServer].insert(pkt_temp_19);
			                            Queue2Server[DestServer].insert(pkt_temp_20);
			                            Queue2Server[DestServer].insert(pkt_temp_21);
	                                    if(pkt_temp->getHopCountNow() == 0) { pkt_temp->setFlag_last(1);}  Queue2Server[DestServer].insert(pkt_temp);
			                                break;
			                            }
			                            case 23:
			                            {
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
			                            Queue2Server[DestServer].insert(pkt_temp_1);
			                            Queue2Server[DestServer].insert(pkt_temp_2);
			                            Queue2Server[DestServer].insert(pkt_temp_3);
			                            Queue2Server[DestServer].insert(pkt_temp_4);
			                            Queue2Server[DestServer].insert(pkt_temp_5);
			                            Queue2Server[DestServer].insert(pkt_temp_6);
			                            Queue2Server[DestServer].insert(pkt_temp_7);
			                            Queue2Server[DestServer].insert(pkt_temp_8);
			                            Queue2Server[DestServer].insert(pkt_temp_9);
			                            Queue2Server[DestServer].insert(pkt_temp_10);
			                            Queue2Server[DestServer].insert(pkt_temp_11);
			                            Queue2Server[DestServer].insert(pkt_temp_12);
			                            Queue2Server[DestServer].insert(pkt_temp_13);
			                            Queue2Server[DestServer].insert(pkt_temp_14);
			                            Queue2Server[DestServer].insert(pkt_temp_15);
			                            Queue2Server[DestServer].insert(pkt_temp_16);
			                            Queue2Server[DestServer].insert(pkt_temp_17);
			                            Queue2Server[DestServer].insert(pkt_temp_18);
			                            Queue2Server[DestServer].insert(pkt_temp_19);
			                            Queue2Server[DestServer].insert(pkt_temp_20);
			                            Queue2Server[DestServer].insert(pkt_temp_21);
			                            Queue2Server[DestServer].insert(pkt_temp_22);
	                                    if(pkt_temp->getHopCountNow() == 0) { pkt_temp->setFlag_last(1);}  Queue2Server[DestServer].insert(pkt_temp);
			                                break;
			                            }
			                            case 24:
			                            {
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
			                            Queue2Server[DestServer].insert(pkt_temp_1);
			                            Queue2Server[DestServer].insert(pkt_temp_2);
			                            Queue2Server[DestServer].insert(pkt_temp_3);
			                            Queue2Server[DestServer].insert(pkt_temp_4);
			                            Queue2Server[DestServer].insert(pkt_temp_5);
			                            Queue2Server[DestServer].insert(pkt_temp_6);
			                            Queue2Server[DestServer].insert(pkt_temp_7);
			                            Queue2Server[DestServer].insert(pkt_temp_8);
			                            Queue2Server[DestServer].insert(pkt_temp_9);
			                            Queue2Server[DestServer].insert(pkt_temp_10);
			                            Queue2Server[DestServer].insert(pkt_temp_11);
			                            Queue2Server[DestServer].insert(pkt_temp_12);
			                            Queue2Server[DestServer].insert(pkt_temp_13);
			                            Queue2Server[DestServer].insert(pkt_temp_14);
			                            Queue2Server[DestServer].insert(pkt_temp_15);
			                            Queue2Server[DestServer].insert(pkt_temp_16);
			                            Queue2Server[DestServer].insert(pkt_temp_17);
			                            Queue2Server[DestServer].insert(pkt_temp_18);
			                            Queue2Server[DestServer].insert(pkt_temp_19);
			                            Queue2Server[DestServer].insert(pkt_temp_20);
			                            Queue2Server[DestServer].insert(pkt_temp_21);
			                            Queue2Server[DestServer].insert(pkt_temp_22);
			                            Queue2Server[DestServer].insert(pkt_temp_23);
	                                    if(pkt_temp->getHopCountNow() == 0) { pkt_temp->setFlag_last(1);}  Queue2Server[DestServer].insert(pkt_temp);
			                                break;
			                            }
			                            case 25:
			                            {
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
			                             pkt *pkt_temp_24 = pkt_temp->dup();
			                             
			                             Queue2Server[DestServer].insert(pkt_temp_1);
			                             Queue2Server[DestServer].insert(pkt_temp_2);
			                             Queue2Server[DestServer].insert(pkt_temp_3);
			                             Queue2Server[DestServer].insert(pkt_temp_4);
			                             Queue2Server[DestServer].insert(pkt_temp_5);
			                             Queue2Server[DestServer].insert(pkt_temp_6);
			                             Queue2Server[DestServer].insert(pkt_temp_7);
			                             Queue2Server[DestServer].insert(pkt_temp_8);
			                             Queue2Server[DestServer].insert(pkt_temp_9);
			                             Queue2Server[DestServer].insert(pkt_temp_10);
			                             Queue2Server[DestServer].insert(pkt_temp_11);
			                             Queue2Server[DestServer].insert(pkt_temp_12);
			                             Queue2Server[DestServer].insert(pkt_temp_13);
			                             Queue2Server[DestServer].insert(pkt_temp_14);
			                             Queue2Server[DestServer].insert(pkt_temp_15);
			                             Queue2Server[DestServer].insert(pkt_temp_16);
			                             Queue2Server[DestServer].insert(pkt_temp_17);
			                             Queue2Server[DestServer].insert(pkt_temp_18);
			                             Queue2Server[DestServer].insert(pkt_temp_19);
			                             Queue2Server[DestServer].insert(pkt_temp_20);
			                             Queue2Server[DestServer].insert(pkt_temp_21);
			                             Queue2Server[DestServer].insert(pkt_temp_22);
			                             Queue2Server[DestServer].insert(pkt_temp_23);
			                             Queue2Server[DestServer].insert(pkt_temp_24);
										 if(pkt_temp->getHopCountNow() == 0) { pkt_temp->setFlag_last(1);}  Queue2Server[DestServer].insert(pkt_temp);
			                                 break;
			                            }
			                            default:
			                                 break;
			                      }
			                    }
			            }
			        }
			    }
			    else if(pkt_temp->getIndex() == 2){
			        int Queue2ServerIndex = pkt_temp->getDst_Server();
			        if(!Queue2Server[Queue2ServerIndex].empty() == 1){  // not empty
			            pkt * pkt_to_send = (pkt *)Queue2Server[Queue2ServerIndex].pop();

			            //int countNow = pkt_to_send->getHopCountNow();
			            //pkt_to_send->setHopCountNow(countNow+1);

			            send(pkt_to_send,"serverout",Queue2ServerIndex%ToRserverNum);
			            finishtime = gate("serverout",Queue2ServerIndex%ToRserverNum)->getTransmissionChannel()->getTransmissionFinishTime();

			            pkt *pkt2l = new pkt("local_pkt_ToR");
			            pkt2l->setDst_Server(Queue2ServerIndex);
			            pkt2l->setIndex(2);
			            scheduleAt(simTime() + slot_ToR,pkt2l);
			        }
			        else{
			            Queue2Server_busy[Queue2ServerIndex] = false;
			        }

			        delete msg;
			    }
			    else if(pkt_temp->getIndex() == 5){
			        coutserver = pkt_temp->getSrc_Server();
			        msgfromserver2headprocessorfinish[coutserver] = true;
			        FinishserverNum++;
			        EV<<"FinishserverNum is"<<FinishserverNum<<"\n";
			        if(FinishserverNum == ToRserverNum){
			            pkt *finishmsg0 = new pkt();
			            finishmsg0->setIndex(5);
			            for(int i=0;i<=NumTx+NumTx_Core + NumTx_ToR2ToR-1;i++)   // all Tx agre plus core;
			                sendDelayed(finishmsg0->dup(),1e-2,"bufferout",i);

			            delete finishmsg0;
			        }
					HeadProcessorfinishtime = simTime();
			        delete msg;

			    }
			    else
			        delete msg;
	}
}
void HeadProcessor::finish()
{
    cModule *targetModule = getParentModule();
    int tt = targetModule->getIndex();
//    recordScalar("ToR-Send-To---other----",receivedPackets);
    recordScalar("Send-To-ToR-Cell-Number",count2ToR[tt]);
    for(int i=0;i<ToRserverNum;i++){

        Queue2ServerAveragelost = Queue2ServerAveragelost + Queue2Serverlost[ToRserverNum*tt+i];

    }
//    recordScalar("receivedPackets--------",receivedPackets);

    recordScalar("Received-interToR-Cells",numReceived_ToR);
    recordScalar("Received-Total----Cells",numReceived_Total);
    recordScalar("ToR-lost---Queue2server",Queue2ServerAveragelost);
    recordScalar("HeadProcesso-FinishTime",HeadProcessorfinishtime);
    recordScalar("Received-interToR--Pkts",PnumReceived_ToR);
    recordScalar("ToR2ToR-Average-Latency",ToR2ToR_Latency);
    recordScalar("PToR2ToRAverage-Latency",PToR2ToR_Latency);

}


