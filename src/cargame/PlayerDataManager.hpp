#ifndef PLAYERDATAMANAGER_HPP
#define PLAYERDATAMANAGER_HPP

#include <string>
#include <array>
#include <map>
#include <vector>
#include <HackerMan/Util/Main.hpp>

#include "LevelData.h"
#include "ModuleIdentifier.hpp"
#include "PlayerAbilityManager.hpp"
#include "PowerPlayerData.hpp"

class PlayerDataManager
{
    // Loaded from file hmUtil data map that couldn't be parsed
    // (kept for compability with other versions)
    HMDataMap playerData;

public:
    enum DataGroup
    {
        DataGroup_main,
        DataGroup_ability,
        DataGroup_achievement,
        DataGroup_equipment,
        DataGroup_level,
        DataGroup_mod,
        DataGroup_power,
        DataGroup_stat,
        DataGroup_tmp,

        DataGroup_Count,

        DataGroup_None,
        DataGroup_Any
    };
    // Data groups:
    // main - Main data (e.g. coins). The data are variable and can be removed.
    // ability - Ability levels
    // achievement - Achievement stats (progress and flags)
    // equipment - Currently equipped powers
    // level - Unlocked levels
    // mod - Module-specific player data
    // power - Power levels
    // stat - Stats. Can only be added. [0.3+]
    // tmp - Temporary data, are resetted every game launch.

    // Creates PlayerDataManager with default values.
    PlayerDataManager();

    // Loads player data from specified file.
    bool load(std::string fileName);

    // Saves player data to specified file.
    bool save(std::string fileName);

    // Fills player data by default values.
    void init();

    typedef std::array<ModuleIdentifier, 2> PlayerEquipment;
    typedef std::map<LevelData::MapType, bool> FlagList;
    typedef std::map<ModuleIdentifier, PowerPlayerData> PlayerPowerList;

    //PlayerDataMap main;               //group: main
    PlayerAbilityManager abilities;     //group: ability
    //AchievementManager achievements;  //group: achievement
    PlayerEquipment equipment;          //group: equipment
    FlagList unlockedLevels;            //group: level
    //PlayerDataMap module;             //group: mod
    PlayerPowerList powerLevels;        //group: power
    //PlayerDataMap playerStats;        //group: stat
    //PlayerDataMap temporary;          //group: tmp

    /////////////////////////
    // ONLY FOR PRERELEASE //
    /////////////////////////

    // Player tutorial step. Can reset by restart.
	// 0 - no tutorial
	// 1 - start game in main menu
	// 2 - buy map
	// 3 - select map
	// 4 - destroy car
	// 5 - avoid bomb
	// 6 - don't let car leave screen
	// 7 - shop
    int tutorialStep; //to tmp:

    // Variable stores if the player started game first one.
    bool isNewPlayer; //to tmp:

    // Points required to get new coin multiplier.
    int pointsToNewMpl; //to main:

	// Player highscore, currently used only by renderer.
    long highScore; //to main:

	// Total player points, not used yet
    long totalPlayerPoints; //to stat:

	// Coin multiplier
    int coinMpl; //to main:

	// Player coins
    long playerCoins; //to main:
};

#endif // PLAYERDATAMANAGER_HPP

