#include "Car.h"
#include <iostream>
#include "GameDisplay.h"
#include "Game.h"
#include "CarType.h"
#include "Level.h"

Car::Car(Car::TypeId id, float speed, int line)
    : typeId(id)
    , carSpeed(speed)
    , lineIn(line)
{
    this->pos = 1080.f;
    this->destroyTick = -1.f;
    this->canErase = false;
	this->animSize = 1;
	this->frameLength = 60;

	// Create car type for car
	this->type = Game::instance->findCarTypeByID(id);
	this->maxHealth = this->type->getMaxHealth();
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
    this->health -= count;

    if(this->health <= 0)
    {
        this->health = 0.f;
        if(this->destroyTick == -1)
        {
            this->destroyTick = 20;
            this->carSpeed /= 2.5;
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
    makeDestroy();

    if(typeId != Car::BOMB)
    {
        if(typeId == Car::RARE)
        {
            game->addScore(2);
        }
        else
        {
            game->addScore(1);
        }

        if(game->getScore() % 2 == 0)
        {
            game->addCoins(game->getCoinMultiplier());
        }
    }
    if(health <= 0.f)
    {
        onDestroy(game);
    }
}

void Car::onDestroy(Game*)
{

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
