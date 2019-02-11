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

    addButton(bStart = Button(sf::Vector2f(400.f, 40.f), sf::Vector2f(game->getSize().x / 2 - 200, game->getSize().y / 2 - 60), "New Game", 0));
    addButton(bSettings = Button(sf::Vector2f(400.f, 40.f), sf::Vector2f(game->getSize().x / 2 - 200, game->getSize().y / 2), "Settings", 2));
    addButton(bQuit = Button(sf::Vector2f(400.f, 40.f), sf::Vector2f(game->getSize().x / 2 - 200, game->getSize().y / 2 + 60), "Quit Game", 1));

    /*
    addButton(bStart = ButtonImage("gui/start", sf::Vector2f(320.f, 320.f), sf::Vector2f(game->getSize().x / 2 - 160, game->getSize().y / 2 - 160.f + 60.f), "New Game", 0));
    addButton(bQuit = ButtonImage("gui/quit", sf::Vector2f(200.f, 200.f), sf::Vector2f(game->getSize().x / 2 + 260, game->getSize().y / 2 - 100.f + 60.f), "Settings", 1));
    addButton(bSettings = ButtonImage("gui/settings", sf::Vector2f(200.f, 200.f), sf::Vector2f(game->getSize().x / 2 - 460, game->getSize().y / 2 - 100.f + 60.f), "Quit Game", 2));
    */
    bStart.setColor(Color::Green);
    bQuit.setColor(Color::Red);
    bSettings.setColor(Color::Blue);
}

void GuiMainMenu::onDraw(sf::RenderWindow& wnd)
{
    bStart.draw(wnd);
    bQuit.draw(wnd);
    bSettings.draw(wnd);

    sf::Text text = GameDisplay::instance->drawCenteredString("Car Destroyer", 150, sf::Vector2f(GameDisplay::instance->getSize().x / 2, 200), sf::Text::Italic);
    text.setFillColor(sf::Color(100, 0, 0));
    wnd.draw(text);

    Text credits = drawString("v0.1 - Sppmacd 2018. Powered by SFML.", 20, Vector2f(10.f, GameDisplay::instance->getSize().y - 60.f));
    wnd.draw(credits);

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

