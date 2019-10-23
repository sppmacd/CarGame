#include "config.hpp"

string CG_trim(string str)
{
    /*if(str.empty())
        return "";
    int fs = str.find_first_not_of(' ');
    int ls = str.find_last_not_of(' ');

    if(fs == string::npos) fs = 0;

    if(ls == string::npos)
        ls = str.size();
    else
        ls++;

    string tmp;
    tmp.resize(str.size() - fs - ls);
    std::copy(str.begin() + fs, str.begin() + ls, tmp.begin());*/
    return str;
}
wstring CG_wstring(string c)
{
    wstring w;
    w.resize(c.size());
    std::copy(c.begin(), c.end(), w.begin());
    return w;
}
string CG_string(wstring c)
{
    string w;
    w.resize(c.size());
    std::copy(c.begin(), c.end(), w.begin());
    return w;
}
