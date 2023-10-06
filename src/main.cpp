#include <iostream>
#include <sstream>
#include <stdexcept>
#include <cmath>
#include <memory>
#include <cstring>
#include <string>
#include <vector>
#include <stack>
#include <queue>

#include "../libs/json.hpp"
#include "include/Const.h"
#include "include/Utilities.h"
#include "include/StdinReader.h"
#include "include/CellIndex.h"
#include "include/Operation.h"
#include "include/Command.h"
#include "include/Operator.h"
#include "include/Table.h"
#include "include/ProgramManager.h"

void testReadOperation() 
{
    OperatorManager operatorManager;
    std::string suppousedOperator;

    std::cout << "Please, insert opeartion: " << std::flush;
    std::cin >> suppousedOperator;

    std::shared_ptr<Operator> operatorSharedPtr = operatorManager.determine(suppousedOperator);
    if (operatorSharedPtr.get() == nullptr)
    {
        std::cout << "Invalid operator." << std::endl;
        return;
    }
    std::cout << "ARITY: " << operatorSharedPtr->getArity() << " PRECEDENCE: " << operatorSharedPtr->getPrecedence() << std::endl;
}

void testReadUserInput()
{
    int *a = new int;

    StdinReader stdinReader{};
    std::string input;

    do
    {
        try
        {
            input = stdinReader.read();
        }
        catch (const std::exception &e)
        {
            std::cout << "Exception caught: " << e.what() << std::endl;
            break;
        }

        if (stdinReader.isInterrupted()) 
        {
            std::cout << "Interruption detected." << std::endl;
            break;
        }

        std::cout << "User input: " << input << std::endl;

    } while (true);

    delete a;
}

void testOperators() 
{
    OperatorManager operatorManager;
    std::vector<std::string> operands = {"102", "2"};

    std::shared_ptr<Operator> add = operatorManager.determine("+");
    std::shared_ptr<Operator> sub = operatorManager.determine("-");
    std::shared_ptr<Operator> mult = operatorManager.determine("*");
    std::shared_ptr<Operator> div = operatorManager.determine("/");
    std::shared_ptr<Operator> rep = operatorManager.determine(".");
    std::shared_ptr<Operator> padl = operatorManager.determine("padl");
    std::shared_ptr<Operator> padr = operatorManager.determine("padr");
    std::shared_ptr<Operator> avg = operatorManager.determine("avg");

    std::cout << "(+) ADD:" << (*add)(operands) << std::endl;
    std::cout << "(-) SUB:" << (*sub)(operands) << std::endl;
    std::cout << "(*) MULT:" << (*mult)(operands) << std::endl;
    std::cout << "(/) DIV:" << (*div)(operands) << std::endl;

    operands[0] = quoteString(operands[0]);

    std::cout << "(.) REP:" << (*rep)(operands) << std::endl;

    operands[1] = "10";
    operands.push_back("\"-\"");

    std::cout << "(padl) PADL:" << (*padl)(operands) << std::endl;
    std::cout << "(padr) PADR:" << (*padr)(operands) << std::endl;

    operands[0] = "1";
    operands[1] = "2";
    operands[2] = "3";

    std::cout << "(avg) AVG:" << (*avg)(operands) << std::endl;
}

void testConfig() 
{
    initializeConstValues();
    std::cout << ADDITION_CONCATENATION_OPERATOR_NAME << std::endl;
}

void testProgramManager() 
{
    
}

int main() 
{
    try 
    {
        initializeConstValues();
    }
    catch (const std::exception &e)
    {
        std::cout << "Exception caught: " << e.what() << std::endl;
        return 0;
    }

    ProgramManager programManager;

    try
    {
        programManager.runSpreadsheetEditor();
    }
    catch (const std::exception &e)
    {
        std::cout << "Exception caught: " << e.what() << std::endl;
    }

    return 0;
}