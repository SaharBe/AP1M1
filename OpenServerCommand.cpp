
#include "ex3.h"
#include <iostream>
#include <string.h>
#include <string>
#include <stdlib.h>
#include <vector>
#include <unordered_map>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <thread>
#include "interpreter.h"
#include <mutex>
#include "OpenServerCommand.h"

using namespace std;

extern unordered_map<string,Command*> commandMap;
extern unordered_map<string,varTypes> varMap;
extern unordered_map<string, double> inputSimMap;
extern  unordered_map <string, string > mapMatch;
mutex m;


void OpenServerCommand:: inputSim(int client_sockek) {
    char buffer[10000] = {0};
    int valRead = -1;

    valRead = read(client_sockek,buffer,10000);

    m.lock();
    int j =0;
    double valueArray[36];
    string temp = "";
    for(int i= 0; i < 35 ; i++){
        if(buffer[j] == ','){
            j++;

        }else{
            while(buffer[j] != ','){
                temp = temp + buffer[j];
                j++;
            }

            valueArray[i] = stod(temp);
            temp = "";
        }


    }
    updateMapFromSim(valueArray);
    m.unlock();
}



void OpenServerCommand::varFromSim(int client_sockek) {
    int valread = 0;
    int j =0;
    double valueArray[36];
    char buffer[100000] = {0};
    string temp = "";
    while(valread != -1){
        ///   lock
        m.lock();
        valread = read(client_sockek, buffer,100000 );
        for(int i= 0; i < 35 ; i++){

            while(buffer[j] != ','){
                temp = temp + buffer[j];
            }
            valueArray[i] = stod(temp);
            temp = "";
        }
        updateMapFromSim(valueArray);
        std:: cout <<buffer<<std::endl;
        m.unlock();
        sleep(0.1);
    }
    close(client_sockek);

}
void OpenServerCommand :: updateMapFromSim(double array[]) {
    inputSimMap["/instrumentation/airspeed-indicator/indicated-speed-kt"]= array[0];
    inputSimMap["/sim/time/warp"]= array[1];
    inputSimMap["/controls/switches/magnetos"]= array[2];
    inputSimMap["/instrumentation/heading-indicator/offset-deg"]= array[3];
    inputSimMap["/instrumentation/altimeter/indicated-altitude-ft"]= array[4];
    inputSimMap["/instrumentation/altimeter/pressure-alt-ft"]= array[5];
    inputSimMap["/instrumentation/attitude-indicator/indicated-pitch-deg"]= array[6];
    inputSimMap["/instrumentation/attitude-indicator/indicated-roll-deg"]= array[7];
    inputSimMap["/instrumentation/attitude-indicator/internal-pitch-deg"]= array[8];
    inputSimMap["/instrumentation/attitude-indicator/internal-roll-deg"]= array[9];
    inputSimMap["/instrumentation/encoder/indicated-altitude-ft"]= array[10];
    inputSimMap["/instrumentation/encoder/pressure-alt-ft"]= array[11];
    inputSimMap["/instrumentation/gps/indicated-altitude-ft"]= array[12];
    inputSimMap["instrumentation/gps/indicated-ground-speed-kt"]= array[13];
    inputSimMap["/instrumentation/gps/indicated-vertical-speed"]= array[14];
    inputSimMap["/instrumentation/heading-indicator/indicated-heading-deg"]= array[15];
    inputSimMap["/instrumentation/magnetic-compass/indicated-heading-deg"]= array[16];
    inputSimMap["/instrumentation/slip-skid-ball/indicated-slip-skid"]= array[17];
    inputSimMap["/instrumentation/turn-indicator/indicated-turn-rate"]= array[18];
    inputSimMap["/instrumentation/vertical-speed-indicator/indicated-speed-fpm"]= array[19];
    inputSimMap["/controls/flight/aileron"]= array[20];
    inputSimMap["/controls/flight/elevator"]= array[21];
    inputSimMap["/controls/flight/rudder"]= array[22];
    inputSimMap["/controls/flight/flaps"]= array[23];
    inputSimMap["/controls/engines/engine/throttle"]= array[24];
    inputSimMap["/controls/engines/current-engine/throttle"]= array[25];
    inputSimMap["/controls/switches/master-avionics"]= array[26];
    inputSimMap["/controls/switches/starter"]= array[27];
    inputSimMap["/engines/active-engine/auto-start"]= array[28];
    inputSimMap["/controls/flight/speedbrake"]= array[29];
    inputSimMap["/sim/model/c172p/brake-parking"]= array[30];
    inputSimMap["/controls/engines/engine/primer"]= array[31];
    inputSimMap["/controls/engines/current-engine/mixture"]= array[32];
    inputSimMap["/controls/switches/master-bat"]= array[33];
    inputSimMap["/controls/switches/master-alt"]= array[34];
    inputSimMap["/engines/engine/rpm"]= array[35];

}

//create server,first thread waits for input from sim.second thread reads from sim until the end.
int OpenServerCommand::execute(std::vector<string>::iterator iterator) {
    int index = 0;
    int counterValue = 0;
    iterator+=2;
    index+=2;
    string value = "";
    double expValue;
    while (*iterator != "\n") {
        if(*iterator==")"){
            iterator++;
            continue;
        }
        value = value + *iterator;
        iterator++;
        index++;
        counterValue++;
    }
    //its only one number
    if (counterValue == 1) {
        expValue = stod(value);
    } else {
        Interpreter *i = new Interpreter();
        Expression *expression = i->interpret(value);
        expValue = (expression->calculate());
    }
    int socketFD = socket(AF_INET, SOCK_STREAM, 0);
    if (socketFD == -1) {
        //error
        std::cerr << "Could not create a socket\n" << std::endl;
        return -1;
    }

    sockaddr_in address;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(expValue);
    address.sin_family = AF_INET;

    int bind1 = bind(socketFD, (struct sockaddr *) &address, sizeof(address));
    if (!bind1 == -1) {
        //error
        std:: cerr << "Could not bind the socket to an IP\n" << std:: endl;
        return -2;
    }
    if (listen(socketFD, 5) == -1) {
        //error
        std:: cerr << "Error during listening command\n" << std:: endl;
    }
    int client_sockek = accept(socketFD, (struct sockaddr *) &address, (socklen_t *) &address);
    if (client_sockek == -1) {
        //error
        std:: cerr << "Error accepting client\n" << std:: endl;
    }
    std::thread t(&OpenServerCommand::inputSim,this,client_sockek);

    t.join();
    std::thread t2(&OpenServerCommand::varFromSim,this,expValue);

    t2.detach();

    return index +2 ;
}