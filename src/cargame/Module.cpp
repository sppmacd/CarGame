#include "Module.hpp"
#include "ModuleManager.hpp"

Module::Module(ModuleManager* manager, std::string name, void* system)
    : modManager(manager)
    , modName(name)
    , systemInfo(system)
{
    modName = name;
    systemInfo = system;
}
Module::~Module()
{

}

std::string Module::getName()
{
    return modName;
}

bool Module::call(std::string func)
{
    return modManager->call(this, func);
}

bool Module::checkFor(std::string func)
{
    return modManager->checkForFunc(this, func);
}

void* Module::getSystemInfo()
{
    return systemInfo;
}
