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
    map<String, TranslationEntry> translations;
    String displayLangName;
    String displayCountryName;
    String languageCode;

public:
    bool loadFromFile(String code);
    void addTranslation(String unlocalized, String localized);
    String get(String unlocalized, initializer_list<String> values = {});
};
