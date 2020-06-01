#include "GameDisplay.h"
#include "Game.h"
#include <iostream>
#include "FileUtil.hpp"
#include "DebugLogger.hpp"
#include "ModuleManager.hpp"

CGAPI GameDisplay* GameDisplay::instance = NULL;
String GameDisplay::loadingStr;
String GameDisplay::consoleStr;

// GameDisplay error codes:
// D00 could not load texture
// D01 could not load /res folder

GameDisplay::GameDisplay(sf::RenderWindow* wnd)
    : renderWnd(wnd), error(false)
{
	wndSizeDefault = renderWnd->getSize();
	loadingStr = "Loading GameDisplay...";

    DebugLogger::log("Creating GameDisplay", "GameDisplay", "INFO");
    instance = this;

	fullscreenMode = 0;
	vsync = true;
	pointAnimTick = 0;
	noResMode = false;
}

void GameDisplay::clearTextures()
{
    DebugLogger::logDbg("Clearing textures", "GameDisplay");
    texturesByName.clear();
}

void GameDisplay::reload(bool _noResMode)
{
    ModuleManager::instance->setCurrent("api");

    DebugLogger::log("Reloading ResourceManager", "GameDisplay");
    GameDisplay::loadingStr = "Loading textures...";

    clearTextures();
    noResMode |= _noResMode; //cannot unset when once set by command line

    GameDisplay::loadingStr = "Loading textures... Unknown texture";
    DebugLogger::logDbg("Generating unknown texture", "GameDisplay");
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

            bool isNB = ((i <= 32 && j > 32) || (i > 32 && j <= 32));
            imgUnknownTexture.setPixel(i, j, isNB ? Color::Red : Color::Green);
        }
        unknownTexture.loadFromImage(imgUnknownTexture);
        unknownTexture.setRepeated(true);
    }

    DebugLogger::log("NoRes Mode: " + std::to_string(_noResMode), "GameDisplay");
    if(!noResMode)
    {
        if(FileUtil::getFileType("res") == FileUtil::NOTEXISTING)
        {
            DebugLogger::log("Resource folder not found", "GameDisplay", "FATAL");
            throw runtime_error("No resource folder found. Try reinstalling the game.\nD01");
        }

        DebugLogger::log("Loading textures for each module", "GameDisplay", "INFO");
        GameDisplay::loadingStr = "Loading textures... Cars";
        for(auto& car: Game::instance->gpo.carTypes.arr())
        {
            CarType* type = car.second;
            if(type != NULL)
                addTexture("car/" + type->getTextureName(), type->getModuleName());
            else
                DebugLogger::log("Unknown car type " + car.first.baseId.toString() + ", cannot load textures.", "GameDisplay", "ERROR");
        }

        GameDisplay::loadingStr = "Loading textures... Maps";
        if(Game::instance->gpo.levels.count() > 0)
        {
            for(auto ld : Game::instance->gpo.levels.arr())
            {
                LevelData* data = ld.second;
                addTexture("bg/" + data->getTextureName(), data->getModuleName(), true, true);
                addTexture("map/" + data->getTextureName(), data->getModuleName());
            }
        }

        DebugLogger::logDbg("Adding power icons", "GameDisplay");
        GameDisplay::loadingStr = "Loading textures... Powers";
        for(auto it = Game::instance->gpo.powers.arr().begin(); it != Game::instance->gpo.powers.arr().end(); it++)
        {
            DebugLogger::logDbg("Adding textures for power: " + it->second->getName());
            addTexture("power/" + it->first.baseId.getObjectId(), it->second->getModuleName());
            it->second->onTextureLoad();
        }

        GameDisplay::loadingStr = "Loading textures... Stat & GUI";
        DebugLogger::logDbg("Adding stat and GUI textures", "GameDisplay");
        addTexture("stat/coin");
        addTexture("stat/high");
        addTexture("stat/score");
        addTexture("stat/mpl");
        addTexture("stat/points_mpl");
        addTexture("gui/start");
        addTexture("gui/settings");
        addTexture("gui/quit");
    }

    ModuleManager::instance->setCurrent("");
}

