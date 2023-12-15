#include <stdio.h>
#include <string.h>
#include <omnetpp.h>
#include <fstream>
#include "pkt_m.h"
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <cstring>
#include "Optical_m.h"

//using namespace std;
using namespace omnetpp;

int count2server[166400];
bool msgfromserverfinish[166400];

//bool finishsimulation[10240]; //totalserverNum

class ServerY : public cSimpleModule
{
    private:

    int AgreNum;
    int FOSRadix;
    int FraudClusterToRNum;
    double dataRate;
    simtime_t finish_time;  // timeout
    long int msg_count,z;
    double loadServer;
    long int PnumReceived_ToR2ToR;
    long int PnumReceived_intraToR;
    long int numReceived_intraToR;
    simtime_t Ptime_TOR2TORlatency;
    simtime_t Ptime_intraTORlatency;

    long int numSent_Server;
    long int numReceived_Server;

    double previoustime;
    double lastarrivaltime;
    double slot0;
    double ontime;
    double prevontime;

    int NumberOfServer;

    int totalserverNum;
    int RealTotalserverNum;
    int ToRserverNum;

    int ClusterToRNum;
    int OPSquareToRNum;
    int AllToRNum;
    int SendPacketToR2ToR;
    int dest;
    simtime_t Ptime_latency;

    simtime_t latency_intraTOR;
    simtime_t time_latency_intraTOR;
    simtime_t latency_TOR2TOR;
    simtime_t time_latency_TOR2TOR;
    simtime_t time_latency;

    simtime_t latency;

    simtime_t nextPeriod;
    simtime_t noTxTime;                 //Time between packets
    simtime_t nextSchedule;

    double TxBufferTime;
    double PreambleInsertdelay;
    double BurstProcessDelay;

    int meanBurstSize;
    int packetsInPeriod;

    int packetLength;
    int minPacketLength;                            //Packet minimum length (in bytes)
    int maxPacketLength;                        //Packet maximum length (in bytes)

    int sentPackets;                        //Number of sent packets
    double sentBytes;                       //Number of sent bytes

    int offPeriods;
    int onPeriods;
    bool lastTX;                        //Whether last was a packet (true) or no transmission (false)
    bool sendThisPacket;                    //Next decision is to send a packet or not

    double paretoMin;                                           //Pareto shape minimum value
    double paretoMax;                                           //Pareto shape maximum value
    double paretoShape;                                         //Pareto function shape (a)

    double linkDataRate;                                        //System datarate to take packets transmission times into account
    double averageBurstSize;
    double averageBurstBytes;

    double simMeanPacketSize;                   //Simulation mean packet size
    double simLoad;                         //Simulation mean load
    int bursts;
    int CellSize;
    int numReceived_ToR2ToR;
    int SendPacketinToR;

    pkt *local_pkt;

//    int linkDataRate;
    public:
        ServerY();
        virtual ~ServerY();

    protected:
    cOutVector endToEndDelayVec;
    // The following redefined virtual function holds the algorithm.
    virtual pkt *generateMessage(int dest, int package_size);
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
    virtual void updateDisplay();
    virtual void finish();
    virtual int destination();
    virtual int CustomCDF1(int min, int max);
    virtual void updateStats();
};

Define_Module(ServerY);

ServerY::ServerY(){

      local_pkt = NULL;
}

ServerY::~ServerY(){

      cancelAndDelete(local_pkt);
}

void ServerY::initialize()
{
    endToEndDelayVec.setName("end-To-EndDelayVec");
    RealTotalserverNum = par("ServerNum");
    FOSRadix = par("FOSRadix");
    numReceived_intraToR = 0;
    PnumReceived_ToR2ToR  = 0;
    PnumReceived_intraToR  = 0;
    Ptime_TOR2TORlatency  = 0;
    Ptime_intraTORlatency  = 0;

    SendPacketToR2ToR = 0;
    numSent_Server = 0;
    numReceived_Server = 0;

    nextSchedule = 0;
    nextPeriod = 0;

    meanBurstSize           = par("meanBurstSize");
    paretoMin = 1.5;
    paretoMax = 20;
    packetsInPeriod = 0;
    noTxTime = 0;                               //Time between packets
    lastTX = true;                              //Whether last was a packet (true) or no transmission (false)
    sendThisPacket = false;
    offPeriods = 0;
    onPeriods = 0;

    sentPackets = 0;                            //Number of sent packets
    sentBytes = 0;                              //Number of sent bytes
    simMeanPacketSize = 0;                                                  //Simulation mean packet size
    simLoad = 0;                                //Simulation load
    bursts = 1;

    packetLength = 0;
    minPacketLength         =   64;                        //Packet minimum length (in bytes)
    maxPacketLength         =  1500;                      //Packet maximum length (in bytes)
    paretoShape = uniform(paretoMin,paretoMax);

    linkDataRate             = par("linkDataRate");

    loadServer = par("load");
    totalserverNum = par("ServerNum");
    ToRserverNum = par("ToRDownRadix");
    AllToRNum = totalserverNum/ToRserverNum;

//    FraudClusterToRNum = sqrt(FOSRadix*FOSRadix*FOSRadix);  //in order to keep the traffic distribution consistent with 2 layers, it is 16 indeed.
    FraudClusterToRNum = par("FOSRadix");
    ClusterToRNum = par("FOSRadix");


    OPSquareToRNum = ClusterToRNum*ClusterToRNum;

    AgreNum = ClusterToRNum*(OPSquareToRNum+1);
    WATCH(numSent_Server);
    WATCH(numReceived_Server);

    time_latency = 0;
    Ptime_latency = 0;
    latency = 0;

    slot0 = 12e-6;

    numReceived_ToR2ToR = 0;
	
	SendPacketinToR = 0;
    CellSize = par("CellSize");
    PreambleInsertdelay = par("PreambleInsertdelay");
    BurstProcessDelay = par("BurstProcessDelay");
    TxBufferTime = CellSize*8/linkDataRate;

    if(getIndex()==0){
        for(int i=0;i<166400;i++){
            count2server[i] = 0;
            msgfromserverfinish[i] = false;
        }
    }

    msg_count = par("SimulatedPacketNum");

    WATCH (msg_count);
    // counter=10;

    local_pkt = new pkt ("local_pkt");
    local_pkt->setIndex(0);
  //  if( getIndex() >= 120 && getIndex() < 160 )
        scheduleAt(simTime(),local_pkt);                   // local pkt for
}

