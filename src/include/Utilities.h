#ifndef __UTILITIES_H__
#define __UTILITIES_H__

#include <string>
#include <vector>

/* string manipulation */

std::string trim(const std::string &str);
double stringToDouble(const std::string &str);
size_t stringToSize_t(const std::string &str);
char quotedStringToChar(const std::string &str);
std::string doubleToString(const double &value);
std::string unquoteString(const std::string &str);
std::string quoteString(const std::string &str);
std::string vectorToString(const std::vector<std::string> &vec, char delimiter);

/* overflow control */

bool isAdditionOverflow(const double &left, const double &right);
bool isSubtractionOverflow(const double &left, const double &right);
bool isMultiplicationOverflow(const double &left, const double &right);

/* char manipulation */

bool isWhiteSpace(char c);
bool isComa(char c);

/* parsing */

std::vector<std::string> split(const std::string &infix, bool (*isDelimeter)(char));
bool isDoubleOrQuotedString(const std::string &operand);

std::vector<std::string> formatTable(const std::vector<std::vector<std::string>> &table, char delimeter, const std::size_t &padding);

#endif // __UTILITIES_H__