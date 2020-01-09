#include "ex3.h"
#include <string>
#include <unordered_map>
#include "ex1.h"
#include "interpreter.h"
#include "Expression.h"
#include <thread>

/**/
unordered_map<string,Command*> commandMap;
unordered_map<string,varTypes> varMap;
unordered_map<string, double> inputSimMap;
unordered_map <string, string > mapMatch;


/*The program calls for a presser, who interprets flight commands that create server-to-client communications,
 * and thus activates the flight-geer*/
int main() {
    Command command;
    command.parser();
    command.parserEnd(true);
    std::vector<string> lexerVector;

    return 0;
}