void ServerY::handleMessage(cMessage *msg)
{
    pkt *temp_msg = check_and_cast<pkt *>(msg);
    if(temp_msg->getIndex()==0){
        if (msg_count>0){

            updateStats();

            if(sentPackets == 0)
                simMeanPacketSize = CustomCDF1(minPacketLength, maxPacketLength);
            EV<<"msg_counter value is :"<<msg_count<<"\n";
           // double p = dblrand();
//            double p = uniform(0,1);
//            if (p < loadServer ){
                if(sentPackets == 0 && offPeriods == 0){
                    sendThisPacket = false;
                    noTxTime= uniform((simtime_t)0,(simtime_t)((meanBurstSize*8/linkDataRate)/loadServer));
              //  noTxTime= uniform((simtime_t)0,(simtime_t)(meanBurstSize*8/linkDataRate));
     //             noTxTime= 0e-9;
                    offPeriods++;
                }
                else{

                    if (packetsInPeriod==0)
                    {
                        onPeriods++;
                        //nextPeriod=simTime()+uniform((simtime_t)0.000000001,(simtime_t)(2*meanBurstSize*8/linkDataRate));
                        paretoShape = uniform(paretoMin,paretoMax);
                        nextPeriod=simTime()+ pareto_shifted(paretoShape,((paretoShape-1)/paretoShape)*meanBurstSize*8/linkDataRate,0);
              //          nextPeriod=simTime()+ meanBurstSize*8/linkDataRate;
              //          dest = destination();

                    }

                    if (simTime()<nextPeriod)
                    {
                        //EV<<"next period is"<<nextPeriod<<"\n";
                        sendThisPacket=true;
                        packetsInPeriod++;
                        packetLength = CustomCDF1(minPacketLength, maxPacketLength);
//                       packetLength = 1536;
                    }
                    else
                    {
                        sendThisPacket=false;
                        paretoShape = uniform(paretoMin,paretoMax);
                        noTxTime = pareto_shifted(paretoShape, ((paretoShape-1)/paretoShape)*((1-loadServer)/loadServer)*(sentPackets+1)/(offPeriods)*simMeanPacketSize,0)*8/linkDataRate;

//					    noTxTime = ((1-loadServer)/loadServer)*meanBurstSize*8/linkDataRate;
                                                //if (noTxTime>interarrivalMaxTime*numSources)
                                                  //  noTxTime = interarrivalMaxTime*numSources;
                        offPeriods++;
                        packetsInPeriod=0;
                    }
                }


                if (sendThisPacket){
                    lastTX = true;

                   if( (getIndex() >= 0 ) && (getIndex() < RealTotalserverNum )  ){
                        //    if( getIndex() == 2881 ){

                          dest = destination();
                      //   dest = 1084;
//                       packetLength = 64;
                        if(dest>=0){
                            pkt *newMsg =  generateMessage(dest,packetLength);
                            count2server[dest]++;


                        EV<<"Packet size is"<<newMsg->getPacketSize()<<"\n";
                        EV<<"dest is"<<dest<<"\n";
                        EV << "generating new packet "<<newMsg<<"and sending the packet to agg\n";
                        EV<< "msg Hop 0  "<<newMsg->getHop(0)<<"\n";
                        EV<< "msg Hop 1  "<<newMsg->getHop(1)<<"\n";
                        EV<< "msg Hop 2  "<<newMsg->getHop(2)<<"\n";
                        EV<< "msg Hop 3  "<<newMsg->getHop(3)<<"\n";
                        EV<< "msg Hop 4  "<<newMsg->getHop(4)<<"\n";
                        EV<< "msg Hop 5  "<<newMsg->getHop(5)<<"\n";

   //                     newMsg->setEnd2EndStartTime(simTime());
                        send(newMsg,"out");

                        numSent_Server++;
                        EV<<"packets sent from source Server "<<newMsg->getSrc_Server()<< "is"<<numSent_Server<<"\n";
                        EV<<"packets sent from dest Server "<<newMsg->getDst_Server()<<"\n";
                        finish_time = gate("out")->getTransmissionChannel()->getTransmissionFinishTime();

                        msg_count--;
    //                    if(getIndex()==0)
    //                        SRC_sizeHistogram.collect(packetLength);
    //                    if (recordData)
    //                        SRC_sizeVector.record(packetLength);
    //                    if (recordData)
    //                                        SRC_timeBetweenPacketsVector.record(nextSchedule-simTime());
                        nextSchedule=simTime()+(packetLength)*8/linkDataRate;
                        EV<<"packet lenght is"<<packetLength<<"\n";
                        EV<<"current time is"<<simTime()<<"\n";
                        EV<<"pariod time is"<<nextSchedule<<"\n";
    //                    if(getIndex()==0)
    //                        SRC_timeBetweenPacketsHistogram.collect(nextSchedule-simTime());
                        scheduleAt(nextSchedule,local_pkt);

                        }
                   }

                }
                else{

                    lastTX = false;
                    scheduleAt(simTime()+noTxTime,local_pkt);
//                    if(getIndex()==0)
//                        SRC_timeBetweenPacketsHistogram.collect(noTxTime);
//                    if (recordData)
//                        SRC_timeBetweenPacketsVector.record(noTxTime);

                }
        }
        else{

            msgfromserverfinish[getIndex()] = true;
            pkt *FinishMsg =  new pkt();
            FinishMsg->setIndex(5);   //finish
            FinishMsg->setSrc_Server(getIndex());
            sendDelayed(FinishMsg,1e-2,"out");
            EV<<"as counter value reaches zero, send finish msg "<<"\n";

            if(local_pkt->isScheduled())
                cancelEvent(local_pkt);
        }
    }
    else if(temp_msg->getIndex()==1){

        if(temp_msg->getFlag_intraToR()==0){

            numReceived_Server++;
        }
        else{
            numReceived_Server = numReceived_Server + temp_msg->getFlag_intraToR();
        }

        if(temp_msg->getFlag_ToR2ToR() == 1){

            numReceived_ToR2ToR++;
            time_latency_TOR2TOR = simTime() - temp_msg->getEnd2EndStartTime() + TxBufferTime + PreambleInsertdelay + BurstProcessDelay;
            latency_TOR2TOR = latency_TOR2TOR + time_latency_TOR2TOR;
            endToEndDelayVec.record(time_latency_TOR2TOR);
            time_latency_TOR2TOR = 0;
        }
        else{
            int cellnum = 0;
            if(temp_msg->getFlag_intraToR()==0){
                cellnum = 1;
                numReceived_intraToR++;
            }
            else{
                cellnum = temp_msg->getFlag_intraToR();
                numReceived_intraToR = numReceived_intraToR + temp_msg->getFlag_intraToR();
            }
             time_latency_intraTOR = simTime() - temp_msg->getEnd2EndStartTime() ;
             latency_intraTOR = latency_intraTOR + time_latency_intraTOR*cellnum;
             time_latency_intraTOR = 0;
        }

        if(temp_msg->getFlag_last()==1){
            if(temp_msg->getFlag_ToR2ToR() == 1){
                PnumReceived_ToR2ToR++;
                Ptime_latency = simTime() - temp_msg->getEnd2EndStartTime() + TxBufferTime + PreambleInsertdelay + BurstProcessDelay;
                Ptime_TOR2TORlatency = Ptime_TOR2TORlatency + Ptime_latency;
                Ptime_latency = 0;
            }
            else if(temp_msg->getFlag_ToR2ToR() == 0){
                PnumReceived_intraToR++;
                Ptime_latency = simTime() - temp_msg->getEnd2EndStartTime();
                Ptime_intraTORlatency = Ptime_intraTORlatency + Ptime_latency;
                Ptime_latency = 0;
            }
        }

        delete msg;
    }
}


