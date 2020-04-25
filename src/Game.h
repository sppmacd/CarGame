#pragma once

#include <vector>
#include <array>

#include "version.hpp"
#include "UpdateChecker.hpp"

#include "Car.h"
#include "CarType.h"

#include "LevelData.h"
#include "PlayerDataManager.hpp"
#include "GameplayObjectManager.hpp"

#include "Power.h"

#include "EventHandler.h"

#include "TranslationManager.hpp"
#include "GameSound.hpp"

#include "SettingsManager.hpp"
#include "ArgMap.hpp"

#include <HackerMan/Util/Main.hpp>
#include <CG/CG.h>

#define TUT_NONE 0 //- no tutorial
#define TUT_START 1 //- start game in main menu
#define TUT_BUYMAP 2 //- buy map
#define TUT_SELECTMAP 3 //- select map
#define TUT_DESTROYCAR 4 //- destroy car
#define TUT_DONTLEAVE 5 //- don't let car leave screen
#define TUT_AVOIDBOMB 6 //- avoid bomb
#define TUT_SHOP 7 //- shop

using namespace std;
using namespace sf;
using namespace cg;

class Game: public GuiHandler
{
public:
    struct Times
    {
        sf::Time timeEvent;
        sf::Time timeTick;
        sf::Time timeRender;
        sf::Time timeGui;
        sf::Time timeWait;
    };

	/////////////////////////
	/////// MAIN DATA ///////
	/////////////////////////

	// Instance of the game.
    static Game* instance;

	// Game tick count (since level loaded).
    int tickCount;

	// Main tick count (since the game started).
    long mainTickCount;

	// Variable storing MouseButtonReleased event status.
    bool wasReleased;

    // The settings manager.
	SettingsManager settings;

	// The update checker.
	UpdateChecker updateChecker;

	// True if update was found, false otherwise.
	bool updateFound;

	// GameSound (sound manager) instance.
	GameSound sound;

	// Power time.
    int powerTime;

	// Power cooldown.
    int powerCooldown;

    // Power max time.
    int powerMaxTime;

    // Current power.
    Power* powerHandle;

	// Current level data.
    LevelData level;

	// Current return value for game.
    int retVal;

	// Time values (debug!).
    Times times;

	// Is the power used? (Right click event status).
    bool isPowerUsed;

	// Variable storing car creating speed.
	int carCreatingSpeed;

	// Variable storing, if the new record is set (used in splash screen).
	bool newRecord;

	// Time that elapsed from last car spawn.
    int lastCarTime;

	// Variable storing language settings.
	TranslationManager translation;

	// Vector storing event handlers.
	multimap<sf::Event::EventType, CGEventHandler> eventHandlers;

	// Gameplay Object Manager - used to store objects like cars or maps.
    GameplayObjectManager gpo;

	/////////////////////////
    ////// PLAYER DATA //////
	/////////////////////////

    PlayerDataManager playerData;

    // Score in the last tick. Used by the game over GUI.
    long lastTickScore;

	/////////////////////////
	////// OTHER DATA ///////
	/////////////////////////

	// move to GameplayObjectManager
	// Registry of the maps, used by MapSelect GUI.

	// Tick time
    sf::Time tickTime;

	// Real tick time (with wait time)
    sf::Time realTickTime;

	// True if the game is in Debug mode (--debug or "Debug Mode" in Settings)
    bool debug;

    // move to GameplayObjectManager
    // Saved in byte format, unlocked levels for player
    long long unlockedLevels;

	// EventsHandler instance
	EventsHandler eventHandlerInst;

	// Error string
	string errStr;

    // Player powers, that can be currently used.
    vector<int> usablePowerIds;

    // Time left to unpause the game (after exiting In-game Menu)
    Time unpauseDelay;

    // Time elapsed from tick start.
    Clock fpsTimer;

    // Time elapsed from last unpause delay splash.
    Clock unpauseSplashTime;

    /////////////////////
	///// FUNCTIONS /////
	/////////////////////

	// Default constructor. Creates Game using startup arguments
	// saved in %argmap.
    Game(ArgMap* argmap);

	// Default destructor
    ~Game();

	// Vector storing cars that are currently on screen
    vector<Car*> cars;

	// Add car to game
    void addCar(Car* car);

	// Called once on LevelTick, updates cars
    void updateCars();

	// Updates power effect
    void updateEffect();

	// Loads specified level
    void loadGame(LevelData level);

	// Reloads current level (after game over)
    void loadGame();

	// Closes current level
    void closeLevel();

	// Ticking normal game (with cars)
    void tickNormalGame();

    // Loads player data from profile or sets to default values if error.
    void loadPlayerData(/*string profileName = "DEFAULT"*/);

    // Saves player data to file.
    void savePlayerData(/*string profileName = "DEFAULT"*/);

