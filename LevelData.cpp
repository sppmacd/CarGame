#include "LevelData.h"
#include "maptype.h"
#include "Game.h"

LevelData* Maps::countryside;
LevelData* Maps::desert;
LevelData* Maps::forest;
LevelData* Maps::ice;
LevelData* Maps::mountains;
LevelData* Maps::beach;

float LevelData::getAcceleration()
{
    return this->acc;
}

int LevelData::getCarCreationSpeed()
{
	return carCreationSpd == 0 ? 70 : carCreationSpd;
}

sf::Color LevelData::getColor()
{
    return this->mapColor;
}

LevelData::MapType LevelData::getMapType()
{
    return this->mapType;
}

sf::String LevelData::getTextureName()
{
    return this->textureName;
}

int LevelData::getCost()
{
    return this->mapCost;
}

LevelData::LevelData(MapType type)
{
    this->mapType = type;
    setCost(0);
}

void LevelData::init()
{
    Maps::countryside = &(new LevelData(COUNTRYSIDE))->setAcceleration(5.f).setColor(sf::Color(82, 133, 75)).setTextureName("countryside").setCarCreationSpeed(70);
	Maps::desert = &(new LevelData(LevelData::DESERT))->setAcceleration(4.4f).setColor(sf::Color(255, 255, 0)).setTextureName("desert").setCarCreationSpeed(80).setCost(100);
    Maps::forest = &(new LevelData(LevelData::FOREST))->setAcceleration(6.f).setColor(sf::Color(0, 140, 0)).setTextureName("forest").setCarCreationSpeed(60).setCost(400);
    Maps::ice = &(new LevelData(LevelData::ICE))->setAcceleration(7.f).setColor(sf::Color(230, 230, 230)).setTextureName("ice").setCarCreationSpeed(90).setCost(1000);
    Maps::mountains = &(new LevelData(LevelData::MOUNTAINS))->setAcceleration(4.f).setColor(sf::Color(85, 86, 85)).setTextureName("mountains").setCarCreationSpeed(85).setCost(4000);
    Maps::beach = &(new LevelData(LevelData::BEACH))->setAcceleration(8.f).setColor(sf::Color(0, 143, 236)).setTextureName("desert").setCarCreationSpeed(50).setCost(10000);

	Game::instance->levelRegistry.insert(make_pair("Countryside", Maps::countryside));
	Game::instance->levelRegistry.insert(make_pair("Desert", Maps::desert));
	Game::instance->levelRegistry.insert(make_pair("Forest", Maps::forest));
	Game::instance->levelRegistry.insert(make_pair("Ice", Maps::ice));
	Game::instance->levelRegistry.insert(make_pair("Mountains", Maps::mountains));
	Game::instance->levelRegistry.insert(make_pair("Beach", Maps::beach));
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
