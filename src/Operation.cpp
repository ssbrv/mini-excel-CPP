#include "include/Const.h"
#include "include/Operation.h"
#include <stdexcept>
#include <string>
#include <vector>

Operation::Operation(const std::string &name, unsigned short arity)
    : m_name(name), m_arity(arity) {}

Operation::~Operation() {}

std::string Operation::getName() const { return m_name; }

unsigned short Operation::getArity() const { return m_arity; }

void Operation::validateArity(const std::vector<std::string> &operand) const
{
    if (operand.size() != m_arity)
        throw std::runtime_error(INVALID_ARITY_ERR);
}