#include "UnixModuleManager.hpp"

#include <version.hpp>
#include <dlfcn.h>
#include <cargame/DebugLogger.hpp>

// System-specific module loader.
Module* UnixModuleManager::loadModule(std::string mod)
{
    // omit ".so" part of name
    size_t i = mod.find_last_of("/");
    std::string modName = mod.substr(i + 1, mod.find_last_of(".") - i - 1);
    DebugLogger::logDbg("Trying to load module '" + modName + "'", "UnixModuleManager");
    void* systemHandle = dlopen(mod.c_str(), RTLD_LAZY);
    Module* module = new Module(this, modName, systemHandle);
    return module;
}

// Does cleanup of specific module.
void UnixModuleManager::cleanup(Module* mod)
{
    dlclose(mod->getSystemInfo());
}

// Calls a specific function from Module.
bool UnixModuleManager::call(Module* module, std::string funcName)
{
    typedef void(*ProcType_cg)();
    DebugLogger::logDbg("Trying to retrieve address of " + funcName + "() from " + module->getName(), "UnixModuleManager");
    ProcType_cg func = (ProcType_cg)dlsym(module->getSystemInfo(), funcName.c_str()); // todo: loading modules !

    if(!func)
        return false;
    func();
    return true;
}

// Checks if %funcName exists.
bool UnixModuleManager::checkForFunc(Module* module, std::string funcName)
{
    typedef void(*ProcType_cg)();
    DebugLogger::logDbg("Trying to check for " + funcName + "() from " + module->getName(), "UnixModuleManager");
    ProcType_cg func = (ProcType_cg)dlsym(module->getSystemInfo(), funcName.c_str());

    if(!func)
        return false;
    else
        return true;
}
