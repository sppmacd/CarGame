#include "DebugLogger.hpp"

#include <iostream>
#include "Game.h"
#include <sstream>
#include <iomanip>

std::string time2str()
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
        << " / " << clock();
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
    std::cout << "{ " << timeString << " } [ " << level << " ]  " << logger << ":  " << str << std::endl;
}
void DebugLogger::logDbg(std::string str, std::string logger, std::string level)
{
    if(!Game::instance || Game::instance->debug)
        log(str, "--DEBUG-- " + logger, level);
}


