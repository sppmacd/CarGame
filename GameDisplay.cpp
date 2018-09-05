#include "GameDisplay.h"
#include "Game.h"
#include <iostream>

GameDisplay* GameDisplay::instance;
string GameDisplay::loadingStr;

GameDisplay::GameDisplay(sf::RenderWindow* wnd)
    : renderWnd(wnd)
{
	loadingStr = "Loading GameDisplay...";

    cout << "GameDisplay: Creating GameDisplay instance..." << endl;
    instance = this;

	this->fullscreenMode = 0;
	this->vsync = true;
    this->reload();
}

void GameDisplay::clearTextures()
{
    cout << "GameDisplay: Clearing texture map..." << endl;
    this->texturesByName.clear();
}

void GameDisplay::reload()
{
	GameDisplay::loadingStr = "Reloading resources...";

    cout << "GameDisplay: Reloading graphics resources..." << endl;
    this->clearTextures();

    this->addTexture("car/default");
    this->addTexture("car/lorry");
    this->addTexture("car/bus");
    this->addTexture("car/ambulance");
    this->addTexture("bg/countryside");
    this->addTexture("bg/desert");
    this->addTexture("bg/forest");
    this->addTexture("bg/ice");
    this->addTexture("bg/mountains");
    this->addTexture("stat/coin");
    this->addTexture("stat/high");
    this->addTexture("stat/score");
    this->addTexture("stat/mpl");
    this->addTexture("stat/points_mpl");
    this->addTexture("power/1");
    this->addTexture("power/2");

    sf::Font font;
    font.loadFromFile("res/font.ttf");
    this->guiFont = font;
}

void GameDisplay::addTexture(string name)
{
    cout << "GameDisplay: Adding texture '" << name << "'..." << endl;

    sf::Texture tx;
    tx.loadFromFile("res/" + name + ".png");
    this->texturesByName.insert(pair<string,sf::Texture>(name,tx));
}

void GameDisplay::setVSync(bool b)
{
	vsync = b;
	renderWnd->setVerticalSyncEnabled(vsync);
	GameDisplay::instance->getRenderWnd()->setFramerateLimit(vsync ? 0 : 60);
}

bool GameDisplay::getVSync()
{
	return vsync;
}

GameDisplay::~GameDisplay()
{
    cout << "GameDisplay: Deleting GameDisplay instance..." << endl;
}

void GameDisplay::display()
{
    Game* game = Game::instance;

    if(!game->isGuiLoaded)
    {
        this->drawGame();
    }
    else
    {
        if(game->displayedGui == 2)
            this->renderWnd->clear(sf::Color(25, 20, 20));
        else
            this->renderWnd->clear(sf::Color(50, 40, 40));
    }

    this->drawGui();

	// Tick and draw splash

	if (this->splashSet)
		this->drawSplash(this->splashText);

	if (this->splashTick > 0)
		this->splashTick--;
	else
		this->splashSet = false;

    this->renderWnd->display();
}

sf::Text GameDisplay::drawString(string tx, int height, sf::Vector2f pos, sf::Text::Style style)
{
    sf::Text text = sf::Text(tx, this->guiFont);
    text.setPosition(pos);
    text.setCharacterSize(height);
    text.setStyle(style);

    return text;
}

sf::Text GameDisplay::drawCenteredString(string tx, int height, sf::Vector2f pos, sf::Text::Style style)
{
	sf::Text text = sf::Text(tx, this->guiFont);
	text.setCharacterSize(height);
	text.setStyle(style);
	text.setPosition(pos.x - text.getLocalBounds().width / 2, pos.y - text.getLocalBounds().height / 2);
	
	return text;
}

