#pragma once

#include <vector>
#include <string>

using namespace std;

namespace FileUtil
{
    vector<string> listFiles(string directory);
    vector<string> getFileList(string folder, string pattern, int depth = 0);

    enum FileType
    {
        DIRECTORY,
        FILE,
        UNKNOWN,
        NOTEXISTING
    };

    FileType getFileType(string name);
}
