#ifndef PLAYERDATAMANAGER_HPP
#define PLAYERDATAMANAGER_HPP

#include <string>
#include <array>
#include <map>
#include <vector>
#include <HackerMan/Util/Main.hpp>

#include <PlayerAbilityManager.hpp>
#include <PowerPlayerData.hpp>

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

    typedef std::array<int, 2> PlayerEquipment;
    typedef long long FlagList;
    typedef std::map<int, PowerPlayerData> PlayerPowerList;

    //PlayerDataMap main;
    PlayerAbilityManager abilities;
    PlayerEquipment equipment;
    FlagList unlockedLevels;
    //PlayerDataMap module;
    PlayerPowerList powerLevels;
    //PlayerDataMap playerStats;
    //PlayerDataMap temporary;

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
    int tutorialStep;

    // Variable stores if the player started game first one.
    bool isNewPlayer;

    // Points required to get new coin multiplier.
    int pointsToNewMpl;

	// Player highscore, currently used only by renderer.
    long highScore;

	// Total player points, not used yet
    long totalPlayerPoints;

	// Coin multiplier
    int coinMpl;

	// Player coins
    long playerCoins;
};

#endif // PLAYERDATAMANAGER_HPP

