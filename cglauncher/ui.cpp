#include "config.hpp"

HICON CG_loadIcon(int id)
{
    return LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(id));
}
void CG_setFont(HWND hCtrl, bool bigger)
{
    if(!mainFont)
    {
        LOGFONTW font;
        GetObjectW(GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONTW), &font);
        WCHAR l[LF_FACESIZE] = L"Consolas";
        memcpy(font.lfFaceName, l, LF_FACESIZE);
        font.lfHeight *= 1.3;
        normalFont = CreateFontIndirectW(&font);
        font.lfHeight *= 2;
        mainFont = CreateFontIndirectW(&font);
    }
    SendMessageW(hCtrl, WM_SETFONT, (WPARAM)(bigger ? mainFont : normalFont), FALSE);
}
HWND CG_createNormalButton(int x, int y, int sx, int sy, wstring caption, int id, bool visible)
{
    HWND hButton = CreateWindowExW(0, L"BUTTON", caption.c_str(), WS_BORDER | WS_CHILD | (WS_VISIBLE & visible), x, y, sx, sy, hWnd, (HMENU)id, GetModuleHandleW(NULL), NULL);
    CG_setFont(hButton, false);
    return hButton;
}
HWND CG_createMainButton(int x, int y, int sx, int sy, wstring caption, int id, bool visible)
{
    HWND hButton = CG_createNormalButton(x, y, sx, sy, caption, id, visible);
    CG_setFont(hButton, true);
    return hButton;
}
void CG_switchScreen(int id)
{
    ShowWindow(hStatic_x_State, SW_SHOW);

    switch(id)
    {
        case 1:
        {
            ShowWindow(hMain_1_Play, SW_SHOW);
            ShowWindow(hNormal_1_ChangeVersion, SW_SHOW);
            ShowWindow(hStatic_1_Version, SW_SHOW);
            ShowWindow(hStatic_1_Logo, SW_SHOW);

            ShowWindow(hMain_2_Back, SW_HIDE);
            ShowWindow(hCombo_2_Version, SW_HIDE);
            ShowWindow(hStatic_2_Version, SW_HIDE);
            break;
        }
        case 2:
        {
            ShowWindow(hMain_2_Back, SW_SHOW);
            ShowWindow(hCombo_2_Version, SW_SHOW);
            ShowWindow(hStatic_2_Version, SW_SHOW);

            ShowWindow(hMain_1_Play, SW_HIDE);
            ShowWindow(hNormal_1_ChangeVersion, SW_HIDE);
            ShowWindow(hStatic_1_Version, SW_HIDE);
            ShowWindow(hStatic_1_Logo, SW_HIDE);
            break;
        }
    }
}
void CG_setState(string str, int prg)
{
    string text;
    if(prg == -1)
        text = str;
    else
        text = str + " (" + to_string(prg) + "%)";
    SetWindowTextA(hStatic_x_State, text.c_str());
}
