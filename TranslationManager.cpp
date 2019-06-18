#include "TranslationManager.hpp"
#include <cstring>
#include <sstream>
#include <fstream>
#include <iostream>

// error codes
// 01 - not enough values given (too many variables in translation string)
// 02 - cannot load language file
// 03 - cannot found translation

TranslationEntry::TranslationEntry(String in)
{
    size_t lastp = 0;
    for(size_t i = 0; i < in.getSize(); i++)
    {
        if(in[i] == '%' && i != in.getSize() - 1 && isdigit(in[i+1]))
        {
            translation.push_back(in.substring(lastp, i - lastp));
            translation.back().replace("%%", "%");
            lastp = i + 2;
            translation.push_back(String("%") + in[i+1]);
        }
        else if(i == in.getSize() - 1)
        {
            translation.push_back(in.substring(lastp, i - lastp + 1));
            translation.back().replace("%%", "%");
        }
    }
}
String TranslationEntry::getValue(initializer_list<String> values)
{
    basic_ostringstream<Uint32> stream;
    vector<String> strings;
    strings.insert(strings.begin(), values.begin(), values.end());
    for(String& str: translation)
    {
        if(str[0] == '%')
        {
            size_t val = str[1] - '0';
            if(val < strings.size() && val >= 0)
                stream << strings[val].toUtf32();
            else
            {
                return "(translation err 01)";
            }
        }
        else
        {
            stream << str.toUtf32();
        }
    }
    return stream.str();
}

///////////////////////////////////////

bool TranslationManager::loadFromFile(String code)
{
    languageCode = code;
    wifstream file("res/lang/" + code + ".lang");
    if(!file.good())
        return false;

    while(!file.eof())
    {
        wstring str;
        getline(file, str);

        String sfStr(str);

        size_t pos = sfStr.find("=");

        if(pos == String::InvalidPos || str[str.find_first_not_of(L" ")] == '#')
            continue;

        String code = sfStr.toUtf32().substr(0, pos);
        String trs = sfStr.toUtf32().substr(pos + 1);
        addTranslation(code, trs);
    }

    return true;
}
void TranslationManager::addTranslation(String unlocalized, String localized)
{
    TranslationEntry entry(localized);
    translations.insert(make_pair(unlocalized, entry));
}
String TranslationManager::get(String unlocalized, initializer_list<String> values)
{
    if(translations.empty())
        return "(translation err 02: " + languageCode + ")";
    auto it = translations.find(unlocalized);

    if(it == translations.end())
        return "(translation err 03: " + unlocalized + ")";

    TranslationEntry entry = it->second;
    return entry.getValue(values);
}
void TranslationManager::setDisplay(String name, String country)
{
    displayCountryName = country;
    displayLangName = name;
}
