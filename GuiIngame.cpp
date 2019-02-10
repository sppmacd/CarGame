#include "GuiIngame.h"
#include "Game.h"
#include "GameDisplay.h"
#include "GuiMainMenu.h"
#include <iostream>

void GuiIngame::onLoad()
{
    GameDisplay* game = GameDisplay::instance;

    addButton(b1 = Button(sf::Vector2f(400.f, 40.f), sf::Vector2f(game->getSize().x / 2 - 200, game->getSize().y / 2), "Return to game", 0));
    addButton(b2 = Button(sf::Vector2f(400.f, 40.f), sf::Vector2f(game->getSize().x / 2 - 200, game->getSize().y / 2 + 60), "Return to main menu", 1));
    addButton(bQuit = Button(sf::Vector2f(400.f, 40.f), sf::Vector2f(game->getSize().x / 2 - 200, game->getSize().y / 2 + 120), "Quit Game", 2));
    b2.setColor(sf::Color::Yellow);
    bQuit.setColor(sf::Color::Red);
}

void GuiIngame::onDraw(sf::RenderWindow& wnd)
{
    //Gui::drawGui(wnd);

    b1.draw(wnd);
    b2.draw(wnd);
    bQuit.draw(wnd);

    wnd.draw(GameDisplay::instance->drawCenteredString("Game menu", 30, sf::Vector2f(GameDisplay::instance->getSize().x / 2, 200)));

    Gui::onDraw(wnd);
}

void GuiIngame::onClick(long button)
{
    Game* game = Game::instance;

	if (button == 0)
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
}
