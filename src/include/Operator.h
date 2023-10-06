#ifndef __OPERATOR_H__
#define __OPERATOR_H__

#include "Operation.h"

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

class Operator : public Operation
{
protected:
    unsigned short m_precedence;

public:
    Operator(const std::string &name, unsigned short arity, unsigned short precedence);
    virtual ~Operator() = 0;

    virtual std::string operator()(const std::vector<std::string> &opearand) const = 0;

    virtual Operator *clone() const = 0;

    unsigned short getPrecedence() const;
};

class Addition : public Operator
{
public:
    Addition();
    ~Addition() override;
    std::string operator()(const std::vector<std::string> &opearand) const override;
    Operator *clone() const override;
};

class Subtraction : public Operator
{
public:
    Subtraction();
    ~Subtraction() override;
    std::string operator()(const std::vector<std::string> &opearand) const override;
    Operator *clone() const override;
};

class Multiplication : public Operator
{
public:
    Multiplication();
    ~Multiplication() override;
    std::string operator()(const std::vector<std::string> &opearand) const override;
    Operator *clone() const override;
};

class Division : public Operator
{
public:
    Division();
    ~Division() override;
    std::string operator()(const std::vector<std::string> &opearand) const override;
    Operator *clone() const override;
};

class Sin : public Operator
{
public:
    Sin();
    ~Sin() override;
    std::string operator()(const std::vector<std::string> &opearand) const override;
    Operator *clone() const override;
};

class Cos : public Operator
{
public:
    Cos();
    ~Cos() override;
    std::string operator()(const std::vector<std::string> &opearand) const override;
    Operator *clone() const override;
};

class Log10 : public Operator
{
public:
    Log10();
    ~Log10() override;
    std::string operator()(const std::vector<std::string> &opearand) const override;
    Operator *clone() const override;
};

class Ln : public Operator
{
public:
    Ln();
    ~Ln() override;
    std::string operator()(const std::vector<std::string> &opearand) const override;
    Operator *clone() const override;
};

class Log2 : public Operator
{
public:
    Log2();
    ~Log2() override;
    std::string operator()(const std::vector<std::string> &opearand) const override;
    Operator *clone() const override;
};

class Avg : public Operator
{
public:
    Avg();
    ~Avg() override;
    std::string operator()(const std::vector<std::string> &opearand) const override;
    Operator *clone() const override;
};

class Concatenation : public Operator
{
public:
    Concatenation();
    ~Concatenation() override;
    std::string operator()(const std::vector<std::string> &opearand) const override;
    Operator *clone() const override;
};

class Repetition : public Operator
{
public:
    Repetition();
    ~Repetition() override;
    std::string operator()(const std::vector<std::string> &opearand) const override;
    Operator *clone() const override;
};

class Pad : public Operator 
{
protected:
    std::string pad(const std::vector<std::string> &opearand, bool right) const;

public:
    Pad(const std::string &name, unsigned short arity, unsigned short precedence);
    ~Pad() override;
};

class Padl : public Pad
{
public:
    Padl();
    ~Padl() override;
    std::string operator()(const std::vector<std::string> &opearand) const override;
    Operator *clone() const override;
};

class Padr : public Pad
{
public:
    Padr();
    ~Padr() override;
    std::string operator()(const std::vector<std::string> &opearand) const override;
    Operator *clone() const override;
};

class AdditionConcatenation : public Operator
{
public:
    AdditionConcatenation();
    ~AdditionConcatenation();
    std::string operator()(const std::vector<std::string> &opearand) const override;
    Operator *clone() const override;
};

class LeftBracket : public Operator
{
public:
    LeftBracket();
    ~LeftBracket() override;
    std::string operator()(const std::vector<std::string> &opearand) const override;
    Operator *clone() const override;
};

class OperatorManager
{
    std::unordered_map<std::string, std::shared_ptr<Operator>> m_operatorDictionary;
public:
    OperatorManager();
    ~OperatorManager() = default;

    std::shared_ptr<Operator> determine(const std::string &supposedOperator) const;
};

#endif // __OPERATOR_H__