#include "include/Const.h"
#include "include/Utilities.h"
#include "include/Table.h"
#include "include/CellIndex.h"
#include "include/Operator.h"
#include "include/ConstantManager.h"

#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <stack>

/* Cell */

Cell::Cell(const CellIndex &cellIndex) : m_cellIndex{cellIndex} {}

bool Cell::isOrAccepts(const CellIndex &cellIndex) const
{
    if (m_cellIndex == cellIndex)
        return true;

    for (Cell *incomingVertex : m_incomingVertices)
        if (incomingVertex->isOrAccepts(cellIndex))
            return true;

    return false;
}

/* Table */

Cell *Table::find(const CellIndex &cellIndex)
{
    if (!m_initialized)
        throw std::runtime_error("Table is not initialized");

    if (!isWithinRange(cellIndex))
        throw std::out_of_range("Cell index out of range");

    return m_table[cellIndex.getColumnIndex()][cellIndex.getRowIndex()];
}

std::vector<std::string> Table::postfixConvertion(const std::vector<std::string> &infix)
{
    std::vector<std::string> postfix;
    std::stack<std::shared_ptr<Operator>> opstack;
    std::shared_ptr<Operator> op;
    std::stack<int> arityForMultinaryOperatorCounter;

    for (const std::string &token : infix)
    {
        // if command
        if ((op = m_operatorManager.determine(token)) != nullptr)
        {
            // if not function
            if (op->getPrecedence() > 1)
            {
                while (!opstack.empty() && opstack.top()->getName() != "(" && opstack.top()->getPrecedence() <= op->getPrecedence())
                {
                    postfix.push_back(opstack.top()->getName());
                    opstack.pop();
                }
            }

            // if left parenthesis, start counting arity for supposed multinary operator
            if (op->getName() == "(")
                arityForMultinaryOperatorCounter.push(1);

            opstack.push(op);
            continue;
        }

        // if coma
        if (token == ",")
        {
            // flush all operators until left parenthesis
            while (!opstack.empty())
            {
                if (opstack.top()->getName() == "(")
                    break;

                postfix.push_back(opstack.top()->getName());
                opstack.pop();
            }

            if (opstack.empty())
                throw std::runtime_error("Missing left parentheses.");

            // increment arity for supposed multinary operator
            int top = arityForMultinaryOperatorCounter.top();
            arityForMultinaryOperatorCounter.pop();
            arityForMultinaryOperatorCounter.push(++top);

            continue;
        }

        if (token != ")")
        {
            // push supposed operand
            postfix.push_back(token);
            continue;
        }

        // if right parenthesis
        
        while (!opstack.empty())
        {
            if (opstack.top()->getName() == "(")
                break;
            
            postfix.push_back(opstack.top()->getName());
            opstack.pop();
        }
        if (opstack.empty())
            throw std::runtime_error("Missing left parentheses.");

        // pop left bracket
        opstack.pop();

        // if multinary operator push its arity as operand
        if (!opstack.empty() && !opstack.top()->getArity() && opstack.top()->getName() != "(")
            postfix.push_back(std::to_string(arityForMultinaryOperatorCounter.top()));

        arityForMultinaryOperatorCounter.pop();        
    }

    // flush remaining operators
    while (!opstack.empty())
    {
        if (opstack.top()->getName() == "(")
            throw std::runtime_error("Missing right parentheses.");

        postfix.push_back(opstack.top()->getName());
        opstack.pop();
    }

    return postfix;
}

