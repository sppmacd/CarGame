#include "GameDisplay.h"
#include "Game.h"
#include <iostream>

GameDisplay* GameDisplay::instance;
String GameDisplay::loadingStr;

// GameDisplay error codes:
// D00 could not load texture

GameDisplay::GameDisplay(sf::RenderWindow* wnd)
    : renderWnd(wnd), error(false)
{
	this->wndSizeDefault = renderWnd->getSize();
	loadingStr = "Loading GameDisplay...";

    cout << "GameDisplay: Creating GameDisplay instance..." << endl;
    instance = this;

	this->fullscreenMode = 0;
	this->vsync = true;
}

void GameDisplay::clearTextures()
{
    this->texturesByName.clear();
}

void GameDisplay::reload()
{
    cout << "GameDisplay: Reloading graphics resources..." << endl;
    GameDisplay::loadingStr = "Reloading resources...";

    this->clearTextures();

    if(unknownTexture.getSize() == Vector2u(0,0))
    {
        // Create unknown texture
        Image imgUnknownTexture;
        imgUnknownTexture.create(64, 64);

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
    }

	for(int i = 0; i < Car::COUNT; i++)
	{
	    CarType* type = Game::instance->findCarTypeByID(Car::TypeId(i));
		if(type != NULL)
            this->addTexture("car/" + type->getTextureName());
        else
            cout << "GameDisplay: unknown car type " << i << ", cannot load textures." << endl;
	}

	if(!Game::instance->levelRegistry.empty())
    {
        for(auto ld : Game::instance->levelRegistry)
        {
            this->addTexture("bg/" + ld.second->getTextureName(), true, true);
            this->addTexture("map/" + ld.second->getTextureName());
        }
    }
    else
        cout << "GameDisplay: cannot load map textures, no level registered." << endl;

    this->addTexture("stat/coin");
    this->addTexture("stat/high");
    this->addTexture("stat/score");
    this->addTexture("stat/mpl");
    this->addTexture("stat/points_mpl");
    this->addTexture("gui/start");
    this->addTexture("gui/settings");
    this->addTexture("gui/quit");

    for(size_t s = 1; s < Game::instance->powerRegistry.size(); s++)
    {
        addTexture("power/" + to_string(s));
    }
}

void GameDisplay::addTexture(string name, bool repeated, bool smooth)
{
    //cout << "GameDisplay: Adding texture '" << name << "'..." << endl;

    sf::Texture tx;
	bool load = tx.loadFromFile("res/" + name + ".png");
	if (!load)
	{
	    cout << "GameDisplay: err D00(name='" << name << "')" << endl;
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

    if(!game->isGuiLoaded())
    {
        this->drawGame();
    }
    else
    {
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

sf::Text GameDisplay::drawString(String tx, int height, sf::Vector2f pos, sf::Text::Style style)
{
    sf::Text text = sf::Text(tx, this->guiFont);
    text.setPosition(pos);
    text.setCharacterSize(height);
    text.setStyle(style);

    return text;
}

sf::Text GameDisplay::drawCenteredString(String tx, int height, sf::Vector2f pos, sf::Text::Style style)
{
	sf::Text text = sf::Text(tx, this->guiFont);
	text.setCharacterSize(height);
	text.setStyle(style);
	text.setPosition(pos.x - text.getLocalBounds().width / 2, pos.y - text.getLocalBounds().height / 2);

	return text;
}

void GameDisplay::drawLoadingProgress(String action, sf::RenderWindow* wnd)
{
    if(GameDisplay::instance)
    {
        if(GameDisplay::instance->guiFont.getInfo().family.empty() && !GameDisplay::instance->error && !GameDisplay::instance->guiFont.loadFromFile("res/font.ttf"))
        {
            GameDisplay::instance->error = true;
            return;
        }

        // we can't use drawString in static function (before the GameDisplay is loaded)
        sf::Text text("Loading...", GameDisplay::instance->guiFont, 30);
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

        if(GameDisplay::instance->logoTexture.getSize() != Vector2u(0, 0))
        {
            Sprite sprite(GameDisplay::instance->logoTexture);
            sprite.setOrigin(Vector2f(GameDisplay::instance->logoTexture.getSize() / 2U));
            sprite.setPosition(wnd->getSize().x / 2, wnd->getSize().y / 4);
            wnd->draw(sprite);
        }
        else
        {
            if(!GameDisplay::instance->logoTexture.loadFromFile("res/gui/logo.png"))
                GameDisplay::instance->error = true;
        }
    }
}

void GameDisplay::drawGame()
{
    Game* game = Game::instance;

    this->renderWnd->clear(game->getLevelColor());

    // BACKGROUND
    sf::Sprite bg(this->getTexture("bg/" + game->level.getTextureName().toAnsiString()), IntRect(0, 0, this->getSize().x, 250));
	bg.setOrigin(0.f, 125.f);
	bg.setPosition(0.f, this->getSize().y / 2.f);
	bg.setScale(2.f, 2.f);
    this->renderWnd->draw(bg);

    // FOG
    VertexArray arr(Quads);
    Color fogColor = game->getLevelColor();
    arr.append(Vertex(Vector2f(0,0), fogColor));
    arr.append(Vertex(Vector2f(this->getSize().x,0), fogColor));
    arr.append(Vertex(Vector2f(this->getSize().x,this->getSize().y/2-250), fogColor));
    arr.append(Vertex(Vector2f(0,this->getSize().y/2-250), fogColor));

    arr.append(Vertex(Vector2f(0,this->getSize().y), fogColor));
    arr.append(Vertex(Vector2f(this->getSize().x,this->getSize().y), fogColor));
    arr.append(Vertex(Vector2f(this->getSize().x,this->getSize().y/2+250), fogColor));
    arr.append(Vertex(Vector2f(0,this->getSize().y/2+250), fogColor));


    arr.append(Vertex(Vector2f(0,this->getSize().y/2-250), fogColor));
    arr.append(Vertex(Vector2f(this->getSize().x,this->getSize().y/2-250), fogColor));
    arr.append(Vertex(Vector2f(this->getSize().x,this->getSize().y/2-200), Color::Transparent));
    arr.append(Vertex(Vector2f(0,this->getSize().y/2-200), Color::Transparent));

    arr.append(Vertex(Vector2f(0,this->getSize().y/2+250), fogColor));
    arr.append(Vertex(Vector2f(this->getSize().x,this->getSize().y/2+250), fogColor));
    arr.append(Vertex(Vector2f(this->getSize().x,this->getSize().y/2+200), Color::Transparent));
    arr.append(Vertex(Vector2f(0,this->getSize().y/2+200), Color::Transparent));
    this->renderWnd->draw(arr);

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
        Car& carobj = *game->cars[i];
		int animFrame = game->mainTickCount/carobj.frameLength % carobj.animSize;

		// health bars
        rect1.setSize(sf::Vector2f((carobj.health / carobj.maxHealth) * 30.f, 3.f));
        rect2.setSize(sf::Vector2f(30.f - ((carobj.health / carobj.maxHealth) * 30.f), 3.f));

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

        if(!game->cars[i]->isDestroying())
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

    if(!game->paused())
    {
        if(game->powerTime > 1)
            game->powerRegistry.find(game->getCurrentPower())->second->drawPower(this->renderWnd);
        else if(game->getCurrentPower() != 0)
            game->powerRegistry.find(game->getCurrentPower())->second->drawPowerIdle(this->renderWnd);
    }
}

bool GameDisplay::isError()
{
    return error;
}

Font* GameDisplay::getGuiFont()
{
    return &guiFont;
}
