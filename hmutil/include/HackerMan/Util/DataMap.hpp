////////////////////////////////
// Hacker Man Utility Library //
//     SPPMACD (c) 2019       //
////////////////////////////////

#pragma once

#include <map>
#include <string>
#include <vector>

using namespace std;

class HMDataMap
{
    map<pair<string,string>,string> m_dataMap;
public:
    // Loads map from specified file.
    bool loadFromFile(string fileName);

    // Loads map from string.
    bool loadFromString(string str);

	// Saves map to file with specified %fname.
	bool saveToFile(string fname);

	// Returns key by %key name, name%space. If key doesn't exist, returns %defaultVal.
    string getKey(string key, string space = "", string defaultVal = "");

    // Returns key converted to number. If key doesn't exist, returns %defaultVal
    // If key is not convertable to number, also returns %defaultVal.
    double getNumberKey(string key, string space = "", double defaultVal = 0.f);

    // Returns all keys with specified name%space.
    HMDataMap getAllByNamespace(string space);

    // Sets value of specified %key in name%space to %value.
    // Returns *this.
    HMDataMap& setKey(string key, string value, string space = "");

    // Sets value of %key in name%space, previously converting it to string.
    // Returns *this.
    HMDataMap& setNumberKey(string key, double value, string space = "");

    // Returns read-only reference to map, which you can use to iterate on map.
    const map<pair<string,string>,string>& getMap();

    // Checks if map has specified %key in name%space. Returns true if has and false if not.
    bool hasKey(string key, string space = "");

    // Removes specified %key in name%space from map.
    void removeKey(string key, string space = "");
};
