#include "ex3.h"
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <unistd.h>
#include "interpreter.h"
#include "ConnectControlCommand.h"
#include "OpenServerCommand.h"
#include <chrono>

using namespace std;


mutex mut;


extern unordered_map<string,Command*> commandMap;
extern unordered_map<string,varTypes> varMap;
extern unordered_map<string, double> inputSimMap;
extern  unordered_map <string, string > mapMatch;
bool endOfParser;
vector<string> outputVector;

mutex & dummy::get_mut() {
    return mut;
}

int Command::execute(std::vector<string>::iterator ) {}

Command::Command() {
    endOfParser = false;
}

std::vector<string> Command::lexer(char* file) {
    std::vector<string> lexerVector;
    ifstream fileStream ("fly.txt");
    bool merchaotFlag = 0;

    if(!fileStream.is_open()) {
        printf(" didnt find file\n");
    }
    char c;
    string temp;
    while(fileStream.get(c)) {
        //if a string
        if(c=='"'){
            fileStream.get(c);
            while(c!= '"')
            {
                temp+=c;
                fileStream.get(c);
            }
            lexerVector.push_back(temp);
            temp="";
            continue;
        }
        else if(c=='(') {
            if(!temp.empty()) {
                lexerVector.push_back(temp);
            }
            lexerVector.push_back("(");
                temp = "";
                continue;

        }
        else if(c==')') {
            if(!temp.empty()) {
                lexerVector.push_back(temp);
            }
            lexerVector.push_back(")");
            temp = "";
            continue;
        }
        else if(c == '\n'){
            if(temp.empty()) {
                temp = "\n";
                lexerVector.push_back(temp);
                temp = "";
            }
            if(!temp.empty()){
                lexerVector.push_back(temp);
                temp = "\n";
                lexerVector.push_back(temp);
                temp = "";

            }

        }else if(c == '\t') {
            if(temp.empty()) {
                temp = "\t";
                lexerVector.push_back(temp);
                temp = "";
            }
            if(!temp.empty()){
                lexerVector.push_back(temp);
                temp = "\t";
                lexerVector.push_back(temp);
                temp = "";

            }
        }
        else if(c!='(' && c!=' ' && c != ')' && c!= '"' && c!=',') {
            temp += c;
        }
        else {
            if(!temp.empty())
                lexerVector.push_back(temp);
            temp = "";
        }



    }

  //  printf("completed building stringVector\n");
    fileStream.close();
    return lexerVector;
}


/*sleep by the allotted time*/
int SleepCommand::execute(std::vector<string>::iterator iterator) {
    iterator+=2;
    this_thread::sleep_for(chrono::milliseconds(stoi(*iterator)));
    return 5;
}
/*print to the consol what we got to print*/
int PrintCommand::execute(std::vector<string>::iterator iterator) {
    iterator+=2;
    if(varMap.find(*iterator) != varMap.end()) {
        int varValue = varMap.at(*iterator).value;
        cout << varValue << endl;
    }
    else {
        cout << *iterator << endl;
    }
    return 5;
}

/*Checking in which context we got the variable and adjusting its values ​​for it.*/
int VarDefineCommand::execute(std::vector<string>::iterator iterator) {

    int index =0;

    //go to var name
    iterator++;
    index++;

    if(varMap.find(*iterator) == varMap.end() ) {
        varTypes var_types;
        string varKey =*iterator;
        iterator++;
        index++;
        double newValue = 0;
        bool isAVar=false;
        //go to <- or -> or = operator
        if(*iterator == "="){
            iterator++;
            index++;
            string value = "";
            while(*iterator != "\n"){
                value = value + *iterator;
                iterator++;
                index++;
            }if(index > 4) {
                Interpreter *i = new Interpreter();
                Expression *e = i->interpret(value);
                double newValue = e->calculate();
                varMap[varKey].value = newValue;
            }
            else {
                for (int i = 0; i < value.length(); i++) {
                    if (!isdigit(value[i])) {
                        if (value[i] == '.') {
                            continue;
                        } else {
                            //not a number
                            isAVar = true;
                            newValue = varMap.at(value).value;
                            varMap[varKey].value = newValue;
                            break;
                        }
                    }

                }
                if (isAVar == false){
                    newValue=stod(value);
                    varMap[varKey].value = newValue;
                }
            }
            if(varMap[varKey].senderOrListener == 0) {
                string toVectorString = varMap[varKey].sim + " " + to_string(varMap[varKey].value)+ "\r\n";
                mut.lock();
                outputVector.push_back(toVectorString);
                mut.unlock();
            }
            return index + 1;

        }
        if(*iterator == "->") {
            var_types.senderOrListener = 0;
            iterator+=3;
            index+=4;
          //  varTypes *currentVarType = new varTypes();
            var_types.sim = *iterator;

            varMap.emplace(varKey,var_types);
        }
        else if(*iterator == "<-") {
            var_types.senderOrListener =1;
            iterator+=3;
            index+=4;

            var_types.sim = *iterator;
            varMap.emplace(varKey,var_types);
        }


    }
    else {
        throw printf("why it declared twice :(");
    }
    //2 more to get from "sim" to the next line
    return index+ 2;

}

