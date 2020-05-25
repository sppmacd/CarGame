#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <map>
#include <iostream>

using namespace std;
using namespace sf;

class GameDisplay
{
public:
	// Default constructor with wnd.
    GameDisplay(sf::RenderWindow* wnd);

	// Default constructor.
	GameDisplay() {}

	// Destructor.
    virtual ~GameDisplay();

	// Draw GUI [in loop].
    void drawGui();

	// Create window with fullscreen mode.
	void createFullscreenWnd();

	// Display [in loop].
    void display();

	// Draw game: cars, background, powers etc [in loop].
    void drawGame();

	// Draw effect (power) [in loop].
    void drawEffect();

	// Reload GameDisplay: textures,...
    void reload();

	// Delete all textures, called before reloading.
    void clearTextures();

	// Draw stat. Params: statX, statY, stat texture name, displayed value, animation tick.
    void drawStat(int x, int y, String name, long val, int animTick = 0);

	// Draw loading screen [in loading loop].
	static void drawLoading(sf::RenderWindow* wnd);

	// The string displayed on loading screen.
	static String loadingStr;

	// The string displayed on console (used to display more loading info when GameDisplay is not initialized)
	static String consoleStr;

	// Draw simple loading screen.
    static void drawLoadingProgress(String action, sf::RenderWindow* wnd);

	// Draw tutorial text and rectangle.
	// %pos - Tutorial rectangle position.
	// %size - Tutorial rectangle size.
	// %str - Tutorial string (localized).
    void drawTutorial(sf::Vector2f pos, sf::Vector2f size, String str);

	// Draw splash with specified text.
	void drawSplash(String text);

	// Set splash text.
	void setSplash(String text);

	// Retrieve mouse position (in game coordinates - the GUI coordinates are trivial)
	sf::Vector2i mousePos();

	// Get drawable string prepared to draw. Params: string,size,position,style. Returns: instance of sf::Text.
    sf::Text drawString(String tx, int height, sf::Vector2f pos, sf::Text::Style style = sf::Text::Regular);

	// Get drawable centered string prepared to draw. Params: string,size,position,style. Returns: instance of sf::Text.
    sf::Text drawCenteredString(String tx, int height, sf::Vector2f pos, sf::Text::Style style = sf::Text::Regular);

	// Retrieve current RenderWindow.
    sf::RenderWindow* getRenderWnd()
    {
        return this->renderWnd;
    }

	// Add/register texture to GameDisplay.
    void addTexture(string name, string modName = "api", bool repeated = false, bool smooth = false);

	// Get texture instance.
    sf::Texture& getTexture(string name);

    // Set vsync state to 'b'
	void setVSync(bool b);

	// Retrieve vsync state.
	bool getVSync();

	// Get default size of the view.
	sf::Vector2u getSize();

	// A default instance of GameDisplay.
    static GameDisplay* instance;

	// Texture map.
	map<string, sf::Texture> texturesByName;

	Texture logoTexture;

	// \deprecated
    // Set default size. It doesn't change real size of the window, it is only called when VideoMode is changed.
	void setWndSize(sf::Vector2u vec);

	// Check if GameDisplay encountered an error while loading. If set, the game will close immediately.
	bool isError();

	// Returns default GUI font.
    Font* getGuiFont();

    // Starts point animation. Called when the player gets points.
    void resetPointAnim();

    // Draw tick times info (profiler output).
    void drawDebugPie(sf::RenderWindow* wnd);

    // Draw debug info (shift + f3).
    void drawDebugInfo(sf::RenderWindow* wnd);

protected:

private:
	// render window
    sf::RenderWindow* renderWnd;

	// unknown texture handle
    sf::Texture unknownTexture;

	// default GUI font
    sf::Font guiFont;

	// Splash data - text
	String splashText;

	// Splash data - is the splash set?
	bool splashSet;

	// Splash data - animation tick
	int splashTick;

	// vsync state
	bool vsync;

	// current fullscreen mode
	int fullscreenMode;

	// Default wnd size, set on create.
	sf::Vector2u wndSizeDefault;

	bool error;

	// Animation tick of point stat.
	int pointAnimTick;
};
