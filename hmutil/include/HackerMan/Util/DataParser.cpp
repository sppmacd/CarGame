#include "DataParser.hpp"
#include <vector>
#include <iostream>

bool HMDataParser::parse(HMDataMap& map, string& data)
{
    // Create map.
    vector<string> m;
    int lp = -1;
    char lastD = ' ';
    data += '\0';
    bool err = false;
    bool comment = false;
    bool mlComment = false;

    if(data.empty())
    {
        cerr << "HMDataParser: data is empty (00)" << endl;
        return false;
    }

    for(unsigned i = 0; i < data.size(); i++)
    {
        char d = data[i];
        if((d == '\n' || d == '\0' || d == '=' || d == ':') && !comment && !mlComment)
        {
            if(d == '\n' && lastD == '\n' && i-lp > 1)
            {
                cerr << "HMDataParser: expected '=' (01)" << endl;
                err = true;
            }

            if(d == '=' && lastD == '=')
            {
                cerr << "HMDataParser: expected new line (02)" << endl;
                err = true;
            }

            if(d == ':' && lastD == '=')
            {
                cerr << "HMDataParser: expected new line (03)" << endl;
                err = true;
            }

            lastD = d;
            if(i-lp <= 1)
            {
                if(d != '\n' && d != '\0')
                    m.push_back("");
            }
            else
            {
                if(lp == -1)
                    m.push_back("\n" + data.substr(lp + 1, i - lp - 1));
                else
                    m.push_back(data.substr(lp, i-lp));
            }
            lp = i;
        }
        else if(d == '\n' && comment)
        {
            comment = false;
            lp = i;
        }
        else if(d == '/' && i != 0 && data[i-1] == '*' && mlComment)
        {
            mlComment = false;
            lp = i;
        }
        else if(d == '/' && data[i+1] == '*')
        {
            mlComment = true;
        }
        else if(d == '/' && data[i+1] == '/')
        {
            comment = true;
        }
    }

    /*if(m.size() % 3 != 0)
    {
        cerr << "HMDataParser: syntax error (04)" << endl;
        return false;
    }*/

    // Add keys
    std::string key,nameSpace,value,_lastmv;
    for(unsigned i = 0; i < m.size() + 1; i++)
    {
        std::string _mv = (i == m.size() ? " " : m[i]);
        //cout << _mv << endl;
        if(_mv[0] == '=')
        {
            value += _mv;
            key = _lastmv;
        }
        else if(_lastmv[0] == '\n' || _lastmv[0] == ':')
        {
            nameSpace += _lastmv;
        }

        if(!value.empty() && !key.empty())
        {
            if(nameSpace.empty())
                nameSpace = "\n";
            //cout << "Adding: '" << nameSpace.substr(1) << "':'" << key.substr(1) << "'='" << value.substr(1) << "'" << endl;
            map.setKey(key.substr(1), value.substr(1), nameSpace.substr(1));
            value.clear();
            key.clear();
            nameSpace.clear();
        }

        _lastmv = _mv;
    }

    return !err;
}
