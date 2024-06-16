#include "include/Utilities.h"

#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <limits>

std::string trim(const std::string &str)
{
    // first non-whitespace character
    std::size_t start = str.find_first_not_of(" \t\n\r");

    // all whitespace
    if (start == std::string::npos)
        return "";

    // last non-whitespace character
    std::size_t end = str.find_last_not_of(" \t\n\r");

    // extract the trimmed substring
    return str.substr(start, end - start + 1);
}

double stringToDouble(const std::string &str) 
{
    if (str.empty())
        throw std::invalid_argument("Empty string when attempting to convert to double");

    // Check the sign
    if (!isdigit(str[0]) && str[0] != '-' && str[0] != '+')
        throw std::invalid_argument("First character of string is not a digit or '-' or '+'");

    std::size_t coma = str.find_first_of(".");
    std::string integerPart;

    if (coma == std::string::npos)
        integerPart = str;
    else if (coma == str.size() - 1)
        throw std::invalid_argument("No digits after decimal point");
    else
        integerPart = str.substr(0, coma) + str.substr(coma + 1);

    integerPart[0] = '1';
    std::size_t tmp = integerPart.find_first_not_of("0123456789");

    if (tmp != std::string::npos)
        throw std::invalid_argument("Found non-numeric characters");

    return std::stod(str);
}

size_t stringToSize_t(const std::string &str) 
{
    std::size_t tmp = str.find_first_not_of("0123456789");
    if (tmp != std::string::npos)
        throw std::invalid_argument("Found non-numeric characters");

    return std::stoul(str);
}

char quotedStringToChar(const std::string &str)
{
    std::string strTrimmed = trim(str);
    if (strTrimmed.size() != 3 || strTrimmed[0] != '"' || strTrimmed[2] != '"')
        throw std::invalid_argument("Invalid argument:std::string doubleToString " + strTrimmed);

    return strTrimmed[1];
}

std::string doubleToString(const double &value) { return std::to_string(value); }

std::string unquoteString(const std::string &str)
{
    std::string strTrimmed = trim(str);
    if (strTrimmed.size() < 2 || strTrimmed[0] != '"' || strTrimmed[strTrimmed.size() - 1] != '"')
        throw std::invalid_argument("Invalid argument: " + strTrimmed);

    return strTrimmed.substr(1, strTrimmed.size() - 2);
}

std::string quoteString(const std::string &str) { return "\"" + str + "\""; }

std::string vectorToString(const std::vector<std::string> &vec, char delimiter)
{
    std::string str;
    for (const std::string &s : vec)
        str += s + delimiter;

    if (!str.empty())
        str.pop_back();
    
    return str;
}

bool isAdditionOverflow(const double &left, const double &right)
{
    double maxDouble = std::numeric_limits<double>::max();
    return (left > 0 && right > maxDouble - left) || (left < 0 && right < -maxDouble - left);
}

bool isSubtractionOverflow(const double &left, const double &right)
{
    double minDouble = std::numeric_limits<double>::lowest();
    return (left > 0 && right < minDouble + left) || (left < 0 && right > -minDouble + left);
}

bool isMultiplicationOverflow(const double &left, const double &right)
{
    double maxDouble = std::numeric_limits<double>::max();
    double minDouble = std::numeric_limits<double>::lowest();

    if (left > 0)
        return right > maxDouble / left || right < minDouble / left;
    if (left < 0)
        return right < maxDouble / left || right > minDouble / left;

    return false;
}

bool isWhiteSpace(char c) { return c == ' ' || c == '\t' || c == '\r' || c == '\n'; }
bool isComa(char c) { return c == ','; }

std::vector<std::string> split(const std::string &infix, bool (*isDelimeter)(char))
{
    std::vector<std::string> tokens;

    std::string token = "";
    bool ignore = false;
    bool quotes = false;

    for (char c : infix)
    {
        if (ignore)
        {
            ignore = false;
            token += c;
            continue;
        }
        if (c == '\\')
        {
            ignore = true;
            continue;
        }
        if (c == '"')
        {
            token += c;
            quotes = !quotes;
            continue;
        }
        
        // default
        if (quotes || !isDelimeter(c))
            token += c;

        // delimiter
        else if (!token.empty() || !isWhiteSpace(c))
        {
            tokens.push_back(token);
            token.clear();
        }
    }

    if (quotes)
        throw std::runtime_error("Quoted string is not closed");

    if (!token.empty() || !isDelimeter(' '))
        tokens.push_back(token);

    return tokens;
}

bool isDoubleOrQuotedString(const std::string &operand)
{
    try { stringToDouble(operand); }
    catch (const std::exception &e1)
    {
        try { unquoteString(operand); }
        catch (const std::exception &e2) { return false; }
    }

    return true;
}

std::vector<std::string> formatTable(const std::vector<std::vector<std::string>> &table, char delimeter, const std::size_t &padding)
{
    std::vector<std::string> formattedTable(table[0].size());

    for (auto columnIt = table.begin(); columnIt != table.end(); ++columnIt)
    {
        size_t max_length = 0;

        for (auto &cell : *columnIt)
            if (cell.length() > max_length)
                max_length = cell.length();
        
        std::string cellClosing = (columnIt == table.end() - 1) ? "" : std::string(padding, ' ') + delimeter + std::string(padding, ' ');

        for (size_t i = 0; i < columnIt->size(); ++i)
            formattedTable[i] += (*columnIt)[i] + std::string(max_length - (*columnIt)[i].length(), ' ') + cellClosing;
    }

    return formattedTable;
}
