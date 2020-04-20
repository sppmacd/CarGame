////////////////////////////////
// Hacker Man Utility Library //
//     SPPMACD (c) 2019       //
////////////////////////////////

#pragma once

#include <HackerMan/Util/DataMap.hpp>

class HMDataParser
{
public:
    // Populates %map by entries, basing on %data.
    // Returns true if parse was successful, and false otherwise.
    bool parse(HMDataMap& map, string& data);
};
