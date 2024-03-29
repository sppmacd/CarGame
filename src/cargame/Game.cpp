#include <cmath>
#include <iostream>
#include <fstream>

#include "ArgMap.hpp"

#include "Game.h"
#include "GameDisplay.h"
#include "GameLoader.hpp"

#include "EventHandler.h"
#include "GameEvent.h"
#include "ModuleIdentifier.hpp"

#include "DebugLogger.hpp"

#include "GuiGameOver.h"
#include "GuiLanguage.hpp"
#include "GuiSettings.h"
#include "GuiIngame.h"
#include "GuiMainMenu.h"
#include "GuiError.hpp"

#include <HackerMan/Util/Main.hpp>

// core handler
//#include <core/CoreLoader.hpp>

// error codes:
// G00 could not load default language
// [not used] G01 could not load language config
// [not used] G02 registering GUIs is deprecated
// G03 second Game instance
// G04 invalid profile

CGAPI Game* Game::instance = NULL;

Game::Game(ArgMap* argmap): GuiHandler(GameDisplay::instance->getRenderWnd(), GameDisplay::instance->getGuiFont())
{
    ModuleManager::instance->setCurrent("api");
    DebugLogger::log("Loading game engine...", "Game");

	GameDisplay::loadingStr = "Loading game data...";

	if (!instance)
	{
	    DebugLogger::logDbg("Performing basic setup", "Game");
		instance = this; //Set main game instance to this
		this->running = true; //Set game running

		// Perform first initializations
		// and check for updates
		DebugLogger::logDbg("Checking for updates", "Game");
		GameDisplay::loadingStr = "Checking for updates...";

		// Register settings
		DebugLogger::logDbg("Loading game API", "Game");
		GameDisplay::loadingStr = "Loading game engine...";

        tmpSession = argmap->a_tmp || !argmap->a_debug_options.empty();
		if(tmpSession)
        {
            DebugLogger::log("Enabled Temporary Session Mode", "Game");
        }

		this->registerSettings();

		// Reset game state
		this->mainTickCount = 0; //Reset ticking
		this->damageMultiplier = 0.f; //to not confuse user with strange values on F3 debug
		this->pause(true); //Pause game (to not spawn cars!)
		this->debug = stoi(settings.getSetting("debug", "global")) || argmap->a_debug; //Disable/enable debug mode
		this->fullscreen = true;
		cg::colors::bgColor = sf::Color(50, 40, 40);
		cg::colors::textSize = 28;
		DebugLogger::logDbg("Initializing player profile", "Game");
		playerData.isNewPlayer = false;
		playerData.tutorialStep = 0;
		DebugLogger::logDbg("Starting power setup", "Game");
        this->powerTime = 0;
        this->maxPowerTime = 0;
        this->powerCooldown = 0;
        this->maxPowerCooldown = 0;
		this->isPowerUsed = false;
		this->powerHandle = NULL;
		this->currentPower = 0;
		this->tickCount = 0;

		// Initialize registries
		DebugLogger::logDbg("Starting registry filling", "Game");
		registerPowers();

        if(!ModuleManager::instance->gameInitAllModules())
        {
            DebugLogger::log("Couldn't call gameInit for modules!", "Game", "ERROR");
            throw std::runtime_error("G05 - Couldn't initialize modules");
        }

		// Load player data
		this->loadPlayerData();

        // Load language configuration
        DebugLogger::logDbg("Loading language config", "Game");
		this->loadLanguages();

		this->applyDebugOptions(argmap->a_debug_options);
	}
	else //fatal error
	{
	    DebugLogger::log("Loading game engine...", "Game", "FATAL");
		instance->displayError("Duplicate Game instance.\nCheck your mod config.", "G03");
	}
}

void Game::wheelEvent(sf::Event::MouseWheelScrollEvent event)
{
    if(!this->isGuiLoaded() && this->powerTime <= 0 && !usablePowerIds.empty())
    {
        if(event.delta > 0.f)
        {
            this->currentPower++;
            if(this->currentPower == usablePowerIds.size())
                this->currentPower = 0;
        }
        else if(event.delta < 0.f)
        {
            --this->currentPower;
            if(this->currentPower == -1)
                this->currentPower = usablePowerIds.size() - 1;
        }
    }
}

bool Game::getPower(ModuleIdentifier id)
{
    return playerData.powerLevels[id]->upgrade(this);
}

