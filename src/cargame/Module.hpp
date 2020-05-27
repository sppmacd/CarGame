#ifndef MODULE_HPP
#define MODULE_HPP

#include <string>

class ModuleManager;

class Module
{
public:
    Module(ModuleManager* manager, std::string name, void* system);
    ~Module();

    std::string getName();

    bool call(std::string func);

    bool checkFor(std::string func);

    void* getSystemInfo();
private:

    ModuleManager* modManager;
    std::string modName;
    void* systemInfo;
};

#endif // MODULE_HPP