	// Start new tick, increment tickCount
    void newTick();

	// Checks if the game is over
    bool isGameOver() { return this->gameOver; }

	// Returns player score
    int getScore() { return this->score; }

	// Sets game to game over stat
    void setGameOver();

	// Pauses / unpauses game
	void pause(bool s);

	// Check if the game is already paused
    bool paused() { return this->gamePaused; }

	// Ticks gui. \param event Last event matching to GUI
    void tickGui(sf::Event& event);

	// Toggles fullscreen
    void toggleFullscreen();

    // Fill player data by default values.
    void initProfile();

	// Returns level color, used by renderer
    sf::Color getLevelColor() { return this->level.getColor(); }

	// Checks if the level is unlocked
    bool isLevelUnlocked(LevelData::MapType lvl);

	// Checks if the game is running
    bool isRunning();

	// Sets game state to not running and sets return value to specified value
    void exit(int ret);

	// Handles wheel event, used to change power
    void wheelEvent(sf::Event::MouseWheelScrollEvent event);

	// Get game speed value. Used externally by powers
	float getGameSpeed();

	// Set game speed value. Used externally by powers
	void setGameSpeed(float speed);

	// Runs EventHandler for event.
	void runEventHandler(Event& event);

	// Runs GameEventHandler for event.
	bool runGameEventHandler(GameEvent& event);

	// Registers event handlers. Called once on loading.
	void registerEventHandlers();

	// Add event handler.
	void addEventHandler(Event::EventType type, CGEventHandler handler);

	// Display error screen.
	void displayError(string text);

	// Loads language list.
	void loadLanguages();

	// Setup new game - set map, unpause game, initialize variables,
	// reinitialize powers
	void setupGame();

	// Set and initialize current power.
	void setCurrentPower(Power* power);

	// Stop current power and start cooldown.
	void stopCurrentPower();

	// Returns current point multiplier.
	float getPointMultiplier();

	// Sets current point multiplier.
	void setPointMultiplier(float ptmpl);

	// Called on load settings registration and load.
	void registerSettings();

	// Open settings GUI.
	void openSettings();

    // Check if game is currently in fullscreen mode
	bool isFullscreen();

	// Called in the first tick, after loading but before any action is performed.
	void postInit();

	// Unpauses game and set all splashes.
	void unpauseGame(float delayTime);

	// Check if car should be spawned and does that if so.
	void checkSpawnCar();

	// Sets damage multiplier. By default, it's set to ability.damage value
	void setDamageMultiplier(float dmg);

	// Returns current dmg multiplier.
	float getDamageMultiplier();

	// Returns current GUI (CGUI) View. Used by GameDisplay::mousePos().
	sf::View getGUIView();

    // Returns current game view.
    sf::View getGameView();

	////////////////////////////////////////////
	// Helper functions for PlayerDataManager //
	////////////////////////////////////////////

	// Adds specified (v) coins to player. Plays add sound.
    void addCoins(long v);

	// Removes specified (v) coins to player. Plays remove sound.
    void removeCoins(long v);

	// Adds score to player. Plays sound.
    void addScore(int s);

	// Returns current power ID
    int getCurrentPower();

	// Upgrades the specified power
    bool getPower(int id);

	// Use player power (--)
    bool usePower(int id);

    // Returns true if specified power is equipped (can use)
	bool isPowerEquipped(int id);

	// Returns true if any power can be upgraded or equipped. Used by GuiMapSelect
	// and GuiShop to know when to display blinking border.
	bool canPowerBuyOrEquip();

    // Register all powers
	void registerPowers();

	/////////////////////////////////
	/////////////////////////////////
	/////////////////////////////////

	// The biggest ID of player power (displayed in Powers GUI)
	int biggestPlayerPowerID; // player powers - 1-100 (0 = no power)

	// The biggest ID of generic power ("dropped" by bomb)
	int biggestGenericPowerID; //generic powers - 101+


	// All languages.
	HMDataMap hmLangCfg;

	// Code of current language.
	string currentLangCode;

	friend class Triggers;

private:
	// Current game speed
    float gameSpeed;

    // Game speed, with which you started the game.
    float initialGameSpeed;

	// Variable storing game over stat
    bool gameOver;

	// Variable storing game pause stat
    bool gamePaused;

	// Variable storing fullscreen stat
    bool fullscreen;

	// Variable storing game running stat. Almost always true.
    bool running;

	// Player score
    long score;

    // The multiplier applied to points on adding.
    float pointMultiplier;

	// Player current using power (if not used, then 0)
    int currentPower;

    // Variable storing default language (English)
	TranslationManager enUSTranslation;

	// The damage multiplier. It's set to ability.damage by default and changed by PowerDamageDecrease.
	float damageMultiplier;
};
