/*
 * ping3.cc
 *
 *  Created on: 2020Äê4ÔÂ28ÈÕ
 *      Author: 13733
 */






#include<omnetpp.h>
#include<iostream>
#include<mymessage_m.h>
#include<stack>
#include<deque>
#include <iostream>
#include <fstream>
#include <string>
#include<vector>
#include<unordered_map>
#include<algorithm>
#include <random>
#include <chrono>
using namespace omnetpp;

using std::vector;
using std::unordered_map;
using std::reverse;
using std::begin;
using std::end;
using std::string;
vector<int> path;
int cnt = 0;
int citys = 29;
int MOD = 300;
const int min_key_nums = 20;
const int max_key_nums = 128;
const int wait_time = 30;
unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
std::default_random_engine gen(seed);
std::normal_distribution<double> dis(0.005,0.004);
//cNormal c(nullptr,0.00,10.00);
unordered_map<int, const char*> m_{{0,"shanghai"},{1,"beijing"},{2,"singapore"},{3,"nanjing"},{4, "jinan"},{5,"shenyang"},{6, "huhehaote"},{7, "nanchang"},{8, "fuzhou"},{9, "shijiazhuang"},{10, "changcun"},{11, "haerbin"},{12, "wulumuqi"},{13, "lanzhou"},{14, "yinchuan"},{15, "xian"},{16, "fujian"},{17, "guangzhou"},{18, "guiyang"},{19, "changsha"},{20, "wuhan"},{21, "chengdu"},{22, "kunming"},{23, "lasha"},{24, "xining"},{25, "tianjin"},{26, "taibei"},{27, "haikou"},{28,"hangzhou"}};



void create_path_nums(){
    std::srand(time(0));
    cnt = 0;
    while(cnt < min_key_nums || cnt > max_key_nums){

        cnt = std::rand()%MOD;
    }
    EV <<  cnt << endl;



};


class A: public cSimpleModule{
protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void finish() override;
public:
    cMessage *event;
    std::ofstream oFile;
    static int id;
    simtime_t send_time;
    simtime_t receive_time;

};
Define_Module(A);
int A::id = 1;
void A::initialize(){

    event = new cMessage("event");
    oFile.open("C://Users//13733//Desktop//a.csv",std::ios::out|std::ios::trunc);
    oFile<<"id"<<","<<"time"<<endl;
   // message = new mymessage();
    receive_time = 0;
    scheduleAt(simTime()+2,event);


}

void A::handleMessage(cMessage *msg){
    if(msg==event){
        create_path_nums();
        mymessage *myMsg = new mymessage();

        myMsg->setReceive_time(receive_time);
        myMsg->setN(cnt);
        myMsg->setDirection(1);
        const char* current_city = msg->getArrivalModule()->getFullName();
        myMsg->setLast_city(current_city);
        std::srand(time(0));
        int next = rand()%citys;
        //send(myMsg, "out", next);
        sendDelayed(myMsg,  abs(dis(gen)), "out", next);
        send_time = simTime();

       }


    else{
        receive_time = msg->getArrivalTime();
        mymessage *myMsg = check_and_cast<mymessage *>(msg);
        simtime_t endtime = myMsg->getReceive_time();
        EV << endtime - send_time << endl;
        oFile<< id++ <<","<< (endtime - send_time)*1000 <<endl;
        int wait = std::rand()%wait_time;
        wait = 0;
        scheduleAt(simTime()+wait,event);
    }



}
void A::finish(){
    oFile.close();
}


class B: public cSimpleModule{
protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void finish() override;
public:
    std::ofstream oFile;
    static int id;
    simtime_t send_time;
    simtime_t receive_time;
};
Define_Module(B);
int B::id = 1;
void B::initialize(){
    oFile.open("C://Users//13733//Desktop//b.csv",std::ios::out|std::ios::trunc);
    oFile<<"id"<<","<<"time"<<endl;
}

void B::handleMessage(cMessage *msg){
    receive_time = msg->getArrivalTime();
    mymessage *myMsg = check_and_cast<mymessage *>(msg);
    simtime_t endtime = myMsg->getReceive_time();
    if(endtime > 0){
        EV << endtime - send_time << endl;
        oFile<< id++ <<","<< (endtime - send_time)*1000 <<endl;
    }

    myMsg->setReceive_time(receive_time);
    myMsg->setDirection(-1);
    const char* last_city = myMsg->getLast_city();
    int n = gateSize("out");
    int gateindex = 0;
    for(int i = 0; i < n; ++i){
        cGate *out = gate("out",i);
        const char* next= out->getNextGate()->getOwnerModule()->getFullName();
        if(strcmp(next, last_city) == 0){
            gateindex = i;
            break;
        }
    }
    int wait_interval = std::rand()%wait_time;
    //wait(wait_interval);
    sendDelayed(myMsg,  abs(dis(gen)), "out", gateindex);
    //send(myMsg, "out", gateindex);
    send_time = simTime();



}
void B::finish(){
    oFile.close();
}


class relay: public cSimpleModule{
protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void finish() override;
public:
    std::stack<std::string> s_;
};
Define_Module(relay);
void relay::initialize(){

}

void relay::handleMessage(cMessage *msg){
   mymessage *myMsg = check_and_cast<mymessage *>(msg);
   int direction = myMsg->getDirection();
   int n = gateSize("out");
   if(direction == 1){
       s_.push(string(myMsg->getLast_city()));
       const char* current_city = msg->getArrivalModule()->getFullName();
       myMsg->setLast_city(current_city);
       int left = myMsg->getN();
       myMsg->setN(left-1);
       if(left == 1){
           for(int i = 0; i < n; ++i){
              cGate *out = gate("out",i);
              const char* next= out->getNextGate()->getOwnerModule()->getFullName();

              if(strcmp(next, "b") == 0){
                  sendDelayed(myMsg,  abs(dis(gen)), "out", i);
                  //send(myMsg, "out", i);
                  break;
              }
          }
       }
       else{
           int next = rand()%n;
           while(strcmp(gate("out",next)->getNextGate()->getOwnerModule()->getFullName(),"a") == 0 || strcmp(gate("out",next)->getNextGate()->getOwnerModule()->getFullName(),"b") == 0){
               next = rand()%n;
           }
          sendDelayed(myMsg,  abs(dis(gen)), "out", next);
           //send(myMsg, "out", next);
       }
   }
   else{
       string next_city = s_.top();
       s_.pop();
       for(int i = 0; i < n; ++i){
           cGate *out = gate("out",i);
           const char* next= out->getNextGate()->getOwnerModule()->getFullName();
           if(next_city == string(next)){
               sendDelayed(myMsg,  abs(dis(gen)), "out", i);
              //send(myMsg, "out", i);

              break;
           }
        }

   }


}
void relay::finish(){
}

