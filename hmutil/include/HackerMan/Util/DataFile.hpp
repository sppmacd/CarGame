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
    // Creates file from file name.
    HMDataFile(string name = "");

    // Opens specified file.
    bool open(string name = "");

    // Closes currently opened file.
    void close();

    // Reads the whole file and returns it.
    string read();

    // Reads one line from file and returns it.
    string readLine();
};
