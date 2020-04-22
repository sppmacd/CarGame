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
    cars.push_back(car);
}

void Game::updateCars()
{
    lastTickScore = score;

    if(!cars.empty())
    {
        for(unsigned int i = 0; i < cars.size(); i++)
        {
            Car* car = cars[i];
            car->move(gameSpeed / 0.176);
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

                    if(playerData.isNewPlayer)
                    {
                        if(car->typeId == Car::BOMB && playerData.tutorialStep == TUT_AVOIDBOMB)
                        {
                            playerData.tutorialStep = TUT_SHOP;
                        }
                    }

                    continue;
                }

                if(abs(car->getScreenPos().x - GameDisplay::instance->mousePos().x) < 100.f && abs(car->getScreenPos().y - GameDisplay::instance->mousePos().y) < 40.f && wasReleased)
                {
                    GameEvent event;
                    event.type = GameEvent::CarDamaged;
                    event.car.car = car;

                    if(runGameEventHandler(event))
                    {
                        car->onDamage(this);
                        if(playerData.tutorialStep == TUT_DESTROYCAR)
                        {
                            playerData.tutorialStep = TUT_DONTLEAVE;
                        }
                        else if(playerData.tutorialStep == TUT_DONTLEAVE)
                        {
                            playerData.tutorialStep = TUT_AVOIDBOMB;
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
                cars.erase(cars.begin() + i);
            }
        }
        wasReleased = false;
    }
}

void Game::checkSpawnCar()
{
    // [[      time from last spawn     ]]                            [[         game speed multiplier         ]]   [[ is first tick  ]]
    if(tickCount - lastCarTime > carCreatingSpeed / (gameSpeed / initialGameSpeed) || tickCount <= 1)
    {
        lastCarTime = tickCount;
		vector<CarType*> selectedTypes;
		while(selectedTypes.empty())
		for(CarType& type : carTypeRegistry)
		{
		    if(type.getRarity(level.getMapType()) == 0)
                continue;
			if(rand() % type.getRarity(level.getMapType()) == 0)
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
    checkSpawnCar();
    updateCars();

    if(score > playerData.highScore)
    {
		if (!newRecord)
		{
			newRecord = true; // New record!
			//Create splash screen

			if(playerData.highScore != 0)
				GameDisplay::instance->setSplash(translation.get("splash.newrecord"));
		}

		//Update highscore to score
        playerData.highScore = score;
    }

    updateEffect();

    if(playerData.pointsToNewMpl <= 0)
    {
        sound.playSound("point_mpl", 100.f);
        playerData.coinMpl++;
        playerData.pointsToNewMpl = playerData.coinMpl * 200;
    }

    if(gameOver && !paused())
    {
        pause(true);
    }
}

void Game::newTick()
{
    sound.update();

    if(!paused())
    {
        gameSpeed += level.getAcceleration() / 10000;
        ++tickCount;
    }

    if(paused() && unpauseDelay > Time::Zero)
    {
        unpauseDelay -= fpsTimer.getElapsedTime();
        if(unpauseDelay <= sf::Time::Zero)
        {
            pause(false);
            return;
        }
        if(unpauseSplashTime.getElapsedTime() >= seconds(0.99f))
        {
            // display counter: 3... 2... 1...
            GameDisplay::instance->setSplash(to_string(int(Math::round(unpauseDelay.asSeconds()))) + "...");
            unpauseSplashTime.restart();
        }
    }

    mainTickCount++;
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
    powerTime = powerHandle->maxPowerTime * playerData.abilities.calculateValue(PlayerAbilityManager::POWER_TIME);
    powerMaxTime = powerTime;
    powerCooldown = -1;

    powerHandle->setLevel(Power::getCurrentPowerLevel());
    // power 'start'
    if (!powerHandle->onPowerStart())
    {
        // don't allow create power if power don't want this
        // play some sound
        powerTime = 0;
        powerCooldown = 0;
        //powers[getCurrentPower()]++; //Reset power count to previous
    }
}
void Game::stopCurrentPower()
{
    if(powerHandle->cooldownTime <= 1)
        powerCooldown = 1;
    else
        powerCooldown = powerHandle->cooldownTime / playerData.abilities.calculateValue(PlayerAbilityManager::POWER_COOLDOWN_TIME); // 30 seconds
    powerTime = -1; //0 - can use power, -1 - cooldown, >0 - power is used, 1 - set cooldown!

    // power 'stop'
    powerHandle->onPowerStop();
}
void Game::updateEffect()
{
    if(isPowerUsed && powerCooldown <= 0 && getCurrentPower() != -1)
    {
        auto it = powerRegistry.find(getCurrentPower());
        if(it == powerRegistry.end())
            return;

        usePower(getCurrentPower());
        setCurrentPower(it->second);
    }

    if(powerHandle)
    {
        if(powerTime > 0)
        {
            powerTime--;

            // power 'tick'
            powerHandle->onPowerTick(powerTime);

            if(powerTime == 1)
            {
                stopCurrentPower();
            }
        }

        else if(powerTime == -1)
        {
            // power 'cooldown tick'
            powerHandle->onCooldownTick(powerCooldown);
            powerCooldown--;
        }

        if(powerCooldown == 1) // 1-set CUP, >1-cooldown!, -1-power is using!, 0-can use power
        {
            // power 'cooldown stop'
            powerHandle->onCooldownStop();
            powerTime = 0;
            powerCooldown = 0;
            powerHandle = NULL;
        }
    }
    isPowerUsed = false;
}