void GameDisplay::addTexture(string name, string modName, bool repeated, bool smooth)
{
    DebugLogger::logDbg("Adding texture: " + modName + "$" + name + "(repeated=" + std::to_string(repeated) + "," + "smooth=" + std::to_string(smooth) + ")", "GameDisplay");
    sf::Texture tx;
    ModuleManager::instance->setCurrent(modName);

	bool load = tx.loadFromFile("res/" + modName + "/textures/" + name + ".png");
	if(!load)
    {
        DebugLogger::log("Texture not loaded: '" + modName + "$" + name + "'. Error: D00", "GameDisplay", "ERROR");
        DebugLogger::logDbg("Texture path: 'res/" + modName + "/textures/" + name + ".png'", "GameDisplay");
		texturesByName.insert(pair<string, sf::Texture>(name, unknownTexture));
		return;
	}
	else
	{
		tx.setRepeated(repeated);
		tx.setSmooth(smooth);
		texturesByName.insert(make_pair(modName + "$" + name, tx));
	}
}

sf::Texture & GameDisplay::getTexture(string name)
{
	map<string,Texture>::iterator tex = texturesByName.find(name);

	if(tex == texturesByName.end())
    {
        DebugLogger::log("Couldn't get texture: " + name + ". Falling back to unknown texture.", "GameDisplay", "ERROR");
        texturesByName[name] = unknownTexture;
        return unknownTexture;
    }
    else
    {
        return tex->second;
    }
}

void GameDisplay::setVSync(bool b)
{
	vsync = b;
	DebugLogger::logDbg("Setting VSync state to " + std::to_string(b), "GameDisplay");
	renderWnd->setVerticalSyncEnabled(vsync);
	GameDisplay::instance->getRenderWnd()->setFramerateLimit(vsync ? 0 : 60);
}

bool GameDisplay::getVSync()
{
	return vsync;
}

Vector2u GameDisplay::getSize()
{
	return Vector2u(Game::instance->getSize());
}

// TODO: set as deprecated
void GameDisplay::setWndSize(Vector2u vec)
{
	wndSizeDefault = vec;
}

GameDisplay::~GameDisplay()
{
    DebugLogger::log("Deleting GameDisplay...", "GameDisplay");
}

void GameDisplay::display()
{
    Game* game = Game::instance;

    // Prepare viewport for drawing
    // GameView is now common to GUI and Game views.

    // 0.2+ scale window to be like 1920x1080
    float mapSizeX = 1920.f;
    float mapSizeY = 1080.f;
    sf::View view = Game::instance->getGameView();
    renderWnd->setView(view);

    if(!game->isGuiLoaded())
    {
        drawGame();
    }
    else
    {
        renderWnd->clear(sf::Color(50, 40, 40));
    }

    drawGui();

	// Tick and draw splash

	if (splashSet)
		drawSplash(splashText);

	if (splashTick > 0)
		splashTick--;
	else
		splashSet = false;

    renderWnd->display();
}

sf::Text GameDisplay::drawString(String tx, int height, sf::Vector2f pos, sf::Text::Style style)
{
    sf::Text text = sf::Text(tx, guiFont);
    text.setPosition(pos);
    text.setCharacterSize(height);
    text.setStyle(style);

    return text;
}

sf::Text GameDisplay::drawCenteredString(String tx, int height, sf::Vector2f pos, sf::Text::Style style)
{
	sf::Text text = sf::Text(tx, guiFont);
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

        std::string mod = "api";
        if(ModuleManager::instance)
            mod = ModuleManager::instance->current();

        text.setCharacterSize(20);
        text.setString(mod + " $ " + action);
        text.setOrigin(text.getLocalBounds().width / 2, text.getLocalBounds().height / 2);
        text.setPosition((sf::Vector2f(wnd->getSize() / (unsigned int)2)) + sf::Vector2f(0.f, 100.f));
        wnd->draw(text);

        if(GameDisplay::instance->logoTexture.getSize() != Vector2u(0, 0))
        {
            Sprite sprite(GameDisplay::instance->logoTexture);
            sprite.setOrigin(Vector2f(GameDisplay::instance->logoTexture.getSize() / 2U));
            sprite.setPosition(wnd->getSize().x / 2, wnd->getSize().y / 4);
            sprite.setScale(0.75f, 0.75f);
            wnd->draw(sprite);
        }
        else
        {
            if(!GameDisplay::instance->error && !GameDisplay::instance->logoTexture.loadFromFile("res/api/textures/gui/logo.png", sf::IntRect(0, 0, 512, 256)))
            {
                GameDisplay::instance->error = true;
            }
        }
    }
}

