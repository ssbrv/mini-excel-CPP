#include "include/Const.h"
#include "include/ProgramManager.h"
#include "include/Utilities.h"

#include <string>
#include <vector>
#include <stack>
#include <queue>
#include <iostream>
#include <stdexcept>
#include <sstream>

std::string ProgramManager::evaluate(std::vector<std::string> &tokens) const
{
    size_t i = 0;
    std::vector<std::string> operands;

    std::shared_ptr<Command> supposedCommand = m_commandManager.determine(tokens[i]);

    if (supposedCommand == nullptr)
    {
        // push the cell index
        operands.push_back(tokens[i++]);

        if (tokens.size() == i || (supposedCommand = m_commandManager.determine(tokens[i++])) == nullptr || supposedCommand->getName() != "=")
            throw std::runtime_error("Unknown command '" + operands.back() + "'");
    }
    else
    {
        // skip the command
        i++;

        if (supposedCommand->getArity() > tokens.size() - i)
            throw std::runtime_error("Not enough operands for command '" + supposedCommand->getName() + "'. Expected " + std::to_string(supposedCommand->getArity()) + " operands");

        if (supposedCommand->getArity() < tokens.size() - i)
            throw std::runtime_error("Too many operands for command '" + supposedCommand->getName() + "'. Expected " + std::to_string(supposedCommand->getArity()) + " operands");
    }

    // push operands
    operands.insert(operands.end(), tokens.begin() + i, tokens.end());

    return (*supposedCommand)(operands);
}

ProgramManager::ProgramManager() : m_commandManager(m_toExit) {}
ProgramManager::~ProgramManager() {}

void ProgramManager::runSpreadsheetEditor() 
{
    std::cout << WELCOME_MESSAGE << std::endl;

    std::vector<std::string> tokens;
    std::string input;
    std::string output;

    while (!m_toExit)
    {
        std::cout << "> " << std::flush;

        try 
        {
            input = m_stdinReader.read();
        }
        catch (const std::exception& e) 
        {
            std::cout << "Failed reading input: " << e.what() << std::endl;
            continue;
        }

        if (m_stdinReader.isEOF())
            throw std::runtime_error("Unexpected EOF");
        if (m_stdinReader.isInterrupted())
            throw std::runtime_error("Interrupted");

        input = trim(input);
        if (input.empty())
            continue;

        try 
        {
            tokens = split(input, &isWhiteSpace);    
        }
        catch (const std::exception& e)
        {
            std::cout << "Failed parsing: " << e.what() << std::endl;
            continue;
        }

        try
        {
            output = evaluate(tokens);
        }
        catch (const std::exception& e)
        {
            std::cout << "Failed evaluating: " << e.what() << std::endl;
            continue;
        }

        std::cout << output << std::endl;
    }
}