bool Game::usePower(ModuleIdentifier id)
{
    return true;
}


void Game::registerPowers()
{
	//registerPower(0, (Power*)NULL);
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
	for(pair<const Event::EventType, CGEventHandler>& pair : eventHandlers) //must call all event handlers, not only first
	{
		if(pair.first == event.type)
		{
			counter++;
			bool stat = pair.second(event, this);
			if(!stat)
				DebugLogger::log("Cancelling system events not supported.", "Game", "WARN");
		}
	}

	//if(counter < 1)
		//cout << "Event Handler not found for event " << event.type << endl;
}

bool Game::runGameEventHandler(GameEvent& event)
{
    //DebugLogger::logDbg("Called event: " + to_string(int(event.type)), "Game");
	int counter = 0;
	bool stat = true;
	for(pair<const GameEvent::Type, GameEventHandler>& pair : eventHandlerInst.registry)
	{
		if(pair.first == event.type)
		{
			counter++;
			stat &= pair.second(event, this);
		}
	}
	/*if(!stat)
        DebugLogger::logDbg("Canceled event: " + to_string(int(event.type)), "Game");*/

	//if (counter < 1)
		//cout << "Game Event Handler not found for event " << event.type << endl;

	return stat;
}

void Game::registerEventHandlers()
{

}

void Game::addEventHandler(Event::EventType type, CGEventHandler handler)
{
	eventHandlers.insert(make_pair(type, handler));
}

ModuleIdentifier Game::getCurrentPower()
{
    return usablePowerIds.size() > 0 ? usablePowerIds[this->currentPower] : "api$no_power_equipped";
}

void Game::addScore(int s)
{
    if(s > 0)
    {
        s *= pointMultiplier;
        this->score += s;
        playerData.totalPlayerPoints += s;
        playerData.pointsToNewMpl -= s;
        sound.playSound("point_add", 75.f);
        GameDisplay::instance->resetPointAnim();
    }
}

void Game::loadPlayerData()
{
	GameDisplay::loadingStr = "Loading player data...";
    DebugLogger::log("Loading player data from default profile file", "Game");
    bool b = playerData.load("profile_1.txt");
    if(!b)
        initProfile();
}

void Game::initProfile()
{
    DebugLogger::log("Cannot load player data! Creating a new profile. You are the new player :)", "Game", "WARN");
    playerData.init();
}

void Game::savePlayerData()
{
    if(!tmpSession)
    {
        DebugLogger::log("Saving player data to default profile file", "Game");
        GameDisplay::drawLoadingProgress("Saving player data...", GameDisplay::instance->getRenderWnd());
        playerData.save("profile_1.txt");
    }
    else
    {
        DebugLogger::log("Temporary Session Mode is enabled, save aborted!", "Game", "WARN");
    }
}

void Game::loadGame(LevelData* level)
{
    GameDisplay::drawLoadingProgress("Loading level " + level->getMapType().toString(), GameDisplay::instance->getRenderWnd());

    DebugLogger::log("Loading level: " + level->getMapType().toString(), "Game");
    this->level = level;
    setupGame();
}

void Game::loadGame()
{
    GameDisplay::drawLoadingProgress("Reloading current level...", GameDisplay::instance->getRenderWnd());

    DebugLogger::log("Reloading level: " + level->getMapType().toString(), "Game");
    setupGame();

}
void Game::setupGame()
{
    this->sound.playSound("start", 100.f);

    GameEvent event;
    event.type = GameEvent::LevelLoadingStart;
    event.level.level = this->level;
    runGameEventHandler(event);

    this->gameSpeed = this->level->getCarCreationSpeed() * 2;
    this->initialGameSpeed = this->gameSpeed;
    this->lastTickScore = 0;
    this->lastCarTime = 1;
    this->tickCount = 1;
    this->score = 0;
    this->gameOver = false;
    this->cars.clear();
    //this->pause(false);
    //this->closeGui();
    this->currentPower = 0;
	this->carCreatingSpeed = this->level->getCarCreationSpeed();
	this->newRecord = false;
    this->powerTime = 0;
    this->maxPowerTime = 0;
	this->powerCooldown = 0;
	this->maxPowerCooldown = 0;

	// Initialize powers
	for(int i = 0; i < usablePowerIds.size(); i++)
    {
        Power* power = gpo.powers.findById(usablePowerIds[i]);
        if(power)
            power->onLevelLoad();
        else
        {
            DebugLogger::log("Invalid Power in usablePowerIds[" + std::to_string(i) + "]. Falling back to api$no_power_equipped", "Game", "WARN");
            usablePowerIds[i] = "api$no_power_equipped";
        }
    }

	this->setPointMultiplier((float(this->level->getPointMultiplier()) + 2.f) * 0.8f);

	this->unpauseGame(3.f);
}

