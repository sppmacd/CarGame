////////////////////////////////
// Hacker Man Utility Library //
//     SPPMACD (c) 2019       //
////////////////////////////////

#pragma once

#include <string>
#include <fstream>

using namespace std;

class HMDataFile
{
    string m_fileName;
    ifstream m_fileStream;
public:
    HMDataFile(string name = "");

    bool open(string name = "");

    void close();

    string read();

    string readLine();
};