void Table::evaluate(Cell *cellPtr, const std::vector<std::string> &postfix)
{
    std::stack<std::string> stack;
    std::shared_ptr<Operator> op;
    size_t arity;

    for (const std::string &token : postfix)
    {
        op = m_operatorManager.determine(token);

        // if operand
        if (op == nullptr)
        {
            stack.push(token);
            continue;
        }

        // if operator

        // set proper arity
        if ((arity = op->getArity()) == 0)
        {
            if (stack.empty())
                throw std::runtime_error("In " + cellPtr->m_cellIndex.getCellIndexName() + " missing operands: " + vectorToString(postfix, ' '));

            arity = stringToSize_t(stack.top());
            stack.pop();
        }

        // form operand container
        std::vector<std::string> operands{arity};
        for (int i = arity - 1; i >= 0; --i)
        {
            if (stack.empty())
                throw std::runtime_error("In " + cellPtr->m_cellIndex.getCellIndexName() + " missing operands: " + vectorToString(postfix, ' '));

            operands[i] = unpack(cellPtr, stack.top());
            stack.pop();
        }

        // evaluate
        stack.push((*op)(operands));
    }

    if (stack.size() > 1)
        throw std::runtime_error("In " + cellPtr->m_cellIndex.getCellIndexName() + " too many operands in the expression: " + vectorToString(postfix, ' '));

    if (stack.size() < 1)
        throw std::runtime_error("In " + cellPtr->m_cellIndex.getCellIndexName() + " not enough operands in the expression: " + vectorToString(postfix, ' '));

    std::string result = unpack(cellPtr, stack.top());
    if (!result.empty() && !isDoubleOrQuotedString(result))
        throw std::runtime_error("In " + cellPtr->m_cellIndex.getCellIndexName() + " invalid operand: " + result + "'");

    cellPtr->m_backup = cellPtr->m_output;
    cellPtr->m_output = result;

    try 
    {
        updateIncomingVertices(cellPtr);
    }
    catch (const std::exception &e)
    {
        backupIncomingVertices(cellPtr);
        throw std::runtime_error(e.what());
    }
}

std::string Table::unpack(Cell *cellPtr, const std::string &operand)
{
    CellIndex cellIndex;

    try 
    {
        cellIndex = parseCellIndexName(operand);
    }
    catch (const std::exception &e)
    {
        std::shared_ptr<Constant> constant = m_constantManager.determine(operand);
        return (constant == nullptr) ? operand : constant->getValue();
    }

    Cell *outgoingVertex = find(cellIndex);
    cellPtr->m_waitList.push_back(outgoingVertex);
    if (cellPtr->isOrAccepts(outgoingVertex->m_cellIndex))
        throw std::runtime_error("Index '" + outgoingVertex->m_cellIndex.getCellIndexName() + "' caused a cycle!");

    return outgoingVertex->m_output;
}

void Table::evaluateRecursive(Cell *cellPtr)
{
    std::stack<std::string> stack;
    std::shared_ptr<Operator> op;
    size_t arity;

    for (const std::string &token : cellPtr->m_postfix)
    {
        op = m_operatorManager.determine(token);

        // if operand
        if (op == nullptr)
        {
            stack.push(token);
            continue;
        }

        // if operator

        // set proper arity
        if ((arity = op->getArity()) == 0)
        {
            if (stack.empty())
                throw std::runtime_error("In " + cellPtr->m_cellIndex.getCellIndexName() + " missing operands: " + vectorToString(cellPtr->m_postfix, ' '));

            arity = stringToSize_t(stack.top());
            stack.pop();
        }

        // form operand container
        std::vector<std::string> operands{arity};
        for (int i = arity - 1; i >= 0; --i)
        {
            if (stack.empty())
                throw std::runtime_error("In " + cellPtr->m_cellIndex.getCellIndexName() + " missing operands: " + vectorToString(cellPtr->m_postfix, ' '));

            operands[i] = unpackRecursive(cellPtr, stack.top());
            stack.pop();
        }

        // evaluate
        stack.push((*op)(operands));
    }

    if (stack.size() > 1)
        throw std::runtime_error("In " + cellPtr->m_cellIndex.getCellIndexName() + " too many operands in the expression: " + vectorToString(cellPtr->m_postfix, ' '));

    if (stack.size() < 1)
        throw std::runtime_error("In " + cellPtr->m_cellIndex.getCellIndexName() + " not enough operands in the expression: " + vectorToString(cellPtr->m_postfix, ' '));

    cellPtr->m_output = unpackRecursive(cellPtr, stack.top());
    if (!cellPtr->m_output.empty() && !isDoubleOrQuotedString(cellPtr->m_output))
        throw std::runtime_error("In " + cellPtr->m_cellIndex.getCellIndexName() + " invalid operand: " + cellPtr->m_output + "'");
}

