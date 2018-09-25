#include "Game.h"
#include "CarLorry.h"
#include "CarRare.h"
#include <cmath>
#include <iostream>
#include <fstream>
#include "GameDisplay.h"

#include "Gui.h"
#include "GuiGameOver.h"
#include "GuiIngame.h"
#include "GuiMainMenu.h"
#include "GuiSettings.h"
#include "GuiMapSelect.h"
#include "GuiPowers.h"
#include "GuiYesNo.h"

#include "EventHandler.h"
#include "GameEvent.h"

Game* Game::instance;

Game::Game()
    : displayedGui(-1)
{
    cout << "Game: Started loading game engine..." << endl;

	GameDisplay::loadingStr = "Loading game data...";

	if (!instance)
	{
		instance = this; //Set main game instance to this

		// Perform first initializations 
		this->mainTickCount = 0; //Reset ticking
		this->running = true; //Set game running	
		this->pause(true); //Pause game (to not spawn cars!)
		this->debug = false; //Disable debug mode
		this->fullscreen = false;
		this->registerEventHandlers();

		// Reset player stats
		this->isNewPlayer = false;
		this->tutorialStep = 0;
		this->powers = new int[3];

		// Reset level and car data
		LevelData::init();
		CarType::init();

		// Reset powers
		this->powerCooldown = 0;
		this->powerTime = 0;
		this->isPowerUsed = false;
		this->registerPowers();

		// Load player data
		this->loadPlayerData();

		// Register GUI data
		this->registerGUIs();
	}
	else //fatal error
	{
		instance->exit(-1);
	}
}

void Game::wheelEvent(sf::Event::MouseWheelScrollEvent event)
{
    int powercount = 2;

    if(!this->isGuiLoaded && this->powerTime <= 0)
    {
        if(event.delta > 0.f)
        {
            this->currentPower++;
            if(this->currentPower > powercount) this->currentPower = 1;
        }
        else if(event.delta < 0.f)
        {
            --this->currentPower;
            if(this->currentPower < 1) this->currentPower = powercount;
        }
    }
}

void Game::getPower(int id)
{
    this->powers[id]++;
}

void Game::usePower(int id)
{
    if(this->powers[id] > 0)
        this->powers[id]--;
}

void Game::registerGUIs()
{
	Gui::registerGuiHandlers(0, GuiData{GuiIngame::draw, GuiIngame::onMouseClick, GuiIngame::onMouseMove, GuiIngame::onButtonClicked, GuiIngame::onLoad, GuiIngame::onClose, GuiIngame::onDialogFinished});
	Gui::registerGuiHandlers(1, GuiData{GuiGameOver::draw, GuiGameOver::onMouseClick, GuiGameOver::onMouseMove, GuiGameOver::onButtonClicked, GuiGameOver::onLoad, GuiGameOver::onClose, GuiGameOver::onDialogFinished });
	Gui::registerGuiHandlers(2, GuiData{GuiMainMenu::draw, GuiMainMenu::onMouseClick, GuiMainMenu::onMouseMove, GuiMainMenu::onButtonClicked, GuiMainMenu::onLoad, GuiMainMenu::onClose, GuiMainMenu::onDialogFinished });
	Gui::registerGuiHandlers(3, GuiData{GuiSettings::draw, GuiSettings::onMouseClick, GuiSettings::onMouseMove, GuiSettings::onButtonClicked, GuiSettings::onLoad, GuiSettings::onClose, GuiSettings::onDialogFinished });
	Gui::registerGuiHandlers(4, GuiData{GuiMapSelect::draw, GuiMapSelect::onMouseClick, GuiMapSelect::onMouseMove, GuiMapSelect::onButtonClicked, GuiMapSelect::onLoad, GuiMapSelect::onClose, GuiMapSelect::onDialogFinished });
	Gui::registerGuiHandlers(5, GuiData{GuiPowers::draw, GuiPowers::onMouseClick, GuiPowers::onMouseMove, GuiPowers::onButtonClicked, GuiPowers::onLoad, GuiPowers::onClose, GuiPowers::onDialogFinished });
	
	//dialogs
	Gui::registerGuiHandlers(100, GuiData{GuiYesNo::draw, GuiYesNo::onMouseClick, GuiYesNo::onMouseMove, GuiYesNo::onButtonClicked, GuiYesNo::onLoad, GuiYesNo::onClose, GuiYesNo::onDialogFinished });
}

void Game::registerPowers() 
{
	/*
	DrawPowerFunc drawPower = Power::drawPower;
	PowerStartHandler onPowerStart = Power::onPowerStart;
	PowerStopHandler onPowerStop = Power::onPowerStop;
	PowerTickHandler onPowerTick = Power::onPowerTick;
	DrawPowerFunc drawPowerIdle = Power::drawPowerIdle;
	CooldownTickHandler onCooldownTick = Power::onCooldownTick;
	CooldownStopHandler onCooldownStop = Power::onCooldownStop;
	*/
	powerRegistry.insert(make_pair(0, PowerHandles{}));
	powerRegistry.insert(make_pair(1, PowerHandles{ PowerOil::drawPower, PowerOil::onPowerStart, PowerOil::onPowerStop, PowerOil::onPowerTick, PowerOil::drawPowerIdle }));
	powerRegistry.insert(make_pair(2, PowerHandles{ PowerFreeze::drawPower, PowerFreeze::onPowerStart, PowerFreeze::onPowerStop }));
}

