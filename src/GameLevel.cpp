#include "Game.h"
#include "CarLorry.h"
#include "CarRare.h"
#include "CarBus.h"
#include "CarAmbulance.h"
#include "GameSound.hpp"
#include "GameDisplay.h"
#include "Math.hpp"
#include "DebugLogger.hpp"

void Game::addCar(Car* car)
{
    this->cars.push_back(car);
}

void Game::updateCars()
{
    this->lastTickScore = this->score;

    if(!this->cars.empty())
    {
        for(unsigned int i = 0; i < this->cars.size(); i++)
        {
            Car* car = this->cars[i];
            car->move(this->gameSpeed / 0.176);
			car->onUpdate(this);

            if(car->isDestroying())
            {
                if(car->tickDestroy())
                {
                    car->setToErase();
                }
            }
            else
            {
                if(car->pos < -50.f)
                {
                    GameEvent event;
                    event.type = GameEvent::CarLeaved;
                    event.car.car = car;

                    if(runGameEventHandler(event))
                    {
                        car->onLeave(this);
                    }
                    car->setToErase();

                    if(isNewPlayer)
                    {
                        if(car->typeId == Car::BOMB && this->tutorialStep == TUT_AVOIDBOMB)
                        {
                            this->tutorialStep = TUT_SHOP;
                        }
                    }

                    continue;
                }

                if(abs(car->getScreenPos().x - GameDisplay::instance->mousePos().x) < 100.f && abs(car->getScreenPos().y - GameDisplay::instance->mousePos().y) < 40.f && this->wasReleased)
                {
                    GameEvent event;
                    event.type = GameEvent::CarDamaged;
                    event.car.car = car;

                    if(runGameEventHandler(event))
                    {
                        car->onDamage(this);
                        if(this->tutorialStep == TUT_DESTROYCAR)
                        {
                            this->tutorialStep = TUT_DONTLEAVE;
                        }
                        else if(this->tutorialStep == TUT_DONTLEAVE)
                        {
                            this->tutorialStep = TUT_AVOIDBOMB;
                        }
                    }

                    continue;
                }
            }

            if(car->canErase)
            {
                GameEvent event;
                event.type = GameEvent::CarDeleted;
                event.car.car = car;
                if(!runGameEventHandler(event))
                {
                    car->canErase = false;
                    continue;
                }
                this->cars.erase(this->cars.begin() + i);
            }
        }
        this->wasReleased = false;
    }
}

void Game::checkSpawnCar()
{
    // [[      time from last spawn     ]]                            [[         game speed multiplier         ]]   [[ is first tick  ]]
    if(this->tickCount - this->lastCarTime > this->carCreatingSpeed / (this->gameSpeed / this->initialGameSpeed) || this->tickCount <= 1)
    {
        lastCarTime = tickCount;
		vector<CarType*> selectedTypes;
		while(selectedTypes.empty())
		for(CarType& type : carTypeRegistry)
		{
		    if(type.getRarity(this->level.getMapType()) == 0)
                continue;
			if(rand() % type.getRarity(this->level.getMapType()) == 0)
				selectedTypes.push_back(&type);
		}

		CarType* carType = selectedTypes[rand() % selectedTypes.size()];

		// Create event
		GameEvent event;
		event.type = GameEvent::CarSpawning;
		event.carSpawned.carToCreate = NULL;
		event.carSpawned.type = carType;
		bool createCar = runGameEventHandler(event);

		if(createCar && event.carSpawned.carToCreate)
		{
			event.type = GameEvent::CarSpawned;
			event.car.car = event.carSpawned.carToCreate;
			if(runGameEventHandler(event))
            {
                addCar(event.carSpawned.carToCreate);
                event.carSpawned.carToCreate->onCreate(this);
            }
		}
    }
}

