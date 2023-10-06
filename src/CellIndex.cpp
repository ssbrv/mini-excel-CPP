#include "include/CellIndex.h"
#include "include/Utilities.h"
#include <string>
#include <stdexcept>

std::size_t columnNameToColumnIndex(const std::string &columnName) 
{
    if (columnName.empty())
        throw std::invalid_argument("Column name cannot be empty.");

    std::size_t columnIndex = 0;
    std::size_t power = 1;

    for (auto it = columnName.rbegin(); it != columnName.rend(); ++it)
    {
        char ch = *it;
        if (ch < 'A' || ch > 'Z')
            throw std::invalid_argument("Invalid column name.");

        columnIndex += (ch - 'A' + 1) * power;
        power *= 26;
    }

    return columnIndex - 1;
}
std::string columnIndexToColumnName(std::size_t columnIndex) 
{
    std::string columnName;
    columnIndex++;

    while (columnIndex > 0)
    {
        --columnIndex;
        columnName = static_cast<char>('A' + (columnIndex % 26)) + columnName;
        columnIndex /= 26;
    }

    return columnName;
}

std::size_t rowNameToRowIndex(const std::string &rowName)
{
    return stringToSize_t(rowName) - 1;
}

std::string rowIndexToRowName(const std::size_t &rowIndex) 
{
    return std::to_string(rowIndex + 1);    
}

CellIndex parseCellIndexName(const std::string &cellIndexName) 
{
    // Check if the cell index is empty
    if (cellIndexName.empty())
        throw std::invalid_argument("Cell index cannot be empty.");

    // Find the index where the column name ends
    std::size_t columnEnd = cellIndexName.find_first_of("1234567890");
    if (columnEnd == std::string::npos)
        throw std::invalid_argument("Invalid row name.");

    // Extract the column name and convert it to column index
    std::string columnName = cellIndexName.substr(0, columnEnd);
    std::size_t columnIndex = columnNameToColumnIndex(columnName);

    // Extract the row index and convert it to std::size_t
    std::string rowName = cellIndexName.substr(columnEnd);
    std::size_t rowIndex = rowNameToRowIndex(rowName);


    return CellIndex(columnIndex, rowIndex);
}

bool isValidRange(const CellIndex &first, const CellIndex &last) { return (first.getColumnIndex() <= last.getColumnIndex() && first.getRowIndex() <= last.getRowIndex()); }

CellIndex::CellIndex(const std::string &cellIndexString) 
{
    *this = parseCellIndexName(cellIndexString);
}
CellIndex::CellIndex(const std::size_t &columnIndex, const std::size_t &rowIndex) : m_columnIndex(columnIndex), m_rowIndex(rowIndex) {}

std::size_t CellIndex::getColumnIndex() const { return m_columnIndex; }
std::size_t CellIndex::getRowIndex() const { return m_rowIndex; }

std::string CellIndex::getColumnName() const { return columnIndexToColumnName(m_columnIndex); }
std::string CellIndex::getRowName() const { return rowIndexToRowName(m_rowIndex); }
std::string CellIndex::getCellIndexName() const { return columnIndexToColumnName(m_columnIndex) + rowIndexToRowName(m_rowIndex); }

bool CellIndex::operator==(const CellIndex &other) const { return (m_columnIndex == other.m_columnIndex && m_rowIndex == other.m_rowIndex); }
bool CellIndex::operator!=(const CellIndex &other) const { return !(*this == other); }
bool CellIndex::operator<(const CellIndex &other) const 
{
    if (m_columnIndex < other.m_columnIndex)
        return m_rowIndex <= other.m_rowIndex;

    if (m_rowIndex < other.m_rowIndex)
        return m_columnIndex == other.m_columnIndex;
    
    return false;
}
bool CellIndex::operator>(const CellIndex &other) const 
{
    if (m_columnIndex > other.m_columnIndex)
        return m_rowIndex >= other.m_rowIndex;

    if (m_rowIndex > other.m_rowIndex)
        return m_columnIndex == other.m_columnIndex;

    return false;
}
bool CellIndex::operator<=(const CellIndex &other) const 
{
    return (m_columnIndex <= other.m_columnIndex && m_rowIndex <= other.m_rowIndex);
}
bool CellIndex::operator>=(const CellIndex &other) const 
{
    return (m_columnIndex >= other.m_columnIndex && m_rowIndex >= other.m_rowIndex);
}