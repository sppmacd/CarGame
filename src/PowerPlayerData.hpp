#pragma once

// TODO 0.2

class PowerPlayerData
{
    unsigned int count;
    unsigned int level;
public:
    unsigned int getCount();
    unsigned int getLevel();
    unsigned int getUpgradeCost();
    unsigned int getBuyCost();
    bool buy();
    bool upgrade();
};
