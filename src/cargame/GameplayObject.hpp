#ifndef GAMEPLAYOBJECT_HPP
#define GAMEPLAYOBJECT_HPP

#include <string>

class GameplayObject
{
    std::string moduleName;
public:
    GameplayObject(std::string modName);

    std::string getModuleName();

    std::string getNamespacedId(std::string name);
};

#endif // GAMEPLAYOBJECT_HPP

