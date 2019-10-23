#include "config.hpp"

HWND hWnd;
HWND hMain_1_Play;
HWND hNormal_1_ChangeVersion;
HWND hMain_2_Back;
HWND hCombo_2_Version;
HWND hStatic_2_Version;
HWND hStatic_1_Version;
HWND hStatic_x_State;
HWND hStatic_1_Logo;

HFONT mainFont = NULL;
HFONT normalFont = NULL;

CGUpdateConfig updateConfig;
CGVersion currentVer;

int runThreadId = 0;
HANDLE cgProcessHandle = NULL;
string runningDir = "";

void __cdecl CG_runentry(void* data)
{
    // wait for CG to finish
    WaitForSingleObject(cgProcessHandle, INFINITE);

    // make cleanup
    cgProcessHandle = NULL;
    CG_setState("");

    string dir = "instances/" + updateConfig.selectedVersion + "/cargame-" + updateConfig.selectedVersion;
    // Copy global / common files from main update config. (instance->global)
    for(size_t s = 0; s < updateConfig.keptFiles.size(); s++)
    {
        CG_copyFile(dir + "/" + updateConfig.keptFiles[s], "global/" + updateConfig.keptFiles[s]);
    }

    // Copy global / common files from current update config. (instance->global)
    for(size_t s = 0; s < currentVer.keptFiles.size(); s++)
    {
        CG_copyFile(dir + "/" + currentVer.keptFiles[s], "global/" + currentVer.keptFiles[s]);
    }

    // end thread
    _endthread();
}

// definitions

void CG_run()
{
    if(cgProcessHandle)
    {
        CG_err(L"Game is already running!");
        CG_setState("");
        return;
    }

    // Configure environment if not configured
    CG_setState("Initializing");
    CreateDirectoryW(L"instances", NULL);
    CreateDirectoryW(L"global", NULL);

    string zipFileName = "tmp/cargame-" + CG_trim(updateConfig.selectedVersion) + ".zip";

    wstring tag;

    // Find currently launched version
    for(size_t s = 0; s < updateConfig.foundVersions.size(); s++)
    {
        if(updateConfig.foundVersions[s].verSignature == updateConfig.selectedVersion)
        {
            tag = CG_wstring(updateConfig.foundVersions[s].verTag);
            currentVer = updateConfig.foundVersions[s];
        }
    }

    if(updateConfig.selectedVersion == "latest")
        updateConfig.selectedVersion = updateConfig.latestVersion;

    // Specify executable file basing on configuration
    // (If version not specifies executable, try use global executable name)
    string dir = "instances/" + updateConfig.selectedVersion + "/cargame-" + updateConfig.selectedVersion;
    string execName;

    if(CG_is64BitWindows())
        execName = currentVer.execName64.empty() ? updateConfig.execName64 : currentVer.execName64;
    else
        execName = currentVer.execName32.empty() ? updateConfig.execName32 : currentVer.execName32;
    string str = dir + "/" + execName;

    if(!CG_fileHasData(str))
    {
        // Download game files if not downloaded
        if(!CG_fileHasData(zipFileName))
        {
            // Download package
            CreateDirectoryW(L"tmp", NULL);
            string s = CG_downloadFiles(tag, CG_wstring(updateConfig.selectedVersion));
            if(s.empty())
            {
                CG_setState("");
                return;
            }

            // Save package
            CG_saveFile(s, zipFileName, false);
        }

        // Unpack game files
        if(!CG_unpackTo(zipFileName, "instances/" + updateConfig.selectedVersion))
        {
            CG_setState("");
            return;
        }

        // Delete all temporary files
        remove(zipFileName.c_str());
    }

    cout << "Copying " << updateConfig.keptFiles.size() << " files from main config and " << currentVer.keptFiles.size() << " files from version config" << endl;
    CG_setState("Copying Files");
    // Copy global / common files from main update config. (global->instance)
    for(size_t s = 0; s < updateConfig.keptFiles.size(); s++)
    {
        CG_copyFile("global/" + updateConfig.keptFiles[s], dir + "/" + updateConfig.keptFiles[s]);
    }

    // Copy global / common files from current update config. (global->instance)
    for(size_t s = 0; s < currentVer.keptFiles.size(); s++)
    {
        CG_copyFile("global/" + currentVer.keptFiles[s], dir + "/" + currentVer.keptFiles[s]);
    }

    // Finally, run the game !
    cout << "Running " << str << endl;
    CG_setState("Running " + str);
    char* pn = const_cast<char*>(str.c_str());
    PROCESS_INFORMATION pi;
    ZeroMemory(&pi, sizeof(pi));
    STARTUPINFO si;
    ZeroMemory(&si, sizeof(si));
    cout << "Dir=" << dir << endl;
    bool b = CreateProcessA(pn, pn, NULL, NULL, FALSE, 0, NULL, dir.c_str(), &si, &pi);
    if(!b)
    {
        CG_err(L"Running error: " + to_wstring(GetLastError()));
        CG_setState("");
        cgProcessHandle = NULL;
        return;
    }
    else
    {
        cgProcessHandle = pi.hProcess;
        _beginthread(CG_runentry, 0, NULL);
    }
}

