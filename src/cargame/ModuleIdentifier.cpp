#include "ModuleIdentifier.hpp"

ModuleIdentifier::ModuleIdentifier(std::string mod, std::string id)
    : module(mod)
    , objectId(id)
{
}

ModuleIdentifier::ModuleIdentifier(std::string fullId)
{
    size_t s = fullId.find('$');
    if(s == std::string::npos)
    {
        module = "cgcore";
        objectId = fullId;
    }
    else
    {
        module = fullId.substr(0, s);
        objectId = fullId.substr(s + 1);
    }
}

std::string ModuleIdentifier::getModule()
{
    return module;
}

std::string ModuleIdentifier::getObjectId()
{
    return objectId;
}

std::string ModuleIdentifier::toString()
{
    return module + "$" + objectId;
}
