#include "config.hpp"

bool CG_is64BitWindows()
{
    #if defined(_WIN64)
        return TRUE;  // 64-bit programs run only on Win64
    #elif defined(_WIN32)
        // 32-bit programs run on both 32-bit and 64-bit Windows
        // so must sniff
        BOOL f64 = FALSE;
        return IsWow64Process(GetCurrentProcess(), &f64) && f64;
    #else
        return FALSE; // Win64 does not support Win16
    #endif
}
void CG_err(wstring msg)
{
    MessageBoxW(NULL, (L"Error: " + msg).c_str(), L"Error", MB_ICONERROR);
}
