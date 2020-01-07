

#include "ex3.h"
#include <thread>

#ifndef UNTITLED8__OPENSERVERCOMMAND_H_
#define UNTITLED8__OPENSERVERCOMMAND_H_


class OpenServerCommand : public Command {
public:
    OpenServerCommand(){}
    virtual int execute(std::vector<string>::iterator);
    void inputSim(int client_sockek);
    void varFromSim(int client_sockek);
    void updateMapFromSim(double array[]);
};
#endif //UNTITLED8__OPENSERVERCOMMAND_H_