void Game::closeLevel()
{
    DebugLogger::log("Closing level: " + level->getMapType().toString(), "Game");
    GameDisplay::drawLoadingProgress("Closing level...", GameDisplay::instance->getRenderWnd());
    if(!this->cars.empty())
    {
        for(Car* car: cars)
            delete car;
        this->cars.clear();
    }
    this->tickCount = 0;

    if(this->powerHandle)
    {
        // Reset powers.
        this->powerHandle->onPowerStop();
        this->powerHandle->onCooldownStop();

        // fix bug with powers not started if closed level during power time.
        powerHandle = NULL;
    }
    this->pause(true);
}

Game::~Game()
{
    DebugLogger::log("Deleting Game... (don't panic, it isn't removing the Game from disk !)", "Game");
}

void Game::setGameOver()
{
    DebugLogger::log("Setting game over", "Game");
    this->gameOver = true;
    this->savePlayerData();
    this->displayGui(new GuiGameOver);
    this->sound.playSound("game_over", 100.f);
    this->closeLevel();

    if(playerData.isNewPlayer)
    {
        if(playerData.tutorialStep >= TUT_AVOIDBOMB) //bomb or click car
        {
            playerData.tutorialStep = TUT_SHOP;
        }
        else
        {
            playerData.tutorialStep = TUT_DONTLEAVE;
        }
    }
}

void Game::pause(bool s)
{
	this->gamePaused = s;
	if(GameDisplay::instance)
		GameDisplay::instance->getRenderWnd()->setKeyRepeatEnabled(s);
}

void Game::toggleFullscreen()
{
    sf::RenderWindow* wnd = GameDisplay::instance->getRenderWnd();

    if(this->fullscreen)
    {
        wnd->create(sf::VideoMode(1280, 720, 32), "CG " + string(CG_VERSION));
        this->fullscreen = false;
    }
    else
    {
		GameDisplay::instance->createFullscreenWnd();
        this->fullscreen = true;
    }
    guiView = View(wnd->getDefaultView()); //bugfix
    GameDisplay::instance->setWndSize(wnd->getSize());

    if(getCurrentGUI())
        getCurrentGUI()->reloadOnResize(); //resize GUI and its dialogs to update widgets (+0.2)

	GameDisplay::instance->setVSync(GameDisplay::instance->getVSync()); // fix vsync bug on toggling fullscreen
}

void Game::addCoins(long v)
{
    playerData.playerCoins += v;
    sound.playSound("coin_add", 75.f);
}

void Game::removeCoins(long v)
{
    if(playerData.playerCoins - v >= 0)
        playerData.playerCoins -= v;
    sound.playSound("coin_remove", 75.f);
}

bool Game::isLevelUnlocked(LevelData::MapType type)
{
    return playerData.unlockedLevels[type];
}

bool Game::isRunning()
{
    return this->running;
}

void Game::exit(int ret)
{
    DebugLogger::log("Preparing to exit game with return status " + std::to_string(ret), "Game");
    this->running = false;
    this->retVal = ret;
    close(ret);
}

void Game::displayError(string text, string code)
{
    errStr = text;
    DebugLogger::logDbg("An error occurred: " + errStr, "Game", "FATAL");
    displayGui(new GuiError(   "AN ERROR OCCURRED\n"
                            "\n'"
                            + errStr + "'\n\n"
                            "See https://github.com/sppmacd/CarGame/docs/error_codes.md\n"
                            "Try checking log for details or start game in Debug Mode (--debug)\n"
                            "Error code: " + code + "\n"
                            ":: Press Esc to close game ::"
                            ));
    mainTickCount = 2; //to skip post-init
    error = true;
}

