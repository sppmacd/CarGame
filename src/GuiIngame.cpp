#include "GuiIngame.h"
#include "Game.h"
#include "GameDisplay.h"
#include "GuiMainMenu.h"
#include "GuiSettings.h"
#include <iostream>

void GuiIngame::onLoad()
{
    GameDisplay* game = GameDisplay::instance;

    addWidget(&(b1 = Button(this, sf::Vector2f(400.f, 40.f), sf::Vector2f(game->getSize().x / 2 - 200, game->getSize().y / 2), Game::instance->translation.get("gui.ingame.returngame"), 0)));
    addWidget(&(b2 = Button(this, sf::Vector2f(400.f, 40.f), sf::Vector2f(game->getSize().x / 2 - 200, game->getSize().y / 2 + 60), Game::instance->translation.get("gui.ingame.returnmainmenu"), 1)));
    addWidget(&(bQuit = Button(this, sf::Vector2f(400.f, 40.f), sf::Vector2f(game->getSize().x / 2 - 200, game->getSize().y / 2 + 180), Game::instance->translation.get("gui.ingame.quit"), 2)));
    addWidget(&(bSettings = Button(this, Vector2f(400.f, 40.f), Vector2f(game->getSize().x / 2 - 200, game->getSize().y / 2 + 120), Game::instance->translation.get("gui.settings.title"), 3)));
    b2.setColor(sf::Color::Yellow);
    bQuit.setColor(sf::Color::Red);
}

void GuiIngame::onDraw(sf::RenderWindow& wnd)
{
    //Gui::drawGui(wnd);

    b1.draw(wnd);
    b2.draw(wnd);
    bQuit.draw(wnd);
    bSettings.draw(wnd);

    wnd.draw(GameDisplay::instance->drawCenteredString(Game::instance->translation.get("gui.ingame.title"), 30, sf::Vector2f(GameDisplay::instance->getSize().x / 2, 200)));

    Gui::onDraw(wnd);
}

void GuiIngame::onResize()
{
    GameDisplay* game = GameDisplay::instance;
    b1.setPosition(Vector2f(game->getSize().x / 2 - 200, game->getSize().y / 2));
    b2.setPosition(Vector2f(game->getSize().x / 2 - 200, game->getSize().y / 2 + 60));
    bQuit.setPosition(Vector2f(game->getSize().x / 2 - 200, game->getSize().y / 2 + 180));
    bSettings.setPosition(Vector2f(game->getSize().x / 2 - 200, game->getSize().y / 2 + 120));
}

void GuiIngame::onClick(int button)
{
    Game* game = Game::instance;

	if(button == 0)
	{
		game->pause(false);
		game->closeGui();
	}
    else if(button == 1)
    {
        game->displayGui(new GuiMainMenu); //main menu
        game->closeLevel();
    }
    else if(button == 2)
    {
        game->exit(0);
    }
    else if(button == 3)
    {
        runDialog(new GuiSettings, -1);
    }
}

void GuiIngame::onKeyboardEvent(Keyboard::Key key, bool released, bool, bool, bool, bool)
{
    if(key == Keyboard::Escape && released == false)
        onClick(0);
}
