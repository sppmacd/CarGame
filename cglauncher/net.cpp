#include "config.hpp"

//empty code to download version from server and add selections to combo (sets latest version)
void CG_versionUpdate(wstring code)
{
    CG_setState("Updating Version");
    string ver;
    if(code.empty())
    {
        ver = CG_readUpdateConfig("UPDATE");
        updateConfig = CG_parseUpdateConfig(ver);
        if(updateConfig.errorCode != 0)
        {
            cout << "UpdateConfig error: " << updateConfig.errorCode << " on line " << updateConfig.errorLine << endl;
            //cout << "ver=" << ver << endl;
            ver = "Error";
        }
        cout << "Updater found " << updateConfig.foundVersions.size() << " versions" << endl;

        ComboBox_AddString(hCombo_2_Version, "latest");
        for(size_t s = 0; s < updateConfig.foundVersions.size(); s++)
        {
            ComboBox_AddString(hCombo_2_Version, updateConfig.foundVersions[s].verSignature.c_str());
            if(updateConfig.foundVersions[s].id == updateConfig.currentVersionId)
                ver = updateConfig.foundVersions[s].verSignature;
        }
        updateConfig.selectedVersion = ver;
    }
    else
    {
        ver = CG_string(code);
    }
    SetWindowText(hStatic_1_Version, ("version: " + ver).c_str());
    CG_setState("");
}

string CG_download(wstring serverAddr, wstring serverUrl)
{
    wcout << L"Downloading " << serverUrl << L" from " << serverAddr << endl;
    CG_setState("Connecting " + CG_string(serverAddr));
    HINTERNET hInternet = WinHttpOpen(L"CG Launcher", WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);
    if(hInternet)
    {
        HINTERNET connection = WinHttpConnect(hInternet, serverAddr.c_str(), INTERNET_DEFAULT_HTTPS_PORT, 0);
        if(!connection)
        {
            cout << "Couldn't download file: connection failed: " << GetLastError() << endl;
            return "";
        }
        HINTERNET request = WinHttpOpenRequest(connection, L"GET", serverUrl.c_str(), NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, WINHTTP_FLAG_SECURE);
        if(!request)
        {
            cout << "Couldn't download file: request open failed: " << GetLastError() << endl;
            return "";
        }
        int err = WinHttpSendRequest(request, WINHTTP_NO_ADDITIONAL_HEADERS, 0, WINHTTP_NO_REQUEST_DATA, 0, 0, 0);
        if(!err)
        {
            cout << "Couldn't download file: request failed: " << GetLastError() << endl;
            return "";
        }
        err = WinHttpReceiveResponse(request, NULL);
        if(!err)
        {
            cout << "Couldn't download file: response failed: " << GetLastError() << endl;
            return "";
        }

        DWORD s = 0;
        string buf = "";
        bool first = true;
        size_t counter = 0;

        do
        {
            counter++;
            CG_setState(string("Downloading ") + CG_string(serverUrl) + ": Block " + to_string(counter));
            err = WinHttpQueryDataAvailable(request, &s);
            if(!err)
            {
                cout << "Couldn't download file: size request failed: " << GetLastError() << endl;
                return "";
            }

            DWORD read = 0;
            string buf2;
            buf2.resize(s);
            //cout << s << endl;
            err = WinHttpReadData(request, (LPVOID)buf2.c_str(), s, &read);
            if(!err)
            {
                cout << "Couldn't download file: read failed: " << GetLastError() << endl;
                return "";
            }
            else
            {
                buf += buf2;
                if(first)
                {
                    istringstream iss(buf2);
                    int errcode;
                    if(iss >> errcode)
                    {
                        cout << "Couldn't download file: HTTP ERROR " << errcode << endl;
                    }
                    first = false;
                }
            }
        } while(s > 0);
        buf += '\0';

        CloseHandle(connection);
        CloseHandle(request);
        CloseHandle(hInternet);
        wcout << L"Successfully downloaded '" << serverUrl << "' (" << buf.size() << " bytes)" << endl;
        return buf;
    }
    return "";
}
string CG_downloadConfig(wstring code)
{
    return CG_download(L"raw.githubusercontent.com", L"/sppmacd/CarGame/master/updater/" + code + L".DATA");
}
string CG_downloadFiles(wstring tagCode, wstring versionCode)
{
    return CG_download(L"github.com", L"/sppmacd/CarGame/releases/download/" + tagCode + L"/cargame-" + versionCode + L".zip");
}
string CG_readUpdateConfig(string name)
{
    CreateDirectory("tmp", NULL);

    if(name == "UPDATE")
    {
        string s = CG_downloadConfig(CG_wstring(name));
        if(s.empty())
        {
            ifstream file("tmp/" + name + ".DATA");
            while(!file.fail() && !file.eof())
            {
                string s2;
                getline(file, s2);
                s += (s2 + "\n");
            }
        }
        ofstream file2("tmp/" + name + ".DATA");
        file2.write(s.c_str(), s.size());
        return s;
    }
    else
    {
        string s;

        ifstream file("tmp/" + name + ".DATA");
        if(file.good())
        {
            while(!file.eof())
            {
                string s2;
                getline(file, s2);
                s += (s2 + "\n");
            }
        }
        //cout << "fileval:" << s << endl;
        if(s.empty())
        {
            s = CG_downloadConfig(CG_wstring(name));
        }
		ofstream file2("tmp/" + name + ".DATA");
        file2.write(s.c_str(), s.size() - 1);
        return s;
    }
    return "";
}
