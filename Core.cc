#include<stdio.h>
#include <string.h>
#include <omnetpp.h>
#include "pkt_m.h"
#include "Optical_m.h"
//bool Core_Channel_busy[256];
using namespace omnetpp;
bool Core_Channel_busy[4160*16];

class CoreY : public cSimpleModule
{
   private:
        simtime_t finish_time;
        int FOSRadix;
        int totalserverNum;
        int ToRserverNum;
        int ClusterToRNum;
        int AllToRNum;
        int AgreRadix;
        int CoreRadix;
        int PreviousToR;
        int NextToR;
        int NumTx;
        int NumTx_Core;
        double slot_OPS;
        cQueue SendLaterCore;
        int Core_queueAveragelost ;

        bool test;

        simtime_t delayCore;
        simtime_t delayforACK;

        simtime_t dealy_MSG;
        simtime_t dealy_ACK;

        pkt *local_CorePkt_MSG;
        pkt *slot_OPS_MSG;

        int OutputPortNum;
        int InputPortNum;

        int OPSPortNum;
        int CurrentOPSIndex;

        int GroupToRIndex;
        int TxIndex;

        int GroupToRCore;
   protected:
        virtual void initialize();
        virtual void handleMessage(cMessage *msg);
//        virtual void finish();

   public:
        virtual ~CoreY();
};

Define_Module(CoreY);

CoreY::~CoreY(){
    cancelAndDelete(local_CorePkt_MSG);
    cancelAndDelete(slot_OPS_MSG);
    while(!SendLaterCore.empty()){
        Optical *Optical_pkt_temp = check_and_cast<Optical *>(SendLaterCore.pop());
        int optNum = Optical_pkt_temp->getOptical_num();
        for(int i=0;i<optNum;i++){
            switch(i){
                case 0:
                    Optical_pkt_temp->removeMessage(Optical_pkt_temp->getPkt1());
                    delete Optical_pkt_temp->getPkt1();
                    break;
                case 1:
                    Optical_pkt_temp->removeMessage(Optical_pkt_temp->getPkt2());
                    delete Optical_pkt_temp->getPkt2();
                    break;
                case 2:
                    Optical_pkt_temp->removeMessage(Optical_pkt_temp->getPkt3());
                    delete Optical_pkt_temp->getPkt3();
                    break;
                case 3:
                    Optical_pkt_temp->removeMessage(Optical_pkt_temp->getPkt4());
                    delete Optical_pkt_temp->getPkt4();
                    break;
                case 4:
                    Optical_pkt_temp->removeMessage(Optical_pkt_temp->getPkt5());
                    delete Optical_pkt_temp->getPkt5();
                    break;
				case 5:
                    Optical_pkt_temp->removeMessage(Optical_pkt_temp->getPkt6());
                    delete Optical_pkt_temp->getPkt6();
                    break;
                case 6:
                    Optical_pkt_temp->removeMessage(Optical_pkt_temp->getPkt7());
                    delete Optical_pkt_temp->getPkt7();
                    break;			
                case 7:
                    Optical_pkt_temp->removeMessage(Optical_pkt_temp->getPkt8());
                    delete Optical_pkt_temp->getPkt8();
                    break;
                case 8:
                    Optical_pkt_temp->removeMessage(Optical_pkt_temp->getPkt9());
                    delete Optical_pkt_temp->getPkt9();
                    break;
                case 9:
                    Optical_pkt_temp->removeMessage(Optical_pkt_temp->getPkt10());
                    delete Optical_pkt_temp->getPkt10();
                    break;
                case 10:
                    Optical_pkt_temp->removeMessage(Optical_pkt_temp->getPkt11());
                    delete Optical_pkt_temp->getPkt11();
                    break;
                case 11:
                    Optical_pkt_temp->removeMessage(Optical_pkt_temp->getPkt12());
                    delete Optical_pkt_temp->getPkt12();
                    break;
                case 12:
                    Optical_pkt_temp->removeMessage(Optical_pkt_temp->getPkt13());
                    delete Optical_pkt_temp->getPkt13();
                    break;
                case 13:
                    Optical_pkt_temp->removeMessage(Optical_pkt_temp->getPkt14());
                    delete Optical_pkt_temp->getPkt14();
                    break;
                case 14:
                    Optical_pkt_temp->removeMessage(Optical_pkt_temp->getPkt15());
                    delete Optical_pkt_temp->getPkt15();
                    break;
				case 15:
                    Optical_pkt_temp->removeMessage(Optical_pkt_temp->getPkt16());
                    delete Optical_pkt_temp->getPkt16();
                    break;
                case 16:
                    Optical_pkt_temp->removeMessage(Optical_pkt_temp->getPkt17());
                    delete Optical_pkt_temp->getPkt17();
                    break;			
                case 17:
                    Optical_pkt_temp->removeMessage(Optical_pkt_temp->getPkt18());
                    delete Optical_pkt_temp->getPkt18();
                    break;
                case 18:
                    Optical_pkt_temp->removeMessage(Optical_pkt_temp->getPkt19());
                    delete Optical_pkt_temp->getPkt19();
                    break;
                case 19:
                    Optical_pkt_temp->removeMessage(Optical_pkt_temp->getPkt20());
                    delete Optical_pkt_temp->getPkt20();
                    break;
                case 20:
                    Optical_pkt_temp->removeMessage(Optical_pkt_temp->getPkt21());
                    delete Optical_pkt_temp->getPkt21();
                    break;
                case 21:
                    Optical_pkt_temp->removeMessage(Optical_pkt_temp->getPkt22());
                    delete Optical_pkt_temp->getPkt22();
                    break;
                case 22:
                    Optical_pkt_temp->removeMessage(Optical_pkt_temp->getPkt23());
                    delete Optical_pkt_temp->getPkt23();
                    break;
                case 23:
                    Optical_pkt_temp->removeMessage(Optical_pkt_temp->getPkt24());
                    delete Optical_pkt_temp->getPkt24();
                    break;
                case 24:
                    Optical_pkt_temp->removeMessage(Optical_pkt_temp->getPkt25());
                    delete Optical_pkt_temp->getPkt25();
                    break;	
            }
        }
        delete Optical_pkt_temp;
    }
}