void Game::loadLanguages(bool useFile)
{
    GameDisplay::loadingStr = "Loading language...";
    DebugLogger::log("Loading language config", "Game");
    bool b = true;

    if(useFile)
        b = hmLangCfg.loadFromFile("lang.hmd");

    if(!b)
    {
        DebugLogger::log("Could not load translation config! Creating a new one...", "Game", "WARN");

        hmLangCfg.setKey("current","en_US","lang");
        if(!tmpSession)
            hmLangCfg.saveToFile("lang.hmd");

        // try to load newly created config
        b = hmLangCfg.loadFromFile("lang.hmd");
        if(!b)
        {
            displayError("Could not load language config", "G01");
        }
    }

    bool b2 = enUSTranslation.loadFromFile("en_US");
    if(!b2)
    {
        DebugLogger::log("Could not load default translation!", "Game", "ERROR");
        displayError("Could not load default translation", "G00");
        return;
    }

    // Load user-defined translation
    string code = hmLangCfg.getKey("current","lang","en_US");
    bool b1 = translation.loadFromFile(code);
    translation.setParent(&enUSTranslation);
    if(!b1)
    {
        DebugLogger::log("Could not load user-defined translation!", "Game", "WARN");
        currentLangCode = "en_US";
    }
    else
    {
        currentLangCode = code;
    }
}

float Game::getPointMultiplier()
{
    return pointMultiplier;
}
void Game::setPointMultiplier(float ptmpl)
{
    pointMultiplier = ptmpl;
}

class Triggers
{
public:
    static bool s_refreshres(string)
    {
        GameDisplay::instance->reload();
        Game::instance->sound.reload();
        return true;
    }
    static bool s_verticalsync(string val)
    {
        GameDisplay::instance->setVSync(stoi(val));
        return true;
    }
    static bool s_fullscreen(string val)
    {
        if(Game::instance->fullscreen != stoi(val))
            Game::instance->toggleFullscreen();
        //Game::instance->fullscreen = stoi(val);
        return true;
    }
    static bool s_resetgame(string)
    {
		remove("data.txt");
		remove("highscore.txt");
		remove("profile_1.txt");
		Game::instance->loadPlayerData();
		Game::instance->displayGui(new GuiMainMenu);
		return true;
    }
    static bool s_language(string)
    {
        DebugLogger::log("Opening language GUI", "Triggers");
        Game::instance->displayedGui->runDialog(new GuiLanguage, -1);
        return true;
    }
    static bool s_volume(string val)
    {
        Game::instance->sound.soundVolume = stof(val);
        return true;
    }
    static bool s_debug(string val)
    {
        Game::instance->debug = stoi(val);
        return true;
    }
	static bool s_resetsettings(string)
	{
		Game::instance->settings.resetSettings("settings.txt");
		GuiSettings* gs = dynamic_cast<GuiSettings*>(Game::instance->getCurrentGUI());
		if(gs) gs->reset();
		Game::instance->settings.addWidgetsToSettings(gs);
		return true;
	}
};

void Game::registerSettings()
{
    settings.registerSetting("refreshres", SettingsManager::TRIGGER, "graphics");
    settings.registerSetting("verticalsync", SettingsManager::BOOLEAN, "graphics", "1");
    settings.registerSetting("fullscreen", SettingsManager::BOOLEAN, "graphics", "1");
    settings.registerSetting("resetgame", SettingsManager::CONFIRM_TRIGGER, "game");
    settings.registerSetting("language", SettingsManager::TRIGGER, "global");
    settings.registerSetting("resetsettings", SettingsManager::CONFIRM_TRIGGER, "global");
    settings.registerSetting("debug", SettingsManager::BOOLEAN, "global", "0");
    settings.registerSetting("volume", SettingsManager::NUMERIC, "sound", "0.5");

    settings.registerTrigger("refreshres", Triggers::s_refreshres, "graphics");
    settings.registerTrigger("verticalsync", Triggers::s_verticalsync, "graphics");
    settings.registerTrigger("fullscreen", Triggers::s_fullscreen, "graphics");
    settings.registerTrigger("resetgame", Triggers::s_resetgame, "game");
    settings.registerTrigger("language", Triggers::s_language, "global");
    settings.registerTrigger("resetsettings", Triggers::s_resetsettings, "global");
    settings.registerTrigger("debug", Triggers::s_debug, "global");
    settings.registerTrigger("volume", Triggers::s_volume, "sound");

    settings.loadSettings("settings.txt");
}

void Game::openSettings()
{
    GuiSettings* settingsGui = settings.generateWidgets();

    if(instanceof_ptr(displayedGui, GuiMainMenu))
        settingsGui->returnGui = new GuiMainMenu;
    else if(instanceof_ptr(displayedGui, GuiIngame))
        settingsGui->returnGui = new GuiIngame;

    displayGui(settingsGui);
}

