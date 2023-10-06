#ifndef __CONSTANTMANAGER_H__
#define __CONSTANTMANAGER_H__

#include <string>
#include <unordered_map>
#include <memory>

class Constant 
{
    std::string m_name;
    std::string m_value;
public:
    Constant(const std::string& name, const std::string& value);
    std::string getName() const;
    std::string getValue() const;
};

class ConstantManager
{
    std::unordered_map<std::string, std::shared_ptr<Constant>> m_constantDictionary;

public:
    ConstantManager();
    ~ConstantManager() = default;

    std::shared_ptr<Constant> determine(const std::string &supposedConstant) const;

    void add(const std::string& name, const std::string& value);
};

#endif // __CONSTANTMANAGER_H__