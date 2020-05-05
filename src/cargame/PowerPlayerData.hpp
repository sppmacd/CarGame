#pragma once

#include "Power.h"

class Game;

class PowerPlayerData
{
    unsigned int level;
    Power* powerInst;
public:
    PowerPlayerData(Power* power = NULL, unsigned int levelIn = 0);
    unsigned int getLevel();
    unsigned int getUpgradeCost(unsigned int nextLvl = 0);
    bool upgrade(Game* game);
};

bool operator>(PowerPlayerData& data, int );
