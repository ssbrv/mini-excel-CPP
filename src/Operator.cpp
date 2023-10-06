#include "include/Operator.h"
#include "include/Utilities.h"
#include "include/Const.h"
#include "include/CellIndex.h" 
#include <cmath>

Operator::Operator(const std::string &name, unsigned short arity, unsigned short precedence)
    : Operation{name, arity}, m_precedence{precedence} {}
Operator::~Operator() {}
unsigned short Operator::getPrecedence() const { return m_precedence; }

Addition::Addition() : Operator{ADDITION_CONCATENATION_OPERATOR_NAME, 2, 3} {}
Addition::~Addition() {}
std::string Addition::operator()(const std::vector<std::string> &operand) const
{
    validateArity(operand);

    double left = stringToDouble(operand[0]);
    double right = stringToDouble(operand[1]);

    if (isAdditionOverflow(left, right)) 
        throw std::overflow_error(OVERFLOW_ERR);

    return doubleToString(left + right);
}
Operator *Addition::clone() const { return new Addition(*this); }

Subtraction::Subtraction() : Operator{SUBTRACTION_OPERATOR_NAME, 2, 3} {}
Subtraction::~Subtraction() {}
std::string Subtraction::operator()(const std::vector<std::string> &operand) const
{
    validateArity(operand);

    double left = stringToDouble(operand[0]);
    double right = stringToDouble(operand[1]);

    if (isSubtractionOverflow(left, right))
        throw std::overflow_error(OVERFLOW_ERR);

    return doubleToString(left - right);
}
Operator *Subtraction::clone() const { return new Subtraction(*this); }

Multiplication::Multiplication() : Operator{MULTIPLICATION_OPERATOR_NAME, 2, 2} {}
Multiplication::~Multiplication() {}
std::string Multiplication::operator()(const std::vector<std::string> &operand) const
{
    validateArity(operand);

    double left = stringToDouble(operand[0]);
    double right = stringToDouble(operand[1]);

    if (isMultiplicationOverflow(left, right))
        throw std::overflow_error(OVERFLOW_ERR);

    return doubleToString(left * right);
}
Operator *Multiplication::clone() const { return new Multiplication(*this); }

Division::Division() : Operator{DIVISION_OPERATOR_NAME, 2, 2} {}
Division::~Division() {}
std::string Division::operator()(const std::vector<std::string> &operand) const
{
    validateArity(operand);

    double left = stringToDouble(operand[0]);
    double right = stringToDouble(operand[1]);

    if (right == 0)
        throw std::invalid_argument(ZERO_ARGUMENT_ERR);

    return doubleToString(left / right);
}
Operator *Division::clone() const { return new Division(*this); }

Sin::Sin() : Operator{SIN_OPERATOR_NAME, 1, 1} {}
Sin::~Sin() {}
std::string Sin::operator()(const std::vector<std::string> &operand) const
{
    validateArity(operand);

    double argument = stringToDouble(operand[0]);

    return doubleToString(std::sin(argument));
}
Operator *Sin::clone() const { return new Sin(*this); }

Cos::Cos() : Operator{COS_OPERATOR_NAME, 1, 1} {}
Cos::~Cos() {}
std::string Cos::operator()(const std::vector<std::string> &operand) const
{
    validateArity(operand);

    double argument = stringToDouble(operand[0]);

    return doubleToString(std::cos(argument));
}
Operator *Cos::clone() const { return new Cos(*this); }

Log10::Log10() : Operator{LOG10_OPERATOR_NAME, 1, 1} {}
Log10::~Log10() {}
std::string Log10::operator()(const std::vector<std::string> &operand) const
{
    validateArity(operand);

    double argument = stringToDouble(operand[0]);

    if (argument < 0)
        throw std::invalid_argument(NEGATIVE_ARGUMENT_ERR);

    return doubleToString(std::log10(argument));
}
Operator *Log10::clone() const { return new Log10(*this); }

Ln::Ln() : Operator{LN_OPERATOR_NAME, 1, 1} {}
Ln::~Ln() {}
std::string Ln::operator()(const std::vector<std::string> &operand) const
{
    validateArity(operand);

    double argument = stringToDouble(operand[0]);

    if (argument < 0)
        throw std::invalid_argument(NEGATIVE_ARGUMENT_ERR);

    return doubleToString(std::log(argument));
}
Operator *Ln::clone() const { return new Ln(*this); }

Log2::Log2() : Operator{LOG2_OPERATOR_NAME, 1, 1} {}
Log2::~Log2() {}
std::string Log2::operator()(const std::vector<std::string> &operand) const
{
    validateArity(operand);

    double argument = stringToDouble(operand[0]);

    if (argument < 0)
        throw std::invalid_argument(NEGATIVE_ARGUMENT_ERR);

    return doubleToString(std::log2(argument));
}
Operator *Log2::clone() const { return new Log2(*this); }