void Game::registerCarType(CarType type)
{
	carTypeRegistry.push_back(type);
}

CarType* Game::findCarTypeByID(Car::TypeId id)
{
	for (CarType& type : carTypeRegistry)
	{
		if (type == id)
			return &type;
	}
	return nullptr;
}

float Game::getGameSpeed()
{
	return this->gameSpeed;
}

void Game::setGameSpeed(float speed)
{
	this->gameSpeed = speed;
}

void Game::runEventHandler(Event& event)
{
	int counter = 0;
	for (pair<const Event::EventType, EventHandler>& pair : eventHandlers)
	{
		if (pair.first == event.type)
		{
			counter++;
			bool stat = pair.second(event, this);
			if (!stat)
				cout << "Event canceling is not implemented in CG 0.1 MPI!" << endl;
		}
	}

	if(counter < 1)
		cout << "Event Handler not found for event " << event.type << endl;
}

bool Game::runGameEventHandler(GameEvent & event)
{
	int counter = 0;
	bool stat = true;
	for (pair<const GameEvent::Type, GameEventHandler>& pair : eventHandler.registry)
	{
		if (pair.first == event.type)
		{
			counter++;
			stat &= pair.second(event, this);
		}
	}

	if (counter < 1)
		cout << "Game Event Handler not found for event " << event.type << endl;

	return stat;
}

void Game::registerEventHandlers()
{
	addEventHandler(Event::Closed, EventHandlers::onClose);
	addEventHandler(Event::MouseButtonReleased, EventHandlers::onMouseButtonReleased);
	addEventHandler(Event::KeyPressed, EventHandlers::onKeyPressed);
	addEventHandler(Event::MouseWheelScrolled, EventHandlers::onMouseWheelScrolled);
	eventHandler.registerGameEvent(GameEvent::CarSpawning, EventHandlers::onCarSpawning);
}

void Game::addEventHandler(Event::EventType type, EventHandler handler)
{
	eventHandlers.insert(make_pair(type, handler));
}

int  Game::getCurrentPower()
{
    return this->currentPower;
}

void Game::addScore(int s)
{
    this->score += s;
    this->totalPlayerPoints += s;
    this->pointsToNewMpl -= s;
}

long Game::getTotalPoints()
{
    return this->totalPlayerPoints;
}

int Game::getCoinMultiplier()
{
    return this->coinMpl;
}

void Game::loadPlayerData()
{
	GameDisplay::loadingStr = "Loading player data...";

    cout << "Game: Loading player data..." << endl;

    std::ifstream file;
    file.open("data.txt");

    if(file.good())
    {
        cout << "Game: Player data read from data.txt" << endl;

        file
        >> this->highScore
        >> this->playerCoins
        >> this->unlockedLevels
        >> this->totalPlayerPoints
        >> this->coinMpl
        >> this->pointsToNewMpl
        >> this->powers[1]
        >> this->powers[2];
    }
    else
    {
        file.open("highscore.txt");

        if(file.good())
        {
            cout << "Game: Couldn't load data.txt! Converting from old player data format..." << endl;
            file
            >> this->highScore
            >> this->playerCoins
            >> this->unlockedLevels
            >> this->totalPlayerPoints
            >> this->coinMpl
            >> this->pointsToNewMpl
            >> this->powers[1]
            >> this->powers[2];
        }
        else
        {
            cout << "Game: Cannot load player data! Creating new profile. You are the new player :)" << endl;
            this->highScore = 0;
            this->playerCoins = 0;
            this->unlockedLevels = 0b0;
            this->totalPlayerPoints = 0;
            this->coinMpl = 1;
            this->pointsToNewMpl = 200;
            this->isNewPlayer = true;
            this->tutorialStep = 2;
            this->powers = new int[3];
            this->powers[1] = 0;
            this->powers[2] = 0;
        }
    }
}

void Game::savePlayerData()
{
    cout << "Game: Saving player data in data.txt..." << endl;

    GameDisplay::drawLoadingProgress("Saving player data...", GameDisplay::instance->getRenderWnd());

    std::ofstream file;
    file.open("data.txt");

    file << this->highScore << " "
    << this->getCoins() << " "
    << this->unlockedLevels << " "
    << this->totalPlayerPoints << " "
    << this->coinMpl << " "
    << this->pointsToNewMpl << " "
    << this->powers[1] << " "
    << this->powers[2] << " ";
}

