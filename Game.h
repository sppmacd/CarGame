#pragma once

#include <vector>
#include "Car.h"
#include "Gui.h"
#include "LevelData.h"
#include "Power.h"

using namespace std;

class Game
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

	/// Instance of the game
    static Game* instance;

	/// Game tick count depended on level
    int tickCount;

	/// Main tick count since the game started
    long mainTickCount;

	/// Variable storing MouseButtonReleased event status
    bool wasReleased;

	/// Variable storing gui status
    bool isGuiLoaded;

	/// Variable storing currently displayed GUI
    int displayedGui;

	/// Gui cooldown (deprecated)
    int guiCooldown;

	/// Power time
    int powerTime;

	/// Power cooldown
    int powerCooldown;

	/// Current level data
    LevelData level;

	/// Current return value for game
    int retVal;

	/// Time values (debug!)
    Times times;

	/// Is the power used? (Right click event status)
    bool isPowerUsed;

	/// Variable storing car creating speed.
	int carCreatingSpeed;

	/// Variable storing, if the new record is set (used in splash screen)
	bool newRecord;

	/////////////////////////
    ////// PLAYER DATA //////
	/////////////////////////

	/// Points required to get new coin multiplier
    int pointsToNewMpl;

	/// Score in the last tick. Used by the game over GUI
    long lastTickScore;

	/// Player highscore, currently used only by renderer
    long highScore;

	/// Variable stores if the player started game first one
    bool isNewPlayer;

	/// Player tutorial step. Can reset by restart
    int tutorialStep;

	/////////////////////////
	////// OTHER DATA ///////
	/////////////////////////

	/// Registry of the maps, used by MapSelect GUI.
	map<string, LevelData*> levelRegistry;

	/// Tick time
    sf::Time tickTime;

	/// Real tick time (with wait time)
    sf::Time realTickTime;

	/// Debug mode stat
    bool debug;

    /// Saved in byte format, unlocked levels for player
    int unlockedLevels;

	///// FUNCTIONS /////

	/// Default constructor
    Game();

	/// Default destructor
    ~Game();

	/// Vector storing cars
    vector<Car> cars;

	/// Add car to game
    void addCar(Car car);

	/// Called once on LevelTick, updates cars
    void updateCars();

	/// Deprecated, moves camera. Currently does nothing
    void moveCamera();

	/// Updates power effect
    void updateEffect();

	/// Loads specified level
    void loadGame(LevelData level);

	/// Reloads current level (after game over)
    void loadGame();

	/// Closes current level
    void closeLevel();

	/// Ticking normal game (with cars)
    void tickNormalGame();

    /// \brief Rename to loadPlayerData()! Loads player data
    void loadPlayerData();

    /// \brief Rename to savePlayerData()! Saves player data
    void savePlayerData();

	/// Start new tick, increment tickCount
    void newTick();

	/// Checks if the game is over
    bool isGameOver() { return this->gameOver; }

	/// Returns player score
    int getScore() { return this->score; }

	/// Sets game to game over stat
    void setGameOver();

	/// Pauses / unpauses game
    void pause(bool s) { this->gamePaused = s; }

	/// Check if the game is already paused
    bool paused() { return this->gamePaused; }

	/// Ticks gui. \param event Last event matching to GUI
    void tickGui(sf::Event& event);

	/// Toggles fullscreen
    void toggleFullscreen();

	/// Adds specified (v) coins to player
    void addCoins(long v);

	/// Removes specified (v) coins to player
    void removeCoins(long v);

	/// Returns player coins
    long getCoins();

	/// Displays (and loads) specified GUI
    void displayGui(int gui);

	/// Closes current GUI
    void closeGui();

	/// Returns level color, used by renderer
    sf::Color getLevelColor() { return this->level.getColor(); }

	/// Checks if the level is unlocked
    bool isLevelUnlocked(LevelData::MapType lvl);

	/// Checks if the game is running
    bool isRunning();

	/// Sets game state to not running and sets retval to specified value
    void exit(int ret);

	/// Returns player total points count. Not used yet
    long getTotalPoints();

	/// Returns coin multiplier
    int getCoinMultiplier();

	/// Adds score to player
    void addScore(int s);

	/// Handles wheel event, uses to change power
    void wheelEvent(sf::Event::MouseWheelScrollEvent event);

	/// Returns current power ID
    int getCurrentPower();

	/// Adds power to player (++)
    void getPower(int id);

	/// Use player power (--)
    void usePower(int id);

	/// Store player powers
    int* powers;

	/// Register GUIs
	void registerGUIs();

	/// Register powers
	void registerPowers();

	/// Get game speed value. Used externally by powers
	float getGameSpeed();

	/// Set game speed value. Used externally by powers
	void setGameSpeed(float speed);

	/// Variable storing power data registry
	map<int, PowerHandles> powerRegistry;

private:
	/// Not used variable storing camera position
    int cameraPos;

	/// Current game speed
    float gameSpeed;

	/// Variable storing game over stat
    bool gameOver;

	/// Variable storing game pause stat
    bool gamePaused;

	/// Variable storing fullscreen stat
    bool fullscreen;

	/// Variable storing game running stat. Almost always true.
    bool running;

    
	/// Total player points, not used yet
    long totalPlayerPoints;

	/// Coin multiplier
    int coinMpl;

	/// Player coins
    long playerCoins;

	/// Player score
    long score;

	/// Player current using power (if not used, then 0)
    int currentPower;

	///// FUNCTIONS /////

	/// Tick mouse move event internally (gui loop)
    void tickEventMouseMove(sf::Vector2f pos);

	/// Tick mouse click event internally (gui loop)
    void tickEventMouseClick(sf::Vector2f pos);
};
