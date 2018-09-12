#ifndef GAMEDISPLAY_H
#define GAMEDISPLAY_H

#include <SFML/Graphics.hpp>
#include <string>
#include <map>
#include <iostream>

using namespace std;

class GameDisplay
{
public:
    GameDisplay(sf::RenderWindow* wnd);
	GameDisplay() {}
    virtual ~GameDisplay();

    void drawGui();
	void nextFullscreenMode();
    void display();
    void drawGame();
    void drawEffect();
    void reload();
    void clearTextures();
    void drawStat(int, int, string, long);
	static void drawLoading(sf::RenderWindow* wnd);
	static string loadingStr;
    static void drawLoadingProgress(string action, sf::RenderWindow* wnd);
    void drawTutorial(sf::Vector2f, sf::Vector2f, string str);
	void drawSplash(string text);
	void setSplash(string text);

    sf::Text drawString(string tx, int height, sf::Vector2f pos, sf::Text::Style style = sf::Text::Regular);
    sf::Text drawCenteredString(string tx, int height, sf::Vector2f pos, sf::Text::Style style = sf::Text::Regular);

    sf::RenderWindow* getRenderWnd()
    {
        return this->renderWnd;
    }

    void addTexture(string name);
    //sf::Texture getTexture(string name);

    static GameDisplay* instance;

	map<string, sf::Texture> texturesByName;

	void setVSync(bool b);
	bool getVSync();
	Vector2u getSize();
	void setWndSize(Vector2u vec);

protected:

private:
    sf::RenderWindow* renderWnd;

    sf::Texture unknownTexture;

    sf::Font guiFont;

	// Splash data - text
	string splashText;

	// Splash data - is the splash set?
	bool splashSet;

	// Splash data - animation tick
	int splashTick;

	bool vsync;
	int fullscreenMode;

	// Default wnd size, set on create.
	Vector2u wndSizeDefault;
};

#endif // GAMEDISPLAY_H