void GameDisplay::drawGame()
{
    Game* game = Game::instance;

    float mapSizeX = 1920.f;
    float mapSizeY = 1080.f;

    renderWnd->clear(/*game->getLevelColor()*/Color::Black);

    { // BACKGROUND
        float fac = 1.5625f;

        // will it be valid after textures reloaded ??
        //static Texture* tex = &getTexture("bg/" + game->level.getTextureName().toAnsiString());

        VertexArray arrbg(Quads, 8);
        Color fogColor = game->getLevelColor();

        // bg
        arrbg[0] = sf::Vertex(sf::Vector2f(0.f, 0.f), fogColor);
        arrbg[1] = sf::Vertex(sf::Vector2f(mapSizeX, 0.f), fogColor);
        arrbg[2] = sf::Vertex(sf::Vector2f(mapSizeX, mapSizeY), fogColor);
        arrbg[3] = sf::Vertex(sf::Vector2f(0.f, mapSizeY), fogColor);
        renderWnd->draw(arrbg);

        // road
        arrbg[0] = sf::Vertex(sf::Vector2f(0.f, mapSizeY/2 - 160.f * fac), sf::Color::White, sf::Vector2f(0.f, 0.f));
        arrbg[1] = sf::Vertex(sf::Vector2f(0.f, mapSizeY/2 + 160.f * fac), sf::Color::White, sf::Vector2f(0.f, 320.f));
        arrbg[2] = sf::Vertex(sf::Vector2f(mapSizeX, mapSizeY/2 + 160.f * fac), sf::Color::White, sf::Vector2f(mapSizeX / fac, 320.f));
        arrbg[3] = sf::Vertex(sf::Vector2f(mapSizeX, mapSizeY/2 - 160.f * fac), sf::Color::White, sf::Vector2f(mapSizeX / fac, 0.f));
        renderWnd->draw(arrbg, sf::RenderStates(&getTexture(game->level->getModuleName() + "$bg/" + game->level->getTextureName().toAnsiString())));
    }
    { // FOG
        VertexArray arr(Quads, 8);
        Color fogColor = game->getLevelColor();

        //up
        arr[0] = (Vertex(Vector2f(0,mapSizeY/2-250), fogColor));
        arr[1] = (Vertex(Vector2f(mapSizeX,mapSizeY/2-250), fogColor));
        arr[2] = (Vertex(Vector2f(mapSizeX,mapSizeY/2-200), Color::Transparent));
        arr[3] = (Vertex(Vector2f(0,mapSizeY/2-200), Color::Transparent));

        //down
        arr[4] = (Vertex(Vector2f(0,mapSizeY/2+250), fogColor));
        arr[5] = (Vertex(Vector2f(mapSizeX,mapSizeY/2+250), fogColor));
        arr[6] = (Vertex(Vector2f(mapSizeX,mapSizeY/2+200), Color::Transparent));
        arr[7] = (Vertex(Vector2f(0,mapSizeY/2+200), Color::Transparent));
        renderWnd->draw(arr);
    }

    if(game->playerData.isNewPlayer && game->playerData.tutorialStep == TUT_DONTLEAVE)
    {
        drawTutorial(Vector2f(0.f, mapSizeY/2 - 160.f), Vector2f(50.f, 320.f), game->translation.get("tutorial.dontleave"));
    }

    // CARS
    sf::Sprite car(getTexture("api$car/default"));
    car.setOrigin(50.f, 20.f);
    car.setScale(2.0f, 2.0f);

    sf::RectangleShape rect1;
    rect1.setFillColor(sf::Color::Green);
    sf::RectangleShape rect2;
    rect2.setFillColor(sf::Color::Red);

    for(int i = game->cars.size() - 1; i >= 0; i--)
    {
        Car& carobj = *game->cars[i];
		int animFrame = game->mainTickCount/carobj.frameLength % carobj.animSize;

		// health bars...
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
        car.setTexture(getTexture(carobj.type->getModuleName() + "$car/" + carobj.getTextureName()));
		car.setTextureRect(IntRect(0, 0, 100, 40));
        renderWnd->draw(car);

		// car foreground layer (animated)
		car.setColor(Color::White);
		car.setTextureRect(IntRect(0, (animFrame + 1) * 40, 100, 40));
		renderWnd->draw(car);

		// reset
        car.setScale(2.0f, 2.0f);
        car.setRotation(0.f);

        // ...health bars
        rect1.setPosition(car.getPosition().x, car.getPosition().y + 30);
        rect2.setPosition(car.getPosition().x + rect1.getSize().x, car.getPosition().y + 30);

        if(!game->cars[i]->isDestroying())
        {
            renderWnd->draw(rect1);
            renderWnd->draw(rect2);
        }

        // todo
        // tutorial
        if(game->playerData.isNewPlayer)
        {
            /*if(game->playerData.tutorialStep == TUT_DONTLEAVE && i == 0)
            {
                if(carobj.typeId == Car::BOMB)
                {
                    game->playerData.tutorialStep = TUT_AVOIDBOMB;
                }
                else
                {
                    drawTutorial(car.getPosition() - car.getOrigin(), Vector2f(car.getTextureRect().width * 2, car.getTextureRect().height * 2), game->translation.get("tutorial.destroycar"));
                }
            }
            else if(game->playerData.tutorialStep == TUT_AVOIDBOMB && i == 0 && carobj.typeId == Car::BOMB)
            {
                drawTutorial(car.getPosition() - car.getOrigin(), Vector2f(car.getTextureRect().width * 2, car.getTextureRect().height * 2), game->translation.get("tutorial.avoidbomb"));
            }*/
        }
    }

    drawEffect();
}

