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
    , lineIn(line)
{
    speed += Game::instance->getGameSpeed();
    initialCarSpeed = speed;
    carSpeed = speed * 1.5f;
    carAcceleration = 0.f;
    this->pos = 1130.f;
    this->destroyTick = -1.f;
    this->canErase = false;
	this->animSize = 1;
	this->frameLength = 60;
	friction = 5.f;
	DebugLogger::logDbg("INIT: car " + std::to_string((size_t)this) + ": v=" + std::to_string(carSpeed) + ", f=" + std::to_string(friction) + ", v_i=" + std::to_string(initialCarSpeed), "Car");

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
    float newPos = pos - carSpeed / 60.f;
    if(!isDestroying())
    {
        Car* collider = NULL;
        for(Car* car: Game::instance->cars)
        {
            if(isCrashedWith(car) && car != this && !car->isDestroying())
            {
                collider = car;
                break;
            }
        }
        if(collider)
        {
            makeDestroy(abs(carSpeed - collider->carSpeed) / 200.f);
            collider->makeDestroy(abs(collider->carSpeed - carSpeed) / 200.f);
            float tmp = carSpeed;
            carSpeed = collider->carSpeed;
            collider->carSpeed = tmp;
        }
        else
        {
            pos = newPos;
        }
    }
    else
    {
        pos = newPos;
    }
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
    return car->getLine() == this->getLine() && abs(car->getPos() - this->getPos()) < 100.f;
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
    updateAIAndPhysics();
}

void Car::onLeave(Game* game)
{
    game->setGameOver();
}

void Car::updateAIAndPhysics()
{
    //DebugLogger::logDbg("0: car " + std::to_string((size_t)this) + ": a=" + std::to_string(carAcceleration) + ", v=" + std::to_string(carSpeed) + ", s=" + std::to_string(pos) + ", F=" + std::to_string(friction), "Car");
    carAcceleration = 0;

    // todo: check collisions (brake on car in front of)

    /////////////
    // PHYSICS //
    /////////////

    // friction
    carAcceleration -= carSpeed == 0.f ? 0.f : ((friction) * (carSpeed < 0 ? -1 : 1));

    //////////
    // "AI" //
    //////////

    tickAI();

    // Physics ...
    // acceleration
    carSpeed += carAcceleration / 60.f;

    // round speed to 0 if very small
    if(abs(carSpeed) < 0.001f)
        carSpeed = 0.f;

    //DebugLogger::logDbg("1: car " + std::to_string((size_t)this) + ": a=" + std::to_string(carAcceleration) + ", v=" + std::to_string(carSpeed) + ", s=" + std::to_string(pos) + ", F=" + std::to_string(friction), "Car");
}

void Car::tickAI()
{
    // keep speed
    carAcceleration += max(50.f, (initialCarSpeed - carSpeed) * 4.f);
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
