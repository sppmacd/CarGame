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
    Maps::countryside = new LevelData(COUNTRYSIDE);
    Maps::countryside->setAcceleration(4.f);
    Maps::countryside->setColor(sf::Color(82, 133, 75));
    Maps::countryside->setTextureName("countryside");
	Maps::countryside->setCarCreationSpeed(70);

	Maps::desert = new LevelData(LevelData::DESERT);
    Maps::desert->setAcceleration(3.4f);
    Maps::desert->setColor(sf::Color::Yellow);
    Maps::desert->setTextureName("desert");
	Maps::desert->setCarCreationSpeed(80);

    Maps::forest = new LevelData(LevelData::FOREST);
    Maps::forest->setAcceleration(5.f);
    Maps::forest->setColor(sf::Color(0, 140, 0));
    Maps::forest->setTextureName("forest");
	Maps::forest->setCarCreationSpeed(60);

    Maps::ice = new LevelData(LevelData::ICE);
    Maps::ice->setAcceleration(6.f);
    Maps::ice->setColor(sf::Color(230, 230, 230));
    Maps::ice->setTextureName("ice");
	Maps::ice->setCarCreationSpeed(90);

    Maps::mountains = new LevelData(LevelData::MOUNTAINS);
    Maps::mountains->setAcceleration(3.f);
    Maps::mountains->setColor(sf::Color(85, 86, 85));
    Maps::mountains->setTextureName("mountains");
	Maps::mountains->setCarCreationSpeed(85);

	Game::instance->levelRegistry.insert(make_pair("Countryside", Maps::countryside));
	Game::instance->levelRegistry.insert(make_pair("Desert", Maps::desert));
	Game::instance->levelRegistry.insert(make_pair("Forest", Maps::forest));
	Game::instance->levelRegistry.insert(make_pair("Ice", Maps::ice));
	Game::instance->levelRegistry.insert(make_pair("Mountains", Maps::mountains));
}

void LevelData::setAcceleration(float f)
{
    this->acc = f;
}

void LevelData::setCarCreationSpeed(int ccs)
{
	this->carCreationSpd = ccs;
}

void LevelData::setColor(sf::Color c)
{
    this->mapColor = c;
}

void LevelData::setTextureName(sf::String name)
{
    this->textureName = name;
}