void CoreY::initialize()
{
    EV << "Core initialize ";
    FOSRadix = par("FOSRadix");
    Core_queueAveragelost = 0;

    totalserverNum = par("ServerNum");
    ToRserverNum = par("ToRDownRadix");
    AllToRNum = totalserverNum/ToRserverNum;
    ClusterToRNum = sqrt(AllToRNum);
    AgreRadix = par("AgreRadix");
    CoreRadix = par("CoreRadix");
    NumTx_Core=par("NumTx_Core");
    NumTx=par("NumTx");
    GroupToRCore=par("GroupToRCore");
//    OPSPortNum = AgreRadix;
    OPSPortNum = CoreRadix*NumTx_Core;
//    delayCore = 20e-9;

    delayforACK = 19e-9;

    dealy_MSG = 20e-9;
//    dealy_ACK = 1e-9;
    test = false;

    slot_OPS = 10e-9;

    OutputPortNum = 0;
    InputPortNum = 0;

    GroupToRIndex = 0;
    TxIndex = 0;
    if(getIndex()==0){
        for(int j=0;j<4160*16;j++){
            Core_Channel_busy[j] = false;
        }
    }

//    Core_Channel_busy = false;
    local_CorePkt_MSG = new pkt ("local_CorePkt_MSG");
    local_CorePkt_MSG->setKind(0);

    slot_OPS_MSG = new pkt ("slot_OPS_MSG");
    slot_OPS_MSG->setKind(400);

    CurrentOPSIndex = 0;
}

