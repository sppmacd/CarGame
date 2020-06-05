#include "ModuleManager.hpp"
#include "DebugLogger.hpp"
#include "GameLoader.hpp"
#include "FileUtil.hpp"

CGAPI ModuleManager* ModuleManager::instance;

ModuleManager::ModuleManager()
{
    instance = this;
}

ModuleManager::~ModuleManager()
{
    DebugLogger::log("Module cleanup started", "ModuleManager");
    for(Module* module: modules)
    {
        DebugLogger::logDbg("Doing cleanup of module: " + module->getName(), "ModuleManager");
        cleanup(module);
        delete module;
    }
}

Module* ModuleManager::loadModule(std::string mod)
{
    DebugLogger::log("Couldn't load module '" + mod + "' - OS not supported.", "ModuleManager", "ERROR");
    return NULL;
}

void ModuleManager::cleanup(Module* mod)
{
    DebugLogger::log("Couldn't do cleanup '" + mod->getName() + "' - OS not supported.", "ModuleManager", "ERROR");
}

bool ModuleManager::call(Module* mod, std::string name)
{
    DebugLogger::log("Couldn't do call " + mod->getName() + "!" + name + "() - OS not supported.", "ModuleManager", "ERROR");
    return false;
}

// Checks if %funcName exists.
bool ModuleManager::checkForFunc(Module*, std::string)
{
    DebugLogger::log("Couldn't do check - OS not supported.", "ModuleManager", "ERROR");
    return false;
}

bool ModuleManager::loadAllModules()
{
    std::string modDir = CG_ARCH_PREFIX + std::string("/mods");
    std::vector<std::string> mods = FileUtil::listFiles(modDir);
    if(mods.empty())
    {
        DebugLogger::log("No mod loaded or OS is not supported. The game won't have any content :(", "ModuleManager", "ERROR");
        return true;
    }
    for(std::string& modFile: mods)
    {
        // Allocate module and all system-specific resources.
        Module* module = loadModule(modFile);
        if(!module)
        {
            DebugLogger::log("Couldn't load file '" + modFile + "' as library. Probably the format is not supported.", "ModuleManager", "ERROR");
            continue;
        }
        if(!module->checkFor("cgGameInit"))
        {
            DebugLogger::log("Couldn't load file '" + modFile + "' as CG module. Try adding cgGameInit function.", "ModuleManager", "ERROR");
            delete module;
            continue;
        }
        currentModule = module->getName();
        bool b1 = module->call("cgLoad");
        if(!b1) {} //it's optional so don't care
        modules.push_back(module);
    }
    currentModule = "root";
    return true;
}

bool ModuleManager::gameInitAllModules()
{
    DebugLogger::log("Initializing game for modules", "ModuleManager");
    for(Module* module: modules)
    {
        currentModule = module->getName();
        DebugLogger::logDbg("Doing gameInit of module: " + module->getName(), "ModuleManager");
        if(!module->call("cgGameInit"))
        {
            DebugLogger::log("Couldn't call gameInit for " + currentModule + +"!", "ModuleManager", "ERROR");
            break;
        }
    }
    currentModule = "api";
    return true;
}

std::string ModuleManager::current()
{
    return currentModule;
}

size_t ModuleManager::getModuleCount()
{
    return modules.size();
}

void ModuleManager::setCurrent(std::string mod)
{
    currentModule = mod;
}

void ModuleManager::getModuleNames(std::vector<std::string>& modArr)
{
    for(size_t s = 0; s < modules.size(); s++)
    {
        modArr.push_back(modules[s]->getName());
    }
}
