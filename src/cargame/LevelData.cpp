#include "LevelData.h"
#include "Game.h"
#include "ModuleManager.hpp"

// LevelData
LevelData::LevelData()
    : GameplayObject(ModuleManager::instance->current())
{
    setCost(0);
}

float LevelData::getAcceleration() const
{
    return acc;
}

int LevelData::getCarCreationSpeed() const
{
	return carCreationSpd == 0 ? 70 : carCreationSpd;
}

sf::Color LevelData::getColor() const
{
    return mapColor;
}

const LevelData::MapType& LevelData::getMapType() const
{
    return mapType;
}

sf::String LevelData::getTextureName() const
{
    return textureName;
}

int LevelData::getCost() const
{
    return mapCost;
}

float LevelData::getPointMultiplier() const
{
    return pointMultiplier;
}

void LevelData::registerLevel(const std::string& strId, LevelData& data)
{
    std::string strIdNamespaced = data.getNamespacedId(strId);
    DebugLogger::logDbg("Adding level: " + strIdNamespaced, "LevelData");
    int i = Game::instance->gpo.levels.add(strIdNamespaced, &data);

    if(i > 0)
    {
        data.setPointMultiplier(i);
        data.mapType = strIdNamespaced;
        return;
    }

    if(i == REG_ERROR_EXISTS)
        DebugLogger::log("A level with id '" + strIdNamespaced + "' already exists!", "LevelData", "ERROR");
}

LevelData& LevelData::setAcceleration(float f)
{
    acc = f;
    return *this;
}

LevelData& LevelData::setCarCreationSpeed(int ccs)
{
	carCreationSpd = ccs;
	return *this;
}

LevelData& LevelData::setColor(sf::Color c)
{
    mapColor = c;
    return *this;
}

LevelData& LevelData::setTextureName(sf::String name)
{
    textureName = name;
    return *this;
}

LevelData& LevelData::setCost(int cost)
{
    mapCost = cost;
    return *this;
}

LevelData& LevelData::setPointMultiplier(float ptMpl)
{
    pointMultiplier = ptMpl;
    return *this;
}
