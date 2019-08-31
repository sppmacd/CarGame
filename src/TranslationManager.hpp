#pragma once

#include <map>
#include <vector>
#include <SFML/System.hpp>
;
using namespace std;
using namespace sf;

class TranslationEntry
{
    vector<String> translation;
public:
    TranslationEntry(String in);
    String getValue(initializer_list<String> values);
};

// The translation manager. Note that all strings must be Unicode format to support special characters.
class TranslationManager
{
    String displayLangName;
    String displayCountryName;
    String languageCode;
    TranslationManager* parentTranslation;
public:
    // Construct from parent. If the translation could not be found,
    // tries to search in parent. If no parent and not found, returns
    // unlocalized string. NULL parent means no parent.
    TranslationManager(TranslationManager* parent = NULL);

    // The translation map.
    map<String, TranslationEntry> translations;

    // Load translations from file.
    bool loadFromFile(String code);

    // Set displayed language info (name and country).
    void setDisplay(String name, String country);

    // Add translation to manager.
    void addTranslation(String unlocalized, String localized);

    // Gets translation by unlocalized string, using specified values.
    String get(String unlocalized, initializer_list<String> values = {});

    // Sets parent to translation manager.
    void setParent(TranslationManager* manager);
};
