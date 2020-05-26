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

    ModuleIdentifier() : ModuleIdentifier("$") {}

    // ...
    std::string getModule() const;

    // ...
    std::string getObjectId() const;

    // Converts identifier to <mod>$<id> format.
    std::string toString() const;

    operator std::string();
private:
    std::string module;
    std::string objectId;
};

bool operator==(const ModuleIdentifier mod1, const ModuleIdentifier mod2);
bool operator!=(const ModuleIdentifier mod1, const ModuleIdentifier mod2);
bool operator<(const ModuleIdentifier mod1, const ModuleIdentifier mod2);

#endif // MODULEIDENTIFIER_HPP
