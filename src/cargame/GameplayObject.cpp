#include "GameplayObject.hpp"

GameplayObject::GameplayObject(std::string modName)
    : moduleName(modName)
{
}

std::string GameplayObject::getModuleName()
{
    return moduleName;
}

std::string GameplayObject::getNamespacedId(std::string name)
{
    return getModuleName() + "$" + name;
}
