#include "FileUtil.hpp"
#include <SFML/Config.hpp>
#include <iostream>
#ifdef SFML_SYSTEM_WINDOWS
#include <windows.h>
#include <shlwapi.h>
#endif // SFML_SYSTEM_WINDOWS
#include <sys/types.h>
#include <sys/stat.h>

namespace FileUtil
{
    vector<string> listFiles(string directory)
    {
        return getFileList(directory, "*.*");
    }
    FileType getFileType(string fileName)
    {
        struct stat info;

        if(stat(fileName.c_str(), &info) != 0)
            return FileUtil::NOTEXISTING;
        else if(info.st_mode & S_IFDIR)  // S_ISDIR() doesn't exist on my windows
            return FileUtil::DIRECTORY;
        else
            return FileUtil::FILE;
    }
    #ifdef SFML_SYSTEM_WINDOWS
        vector<string> getFileList(string folder, string pattern, int depth)
        {
            if(depth < 64)
            {
                vector<string> names;
                string search_path = folder + "/" + pattern;
                WIN32_FIND_DATA fd;
                HANDLE hFind = FindFirstFile(search_path.c_str(), &fd);
                if(hFind != INVALID_HANDLE_VALUE)
                {
                    do
                    {
                        // read all (real) files in current folder
                        // , delete '!' read other 2 default folder . and ..
                        if(!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
                        {
                            //cout << (folder + "/" + fd.cFileName) << endl;
                            names.push_back(folder + "/" + fd.cFileName);
                        }
                    } while(FindNextFile(hFind, &fd));
                    FindClose(hFind);
                }

                hFind = FindFirstFile((folder + "/*").c_str(), &fd);
                if(hFind != INVALID_HANDLE_VALUE)
                {
                    do
                    {
                        if(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY && string(fd.cFileName) != "." && string(fd.cFileName) != "..")
                        {
                            vector<string> fileNames2 = getFileList(folder + "/" + fd.cFileName, pattern, depth + 1);
                            names.insert(names.end(), fileNames2.begin(), fileNames2.end());
                        }
                    } while(FindNextFile(hFind, &fd));
                    FindClose(hFind);
                }
                return names;
            }
            else
            {
                return vector<string>();
            }
        }
    #else
        vector<string> getFileList(string, string, int)
        {
            DebugLogger::log("System not supported, cannot list files.", "FileUtil", "ERROR");
            return vector<string>();
        }
    #endif
}
