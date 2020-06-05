#include "DebugLogger.hpp"

#include "Game.h"
#include "GameLoader.hpp"
#include "ModuleManager.hpp"

#include <iomanip>
#include <iostream>
#include <sstream>

static std::string time2str()
{
    time_t _t = time(NULL);
    tm* _tstr = localtime(&_t);
    std::ostringstream _str;
    _str << std::setfill('0')
        << ""  << std::setw(2) << _tstr->tm_hour
        << ":" << std::setw(2) << _tstr->tm_min
        << ":" << std::setw(2) << _tstr->tm_sec
        << " / " << std::setw(8) << (clock() % 100000000);
    return _str.str();
}

// TODO: change format
void DebugLogger::log(std::string str, std::string logger, std::string level)
{
    // Get the current time string.
    time_t _t;
    time(&_t);
    std::string timeString = time2str();

    // Append current module name if possible
    std::string currentMod = (ModuleManager::instance ? ModuleManager::instance->current() : "init");
    // Display string on standard output.
    std::left(std::cout);

    std::ostringstream oss;
    oss << std::setfill(' ') << "{ " << timeString << " } "
        << currentMod << "/"
        << logger;

    // Append log
    if(GameLoader::instance)
        GameLoader::instance->consoleColor(level);
    std::cout << std::setw(64) << oss.str() << level << ": ";

    if(GameLoader::instance)
        GameLoader::instance->consoleColor("Color");
    std::cout << str << std::endl;
}
void DebugLogger::logDbg(std::string str, std::string logger, std::string level)
{
    if(!Game::instance || Game::instance->debug)
        log(str, logger + "#", level);
}


