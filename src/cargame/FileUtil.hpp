#pragma once

#include <vector>
#include <string>

using namespace std;

namespace FileUtil
{
    // Recursively list all files in %directory (excluding special "." and "..").
    vector<string> listFiles(string directory);

    // Recursive file listing in %folder by %pattern. Max depth is 64-%depth.
    // It excludes special "." and ".." directories.
    vector<string> getFileList(string folder, string pattern, int depth = 0);

    enum FileType
    {
        DIRECTORY,
        FILE,
        UNKNOWN,
        NOTEXISTING
    };

    // Get file type of %name: FILE, DIRECTORY on NONEXISTING. UNKNOWN value is not used yet - if the file is
    // other type than normal file, it will return FILE nevertheless.
    FileType getFileType(string name);
}
