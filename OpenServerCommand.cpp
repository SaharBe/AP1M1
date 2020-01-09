
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
mutex mute;


vector<string> OpenServerCommand::split(string str, string sep) {
    vector<string> answer;
    string token;
    string letter;
    for (int i = 0; i < str.size(); ++i) {
        letter = str[i];
        if (letter == sep) {
            if (!token.empty()) {
                answer.push_back(token);
            }
            token = "";
            continue;
        }
        token += str[i];
    }
    if (!token.empty()) {
        answer.push_back(token);
    }
    return answer;
}



string readFromServer(int client_sockek){
    int valread = 0;
    char buffer[1024] = {0};
    string temp = "";

    ///   lock
    //   mute.lock();
    valread = read(client_sockek, buffer,1024 );
    if(valread == -1) {
        cout << "error in reading" << endl;
        return nullptr;
    }
    return string(buffer);
}

void OpenServerCommand::varFromSim(int client_sockek) {
    string line = "";

    while(!Command().parserDone()){
       // int valread = 0;
        int j =0;
        double valueArray[36];
        char buffer[1024] = {0};
        string temp = "";
        while(line.find('\n') == string::npos){
            line += readFromServer(client_sockek);
        }
        string set_data = line.substr(0, line.find_first_of('\n'));

        //put the rest of the data to be a part of the next set
        line = line.substr(line.find_first_of('\n') + 1, line.size() - 1);

        vector<string> ValuuLIne = split(set_data, ",");

        double valus[36];
        for (int k = 0; k < ValuuLIne.size() ; ++k) {
            valus[k] =stod(ValuuLIne[k]);
        }

        updateMapFromSim(valus, 36);
    }

    close(client_sockek);

}

