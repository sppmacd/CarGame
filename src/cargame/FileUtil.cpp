#include "FileUtil.hpp"
#include <SFML/Config.hpp>
#include <iostream>
#ifdef SFML_SYSTEM_WINDOWS
#include <windows.h>
#include <shlwapi.h>
#else
#include <dirent.h>
#endif // SFML_SYSTEM_WINDOWS
#include <sys/types.h>
#include <sys/stat.h>

#include <cargame/DebugLogger.hpp>

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
            return FileUtil::FileType::NOTEXISTING;
        else if(info.st_mode & S_IFDIR)  // S_ISDIR() etc. doesn't exist on Windows
            return FileUtil::FileType::DIRECTORY;
        else if(info.st_mode & S_IFLNK)
            return FileUtil::FileType::SYMLINK;
        else if(info.st_mode & S_IFCHR)
            return FileUtil::FileType::CHARACTER_DEVICE;
        else if(info.st_mode & S_IFBLK)
            return FileUtil::FileType::BLOCK_DEVICE;
        else if(info.st_mode & S_IFIFO)
            return FileUtil::FileType::FIFO;
        else if(info.st_mode & S_IFSOCK)
            return FileUtil::FileType::SOCKET;
        else
            return FileUtil::FileType::FILE;
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
        // TODO: handle pattern
        vector<string> getFileList(string folder, string pattern, int depth)
        {
            if(depth < 64)
            {
                vector<string> files;

                DIR* dir = opendir(folder.c_str());
                if(!dir)
                    return {};
                dirent* _dirent = nullptr;
                while(_dirent = readdir(dir))
                {
                    if(S_ISDIR(_dirent->d_type) && string(_dirent->d_name) != "." && string(_dirent->d_name) != "..")
                    {
                        vector<string> files2 = getFileList(folder + "/" + _dirent->d_name, pattern, depth + 1);
                        files.insert(files.end(), files2.begin(), files2.end());
                    }
                    else
                        files.push_back(folder + "/" + _dirent->d_name);
                }
                closedir(dir);
                return files;
            }
            else
            {
                DBG::log("(getFileList) Maximum depth encountered (64), aborting", "FileUtil", "WARN");
                return vector<string>();
            }
        }
    #endif
}