std::string Table::unpackRecursive(Cell *cellPtr, const std::string &operand)
{
    CellIndex cellIndex;

    try
    {
        cellIndex = parseCellIndexName(operand);
    }
    catch (const std::exception &e)
    {
        std::shared_ptr<Constant> constant = m_constantManager.determine(operand);
        return (constant == nullptr) ? operand : constant->getValue();
    }

    Cell *outgoingVertex = find(cellIndex);

    cellPtr->m_outgoingVertices.insert(outgoingVertex);
    outgoingVertex->m_incomingVertices.insert(cellPtr);

    return requestRecursive(outgoingVertex);
}

void Table::processWaitList(Cell *cellPtr) 
{
    for (Cell *outgoingVertex : cellPtr->m_outgoingVertices)
        outgoingVertex->m_incomingVertices.erase(cellPtr);
    cellPtr->m_outgoingVertices.clear();

    for (Cell *cellPtrToProccess : cellPtr->m_waitList)
    {
        cellPtr->m_outgoingVertices.insert(cellPtrToProccess);
        cellPtrToProccess->m_incomingVertices.insert(cellPtr);
    }
}

void Table::updateIncomingVertices(Cell *cellPtr)
{
    for (Cell *incomingVertex : cellPtr->m_incomingVertices) 
        evaluate(incomingVertex, incomingVertex->m_postfix);
}

void Table::backupIncomingVertices(Cell *cellPtr)
{
    cellPtr->m_output = cellPtr->m_backup;

    for (Cell *incomingVertex : cellPtr->m_incomingVertices)
        backupIncomingVertices(incomingVertex);
}

void Table::clear()
{
    if (!m_initialized)
        return;

    for (auto &row : m_table)
    {
        for (auto *cell : row)
            delete cell;
        row.clear();
    }

    m_table.clear();
    m_initialized = false;
}

Table::~Table() { clear(); }

void Table::recreate(const size_t &numberOfColumns, const size_t &numberOfRows)
{
    if (numberOfColumns > numberOfColumnsMax())
        throw std::runtime_error("Too many columns. Maximum is " + std::to_string(numberOfColumnsMax()) + ".");

    if (numberOfRows > numberOfRowsMax())
        throw std::runtime_error("Too many rows. Maximum is " + std::to_string(numberOfRowsMax()) + ".");

    if (numberOfColumns == 0 || numberOfRows == 0)
        throw std::runtime_error("Number of rows / columns annot be 0.");

    clear();

    m_table.resize(numberOfColumns);
    for (size_t i = 0; i < numberOfColumns; ++i)
    {
        m_table[i].resize(numberOfRows);
        for (size_t j = 0; j < numberOfRows; ++j)
            m_table[i][j] = new Cell(CellIndex{i, j});
    }

    m_initialized = true;
}

std::string Table::requestRecursive(Cell *cellPtr) 
{
    if (cellPtr->m_requested && !cellPtr->m_processed)
        throw std::runtime_error("Index '" + cellPtr->m_cellIndex.getCellIndexName() + "' caused a cycle!");

    cellPtr->m_requested = true;

    if (cellPtr->m_processed || cellPtr->m_postfix.empty())
        return cellPtr->m_output;

    evaluateRecursive(cellPtr);
    cellPtr->m_processed = true;

    return cellPtr->m_output;
}