// main function

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int cmd)
{
    std::locale::global(std::locale(""));
    runningDir.resize(MAX_PATH);
    GetCurrentDirectory(MAX_PATH, const_cast<char*>(runningDir.data()));

    MSG msg;
    WNDCLASSEXW wc;
    HICON icon = CG_loadIcon(CG_ICON);
    HICON logo = CG_loadIcon(CG_LOGO);

    wc.cbClsExtra = 0;
    wc.cbSize = sizeof(WNDCLASSEXW);
    wc.cbWndExtra = 0;
    LOGBRUSH lb;
    lb.lbColor = RGB(50, 40, 40);
    lb.lbHatch = 0;
    lb.lbStyle = BS_SOLID;
    wc.hbrBackground = CreateBrushIndirect(&lb);
    wc.hCursor = LoadCursorA(NULL, IDC_ARROW);
    wc.hIcon = icon;
    wc.hIconSm = icon;
    wc.hInstance = hInst;
    wc.lpfnWndProc = CG_eventHandler;
    wc.lpszClassName = CG_WNDCLASS;
    wc.lpszMenuName = NULL;
    wc.style = NULL;

    if(!RegisterClassExW(&wc))
    {
        CG_err(L"Couldn't register window class!");
        return 1;
    }

    hWnd = CreateWindowExW(WS_EX_CLIENTEDGE, CG_WNDCLASS, L"CarGame Launcher", WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU, CW_USEDEFAULT, CW_USEDEFAULT, 500, 500, HWND_DESKTOP, NULL, hInst, NULL);

    if(!hWnd)
    {
        CG_err(L"Couldn't create window!");
        return 1;
    }

    hMain_1_Play = CG_createMainButton(100, 20, 300, 60, L"PLAY CG", CG_BUTTON_1_PLAY, false);
    hNormal_1_ChangeVersion = CG_createNormalButton(150, 150, 200, 30, L"CHANGE VERSION", CG_BUTTON_1_CHVER, false);
    hMain_2_Back = CG_createMainButton(166, 80, 167, 40, L"BACK", CG_BUTTON_2_BACK, false);

    hCombo_2_Version = CreateWindowExW(NULL, L"COMBOBOX", NULL, WS_CHILD | CBS_DROPDOWNLIST | WS_VSCROLL, 166, 20, 167, 300, hWnd, (HMENU)CG_COMBO_2_VERID, hInst, NULL);
    hStatic_1_Version = CreateWindowExW(NULL, L"STATIC", L"version: ", WS_CHILD | SS_CENTER, 200, 100, 100, 40, hWnd, NULL, hInst, NULL);
    hStatic_2_Version = CreateWindowExW(NULL, L"STATIC", L"Version: ", WS_CHILD | SS_RIGHT, 10, 20, 156, 30, hWnd, NULL, hInst, NULL);
    hStatic_x_State = CreateWindowExW(NULL, L"STATIC", L"", WS_CHILD | SS_CENTER, 0, 450, 500, 50, hWnd, NULL, hInst, NULL);
    hStatic_1_Logo = CreateWindowExW(NULL, L"STATIC", NULL, WS_CHILD | SS_ICON | SS_REALSIZECONTROL | SS_NOTIFY | SS_REALSIZEIMAGE, 0, 250, 500, 200, hWnd, NULL, hInst, NULL);

    CG_setFont(hCombo_2_Version, false);
    CG_setFont(hStatic_1_Version, false);
    CG_setFont(hStatic_2_Version, false);
    CG_setFont(hStatic_x_State, false);

    ShowWindow(hWnd, cmd);
    UpdateWindow(hWnd);

    CG_setState("");
    CG_switchScreen(1);
    CG_versionUpdate(L"");

    //SendMessage(hStatic_1_Logo, STM_SETICON, (WPARAM)logo, 0);

    // get latest version signature
    for(size_t s = 0; s < updateConfig.foundVersions.size(); s++)
    {
        if(updateConfig.foundVersions[s].id == updateConfig.currentVersionId)
        {
            updateConfig.latestVersion = updateConfig.foundVersions[s].verSignature;
        }
    }

    // load config
    ifstream file("launcher.options");
    if(file.good())
    {
        string str = "latest", latest;

        file >> str >> latest;
        if(str == "latest")
            str = updateConfig.latestVersion;
        if(updateConfig.latestVersion.empty())
            updateConfig.latestVersion = str = latest;
        SetWindowText(hStatic_1_Version, ("version: " + updateConfig.selectedVersion).c_str());
        file.close();
    }

    while(GetMessageW(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }
    return msg.wParam;
}

LRESULT CALLBACK CG_eventHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch(msg)
    {
        case WM_CLOSE:
            if(!cgProcessHandle)
            {
                DestroyWindow(hWnd);
            }
            return 0;
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        case WM_COMMAND:
        {
            switch(HIWORD(wParam))
            {
                case CBN_SELCHANGE:
                {
                    if((HWND)lParam == hCombo_2_Version)
                    {
                        wstring str;
                        int index = SendMessageW(hCombo_2_Version, CB_GETCURSEL, NULL, NULL);
                        size_t s = SendMessageW(hCombo_2_Version, CB_GETLBTEXTLEN, (WPARAM)index, NULL);
                        str.resize(s);
                        SendMessageW(hCombo_2_Version, CB_GETLBTEXT, (WPARAM)index, (LPARAM)str.data());

                        if(updateConfig.selectedVersion == "latest")
                            updateConfig.selectedVersion = updateConfig.latestVersion;

                        CG_versionUpdate(str);
                        updateConfig.selectedVersion = CG_string(str);

                        ofstream file("launcher.options");
                        if(file.good())
                        {
                            file << updateConfig.selectedVersion << " " << updateConfig.latestVersion;
                            file.close();
                        }

                        return 0;
                    }
                    break;
                }
            }
            switch(wParam)
            {
                case CG_BUTTON_1_CHVER: CG_switchScreen(2); break;
                case CG_BUTTON_2_BACK: CG_switchScreen(1); break;
                case CG_BUTTON_1_PLAY: CG_run(); break;
            }
            break;
        }
        case WM_CTLCOLORSTATIC:
        {
            HDC hdcStatic = (HDC) wParam;
            SetTextColor(hdcStatic, RGB(255, 255, 255));
            SetBkColor(hdcStatic, RGB(50,40,40));
            return (INT_PTR)CreateSolidBrush(RGB(50,40,40));
        }
    }
    return DefWindowProcW(hWnd, msg, wParam, lParam);
}
