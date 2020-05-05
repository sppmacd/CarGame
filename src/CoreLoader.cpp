#include "CoreLoader.hpp"

#include "maptype.h"

#include "PowerBall.hpp"
#include "PowerDamageDecrease.hpp"
#include "PowerFence.hpp"
#include "PowerFreeze.hpp"
#include "PowerOil.hpp"
#include "PowerPointBoost.hpp"
#include "PowerSpeedIncrease.hpp"

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

LevelData::MapType CoreLoader::MapIds::COUNTRYSIDE;
LevelData::MapType CoreLoader::MapIds::DESERT;
LevelData::MapType CoreLoader::MapIds::FOREST;
LevelData::MapType CoreLoader::MapIds::ICE;
LevelData::MapType CoreLoader::MapIds::MOUNTAINS;
LevelData::MapType CoreLoader::MapIds::SEA;
LevelData::MapType CoreLoader::MapIds::BEACH;
LevelData::MapType CoreLoader::MapIds::SWAMPLAND;
LevelData::MapType CoreLoader::MapIds::CITY;
LevelData::MapType CoreLoader::MapIds::MOTORWAY;
LevelData::MapType CoreLoader::MapIds::FOREST_TRAILS;
LevelData::MapType CoreLoader::MapIds::COUNT;

namespace CoreLoader
{
    void registerLevels(Game* game)
    {
        Maps::countryside = &(new LevelData(MapIds::COUNTRYSIDE))->setAcceleration(5.f).setColor(sf::Color(82, 133, 75)).setTextureName("countryside").setCarCreationSpeed(70);
        Maps::desert = &(new LevelData(MapIds::DESERT))->setAcceleration(4.4f).setColor(sf::Color(211, 210, 144)).setTextureName("desert").setCarCreationSpeed(80).setCost(400);
        Maps::forest = &(new LevelData(MapIds::FOREST))->setAcceleration(6.f).setColor(sf::Color(52, 82, 44)).setTextureName("forest").setCarCreationSpeed(60).setCost(1600);
        Maps::ice = &(new LevelData(MapIds::ICE))->setAcceleration(7.f).setColor(sf::Color(230, 230, 230)).setTextureName("ice").setCarCreationSpeed(90).setCost(4000);
        Maps::mountains = &(new LevelData(MapIds::MOUNTAINS))->setAcceleration(4.f).setColor(sf::Color(85, 86, 85)).setTextureName("mountains").setCarCreationSpeed(85).setCost(16000);
        Maps::beach = &(new LevelData(MapIds::BEACH))->setAcceleration(8.f).setColor(sf::Color(0, 143, 236)).setTextureName("beach").setCarCreationSpeed(50).setCost(24000);
        Maps::city = &(new LevelData(MapIds::CITY))->setAcceleration(6.f).setColor(sf::Color(105, 105, 105)).setTextureName("city").setCarCreationSpeed(30).setCost(40000);
        Maps::motorway = &(new LevelData(MapIds::MOTORWAY))->setAcceleration(10.f).setColor(sf::Color(26, 26, 26)).setTextureName("motorway").setCarCreationSpeed(100).setCost(60000);
        Maps::sea = &(new LevelData(MapIds::SEA))->setAcceleration(4.f).setColor(sf::Color(44, 48, 112)).setTextureName("sea").setCarCreationSpeed(50).setCost(80000);
        Maps::swampland = &(new LevelData(MapIds::SWAMPLAND))->setAcceleration(5.f).setColor(sf::Color(32, 48, 28)).setTextureName("swampland").setCarCreationSpeed(80).setCost(120000);
        Maps::forest_trails = &(new LevelData(MapIds::FOREST_TRAILS))->setAcceleration(7.f).setColor(sf::Color(33, 94, 18)).setTextureName("forest_trails").setCarCreationSpeed(30).setCost(200000);

        LevelData::registerLevel("countryside", *Maps::countryside);
        LevelData::registerLevel("desert", *Maps::desert);
        LevelData::registerLevel("forest", *Maps::forest);
        LevelData::registerLevel("ice", *Maps::ice);
        LevelData::registerLevel("mountains", *Maps::mountains);
        LevelData::registerLevel("beach", *Maps::beach);
        LevelData::registerLevel("city", *Maps::city);
        LevelData::registerLevel("motorway", *Maps::motorway);
        LevelData::registerLevel("sea", *Maps::sea);
        LevelData::registerLevel("swampland", *Maps::swampland);
        LevelData::registerLevel("forest_trails", *Maps::forest_trails);
    }
    void registerCars(Game* game)
    {
        game->gpo.carTypes.add((new CarType(Cars::NORMAL, "default"))->setRarities(      {2,2,2,2,2,2,2,2,2,2,2}             )->setMaxHealth(1));
        game->gpo.carTypes.add((new CarType(Cars::LORRY, "lorry"))->setRarities(         {3,3,3,15,13,5,2,5,10,3,15}         )->setMaxHealth(3));
        game->gpo.carTypes.add((new CarType(Cars::RARE, "default"))->setRarities(        {5,3,3,4,7,8,10,5,5,3,5}            )->setMaxHealth(2));
        game->gpo.carTypes.add((new CarType(Cars::BUS, "bus"))->setRarities(             {10,3,14,8,3,3,5,3,5,8,3}           )->setMaxHealth(5));
        game->gpo.carTypes.add((new CarType(Cars::AMBULANCE, "ambulance"))->setRarities( {10,3,10,15,20,5,3,10,3,3,6}        )->setMaxHealth(4));
        game->gpo.carTypes.add((new CarType(Cars::RALLY, "rally"))->setRarities(         {4,20,5,25,10,20,10,20,20,5,20}     )->setMaxHealth(2));
        game->gpo.carTypes.add((new CarType(Cars::BOMB, "bomb"))->setRarities(           {15,15,15,15,15,15,15,15,15,15,15}  )->setMaxHealth(1));
        game->gpo.carTypes.add((new CarType(Cars::FIREMAN, "fireman"))->setRarities(     {5,7,3,10,8,12,4,10,5,5,12}         )->setMaxHealth(5));
        game->gpo.carTypes.add((new CarType(Cars::TANK, "tank"))->setRarities(           {10,7,15,20,25,10,20,20,25,30,10}   )->setMaxHealth(10));
        game->gpo.carTypes.add((new CarType(Cars::OLD, "old"))->setRarities(             {5,20,8,10,15,5,15,20,15,15,5}      )->setMaxHealth(2));
        game->gpo.carTypes.add((new CarType(Cars::ARMORED, "lorry"))->setRarities(       {20,14,30,40,50,15,40,10,12,15,5}   )->setMaxHealth(15));
        game->gpo.carTypes.add((new CarType(Cars::TRAIN, "train"))->setRarities(         {20,50,5,50,25,10,5,30,10,10,20}    )->setMaxHealth(10));
    }
    void registerPowers(Game* game)
    {
        // move to core DLL
        game->gpo.registerPower(game, 1, &(new PowerOil)->setMaxTime(1800));
        game->gpo.registerPower(game, 2, &(new PowerFreeze)->setMaxTime(3000));
        game->gpo.registerPower(game, 3, new PowerPointBoost);
        game->gpo.registerPower(game, 4, new PowerFence);
        game->gpo.registerPower(game, 5, new PowerBall);
        // PowerRegisterEvent

        game->gpo.registerPower(game, 101, new PowerSpeedIncrease);
        game->gpo.registerPower(game, 102, new PowerDamageDecrease);
        // AntipowerRegisterEvent
    }
    void registerEventHandlers(Game* game)
    {
        game->eventHandlerInst.registerGameEvent(GameEvent::CarSpawning, CoreEventHandlers::onCarSpawning);
        game->eventHandlerInst.registerGameEvent(GameEvent::CarDamaged, CoreEventHandlers::onCarDamaged);
        game->eventHandlerInst.registerGameEvent(GameEvent::LevelLoadingStart, CoreEventHandlers::onLevelLoadingStart);
    }
    void registerAbilities(PlayerAbilityManager* ab)
    {
        ab->registerAbility(Abilities::POWER_COOLDOWN_TIME, new AbilityBaseLinear(2000, 0.3, 1.0, "power_cooldown_time"));
        ab->registerAbility(Abilities::POWER_TIME, new AbilityBaseLinear(4000, 0.25, 1.0, "power_time"));
        ab->registerAbility(Abilities::DAMAGE, new AbilityBaseLinear(2500, 0.25, 1.0, "damage"));
    }
}

// Called when module is initialized in Game.
CGAPI void cgGameInit(Game* game)
{
    CoreLoader::registerEventHandlers(game);

    CoreLoader::registerLevels(game);
    CoreLoader::registerCars(game);
    CoreLoader::registerPowers(game);
    CoreLoader::registerAbilities(&game->playerData.abilities);
}

// Called when module is added to Game, before anything starts.
CGAPI void cgLoad(GameLoader* loader)
{
    // nothing
}
