#ifndef LevelData_H
#define LevelData_H

#include <array>
#include <SFML/Graphics.hpp>
#include "Car.h"

using namespace std;
using namespace sf;

/// Class used with loading game, stores main level data.
class LevelData
{
public:
    enum MapType
    {
        COUNTRYSIDE,
        DESERT,
        FOREST,
        ICE,
        MOUNTAINS,
        BEACH,
        CITY,
        COUNT
    };
    static void init();

    LevelData(MapType type);
    LevelData() {}
    MapType getMapType();

    LevelData& setColor(sf::Color c);
    LevelData& setTextureName(sf::String name);
    LevelData& setAcceleration(float f);
	LevelData& setCarCreationSpeed(int ccs);
	LevelData& setCost(int cost);

    Color getColor();
    String getTextureName();
    float getAcceleration();
	int getCarCreationSpeed();
	int getCost();

private:
    Color mapColor;
    String textureName;
    float acc;
    MapType mapType;
	int carCreationSpd;
	int mapCost;
};


#endif // LevelData_H
