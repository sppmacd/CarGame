#include "PowerPlayerData.hpp"
#include "Game.h"

PowerPlayerData::PowerPlayerData(Power* power, unsigned int levelIn) : level(levelIn), powerInst(power) {}

unsigned int PowerPlayerData::getLevel()
{
    return level;
}
unsigned int PowerPlayerData::getUpgradeCost(unsigned int nextLvl)
{
    if(nextLvl == 0)
        return powerInst->getCost() * (level + 1) * (level + 1);
    else
        return powerInst->getCost() * nextLvl * nextLvl;
}
bool PowerPlayerData::upgrade(Game* game)
{
    int cash = game->getCoins() - getUpgradeCost();
    if(cash >= 0)
    {
        game->removeCoins(getUpgradeCost());
        level++;
        return true;
    }
    return false;
}
