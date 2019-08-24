#pragma once

#include <SFML/Network.hpp>
#include <vector>
#include <string>

using namespace std;
using namespace sf;

struct FileToDownload
{
    string fileName;
    string fileUrl;
};

class UpdateChecker
{
    vector<FileToDownload> filesToDownload;
    vector<string> filesToDelete;
    string newUpdateString;
    static string currentUpdateString;
public:
    bool updateMade;

    // Checks for updates. Returns true if update was found, false if not found or error.
    bool checkUpdates();

    // Makes update. Downloads all required files.
    void makeUpdate();

    // Parses update file.
    bool parseUpdateFile(string in);

    // Gets new CG version signature.
    string getVersion();
};
