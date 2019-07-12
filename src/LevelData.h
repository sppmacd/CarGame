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
    class MapType
    {
        int mapId;
        static int nextId;
    public:
        MapType(int id = nextId++);
        operator int() const;
    };

    static MapType COUNTRYSIDE;
    static MapType DESERT;
    static MapType FOREST;
    static MapType ICE;
    static MapType MOUNTAINS;
    static MapType SEA;
    static MapType BEACH;
    static MapType SWAMPLAND;
    static MapType CITY;
    static MapType MOTORWAY;
    static MapType FOREST_TRAILS;
    static MapType COUNT;

    static void init();

    LevelData(const MapType& type);
    LevelData(): mapType(COUNT) {}
    const MapType& getMapType() const;

    LevelData& setColor(sf::Color c);
    LevelData& setTextureName(sf::String name);
    LevelData& setAcceleration(float f);
	LevelData& setCarCreationSpeed(int ccs);
	LevelData& setCost(int cost);

    Color getColor() const;
    String getTextureName() const;
    float getAcceleration() const;
	int getCarCreationSpeed() const;
	int getCost() const;

private:
    Color mapColor;
    String textureName;
    float acc;
    MapType mapType;
	int carCreationSpd;
	int mapCost;
};


#endif // LevelData_H
