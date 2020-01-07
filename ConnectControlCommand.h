//
// Created by eyal222222 on 1/4/20.
//

#include "ex3.h"
#include <thread>
#ifndef UNTITLED8__CONNECTCONTROLCOMMAND_H_
#define UNTITLED8__CONNECTCONTROLCOMMAND_H_




class ConnectControlCommand : public Command {
public:
    ConnectControlCommand(){}
    virtual int execute(std::vector<string>::iterator);
    void outputSim(int serverValue);
};

#endif //UNTITLED8__CONNECTCONTROLCOMMAND_H_