/*Calls by the loop and the if commands,
 * checking the condition that guides them.*/
bool ConditionParser:: operSwitch(std::vector<string>::iterator iterator) {
    iterator += 1;
    string var = *iterator;
    iterator += 1;
    string oper = *iterator;
    iterator += 1;
    int maxAlt = stod(*iterator);

    //varMap[var].value == maxAlt
    if(oper == "<"){
        return varMap[var].value < maxAlt;
    } else if(oper == ">"){
        return varMap[var].value > maxAlt;
    }else if(oper == "=="){
        return varMap[var].value == maxAlt;
    }else if(oper == "<="){
        return varMap[var].value <= maxAlt;
    }else if(oper == ">="){
        return varMap[var].value >= maxAlt;
    }else if(oper == "!="){
        return varMap[var].value != maxAlt;
    }

}

/*if their is a function - we interpreter it here*/
int functionCommand::execute(std::vector<string>::iterator iterator) {
    bool flag = 0;
    bool thisIs =0;
    int index = 0;

    while (*iterator != "{") {
        iterator++;
        index++;
    }
    string soger = *iterator;
    while(!thisIs){
        if(*iterator == "}" && flag == 0){
            return index;
        }
        if(*iterator != soger){
            iterator++;
            index++;
        }else{
            flag = 1;
            while (*iterator != "}"){
                iterator++;
                index++;
            }
            flag =0;
        }

    }
}

/*Goes through all the while loop we came across in the press
 * - and interprets its internal commands*/
int LoopCommand ::execute(std::vector<string>::iterator iterator) {
    int index=0;
   std::vector<string>::iterator operIterator = iterator;
    bool condition = operSwitch(iterator);
    iterator += 1;
    int counter = 1;
    int indexOfConditiLine = 1;
    string var = *iterator;

    double expValue ;
    bool isAVar = false;
    double newValue;


    string expression = "";
    string tempVar = "";
    varTypes equalTemp;

    Command *c;
    int returnValCommand;

    vector<string> vector;
    //
    while (*iterator != "{") {
        iterator++;
        counter++;
        indexOfConditiLine++;
    }
    //skip the \t
    iterator+=3;
    counter+=3;
    indexOfConditiLine+=4;

    std::vector<string>::iterator savedIterator = iterator;

    int flag = 0;
    while (operSwitch(operIterator)) {
        iterator = savedIterator;
        while (*iterator != "}") {
            if(*iterator  == "\t"){
                iterator++;
                if (flag == 0) {
                    counter++;
                }
            }else if ((*(iterator + 1) == "=" && expression == "")) {
                tempVar = *iterator;

                iterator += 2;

                if (flag == 0) {
                    counter+=2;
                }

                while (*iterator != "\t" && *iterator != "\n") {
                    if (flag == 0) {
                       counter++;
                    }
                    expression = "" + expression + *iterator;
                    iterator++;
                    index++;
                }
                iterator++;

                if (flag == 0) {
                    counter++;
                }

                if (index > 1) {
                    Interpreter *i ;
                    Expression *exp = i->interpret(expression);
                    expValue = (exp->calculate());
                    varMap[tempVar].value = expValue;
                    expression = "";
                    index = 0;
                } else {
                    for (int i = 0; i < expression.length(); i++) {
                        if (!isdigit(expression[i])) {
                            if (expression[i] == '.') {
                                continue;
                            } else {
                                //not a number
                                isAVar = true;
                                newValue = varMap.at(expression).value;
                                varMap[tempVar].value = newValue;
                                break;
                            }
                        }

                    }
                    if (isAVar == false) {
                        newValue = stod(expression);
                        varMap[tempVar].value = newValue;
                    }
                }
            }
            else {
                    c = commandMap.at(*iterator);


                    returnValCommand = c->execute(iterator);
                    iterator +=returnValCommand;

                    if (flag == 0) {
                        counter += returnValCommand;
                        //counter is on }....
                        counter+=2;
                        //now he finish
                    }
                }



            }

        flag = 1;
        iterator -= (counter-indexOfConditiLine);



        }

          //  cout << "done" << endl;
            return counter;
}


int SimDefineCommand:: execute(std::vector<string>::iterator ) {
    return 2;
}

