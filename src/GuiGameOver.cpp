#include "GuiGameOver.h"
#include "Game.h"
#include "GameDisplay.h"
#include <iostream>
#include "GuiMainMenu.h"

using namespace std;
using namespace sf;

void GuiGameOver::onLoad()
{
    GameDisplay* game = GameDisplay::instance;
    Game* game2 = Game::instance;

    addWidget(&(b1 = Button(this, Vector2f(400.f, 40.f), Vector2f(game->getSize().x / 2 - 200, game->getSize().y / 2), game2->translation.get("gui.gameover.newgame"), 0)));
    addWidget(&(bMainMenu = Button(this, Vector2f(400.f, 40.f), Vector2f(game->getSize().x / 2 - 200, game->getSize().y / 2 + 60), game2->translation.get("gui.gameover.return"), 1)));

    b1.setColor(sf::Color::Green);
    bMainMenu.setColor(sf::Color::Red);
}

void GuiGameOver::onDraw(sf::RenderWindow& wnd)
{
    //Gui::drawGui(wnd);

    b1.draw(wnd);
    bMainMenu.draw(wnd);

    Game* game = Game::instance;

    sf::Text gameover = GameDisplay::instance->drawCenteredString(game->translation.get("gui.gameover.title"), 100, sf::Vector2f(GameDisplay::instance->getSize().x / 2, GameDisplay::instance->getSize().y / 2 - 250), sf::Text::Italic);
    gameover.setFillColor(sf::Color(100, 0, 0));
    wnd.draw(gameover);

    sf::Text sc = GameDisplay::instance->drawCenteredString(game->translation.get("gui.gameover.score", {to_string(game->lastTickScore)}), 40, sf::Vector2f(GameDisplay::instance->getSize().x / 2, GameDisplay::instance->getSize().y / 2 - 70));
    sc.setFillColor(sf::Color::Yellow);
    wnd.draw(sc);

    Gui::onDraw(wnd);
}

void GuiGameOver::onClick(int button)
{
    Game* game = Game::instance;
    if(button == 0)
    {
        game->loadGame();
    }
    else if(button == 1)
    {
        game->displayGui(new GuiMainMenu); //main menu
    }
}
