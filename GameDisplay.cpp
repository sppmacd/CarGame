#include "GameDisplay.h"
#include "Game.h"
#include <iostream>

GameDisplay* GameDisplay::instance;
string GameDisplay::loadingStr;

GameDisplay::GameDisplay(sf::RenderWindow* wnd)
    : renderWnd(wnd)
{
	this->wndSizeDefault = renderWnd->getSize();
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
    cout << "GameDisplay: Reloading graphics resources..." << endl;
    this->clearTextures();

	// Create unknown texture
	Image imgUnknownTexture;
	imgUnknownTexture.create(64, 64);

	GameDisplay::loadingStr = "Creating unknown texture...";
	for(int i = 0; i < 64; i++)
	for(int j = 0; j < 64; j++)
	{ 
		// 0     150    300
		// *******%%%%%%% 0
		// *******%%%%%%%
		// *******%%%%%%% 150
		// %%%%%%%*******
		// %%%%%%%*******
		// %%%%%%%******* 300

		bool isNB = ((i <= 32 && j >= 32) || (i >= 32 && j <= 32));
		imgUnknownTexture.setPixel(i, j, isNB ? Color::Red : Color::Green);
	}
	this->unknownTexture.loadFromImage(imgUnknownTexture);
	this->unknownTexture.setRepeated(true);

	GameDisplay::loadingStr = "Reloading resources...";

    this->addTexture("car/default");
    this->addTexture("car/lorry");
    this->addTexture("car/bus");
    this->addTexture("car/ambulance");

	for (int i = 0; i < Car::COUNT; i++)
	{
		this->addTexture("car/" + Game::instance->findCarTypeByID(Car::TypeId(i))->getTextureName());
	}

	for (auto ld : Game::instance->levelRegistry)
	{
		this->addTexture("bg/" + ld.second->getTextureName(), true, true);
		this->addTexture("map/" + ld.second->getTextureName());
	}

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

void GameDisplay::addTexture(string name, bool repeated, bool smooth)
{
    cout << "GameDisplay: Adding texture '" << name << "'..." << endl;

    sf::Texture tx;
	bool load = tx.loadFromFile("res/" + name + ".png");
	if (!load)
	{
		this->texturesByName.insert(pair<string, sf::Texture>(name, unknownTexture));
		return;
	}
	else
	{
		tx.setRepeated(repeated);
		tx.setSmooth(smooth);
		this->texturesByName.insert(make_pair(name, tx));
	}
}

sf::Texture & GameDisplay::getTexture(string name)
{
	map<string,Texture>::iterator tex = texturesByName.find(name);

	return (tex != texturesByName.end()) ? tex->second : unknownTexture;
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

Vector2u GameDisplay::getSize()
{
	return wndSizeDefault;
}

void GameDisplay::setWndSize(Vector2u vec)
{
	wndSizeDefault = vec;
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
    
	// Temporarily load Font
	if (!l)
	{
		f.loadFromFile("res/font.ttf");
		l = true;
	}

	// we can't use drawString in static function (before the GameDisplay is loaded)
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
    sf::Sprite bg(this->getTexture("bg/" + game->level.getTextureName().toAnsiString()), IntRect(0, 0, this->getSize().x, 500));
	bg.setOrigin(0.f, 250.f);
	bg.setPosition(0.f, this->getSize().y / 2.f);
	bg.setScale(1.f, 1.f);
    this->renderWnd->draw(bg);

    // CARS
    sf::Sprite car(this->getTexture("car/default"));
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
        car.setTexture(this->getTexture("car/" + carobj.getTextureName()));
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