/*Chaking the conditions in the "if" */
int IfCommand ::execute(std::vector<string>::iterator iterator) {
    int index=0;
    std::vector<string>::iterator operIterator = iterator;
    bool condition = operSwitch(iterator);
    iterator += 1;
    int counter = 1;
    int indexOfConditiLine = 1;
    string var = *iterator;

    double expValue ;
    bool isAVar = false;
    double newValue;


    string expression = "";
    string tempVar = "";
    varTypes equalTemp;

    Command *c;
    int returnValCommand;

    vector<string> vector;
    //
    while (*iterator != "{") {
        iterator++;
        counter++;
        indexOfConditiLine++;
    }
    //skip the \t
    iterator+=3;
    counter+=3;
    indexOfConditiLine+=4;

    std::vector<string>::iterator savedIterator = iterator;


    if (operSwitch(operIterator)) {
        iterator = savedIterator;
        while (*iterator != "}") {
            if(*iterator  == "\t"){
                iterator++;

                counter++;
            }else if ((*(iterator + 1) == "=" && expression == "")) {
                tempVar = *iterator;
                //-------
                iterator += 2;

                counter+=2;
                while (*iterator != "\t" && *iterator != "\n") {

                    counter++;
                    expression = "" + expression + *iterator;
                    iterator++;
                    index++;
                }
                iterator++;

                counter++;


                if (index > 1) {
                    Interpreter *i ;
                    Expression *exp = i->interpret(expression);
                    expValue = (exp->calculate());
                    varMap[tempVar].value = expValue;
                    expression = "";
                    index = 0;
                } else {
                    for (int i = 0; i < expression.length(); i++) {
                        if (!isdigit(expression[i])) {
                            if (expression[i] == '.') {
                                continue;
                            } else {
                                //not a number
                                isAVar = true;
                                newValue = varMap.at(expression).value;
                                varMap[tempVar].value = newValue;
                                break;
                            }
                        }

                    }
                    if (isAVar == false) {
                        newValue = stod(expression);
                        varMap[tempVar].value = newValue;
                    }
                }
            }
            else {
                c = commandMap.at(*iterator);

                returnValCommand = c->execute(iterator);
                iterator +=returnValCommand;

                counter += returnValCommand;
                //counter is on }....
                counter+=2;
                //now he finish
            }



        }

        iterator -= (counter-indexOfConditiLine);
    }

    cout << "done" << endl;
    return counter;
}
/*When the parser arrives in the loop to var,
 * we analyze here the meaning of it*/
int CommendUpdateVar::execute(std::vector<string>::iterator iterator) {
    int index = 0;
    string var = *iterator;

    iterator+=2;
    index+=2;

    double expValue = 0;
    string exp = "";
    while(*iterator != "\n"){
        exp = exp + *iterator;
        iterator++;
        index++;
    }//check if spaces work
    Interpreter* i = new Interpreter();

    bool itisExpression = 0;
    for(int j = 0; j< exp.length();j++){
        if(exp[j] == '+' || exp[j] == '-' ||exp[j] == '*' ||exp[j] == '/') {
            itisExpression = true;
            break;
        }
    }
    if(itisExpression){
        Expression* expression =i->interpret(exp);
        expValue = (expression->calculate());
    }else{
        expValue = stod(exp);

    }
    if(index > 3){
        Expression* expression =i->interpret(exp);
        expValue = (expression->calculate());
    }else{
        expValue = stod(exp);
    }
    varMap[var].value = expValue;
    index++;
    if(varMap[var].senderOrListener == 0) {
        string toVectorString = "" + varMap[var].sim + " " + to_string(varMap[var].value) +" \r\n";

        outputVector.push_back(toVectorString);
      //  cout << toVectorString << endl;
    }
    return index;

}


/*The Parser goes over all the values that ​​we got from the Lexer vector,
 * and analyzes their meaning */
void Command :: parser() {
    endOfParser = 0;
    char *file = (char *) "fly.txt";
    std::vector<string> stringVector = lexer(file);
    commandMap.insert({"openDataServer", new OpenServerCommand});
    commandMap.insert({"connectControlClient", new ConnectControlCommand});
    commandMap.insert({"var", new VarDefineCommand});
    commandMap.insert({"Print", new PrintCommand});
    commandMap.insert({"Sleep", new SleepCommand});
    ConditionParser *condition_parser = new LoopCommand;
    commandMap.insert({"while", condition_parser});
    ConditionParser *condition_parser1 = new IfCommand;
    commandMap.insert({"if", condition_parser1});
    CommendUpdateVar *commend_update_var = new CommendUpdateVar;

    int index = 0;
    while (index < stringVector.size()) {
        try {
            if (varMap.find(stringVector[index]) != varMap.end()) {

                index += commend_update_var->execute(stringVector.begin() + index);

            }else{
            Command *c = commandMap.at(stringVector[index]);

              index += c->execute(stringVector.begin() + index);
            }
        } catch (error_t) {
            cerr << "error in the while" << endl;
        }

    }
    endOfParser = 1;
}

void Command::parserEnd(bool parserFinished) {
    parserFinished = endOfParser;
}

bool Command::parserDone() {
    return endOfParser;
}


