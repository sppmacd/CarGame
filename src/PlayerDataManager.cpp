#include "PlayerDataManager.hpp"
#include "Game.h"

#include <iostream>
#include <cmath>

using namespace std;

PlayerDataManager::PlayerDataManager()
{
    // cannot init here because here game not finished init
    //init();
}

bool PlayerDataManager::load(std::string fileName)
{
    Game* game = Game::instance;

    game->usablePowerIds.clear();

    playerData = HMDataMap(); //clear all data
    if(playerData.loadFromFile("profile_1.txt"))
    {
        int ver = playerData.getNumberKey("version", "", 0);

        cout << "PlayerDataManager: Loading player data from profile version " << ver << endl;
        highScore = playerData.getNumberKey("highScore", "main", 0);
        playerCoins = playerData.getNumberKey("coins", "main", 0);

        for(size_t t = 0; t < game->levelRegistry.size(); t++)
        {
            bool unlocked = playerData.getKey("unlocked_" + game->levelRegistry[t].first, "level", "false") == "true";
            unlockedLevels |= (unlocked << t);
        }
        totalPlayerPoints = playerData.getNumberKey("totalPoints", "main", 0);
        coinMpl = playerData.getNumberKey("coinMultiplier", "main", 1);
        pointsToNewMpl = playerData.getNumberKey("pointsToNewMultiplier", "main", 0);

        if(ver == 3)
        {
            for(auto it = game->powerRegistry.begin(); it != game->powerRegistry.end(); it++)
            {
                int c = playerData.getNumberKey("count_" + to_string(it->first), "power", 0);
                powerLevels[it->first] = PowerPlayerData(it->second, sqrt(c));
            }
        }
        else if(ver == 4)
        {
            for(auto it = game->powerRegistry.begin(); it != game->powerRegistry.end(); it++)
            {
                int c = playerData.getNumberKey("level_" + to_string(it->first), "power", 0);
                this->powerLevels[it->first] = PowerPlayerData(it->second, c);
            }
            for(size_t t = 0; t < equipment.size(); t++)
            {
                equipment[t] = playerData.getNumberKey("power_" + to_string(t), "equipment", 0);
                if(equipment[t] != 0)
                    game->usablePowerIds.push_back(equipment[t]);
            }
        }
        else
        {
            cout << "PlayerDataManager: Unsupported profile version!" << endl;
            game->displayError("Tried to load newer/invalid profile version.\n\
                         Try installing a new version of Car Game.\n\
                         Code: G04");
            game->initProfile();
            return false;
        }
    }
    else
    {
        ifstream file("data.txt");
        if(file.good())
        {
            cout << "PlayerDataManager: Couldn't load profile! Converting from old player data format (v2)..." << endl;

            file
            >> highScore
            >> playerCoins
            >> unlockedLevels
            >> totalPlayerPoints
            >> coinMpl
            >> pointsToNewMpl;
            int i1, i2;
            file
            >> i1 >> i2;
            powerLevels[1] = PowerPlayerData(game->powerRegistry[1], sqrt(i1));
            powerLevels[2] = PowerPlayerData(game->powerRegistry[2], sqrt(i2));
        }
        else
        {
            file.open("highscore.txt");

            if(file.good())
            {
                cout << "PlayerDataManager: Couldn't load data.txt! Converting from old player data format (v1)..." << endl;
                file
                >> highScore
                >> playerCoins
                >> unlockedLevels
                >> totalPlayerPoints
                >> coinMpl
                >> pointsToNewMpl;
                cout << "PlayerDataManager: Powers are incompatible with old format on " + string(CG_VERSION) + "!" << endl;
            }
            else
            {
                game->initProfile();
                return false;
            }
        }
    }
    //this->abilities = PlayerAbilityManager();
    abilities.read(playerData);
    return true;
}

bool PlayerDataManager::save(std::string fileName)
{
    Game* game = Game::instance;

    // Save using hmUtil
    playerData.setNumberKey("version", 4, "");

    playerData.setNumberKey("highScore", highScore, "main");
    playerData.setNumberKey("coins", playerCoins, "main");
    playerData.setNumberKey("totalPoints", totalPlayerPoints, "main");
    playerData.setNumberKey("coinMultiplier", coinMpl, "main");
    playerData.setNumberKey("pointsToNewMultiplier", pointsToNewMpl, "main");

    for(size_t t = 0; t < equipment.size(); t++)
    {
        playerData.setNumberKey("power_" + to_string(t), equipment[t], "equipment");
    }
    for(size_t t = 0; t < game->levelRegistry.size(); t++)
    {
        playerData.setKey("unlocked_" + game->levelRegistry[t].first, (game->isLevelUnlocked((LevelData::MapType)t) ? "true" : "false"), "level");
    }
    for(auto it = game->powerRegistry.begin(); it != game->powerRegistry.end(); it++)
    {
        playerData.setNumberKey("level_" + to_string(it->first), powerLevels[it->first].getLevel(), "power");
    }
    abilities.write(playerData);

    playerData.saveToFile("profile_1.txt");
}

void PlayerDataManager::init()
{
    Game* game = Game::instance;
    highScore = 0;
    playerCoins = 0;
    unlockedLevels  = 0;

    totalPlayerPoints = 0;
    coinMpl = 1;
    pointsToNewMpl = 200;

    isNewPlayer = true;
    tutorialStep = 1;

    for(auto it = game->powerRegistry.begin(); it != game->powerRegistry.end(); it++)
    {
        powerLevels[it->first] = PowerPlayerData(it->second);
    }
    for(size_t t = 0; t < equipment.size(); t++)
    {
        equipment[t] = 0;
    }
    abilities.clear();
}
