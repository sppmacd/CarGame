#include "TranslationManager.hpp"

#include "DebugLogger.hpp"
#include "ModuleManager.hpp"

#include <codecvt>
#include <cstring>
#include <fstream>
#include <iostream>
#include <locale>
#include <sstream>

// error codes
// 01 - not enough values given (too many variables in translation string)
// 02 - language file invalid (empty)
// 03 - cannot found translation
// 04 - language file not exist

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
        else if(in[i] == '\\' && i != in.getSize() - 1)
        {
            in.erase(i);
            switch(in[i])
            {
                case '\\': in[i] = '\\'; break;
                case 'n': in[i] = '\n'; break;
                case 't': in[i] = '\t'; break;
                default: break;
            }
        }
    }
}
String TranslationEntry::getValue(initializer_list<String> values)
{
    basic_ostringstream<Uint32> stream;
    vector<String> strings;
    strings.insert(strings.begin(), values.begin(), values.end());
    if(translation.empty())
        return "";
    for(String& str: translation)
    {
        if(str[0] == '%' && str.getSize() > 1)
        {
            size_t val = str[1] - '0';
            if(val < strings.size())
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

TranslationManager::TranslationManager(TranslationManager* parent): parentTranslation(parent)
{

}

bool TranslationManager::loadFromFile(String code)
{
    // Delete all translations.
    for(auto it: translations)
        delete it.second;
    translations.clear();

    languageCode = code;

    std::vector<std::string> modules = {"api"};
    ModuleManager::instance->getModuleNames(modules);

    for(std::string& mod: modules)
    {
        DebugLogger::log("Loading translation: " + mod + ":" + code.toAnsiString(), "TranslationManager");
        wifstream file("res/" + mod + "/lang/" + code + ".lang");
        codecvt_utf8_utf16<wchar_t>* c = new codecvt_utf8_utf16<wchar_t>;
        file.imbue(locale(file.getloc(), c));
        if(!file.good())
        {
            DebugLogger::log("L04: Couldn't open language file: " + mod + ":" + code);
            return false; //err:04
        }

        // parse
        while(!file.eof())
        {
            wstring str;
            getline(file, str);

            String sfStr(str);

            size_t pos = sfStr.find("=");

            if(pos == String::InvalidPos || str[str.find_first_not_of(32)] == '#')
                continue;

            String code = sfStr.substring(0, pos);
            String trs = sfStr.substring(pos + 1);
            addTranslation(code, trs);
        }
    }

    return true;
}
void TranslationManager::addTranslation(String unlocalized, String localized)
{
    DebugLogger::logDbg("Adding translation: " + unlocalized + " --> " + localized, "TranslationManager");
    TranslationEntry* entry = new TranslationEntry(localized);
    translations.insert(make_pair(unlocalized, entry));
}
String TranslationManager::get(String unlocalized, initializer_list<String> values)
{
    /*if(translations.empty())
        return "(translation err 02: " + languageCode + ")";*/
    auto it = translations.find(unlocalized);

    if(it == translations.end())
    {
        if(parentTranslation)
            return parentTranslation->get(unlocalized, values); // Try to search in parent.
        else
            return unlocalized; // No parent and no translation - return default string.
    }

    TranslationEntry* entry = it->second;
    return entry->getValue(values);
}
void TranslationManager::setDisplay(String name, String country)
{
    displayCountryName = country;
    displayLangName = name;
}
void TranslationManager::setParent(TranslationManager* manager)
{
    parentTranslation = manager;
}