void Table::import(std::vector<std::vector<std::string>> table)
{
    if (table.empty())
        throw std::runtime_error("Empty file.");

    recreate(table[0].size(), table.size());

    for (size_t i = 0; i < table.size(); ++i)
    {
        if (table[i].size() != table[0].size())
            throw std::runtime_error("Non-matching row sizes.");

        for (size_t j = 0; j < table[i].size(); ++j)
        {
            Cell *cellPtr = find(CellIndex{j, i});

            cellPtr->m_infix = split(table[i][j], &isWhiteSpace);
            cellPtr->m_postfix = postfixConvertion(cellPtr->m_infix);
        }
    }

    for (auto &v : m_table)
        for (Cell *cellPtr : v)
            cellPtr->m_backup = requestRecursive(cellPtr);
}

void Table::tryImport(std::vector<std::vector<std::string>> table)
{
    try
    {
        import(table);
    }
    catch (const std::exception &e)
    {
        clear();
        throw std::runtime_error(std::string("Table discarded: ") + e.what());
    }
}

bool Table::isWithinRange(const CellIndex &cellIndex) const
{
    return (m_initialized &&
            m_table.size() > cellIndex.getColumnIndex() &&
            m_table[cellIndex.getColumnIndex()].size() > cellIndex.getRowIndex());
}

std::string Table::modify(const CellIndex &cellIndex, const std::vector<std::string> &input)
{
    Cell *cellPtr = find(cellIndex);
    cellPtr->m_waitList.clear();

    std::vector<std::string> postfix = postfixConvertion(input);
    evaluate(cellPtr, postfix);
    processWaitList(cellPtr);

    cellPtr->m_infix = input;
    cellPtr->m_postfix = postfix;
    cellPtr->m_backup = cellPtr->m_output;

    return cellPtr->m_output;
}

std::string Table::getInfix(const CellIndex &cellIndex) { return vectorToString(find(cellIndex)->m_infix, ' '); }

std::string Table::getOutput(const CellIndex &cellIndex) { return find(cellIndex)->m_output; }

size_t Table::numberOfColumns() const 
{
    if (!m_initialized)
        throw std::runtime_error("Table not initialized.");

    return m_table.size();
}

size_t Table::numberOfRows() const 
{
    if (!m_initialized)
        throw std::runtime_error("Table not initialized.");

    return m_table[0].size();
}

std::vector<std::vector<std::string>> Table::getTable(const CellIndex &first, const CellIndex &last) const
{
    if (!m_initialized)
        throw std::runtime_error("Table not initialized.");

    if (!isWithinRange(first) || !isWithinRange(last))
        throw std::runtime_error("Out of range.");

    if (!isValidRange(first, last))
        throw std::runtime_error("Invalid range.");

    std::vector<std::vector<std::string>> table(last.getColumnIndex() - first.getColumnIndex() + 1);

    for (size_t row = first.getRowIndex(); row <= last.getRowIndex(); ++row)
        for (size_t column = first.getColumnIndex(); column <= last.getColumnIndex(); ++column)
            table[column].push_back(m_table[column][row]->m_output);
            
    return table;
}

std::vector<std::vector<std::string>> Table::getTableFormula(const CellIndex &first, const CellIndex &last) const
{
    if (!m_initialized)
        throw std::runtime_error("Table not initialized.");

    if (!isWithinRange(first) || !isWithinRange(last))
        throw std::runtime_error("Out of range.");

    if (!isValidRange(first, last))
        throw std::runtime_error("Invalid range.");

    std::vector<std::vector<std::string>> table(last.getColumnIndex() - first.getColumnIndex() + 1);

    for (size_t row = first.getRowIndex(); row <= last.getRowIndex(); ++row)
        for (size_t column = first.getColumnIndex(); column <= last.getColumnIndex(); ++column)
            table[column].push_back(vectorToString(m_table[column][row]->m_infix, ' '));

    return table;
}

size_t Table::numberOfColumnsMax() const { return NUMBER_OR_COLUMNS_MAX; }
size_t Table::numberOfRowsMax() const { return NUMBER_OR_ROWS_MAX; }