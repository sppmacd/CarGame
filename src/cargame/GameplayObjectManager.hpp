#ifndef GAMEPLAYOBJECTMANAGER_HPP
#define GAMEPLAYOBJECTMANAGER_HPP

#include <string>
#include "GPORegistry.hpp"

#include "CarType.h"
#include "LevelData.h"
#include "Power.h"

#define POWER_REGULAR_ID_START 0
#define POWER_ANTI_ID_START 100

class Game;

class GameplayObjectManager
{
public:
    GPORegistry<int, CarType> carTypes;
    GPORegistry<std::string, LevelData> levels;
    GPORegistry<int, Power> powers;
    GPORegistry<int, void> user;

    // Clear all registries.
    void clear();

    // Register power. It needs some attention because there
    // are Powers and Anti-Powers and Game needs to set some
    // variables basing on ID.
    void registerPower(Game* game, int id, Power* obj);

    // Register car. It assigns ID to CarType after registering.
    int registerCarType(CarType* carType);
};

// Add int to std::string.
std::string operator+(std::string&, int);

#endif // GAMEPLAYOBJECTMANAGER_HPP