pkt *ServerY::generateMessage(int dest, int packet_size)
{
    int  src = getIndex();
//    int  dest;
//    int  Packet_Size = 1500;
    char msgname[30];
    int  PathIndex;

    sentPackets++;
    sentBytes = sentBytes + packet_size;
//    packet_Size = CustomCDF1(minPacketLength, maxPacketLength);

//    int ClusterToRNum = FOSRadix;
//    int AllToRNum = FOSRadix*FOSRadix*FOSRadix;
    sprintf(msgname, "server-%d-to-%d at %f", src, dest,simTime().dbl());

    pkt *msg = new pkt(msgname);
    msg->setSrc_Server(src);
    msg->setDst_Server(dest);

    msg->setSrc_ToR(src/ToRserverNum);

    msg->setDst_ToR(dest/ToRserverNum);

    msg->setPacketSize(packet_size);

    msg->setEnd2EndStartTime(SIMTIME_DBL(simTime()));

    msg->setSchedulingPriority(10);

    msg->setFlag_intraToR(0);


    int src_cluster = msg->getSrc_ToR()/(ClusterToRNum*ClusterToRNum);
    int src_ToR_index_of_cluster = msg->getSrc_ToR()%(ClusterToRNum*ClusterToRNum);

    int dest_cluster = msg->getDst_ToR()/(ClusterToRNum*ClusterToRNum);
    int dest_ToR_index_of_cluster = msg->getDst_ToR()%(ClusterToRNum*ClusterToRNum);

    int src_cluster_connected = 0;
    int src_ToR_index_of_cluster_connected = 0;

    int dest_cluster_connected = 0;
    int dest_ToR_index_of_cluster_connected = 0;

    if(src_ToR_index_of_cluster >= src_cluster){
        src_cluster_connected = src_ToR_index_of_cluster + 1;
        src_ToR_index_of_cluster_connected = src_cluster;
    }
    else{
        src_cluster_connected = src_ToR_index_of_cluster;
        src_ToR_index_of_cluster_connected = src_cluster - 1;
    }

    if(dest_ToR_index_of_cluster >= dest_cluster){
        dest_cluster_connected = dest_ToR_index_of_cluster + 1;
        dest_ToR_index_of_cluster_connected = dest_cluster;
    }
    else{
        dest_cluster_connected = dest_ToR_index_of_cluster;
        dest_ToR_index_of_cluster_connected = dest_cluster - 1;
    }

    int TOR_mid = -1;


   if(       msg->getDst_ToR() == msg->getSrc_ToR() )    //inside the same ToR
    {
        msg->setHop(0, msg->getDst_ToR());
        msg->setHop(1, -100);
        msg->setHop(2, -100);
        msg->setHop(3, -100);
        msg->setHop(4, -100);
        msg->setHop(5, -100);
        msg->setHop(6, -100);
        msg->setHop(7, -100);
        msg->setHop(8, -100);
        msg->setHop(9, -100);
        msg->setHop(10, -100);
        msg->setTotalHopCount(1);

        msg->setFlag_ToR2ToR(0);
    }

    else if( msg->getDst_ToR()/ClusterToRNum == msg->getSrc_ToR()/ClusterToRNum){  //the same cluster
        msg->setHop(0, msg->getSrc_ToR());
        msg->setHop(1, AllToRNum + msg->getSrc_ToR()/ClusterToRNum);           //Agre layer
        msg->setHop(2, msg->getDst_ToR());
        msg->setHop(3, -100);
        msg->setHop(4, -100);
        msg->setHop(5, -100);
        msg->setHop(6, -100);
        msg->setHop(7, -100);
        msg->setHop(8, -100);
        msg->setHop(9, -100);
        msg->setHop(10, -100);
        msg->setTotalHopCount(3);

        msg->setFlag_ToR2ToR(1);
    }
    else if( ( msg->getDst_ToR()/(FOSRadix*FOSRadix) == msg->getSrc_ToR()/(FOSRadix*FOSRadix)  )  && ( abs( msg->getDst_ToR() - msg->getSrc_ToR()  )%ClusterToRNum ) == 0 ){   //connected the same core switch
        int OPSquareIndex = msg->getDst_ToR()/OPSquareToRNum;
        msg->setHop(0, msg->getSrc_ToR());
        msg->setHop(1, AllToRNum + AgreNum + OPSquareIndex*ClusterToRNum  + msg->getDst_ToR()% ClusterToRNum );   // core layer
        msg->setHop(2, msg->getDst_ToR());
        msg->setHop(3, -100);
        msg->setHop(4, -100);
        msg->setHop(5, -100);
        msg->setHop(6, -100);
        msg->setHop(7, -100);
        msg->setHop(8, -100);
        msg->setHop(9, -100);
        msg->setHop(10, -100);
        msg->setTotalHopCount(3);
        msg->setFlag_ToR2ToR(1);
    }
    else if( ( msg->getDst_ToR()/(FOSRadix*FOSRadix) == msg->getSrc_ToR()/(FOSRadix*FOSRadix)  )  ){   //in same opsquare, two hops.
        msg->setHop(0, msg->getSrc_ToR());
        PathIndex = intuniform(0,1);
        if(PathIndex == 0){
            int OPSquareIndex = msg->getSrc_ToR()/OPSquareToRNum;
            msg->setHop(1, AllToRNum + AgreNum + OPSquareIndex*ClusterToRNum + msg->getSrc_ToR()%ClusterToRNum ); // core
            msg->setHop(2, ( msg->getDst_ToR()/ClusterToRNum)*ClusterToRNum + msg->getSrc_ToR()%ClusterToRNum ); //ToR
            msg->setHop(3, AllToRNum + msg->getDst_ToR()/ClusterToRNum );//agre
            msg->setHop(4, msg->getDst_ToR());
            msg->setHop(5, -100);
        }
        else{
            msg->setHop(1, AllToRNum + msg->getSrc_ToR()/ClusterToRNum);  //agre
            msg->setHop(2, (msg->getSrc_ToR()/ClusterToRNum)*ClusterToRNum + msg->getDst_ToR()%ClusterToRNum ); //ToR
            msg->setHop(3, AllToRNum + AgreNum + (msg->getDst_ToR()/OPSquareToRNum)*ClusterToRNum  + msg->getDst_ToR()%ClusterToRNum ); // core
            msg->setHop(4, msg->getDst_ToR());
            msg->setHop(5, -100);
        }
        msg->setHop(6, -100);
        msg->setHop(7, -100);
        msg->setHop(8, -100);
        msg->setHop(9, -100);
        msg->setHop(10, -100);
    }

//  different opsquare
    else if(  (src_ToR_index_of_cluster >= src_cluster )     &&  (src_cluster  ==  dest_ToR_index_of_cluster)  &&  ( src_ToR_index_of_cluster  + 1 ==  dest_cluster)  ){
        msg->setHop(0, msg->getSrc_ToR());
        msg->setHop(1, msg->getDst_ToR());            //Two ToRs are directly connected  src j>=i
        msg->setHop(2, -100);
        msg->setHop(3, -100);
        msg->setHop(4, -100);
        msg->setHop(5, -100);
        msg->setHop(6, -100);
        msg->setHop(7, -100);
        msg->setHop(8, -100);
        msg->setHop(9, -100);
        msg->setHop(10, -100);
        msg->setTotalHopCount(2);
        msg->setFlag_ToR2ToR(1);
    }

    else if( (dest_ToR_index_of_cluster >= dest_cluster ) && (dest_cluster  ==  src_ToR_index_of_cluster)  &&  ( dest_ToR_index_of_cluster  + 1 ==  src_cluster)  ){
        msg->setHop(0, msg->getSrc_ToR());
        msg->setHop(1, msg->getDst_ToR());            //Two ToRs are directly connected  dest j>=i
        msg->setHop(2, -100);
        msg->setHop(3, -100);
        msg->setHop(4, -100);
        msg->setHop(5, -100);
        msg->setHop(6, -100);
        msg->setHop(7, -100);
        msg->setHop(8, -100);
        msg->setHop(9, -100);
        msg->setHop(10, -100);
        msg->setTotalHopCount(2);

        msg->setFlag_ToR2ToR(1);
    }

    else{
        if(src_cluster_connected == dest_cluster) { //
            msg->setHop(0, msg->getSrc_ToR());
            TOR_mid = src_cluster_connected*ClusterToRNum*ClusterToRNum + src_ToR_index_of_cluster_connected;
            msg->setHop(1, src_cluster_connected*ClusterToRNum*ClusterToRNum + src_ToR_index_of_cluster_connected  ); // go to ToR the is connected to dest ToR by same Agre

            if( msg->getDst_ToR()/ClusterToRNum == TOR_mid/ClusterToRNum ){  //the same cluster
                msg->setHop(2, AllToRNum + TOR_mid/ClusterToRNum);           //Agre layer
                msg->setHop(3, msg->getDst_ToR());
                msg->setHop(4, -100);
                msg->setHop(5, -100);
                msg->setHop(6, -100);
                msg->setHop(7, -100);
                msg->setHop(8, -100);
                msg->setHop(9, -100);
                msg->setHop(10, -100);
                msg->setTotalHopCount(4);

                msg->setFlag_ToR2ToR(1);
            }

            else if( ( abs( msg->getDst_ToR() - TOR_mid  )%ClusterToRNum ) == 0 ){   //connected the same core switch
                msg->setHop(2, AllToRNum + AgreNum + (msg->getDst_ToR()/OPSquareToRNum)*ClusterToRNum + msg->getDst_ToR()%ClusterToRNum );   // core layer
                msg->setHop(3, msg->getDst_ToR());
                msg->setHop(4, -100);
                msg->setHop(5, -100);
                msg->setHop(6, -100);
                msg->setHop(7, -100);
                msg->setHop(8, -100);
                msg->setHop(9, -100);
                msg->setHop(10, -100);
                msg->setTotalHopCount(4);
                msg->setFlag_ToR2ToR(1);
            }

            else{

                    msg->setHop(2, AllToRNum + AgreNum + (TOR_mid/OPSquareToRNum)*ClusterToRNum +  TOR_mid%ClusterToRNum ); // core
                    msg->setHop(3, ( msg->getDst_ToR()/ClusterToRNum)*ClusterToRNum + TOR_mid%ClusterToRNum ); //ToR
                    msg->setHop(4, AllToRNum + msg->getDst_ToR()/ClusterToRNum );//agre
                    msg->setHop(5, msg->getDst_ToR());
                    msg->setHop(6, -100);
                    msg->setHop(6, -100);
                    msg->setHop(7, -100);
                    msg->setHop(8, -100);
                    msg->setHop(9, -100);
                    msg->setHop(10, -100);

                    msg->setTotalHopCount(5);
                    msg->setFlag_ToR2ToR(1);
                }
            }

        else if(dest_cluster_connected == src_cluster){  //A   // go to Agre,then to the ToR which connect the dest ToR


            TOR_mid = src_cluster*OPSquareToRNum + dest_ToR_index_of_cluster_connected;
            msg->setHop(0, msg->getSrc_ToR());
            if( msg->getSrc_ToR()/ClusterToRNum == TOR_mid/ClusterToRNum ){  //the same cluster
                msg->setHop(1, AllToRNum + TOR_mid/ClusterToRNum);           //Agre layer
                msg->setHop(2, TOR_mid);
                msg->setHop(3, msg->getDst_ToR());
                msg->setHop(4, -100);
                msg->setHop(5, -100);
                msg->setHop(6, -100);
                msg->setHop(7, -100);
                msg->setHop(8, -100);
                msg->setHop(9, -100);
                msg->setHop(10, -100);
                msg->setTotalHopCount(4);

                msg->setFlag_ToR2ToR(1);
            }

            else if( ( abs( msg->getSrc_ToR() - TOR_mid  )%ClusterToRNum ) == 0 ){   //connected the same core switch
                msg->setHop(1, AllToRNum + AgreNum + (TOR_mid/OPSquareToRNum) *ClusterToRNum  +    TOR_mid%ClusterToRNum );   // core layer
                msg->setHop(2, TOR_mid);
                msg->setHop(3, msg->getDst_ToR());
                msg->setHop(4, -100);
                msg->setHop(5, -100);
                msg->setHop(6, -100);
                msg->setHop(7, -100);
                msg->setHop(8, -100);
                msg->setHop(9, -100);
                msg->setHop(10, -100);
                msg->setTotalHopCount(5);
                msg->setFlag_ToR2ToR(1);
            }

            else{

                    msg->setHop(1, AllToRNum + AgreNum + (msg->getSrc_ToR()/OPSquareToRNum)*ClusterToRNum + msg->getSrc_ToR()%ClusterToRNum); // core
                    msg->setHop(2, ( TOR_mid/ClusterToRNum)*ClusterToRNum + msg->getSrc_ToR()%ClusterToRNum ); //ToR
                    msg->setHop(3, AllToRNum + TOR_mid/OPSquareToRNum );//agre
                    msg->setHop(4, TOR_mid);
                    msg->setHop(5, msg->getDst_ToR());
                    msg->setHop(6, -100);
                    msg->setHop(7, -100);
                    msg->setHop(8, -100);
                    msg->setHop(9, -100);
                    msg->setHop(10, -100);

                    msg->setTotalHopCount(6);
                    msg->setFlag_ToR2ToR(1);
            }
         }
         else if(dest_cluster_connected == src_cluster_connected){  //B  //one Agre, two direct ToR to ToR connections

            int mid_TOR_1 = src_cluster_connected*OPSquareToRNum + src_ToR_index_of_cluster_connected;
            int mid_TOR_2 = dest_cluster_connected*OPSquareToRNum + dest_ToR_index_of_cluster_connected;
            msg->setHop(0, msg->getSrc_ToR());
            msg->setHop(1, src_cluster_connected*OPSquareToRNum + src_ToR_index_of_cluster_connected  );

            if( mid_TOR_1/ClusterToRNum == mid_TOR_2/ClusterToRNum){  //the same cluster
                msg->setHop(2, AllToRNum + mid_TOR_1/ClusterToRNum);           //Agre layer
                msg->setHop(3, mid_TOR_2);
                msg->setHop(4, msg->getDst_ToR());
                msg->setHop(5, -100);
                msg->setHop(6, -100);
                msg->setHop(7, -100);
                msg->setHop(8, -100);
                msg->setHop(9, -100);
                msg->setHop(10, -100);
                msg->setTotalHopCount(5);
            }
            else if( ( mid_TOR_2/(FOSRadix*FOSRadix) == mid_TOR_1/(FOSRadix*FOSRadix)  )  && ( abs( mid_TOR_2 - mid_TOR_1  )%ClusterToRNum ) == 0 ){   //connected the same core switch
                int OPSquareIndex = mid_TOR_2/OPSquareToRNum;
                msg->setHop(2, AllToRNum + AgreNum + OPSquareIndex*ClusterToRNum  + mid_TOR_2% ClusterToRNum );   // core layer
                msg->setHop(3, mid_TOR_2);
                msg->setHop(4, msg->getDst_ToR());
                msg->setHop(5, -100);
                msg->setHop(6, -100);
                msg->setHop(7, -100);
                msg->setHop(8, -100);
                msg->setHop(9, -100);
                msg->setHop(10, -100);
                msg->setTotalHopCount(5);
            }
            else if( ( mid_TOR_2/(FOSRadix*FOSRadix) == mid_TOR_1/(FOSRadix*FOSRadix)  )  ){   //in same opsquare, two hops.
                    int OPSquareIndex = mid_TOR_2/OPSquareToRNum;
                    msg->setHop(2, AllToRNum + AgreNum + OPSquareIndex*ClusterToRNum + mid_TOR_1%ClusterToRNum ); // core
                    msg->setHop(3, ( mid_TOR_2/ClusterToRNum)*ClusterToRNum + mid_TOR_1%ClusterToRNum ); //ToR
                    msg->setHop(4, AllToRNum + mid_TOR_2/ClusterToRNum );//agre
                    msg->setHop(5, mid_TOR_2);
                    msg->setHop(6, msg->getDst_ToR());
                    msg->setHop(7, -100);
                    msg->setHop(8, -100);
                    msg->setHop(9, -100);
                    msg->setHop(10, -100);
                    msg->setTotalHopCount(7);
            }
            msg->setFlag_ToR2ToR(1);

         }
         else{ //C
             msg->setHop(0, msg->getSrc_ToR());//Two Agre, one direct ToR to ToR connection
             int mid_TOR_1, mid_TOR_2 = -1;
             if( dest_cluster - 1 >= src_cluster){
                 mid_TOR_1 =  src_cluster*OPSquareToRNum + dest_cluster - 1 ; //
                 mid_TOR_2 =  dest_cluster*OPSquareToRNum + src_cluster ; //
             }
             else{
                 mid_TOR_1 =  src_cluster*OPSquareToRNum + dest_cluster; //
                 mid_TOR_2 =  dest_cluster*OPSquareToRNum + src_cluster-1; //
             }


             if( mid_TOR_1/ClusterToRNum == msg->getSrc_ToR()/ClusterToRNum){  //the same cluster   SRC and Mid1
                 msg->setHop(0, msg->getSrc_ToR());
                 msg->setHop(1, AllToRNum + msg->getSrc_ToR()/ClusterToRNum);           //Agre layer
                 msg->setHop(2, mid_TOR_1);

                 if( msg->getDst_ToR()/ClusterToRNum == mid_TOR_2/ClusterToRNum){  //the same cluster   Mid2 and Dst
                     msg->setHop(3, mid_TOR_2);
                     msg->setHop(4, AllToRNum + mid_TOR_2/ClusterToRNum);           //Agre layer
                     msg->setHop(5, msg->getDst_ToR());
                     msg->setHop(6, -100);
                     msg->setHop(7, -100);
                     msg->setHop(8, -100);
                     msg->setHop(9, -100);
                     msg->setHop(10, -100);
                     msg->setTotalHopCount(6);
                 }
                 else if( ( msg->getDst_ToR()/(FOSRadix*FOSRadix) == mid_TOR_2/(FOSRadix*FOSRadix)  )  && ( abs( msg->getDst_ToR() - mid_TOR_2  )%ClusterToRNum ) == 0 ){   //connected the same core switch
                     int OPSquareIndex = msg->getDst_ToR()/OPSquareToRNum;
                     msg->setHop(3, mid_TOR_2);
                     msg->setHop(4, AllToRNum + AgreNum + OPSquareIndex*ClusterToRNum  + msg->getDst_ToR()% ClusterToRNum );   // core layer
                     msg->setHop(5, msg->getDst_ToR());
                     msg->setHop(6, -100);
                     msg->setHop(7, -100);
                     msg->setHop(8, -100);
                     msg->setHop(9, -100);
                     msg->setHop(10, -100);
                     msg->setTotalHopCount(6);

                 }
                 else if( ( msg->getDst_ToR()/(FOSRadix*FOSRadix) == mid_TOR_2/(FOSRadix*FOSRadix)  )  ){   //in same opsquare, two hops.
                     msg->setHop(3, mid_TOR_2);
                     int OPSquareIndex = msg->getDst_ToR()/OPSquareToRNum;
                     msg->setHop(4, AllToRNum + AgreNum + OPSquareIndex*ClusterToRNum + mid_TOR_2%ClusterToRNum ); // core
                     msg->setHop(5, ( msg->getDst_ToR()/ClusterToRNum)*ClusterToRNum + mid_TOR_2%ClusterToRNum ); //ToR
                     msg->setHop(6, AllToRNum + msg->getDst_ToR()/ClusterToRNum );//agre
                     msg->setHop(7, msg->getDst_ToR());
                     msg->setHop(8, -100);
                     msg->setHop(9, -100);
                     msg->setHop(10, -100);
                     msg->setTotalHopCount(8);
                 }

             }
             else if( ( mid_TOR_1/(FOSRadix*FOSRadix) == msg->getSrc_ToR()/(FOSRadix*FOSRadix)  )  && ( abs( mid_TOR_1 - msg->getSrc_ToR()  )%ClusterToRNum ) == 0 ){   //connected the same core switch
                 int OPSquareIndex = mid_TOR_1/OPSquareToRNum;
                 msg->setHop(0, msg->getSrc_ToR());
                 msg->setHop(1, AllToRNum + AgreNum + OPSquareIndex*ClusterToRNum  + mid_TOR_1% ClusterToRNum );   // core layer
                 msg->setHop(2, mid_TOR_1);
                 if( msg->getDst_ToR()/ClusterToRNum == mid_TOR_2/ClusterToRNum){  //the same cluster
                     msg->setHop(3, mid_TOR_2);
                     msg->setHop(4, AllToRNum + mid_TOR_2/ClusterToRNum);           //Agre layer
                     msg->setHop(5, msg->getDst_ToR());
                     msg->setHop(6, -100);
                     msg->setHop(7, -100);
                     msg->setHop(8, -100);
                     msg->setHop(9, -100);
                     msg->setHop(10, -100);
                     msg->setTotalHopCount(6);
                 }
                 else if( ( msg->getDst_ToR()/(FOSRadix*FOSRadix) == mid_TOR_2/(FOSRadix*FOSRadix)  )  && ( abs( msg->getDst_ToR() - mid_TOR_2  )%ClusterToRNum ) == 0 ){   //connected the same core switch
                     int OPSquareIndex = msg->getDst_ToR()/OPSquareToRNum;
                     msg->setHop(3, mid_TOR_2);
                     msg->setHop(4, AllToRNum + AgreNum + OPSquareIndex*ClusterToRNum  + msg->getDst_ToR()% ClusterToRNum );   // core layer
                     msg->setHop(5, msg->getDst_ToR());
                     msg->setHop(6, -100);
                     msg->setHop(7, -100);
                     msg->setHop(9, -100);
                     msg->setHop(10, -100);
                     msg->setTotalHopCount(6);
                 }
                 else if( ( msg->getDst_ToR()/(FOSRadix*FOSRadix) == mid_TOR_2/(FOSRadix*FOSRadix)  )  ){   //in same opsquare, two hops.
                     msg->setHop(3, mid_TOR_2);
                     int OPSquareIndex = msg->getDst_ToR()/OPSquareToRNum;
                     msg->setHop(4, AllToRNum + AgreNum + OPSquareIndex*ClusterToRNum + mid_TOR_2%ClusterToRNum ); // core
                     msg->setHop(5, ( msg->getDst_ToR()/ClusterToRNum)*ClusterToRNum + mid_TOR_2%ClusterToRNum ); //ToR
                     msg->setHop(6, AllToRNum + msg->getDst_ToR()/ClusterToRNum );//agre
                     msg->setHop(7, msg->getDst_ToR());
                     msg->setHop(8, -100);
                     msg->setHop(9, -100);
                     msg->setHop(10, -100);
                     msg->setTotalHopCount(8);
                 }

             }
             else if( ( mid_TOR_1/(FOSRadix*FOSRadix) == msg->getSrc_ToR()/(FOSRadix*FOSRadix)  )  ){
                 msg->setHop(0, msg->getSrc_ToR());
                 int OPSquareIndex = mid_TOR_1/OPSquareToRNum;
                 msg->setHop(1, AllToRNum + AgreNum + OPSquareIndex*ClusterToRNum + msg->getSrc_ToR()%ClusterToRNum ); // core
                 msg->setHop(2, ( mid_TOR_1/ClusterToRNum)*ClusterToRNum + msg->getSrc_ToR()%ClusterToRNum ); //ToR
                 msg->setHop(3, AllToRNum + mid_TOR_1/ClusterToRNum );//agre
                 msg->setHop(4, mid_TOR_1);

                 if( msg->getDst_ToR()/ClusterToRNum == mid_TOR_2/ClusterToRNum){  //the same cluster
                     msg->setHop(5, mid_TOR_2);
                     msg->setHop(6, AllToRNum + mid_TOR_2/ClusterToRNum);           //Agre layer
                     msg->setHop(7, msg->getDst_ToR());
                     msg->setHop(8, -100);
                     msg->setHop(9, -100);
                     msg->setHop(10, -100);
                     msg->setTotalHopCount(8);

                 }
                 else if( ( msg->getDst_ToR()/(FOSRadix*FOSRadix) == mid_TOR_2/(FOSRadix*FOSRadix)  )  && ( abs( msg->getDst_ToR() - mid_TOR_2  )%ClusterToRNum ) == 0 ){   //connected the same core switch
                     int OPSquareIndex = msg->getDst_ToR()/OPSquareToRNum;
                     msg->setHop(5, mid_TOR_2);
                     msg->setHop(6, AllToRNum + AgreNum + OPSquareIndex*ClusterToRNum  + msg->getDst_ToR()% ClusterToRNum );   // core layer
                     msg->setHop(7, msg->getDst_ToR());
                     msg->setHop(8, -100);
                     msg->setHop(9, -100);
                     msg->setHop(10, -100);
                     msg->setTotalHopCount(8);
                 }
                 else if( ( msg->getDst_ToR()/(FOSRadix*FOSRadix) == mid_TOR_2/(FOSRadix*FOSRadix)  )  ){   //in same opsquare, two hops.
                     msg->setHop(5, mid_TOR_2);
                     int OPSquareIndex = msg->getDst_ToR()/OPSquareToRNum;
                     msg->setHop(6, AllToRNum + AgreNum + OPSquareIndex*ClusterToRNum + mid_TOR_2%ClusterToRNum ); // core
                     msg->setHop(7, ( msg->getDst_ToR()/ClusterToRNum)*ClusterToRNum + mid_TOR_2%ClusterToRNum ); //ToR
                     msg->setHop(8, AllToRNum + msg->getDst_ToR()/ClusterToRNum );//agre
                     msg->setHop(9, msg->getDst_ToR());
                     msg->setHop(10, -100);
                     msg->setTotalHopCount(10);
                 }
             }

             msg->setFlag_ToR2ToR(1);
         }

    }

    msg->setIndex(1);
    msg->setKind(1);
    msg->setTimestamp();
    msg->setHopCountNow(0);
    msg->setSchedulingPriority(10);

    return msg;
}


