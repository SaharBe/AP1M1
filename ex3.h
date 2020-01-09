
#include <fstream>
#include <iostream>
#include <string.h>
#include <string>
#include <stdlib.h>
#include <vector>
#include <unordered_map>
#include <sys/socket.h>
#include <netinet/in.h>
#include <thread>
#include <mutex>


using namespace std;

#ifndef EX3__EX3_H_
#define EX3__EX3_H_



class dummy{
public:
    static mutex& get_mut();
};

class varTypes {
public:
    double value;
    string sim;
    bool senderOrListener;
};

class Command {
public:
    //  static unordered_map<string,Command*> commandMap;
    virtual std::vector<string> lexer(char *file);

    void parser();

    // unordered_map<string,varTypes> varMap;
    virtual int execute(std::vector<string>::iterator);

    Command();

    void parserEnd(bool parserFinished);

    bool parserDone();
};


class VarDefineCommand : public Command {  //--
public:
    VarDefineCommand() {}

    virtual int execute(std::vector<string>::iterator);
};

class SimDefineCommand : public Command {
public:
    SimDefineCommand() {};

    virtual int execute(std::vector<string>::iterator);
};

class PrintCommand : public Command { //--
public:
    PrintCommand() {}

    virtual int execute(std::vector<string>::iterator);
};

class SleepCommand : public Command { //--
public:
    SleepCommand() {}

    virtual int execute(std::vector<string>::iterator);
};

class ConditionParser : public Command {
public:
    virtual int execute(std::vector<string>::iterator) {};

    ConditionParser() {};

    virtual bool operSwitch(std::vector<string>::iterator);

};

class LoopCommand : public ConditionParser {
public:
    LoopCommand() {}

    virtual int execute(std::vector<string>::iterator);

};

class IfCommand : public ConditionParser {
public:
    IfCommand() {}

    virtual int execute(std::vector<string>::iterator);


};

class CommendUpdateVar : public Command {
public:
    CommendUpdateVar() {};

    virtual int execute(std::vector<string>::iterator);

};

class functionCommand : public Command {
public:
    functionCommand() {};

    virtual int execute(std::vector<string>::iterator);
};

class ex3 {


};


#endif //EX3__EX3_H_