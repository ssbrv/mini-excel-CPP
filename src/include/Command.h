#ifndef __COMMAND_H__
#define __COMMAND_H__

#include "Operation.h"
#include "Table.h"

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

class Command : public Operation
{
protected:
    Table &m_table;

public:
    Command(Table &table, const std::string &name, unsigned short arity);
    virtual ~Command() = 0;

    virtual std::string operator()(const std::vector<std::string> &opearand) = 0;
};

class Help : public Command
{
public:
    Help(Table &table);
    ~Help() override;
    std::string operator()(const std::vector<std::string> &operand) override;
};

class Oplist : public Command
{
public:
    Oplist(Table &table);
    ~Oplist() override;
    std::string operator()(const std::vector<std::string> &operand) override;
};

class Maxsize : public Command
{
public:
    Maxsize(Table &table);
    ~Maxsize() override;
    std::string operator()(const std::vector<std::string> &operand) override;
};

class Create : public Command
{
public:
    Create(Table &table);
    ~Create() override;
    std::string operator()(const std::vector<std::string> &operand) override;
};

class Import : public Command
{
public:
    Import(Table &table);
    ~Import() override;
    std::string operator()(const std::vector<std::string> &operand) override;
};

class Currsize : public Command
{
public:
    Currsize(Table &table);
    ~Currsize() override;
    std::string operator()(const std::vector<std::string> &operand) override;
};

class Assign : public Command
{
public:
    Assign(Table &table);
    ~Assign() override;
    std::string operator()(const std::vector<std::string> &operand) override;
};

class Show : public Command
{

public:
    Show(Table &table, const std::string &name);
    ~Show() override;
    void printTable(const std::vector<std::vector<std::string>> &table) const;
};

class Formula : public Show
{
public:
    Formula(Table &table);
    ~Formula() override;
    std::string operator()(const std::vector<std::string> &operand) override;
};

class Print : public Show
{
public:
    Print(Table &table);
    ~Print() override;
    std::string operator()(const std::vector<std::string> &operand) override;
};

class Export : public Command
{
public:
    Export(Table &table);
    ~Export() override;
    std::string operator()(const std::vector<std::string> &operand) override;
};

class Exit : public Command
{
    bool &m_toExit;

public:
    Exit(Table &table, bool &toExit);
    ~Exit() override;
    std::string operator()(const std::vector<std::string> &operand) override;
};

class CommandManager
{
    Table m_table;
    std::unordered_map<std::string, std::shared_ptr<Command>> m_commandDictionary;

public:
    CommandManager(bool &toExit);
    ~CommandManager() = default;

    std::shared_ptr<Command> determine(const std::string &supposedCommand) const;

    bool toExit() const;
};

#endif // __COMMAND_H__