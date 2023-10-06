#include "include/Command.h"
#include "include/Operation.h"
#include "include/Table.h"
#include "include/CellIndex.h"
#include "include/Utilities.h"
#include "include/Const.h"
#include "include/StdinReader.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

Command::Command(Table &table, const std::string &name, unsigned short arity) : Operation{name, arity}, m_table{table} {}
Command::~Command() {}

Help::Help(Table &table) : Command{table, HELP_COMMAND_NAME, 0} {}
Help::~Help() {}
std::string Help::operator()(const std::vector<std::string> &operand)
{ 
    return HELP_COMMAND_OUTPUT; 
}

Oplist::Oplist(Table &table) : Command{table, OPLIST_COMMAND_NAME, 0} {}
Oplist::~Oplist() {}
std::string Oplist::operator()(const std::vector<std::string> &operand)
{ 
    return OPLIST_COMMAND_OUTPUT; 
}

Maxsize::Maxsize(Table &table) : Command{table, MAXSIZE_COMMAND_NAME, 0} {}
Maxsize::~Maxsize() {}
std::string Maxsize::operator()(const std::vector<std::string> &operand) 
{
    CellIndex first{0, 0}, last{m_table.numberOfColumnsMax() - 1, m_table.numberOfRowsMax() - 1};
    return MAXSIZE_COMMAND_OUTPUT + std::to_string(m_table.numberOfColumnsMax()) + "x" + std::to_string(m_table.numberOfRowsMax()) + " (" + first.getCellIndexName() + ":" + last.getCellIndexName() + ").";
}

Create::Create(Table &table) : Command{table, CREATE_COMMAND_NAME, 2} {}
Create::~Create() {}
std::string Create::operator()(const std::vector<std::string> &operand)
{
    size_t numberOfColumns = stringToSize_t(operand[0]);
    size_t numberOfRows = stringToSize_t(operand[1]);

    m_table.recreate(numberOfColumns, numberOfRows);

    return CREATE_COMMAND_OUTPUT + std::to_string(m_table.numberOfColumns()) + "x" + std::to_string(m_table.numberOfRows());
}

Import::Import(Table &table) : Command{table, IMPORT_COMMAND_NAME, 1} {}
Import::~Import() {}
std::string Import::operator()(const std::vector<std::string> &operand)
{
    std::string fileName = unquoteString(operand[0]);
    std::ifstream file(fileName);

    if (!file.is_open())
        throw std::runtime_error{"Could not open file " + fileName};

    std::vector<std::vector<std::string>> table;
    std::string row;

    while (std::getline(file, row))
        table.push_back(split(row, &isComa));

    file.close();
    m_table.tryImport(table);
    return std::to_string(m_table.numberOfColumns()) + "x" + std::to_string(m_table.numberOfRows()) + IMPORT_COMMAND_OUTPUT + operand[0];
}

Currsize::Currsize(Table &table) : Command{table, CURRSIZE_COMMAND_NAME, 0} {}
Currsize::~Currsize() {}
std::string Currsize::operator()(const std::vector<std::string> &operand) 
{
    CellIndex first{0, 0}, last{m_table.numberOfColumns() - 1, m_table.numberOfRows() - 1};
    return CURRSIZE_COMMAND_OUTPUT + std::to_string(m_table.numberOfColumns()) + "x" + std::to_string(m_table.numberOfRows()) + " (" + first.getCellIndexName() + ":" + last.getCellIndexName() + ").";
}

Assign::Assign(Table &table) : Command{table, ASSIGN_COMMAND_NAME, 2} {}
Assign::~Assign() {}
std::string Assign::operator()(const std::vector<std::string> &operand)
{
    CellIndex cellIndex{operand[0]};
    return ASSIGN_COMMAND_OUTPUT + cellIndex.getCellIndexName() + " = " + m_table.modify(cellIndex, {operand.begin() + 1, operand.end()});
}

Show::Show(Table &table, const std::string &name) : Command{table, name, 1} {}
Show::~Show() {}
void Show::printTable(const std::vector<std::vector<std::string>> &table) const
{
    std::vector<std::string> tableFormatted = formatTable(table, '|', 1);
    size_t table_width = tableFormatted[0].length();

    size_t i = 0;
    char input = '\0';

    while (input != 'q' && ! std::cin.eof())
    {
        std::system("clear");

        size_t bound = std::min(SCREEN_WIDTH, table_width - i);

        std::cout << std::string(bound, '-') << std::endl;
        for (auto &line : tableFormatted) 
        {
            std::cout << line.substr(i, bound) << std::endl;
            std::cout << std::string(bound,'-') << std::endl;
        }

        std::cout << "Press 'a' to scroll left, 'd' to scroll right, or 'q' to quit: ";
        std::cin >> input;

        if (input == 'a' && i > 0)
            --i;
        else if (input == 'd' && table_width - i >= SCREEN_WIDTH)
            ++i;
    }
}