int ServerY::destination()
{
    int src,dest;
    double p1;
    double pintraTOR,pintraCluster,pinterCluster;
    int ToRStartServer;
    int ToREndServer;
    int ClusterStartServer;
    int ClusterEndServer;
    int pinterSuperCluster;
    int SuperClusterStartServer;
    int SuperClusterEndServer;

    src = getIndex();

    ToRStartServer = (src/ToRserverNum)*ToRserverNum;
    ToREndServer   =  (src/ToRserverNum)*ToRserverNum + ToRserverNum - 1 ;



//        p1 = uniform(0,1);
//
//        if(p1<0.5){
//
//             dest=intuniform(ToRStartServer,ToREndServer - 1);
//             if(dest>=src)
//                 dest = dest + 1;
//        }
//        else{
//
//             dest = intuniform(0,totalserverNum - ToRserverNum - 1);
//             if(dest/ToRserverNum >= src/ToRserverNum    )
//                 dest = dest + ToRserverNum;
//        }

     ClusterStartServer = (src/(ToRserverNum*FraudClusterToRNum) )*(ToRserverNum*FraudClusterToRNum);
     ClusterEndServer   = (src/(ToRserverNum*FraudClusterToRNum) )*(ToRserverNum*FraudClusterToRNum)  + (ToRserverNum*FraudClusterToRNum) - 1 ;

     pintraTOR = 0.5;
        pintraCluster = 0.375;
        pinterCluster = 0.125;

        p1 = uniform(0,1);

        if(p1<= pintraTOR){   //  ToR inside

            dest=intuniform(ToRStartServer,ToREndServer - 1);
            if(dest>=src)
                dest = dest + 1;
            SendPacketinToR++;
        }
        else if(p1 <= (pintraTOR + pintraCluster) && p1 > pintraTOR){ //cluster inside
            dest = intuniform(ClusterStartServer,ClusterEndServer - ToRserverNum);
            if(dest/ToRserverNum >= src/ToRserverNum   )
                dest = dest + ToRserverNum;

            SendPacketToR2ToR++;
        }
        else{   // go to other cluster
            dest = intuniform(0,RealTotalserverNum - 1 - ToRserverNum*FraudClusterToRNum );
                       if(dest/(ToRserverNum*FraudClusterToRNum) >= src/(ToRserverNum*FraudClusterToRNum)  )
                           dest = dest + ToRserverNum*FraudClusterToRNum;

            SendPacketToR2ToR++;
        }

        //printf("dest = %d\n",dest);


  /*        pintraTOR = 0.5;
       pintraCluster = 0.375;
       pinterCluster = 0.0625;
       pinterSuperCluster = 0.0625;
       ClusterStartServer = (src/(ToRserverNum*ClusterToRNum) )*(ToRserverNum*ClusterToRNum);
       ClusterEndServer   = (src/(ToRserverNum*ClusterToRNum) )*(ToRserverNum*ClusterToRNum)  + (ToRserverNum*ClusterToRNum) - 1 ;

       SuperClusterStartServer = (src/(ToRserverNum*ClusterToRNum*FOSRadix) )*(ToRserverNum*ClusterToRNum*FOSRadix);
       SuperClusterEndServer   = (src/(ToRserverNum*ClusterToRNum*FOSRadix) )*(ToRserverNum*ClusterToRNum*FOSRadix)  + (ToRserverNum*ClusterToRNum*FOSRadix) - 1 ;

         p1 = uniform(0,1);

            if(p1<= pintraTOR){   // ToR inside

                 dest=intuniform(ToRStartServer,ToREndServer - 1);
                 if(dest>=src)
                     dest = dest + 1;
                 SendPacketinToR++;
             }
            else if(p1 <= (pintraTOR + pintraCluster) && p1 > pintraTOR){
                dest = intuniform(ClusterStartServer,ClusterEndServer - ToRserverNum);
                if(dest/ToRserverNum >= src/ToRserverNum   )
                    dest = dest + ToRserverNum;
                SendPacketToR2ToR++;

            }
             else if(p1 <= (pintraTOR + pintraCluster + pinterCluster ) && p1 > (pintraTOR + pintraCluster)  ){ //cluster inside
                 dest = intuniform(SuperClusterStartServer,SuperClusterEndServer - ToRserverNum*ClusterToRNum);
                 if(dest/(ToRserverNum*ClusterToRNum) >= src/(ToRserverNum*ClusterToRNum)   )
                     dest = dest + ToRserverNum*ClusterToRNum;

                 SendPacketToR2ToR++;
             }
             else if(p1 <= 1 && p1 > (pintraTOR + pintraCluster + pinterCluster )){   // go to other cluster
                 dest = intuniform(0,totalserverNum - 1 - ToRserverNum*ClusterToRNum*FOSRadix );
                            if(dest/(ToRserverNum*ClusterToRNum*FOSRadix) >= src/(ToRserverNum*ClusterToRNum*FOSRadix)  )
                                dest = dest + ToRserverNum*ClusterToRNum*FOSRadix;

                 SendPacketToR2ToR++;
             }*/

        return dest;
}

