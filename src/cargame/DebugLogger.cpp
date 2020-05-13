#include "DebugLogger.hpp"

#include <iostream>
#include "Game.h"
#include "GameLoader.hpp"
#include <sstream>
#include <iomanip>

#ifdef SFML_SYSTEM_WINDOWS
#include <windows.h>
#endif // SFML_SYSTEM_WINDOWS

static std::string time2str()
{
    time_t _t = time(NULL);
    tm* _tstr = localtime(&_t);
    std::ostringstream _str;
    _str << std::setfill('0')
               << std::setw(4) << (_tstr->tm_year + 1900)
        << "-" << std::setw(2) << _tstr->tm_mon
        << "-" << std::setw(2) << _tstr->tm_mday
        << " " << std::setw(2) << _tstr->tm_hour
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

    // Display string on standard output.
    std::left(std::cout);
    GameLoader::instance->consoleColor(level);
    std::cout << std::setfill(' ') << "{ " << timeString << " } [ " << std::setw(5) << level << " ]\t" << std::setw(25) << logger << "" << str << std::endl;
}
void DebugLogger::logDbg(std::string str, std::string logger, std::string level)
{
    if(!Game::instance || Game::instance->debug)
        log(str, "- " + logger, level);
}


