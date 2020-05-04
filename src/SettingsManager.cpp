#include "SettingsManager.hpp"
#include <iostream>
#include <HackerMan/Util/Main.hpp>
#include "GuiSettings.h"
#include "Game.h"
#include "DebugLogger.hpp"

void SettingsManager::registerSetting(string name, SettingsManager::SettingType type, string space, string defaultVal)
{
    DebugLogger::logDbg("Registering setting: " + space + ":" + name + "=" + defaultVal + " (type=" + std::to_string(type) + ")", "SettingsManager");
    SettingsManager::Setting setting;
    setting.defaultVal = defaultVal;
    setting.space = space;
    setting.type = type;
    setting.currentVal = defaultVal;
    string key = space + ":" + name;
    bool exists = settingRegistered(name, space);

    if(exists)
    {
        DebugLogger::log("The setting already exists: " + key, "SettingsManager", "ERROR");
        return;
    }
    settings.insert(make_pair(key, setting));
}

void SettingsManager::loadSettings(string fileName)
{
    DebugLogger::log("Loading settings from " + fileName, "SettingsManager");
    HMDataMap hmdatamap;
    hmdatamap.loadFromFile(fileName);
    auto settingsMap = hmdatamap.getMap();
    typedef pair<pair<string,string>,string> Pair;

	// the default val will NOT be overwritten if not exist in settings file.
    for(Pair _pair: settingsMap)
    {
        string space = _pair.first.first;
        string key = _pair.first.second;
        string val = _pair.second;
        setSetting(key, val, space);
    }
}

void SettingsManager::resetSettings(string fileName)
{
    DebugLogger::logDbg("Resetting settings to defaults", "SettingsManager");
	remove(fileName.c_str());
	loadSettings(fileName);
	saveSettings(fileName);
}

void SettingsManager::registerTrigger(string settingName, SettingsManager::TriggerFunc func, string space)
{
    DebugLogger::logDbg("Registering trigger: " + space + ":" + settingName, "SettingsManager");
    triggers.insert(make_pair(space + ":" + settingName, func));
}

string SettingsManager::getSetting(string setting, string space)
{
    string name = space + ":" + setting;
    auto it = settings.find(name);
    if(it != settings.end())
    {
        string val = it->second.currentVal;
        return val;
    }
    DebugLogger::log("Invalid setting: " + space + ":" + setting, "SettingsManager", "ERROR");
    return "";
}

double SettingsManager::getSettingNumeric(string setting, string space)
{
    string str = getSetting(setting, space);
    if(str.empty())
    {
        // setting not found so it isn't a number.
        return 0.0;
    }
    try
    {
        double number = stod(str);
        return number;
    }
    catch(exception& e)
    {
        string str = setting + ":" + space;

        DebugLogger::log("Invalid setting format: " + str, "SettingsManager", "ERROR");
        DebugLogger::log("Exception:" + std::string(e.what()), "SettingsManager", "ERROR");

        try
        {
            auto d = settings.find(str);
            if(d == settings.end())
            {
                return 0.0;
            }
            double number = stod(d->second.defaultVal);
            return number;
        }
        catch(exception& e)
        {
            DebugLogger::log("Invalid setting format: " + str, "SettingsManager", "ERROR");
            DebugLogger::log("Exception:" + std::string(e.what()), "SettingsManager", "ERROR");
        }
    }
    return 0.0;
}

void SettingsManager::setSetting(string setting, string val, string space)
{
    string key = space + ":" + setting;
    auto d = settings.find(key);
    if(d == settings.end())
    {
        DebugLogger::log("Invalid setting: " + key, "SettingsManager");
        return;
    }
    d->second.currentVal = val;
}

bool SettingsManager::triggerSetting(string settingName, string space)
{
    string key = space + ":" + settingName;
    DebugLogger::logDbg("Triggering: " + key, "SettingsManager");
    bool succeed = true;
    string val = getSetting(settingName, space);
    for(auto it: triggers)
    {
        if(it.first == key)
        {
            succeed &= it.second(val);
        }
    }
    if(!succeed)
    {
        DebugLogger::log("Trigger was unsuccessful for: " + key, "SettingsManager", "ERROR");
    }
    return succeed;
}

void SettingsManager::saveSettings(string file)
{
    DebugLogger::log("Saving settings to file: " + file, "SettingsManager");
    ofstream _file(file);
    _file << "// Auto-generated CG settings file" << endl;
    if(_file.fail())
    {
        DebugLogger::log("Couldn't save settings" + file, "SettingsManager", "ERROR");
    }
    for(auto it: settings)
    {
        if(it.second.type != SettingsManager::TRIGGER && it.second.type != SettingsManager::CONFIRM_TRIGGER)
        {
            string key = it.first;
            string line = key + "=" + it.second.currentVal;
            _file << line << endl;
        }
    }
}

