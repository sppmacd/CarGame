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
    bool loadFromFile(string fileName);

    bool loadFromString(string str);
	
	bool saveToFile(string fname);

    string getKey(string key, string space = "", string defaultVal = "");

    double getNumberKey(string key, string space = "", double defaultVal = 0.f);

    HMDataMap getAllByNamespace(string space);

    HMDataMap& setKey(string key, string value, string space = "");

    HMDataMap& setNumberKey(string key, double value, string space = "");

    map<pair<string,string>,string> getMap();

    bool hasKey(string key, string space = "");

    void removeKey(string key, string space = "");
};
