#ifndef MODULEMANAGER_HPP
#define MODULEMANAGER_HPP

#include <string>
#include <vector>

#include "Module.hpp"
#include "Export.hpp"

class GameLoader;

class ModuleManager
{
public:
    ModuleManager();

    // Destructor. Calls cgCleanup for all modules and removes handles
    // to them.
    virtual ~ModuleManager();

    //   Implemented by OS-specific ROOTS

    // System-specific module loader.
    virtual Module* loadModule(std::string mod);

    // Does cleanup of specific module.
    virtual void cleanup(Module* mod);

    // Calls a specific function from Module.
    virtual bool call(Module* module, std::string funcName);

    // Checks if %funcName exists.
    virtual bool checkForFunc(Module* module, std::string funcName);

    // ^^ Implemented by OS-specific ROOTS ^^

    // Loads all modules from /mods folder and calls cgLoad() function.
    bool loadAllModules();

    // Calls cgGameInit() for all modules.
    bool gameInitAllModules();

    // Gets currently loaded/handled module. Used by DebugLogger
    // and GameplayObjects to determine registrant module.
    std::string current();

    void setCurrent(std::string mod);

    CGAPI static ModuleManager* instance;

private:
    std::vector<Module*> modules;
    std::string currentModule;
};

#endif // MODULEMANAGER_HPP

