#pragma once

#include <string>

// A structure to store command-line arguments.
struct ArgMap
{
    bool a_debug;
    bool a_help;
    bool a_info;
    std::string a_message;
    bool a_no_resources;
};
