#include<stdio.h>
#include <string.h>
#include <omnetpp.h>
#include "pkt_m.h"
#include "Optical_m.h"

using namespace omnetpp;
bool AGG_Channel_busy[4160*16];
using namespace omnetpp;

class AgreY : public cSimpleModule
{
   private:
        int FOSRadix;
        simtime_t finish_time;
        int totalserverNum;
        int ToRserverNum;
        int ClusterToRNum;
        int AllToRNum;
        int Agre_queueAveragelost;
        int AgreRadix;
        int GroupToR;
        int GroupToRIndex;
        int PreviousToR;
        int NextToR;
        int TxIndex;
        int OutputPortNum;
        int InputPortNum;
        int NumTx;  // total Tx num
        double slot_OPS;
        int OPSPortNum;
        int CurrentOPSIndex;

//        bool AGG_Channel_busy[16*4];

        simtime_t delayforACK;
        simtime_t dealy_MSG;

        cQueue SendLaterAgre;

        pkt *local_AgrePkt_MSG;
        pkt *slot_OPS_MSG;

   protected:
    // The following redefined virtual function holds the algorithm.
        virtual void initialize();
        virtual void handleMessage(cMessage *msg);

   public:
        virtual ~AgreY();

};

Define_Module(AgreY);

AgreY::~AgreY(){
    cancelAndDelete(local_AgrePkt_MSG);
    cancelAndDelete(slot_OPS_MSG);
    while(!SendLaterAgre.empty()){
        Optical *Optical_pkt_temp = check_and_cast<Optical *>(SendLaterAgre.pop());
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

void AgreY::initialize()
{
    Agre_queueAveragelost = 0;
    FOSRadix = par("FOSRadix");
    totalserverNum = par("ServerNum");
    ToRserverNum = par("ToRDownRadix");
    AllToRNum = totalserverNum/ToRserverNum;
    //ClusterToRNum = sqrt(AllToRNum);
    ClusterToRNum = par("AgreRadix");
    NumTx=par("NumTx");
    AgreRadix = par("AgreRadix");
    GroupToR = par("GroupToR");

    OPSPortNum = AgreRadix*NumTx;
    CurrentOPSIndex = 0;

    delayforACK = 20e-9;

    slot_OPS = 10e-9;

    dealy_MSG = 20e-9;

    OutputPortNum = 0;
    InputPortNum = 0;

    if(getIndex()==0){
        for(int j=0;j<4160*16;j++){
            AGG_Channel_busy[j] = false;
        }
    }

    local_AgrePkt_MSG = new pkt ("local_AgrePkt_MSG");
    local_AgrePkt_MSG->setKind(0);

    slot_OPS_MSG = new pkt ("slot_OPS_MSG");
    slot_OPS_MSG->setKind(200);
}

void AgreY::handleMessage(cMessage *msg)
{
//    pkt *pkt_temp =  check_and_cast<pkt*>(msg);

    if(msg->getKind() == 1000){    // Index = 1, that is packet type

		Optical *Optical_pkt_temp =  check_and_cast<Optical *>(msg);
//        PreviousToR = pkt_temp->getHop(pkt_temp->getHopCountNow() - 1);

//        NextToR = pkt_temp->getHop(pkt_temp->getHopCountNow() + 1);
//		int xoptnum1 = Optical_pkt_temp->getOptical_num();
        PreviousToR = Optical_pkt_temp->getPreviousToR() ;

        NextToR = Optical_pkt_temp->getNextToR();

        TxIndex = Optical_pkt_temp->getTxIndex();
        GroupToRIndex = Optical_pkt_temp->getGroupToRIndex();

        InputPortNum  = (PreviousToR%(FOSRadix*FOSRadix)%AgreRadix)*NumTx + TxIndex;

        OutputPortNum = (NextToR%(FOSRadix*FOSRadix)%AgreRadix)*NumTx + (PreviousToR%(FOSRadix*FOSRadix)%AgreRadix)/GroupToR;

        CurrentOPSIndex = PreviousToR/AgreRadix;
        slot_OPS_MSG->setDst_ToR(CurrentOPSIndex*OPSPortNum + OutputPortNum); //set output port in total
        scheduleAt(simTime() + slot_OPS,slot_OPS_MSG->dup() );

      //  EV<< "received a packet from" <<pkt_temp->getSrc_Server() <<" to "<<pkt_temp->getDst_Server()<<"\n";
//
 //       EV<<"OutputPortNum is"<<OutputPortNum <<"InputPortNum is" <<InputPortNum <<"TxIndex is" <<TxIndex;
        int optNum = Optical_pkt_temp->getOptical_num();

        /////////////////////////////////////////////////////////////////////////////////////////////////////
        //cModule *targetModule = getParentModule();
        // int tt = getIndex();
        /////////////////////////////////////////////////////////////////////////////////////////////////////

        if( AGG_Channel_busy[CurrentOPSIndex*OPSPortNum +OutputPortNum] == false ){
//        if(gate("out",NextToR%ClusterToRNum)->getTransmissionChannel()->isBusy() == false){

            AGG_Channel_busy[CurrentOPSIndex*OPSPortNum +OutputPortNum] = true;

            SendLaterAgre.insert(Optical_pkt_temp);

            pkt *AckFromAgre = new pkt("AckFromAgre");

            AckFromAgre->setSrc_ToR(PreviousToR);
            AckFromAgre->setDst_ToR(NextToR);
            AckFromAgre->setTxIndex(TxIndex);
            AckFromAgre->setGroupToRIndex(GroupToRIndex);
            AckFromAgre->setIndex(8);                     // from Agre, ACK Index is 8;
            AckFromAgre->setKind(8);// from Agre, ACK Index is 8;
            AckFromAgre->setRetransmission(0);
            AckFromAgre->setSchedulingPriority(0);
            AckFromAgre->setOptNum(optNum);
            sendDelayed(AckFromAgre,delayforACK,"ackout",InputPortNum);

            gate("ackout",InputPortNum)->getTransmissionChannel()->forceTransmissionFinishTime(simTime());

            finish_time= gate("ackout",InputPortNum)->getTransmissionChannel()->getTransmissionFinishTime();
    //        EV<<"For Agre, ack finish time is "<< finish_time <<"\n";
///////////////
//            EV<<"ACK = 0"<<"Send Packet is "<<pkt_temp->getSrc_Server()<<" to "<<pkt_temp->getDst_Server()<<"\n";
 //           EV<<"ToR "<<pkt_temp->getSrc_ToR()<<" to "<<"ToR "<< pkt_temp->getDst_ToR()<<"\n";
            scheduleAt(simTime() + dealy_MSG,local_AgrePkt_MSG->dup());
        }
        else{

//             EV<<"ACK = 1"<<"Send Packet is "<<pkt_temp->getSrc_Server()<<" to "<<pkt_temp->getDst_Server()<<"\n";

             pkt *AckFromAgre = new pkt("AckFromAgre");
             AckFromAgre->setSrc_ToR(PreviousToR);
             AckFromAgre->setDst_ToR(NextToR);
             AckFromAgre->setTxIndex(TxIndex);
             AckFromAgre->setGroupToRIndex(GroupToRIndex);
             AckFromAgre->setIndex(8);
             AckFromAgre->setKind(8);
             AckFromAgre->setRetransmission(1);
             AckFromAgre->setSchedulingPriority(0);
             AckFromAgre->setOptNum(optNum);
//             pkt8l->setByteLength(1);

          //   EV<<"ACK = 1"<<"Send Packet is "<<pkt_temp->getSrc_Server()<<" to "<<pkt_temp->getDst_Server()<<"\n";
   //          EV<<"ToR "<<pkt_temp->getSrc_ToR()<<" to "<<"ToR "<< pkt_temp->getDst_ToR()<<"\n";
             sendDelayed(AckFromAgre,delayforACK,"ackout",InputPortNum);
             gate("ackout",InputPortNum)->getTransmissionChannel()->forceTransmissionFinishTime(simTime());  // not influence the normal packet transmission
            //For transmission channels: Forcibly overwrites the finish time of the current transmission in the channel (see getTransmissionFinishTime()).
            // This method is a crude device that allows for implementing aborting transmissions; it is not needed for normal packet transmissions.
            //Calling this method with the current simulation time will allow you to immediately send another packet on the channel without the channel reporting error due to its being busy.
            //   Note that this call does NOT affect the delivery of the packet being transmitted: the packet object is delivered to the target module at the time it would without the call to this method.
            //The sender needs to inform the target module in some other way that the transmission was aborted and the packet should be treated accordingly (i.e. discarded as incomplete);
            // for example by sending an out-of-band cMessage that the receiver has to understand.
             delete msg;
        }
    }
    else if(msg->getKind() == 0){

        if(SendLaterAgre.length() > 0){

            Optical * Optical_pkt_to_send = (Optical *)SendLaterAgre.pop();

            PreviousToR = Optical_pkt_to_send->getPreviousToR()%(FOSRadix*FOSRadix) ;

            NextToR = Optical_pkt_to_send->getNextToR()%(FOSRadix*FOSRadix) ;

            TxIndex = Optical_pkt_to_send->getTxIndex();
            GroupToRIndex = Optical_pkt_to_send->getGroupToRIndex();


            InputPortNum  = (PreviousToR%AgreRadix)*NumTx + TxIndex;

            OutputPortNum = (NextToR%AgreRadix)*NumTx + (PreviousToR%AgreRadix)/GroupToR;


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
         //   EV<<"NextToR is"<< NextToR<<"\n";

          //  EV<<"pkt_to_send detail"<<pkt_to_send<<"\n";

            send(Optical_pkt_to_send,"out",OutputPortNum);

            finish_time= gate("out",OutputPortNum)->getTransmissionChannel()->getTransmissionFinishTime();
     //       EV<<"For Agre, msg finish time is "<< finish_time <<"\n";

            delete msg;
        }

    }

    else if(msg->getKind() == 200){
        pkt *pkt_temp =  check_and_cast<pkt*>(msg);
        int kk = pkt_temp->getDst_ToR();
        AGG_Channel_busy[kk] = false;  // set to false
        delete msg;
    }
}







