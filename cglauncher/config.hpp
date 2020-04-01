#pragma once

// includes

#include <iostream>
#include <vector>
#include <string>
#include <windows.h>
#include <winhttp.h>
#include <unzip.h>
#include <fstream>
#include <sstream>
#include <windowsx.h>
#include <shlobj.h>
#include <direct.h>
#include <io.h>
#include <list>
#include <thread>
#include <process.h>
#include <shellapi.h>

using namespace std;

#include "macros.hpp"
#include "structs.hpp"
#include "globals.hpp"

#define CGLAUNCHER_VER 3

// declarations

// <main>
LRESULT CALLBACK CG_eventHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
void CG_run();
void _cdecl CG_runentry(void* data);

// self-update
LRESULT CALLBACK CG_selfUpdateEventHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
void CG_selfUpdateEvent(void* arg);
int CG_runSelfUpdate(HINSTANCE hInst, int cmd);

// <parse>
CGVersion CG_parseUpdate(string in, CGVersion cfg);
CGUpdateConfig CG_parseUpdateConfig(string in);

// <net>
string CG_downloadConfig(wstring code);
string CG_download(wstring serverAddr, wstring serverUrl);
string CG_downloadFiles(wstring tagCode, wstring versionCode);
void CG_versionUpdate(wstring code);
string CG_readUpdateConfig(string name);

// <config>
void CG_err(wstring msg);
bool CG_is64BitWindows();

// <ui>
HICON CG_loadIcon(int id);
void CG_setFont(HWND hCtrl, bool bigger);
HWND CG_createNormalButton(int x, int y, int sx, int sy, wstring caption, int id, bool visible);
HWND CG_createMainButton(int x, int y, int sx, int sy, wstring caption, int id, bool visible);
void CG_switchScreen(int id);
void CG_setState(string str, int prg = -1);
//void CG_startProgressGUI(string str, int& progress);

// <pack>
bool CG_unpackTo(string fileNameSrc, string folderNameDest);

// <strings>
wstring CG_wstring(string c);
string CG_string(wstring c);
string CG_trim(string str);

// <file>
bool CG_fileHasData(string name);
bool CG_copyFile(string fileSrc, string fileDest);
bool CG_createFolder(std::string folderName);
bool CG_folderExists(const char* folderName);
void CG_saveFile(string data, string name, bool notOverride);