void Game::tickNormalGame()
{
    this->checkSpawnCar();
    this->updateCars();

    if(this->score > this->highScore)
    {
		if (!this->newRecord)
		{
			this->newRecord = true; // New record!
			//Create splash screen

			if(this->highScore != 0)
				GameDisplay::instance->setSplash(translation.get("splash.newrecord"));
		}

		//Update highscore to score
        this->highScore = this->score;
    }

    this->updateEffect();

    if(this->pointsToNewMpl <= 0)
    {
        this->sound.playSound("point_mpl", 100.f);
        this->coinMpl++;
        this->pointsToNewMpl = this->getCoinMultiplier() * 200;
    }

    if(this->gameOver && !this->paused())
    {
        this->pause(true);
    }
}

void Game::newTick()
{
    sound.update();

    if(!this->paused())
    {
        this->gameSpeed += this->level.getAcceleration() / 10000;
        ++tickCount;
    }

    if(this->paused() && this->unpauseDelay > Time::Zero)
    {
        this->unpauseDelay -= this->fpsTimer.getElapsedTime();
        if(this->unpauseDelay <= sf::Time::Zero)
        {
            this->pause(false);
            return;
        }
        if(this->unpauseSplashTime.getElapsedTime() >= seconds(0.99f))
        {
            // display counter: 3... 2... 1...
            GameDisplay::instance->setSplash(to_string(int(Math::round(this->unpauseDelay.asSeconds()))) + "...");
            this->unpauseSplashTime.restart();
        }
    }

    this->mainTickCount++;
}
void Game::setCurrentPower(Power* power)
{
    // completely disable previous power if exists
    if(powerHandle)
    {
        // power 'stop'
        powerHandle->onPowerStop();
        // power 'cooldown stop'
        powerHandle->onCooldownStop();
        powerHandle = NULL;
    }

    powerHandle = power;
    this->powerTime = powerHandle->maxPowerTime * this->abilities.calculateValue(PlayerAbilityManager::POWER_TIME);
    this->powerMaxTime = this->powerTime;
    this->powerCooldown = -1;

    powerHandle->setLevel(Power::getCurrentPowerLevel());
    // power 'start'
    if (!powerHandle->onPowerStart())
    {
        // don't allow create power if power don't want this
        // play some sound
        this->powerTime = 0;
        this->powerCooldown = 0;
        //this->powers[this->getCurrentPower()]++; //Reset power count to previous
    }
}
void Game::stopCurrentPower()
{
    if(this->powerHandle->cooldownTime <= 1)
        this->powerCooldown = 1;
    else
        this->powerCooldown = this->powerHandle->cooldownTime / this->abilities.calculateValue(PlayerAbilityManager::POWER_COOLDOWN_TIME); // 30 seconds
    this->powerTime = -1; //0 - can use power, -1 - cooldown, >0 - power is used, 1 - set cooldown!

    // power 'stop'
    powerHandle->onPowerStop();
}
void Game::updateEffect()
{
    if(this->isPowerUsed && this->powerCooldown <= 0 && this->getCurrentPower() != -1)
    {
        auto it = this->powerRegistry.find(this->getCurrentPower());
        if(it == this->powerRegistry.end())
            return;

        this->usePower(this->getCurrentPower());
        this->setCurrentPower(it->second);
    }

    if(powerHandle)
    {
        if(this->powerTime > 0)
        {
            this->powerTime--;

            // power 'tick'
            powerHandle->onPowerTick(this->powerTime);

            if(this->powerTime == 1)
            {
                this->stopCurrentPower();
            }
        }

        else if(this->powerTime == -1)
        {
            // power 'cooldown tick'
            powerHandle->onCooldownTick(this->powerCooldown);
            this->powerCooldown--;
        }

        if(this->powerCooldown == 1) // 1-set CUP, >1-cooldown!, -1-power is using!, 0-can use power
        {
            // power 'cooldown stop'
            powerHandle->onCooldownStop();
            this->powerTime = 0;
            this->powerCooldown = 0;
            powerHandle = NULL;
        }
    }
    this->isPowerUsed = false;
}
