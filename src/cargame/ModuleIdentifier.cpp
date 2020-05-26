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

std::string ModuleIdentifier::getModule() const
{
    return module;
}

std::string ModuleIdentifier::getObjectId() const
{
    return objectId;
}

std::string ModuleIdentifier::toString() const
{
    return module + "$" + objectId;
}

ModuleIdentifier::operator std::string()
{
    return toString();
}

bool operator==(const ModuleIdentifier mod1, const ModuleIdentifier mod2)
{
    return mod1.getModule() == mod2.getModule() && mod1.getObjectId() == mod2.getObjectId();
}

bool operator!=(const ModuleIdentifier mod1, const ModuleIdentifier mod2)
{
    return !(mod1 == mod2);
}

bool operator<(const ModuleIdentifier mod1, const ModuleIdentifier mod2)
{
    return mod1.getModule() < mod2.getModule() || mod1.getObjectId() < mod2.getObjectId();
}
