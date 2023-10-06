#include "include/StdinReader.h"

StdinReader::StdinReader()
{
    // uncomment if you want to catch ctrl + c
    // std::signal(SIGINT, signalHandler);
}
 
void StdinReader::signalHandler(int signal)
{
    if (signal == SIGINT)
        m_interrupted = true;
}

std::string StdinReader::read()
{
    std::string input;

    if (!(std::getline(std::cin, input)))
    {
        if (!(std::cin.eof()))
            throw std::ios_base::failure("Input error");
        m_eof = true;
    }

    return input;
}

bool StdinReader::isInterrupted() const { return m_interrupted; }
bool StdinReader::isEOF() const { return m_eof; }

bool StdinReader::m_interrupted = false;