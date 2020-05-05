#include "LevelData.h"
#include "Game.h"

int LevelData::MapType::nextId = 1;

LevelData::MapType::MapType(int id): mapId(id)
{

}
LevelData::MapType::operator int() const
{
    return mapId;
}

float LevelData::getAcceleration() const
{
    return this->acc;
}

int LevelData::getCarCreationSpeed() const
{
	return carCreationSpd == 0 ? 70 : carCreationSpd;
}

sf::Color LevelData::getColor() const
{
    return this->mapColor;
}

const LevelData::MapType& LevelData::getMapType() const
{
    return this->mapType;
}

sf::String LevelData::getTextureName() const
{
    return this->textureName;
}

int LevelData::getCost() const
{
    return this->mapCost;
}

LevelData::LevelData(const MapType& type): mapType(type)
{
    setCost(0);
}

void LevelData::registerLevel(std::string strId, LevelData& data)
{
    int i = Game::instance->gpo.levels.add(strId, &data);
    if(i == REG_ERROR_EXISTS)
        DebugLogger::log("A level with id '" + strId + "' already exists!", "LevelData", "ERROR");
}

LevelData& LevelData::setAcceleration(float f)
{
    this->acc = f;
    return *this;
}

LevelData& LevelData::setCarCreationSpeed(int ccs)
{
	this->carCreationSpd = ccs;
	return *this;
}

LevelData& LevelData::setColor(sf::Color c)
{
    this->mapColor = c;
    return *this;
}

LevelData& LevelData::setTextureName(sf::String name)
{
    this->textureName = name;
    return *this;
}

LevelData& LevelData::setCost(int cost)
{
    this->mapCost = cost;
    return *this;
}
