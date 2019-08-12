#include "SettingsManager.hpp"
#include <iostream>
#include <HackerMan/Util/Main.hpp>
#include "GuiSettings.h"
#include "Game.h"

void SettingsManager::registerSetting(string name, SettingsManager::SettingType type, string space, string defaultVal)
{
    SettingsManager::Setting setting;
    setting.defaultVal = defaultVal;
    setting.space = space;
    setting.type = type;
    setting.currentVal = defaultVal;
    string key = space + ":" + name;
    bool exists = settingRegistered(name, space);

    if(exists)
    {
        cout << "SettingsManager: The specified setting " << key << " already exists!" << endl;
        return;
    }
    settings.insert(make_pair(key, setting));
}

void SettingsManager::loadSettings(string fileName)
{
    cout << "SettingsManager: Loading settings from file: " << fileName << endl;
    HMDataMap hmdatamap;
    hmdatamap.loadFromFile(fileName);
    auto settingsMap = hmdatamap.getMap();
    typedef pair<pair<string,string>,string> Pair;

    for(Pair _pair: settingsMap)
    {
        string space = _pair.first.first;
        string key = _pair.first.second;
        string val = _pair.second;
        setSetting(key, val, space);
    }
}

void SettingsManager::registerTrigger(string settingName, SettingsManager::TriggerFunc func, string space)
{
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
    cout << "SettingsManager: Invalid setting " << name << " specified!" << endl;
    return "";
}

double SettingsManager::getSettingNumeric(string setting, string space)
{
    string str = getSetting(setting, space);
    if(str.empty())
    {
        // setting not found or it isn't naturally a number
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
        cout << "SettingsManager: Invalid number format for key " << str << "!" << endl;
        cout << "SettingsManager: " << e.what() << endl;

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
            cout << "SettingsManager: The setting " << str << " is not a number or not exists!" << endl;
            cout << "SettingsManager: " << e.what() << endl;
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
        cout << "SettingsManager: The setting " << key << " is not registered!" << endl;
        return;
    }
    d->second.currentVal = val;
}

bool SettingsManager::triggerSetting(string settingName, string space)
{
    string key = space + ":" + settingName;
    cout << "SettingsManager: Triggering: " << key << endl;
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
        cout << "SettingsManager: Setting trigger was not successful!" << endl;
    }
    return succeed;
}

void SettingsManager::saveSettings(string file)
{
    cout << "SettingsManager: Saving settings to file: " << file << endl;
    ofstream _file(file);
    _file << "// Auto-generated CG settings file" << endl;
    if(_file.fail())
    {
        cout << "SettingsManager: Couldn't save settings!" << endl;
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

GuiSettings* SettingsManager::generateWidgets()
{
    GuiSettings* guisettings = new GuiSettings;
    int counter = 0;
    for(auto it: settings)
    {
        SettingsManager::SettingType type = it.second.type;
        pair<string,string> ns = toNameAndSpace(it.first);
        string spaceUnlocalized = ns.first + "." + ns.second;
        String spaceLocal = Game::instance->translation.get("settings." + spaceUnlocalized);
        guisettings->idToSetting.insert(make_pair(counter, ns));

        switch(type)
        {
            case SettingsManager::BOOLEAN:
            {
                cg::ButtonToggle* bt = new cg::ButtonToggle(guisettings, Vector2f(400.f, 40.f), Vector2f(10.f, counter * 50.f + guisettings->scSettings->getPosition().y + 10.f), spaceLocal, counter, getSettingNumeric(ns.second, ns.first));
                guisettings->scSettings->addWidget(bt);
                break;
            }
            case SettingsManager::NUMERIC:
            {
                cg::Slider* bt = new cg::Slider(guisettings, Vector2f(10.f, counter * 50.f + guisettings->scSettings->getPosition().y + 10.f), 400.f, 1.f, counter);
                bt->setSliderPosition(getSettingNumeric(ns.second, ns.first));
                bt->setLabel(spaceLocal);
                guisettings->scSettings->addWidget(bt);
                break;
            }
            case SettingsManager::CONFIRM_TRIGGER:
            case SettingsManager::TRIGGER:
            {
                cg::Button* bt = new cg::Button(guisettings, Vector2f(400.f, 40.f), Vector2f(10.f, counter * 50.f + guisettings->scSettings->getPosition().y + 10.f), spaceLocal, counter);
                guisettings->scSettings->addWidget(bt);
                break;
            }
            case SettingsManager::TEXTVALUE:
            {
                cg::TextBox* bt = new cg::TextBox(guisettings, Vector2f(10.f, counter * 50.f + guisettings->scSettings->getPosition().y + 10.f), Vector2f(400.f, 40.f), counter);
                bt->setTooltip(spaceLocal);
                bt->setText(getSetting(ns.second, ns.first));
                guisettings->scSettings->addWidget(bt);
                break;
            }
            default:
            {
                cout << "SettingsManager: Invalid setting type specified (" << int(type) << "), cannot generate widget!" << endl;
                break;
            }
        }
        counter++;
    }
    counter = 0;
    for(auto it: triggers)
    {
        pair<string,string> ns = toNameAndSpace(it.first);
        guisettings->triggerToCallId.insert(make_pair(ns, counter));
        guisettings->callIdToSetting.insert(make_pair(counter, ns));
        counter++;
    }
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
    cout << "SettingsManager: invalid setting " << name2 << " specified!" << endl;
    return SettingsManager::BOOLEAN;
}

bool SettingsManager::triggerAllClose()
{
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
        cout << "SettingsManager: On-close trigger call was not successful!" << endl;
    }
    return succeed;
}