void Game::loadGame(LevelData level)
{
    GameDisplay::drawLoadingProgress("Loading level " + level.getMapType(), GameDisplay::instance->getRenderWnd());

    cout << "Game: Loading level " << level.getMapType() << "..." << endl;

    this->level = level;

    this->lastTickScore = 0;
    this->cameraPos = 0;
    this->tickCount = 0;
    this->gameSpeed = level.getAcceleration() / 2.2;
    this->score = 0;
    this->gameOver = false;
    this->cars.clear();
    this->pause(false);
    this->closeGui();
    this->currentPower = 0;
	this->carCreatingSpeed = level.getCarCreationSpeed();
	this->newRecord = false; //Set new record to false
}

void Game::loadGame()
{
    GameDisplay::drawLoadingProgress("Reloading current level...", GameDisplay::instance->getRenderWnd());

    cout << "Game: Reloading current level..." << endl;

    this->lastTickScore = 0;
    this->cameraPos = 0;
    this->tickCount = 0;
    this->gameSpeed = this->level.getAcceleration() / 2.2;
    this->score = 0;
    this->gameOver = false;
    this->isGuiLoaded = false;
    this->cars.clear();
    this->pause(false);
    this->closeGui();
    this->currentPower = 0;
	this->carCreatingSpeed = this->level.getCarCreationSpeed();
	this->newRecord = false; //Set new record to false
}

void Game::closeLevel()
{
    cout << "Game: Closing level..." << endl;
    GameDisplay::drawLoadingProgress("Closing level...", GameDisplay::instance->getRenderWnd());
    if(!this->cars.empty()) this->cars.clear();
    this->tickCount = 0;
    this->pause(true);
}

Game::~Game()
{
    cout << "Game: Deleting game engine instance..." << endl;
    this->closeLevel();
	delete[] this->powers;
	
	for (Car& car : cars)
	{
		delete &car;
	}
}

void Game::tickGui(sf::Event& event)
{
	if (event.type == sf::Event::MouseMoved)
	{
		sf::Vector2i mousepos(event.mouseMove.x, event.mouseMove.y);
		this->tickEventMouseMove(GameDisplay::instance->getRenderWnd()->mapPixelToCoords(mousepos));
	}

    if(event.type == sf::Event::MouseButtonReleased)
    {
		sf::Vector2i mousepos(event.mouseButton.x, event.mouseButton.y);
        this->tickEventMouseClick(GameDisplay::instance->getRenderWnd()->mapPixelToCoords(mousepos));
    }
}

void Game::tickEventMouseMove(sf::Vector2f pos)
{
    if(this->isGuiLoaded)
    {
		const GuiData& handler = Gui::findHandlerByID(this->displayedGui);
		handler.onMouseMove(pos);
    }
}

void Game::tickEventMouseClick(sf::Vector2f pos)
{
    if(this->isGuiLoaded)
    {
		const GuiData& handler = Gui::findHandlerByID(this->displayedGui);
		int buttonId = handler.onMouseClick(pos).id;
		handler.onButtonClick(buttonId);
    }
}

void Game::setGameOver()
{
    cout << "Game: Setting game over..." << endl;
    this->gameOver = true;
    this->savePlayerData();
    this->displayGui(1);
}

void Game::pause(bool s)
{
	this->gamePaused = s;
	if(GameDisplay::instance)
		GameDisplay::instance->getRenderWnd()->setKeyRepeatEnabled(s);
}

void Game::moveCamera()
{
    this->cameraPos += this->gameSpeed * 4.5;
}

void Game::displayGui(int gui)
{
	this->closeGui(); //Close previous GUI
	
	GuiData handler = Gui::findHandlerByID(gui);
	handler.load();
	
	this->displayedGui = gui;
    this->isGuiLoaded = true;
    this->guiCooldown = 0;
}

void Game::closeGui()
{
	if (this->isGuiLoaded)
	{
		Gui::onClose();
		GuiData handler = Gui::findHandlerByID(this->displayedGui);
		handler.close();

		this->displayedGui = -1;
		this->isGuiLoaded = false;
	}
}

void Game::toggleFullscreen()
{
    sf::RenderWindow* wnd = GameDisplay::instance->getRenderWnd();

    if(this->fullscreen)
    {
        wnd->create(sf::VideoMode(1280, 720, 32), "Car Game");
        this->fullscreen = false;
    }
    else
    {
		wnd->create(sf::VideoMode::getFullscreenModes()[0], sf::String(), sf::Style::Fullscreen);
        this->fullscreen = true;
    }
	GameDisplay::instance->setWndSize(wnd->getSize());
}

void Game::addCoins(long v)
{
    this->playerCoins += v;
}

void Game::removeCoins(long v)
{
    this->playerCoins -= v;
}

long Game::getCoins()
{
    return this->playerCoins;
}

bool Game::isLevelUnlocked(LevelData::MapType type)
{
    return (this->unlockedLevels >> type) & 0x1;
}

bool Game::isRunning()
{
    return this->running;
}

void Game::exit(int ret)
{
    cout << "Game: Handling exit()..." << endl;
    this->running = false;
    this->retVal = ret;
}
