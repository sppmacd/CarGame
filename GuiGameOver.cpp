#include "GuiGameOver.h"
#include "Game.h"
#include "GameDisplay.h"
#include <iostream>

using namespace std;

Button GuiGameOver::b1;
Button GuiGameOver::bMainMenu;

void GuiGameOver::onLoad()
{
    GameDisplay* game = GameDisplay::instance;

    addButton(b1 = Button(sf::Vector2f(400.f, 40.f), sf::Vector2f(game->getSize().x / 2 - 200, game->getSize().y / 2), "New Game", 0));
    addButton(bMainMenu = Button(sf::Vector2f(400.f, 40.f), sf::Vector2f(game->getSize().x / 2 - 200, game->getSize().y / 2 + 60), "Return to Menu", 1));

    b1.setColor(sf::Color::Green);
    bMainMenu.setColor(sf::Color::Red);
}

void GuiGameOver::draw(sf::RenderWindow* wnd)
{
    //Gui::drawGui(wnd);

    b1.draw(wnd);
    bMainMenu.draw(wnd);

    Game* game = Game::instance;

    sf::Text gameover = GameDisplay::instance->drawCenteredString("Game Over!", 100, sf::Vector2f(GameDisplay::instance->getSize().x / 2, GameDisplay::instance->getSize().y / 2 - 250), sf::Text::Italic);
    gameover.setFillColor(sf::Color(100, 0, 0));
    wnd->draw(gameover);

    sf::Text sc = GameDisplay::instance->drawCenteredString(std::string("Your score was ") + to_string(game->lastTickScore), 40, sf::Vector2f(GameDisplay::instance->getSize().x / 2, GameDisplay::instance->getSize().y / 2 - 70));
    sc.setFillColor(sf::Color::Yellow);
    wnd->draw(sc);
}

void GuiGameOver::onButtonClicked(long button)
{
    Game* game = Game::instance;
    if(button == 0)
    {
        game->loadGame();
    }
    else if(button == 1)
    {
        game->displayGui(2); //main menu
    }
}
