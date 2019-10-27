#pragma once

//globals

extern HWND hWnd;
extern HWND hMain_1_Play;
extern HWND hNormal_1_ChangeVersion;
extern HWND hMain_2_Back;
extern HWND hCombo_2_Version;
extern HWND hStatic_2_Version;
extern HWND hStatic_1_Version;
extern HWND hStatic_x_State;
extern HWND hStatic_1_Logo;
extern HWND hNormal_sf_Cancel;
extern HWND hStatic_sf_UpdateProgress;

extern HFONT mainFont;
extern HFONT normalFont;

extern CGUpdateConfig updateConfig;
extern CGVersion currentVer;

extern int runThreadId;
extern HANDLE cgProcessHandle;
extern string runningDir;
