#ifndef __OPERATION_H__
#define __OPERATION_H__

#include <string>
#include <vector>

class Operation
{
protected:
    std::string m_name;
    unsigned short m_arity;

public:
    Operation(const std::string &name, unsigned short arity);
    virtual ~Operation() = 0;

    std::string getName() const;
    unsigned short getArity() const;

    void validateArity(const std::vector<std::string> &operand) const;
};

#endif // __OPERATION_H__