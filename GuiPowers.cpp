#include "GuiPowers.h"
#include "Game.h"
#include "GameDisplay.h"
#include "GuiMapSelect.h"
#include <iostream>

using namespace std;
using namespace sf;

void GuiPowers::onLoad()
{
    cooldown = 0;
    GameDisplay* game = GameDisplay::instance;

    addButton(bPower1 = Button(Vector2f(400.f, 40.f), Vector2f(game->getSize().x / 2 - 200, game->getSize().y / 2 - 120), "Buy Oil: 400$", 1));
    addButton(bPower2 = Button(Vector2f(400.f, 40.f), Vector2f(game->getSize().x / 2 - 200, game->getSize().y / 2 - 60), "Buy Ice: 1000$", 2));

    addButton(bReturn = Button(Vector2f(400.f, 40.f), Vector2f(game->getSize().x / 2 - 200, game->getSize().y / 2 + 60), "Return to Maps", 0));
}

void GuiPowers::onDraw(sf::RenderWindow& wnd)
{
    Game* game = Game::instance;

    if(game->getCoins() < 400 || cooldown > 0)
    {
        bPower1.enabled = false;
    }
    else
    {
        bPower1.enabled = true;
    }

    if(game->getCoins() < 1000 || cooldown > 0)
    {
        bPower2.enabled = false;
    }
    else
    {
        bPower2.enabled = true;
    }

    bPower1.draw(wnd);
    bPower2.draw(wnd);
    bReturn.draw(wnd);

    wnd.draw(GameDisplay::instance->drawCenteredString("Buy Powers", 30, sf::Vector2f(GameDisplay::instance->getSize().x / 2, 200)));

    wnd.draw(drawString(to_string(game->powers[1]), 30, Vector2f(GameDisplay::instance->getSize().x / 2 + 300, GameDisplay::instance->getSize().y / 2 - 120)));
    wnd.draw(drawString(to_string(game->powers[2]), 30, Vector2f(GameDisplay::instance->getSize().x / 2 + 300, GameDisplay::instance->getSize().y / 2 - 60)));

    if(cooldown > 0) cooldown--;

    Gui::onDraw(wnd);
}

void GuiPowers::onClick(long button)
{
    Game* game = Game::instance;

    if(button == 0)
    {
        game->displayGui(new GuiMapSelect); //map selection
    }

    if(button == 1)
    {
        game->getPower(1);
        game->removeCoins(400);
        cooldown = 125;
    }

    if(button == 2)
    {
        game->getPower(2);
        game->removeCoins(1000);
        cooldown = 125;
    }
}

