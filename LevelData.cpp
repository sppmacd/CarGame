#include "LevelData.h"
#include "maptype.h"
#include "Game.h"

LevelData* Maps::countryside;
LevelData* Maps::desert;
LevelData* Maps::forest;
LevelData* Maps::ice;
LevelData* Maps::mountains;

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

LevelData::LevelData(MapType type)
{
    this->mapType = type;
}

void LevelData::init()
{
    Maps::countryside = &(new LevelData(COUNTRYSIDE))->setAcceleration(4.f).setColor(sf::Color(82, 133, 75)).setTextureName("countryside").setCarCreationSpeed(70);
	Maps::desert = &(new LevelData(LevelData::DESERT))->setAcceleration(3.4f).setColor(sf::Color::Yellow).setTextureName("desert").setCarCreationSpeed(80);
    Maps::forest = &(new LevelData(LevelData::FOREST))->setAcceleration(5.f).setColor(sf::Color(0, 140, 0)).setTextureName("forest").setCarCreationSpeed(60);
    Maps::ice = &(new LevelData(LevelData::ICE))->setAcceleration(6.f).setColor(sf::Color(230, 230, 230)).setTextureName("ice").setCarCreationSpeed(90);
    Maps::mountains = &(new LevelData(LevelData::MOUNTAINS))->setAcceleration(3.f).setColor(sf::Color(85, 86, 85)).setTextureName("mountains").setCarCreationSpeed(85);

	Game::instance->levelRegistry.insert(make_pair("Countryside", Maps::countryside));
	Game::instance->levelRegistry.insert(make_pair("Desert", Maps::desert));
	Game::instance->levelRegistry.insert(make_pair("Forest", Maps::forest));
	Game::instance->levelRegistry.insert(make_pair("Ice", Maps::ice));
	Game::instance->levelRegistry.insert(make_pair("Mountains", Maps::mountains));
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
