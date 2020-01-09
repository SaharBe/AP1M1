

#include "ex3.h"
#include <thread>

#ifndef UNTITLED8__OPENSERVERCOMMAND_H_
#define UNTITLED8__OPENSERVERCOMMAND_H_


class OpenServerCommand : public Command {
public:
    OpenServerCommand(){}
    virtual int execute(std::vector<string>::iterator);
    vector<string> split(string str, string sep);
    void varFromSim(int client_sockek);
   void updateMapFromSim(double array[], int size);
};
#endif //UNTITLED8__OPENSERVERCOMMAND_H_