Avg::Avg() : Operator{AVG_OPERATOR_NAME, 0, 1} {}
Avg::~Avg() {}
std::string Avg::operator()(const std::vector<std::string> &operand) const
{
    double avg = 0;
    for (size_t i = 0; i < operand.size(); i++)
        avg += (stringToDouble(operand[i]) - avg) / (i + 1);

    return doubleToString(avg);
}
Operator *Avg::clone() const { return new Avg(*this); }

Concatenation::Concatenation() : Operator{ADDITION_CONCATENATION_OPERATOR_NAME, 2, 3} {}
Concatenation::~Concatenation() {}
std::string Concatenation::operator()(const std::vector<std::string> &operand) const
{
    validateArity(operand);

    std::string left = unquoteString(operand[0]);
    std::string right = unquoteString(operand[1]);

    return std::string{quoteString(left + right)};
}
Operator *Concatenation::clone() const { return new Concatenation(*this); }

Repetition::Repetition() : Operator{REPETITION_OPERATOR_NAME, 2, 2} {}
Repetition::~Repetition() {}
std::string Repetition::operator()(const std::vector<std::string> &operand) const
{
    validateArity(operand);

    long timesToRepeat = stringToDouble(operand[1]);

    if (timesToRepeat < 0)
        throw std::invalid_argument(NEGATIVE_ARGUMENT_ERR);

    std::string stringToRepeat = unquoteString(operand[0]);
    std::string result = "";

    for (int i = 0; i < timesToRepeat; i++)
        result += stringToRepeat;

    return quoteString(result);
}
Operator *Repetition::clone() const { return new Repetition(*this); }

Pad::Pad(const std::string &name, unsigned short arity, unsigned short precedence) : Operator{name, arity, precedence} {}
Pad::~Pad() {}
std::string Pad::pad(const std::vector<std::string> &operand, bool right) const
{
    validateArity(operand);

    std::string stringToPad = unquoteString(operand[0]);
    size_t lenResult = stringToDouble(operand[1]);
    size_t lenStringToPad = stringToPad.length();

    if (lenStringToPad >= lenResult)
        return stringToPad;

    char charPadding = quotedStringToChar(operand[2]);

    size_t lenPadding = lenResult - lenStringToPad;
    std::string stringPadding(lenPadding, charPadding);
    
    return quoteString(((right) ? stringToPad + stringPadding : stringPadding + stringToPad));
}

Padl::Padl() : Pad{PADL_OPERATOR_NAME, 3, 1} {}
Padl::~Padl() {}
std::string Padl::operator()(const std::vector<std::string> &operand) const { return pad(operand, 0); }
Operator *Padl::clone() const { return new Padl(*this); }

Padr::Padr() : Pad{PADR_OPERATOR_NAME, 3, 1} {}
Padr::~Padr() {}
std::string Padr::operator()(const std::vector<std::string> &operand) const { return pad(operand, 1); }
Operator *Padr::clone() const { return new Padr(*this); }

AdditionConcatenation::AdditionConcatenation() : Operator{ADDITION_CONCATENATION_OPERATOR_NAME, 2, 3} {}
AdditionConcatenation::~AdditionConcatenation() {}
std::string AdditionConcatenation::operator()(const std::vector<std::string> &operand) const
{
    try
    {
        Addition add;
        return add(operand);
    }
    catch (const std::exception &e)
    {
        Concatenation cat;
        return cat(operand);
    }
}
Operator *AdditionConcatenation::clone() const { return new AdditionConcatenation(*this); }

LeftBracket::LeftBracket() : Operator{"(", 0, 0} {}
LeftBracket::~LeftBracket() {}
std::string LeftBracket::operator()(const std::vector<std::string> &operand) const
{
    throw std::runtime_error("Cannot execute left bracket operator");
    return "";
}
Operator *LeftBracket::clone() const { return new LeftBracket(*this); }

OperatorManager::OperatorManager()
{
    // to-do: fetch in constsants

    m_operatorDictionary["+"] = std::make_shared<AdditionConcatenation>();
    m_operatorDictionary["-"] = std::make_shared<Subtraction>();
    m_operatorDictionary["*"] = std::make_shared<Multiplication>();
    m_operatorDictionary["/"] = std::make_shared<Division>();
    m_operatorDictionary["sin"] = std::make_shared<Sin>();
    m_operatorDictionary["cos"] = std::make_shared<Cos>();
    m_operatorDictionary["log10"] = std::make_shared<Log10>();
    m_operatorDictionary["ln"] = std::make_shared<Ln>();
    m_operatorDictionary["log2"] = std::make_shared<Log2>();
    m_operatorDictionary["avg"] = std::make_shared<Avg>();
    m_operatorDictionary["."] = std::make_shared<Repetition>();
    m_operatorDictionary["padl"] = std::make_shared<Padl>();
    m_operatorDictionary["padr"] = std::make_shared<Padr>();
    m_operatorDictionary["("] = std::make_shared<LeftBracket>();
}

std::shared_ptr<Operator> OperatorManager::determine(const std::string &supposedOperator) const
{
    auto iter = m_operatorDictionary.find(supposedOperator);
    return (iter == m_operatorDictionary.end()) ? nullptr : iter->second;
}
