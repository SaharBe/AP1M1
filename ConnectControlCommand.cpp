#include "ex3.h"
#include <iostream>
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
#include <exception>

using namespace std;

extern unordered_map<string,Command*> commandMap;
extern unordered_map<string,varTypes> varMap;
extern unordered_map<string, double> inputSimMap;
extern  unordered_map <string, string > mapMatch;
extern vector<string> outputVector;



class TimeoutException;

void ConnectControlCommand::outputSim(int serverValue) {

        int connectSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (connectSocket == -1) {
            //error
            std::cerr << "Couldn't create a socket" << std::endl;
        }
        sockaddr_in address;
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = inet_addr("127.0.0.1");
        address.sin_port = htons(serverValue);

        int is_connect = connect(connectSocket, (struct sockaddr *) &address, sizeof(address));
       // cout << "Connected" << endl;
        if (is_connect == -1) {
            //error
            std::cerr << "Could not connect to host server" << std::endl;

        }

        string message1;
        int is_sent = 0;
        bool check = true;
        while (!Command().parserDone()) {
            dummy::get_mut().lock();
            while (!outputVector.empty()) {
             //   cout << outputVector[0] << endl;

                //if val is a sender
                message1 = "set " + outputVector.front();
                //cout << message1 << endl;
                outputVector.erase(outputVector.begin());
                is_sent = send(connectSocket, message1.c_str(), message1.size(), 0);
                if (is_sent == -1) {
                    cout << "Could not send" << endl;
                }
                check = false;
            }
            dummy::get_mut().unlock();
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
    bool itisExpression = 0;
    for(int j = 0; j< value.length();j++){
        if(value[j] == '+' || value[j] == '-' ||value[j] == '*' ||value[j] == '/') {
            itisExpression = true;
            break;
        }
    }
    if(itisExpression){
        Expression* expression =i->interpret(value);
        expValue = (expression->calculate());
    }else{
        expValue = stod(value);

    }


   std::thread t3(&ConnectControlCommand::outputSim,this,(int) expValue);
    t3.detach();

    return index + 3;
}