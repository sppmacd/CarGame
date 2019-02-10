#include "GuiMainMenu.h"
#include "Game.h"
#include "GameDisplay.h"
#include "GuiSettings.h"
#include "GuiMapSelect.h"
#include <iostream>

using namespace std;

void GuiMainMenu::onLoad()
{
    GameDisplay* game = GameDisplay::instance;

    addButton(b1 = Button(sf::Vector2f(400.f, 40.f), sf::Vector2f(game->getSize().x / 2 - 200, game->getSize().y / 2 - 60), "New Game", 0));
    addButton(b2 = Button(sf::Vector2f(400.f, 40.f), sf::Vector2f(game->getSize().x / 2 - 200, game->getSize().y / 2), "Settings", 2));
    addButton(b3 = Button(sf::Vector2f(400.f, 40.f), sf::Vector2f(game->getSize().x / 2 - 200, game->getSize().y / 2 + 60), "Quit Game", 1));
    b3.setColor(sf::Color::Red);
}

void GuiMainMenu::onDraw(sf::RenderWindow& wnd)
{
    b1.draw(wnd);
    b2.draw(wnd);
    b3.draw(wnd);

    sf::Text text = GameDisplay::instance->drawCenteredString("Car Game", 200, sf::Vector2f(GameDisplay::instance->getSize().x / 2, 200), sf::Text::Italic);
    text.setFillColor(sf::Color(100, 0, 0));
    wnd.draw(text);

    Gui::onDraw(wnd);
}

void GuiMainMenu::onClick(long button)
{
    Game* game = Game::instance;

    if(button == 0)
    {
        game->displayGui(new GuiMapSelect); //level selection
        if(game->isNewPlayer && game->tutorialStep == 2)
        {
            game->tutorialStep = 3;
        }
    }

    if(button == 1)
    {
        game->exit(0);
    }

    if(button == 2)
    {
        game->displayGui(new GuiSettings); //settings
    }
}