bool Game::isFullscreen()
{
    return fullscreen;
}

void Game::postInit()
{
    DebugLogger::logDbg("Performing post-init", "Game");
    /*else
    {
        GameDisplay::instance->getRenderWnd()->create(sf::VideoMode(1280, 720, 32), "CG " + string(CG_VERSION));
    }*/

    displayGui(new GuiMainMenu);
    settings.triggerAllClose();
    if(isFullscreen())
        GameDisplay::instance->createFullscreenWnd();

    //GameDisplay::instance->createFullscreenWnd();
}

bool Game::isPowerEquipped(ModuleIdentifier id)
{
    for(size_t s = 0; s < playerData.equipment.size(); s++)
    {
        if(playerData.equipment[s] == id)
        {
            return true;
        }
    }
    return false;
}

void Game::unpauseGame(float time)
{
    closeGui();
    unpauseDelay = sf::seconds(time);
    GameDisplay::instance->setSplash(to_string(int(unpauseDelay.asSeconds())) + "...");
    unpauseSplashTime.restart(); //reset splash to display counter properly
}

bool Game::canPowerBuyOrEquip()
{
    bool powerCanBuy = false;
    bool powerCanEquip = false;
    int unlockedPowers = 0;
    for(auto it: gpo.powers.arr())
    {
        PowerPlayerData* ppd = playerData.powerLevels[it.first.baseId];
        if(!ppd)
            return false;

        if(ppd->getLevel() > 0)
            unlockedPowers++;
        else if(ppd->getUpgradeCost() <= playerData.playerCoins)
            powerCanBuy = true;
    }
    if(usablePowerIds.size() < 2 && unlockedPowers > 1)
        powerCanEquip = true;
    return powerCanBuy || powerCanEquip;
}

void Game::setDamageMultiplier(float dmg)
{
    if(dmg < 0.f)
        dmg = 0.f; //don't allow negative
    damageMultiplier = dmg;
}

float Game::getDamageMultiplier()
{
    return damageMultiplier;
}

sf::View Game::getGUIView()
{
    return guiView;
}

sf::View Game::getGameView()
{
    // use real size of window, not view size
    float aspect = (float(getWindow()->getSize().x) / getWindow()->getSize().y) / (16.f / 9.f);
    float mapSizeX = 1920.f;
    float mapSizeY = 1080.f;

    float y = 0.5f-(0.5f * aspect);
    float x = 0.5f-(0.5f / aspect);

    sf::View view(sf::FloatRect(0.f, 0.f, mapSizeX, mapSizeY));

    if(aspect < 1)
    {
        view.setViewport(sf::FloatRect(0.f, y, 1.f, 1.f * aspect));
    }
    else
    {
        view.setViewport(sf::FloatRect(x, 0.f, 1.f / aspect, 1.f));
    }
    return view;
}

void Game::drawGui()
{
    guiView = getGameView();
    GuiHandler::drawGui(false);
}

void Game::applyDebugOptions(std::string options)
{
    if(!options.empty())
    {
        DebugLogger::log("Applying Debug Options", "Game", "IMP");
        sf::String optStr = options;
        optStr.replace(" ", "\n");

        HMDataMap hmMap;
        hmMap.loadFromString(optStr.toAnsiString());

        for(auto& pr: hmMap.getMap())
        {
            DebugLogger::logDbg("Trying to apply debug option: '" + pr.first.first + "':'" + pr.first.second + "'='" + pr.second + "'", "Game");
            std::string type = pr.first.first.substr(0, 2);
            if(type == "lg")
                hmLangCfg.setKey(pr.first.second, pr.second, pr.first.first.substr(3));
            else if(type == "pp")
                playerData.getHMMap().setKey(pr.first.second, pr.second, pr.first.first.substr(3));
            else if(type == "st")
                settings.setSetting(pr.first.second, pr.second, pr.first.first.substr(3));
            else
                DebugLogger::logDbg("Invalid debug option type: " + type, "Game", "WARN");
        }
        loadLanguages();
        playerData.load();

        HMDataMap soNS = hmMap.getAllByNamespace("so");
        if(soNS.getNumberKey("newPlayer", "so", 0))
            initProfile();
    }
}

bool Game::isTmpSession()
{
    return tmpSession;
}
