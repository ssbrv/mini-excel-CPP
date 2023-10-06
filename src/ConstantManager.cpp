#include "include/Const.h"
#include "include/ConstantManager.h"

#include <memory>
#include <unordered_map>
#include <string>

Constant::Constant(const std::string &name, const std::string &value) : m_name(name), m_value(value) {}
std::string Constant::getName() const { return m_name; }
std::string Constant::getValue() const { return m_value; }

ConstantManager::ConstantManager()
{
    add(PI_CONSTANT_NAME, PI_CONSTANT_VALUE);
    add(E_CONSTANT_NAME, E_CONSTANT_VALUE);
    add(USERNAME_CONSTANT_NAME, USERNAME_CONSTANT_VALUE);
}

std::shared_ptr<Constant> ConstantManager::determine(const std::string &supposedConstant) const
{
    auto iter = m_constantDictionary.find(supposedConstant);
    return (iter == m_constantDictionary.end()) ? nullptr : iter->second;
}

void ConstantManager::add(const std::string &name, const std::string &value) 
{
    m_constantDictionary[name] = std::make_shared<Constant>(name, value);
}