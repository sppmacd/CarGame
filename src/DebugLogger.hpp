#pragma once

#include <string>

namespace DebugLogger
{
    // Displays log entry on stdout.
    void log(std::string str, std::string logger = "main", std::string level = "INFO");

    // Displays log entry on stdout, if it's enabled by Debug Mode
    // or this option is unknown (e.g. because of Game not created)
    void logDbg(std::string str, std::string logger = "main", std::string level = "DEBUG");
};
