#ifndef __CELLINDEX_H__
#define __CELLINDEX_H__

#include <string>

class CellIndex;

std::size_t columnNameToColumnIndex(const std::string &columnName);
std::string columnIndexToColumnName(std::size_t columnIndex);

std::size_t rowNameToRowIndex(const std::string &rowName);
std::string rowIndexToRowName(const std::size_t &rowIndex);

CellIndex parseCellIndexName(const std::string &cellIndexName);

bool isValidRange(const CellIndex &first, const CellIndex &last);

class CellIndex
{
    std::size_t m_columnIndex;
    std::size_t m_rowIndex;

public:
    CellIndex(const std::string &cellIndexString);
    CellIndex(const std::size_t &columnIndex, const std::size_t &rowIndex);
    CellIndex() = default;

    std::size_t getColumnIndex() const;
    std::size_t getRowIndex() const;

    std::string getColumnName() const;
    std::string getRowName() const;

    std::string getCellIndexName() const;

    bool operator==(const CellIndex &other) const;
    bool operator!=(const CellIndex &other) const;
    bool operator<(const CellIndex &other) const;
    bool operator>(const CellIndex &other) const;
    bool operator<=(const CellIndex &other) const;
    bool operator>=(const CellIndex &other) const;
};

#endif // __CELLINDEX_H__
