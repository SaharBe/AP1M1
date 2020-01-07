//
// Created by sahar on 12/11/2019.
//

#include "interpreter.h"
#include "ex1.h"
#include "Expression.h"
#include <stack>
#include <queue>
#include <string>
#include <map>
#include <iostream>
#include <regex>
#include "ex3.h"
#define REG_VAR "[A-z_][A-z_0-9]*"
#define REG_VALUE "-?[1-9][0-9]*.?[0-9]*|0"
#define REG_START_OP "[(+*/]"
#define REG_DOUBLE_OP "[+/*-][+/*-]"



extern unordered_map<string,Command*> commandMap;
extern unordered_map<string,varTypes> varMap;
extern unordered_map<string, double> inputSimMap;
extern  unordered_map <string, string > mapMatch;
using namespace std;

Interpreter::Interpreter() {}



Expression *Interpreter::interpret(string exp) {

    map<string, int> pre;
    pre.insert(pair<string ,int>("*",3)) ;
    pre.insert(pair<string ,int>("/",3)) ;
    pre.insert(pair<string ,int>("+",2)) ;
    pre.insert(pair<string ,int>("-",2)) ;
    queue<string> output;
    stack<string> operat;
    stack<Expression*> newStack;

     regex opSart(REG_START_OP);
     regex opDouble("[+*/-][+*/-]");
     regex varStart("-[(][A-z_]");
     if(regex_search(exp, opDouble)|| regex_match("" + exp[0],opSart) || regex_search(exp,varStart)){
         throw "illigal expression";
     }
    string number = "";
    string variable = "";
    bool isNegNum = false;


    Expression *expression;
    Expression *left;
    Expression *right;


    string valueA;
    for (int i = 0; i < exp.length() ; i++) {

        //if the token is  variable
        if(exp[i] >= 'A' && exp[i] <= 'Z' || exp[i] >= 'a' && exp[i] <= 'z' || exp[i] == '_' ){
                    variable = variable + exp[i];
            i++;
            while(exp[i] != '+' && exp[i] != '-' && exp[i] != '*' && exp[i] != '/'
                  &&  exp[i] != ')' && exp[i] != '(' ){
                variable = variable + exp[i];
                i++;
            }
            valueA = to_string(varMap[variable].value);
            output.push(valueA);
            variable ="";
        }



        //Check if is unary
        if((exp[i] == '-' || exp[i] == '+' )&&( exp[i-1] =='(' || i == 0)){
            if(exp[i] == '-'){
                if (i < exp.size() && isdigit(exp[i+1])){
                    isNegNum = true;
                    continue;
                } else{
                    output.push("-1");
                    exp[i] = '*';
                }
            }
            if(exp[i] == '+')
                continue;

        }


        //if the token is a number
        if (isdigit(exp[i]) || exp[i] == '.') {
            number = number + exp[i];
            if(isdigit(exp[i+1])){
                continue;
            }else{
                operat.push(number);
                number = "";
            }

        } else if ( exp[i] == '+' || exp[i] == '-' ||  exp[i] == '*' || exp[i] == '/'
                   ||  exp[i] == ')' || exp[i] == '(') {
            if (number != ""){
                if(isNegNum){
                    number = "-" + number;
                    isNegNum = false;
                }
                output.push(number);
                number = "";
            }

         //if the stack is empty
        } if (operat.empty() == true) {
            string s(1, exp[i]);
            operat.push( s );
         //if the token is an operator

        } else {
            if(exp[i] == '+' || exp[i] == '-' ||  exp[i] == '*'
                    || exp[i] == '/') {
                string s(1, exp[i]);

                while ((pre[operat.top()] > pre[s]) || (pre[operat.top()] == pre[s]) &&
                                                       operat.top() != "(") {
                    output.push(operat.top());
                    operat.pop();
                    if(operat.empty()){
                        break;
                    }
                }
                operat.push(s);
            }
            string s(1, exp[i]);
                if(s == "("){
                    operat.push(s);
                }
                if(s == ")"){
                    while(operat.top() != "("){
                        output.push(operat.top());
                        operat.pop();
                    }
                    operat.pop();
                }

            }
        }
    //empty the stack
    int max = operat.size();

    for (int i = 0; i < max; i++) {
        if(operat.size() == 0){
            break;
        }
        if(operat.top() == "("){
            operat.pop();
        }
        output.push(operat.top());
        operat.pop();
    }

    Expression *ex;
    //create the expression
    while (!output.empty()) {

        while (output.front() != "+" && output.front() != "-" && output.front() != "*" &&output.front() != "/") {
            if(output.front() == "")
                throw "illigal math expression";
            ex = new Value(stod(output.front()));

            newStack.push(ex);
            output.pop();
        }
        string oper = output.front();
        output.pop();

        if( newStack.size() < 2 ){
            throw "error";
        }


        right = newStack.top();
        newStack.pop();
        left = newStack.top();
        newStack.pop();
        if (oper == "+") {
            expression = new Plus(left, right);

            newStack.push(expression);
            //garbage.push_back(expression);
            //  return expression;
        }
        if (oper == "-") {
            expression = new Minus(left, right);
            newStack.push(expression);
            // garbage.push_back(expression);
        }
        if (oper == "*") {
            expression = new Mul(left, right);
            newStack.push(expression);
            //  garbage.push_back(expression);
        }
        if (oper == "/") {
            expression = new Div(left, right);
            newStack.push(expression);
            //garbage.push_back(expression);
        }

    }

    return expression;
}



void Interpreter::setVariables(string str) {
   // string token = "";
  //  string tokenVal = "";

    queue<string> tokensQueue;
    queue<string> temp;
    queue<string> tokenVariableQueue;
    queue<string> tokenValueQueue;
    //split with ;
    vector<string> tokens = split(str, ";");
    for(string token: tokens){
        vector<string> var = split(token , "=");
        if(var.size() != 2){
            throw "error!";
        }
        regex rVar (REG_VAR);
        regex rVal (REG_VALUE);
        if(!regex_match(var[0], rVar )|| !regex_match(var[1], rVal)){
            throw "error!";
        }
        mapMatch[var[0]] = (var[1]);

    }

}

Interpreter::~Interpreter() {
    for (Expression *expression: garbage) {
        delete ((expression));
    }
}
vector<string> Interpreter::split(string str, string sep) {
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