//
// Created by sahar on 12/11/2019.
//

#ifndef UNTITLED2_INTERPRETER_H
#define UNTITLED2_INTERPRETER_H

#include <string>
#include <functional>
#include <unordered_map>
#include "Expression.h"
#include <vector>

extern   unordered_map <string, string > mapMatch;

using namespace std;

class Interpreter {
private:
    vector<Expression*> garbage;
public:

   Expression* interpret(string string);
   void setVariables(string str);
   vector<string> split(string str, string sep);
   Interpreter();
    virtual ~Interpreter();


};


#endif //UNTITLED2_INTERPRETER_H
