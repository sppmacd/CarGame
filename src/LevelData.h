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

    // Map type variables - ID is automatically generated.
    // todo: move to core dll
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

    // todo: move to core dll
    // Initializes default maps
    static void init();

    // Register a new level.
    static void registerLevel(std::string strId, LevelData& data);

    // Creates a new instance of Map with specified ID.
    LevelData(const MapType& type);

    // Creates an invalid map.
    LevelData(): mapType(COUNT) {}

    // Returns ID of this map.
    const MapType& getMapType() const;

    // Sets display color of map.
    LevelData& setColor(sf::Color c);

    // Sets map texture name (and unlocalized name - it's the same)
    LevelData& setTextureName(sf::String name);

    // Sets map acceleration (speed change per tick)
    LevelData& setAcceleration(float f);

    // todo: change to car distance
    // Sets map creation speed in ticks (car spawn delay in first tick)
	LevelData& setCarCreationSpeed(int ccs);

    // Set map cost.
	LevelData& setCost(int cost);

	// Returns display map color (background)
    Color getColor() const;

    // Returns texture name and unlocalized name of map.
    String getTextureName() const;

    // Returns map acceleration (speed change per tick)
    float getAcceleration() const;

    // Returns car creation speed in ticks (car spawn delay in first tick)
	int getCarCreationSpeed() const;

	// Returns map cost.
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
