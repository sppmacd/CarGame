#pragma once

#include <string>

namespace DebugLogger
{
    void log(std::string str, std::string logger = "main");
    void logDbg(std::string str, std::string logger = "main");
};
