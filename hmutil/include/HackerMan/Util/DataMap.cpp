#include "DataMap.hpp"
#include <HackerMan/Util/DataFile.hpp>
#include <HackerMan/Util/DataParser.hpp>
#include <iostream>
#include <fstream>

bool HMDataMap::loadFromFile(string fileName)
{
    HMDataFile file;

    if(!file.open(fileName))
    {
        cerr << "HMDataMap: cannot load map from file (00)" << endl;
        return false;
    }

    string str = file.read();
    return loadFromString(str);
}

HMDataMap HMDataMap::getAllByNamespace(string space)
{
    HMDataMap m;
    for(auto it = m_dataMap.begin(); it != m_dataMap.end(); it++)
    {
        if(it->first.first == space)
            m.setKey(it->first.second, it->second, it->first.first);
    }
    return m;
}

HMDataMap& HMDataMap::setNumberKey(string key, double value, string space)
{
    m_dataMap[make_pair(space,key)] = to_string(value);
    return *this;
}

const map<pair<string,string>,string>& HMDataMap::getMap()
{
    return m_dataMap;
}

bool HMDataMap::hasKey(string key, string space)
{
    return m_dataMap.count(make_pair(space,key)) > 0;
}

void HMDataMap::removeKey(string key, string space)
{
    auto it = m_dataMap.find(make_pair(space,key));

    if(it != m_dataMap.end())
    {
        m_dataMap.erase(it);
    }
}

bool HMDataMap::saveToFile(string fname)
{
	std::ofstream file(fname);
	if(file.fail()) return false;
	for(auto it : m_dataMap)
	{
		std::string ns = it.first.first;
		std::string key = it.first.second;
		std::string val = it.second;
		file << ns << ":" << key << "=" << val << std::endl;
	}
	return true;
}

double HMDataMap::getNumberKey(string key, string space, double defaultVal)
{
    auto it = m_dataMap.find(make_pair(space,key));

    if(it != m_dataMap.end())
    {
        try
        {
            return stod(it->second);
        }
        catch(...)
        {
            return defaultVal;
        }
    }
    return defaultVal;
}

bool HMDataMap::loadFromString(string str)
{
    HMDataParser parser;
    if(parser.parse(*this, str))
        return true;
    else
    {
        cerr << "HMDataMap: parser error (01)" << endl;
        return false;
    }
}

string HMDataMap::getKey(string key, string space, string defaultVal)
{
    auto it = m_dataMap.find(make_pair(space,key));

    if(it != m_dataMap.end())
        return it->second;

    return defaultVal;
}

HMDataMap& HMDataMap::setKey(string key, string value, string space)
{
    m_dataMap[make_pair(space,key)] = value;
    return *this;
}
