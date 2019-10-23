#include "config.hpp"

void CG_saveFile(string data, string name, bool notOverride)
{
    cout << "Saving '" << name << "' (" << data.size() << " bytes)" << endl;
    CG_setState("Saving " + name);
    if(!notOverride || !CG_fileHasData(name))
    {
        std::ofstream file(name, std::ios::binary | std::ios::out | std::ios::trunc);
        if(!file.fail())
        {
            file.write(data.c_str(), data.size());
        }
        file.flush();
        file.close();
    }
}
bool CG_fileHasData(string name)
{
    std::wifstream file1(name);
    return file1.good();
}
bool CG_createFolder(std::string folderName)
{
    list<std::string> folderLevels;
    char* c_str = (char*)folderName.c_str();

    // Point to end of the string
    char* strPtr = &c_str[strlen(c_str) - 1];

    // Create a list of the folders which do not currently exist
    do
    {
        if (CG_folderExists(c_str))
        {
            break;
        }
        // Break off the last folder name, store in folderLevels list
        do
        {
            strPtr--;
        } while ((*strPtr != '\\') && (*strPtr != '/') && (strPtr >= c_str));
        folderLevels.push_front(string(strPtr + 1));
        strPtr[1] = 0;
    } while (strPtr >= c_str);

    if (_chdir(c_str))
    {
        SetCurrentDirectory(runningDir.c_str());
        return true;
    }

    // Create the folders iteratively
    for (list<std::string>::iterator it = folderLevels.begin(); it != folderLevels.end(); it++)
    {
        if (CreateDirectory(it->c_str(), NULL) == 0)
        {
            SetCurrentDirectory(runningDir.c_str());
            return true;
        }
        _chdir(it->c_str());
    }

    SetCurrentDirectory(runningDir.c_str());
    return false;
}
bool CG_folderExists(const char* folderName)
{
    if (_access(folderName, 0) == -1)
    {
        //File not found
        return false;
    }

    DWORD attr = GetFileAttributes((LPCSTR)folderName);
    if (!(attr & FILE_ATTRIBUTE_DIRECTORY))
    {
        // File is not a directory
        return false;
    }

    return true;
}
bool CG_copyFile(string fileSrc, string fileDest)
{
    ifstream src(fileSrc, std::ios::binary);
	int slashPos = fileDest.find_last_of('\\');
	int backslashPos = fileDest.find_last_of('/');

	if(slashPos == string::npos) slashPos = 0;
	if(backslashPos == string::npos) backslashPos = 0;

    string dirPath = fileDest.substr(0, max(slashPos, backslashPos));
    cout << "Copying " << fileSrc << " to " << fileDest << "(dir=" << dirPath << ")" << endl;
    if(!dirPath.empty())
		CG_createFolder(dirPath);

    if(src.fail())
        return false;

    ofstream dest(fileDest, std::ios::binary);

    if(dest.fail())
        return false;
    while(!src.eof())
    {
        string s;
        getline(src, s);
        s += '\n';
        dest.write(s.c_str(), s.size());
    }
    return true;
}
