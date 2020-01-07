#include "ex3.h"
#include <iostream>
#include <string.h>
#include <string>
#include <vector>
#include <unordered_map>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "interpreter.h"
#include <mutex>
#include "ConnectControlCommand.h"

using namespace std;

extern unordered_map<string,Command*> commandMap;
extern unordered_map<string,varTypes> varMap;
extern unordered_map<string, double> inputSimMap;
extern  unordered_map <string, string > mapMatch;
extern vector<string> outputVector;

mutex mut;

void ConnectControlCommand::outputSim(int serverValue) {
    std:: cout << "its in outputSim" << std::endl;
    int connectSocket = socket(AF_INET,SOCK_STREAM,0);
    if (connectSocket == -1) {
        //error
        std:: cerr << "Couldn't create a socket" << std::endl;
    }
    sockaddr_in address;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_addr.s_addr = inet_addr("127.0.0.1");
    address.sin_port = htons(serverValue);

    int is_connect = connect(connectSocket,(struct sockaddr*) &address,sizeof(address));
    if (is_connect ==-1){
        //error
        std:: cerr <<"Could not connect to host server"<<std::endl;

    }
    printf("starting sending\n");
    string simString = "";

    string valValue = "0";

    string message1 ;
    int is_sent =0 ;


    while(is_sent != -1) {

        /// //the Crash is here:
        //   printf("sending\n")'
        ///

        while (!outputVector.empty()) {
            cout << outputVector[0] <<endl;

            mut.lock();
            //if val is a sender

            message1 = "set " + outputVector.back();
            outputVector.pop_back();
            is_sent = send(connectSocket, message1.c_str(), message1.size(), 0);
            mut.unlock();

        }
        sleep(1);
    }



       close(connectSocket);

    }




int ConnectControlCommand ::execute(std::vector<string>::iterator iterator) {
    int index=2;
    iterator+=2;
    string ipString=*iterator;

    iterator++;
    string value = "";
    while(*iterator != "\n"){
        if(*iterator == ")"){
            iterator++;
            continue;
        }
        value = value + *iterator;
        iterator++;
        index++;
    }
    Interpreter* i = new Interpreter();

    double expValue = 0;
    if(index > 3 ){
        Expression* expression =i->interpret(value);
        expValue = (expression->calculate());
    }else{
        expValue = stod(value);

    }


    std::thread t3(&ConnectControlCommand::outputSim,this,expValue);
   //t3.join();
    t3.detach();

    return index + 3;
}