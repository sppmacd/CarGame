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
            data->cost = data->power->getCost();
            data->count = Game::instance->powers[s.first];
            powerData.push_back(data);
            addButton(data->bBuyPower = Button(Vector2f(400.f, 40.f), Vector2f(game->getSize().x / 2 - 200.f, s.first * 50.f + game->getSize().y / 4),
                                    Game::instance->translation.get("gui.powers.buy", {
                                    Game::instance->translation.get("power." + data->power->getName()), String(to_string(data->cost))
                                    }), s.first + 100));
        }
    }

    addButton(bReturn = Button(Vector2f(400.f, 40.f), Vector2f(game->getSize().x / 2 - 200, (Game::instance->biggestPlayerPowerID + 1) * 50.f + game->getSize().y / 4),
                               Game::instance->translation.get("gui.powers.return"), 0));
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
            data->bBuyPower.enabled = false;
        }
        else
        {
            data->bBuyPower.enabled = true;
        }
        data->bBuyPower.draw(wnd);
        wnd.draw(drawString(to_string(data->count), 30, Vector2f(data->bBuyPower.getPos()) + Vector2f(420.f, 0.f)));
    }
    bReturn.draw(wnd);

    wnd.draw(GameDisplay::instance->drawCenteredString(Game::instance->translation.get("gui.powers.title"), 30, sf::Vector2f(GameDisplay::instance->getSize().x / 2, 200)));
    if(cooldown > 0)
        cooldown--;

    Gui::onDraw(wnd);
}

void GuiPowers::onClick(long button)
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
            if(data->bBuyPower.id == button)
            {
                if(unsigned(game->getCoins()) >= data->cost)
                {
                    game->removeCoins(data->cost);
                    game->getPower(powerId);
                    data->count++;
                    cooldown = 30;
                }
            }
        }
    }
}

