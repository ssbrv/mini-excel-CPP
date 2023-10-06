#ifndef __PROGRAMMANAGER_H__
#define __PROGRAMMANAGER_H__

#include "StdinReader.h"
#include "Command.h"

#include <queue>
#include <string>

class ProgramManager
{
    bool m_toExit = false;
    StdinReader m_stdinReader;
    CommandManager m_commandManager;

    std::string evaluate(std::vector<std::string> &tokens) const;

public:
    ProgramManager();
    ~ProgramManager();

    void runSpreadsheetEditor();
};

#endif // __PROGRAMMANAGER_H__