int  ServerY::CustomCDF1(int min, int max)
{
    double PaperCDFProbs[] = {0, 0.21025, 0.211, 0.21175, 0.2123333334, 0.2128333334, 0.2133333334, 0.2138333334, 0.2143333334, 0.2148333334, 0.2181428573,
                    0.222857143, 0.2275714286, 0.2322857143, 0.237, 0.237125, 0.23725, 0.237375, 0.2375, 0.2391875, 0.240875, 0.2425625, 0.24425, 0.2459375, 0.247625,
                    0.2493125, 0.251, 0.25169, 0.25238, 0.25307, 0.25376, 0.25445, 0.25514, 0.25583, 0.25652, 0.25721, 0.2579, 0.25859, 0.25928, 0.25997, 0.26066,
                    0.26135, 0.26204, 0.26225, 0.2625, 0.26275, 0.2635, 0.2637500001, 0.2655555556, 0.2663888889, 0.2672222223, 0.2680555556, 0.2688888889, 0.2697222223,
                    0.2705555556, 0.2713888889, 0.2722222223, 0.2730555556, 0.2738888889, 0.2747222223, 0.276923077, 0.2798076924, 0.2826923078, 0.2855769231,
                    0.2884615385, 0.2913461539, 0.2942307693, 0.2971153846, 0.3, 0.3015625, 0.303125, 0.3046875, 0.30625, 0.3078125, 0.309375, 0.3109375, 0.3125,
                    0.3139423077, 0.3153846154, 0.3168269231, 0.3182692307, 0.3197115384, 0.3211538461, 0.3225961538, 0.3240384615, 0.32525, 0.326, 0.32675, 0.3275,
                    0.32825, 0.329, 0.32975, 0.3305, 0.33125, 0.332, 0.33275, 0.3335, 0.33425, 0.335, 0.33575, 0.3365, 0.33725, 0.338, 0.33875, 0.3395, 0.3407142857,
                    0.3428571428, 0.345, 0.3471428572, 0.3492857143, 0.3508333334, 0.3520833334, 0.3533333334, 0.3545833334, 0.3558333334, 0.3570833334, 0.3583333334,
                    0.3595833334, 0.365, 0.3701086956, 0.3704347826, 0.3707608696, 0.3710869565, 0.3714130435, 0.3717391304, 0.3720652174, 0.3723913043, 0.3727173913,
                    0.3730434783, 0.3733695652, 0.3736956522, 0.3740217391, 0.3743478261, 0.374673913, 0.375, 0.3755769231, 0.3761538461, 0.3767307692, 0.3773076923,
                    0.3778846154, 0.3784615384, 0.3790384615, 0.3796153846, 0.3801, 0.3804, 0.3807, 0.381, 0.3813, 0.3816, 0.3819, 0.3822, 0.3825, 0.3828, 0.3831, 0.3834,
                    0.3837, 0.384, 0.3843, 0.3846, 0.3849, 0.3852, 0.3855, 0.3858, 0.3861, 0.3864, 0.3867, 0.387, 0.3873, 0.3876, 0.3879, 0.3882, 0.3885, 0.3888, 0.3891,
                    0.3894, 0.3897, 0.39, 0.3906, 0.3912, 0.3918, 0.3924, 0.393, 0.3936, 0.3942, 0.3948, 0.3954, 0.396, 0.3966, 0.3972, 0.3978, 0.3984, 0.399, 0.3996,
                    0.4002, 0.4008, 0.4014, 0.402, 0.4026, 0.4032, 0.4038, 0.4044, 0.405, 0.4056, 0.4062, 0.4068, 0.4074, 0.408, 0.4086, 0.4092, 0.4098, 0.410025,
                    0.4100625, 0.4101, 0.4101375, 0.410175, 0.4102125, 0.41025, 0.4102875, 0.410325, 0.4103625, 0.4104, 0.4104375, 0.410475, 0.4105125, 0.41055,
                    0.4105875, 0.410625, 0.4106625, 0.4107, 0.4107375, 0.410775, 0.4108125, 0.41085, 0.4108875, 0.410925, 0.4109625, 0.411, 0.4110375, 0.411075,
                    0.4111125, 0.41115, 0.4111875, 0.411225, 0.4112625, 0.4113, 0.4113375, 0.411375, 0.4114125, 0.41145, 0.4114875, 0.411525, 0.4115625, 0.4116,
                    0.4116375, 0.411675, 0.4117125, 0.41175, 0.4117875, 0.411825, 0.4118625, 0.4119, 0.4119375, 0.411975, 0.4120125, 0.41205, 0.4120875, 0.412125,
                    0.4121625, 0.4122, 0.4122375, 0.412275, 0.4123125, 0.41235, 0.4123875, 0.412425, 0.4124625, 0.4125, 0.4125375, 0.412575, 0.4126125, 0.41265,
                    0.4126875, 0.412725, 0.4127625, 0.4128, 0.4128375, 0.412875, 0.4129125, 0.41295, 0.4129875, 0.413025, 0.4130625, 0.4131, 0.4131375, 0.413175,
                    0.4132125, 0.41325, 0.4132875, 0.413325, 0.4133625, 0.4134, 0.4134375, 0.413475, 0.4135125, 0.41355, 0.4135875, 0.413625, 0.4136625, 0.4137,
                    0.4137375, 0.413775, 0.4138125, 0.41385, 0.4138875, 0.413925, 0.4139625, 0.414, 0.4140375, 0.414075, 0.4141125, 0.41415, 0.4141875, 0.414225,
                    0.4142625, 0.4143, 0.4143375, 0.414375, 0.4144125, 0.41445, 0.4144875, 0.414525, 0.4145625, 0.4146, 0.4146375, 0.414675, 0.4147125, 0.41475,
                    0.4147875, 0.414825, 0.4148625, 0.4149, 0.4149375, 0.414975, 0.4151041667, 0.4154166667, 0.4157291667, 0.4160416667, 0.4163541667, 0.4166666667,
                    0.4169791667, 0.4172916667, 0.4176041667, 0.4179166667, 0.4182291667, 0.4185416667, 0.4188541667, 0.4191666667, 0.4194791667, 0.4197916667,
                    0.4224999998, 0.43, 0.4300907258, 0.4301814516, 0.4302721774, 0.4303629032, 0.430453629, 0.4305443548, 0.4306350806, 0.4307258065, 0.4308165323,
                    0.4309072581, 0.4309979839, 0.4310887097, 0.4311794355, 0.4312701613, 0.4313608871, 0.4314516129, 0.4315423387, 0.4316330645, 0.4317237903,
                    0.4318145161, 0.4319052419, 0.4319959677, 0.4320866935, 0.4321774194, 0.4322681452, 0.432358871, 0.4324495968, 0.4325403226, 0.4326310484,
                    0.4327217742, 0.4328125, 0.4329032258, 0.4329939516, 0.4330846774, 0.4331754032, 0.433266129, 0.4333568548, 0.4334475806, 0.4335383065,
                    0.4336290323, 0.4337197581, 0.4338104839, 0.4339012097, 0.4339919355, 0.4340826613, 0.4341733871, 0.4342641129, 0.4343548387, 0.4344455645,
                    0.4345362903, 0.4346270161, 0.4347177419, 0.4348084677, 0.4348991935, 0.4349899194, 0.4350806452, 0.435171371, 0.4352620968, 0.4353528226,
                    0.4354435484, 0.4355342742, 0.435625, 0.4357157258, 0.4358064516, 0.4358971774, 0.4359879032, 0.436078629, 0.4361693548, 0.4362600806,
                    0.4363508064, 0.4364415323, 0.4365322581, 0.4366229839, 0.4367137097, 0.4368044355, 0.4368951613, 0.4369858871, 0.4370766129, 0.4371673387,
                    0.4372580645, 0.4373487903, 0.4374395161, 0.437525, 0.4376, 0.437675, 0.43775, 0.437825, 0.4379, 0.437975, 0.43805, 0.438125, 0.4382, 0.438275,
                    0.43835, 0.438425, 0.4385, 0.438575, 0.43865, 0.438725, 0.4388, 0.438875, 0.43895, 0.439025, 0.4391, 0.439175, 0.43925, 0.439325, 0.4394, 0.439475,
                    0.43955, 0.439625, 0.4397, 0.439775, 0.43985, 0.439925, 0.44, 0.4404411765, 0.4408823529, 0.4413235294, 0.4417647059, 0.4422058824, 0.4426470588,
                    0.4430882353, 0.4435294118, 0.4439705882, 0.4444117647, 0.4448529412, 0.445, 0.445, 0.445, 0.445, 0.445, 0.445, 0.445, 0.445, 0.445, 0.445, 0.445,
                    0.445, 0.445, 0.445, 0.445, 0.445, 0.445, 0.445, 0.445, 0.445, 0.445, 0.445, 0.4451, 0.44525, 0.4454, 0.44555, 0.4457, 0.44585, 0.446, 0.44615,
                    0.4463, 0.44645, 0.4466, 0.44675, 0.4469, 0.44705, 0.4472, 0.44735, 0.4475, 0.44765, 0.4478, 0.44795, 0.4481, 0.44825, 0.4484, 0.44855, 0.4487,
                    0.44885, 0.449, 0.44915, 0.4493, 0.44945, 0.4496, 0.44975, 0.4499, 0.45005, 0.4502, 0.45035, 0.4505, 0.45065, 0.4508, 0.45095, 0.4511, 0.45125,
                    0.4514, 0.45155, 0.4517, 0.45185, 0.452, 0.45215, 0.4523, 0.45245, 0.4526, 0.45275, 0.4529, 0.45305, 0.4532, 0.45335, 0.4535, 0.45365, 0.4538,
                    0.45395, 0.4541, 0.45425, 0.4544, 0.45455, 0.4547, 0.45485, 0.455, 0.4551125, 0.455225, 0.4553375, 0.45545, 0.4555625, 0.455675, 0.4557875, 0.4559,
                    0.4560125, 0.456125, 0.4562375, 0.45635, 0.4564625, 0.456575, 0.4566875, 0.4568, 0.4569125, 0.457025, 0.4571375, 0.45725, 0.4573625, 0.457475,
                    0.4575875, 0.4577, 0.4578125, 0.457925, 0.4580375, 0.45815, 0.4582625, 0.458375, 0.4584875, 0.4586, 0.4587125, 0.458825, 0.4589375, 0.45905,
                    0.4591625, 0.459275, 0.4593875, 0.4595, 0.4596125, 0.459725, 0.4598375, 0.45995, 0.4600625, 0.460175, 0.4602875, 0.4604, 0.4605125, 0.460625,
                    0.4607375, 0.46085, 0.4609625, 0.461075, 0.4611875, 0.4613, 0.4614125, 0.461525, 0.4616375, 0.46175, 0.4618625, 0.461975, 0.4620875, 0.4622,
                    0.4623125, 0.462425, 0.4625125, 0.46255, 0.4625875, 0.462625, 0.4626625, 0.4627, 0.4627375, 0.462775, 0.4628125, 0.46285, 0.4628875, 0.462925,
                    0.4629625, 0.463, 0.4630375, 0.463075, 0.4631125, 0.46315, 0.4631875, 0.463225, 0.4632625, 0.4633, 0.4633375, 0.463375, 0.4634125, 0.46345,
                    0.4634875, 0.463525, 0.4635625, 0.4636, 0.4636375, 0.463675, 0.4637125, 0.46375, 0.4637875, 0.463825, 0.4638625, 0.4639, 0.4639375, 0.463975,
                    0.4640125, 0.46405, 0.4640875, 0.464125, 0.4641625, 0.4642, 0.4642375, 0.464275, 0.4643125, 0.46435, 0.4643875, 0.464425, 0.4644625, 0.4645,
                    0.4645375, 0.464575, 0.4646125, 0.46465, 0.4646875, 0.464725, 0.4647625, 0.4648, 0.4648375, 0.464875, 0.4649125, 0.46495, 0.4649875, 0.465042735,
                    0.4651068376, 0.4651709402, 0.4652350427, 0.4652991453, 0.4653632479, 0.4654273504, 0.465491453, 0.4655555556, 0.4656196581, 0.4656837607,
                    0.4657478633, 0.4658119658, 0.4658760684, 0.4659401709, 0.4660042735, 0.4660683761, 0.4661324786, 0.4661965812, 0.4662606838, 0.4663247863,
                    0.4663888889, 0.4664529915, 0.466517094, 0.4665811966, 0.4666452991, 0.4667094017, 0.4667735043, 0.4668376068, 0.4669017094, 0.466965812,
                    0.4670299145, 0.4670940171, 0.4671581197, 0.4672222222, 0.4672863248, 0.4673504274, 0.4674145299, 0.4674786325, 0.467542735, 0.4676068376,
                    0.4676709402, 0.4677350427, 0.4677991453, 0.4678632479, 0.4679273504, 0.467991453, 0.4680555556, 0.4681196581, 0.4681837607, 0.4682478633,
                    0.4683119658, 0.4683760684, 0.4684401709, 0.4685042735, 0.4685683761, 0.4686324786, 0.4686965812, 0.4687606838, 0.4688247863, 0.4688888889,
                    0.4689529915, 0.469017094, 0.4690811966, 0.4691452991, 0.4692094017, 0.4692735043, 0.4693376068, 0.4694017094, 0.469465812, 0.4695299145,
                    0.4695940171, 0.4696581197, 0.4697222222, 0.4697863248, 0.4698504274, 0.4699145299, 0.4699786325, 0.475, 0.4800513699, 0.4802054794, 0.480359589,
                    0.4805136986, 0.4806678082, 0.4808219178, 0.4809760274, 0.481130137, 0.4812842466, 0.4814383562, 0.4815924657, 0.4817465753, 0.4819006849,
                    0.4820547945, 0.4822089041, 0.4823630137, 0.4825171233, 0.4826712329, 0.4828253425, 0.4829794521, 0.4831335616, 0.4832876712, 0.4834417808,
                    0.4835958904, 0.48375, 0.4839041096, 0.4840582192, 0.4842123288, 0.4843664384, 0.4845205479, 0.4846746575, 0.4848287671, 0.4849828767,
                    0.4851369863, 0.4852910959, 0.4854452055, 0.4855993151, 0.4857534247, 0.4859075342, 0.4860616438, 0.4862157534, 0.486369863, 0.4865239726,
                    0.4866780822, 0.4868321918, 0.4869863014, 0.487140411, 0.4872945205, 0.4874486301, 0.4876027397, 0.4877568493, 0.4879109589, 0.4880650685,
                    0.4882191781, 0.4883732877, 0.4885273973, 0.4886815068, 0.4888356164, 0.488989726, 0.4891438356, 0.4892979452, 0.4894520548, 0.4896061644,
                    0.489760274, 0.4899143836, 0.4900684931, 0.4902226027, 0.4903767123, 0.4905308219, 0.4906849315, 0.4908390411, 0.4909931507, 0.4911472603,
                    0.4913013699, 0.4914554795, 0.491609589, 0.4917636986, 0.4919178082, 0.4920719178, 0.4922260274, 0.492380137, 0.4925342466, 0.4926883562,
                    0.4928424658, 0.4929965753, 0.4931506849, 0.4933047945, 0.4934589041, 0.4936130137, 0.4937671233, 0.4939212329, 0.4940753425, 0.4942294521,
                    0.4943835616, 0.4945376712, 0.4946917808, 0.4948458904, 0.495, 0.4965, 0.498, 0.4995, 0.501, 0.5025, 0.504, 0.505125, 0.5055, 0.505875, 0.50625,
                    0.506625, 0.507, 0.507375, 0.50775, 0.508125, 0.5085, 0.508875, 0.50925, 0.509625, 0.51, 0.5115, 0.513, 0.5145, 0.51518, 0.51545, 0.51572,
                    0.51599, 0.51626, 0.51653, 0.5168, 0.51707, 0.51734, 0.51761, 0.51788, 0.51815, 0.51842, 0.51869, 0.51896, 0.51923, 0.5195, 0.51977, 0.52004,
                    0.52031, 0.52058, 0.52085, 0.52112, 0.52139, 0.52166, 0.52193, 0.5222, 0.52247, 0.52274, 0.52301, 0.52328, 0.52355, 0.52382, 0.5240416667,
                    0.5241666667, 0.5242916667, 0.5244166667, 0.5245416667, 0.5246666667, 0.5247916667, 0.5249166667, 0.5264999999, 0.531, 0.5322, 0.5334, 0.5346,
                    0.655, 0.7762499999, 0.78, 0.84508, 0.84532, 0.84556, 0.8458, 0.84604, 0.84628, 0.84652, 0.84676, 0.847, 0.84724, 0.84748, 0.84772, 0.84796,
                    0.8482, 0.84844, 0.84868, 0.84892, 1};              //CDF function for Paper packet size distribution

    double i = uniform(0,1);
    int pos = 0;

    while(PaperCDFProbs[pos]<i)
        pos++;

    int size=(int)(min+(max-min)*((double)(pos)/976));

    return size;
}


