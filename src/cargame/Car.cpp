#include "Car.h"
#include <iostream>
#include "GameDisplay.h"
#include "Game.h"
#include "CarType.h"
#include "Level.h"
#include "DebugLogger.hpp"

// CAR FATAL ERRORS
// C01 - CarType not found

Car::Car(Car::TypeId id, float speed, int line)
    : typeId(id)
    , carSpeed(speed)
    , lineIn(line)
{
    this->pos = 1130.f;
    this->destroyTick = -1.f;
    this->canErase = false;
	this->animSize = 1;
	this->frameLength = 60;

	// Find car type for car
	this->type = Game::instance->gpo.carTypes.findById(id);
	this->typeId = id;

	// It's critical error; stop the game.
	if(!type)
    {
        DebugLogger::log("Couldn't find CarType for ID '" + id.toString() + "'", "Car", "ERROR");
        DebugLogger::log("Probably the mod is broken", "Car", "ERROR");
        DebugLogger::log("Try reinstalling the game", "Car", "ERROR");
        Game::instance->displayError("CarType not found.\nSee log for details.", "C01");
        return;
    }
	this->setMaxHealth(this->type->getMaxHealth());
	this->textureName = this->type->getTextureName();
	this->health = this->maxHealth;
	this->colorMultiplier = Color::White;
}

bool Car::tickDestroy()
{
    this->destroyTick -= 0.3;
    if(this->destroyTick < 0.0)
    {
        return true;
    }
    else
    {
        this->colorMultiplier += sf::Color(3,0,0);
        return false;
    }
}

float Car::getSpeed()
{
    return this->carSpeed;
}

int Car::getLine()
{
    return this->lineIn;
}

void Car::setColor(sf::Color color)
{
    this->colorMultiplier = color;
}

void Car::move(float gameSpeed)
{
    //this->pos -= this->carSpeed;
    this->pos -= this->carSpeed / 6.f;
    this->pos -= gameSpeed / 6.f;
}

sf::Color Car::getColor()
{
    return this->colorMultiplier;
}

void Car::makeDestroy(float count)
{
    GameEvent event;
    event.type = GameEvent::DamageDealenCar;
    event.carDamage.car = this;
    event.carDamage.damage = &count;
    bool dmg = Game::instance->runGameEventHandler(event);
    if(!dmg)
        return;

    this->health -= count;
    Game::instance->sound.playSound("damage", 100.f);

    if(this->health <= 0)
    {
        this->health = 0.f;

        if(this->destroyTick == -1)
        {
            GameEvent event;
            event.type = GameEvent::CarDestroyed;
            event.car.car = this;
            if(!Game::instance->runGameEventHandler(event)) return;

            Game::instance->sound.playSound("destroy", 50.f);
            this->destroyTick = 20;
            this->carSpeed /= 2.5;

            onDestroy(Game::instance);
        }
    }
}

bool Car::isCrashedWith(Car* car)
{
    return car->getLine() == this->getLine() && abs(car->getPos() - this->getPos()) < 50.f;
}

void Car::onCreate(Game* )
{

}

void Car::onDamage(Game* game)
{
    game->addScore(1);
    makeDestroy(game->getDamageMultiplier());
}

void Car::onDestroy(Game* game)
{
    game->addCoins(game->playerData.coinMpl);
}

void Car::onUpdate(Game*)
{
    //...
}

void Car::onLeave(Game* game)
{
    game->setGameOver();
}

sf::Vector2f Car::getScreenPos()
{
    return sf::Vector2f(this->pos * 2, LevelUtility::getLanePos(this->getLine()).y/*+laneChangeTick*/);
}

string Car::getTextureName()
{
    return this->textureName;
}

void Car::setSpeed(float speed)
{
    this->carSpeed = speed;
}

void Car::setMaxHealth(float maxH)
{
    this->maxHealth = maxH;
    this->health = maxH;
}
