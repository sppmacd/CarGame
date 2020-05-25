#ifndef MODULEIDENTIFIER_HPP
#define MODULEIDENTIFIER_HPP

#include <string>

class ModuleIdentifier
{
public:
    // Creates identifier from <mod> and <id>.
    ModuleIdentifier(std::string mod, std::string id);

    // Creates identifier from <mod>$<id> format. If <mod> is not
    // specified, defaults to 'cgcore'.
    ModuleIdentifier(std::string fullId);

    ModuleIdentifier(const char* fullId) : ModuleIdentifier(std::string(fullId)) {}

    // ...
    std::string getModule();

    // ...
    std::string getObjectId();

    // Converts identifier to <mod>$<id> format.
    std::string toString();
private:
    std::string module;
    std::string objectId;
};

#endif // MODULEIDENTIFIER_HPP