void OpenServerCommand :: updateMapFromSim(double array[], int size) {
    if(size != 36){
        cerr << "it is here";
    }
    mute.lock();
    inputSimMap["/instrumentation/airspeed-indicator/indicated-speed-kt"] = array[0];
    if(varMap["airspeed"].senderOrListener == 1) {
        varMap["airspeed"].value = array[0];
    }
    inputSimMap["/sim/time/warp"] = array[1];
    if(varMap["warp"].senderOrListener == 1) {
        varMap["warp"].value = array[1];
    }

    inputSimMap["/controls/switches/magnetos"]= array[2];
    if(varMap["magnetos"].senderOrListener == 1){
        varMap["magnetos"].value = array[2];
    }

    inputSimMap["/instrumentation/heading-indicator/offset-deg"]= array[3];
    if(varMap["heading"].senderOrListener == 1){
        varMap["heading"].value = array[3];
    }

    inputSimMap["/instrumentation/altimeter/indicated-altitude-ft"] = array[4];
    if(varMap["alt"].senderOrListener == 1) {
        varMap["alt"].value = array[4];
    }
    //
    inputSimMap["/instrumentation/altimeter/pressure-alt-ft"]= array[5];
    //varMap[""].value = array[5];

    inputSimMap["/instrumentation/attitude-indicator/indicated-pitch-deg"]= array[6];
    //   varMap[""].value = array[6];
    //
    inputSimMap["/instrumentation/attitude-indicator/indicated-roll-deg"]= array[7];
    if(varMap["roll"].senderOrListener == 1){
        varMap["roll"].value = array[7];
    }
    inputSimMap["/instrumentation/attitude-indicator/internal-pitch-deg"]= array[8];
    if(varMap["pitch"].senderOrListener == 1){
        varMap["pitch"].value = array[8];
    }
    //
    inputSimMap["/instrumentation/attitude-indicator/internal-roll-deg"]= array[9];
    //  varMap[""].value = array[9];
    //
    inputSimMap["/instrumentation/encoder/indicated-altitude-ft"]= array[10];
    //  varMap[""].value = array[10];

    inputSimMap["/instrumentation/encoder/pressure-alt-ft"]= array[11];
    //  varMap["heading"].value = array[11];
    //
    inputSimMap["/instrumentation/gps/indicated-altitude-ft"]= array[12];
    // varMap[""].value = array[12];

    inputSimMap["instrumentation/gps/indicated-ground-speed-kt"]= array[13];
    //  varMap[""].value = array[13];

    inputSimMap["/instrumentation/gps/indicated-vertical-speed"]= array[14];
    //  varMap[""].value = array[14];

    inputSimMap["/instrumentation/heading-indicator/indicated-heading-deg"]= array[15];
    //   varMap[""].value = array[15];

    inputSimMap["/instrumentation/magnetic-compass/indicated-heading-deg"]= array[16];
    // varMap[""].value = array[16];

    inputSimMap["/instrumentation/slip-skid-ball/indicated-slip-skid"]= array[17];
    // varMap[""].value = array[17];

    inputSimMap["/instrumentation/turn-indicator/indicated-turn-rate"]= array[18];
    // varMap[""].value = array[18];

    inputSimMap["/instrumentation/vertical-speed-indicator/indicated-speed-fpm"]= array[19];
    // varMap[""].value = array[19];

    inputSimMap["/controls/flight/aileron"]= array[20];
    if(varMap["aileron"].senderOrListener == 1){
        varMap["aileron"].value = array[20];
    }

    inputSimMap["/controls/flight/elevator"] = array[21];
    if(varMap["elevator"].senderOrListener == 1) {
        varMap["elevator"].value = array[21];
    }

    inputSimMap["/controls/flight/rudder"] = array[22];
    if(varMap["rudder"].senderOrListener == 1) {
        varMap["rudder"].value = array[22];
    }

    inputSimMap["/controls/flight/flaps"]= array[23];
    //  varMap[""].value = array[23];
    //
    inputSimMap["/controls/engines/engine/throttle"]= array[24];
    //  varMap[""].value = array[24];
    //
    inputSimMap["/controls/engines/current-engine/throttle"] = array[25];
    if(varMap["throttle"].senderOrListener == 1) {
        varMap["throttle"].value = array[25];
    }

    inputSimMap["/controls/switches/master-avionics"]= array[26];
    if(varMap["masteravionics"].senderOrListener == 1){
        varMap["masteravionics"].value = array[26];
    }
    inputSimMap["/controls/switches/starter"] = array[27];
    if(varMap["starter"].senderOrListener == 1) {
        varMap["starter"].value = array[27];
    }
    inputSimMap["/engines/active-engine/auto-start"] = array[28];
    if(varMap["autostart"].senderOrListener == 1) {
        varMap["autostart"].value = array[28];
    }

    inputSimMap["/controls/flight/speedbrake"]= array[29];
    if(varMap["breaks"].senderOrListener == 1){
        varMap["breaks"].value = array[29];
    }

    inputSimMap["/sim/model/c172p/brake-parking"] = array[30];
    if(varMap["brakeparking"].senderOrListener == 1) {
        varMap["brakeparking"].value = array[30];
    }

    inputSimMap["/controls/engines/engine/primer"] = array[31];
    if(varMap["primer"].senderOrListener == 1) {
        varMap["primer"].value = array[31];
    }

    inputSimMap["/controls/engines/current-engine/mixture"] = array[32];
    if(varMap["mixture"].senderOrListener == 1) {
        varMap["mixture"].value = array[32];
    }

    inputSimMap["/controls/switches/master-bat"] = array[33];
    if(varMap["masterbat"].senderOrListener == 1) {
        varMap["masterbat"].value = array[33];
    }

    inputSimMap["/controls/switches/master-alt"] = array[34];
    if(varMap["masterlat"].senderOrListener == 1) {
        varMap["masterlat"].value = array[34];
    }

    inputSimMap["/engines/engine/rpm"] = array[35];
    if(varMap["rpm"].senderOrListener == 1) {
        varMap["rpm"].value = array[35];
    }
    mute.unlock();

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
    /*//its only one number
    if (counterValue == 1) {
        expValue = stod(value);
    } else {
        Interpreter *i = new Interpreter();
        Expression *expression = i->interpret(value);
        expValue = (expression->calculate());
    }*/
    Interpreter* i = new Interpreter();

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
    if (listen(socketFD, 1) == -1) {
        //error
        std:: cerr << "Error during listening command\n" << std:: endl;
    }
    int client_sockek = accept(socketFD, (struct sockaddr *) &address, (socklen_t *) &address);
    if (client_sockek == -1) {
        //error
        std:: cerr << "Error accepting client\n" << std:: endl;
    }



    std::thread t2(&OpenServerCommand::varFromSim,this,client_sockek);


    t2.detach();

    return index +2 ;
}