void SettingsManager::addWidgetsToSettings(GuiSettings* guisettings)
{
	int counter = 100;
    for(auto it: settings)
    {
        SettingsManager::SettingType type = it.second.type;
        pair<string,string> ns = toNameAndSpace(it.first);
        string spaceUnlocalized = ns.first + "." + ns.second;
        String spaceLocal = Game::instance->translation.get("settings." + spaceUnlocalized);
        guisettings->idToSetting.insert(make_pair(counter, ns));

        Vector2f pos = Vector2f(guisettings->scSettings->getSize().x / 2.f - 200.f, (counter - 100) * 60.f + 20.f);

        switch(type)
        {
            case SettingsManager::BOOLEAN:
            {
                cg::ButtonToggle* bt = new cg::ButtonToggle(guisettings, Vector2f(400.f, 40.f), pos, spaceLocal, counter, getSettingNumeric(ns.second, ns.first));
                guisettings->scSettings->addWidget(bt);
                break;
            }
            case SettingsManager::NUMERIC:
            {
                cg::Slider* bt = new cg::Slider(guisettings, pos, 400.f, 1.f, counter);
                bt->setSliderPosition(getSettingNumeric(ns.second, ns.first));
                bt->setLabel(spaceLocal);
                guisettings->scSettings->addWidget(bt);
                break;
            }
            case SettingsManager::CONFIRM_TRIGGER:
            case SettingsManager::TRIGGER:
            {
                cg::Button* bt = new cg::Button(guisettings, Vector2f(400.f, 40.f), pos, spaceLocal, counter);
                guisettings->scSettings->addWidget(bt);
                break;
            }
            case SettingsManager::TEXTVALUE:
            {
                cg::TextBox* bt = new cg::TextBox(guisettings, pos, Vector2f(400.f, 40.f), counter);
                bt->setTooltip(spaceLocal);
                bt->setText(getSetting(ns.second, ns.first));
                guisettings->scSettings->addWidget(bt);
                break;
            }
            default:
            {
                DebugLogger::log("Invalid setting type specified (" + std::to_string(int(type)) + "), cannot generate widget!", "SettingsManager", "ERROR");
                break;
            }
        }
        counter++;
    }
    counter = 101;
    for(auto it: triggers)
    {
        pair<string,string> ns = toNameAndSpace(it.first);
        guisettings->triggerToCallId.insert(make_pair(ns, counter));
        guisettings->callIdToSetting.insert(make_pair(counter, ns));
        counter++;
    }
}

GuiSettings* SettingsManager::generateWidgets()
{
    DebugLogger::logDbg("Generating settings widgets", "SettingsManager");
    GuiSettings* guisettings = new GuiSettings;
    addWidgetsToSettings(guisettings);
    guisettings->manager = this;
    return guisettings;
}

bool SettingsManager::settingRegistered(string name, string space)
{
    auto it = settings.find(space + ":" + name);
    return it != settings.end();
}

bool SettingsManager::triggerRegisteredFor(string name, string space)
{
    auto it = triggers.find(space + ":" + name);
    return it != triggers.end();
}

pair<string,string> SettingsManager::toNameAndSpace(string nameSpace)
{
    int i = nameSpace.find(":");
    string space = nameSpace.substr(0, i);
    string key = nameSpace.substr(i + 1);
    return make_pair(space, key);
}

SettingsManager::SettingType SettingsManager::getSettingType(string name, string space)
{
    string name2 = space + ":" + name;
    auto it = settings.find(name2);
    if(it != settings.end())
    {
        return it->second.type;
    }
    DebugLogger::log("Invalid setting: " + name2, "SettingsManager", "ERROR");
    return SettingsManager::BOOLEAN;
}

bool SettingsManager::triggerAllClose()
{
    DebugLogger::logDbg("Triggering all settings", "SettingsManager");
    bool succeed = true;
    for(auto setting: settings)
    {
        pair<string,string> ns = toNameAndSpace(setting.first);
        if(setting.second.type != SettingsManager::TRIGGER && setting.second.type != SettingsManager::CONFIRM_TRIGGER)
        {
            succeed &= triggerSetting(ns.second, ns.first);
        }
    }
    if(!succeed)
    {
        DebugLogger::log("On-close trigger was unsuccessful", "SettingsManager", "ERROR");
    }
    return succeed;
}