void CoreY::handleMessage(cMessage *msg)
{
//    pkt *pkt_temp =  check_and_cast<pkt*>(msg);

    if(msg->getKind() == 1000){

        //PreviousToR = pkt_temp->getHop(pkt_temp->getHopCountNow() - 1);
        //NextToR = pkt_temp->getHop(pkt_temp->getHopCountNow() + 1);
        Optical *Optical_pkt_temp =  check_and_cast<Optical *>(msg);

        PreviousToR = Optical_pkt_temp->getPreviousToR() ;

        NextToR = Optical_pkt_temp->getNextToR() ;

        TxIndex = Optical_pkt_temp->getTxIndex();
        GroupToRIndex = Optical_pkt_temp->getGroupToRIndex();

        InputPortNum  = (PreviousToR%(FOSRadix*FOSRadix)/CoreRadix)*NumTx_Core + TxIndex;

        OutputPortNum = (NextToR%(FOSRadix*FOSRadix)/CoreRadix)*NumTx_Core + (PreviousToR%(FOSRadix*FOSRadix)/CoreRadix)/GroupToRCore;
        CurrentOPSIndex = (PreviousToR/(CoreRadix*CoreRadix))*CoreRadix +  (PreviousToR%(CoreRadix*CoreRadix) )%CoreRadix;
        slot_OPS_MSG->setDst_ToR(CurrentOPSIndex*OPSPortNum + OutputPortNum); //set output port
        scheduleAt(simTime() + slot_OPS,slot_OPS_MSG->dup() );
        int optNum = Optical_pkt_temp->getOptical_num();
        /////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////////////////////
        if(Core_Channel_busy[CurrentOPSIndex*OPSPortNum +OutputPortNum] == false){

            Core_Channel_busy[CurrentOPSIndex*OPSPortNum +OutputPortNum] = true;

            SendLaterCore.insert(Optical_pkt_temp);

            pkt *AckFromCore = new pkt("AckFromCore");

            AckFromCore->setSrc_ToR(PreviousToR);
            AckFromCore->setDst_ToR(NextToR);
            AckFromCore->setTxIndex(TxIndex);
            AckFromCore->setGroupToRIndex(GroupToRIndex);
            AckFromCore->setIndex(10);
            AckFromCore->setKind(10);// from Agre, ACK Index is 8;
            AckFromCore->setRetransmission(0);
            AckFromCore->setSchedulingPriority(0);
            AckFromCore->setOptNum(optNum);
            sendDelayed(AckFromCore,delayforACK,"ackout",InputPortNum);
            gate("ackout",InputPortNum)->getTransmissionChannel()->forceTransmissionFinishTime(simTime());
            finish_time= gate("ackout",InputPortNum)->getTransmissionChannel()->getTransmissionFinishTime();

            local_CorePkt_MSG->setDst_ToR(NextToR);
            scheduleAt(simTime() + dealy_MSG,local_CorePkt_MSG->dup());

        }
        else{

             delete msg;

             pkt *AckFromCore  = new pkt("local_pkt_ToR");
             AckFromCore ->setSrc_ToR(PreviousToR);
             AckFromCore ->setDst_ToR(NextToR);
             AckFromCore->setTxIndex(TxIndex);
             AckFromCore->setGroupToRIndex(GroupToRIndex);
             AckFromCore ->setIndex(10);
             AckFromCore->setKind(10);
             AckFromCore ->setRetransmission(1);
             AckFromCore ->setSchedulingPriority(0);
             AckFromCore->setOptNum(optNum);

             sendDelayed(AckFromCore ,delayforACK,"ackout",InputPortNum);
             gate("ackout",InputPortNum)->getTransmissionChannel()->forceTransmissionFinishTime(simTime());
        }
    }

    else if(msg->getKind() == 0){


        if(SendLaterCore.length() > 0){
            Optical * Optical_pkt_to_send = (Optical *)SendLaterCore.pop();  //1111


            PreviousToR = Optical_pkt_to_send->getPreviousToR()%(FOSRadix*FOSRadix) ;

            NextToR = Optical_pkt_to_send->getNextToR() %(FOSRadix*FOSRadix);

            TxIndex = Optical_pkt_to_send->getTxIndex();
            GroupToRIndex = Optical_pkt_to_send->getGroupToRIndex();


            InputPortNum  = (PreviousToR/CoreRadix)*NumTx_Core + TxIndex;

            OutputPortNum = (NextToR/CoreRadix)*NumTx_Core + (PreviousToR/AgreRadix)/GroupToRCore;

//            int countNow = Optical_pkt_to_send->getPkt1()->getHopCountNow();
//            EV<< "in FOS , countNow is" <<countNow;
//            EV<< "in FOS , HopNow is" <<Optical_pkt_to_send->getPkt1()->getHop(countNow);

//            AGG_Channel_busy[OutputPortNum] = false;  // set to false???

            int optNum = Optical_pkt_to_send->getOptical_num();
            for(int i=0;i<optNum;i++){
                switch(i){
                    case 0:
                    {
                        int countNow1 =  Optical_pkt_to_send->getPkt1()->getHopCountNow();
                        Optical_pkt_to_send->getPkt1()->setHopCountNow(countNow1+1);
                    }
                        break;
                    case 1:
                    {
                        int countNow2 =  Optical_pkt_to_send->getPkt2()->getHopCountNow();
                        Optical_pkt_to_send->getPkt2()->setHopCountNow(countNow2+1);
                    }
                        break;
                    case 2:
                    {
                        int countNow3 =  Optical_pkt_to_send->getPkt3()->getHopCountNow();
                        Optical_pkt_to_send->getPkt3()->setHopCountNow(countNow3+1);
                    }
                        break;
                    case 3:
                    {
                        int countNow4 =  Optical_pkt_to_send->getPkt4()->getHopCountNow();
                        Optical_pkt_to_send->getPkt4()->setHopCountNow(countNow4+1);
                    }
                        break;
                    case 4:
                    {
                        int countNow5 =  Optical_pkt_to_send->getPkt5()->getHopCountNow();
                        Optical_pkt_to_send->getPkt5()->setHopCountNow(countNow5+1);
                    }
                        break;
                    case 5:
                    {
                        int countNow6 =  Optical_pkt_to_send->getPkt6()->getHopCountNow();
                        Optical_pkt_to_send->getPkt6()->setHopCountNow(countNow6+1);
                    }
					    break;
                    case 6:
                    {
                        int countNow7 =  Optical_pkt_to_send->getPkt7()->getHopCountNow();
                        Optical_pkt_to_send->getPkt7()->setHopCountNow(countNow7+1);
                    }
                        break;				
                    case 7:
                    {
                        int countNow8 =  Optical_pkt_to_send->getPkt8()->getHopCountNow();
                        Optical_pkt_to_send->getPkt8()->setHopCountNow(countNow8+1);
                    }
                        break;					
                    case 8:
                    {
                        int countNow9 =  Optical_pkt_to_send->getPkt9()->getHopCountNow();
                        Optical_pkt_to_send->getPkt9()->setHopCountNow(countNow9+1);
                    }
                        break;					
                    case 9:
                    {
                        int countNow10=  Optical_pkt_to_send->getPkt10()->getHopCountNow();
                        Optical_pkt_to_send->getPkt10()->setHopCountNow(countNow10+1);
                    }	
                        break;					
                    case 10:
                    {
                        int countNow11 =  Optical_pkt_to_send->getPkt11()->getHopCountNow();
                        Optical_pkt_to_send->getPkt11()->setHopCountNow(countNow11+1);
                    }
                        break;
                    case 11:
                    {
                        int countNow12 =  Optical_pkt_to_send->getPkt12()->getHopCountNow();
                        Optical_pkt_to_send->getPkt12()->setHopCountNow(countNow12+1);
                    }
                        break;
                    case 12:
                    {
                        int countNow13 =  Optical_pkt_to_send->getPkt13()->getHopCountNow();
                        Optical_pkt_to_send->getPkt13()->setHopCountNow(countNow13+1);
                    }
                        break;
                    case 13:
                    {
                        int countNow14 =  Optical_pkt_to_send->getPkt14()->getHopCountNow();
                        Optical_pkt_to_send->getPkt14()->setHopCountNow(countNow14+1);
                    }
                        break;
                    case 14:
                    {
                        int countNow15 =  Optical_pkt_to_send->getPkt15()->getHopCountNow();
                        Optical_pkt_to_send->getPkt15()->setHopCountNow(countNow15+1);
                    }
                        break;
                    case 15:
                    {
                        int countNow16 =  Optical_pkt_to_send->getPkt16()->getHopCountNow();
                        Optical_pkt_to_send->getPkt16()->setHopCountNow(countNow16+1);
                    }
                        break;					
                    case 16:
                    {
                        int countNow17 =  Optical_pkt_to_send->getPkt17()->getHopCountNow();
                        Optical_pkt_to_send->getPkt17()->setHopCountNow(countNow17+1);
                    }
                        break;					
                    case 17:
                    {
                        int countNow18 =  Optical_pkt_to_send->getPkt18()->getHopCountNow();
                        Optical_pkt_to_send->getPkt18()->setHopCountNow(countNow18+1);
                    }
                        break;					
                    case 18:
                    {
                        int countNow19 =  Optical_pkt_to_send->getPkt19()->getHopCountNow();
                        Optical_pkt_to_send->getPkt19()->setHopCountNow(countNow19+1);
                    }
                        break;					
                    case 19:
                    {
                        int countNow20=  Optical_pkt_to_send->getPkt20()->getHopCountNow();
                        Optical_pkt_to_send->getPkt20()->setHopCountNow(countNow20+1);
                    }	
                        break;					
                    case 20:
                    {
                        int countNow21 =  Optical_pkt_to_send->getPkt21()->getHopCountNow();
                        Optical_pkt_to_send->getPkt21()->setHopCountNow(countNow21+1);
                    }
                        break;
                    case 21:
                    {
                        int countNow22 =  Optical_pkt_to_send->getPkt22()->getHopCountNow();
                        Optical_pkt_to_send->getPkt22()->setHopCountNow(countNow22+1);
                    }
                        break;
                    case 22:
                    {
                        int countNow23 =  Optical_pkt_to_send->getPkt23()->getHopCountNow();
                        Optical_pkt_to_send->getPkt23()->setHopCountNow(countNow23+1);
                    }
                        break;
                    case 23:
                    {
                        int countNow24 =  Optical_pkt_to_send->getPkt24()->getHopCountNow();
                        Optical_pkt_to_send->getPkt24()->setHopCountNow(countNow24+1);
                    }
                        break;
                    case 24:
                    {
                        int countNow25 =  Optical_pkt_to_send->getPkt25()->getHopCountNow();
                        Optical_pkt_to_send->getPkt25()->setHopCountNow(countNow25+1);
                    }
                        break;
                }
            }

            send(Optical_pkt_to_send,"out",OutputPortNum);

            finish_time= gate("out",OutputPortNum)->getTransmissionChannel()->getTransmissionFinishTime();
            EV<<"For Core, msg finish time is "<< finish_time <<"\n";

            delete msg;
        }
    }
    else if(msg->getKind() == 400){
        pkt *pkt_temp =  check_and_cast<pkt*>(msg);
        int kk = pkt_temp->getDst_ToR();
        Core_Channel_busy[kk] = false;  // set to false
        delete msg;
    }


}
