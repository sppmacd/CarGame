#ifndef LevelData_H
#define LevelData_H

#include <array>
#include <SFML/Graphics.hpp>
#include "Car.h"
#include "GameplayObject.hpp"
#include "ModuleIdentifier.hpp"

using namespace std;
using namespace sf;

/// Class used with loading game, stores main level data.
class LevelData : public GameplayObject
{
public:
    typedef ModuleIdentifier MapType;

    // Register a new level.
    static void registerLevel(const std::string& strId, LevelData& data);

    // Creates a new instance of Map. The ID will be given by registerLevel() function. ^^
    LevelData();

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

	// ...
	LevelData& setPointMultiplier(float ptMpl);

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

	// ...
	float getPointMultiplier() const;

private:
    Color mapColor;
    String textureName;
    float acc;
    MapType mapType;
	int carCreationSpd;
	int mapCost;
	float pointMultiplier;
};


#endif // LevelData_H