void GameDisplay::drawLoadingProgress(string action, sf::RenderWindow* wnd)
{
    static sf::Font f;
	static bool l = false;
    
	if (!l)
	{
		f.loadFromFile("res/font.ttf");
		l = true;
	}

    sf::Text text("Loading...", f, 30);
	text.setOrigin(text.getLocalBounds().width / 2, text.getLocalBounds().height / 2);
	text.setStyle(sf::Text::Bold);

    text.setFillColor(sf::Color::White);
    text.setPosition((sf::Vector2f(wnd->getSize() / (unsigned int)2)));
    wnd->draw(text);

    text.setFillColor(sf::Color::Yellow);
    text.setString(action);
	text.setOrigin(text.getLocalBounds().width / 2, text.getLocalBounds().height / 2);
    text.setPosition((sf::Vector2f(wnd->getSize() / (unsigned int)2)) + sf::Vector2f(0.f, 100.f));
    wnd->draw(text);

    text.setString("Car Game");
    text.setCharacterSize(60);
	text.setOrigin(text.getLocalBounds().width / 2, text.getLocalBounds().height / 2);
    text.setPosition((sf::Vector2f(wnd->getSize() / (unsigned int)2)) - sf::Vector2f(0.f, 100.f));
    text.setFillColor(sf::Color::Red);
    wnd->draw(text);
}

void GameDisplay::drawGame()
{
    Game* game = Game::instance;

    this->renderWnd->clear(game->getLevelColor());

    // BACKGROUND
    sf::Sprite bg(this->texturesByName.find("bg/" + game->level.getTextureName().toAnsiString())->second);
    bg.setScale(1920.0f, 2.0f);
	bg.setOrigin(0.f, 125.f);
	bg.setPosition(0.f, 540.f);
    this->renderWnd->draw(bg);

    // CARS
    sf::Sprite car(this->texturesByName.find("car/default")->second);
    car.setOrigin(50.f, 20.f);
    car.setScale(2.0f, 2.0f);

    sf::RectangleShape rect1;
    rect1.setFillColor(sf::Color::Green);
    sf::RectangleShape rect2;
    rect2.setFillColor(sf::Color::Red);

    for(unsigned int i = 0; i < game->cars.size(); i++)
    {
        Car& carobj = game->cars[i];
		int animFrame = game->mainTickCount/carobj.frameLength % carobj.animSize;

		// health bars
        rect1.setSize(sf::Vector2f((((float)carobj.health+1) / (float)carobj.maxHealth) * 30.f, 3.f));
        rect2.setSize(sf::Vector2f(30.f - ((((float)carobj.health+1) / (float)carobj.maxHealth) * 30.f), 3.f));

		// common car data
		if (carobj.isDestroying())
		{
			car.setRotation(carobj.destroyTick * 7.1f);
			car.setScale(carobj.destroyTick / 20.f, carobj.destroyTick / 20.f);
		}
		car.setPosition(carobj.getScreenPos());

		// car background layer
		car.setColor(carobj.getColor());
        car.setTexture(this->texturesByName.find("car/" + carobj.getTextureName())->second);
		car.setTextureRect(IntRect(0, 0, 100, 40));
        this->renderWnd->draw(car);

		// car foreground layer (animated)
		car.setColor(Color::White);
		car.setTextureRect(IntRect(0, (animFrame + 1) * 40, 100, 40));
		this->renderWnd->draw(car);

		// reset
        car.setScale(2.0f, 2.0f);
        car.setRotation(0.f);

        rect1.setPosition(car.getPosition().x, car.getPosition().y + 30);
        rect2.setPosition(car.getPosition().x + rect1.getSize().x, car.getPosition().y + 30);

        if(!game->cars[i].isDestroying())
        {
            this->renderWnd->draw(rect1);
            this->renderWnd->draw(rect2);
        }
    }

    this->drawEffect();
}

void GameDisplay::drawEffect()
{
    Game* game = Game::instance;

	if(game->powerTime > 1)
		game->powerRegistry.find(game->getCurrentPower())->second.drawPower(this->renderWnd);
	else
		game->powerRegistry.find(game->getCurrentPower())->second.drawPowerIdle(this->renderWnd);
}
