#include "WindowsModuleManager.hpp"

#include <windows.h>
#include <version.hpp>
#include <cargame/DebugLogger.hpp>

// System-specific module loader.
Module* WindowsModuleManager::loadModule(std::string mod)
{
    //std::string modDir = CG_ARCH_PREFIX + std::string("/mods");

    // omit ".dll" part of name
    size_t i = mod.find_last_of("/");
    std::string modName = mod.substr(i + 1, mod.find_last_of(".") - i - 1);
    DebugLogger::logDbg("Trying to load module '" + modName + "'", "WindowsModuleManager");
    HMODULE hMod = LoadLibraryA(mod.c_str());

    if(!hMod)
        return NULL;
    else
        return new Module(this, modName, (void*)hMod);
}

// Does cleanup of specific module.
void WindowsModuleManager::cleanup(Module* mod)
{
    FreeLibrary((HMODULE)mod->getSystemInfo());
}

// Calls a specific function from Module.
bool WindowsModuleManager::call(Module* module, std::string funcName)
{
    typedef void(*ProcType_cg)();
    DebugLogger::logDbg("Trying to load get address of " + funcName + "() from " + module->getName(), "WindowsModuleManager");
    ProcType_cg func = (ProcType_cg)GetProcAddress((HMODULE)module->getSystemInfo(), funcName.c_str());

    if(!func)
        return false;
    func();
    return true;
}

// Checks if %funcName exists.
bool WindowsModuleManager::checkForFunc(Module* module, std::string funcName)
{
    typedef void(*ProcType_cg)();
    DebugLogger::logDbg("Trying to check for " + funcName + "() from " + module->getName(), "WindowsModuleManager");
    ProcType_cg func = (ProcType_cg)GetProcAddress((HMODULE)module->getSystemInfo(), funcName.c_str());

    if(!func)
        return false;
    else
        return true;
}
