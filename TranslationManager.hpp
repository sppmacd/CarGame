#pragma once

#include <map>
#include <vector>
#include <SFML/System.hpp>

using namespace std;
using namespace sf;

class TranslationEntry
{
    vector<String> translation;
public:
    TranslationEntry(String in);
    String getValue(initializer_list<String> values);
};

class TranslationManager
{
    String displayLangName;
    String displayCountryName;
    String languageCode;

public:
    map<String, TranslationEntry> translations;

    bool loadFromFile(String code);
    void setDisplay(String name, String country);
    void addTranslation(String unlocalized, String localized);
    String get(String unlocalized, initializer_list<String> values = {});
};
