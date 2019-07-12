#include "LevelData.h"
#include "maptype.h"
#include "Game.h"

LevelData* Maps::countryside;
LevelData* Maps::desert;
LevelData* Maps::forest;
LevelData* Maps::ice;
LevelData* Maps::mountains;
LevelData* Maps::beach;
LevelData* Maps::city;
LevelData* Maps::sea;
LevelData* Maps::swampland;
LevelData* Maps::motorway;
LevelData* Maps::forest_trails;

int LevelData::MapType::nextId = 0;
LevelData::MapType LevelData::COUNTRYSIDE;
LevelData::MapType LevelData::DESERT;
LevelData::MapType LevelData::FOREST;
LevelData::MapType LevelData::ICE;
LevelData::MapType LevelData::MOUNTAINS;
LevelData::MapType LevelData::SEA;
LevelData::MapType LevelData::BEACH;
LevelData::MapType LevelData::SWAMPLAND;
LevelData::MapType LevelData::CITY;
LevelData::MapType LevelData::MOTORWAY;
LevelData::MapType LevelData::FOREST_TRAILS;
LevelData::MapType LevelData::COUNT;

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

void LevelData::init()
{
    Maps::countryside = &(new LevelData(COUNTRYSIDE))->setAcceleration(5.f).setColor(sf::Color(82, 133, 75)).setTextureName("countryside").setCarCreationSpeed(70);
	Maps::desert = &(new LevelData(LevelData::DESERT))->setAcceleration(4.4f).setColor(sf::Color(255, 255, 0)).setTextureName("desert").setCarCreationSpeed(80).setCost(400);
    Maps::forest = &(new LevelData(LevelData::FOREST))->setAcceleration(6.f).setColor(sf::Color(0, 140, 0)).setTextureName("forest").setCarCreationSpeed(60).setCost(1600);
    Maps::ice = &(new LevelData(LevelData::ICE))->setAcceleration(7.f).setColor(sf::Color(230, 230, 230)).setTextureName("ice").setCarCreationSpeed(90).setCost(4000);
    Maps::mountains = &(new LevelData(LevelData::MOUNTAINS))->setAcceleration(4.f).setColor(sf::Color(85, 86, 85)).setTextureName("mountains").setCarCreationSpeed(85).setCost(16000);
    Maps::beach = &(new LevelData(LevelData::BEACH))->setAcceleration(8.f).setColor(sf::Color(0, 143, 236)).setTextureName("beach").setCarCreationSpeed(50).setCost(24000);
    Maps::city = &(new LevelData(LevelData::CITY))->setAcceleration(6.f).setColor(sf::Color(105, 105, 105)).setTextureName("city").setCarCreationSpeed(30).setCost(40000);
    Maps::motorway = &(new LevelData(LevelData::MOTORWAY))->setAcceleration(10.f).setColor(sf::Color(26, 26, 26)).setTextureName("motorway").setCarCreationSpeed(100).setCost(60000);
    Maps::sea = &(new LevelData(LevelData::SEA))->setAcceleration(4.f).setColor(sf::Color(44, 48, 112)).setTextureName("sea").setCarCreationSpeed(50).setCost(80000);
    Maps::swampland = &(new LevelData(LevelData::SWAMPLAND))->setAcceleration(5.f).setColor(sf::Color(32, 48, 28)).setTextureName("swampland").setCarCreationSpeed(80).setCost(120000);
    Maps::forest_trails = &(new LevelData(LevelData::FOREST_TRAILS))->setAcceleration(7.f).setColor(sf::Color(33, 94, 18)).setTextureName("forest_trails").setCarCreationSpeed(30).setCost(200000);

	Game::instance->levelRegistry.push_back(make_pair("countryside", Maps::countryside));
	Game::instance->levelRegistry.push_back(make_pair("desert", Maps::desert));
	Game::instance->levelRegistry.push_back(make_pair("forest", Maps::forest));
	Game::instance->levelRegistry.push_back(make_pair("ice", Maps::ice));
	Game::instance->levelRegistry.push_back(make_pair("mountains", Maps::mountains));
	Game::instance->levelRegistry.push_back(make_pair("beach", Maps::beach));
	Game::instance->levelRegistry.push_back(make_pair("city", Maps::city));
	Game::instance->levelRegistry.push_back(make_pair("motorway", Maps::motorway));
	Game::instance->levelRegistry.push_back(make_pair("sea", Maps::sea));
	Game::instance->levelRegistry.push_back(make_pair("swampland", Maps::swampland));
	Game::instance->levelRegistry.push_back(make_pair("forest_trails", Maps::forest_trails));
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
