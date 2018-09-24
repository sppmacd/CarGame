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
        COUNT
    };
    static void init();

    LevelData(MapType type);
    LevelData() {}
    MapType getMapType();

    void setColor(sf::Color c);
    void setTextureName(sf::String name);
    void setAcceleration(float f);
	void setCarCreationSpeed(int ccs);

    sf::Color getColor();
    sf::String getTextureName();
    float getAcceleration();
	int getCarCreationSpeed();

private:
    Color mapColor;
    String textureName;
    float acc;
    MapType mapType;
	int carCreationSpd;
};


#endif // LevelData_H
