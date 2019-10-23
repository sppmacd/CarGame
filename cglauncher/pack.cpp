#include "config.hpp"

bool CG_unpackTo(string src, string dest)
{
    cout << "Unpacking '" << src << "' to '" << dest << "'" << endl;
    CG_setState("Unpacking " + src + " to " + dest);
    HZIP hz = OpenZip(src.c_str(), NULL);
    ZIPENTRY ze;
    GetZipItem(hz, -1, &ze);
    int numitems = ze.index;

    for(int zi = 0; zi < numitems; zi++)
    {
        CG_setState("Unpacking (" + to_string(zi) + " / " + to_string(numitems) + ") ", zi * 100.f / numitems);
        ZIPENTRY ze;
        GetZipItem(hz, zi, &ze);
        ZRESULT zr = UnzipItem(hz, zi, string(dest + string("/") + ze.name).c_str());
        if(zr != ZR_OK)
        {
            cout << "Unpacking error: " << zr << endl;
            CloseZip(hz);
            return false;
        }
    }

    CloseZip(hz);
    return true;
}
