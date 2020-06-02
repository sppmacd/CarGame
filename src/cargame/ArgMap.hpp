#pragma once

#include <string>

// A structure to store command-line arguments.
struct ArgMap
{
    std::string a_debug_options;
    bool a_debug;
    bool a_help;
    bool a_info;
    std::string a_message;
    bool a_no_resources;
    bool a_tmp;
};