Formula::Formula(Table &table) : Show{table, FORMULA_COMMAND_NAME} {}
Formula::~Formula() {}
std::string Formula::operator()(const std::vector<std::string> &operand)
{
    std::size_t colon = operand[0].find_first_of(":");
    if (colon == std::string::npos)
    {
        CellIndex cellIndex{operand[0]};
        return FORMULA_COMMAND_OUTPUT + cellIndex.getCellIndexName() + " = " + m_table.getInfix(cellIndex);
    }

    std::string begin = operand[0].substr(0, colon);
    std::string end = operand[0].substr(colon + 1);

    CellIndex beginCellIndex{begin};
    CellIndex endCellIndex{end};

    std::vector<std::vector<std::string>> table = m_table.getTableFormula(beginCellIndex, endCellIndex);
    printTable(table);

    return FORMULA_COMMAND_OUTPUT;
}

Print::Print(Table &table) : Show{table, PRINT_COMMAND_NAME} {}
Print::~Print() {}
std::string Print::operator()(const std::vector<std::string> &operand)
{
    std::size_t colon = operand[0].find_first_of(":");
    if (colon == std::string::npos)
    {
        CellIndex cellIndex{operand[0]};
        return PRINT_COMMAND_OUTPUT + cellIndex.getCellIndexName() + " = " + m_table.getOutput(cellIndex);
    }

    std::string begin = operand[0].substr(0, colon);
    std::string end = operand[0].substr(colon + 1);

    CellIndex beginCellIndex{begin};
    CellIndex endCellIndex{end};

    std::vector<std::vector<std::string>> table = m_table.getTable(beginCellIndex, endCellIndex);    

    printTable(table);

    return PRINT_COMMAND_OUTPUT;
}

Export::Export(Table &table) : Command{table, EXPORT_COMMAND_NAME, 1} {}
Export::~Export() {}
std::string Export::operator()(const std::vector<std::string> &operand)
{
    std::string fileName = unquoteString(operand[0]);
    std::ofstream file(fileName);

    if (!file.is_open())
        throw std::runtime_error{"Could not open file " + fileName};

    std::vector<std::vector<std::string>> table = m_table.getTableFormula(CellIndex{0, 0}, CellIndex{m_table.numberOfColumns() - 1, m_table.numberOfRows() - 1});
    std::vector<std::string> tableFormatted = formatTable(table, ',', 1);
    file << vectorToString(tableFormatted, '\n');

    file.close();
    return EXPORT_COMMAND_OUTPUT + operand[0];
}

Exit::Exit(Table &table, bool &toExit) : Command{table, EXIT_COMMAND_NAME, 0}, m_toExit{toExit} {}
Exit::~Exit() {}
std::string Exit::operator()(const std::vector<std::string> &operand)
{
    m_toExit = true;
    return EXIT_COMMAND_OUTPUT;
}

CommandManager::CommandManager(bool &toExit)
{
    m_commandDictionary[HELP_COMMAND_NAME] = std::make_shared<Help>(m_table);
    m_commandDictionary[OPLIST_COMMAND_NAME] = std::make_shared<Oplist>(m_table);
    m_commandDictionary[MAXSIZE_COMMAND_NAME] = std::make_shared<Maxsize>(m_table);
    m_commandDictionary[CREATE_COMMAND_NAME] = std::make_shared<Create>(m_table);
    m_commandDictionary[IMPORT_COMMAND_NAME] = std::make_shared<Import>(m_table);
    m_commandDictionary[CURRSIZE_COMMAND_NAME] = std::make_shared<Currsize>(m_table);
    m_commandDictionary[ASSIGN_COMMAND_NAME] = std::make_shared<Assign>(m_table);
    m_commandDictionary[FORMULA_COMMAND_NAME] = std::make_shared<Formula>(m_table);
    m_commandDictionary[PRINT_COMMAND_NAME] = std::make_shared<Print>(m_table);
    m_commandDictionary[EXPORT_COMMAND_NAME] = std::make_shared<Export>(m_table);
    m_commandDictionary[EXIT_COMMAND_NAME] = std::make_shared<Exit>(m_table, toExit);
}

std::shared_ptr<Command> CommandManager::determine(const std::string &supposedCommand) const
{
    auto iter = m_commandDictionary.find(supposedCommand);
    return (iter == m_commandDictionary.end()) ? nullptr : iter->second;
}

