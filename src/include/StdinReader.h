#ifndef __STDINREADER_H__
#define __STDINREADER_H__

#include <iostream>
#include <string>
#include <stdexcept>
#include <csignal>

class StdinReader
{
private:
    static bool m_interrupted;
    bool m_eof = false;

public:
    StdinReader();
    static void signalHandler(int signal);
    std::string read();
    bool isInterrupted() const;
    bool isEOF() const;
};

#endif // __STDINREADER_H__