void GameDisplay::drawEffect()
{
    Game* game = Game::instance;

    if(!game->paused())
    {
        ModuleIdentifier powerId = game->getCurrentPower();
        if(game->powerTime > 1)
            game->powerHandle->drawPower(renderWnd);
        else if(powerId != ModuleIdentifier("api$no_power_equipped"))
            game->gpo.powers.findById(powerId)->drawPowerIdle(renderWnd);
        else if(game->powerHandle)
            game->powerHandle->drawPowerIdle(renderWnd);
            //game->powerHandle->drawPowerCooldown(renderWnd);
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

void GameDisplay::createFullscreenWnd()
{
    vector<VideoMode> fullscreenModes = sf::VideoMode::getFullscreenModes();
    DebugLogger::log("Trying to create fullscreen window... (Modes=" + std::to_string(fullscreenModes.size()) + ")", "GameDisplay");
    if(fullscreenModes.empty())
    {
        DebugLogger::log("Fullscreen not supported - switching to window mode", "GameDisplay", "ERROR");
        renderWnd->create(VideoMode(1280, 720), sf::String("CG " + string(CG_VERSION) + "(Fullscreen not supported)"));
    }

    for(size_t i = 0; i < fullscreenModes.size(); i++)
    {
        renderWnd->create(fullscreenModes[i], sf::String("CG " + string(CG_VERSION) + "(Fullscreen)"), sf::Style::Fullscreen);
        Game::instance->setWindow(renderWnd); //update guihandler view
        if(renderWnd->isOpen())
            break;
    }
    renderWnd->setVerticalSyncEnabled(true);
}

void GameDisplay::resetPointAnim()
{
    pointAnimTick = 15;
}
