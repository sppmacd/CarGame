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

    for(auto s: Game::instance->powerRegistry)
    {
        // powers > 100 -> functional powers (e.g anti-powers)
        if(s.first != 0 && s.first < 100)
        {
            PowerData* data = new PowerData;
            data->power = s.second;
            data->cost = Game::instance->powers[s.first].getUpgradeCost();
            data->count = Game::instance->powers[s.first].getLevel();
            // data->level = Game::instance->power; // 0.2
            powerData.push_back(data);
            addWidget(&(data->bBuyPower = Button(this, Vector2f(400.f, 40.f), Vector2f(game->getSize().x / 2 - 200.f, s.first * 50.f + game->getSize().y / 4),
                                    Game::instance->translation.get("gui.powers.upgrade", {
                                    Game::instance->translation.get("power." + data->power->getName()), String(to_string(data->cost))
                                    }), s.first + 100)));
        }
    }

    addWidget(&(bReturn = Button(this, Vector2f(400.f, 40.f), Vector2f(game->getSize().x / 2 - 200, game->getSize().y * 3 / 4),
                               Game::instance->translation.get("gui.powers.return"), 0)));
}
void GuiPowers::onResize()
{
    GameDisplay* game = GameDisplay::instance;
    bReturn.setPosition(Vector2f(game->getSize().x / 2 - 200, game->getSize().y * 3 / 4));
    for(PowerData* data: powerData)
    {
        data->bBuyPower.setPosition(Vector2f(game->getSize().x / 2 - 200.f, (data->bBuyPower.getID() - 100) * 50.f + game->getSize().y / 4));
    }
}
GuiPowers::~GuiPowers()
{
    for(PowerData* data: powerData)
    {
        delete data;
    }
}
void GuiPowers::onDraw(sf::RenderWindow& wnd)
{
    Game* game = Game::instance;

    for(PowerData* data: powerData)
    {
        if(unsigned(game->getCoins()) < data->cost || cooldown > 0)
        {
            data->bBuyPower.setEnabled(false);
        }
        else
        {
            data->bBuyPower.setEnabled(true);
        }
        data->bBuyPower.draw(wnd);
        if(data->count == 0)
            wnd.draw(drawString(Game::instance->translation.get("gui.powers.notbought"), 30, Vector2f(data->bBuyPower.getPosition()) + Vector2f(420.f, 0.f)));
        else
            wnd.draw(drawString(Game::instance->translation.get("gui.powers.powerlvl", {to_string(data->count)}), 30, Vector2f(data->bBuyPower.getPosition()) + Vector2f(420.f, 0.f)));
    }
    bReturn.draw(wnd);

    wnd.draw(GameDisplay::instance->drawCenteredString(Game::instance->translation.get("gui.powers.title"), 30, sf::Vector2f(GameDisplay::instance->getSize().x / 2, 200)));
    if(cooldown > 0)
        cooldown--;

    Gui::onDraw(wnd);
}

void GuiPowers::onClick(int button)
{
    Game* game = Game::instance;

    if(button == 0)
    {
        game->displayGui(new GuiMapSelect);
    }
    else if(button > 100)
    {
        int powerId = button - 100;
        for(PowerData* data: powerData)
        {
            if(data->bBuyPower.getID() == button)
            {
                if(unsigned(game->getCoins()) >= data->cost)
                {
                    if(game->getPower(powerId))
                    {
                        data->cost = Game::instance->powers[powerId].getUpgradeCost();
                        data->count++;
                        cooldown = 30;
                    }
                }
            }
        }
    }
}