void ServerY::updateDisplay()
{
       char buf[40];
       sprintf(buf, "rcvd: %ld sent: %ld", numReceived_Server,numReceived_Server); //??Y
       getDisplayString().setTagArg("t",0,buf);
}

void ServerY::updateStats()
{
    simtime_t currentTime = simTime();
    //EV << currentTime << endl;
    simLoad = (sentBytes*8/dataRate)/( currentTime.dbl());
    simMeanPacketSize = sentBytes/sentPackets;
}


void ServerY::finish()
{
recordScalar("Send-packets-num-is",numSent_Server);
recordScalar("PacksendoutToRnumis",SendPacketToR2ToR);
recordScalar("PacksendinToRnum-is",SendPacketinToR);
recordScalar("Num-of-Pkt-send--to",count2server[getIndex()]);
recordScalar("RecePktToR2ToRnumis",PnumReceived_ToR2ToR);
recordScalar("RecePktintraToR--is",PnumReceived_intraToR);
recordScalar("LatePkt-intraTOR-is",Ptime_intraTORlatency);
recordScalar("LatePkt-TOR2TOR--is",Ptime_TOR2TORlatency);
recordScalar("RecetotalCells---is",numReceived_Server);
recordScalar("RecToR2ToRCell---is",numReceived_ToR2ToR);
recordScalar("RecintraToRCell--is",numReceived_intraToR);
//recordScalar("Throughput-------is",numReceived_Server*12000/latency/1024/1024/1024);
recordScalar("LateCellintraTOR-is",latency_intraTOR);
recordScalar("LateCellTOR2TOR--is",latency_TOR2TOR);

//recordScalar("average--latency-is",latency/numReceived_Server);
}


