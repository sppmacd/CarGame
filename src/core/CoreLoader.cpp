#include "CoreLoader.hpp"

#include "PowerBall.hpp"
#include "PowerDamageDecrease.hpp"
#include "PowerFence.hpp"
#include "PowerFreeze.hpp"
#include "PowerOil.hpp"
#include "PowerPointBoost.hpp"
#include "PowerSpeedIncrease.hpp"

namespace CoreLoader
{
    void registerLevels(Game* game)
    {
        LevelData::registerLevel("countryside", (new LevelData())->setAcceleration(5.f).setColor(sf::Color(82, 133, 75)).setTextureName("countryside").setCarCreationSpeed(70));
        LevelData::registerLevel("desert", (new LevelData())->setAcceleration(4.4f).setColor(sf::Color(211, 210, 144)).setTextureName("desert").setCarCreationSpeed(80).setCost(400));
        LevelData::registerLevel("forest", (new LevelData())->setAcceleration(6.f).setColor(sf::Color(52, 82, 44)).setTextureName("forest").setCarCreationSpeed(60).setCost(1600));
        LevelData::registerLevel("ice", (new LevelData())->setAcceleration(7.f).setColor(sf::Color(230, 230, 230)).setTextureName("ice").setCarCreationSpeed(90).setCost(4000));
        LevelData::registerLevel("mountains", (new LevelData())->setAcceleration(4.f).setColor(sf::Color(85, 86, 85)).setTextureName("mountains").setCarCreationSpeed(85).setCost(16000));
        LevelData::registerLevel("beach", (new LevelData())->setAcceleration(8.f).setColor(sf::Color(0, 143, 236)).setTextureName("beach").setCarCreationSpeed(50).setCost(24000));
        LevelData::registerLevel("city", (new LevelData())->setAcceleration(6.f).setColor(sf::Color(105, 105, 105)).setTextureName("city").setCarCreationSpeed(30).setCost(40000));
        LevelData::registerLevel("motorway", (new LevelData())->setAcceleration(10.f).setColor(sf::Color(26, 26, 26)).setTextureName("motorway").setCarCreationSpeed(100).setCost(60000));
        LevelData::registerLevel("sea", (new LevelData())->setAcceleration(4.f).setColor(sf::Color(44, 48, 112)).setTextureName("sea").setCarCreationSpeed(50).setCost(80000));
        LevelData::registerLevel("swampland", (new LevelData())->setAcceleration(5.f).setColor(sf::Color(32, 48, 28)).setTextureName("swampland").setCarCreationSpeed(80).setCost(120000));
        LevelData::registerLevel("forest_trails", (new LevelData())->setAcceleration(7.f).setColor(sf::Color(33, 94, 18)).setTextureName("forest_trails").setCarCreationSpeed(30).setCost(200000));

        LevelData::registerLevel("town", (new LevelData())->setAcceleration(9.f).setColor(sf::Color(145, 145, 145)).setTextureName("city").setCarCreationSpeed(30).setCost(500000));
        LevelData::registerLevel("space_station", (new LevelData())->setAcceleration(10.f).setColor(sf::Color(5, 5, 10)).setTextureName("sea").setCarCreationSpeed(40).setCost(1000000));
        LevelData::registerLevel("space", (new LevelData())->setAcceleration(12.f).setColor(sf::Color(0, 0, 5)).setTextureName("sea").setCarCreationSpeed(60).setCost(1500000));
    }
    void registerCars(Game* game)
    {
        game->gpo.registerCarType("default", (new CarType("default"))
                                  ->setDefaultRarity(2)
                                  ->setMaxHealth(1));
        game->gpo.registerCarType("lorry", (new CarType("lorry"))
                                  //->setRarities(         {4,4,4,15,13,5,2,5,10,4,15})
                                  ->setDefaultRarity(4)
                                  ->setRarityFor("ice", 15)
                                  ->setRarityFor("mountains", 13)
                                  ->setRarityFor("beach", 5)
                                  ->setRarityFor("city", 2)
                                  ->setMaxHealth(3));
        game->gpo.registerCarType("rare", (new CarType("default"))
                                  //->setRarities(        {5,3,3,4,7,8,10,5,5,3,5})
                                  ->setDefaultRarity(5)
                                  ->setRarityFor("desert", 3)
                                  ->setRarityFor("forest", 3)
                                  ->setRarityFor("mountains", 7)
                                  ->setRarityFor("beach", 8)
                                  ->setRarityFor("city", 10)
                                  ->setRarityFor("swampland", 3)
                                  ->setMaxHealth(2));
        game->gpo.registerCarType("bus", (new CarType("bus"))
                                  //->setRarities(             {10,3,14,8,3,3,5,3,5,8,3})
                                  ->setDefaultRarity(6)
                                  ->setRarityFor("countryside", 10)
                                  ->setRarityFor("forest", 14)
                                  ->setRarityFor("ice", 8)
                                  ->setRarityFor("city", 5)
                                  ->setRarityFor("sea", 10)
                                  ->setRarityFor("swampland", 8)
                                  ->setMaxHealth(5));
        game->gpo.registerCarType("ambulance", (new CarType("ambulance"))
                                  //->setRarities( {10,3,10,15,20,5,3,10,3,3,6})
                                  ->setDefaultRarity(4)
                                  ->setRarityFor("countryside", 10)
                                  ->setRarityFor("forest", 10)
                                  ->setRarityFor("ice", 15)
                                  ->setRarityFor("motorway", 10)
                                  ->setRarityFor("sea", 3)
                                  ->setMaxHealth(4));
        game->gpo.registerCarType("rally", (new CarType("rally"))
                                  //->setRarities(         {4,20,5,25,10,20,10,20,20,5,20})
                                  ->setDefaultRarity(20)
                                  ->setRarityFor("countryside", 4)
                                  ->setRarityFor("forest", 5)
                                  ->setRarityFor("ice", 25)
                                  ->setRarityFor("mountains", 10)
                                  ->setRarityFor("city", 10)
                                  ->setRarityFor("swampland", 5)
                                  ->setMaxHealth(2));
        game->gpo.registerCarType("bomb", (new CarType("bomb"))
                                  //->setRarities(           {15,15,15,15,15,15,15,15,15,15,15})
                                  ->setDefaultRarity(15)
                                  ->setMaxHealth(1));
        game->gpo.registerCarType("fireman", (new CarType("fireman"))
                                  //->setRarities(     {5,7,3,10,8,12,4,10,5,5,12})
                                  ->setDefaultRarity(12)
                                  ->setRarityFor("countryside", 5)
                                  ->setRarityFor("desert", 7)
                                  ->setRarityFor("forest", 3)
                                  ->setRarityFor("mountains", 8)
                                  ->setRarityFor("city", 4)
                                  ->setRarityFor("sea", 5)
                                  ->setRarityFor("swampland", 5)
                                  ->setMaxHealth(5));
        game->gpo.registerCarType("tank", (new CarType("tank"))
                                  //->setRarities(           {10,7,15,20,25,10,20,20,25,30,10})
                                  ->setDefaultRarity(20)
                                  ->setRarityFor("countryside", 10)
                                  ->setRarityFor("desert", 7)
                                  ->setRarityFor("forest", 15)
                                  ->setRarityFor("beach", 10)
                                  ->setRarityFor("sea", 25)
                                  ->setRarityFor("swampland", 30)
                                  ->setRarityFor("forest_trails", 10)
                                  ->setMaxHealth(10));
        game->gpo.registerCarType("old", (new CarType("old"))
                                  //->setRarities(             {5,20,8,10,15,5,15,20,15,15,5})
                                  ->setDefaultRarity(30)
                                  ->setRarityFor("countryside", 10)
                                  ->setRarityFor("desert", 20)
                                  ->setRarityFor("forest", 16)
                                  ->setRarityFor("ice", 10)
                                  ->setRarityFor("beach", 10)
                                  ->setRarityFor("motorway", 20)
                                  ->setRarityFor("forest_trails", 10)
                                  ->setMaxHealth(2));
        game->gpo.registerCarType("armored", (new CarType("lorry"))
                                  //->setRarities(       {20,14,30,40,50,15,40,10,12,15,5})
                                  ->setDefaultRarity(40)
                                  ->setRarityFor("countryside", 20)
                                  ->setRarityFor("desert", 14)
                                  ->setRarityFor("beach", 15)
                                  ->setRarityFor("motorway", 10)
                                  ->setRarityFor("sea", 12)
                                  ->setRarityFor("swampland", 15)
                                  ->setRarityFor("forest_trails", 5)
                                  ->setMaxHealth(15));
        game->gpo.registerCarType("train", (new CarType("train"))
                                  //->setRarities(         {20,50,5,50,25,10,5,30,10,10,20})
                                  ->setDefaultRarity(20)
                                  ->setRarityFor("desert", 50)
                                  ->setRarityFor("forest", 5)
                                  ->setRarityFor("ice", 50)
                                  ->setRarityFor("beach", 10)
                                  ->setRarityFor("city", 5)
                                  ->setRarityFor("sea", 10)
                                  ->setRarityFor("swampland", 10)
                                  ->setMaxHealth(10));

        game->gpo.registerCarType("rocket", (new CarType("old"))
                                  ->setDefaultRarity(0)
                                  ->setRarityFor("space_station", 7)
                                  ->setRarityFor("space", 3)
                                  ->setMaxHealth(7));

        game->gpo.registerCarType("coach", (new CarType("bus"))
                                  ->setDefaultRarity(20)
                                  ->setRarityFor("sea", 5)
                                  ->setRarityFor("beach", 3)
                                  ->setMaxHealth(5));
    }
    void registerPowers(Game* game)
    {
        // move to core DLL
        game->gpo.registerPower(game, "oil", &(new PowerOil())->setMaxTime(1800));
        game->gpo.registerPower(game, "freeze", &(new PowerFreeze())->setMaxTime(3000));
        game->gpo.registerPower(game, "point_boost", new PowerPointBoost());
        game->gpo.registerPower(game, "fence", new PowerFence());
        game->gpo.registerPower(game, "ball", new PowerBall());
        // PowerRegisterEvent

        game->gpo.registerPower(game, "speed_increase", new PowerSpeedIncrease());
        game->gpo.registerPower(game, "damage_decrease", new PowerDamageDecrease());
        // AntipowerRegisterEvent
    }
    void registerEventHandlers(Game* game)
    {
        game->eventHandlerInst.registerGameEvent(GameEvent::CarSpawning, CoreEventHandlers::onCarSpawning);
        game->eventHandlerInst.registerGameEvent(GameEvent::CarDamaged, CoreEventHandlers::onCarDamaged);
        game->eventHandlerInst.registerGameEvent(GameEvent::LevelLoadingStart, CoreEventHandlers::onLevelLoadingStart);
        game->eventHandlerInst.registerGameEvent(GameEvent::PowerStarted, CoreEventHandlers::onPowerStarted);
        game->eventHandlerInst.registerGameEvent(GameEvent::PowerStopped, CoreEventHandlers::onPowerStopped);
    }
    void registerAbilities(PlayerAbilityManager* ab)
    {
        ab->registerAbility("power_cooldown_time", new AbilityBaseLinear(2000, 0.3, 1.0, "power_cooldown_time"));
        ab->registerAbility("power_time", new AbilityBaseLinear(4000, 0.25, 1.0, "power_time"));
        ab->registerAbility("damage", new AbilityBaseLinear(2500, 0.25, 1.0, "damage"));
    }
}

extern "C"
{
    // Called when module is initialized in Game.
    CGAPI void cgGameInit()
    {
        Game* game = Game::instance;
        CoreLoader::registerEventHandlers(game);

        CoreLoader::registerLevels(game);
        CoreLoader::registerCars(game);
        CoreLoader::registerPowers(game);
        CoreLoader::registerAbilities(&game->playerData.abilities);
    }

    // Called when module is added to Game, before anything starts.
    CGAPI void cgLoad()
    {
        //cgApiInit(loader);
    }

    CGAPI void cgCleanup()
    {

    }
}
