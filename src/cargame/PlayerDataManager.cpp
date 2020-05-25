#include "PlayerDataManager.hpp"
#include "Game.h"
#include "DebugLogger.hpp"

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

        DebugLogger::log("Loading player data from profile version " + std::to_string(ver), "PlayerDataManager");
        highScore = playerData.getNumberKey("highScore", "main", 0);
        playerCoins = playerData.getNumberKey("coins", "main", 0);

        for(size_t t = 0; t < game->gpo.levels.count(); t++)
        {
            std::string id = game->gpo.levels.arr()[t].first.baseId;
            bool unlocked = playerData.getKey("unlocked_" + id, "level", "false") == "true";
            unlockedLevels[id] = unlocked;
        }
        totalPlayerPoints = playerData.getNumberKey("totalPoints", "main", 0);
        coinMpl = playerData.getNumberKey("coinMultiplier", "main", 1);
        pointsToNewMpl = playerData.getNumberKey("pointsToNewMultiplier", "main", 0);

        if(ver == 3)
        {
            for(auto it = game->gpo.powers.arr().begin(); it != game->gpo.powers.arr().end(); it++)
            {
                int c = playerData.getNumberKey("count_" + to_string(it->first.numericId), "power", 0);
                powerLevels[it->first.numericId] = PowerPlayerData(it->second, sqrt(c));
            }
        }
        else if(ver == 4)
        {
            for(auto it = game->gpo.powers.arr().begin(); it != game->gpo.powers.arr().end(); it++)
            {
                int c = playerData.getNumberKey("level_" + to_string(it->first.numericId), "power", 0);
                this->powerLevels[it->first.numericId] = PowerPlayerData(it->second, c);
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
            DebugLogger::log("Unsupported profile version", "PlayerDataManager", "ERROR");
            game->displayError("Tried to load newer/invalid profile version.\n\
                         Try installing a new version of Car Game.", "G04");
            return false;
        }
    }
    else
    {
        ifstream file("data.txt");
        if(file.good())
        {
            cout << "PlayerDataManager: Couldn't load profile! Converting from old player data format (v2)..." << endl;
            DebugLogger::log("Couldn't load profile! Converting from old player data format (v2)...", "PlayerDataManager");

            long long null;

            file
            >> highScore
            >> playerCoins
            >> null //todo
            >> totalPlayerPoints
            >> coinMpl
            >> pointsToNewMpl;
            int i1, i2;
            file
            >> i1 >> i2;
            powerLevels[1] = PowerPlayerData(game->gpo.powers.findById(1), sqrt(i1));
            powerLevels[2] = PowerPlayerData(game->gpo.powers.findById(2), sqrt(i2));
        }
        else
        {
            DebugLogger::log("Couldn't load data.txt! Converting from old player data format (v1)...", "PlayerDataManager");
            file.open("highscore.txt");

            if(file.good())
            {
                long long null;

                file
                >> highScore
                >> playerCoins
                >> null
                >> totalPlayerPoints
                >> coinMpl
                >> pointsToNewMpl;
                DebugLogger::log("Powers are incompatible with old format on " + string(CG_VERSION) + "!", "PlayerDataManager");
            }
            else
            {
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
    for(auto& level: game->gpo.levels.arr())
    {
        std::string id = level.first.baseId;
        playerData.setKey("unlocked_" + id, ((game->isLevelUnlocked(id)) ? "true" : "false"), "level");
    }
    for(auto it = game->gpo.powers.arr().begin(); it != game->gpo.powers.arr().end(); it++)
    {
        playerData.setNumberKey("level_" + to_string(it->first.numericId), powerLevels[it->first.numericId].getLevel(), "power");
    }
    abilities.write(playerData);

    playerData.saveToFile("profile_1.txt");
}

void PlayerDataManager::init()
{
    Game* game = Game::instance;
    highScore = 0;
    playerCoins = 0;

    totalPlayerPoints = 0;
    coinMpl = 1;
    pointsToNewMpl = 200;

    isNewPlayer = true;
    tutorialStep = 1;

    for(auto it = game->gpo.powers.arr().begin(); it != game->gpo.powers.arr().end(); it++)
    {
        powerLevels[it->first.numericId] = PowerPlayerData(it->second);
    }
    for(size_t t = 0; t < equipment.size(); t++)
    {
        equipment[t] = 0;
    }
    abilities.clear();
}
