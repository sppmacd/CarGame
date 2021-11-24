#ifndef WINDOWSMODULEMANAGER_HPP
#define WINDOWSMODULEMANAGER_HPP

#include <cargame/ModuleManager.hpp>

class UnixModuleManager : public ModuleManager
{
public:
    // System-specific module loader.
    virtual Module* loadModule(std::string mod);

    // Does cleanup of specific module.
    virtual void cleanup(Module* mod);

    // Calls a specific function from Module.
    virtual bool call(Module* module, std::string funcName);

    // Checks if %funcName exists.
    virtual bool checkForFunc(Module* module, std::string funcName);
};

#endif // WINDOWSMODULEMANAGER_HPP

