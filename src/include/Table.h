#ifndef __TABLE_H__
#define __TABLE_H__

#include "ConstantManager.h"
#include "CellIndex.h"
#include "Operator.h"

#include <string>
#include <vector>
#include <unordered_set>

class Table;

class Cell
{
    CellIndex m_cellIndex;

    std::vector<std::string> m_infix;
    std::vector<std::string> m_postfix;
    std::string m_output;
    std::string m_backup;

    std::unordered_set<Cell *> m_incomingVertices;
    std::unordered_set<Cell *> m_outgoingVertices;
    std::vector<Cell *> m_waitList;

    bool m_processed = false;
    bool m_requested = false;

    friend class Table;

public:
    Cell(const CellIndex &cellIndex);
    Cell() = default;

    bool isOrAccepts(const CellIndex &cellIndex) const;
};

class Table
{
    bool m_initialized = false;
    OperatorManager m_operatorManager;
    ConstantManager m_constantManager;

    std::vector<std::vector<Cell *>> m_table;

    Cell *find(const CellIndex &cellIndex);

    // converts infix to postfix using shunting yard algorithm. checks for invalid operators
    std::vector<std::string> postfixConvertion(const std::vector<std::string> &infix);

    // evaluates the postfix, formats the data. checks for cycle, invalid arity
    void evaluate(Cell *cellPtr, const std::vector<std::string> &postfix);

    // updates cells that reference given cell, when it gets new value
    void updateIncomingVertices(Cell *cellPtr);

    // clears the whole table
    void clear();

    std::string unpack(Cell *cellPtr, const std::string &cellIndexName);

    void processWaitList(Cell *cellPtr);

    void backupIncomingVertices(Cell *cellPtr);

    void import(std::vector<std::vector<std::string>> table);
    void evaluateRecursive(Cell *cellPtr);
    std::string unpackRecursive(Cell *cellPtr, const std::string &operand);
    std::string requestRecursive(Cell *cellPtr);

public:
    Table() = default;
    ~Table();

    void recreate(const size_t &numberOfColumns, const size_t &numberOfRows);
    
    void tryImport(std::vector<std::vector<std::string>> table);

    bool isWithinRange(const CellIndex &cellIndex) const;

    std::string modify(const CellIndex &cellIndex, const std::vector<std::string> &input);

    std::string getInfix(const CellIndex &cellIndex);
    std::string getOutput(const CellIndex &cellIndex);

    size_t numberOfColumns() const;
    size_t numberOfRows() const;
    size_t numberOfColumnsMax() const;
    size_t numberOfRowsMax() const;

    std::vector<std::vector<std::string>> getTable(const CellIndex &first, const CellIndex &last) const;
    std::vector<std::vector<std::string>> getTableFormula(const CellIndex &first, const CellIndex &last) const;
};

#endif // __TABLE_H__