#pragma once

#include <map>
#include <vector>
#include <string>
#include <CG/CG.h>
;
class GuiSettings;

using namespace std;

class SettingsManager
{
public:
    // Trigger func returns true if action was successful, false otherwise
    typedef bool(*TriggerFunc)(string);

    enum SettingType
    {
        BOOLEAN, //drawn as toggle button
        NUMERIC, //drawn as slider bar
        TEXTVALUE, //drawn as text box
        TRIGGER, //drawn as button
        CONFIRM_TRIGGER //drawn as button, with confirm dialog
    };

    /*
    enum SubGUIType
    {
        MAIN,
        GRAPHICS,
        SOUND,
        OTHER
    };
    */

    // Structure representing one registered setting.
    struct Setting
    {
        string defaultVal;
        string currentVal;
        string space; //namespace
        SettingType type;
    };

    // SettingsManager();

    // Adds setting to manager. All registered settings will be displayed in GUI.
    // Space - prefix added to name in the following way: settings.[space]:[settingName].
    // Space is saved in file in the following way: [space]:[settingName]=[value].
    void registerSetting(string name, SettingType type, string space = "", string defaultVal = "");

    // Loads settings from specified file.
    void loadSettings(string fileName);
	
	// Resets settings in %fileName to default values.
    void resetSettings(string fileName);

    // Adds trigger to setting. Will be called on click (for TRIGGERs) or on save (for other types)
    void registerTrigger(string settingName, TriggerFunc func, string space = "");

    // Gets setting value.
    string getSetting(string setting, string space = "");

    // Gets setting value, converting it to number first. Returns default value if conversion failed.
    double getSettingNumeric(string setting, string space = "");

    // Sets setting value.
    void setSetting(string setting, string val, string space = "");

    // Calls specified trigger. If not found, nothing is done.
    bool triggerSetting(string settingName, string space = "");

    // Saves settings to file. Calls all value triggers if set.
    void saveSettings(string file);

    // Generates widget list by registered settings. Used by GuiSettings.
    GuiSettings* generateWidgets();
	
	// Adds corresponding widgets for all settings to specified GuiSettings.
	void addWidgetsToSettings(GuiSettings* guisettings);

    // Check if setting is registered.
    bool settingRegistered(string name, string space = "");

    // Check if trigger is registered for specified setting.
    bool triggerRegisteredFor(string name, string space = "");

    pair<string,string> toNameAndSpace(string nameSpace);

    SettingType getSettingType(string name, string space = "");

    bool triggerAllClose();
private:
    // map key: [space]:[settingName]
    map<string, Setting> settings;
    multimap<string, TriggerFunc> triggers;
};
