#include "DebugLogger.hpp"

#include <iostream>
#include "Game.h"

void DebugLogger::log(std::string str, std::string logger)
{
    std::cout << logger << ": " << str << std::endl;
}
void DebugLogger::logDbg(std::string str, std::string logger)
{
    if(!Game::instance || Game::instance->debug)
        std::cout << "[DEBUG] " << logger << ": " << str << std::endl;
}


