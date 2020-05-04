#include "DebugLogger.hpp"

#include <iostream>
#include "Game.h"
#include <sstream>
#include <iomanip>

#ifdef SFML_SYSTEM_WINDOWS
#include <windows.h>
#endif // SFML_SYSTEM_WINDOWS

int level2color(std::string lvl)
{
    if(lvl == "INFO")
        return 0x0F;
    else if(lvl == "WARN")
        return 0x06;
    else if(lvl == "ERROR")
        return 0x0C;
    else if(lvl == "FATAL")
        return 0xCF;
    else if(lvl == "LAG")
        return 0x0B;
    else if(lvl == "DEBUG")
        return 0x08;
    else if(lvl == "EVENT")
        return 0x0D;
    else
        return 0x07;
}

static void consoleColor(std::string level)
{
    #ifdef SFML_SYSTEM_WINDOWS
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, level2color(level));
    #else
        static bool warned = false;
        if(!warned)
        {
            warned = true;
            std::cout << "DebugLogger: Console coloring not supported" << std::endl;
        }
        std::hex(std::cout);
        std::cout << "&" << level2color();
        std::dec(std::cout);
    #endif // SFML_SYSTEM_WINDOWS
}

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
        << " / " << std::setw(8) << clock();
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
    consoleColor(level);
    std::cout << std::setfill(' ') << "{ " << timeString << " } [ " << std::setw(5) << level << " ]\t" << std::setw(25) << logger << "" << str << std::endl;
}
void DebugLogger::logDbg(std::string str, std::string logger, std::string level)
{
    if(!Game::instance || Game::instance->debug)
        log(str, "- " + logger, level);
}


