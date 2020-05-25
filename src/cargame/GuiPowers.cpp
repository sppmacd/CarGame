#include "GuiPowers.h"
#include "Game.h"
#include "GameDisplay.h"
#include "GuiShop.hpp"
#include <iostream>
#include <sstream>
#include <iomanip>

using namespace std;
using namespace sf;

void GuiPowers::onLoad()
{
    cooldown = 0;
    GameDisplay* game = GameDisplay::instance;
    Game::instance->usablePowerIds.clear();

    for(auto s: Game::instance->gpo.powers.arr())
    {
        // powers > 100 -> functional powers (e.g anti-powers)
        if(s.first.baseId != 0 && s.first.baseId < 100)
        {
            PowerData* data = new PowerData;
            data->power = s.second;
            data->cost = Game::instance->playerData.powerLevels[s.first.baseId].getUpgradeCost();
            data->count = Game::instance->playerData.powerLevels[s.first.baseId].getLevel();
            // data->level = Game::instance->power; // 0.2
            powerData.push_back(data);

            data->bBuyPower = Button(this, Vector2f(550.f, 40.f), Vector2f(game->getSize().x / 2 - 300.f, s.first.baseId * 50.f + game->getSize().y / 4), "", s.first.baseId + 100);
            data->bEquipPower = ButtonImage(this, "power/" + to_string(s.first.baseId), Vector2f(40.f, 40.f), Vector2f(), "E", s.first.baseId + 200);

            if(data->count > 0)
            {
                data->bEquipPower.setEnabled(!Game::instance->isPowerEquipped(s.first.baseId));
            }
            else
            {
                data->bEquipPower.setEnabled(false);
            }

            if(data->count == 5)
            {
                data->bBuyPower.setText(Game::instance->translation.get("gui.powers.maxlvl", {
                        Game::instance->translation.get("power." + data->power->getName())
                        }));
                data->bBuyPower.setEnabled(false);
            }
            else
            {
                data->bBuyPower.setText(Game::instance->translation.get("gui.powers.upgrade", {
                        Game::instance->translation.get("power." + data->power->getName()), String(to_string(data->cost)) }
                        ));
            }

            addWidget(&(data->bBuyPower));
            addWidget(&(data->bEquipPower));
        }
    }

    for(size_t s = 0; s < equippedPowerIds.size(); s++)
    {
        EquippedPowerData& epd = equippedPowerIds[s];
        epd.powerId = Game::instance->playerData.equipment[s];
        epd.bImg = ButtonImage(this, epd.powerId == 0 ? "stat/mpl" : ("power/" + to_string(epd.powerId)), Vector2f(40.f, 40.f), Vector2f(), "", 300 + s);
        addWidget(&epd.bImg);
    }

    addWidget(&(bReturn = Button(this, Vector2f(400.f, 40.f), Vector2f(0, 0),
                               Game::instance->translation.get("gui.powers.return"), 0)));
}
void GuiPowers::onResize()
{
    GameDisplay* game = GameDisplay::instance;
    bReturn.setPosition(Vector2f(game->getSize().x / 2 - 200, powerData.size() * 50.f + 470.f));
    for(PowerData* data: powerData)
    {
        data->bBuyPower.setPosition(Vector2f(game->getSize().x / 2 - 300.f, (data->bBuyPower.getID() - 100) * 50.f + 200.f));
        data->bEquipPower.setPosition(Vector2f(game->getSize().x / 2 + 260.f, (data->bBuyPower.getID() - 100) * 50.f + 200.f));
    }
    for(size_t s = 0; s < equippedPowerIds.size(); s++)
    {
        equippedPowerIds[s].bImg.setPosition(Vector2f(game->getSize().x / 2 - 45.f + s * 50.f, powerData.size() * 50.f + 100.f + 200.f));
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

    // powers
    for(PowerData* data: powerData)
    {
        // check which buttons should be enabled
        if(unsigned(game->playerData.playerCoins) < data->cost || cooldown > 0)
        {
            data->bBuyPower.setEnabled(false);
        }
        else
        {
            data->bBuyPower.setEnabled(true);
        }
        if(data->count > 0)
        {
            data->bEquipPower.setEnabled(!Game::instance->isPowerEquipped(data->bBuyPower.getID() - 100));
        }
        else
        {
            data->bEquipPower.setEnabled(false);
        }

        // create power info string.
        ostringstream oss;
        oss << setiosflags(std::ios::fixed) << setprecision(1) << data->power->maxPowerTime / 60.f;
        data->bBuyPower.draw(wnd);
        data->bEquipPower.draw(wnd);

        // draw power info
        if(data->count == 0)
            wnd.draw(drawString(Game::instance->translation.get("gui.powers.notbought"), 15, Vector2f(data->bBuyPower.getPosition()) + Vector2f(620.f, 0.f)));
        else
            wnd.draw(drawString(Game::instance->translation.get("gui.powers.powerinfo", {to_string(data->count), oss.str()}), 15, Vector2f(data->bBuyPower.getPosition()) + Vector2f(620.f, 0.f)));
    }

    // equipped powers
    wnd.draw(GameDisplay::instance->drawCenteredString(Game::instance->translation.get("gui.powers.equippedPowers"), 20, Vector2f(game->getSize().x / 2, powerData.size() * 50.f + 275.f)));

    for(size_t s = 0; s < equippedPowerIds.size(); s++)
    {
        equippedPowerIds[s].bImg.draw(wnd);
    }

    // return button
    bReturn.draw(wnd);

    // menu title
    wnd.draw(GameDisplay::instance->drawCenteredString(Game::instance->translation.get("gui.powers.title"), 30, sf::Vector2f(GameDisplay::instance->getSize().x / 2, 150)));
    wnd.draw(GameDisplay::instance->drawCenteredString(Game::instance->translation.get("gui.powers.tip"), 15, sf::Vector2f(GameDisplay::instance->getSize().x / 2, 190)));
    if(cooldown > 0)
        cooldown--;

    // power description
    for(size_t s = 0; s < powerData.size(); s++)
    {
        PowerData* data = powerData[s];
        Vector2f scaledPos = game->getWindow()->mapPixelToCoords(Mouse::getPosition(*game->getWindow()));
        bool bEquipClicked=data->bEquipPower.isClicked(scaledPos), bBuyClicked=data->bBuyPower.isClicked(scaledPos);
        if(bEquipClicked || bBuyClicked)
        {
            wnd.draw(drawCenteredString(Game::instance->translation.get("power." + data->power->getName() + ".desc"), 20, Vector2f(Game::instance->getSize().x / 2, powerData.size() * 50.f + 360.f)));
            wnd.draw(drawCenteredString(Game::instance->translation.get("power." + data->power->getName() + ".upgr"), 20, Vector2f(Game::instance->getSize().x / 2, powerData.size() * 50.f + 400.f)));
            wnd.draw(drawCenteredString(Game::instance->translation.get("gui.powers.equip"), 20, Vector2f(Game::instance->getSize().x / 2, powerData.size() * 50.f + 440.f), Text::Bold));
            break; //only one string at once can be displayed.
        }
    }

    Gui::onDraw(wnd);
}
void GuiPowers::onClose()
{
    for(size_t s = 0; s < equippedPowerIds.size(); s++)
    {
        if(equippedPowerIds[s].powerId != 0)
            Game::instance->usablePowerIds.push_back(equippedPowerIds[s].powerId);
    }
}
void GuiPowers::onClick(int button)
{
    Game* game = Game::instance;

    if(button == 0)
    {
        game->displayGui(new GuiShop);
    }
    else if(button > 100 && button <= 200)
    {
        int powerId = button - 100;
        for(PowerData* data: powerData)
        {
            if(data->bBuyPower.getID() == button)
            {
                if(unsigned(game->playerData.playerCoins) >= data->cost)
                {
                    if(game->getPower(powerId))
                    {
                        data->cost = Game::instance->playerData.powerLevels[powerId].getUpgradeCost();
                        data->count++;

                        if(data->count == 5)
                        {
                            data->bBuyPower.setText(Game::instance->translation.get("gui.powers.maxlvl", {
                                Game::instance->translation.get("power." + data->power->getName())
                                }));
                            data->bBuyPower.setEnabled(false);
                        }
                        else
                        {
                            data->bBuyPower.setText(Game::instance->translation.get("gui.powers.upgrade", {
                                    Game::instance->translation.get("power." + data->power->getName()), String(to_string(data->cost)) }
                                    ));
                        }

                        cooldown = 30;
                        game->savePlayerData();
                    }
                }
            }
        }
    }
    else if(button > 200 && button < 300)
    {
        int powerId = button - 200;
        for(size_t s = 0; s < equippedPowerIds.size(); s++)
        {
            if(equippedPowerIds[s].powerId == 0)
            {
                equippedPowerIds[s].powerId = powerId;
                game->playerData.equipment[s] = powerId;
                game->savePlayerData();
                equippedPowerIds[s].bImg = ButtonImage(this, "power/" + to_string(powerId), Vector2f(40.f, 40.f), Vector2f(), "", equippedPowerIds[s].bImg.getID());
                equippedPowerIds[s].bImg.setPosition(Vector2f(game->getSize().x / 2 - 45.f + s * 50.f, powerData.size() * 50.f + 300.f));
                break;
            }
        }
    }
    else if(button >= 300)
    {
        int slotId = button - 300;
        equippedPowerIds[slotId].powerId = 0;
        equippedPowerIds[slotId].bImg = ButtonImage(this, "stat/mpl", Vector2f(40.f, 40.f), Vector2f(), "", equippedPowerIds[slotId].bImg.getID());
        equippedPowerIds[slotId].bImg.setPosition(Vector2f(game->getSize().x / 2 - 45.f + slotId * 50.f, powerData.size() * 50.f + 300.f));
        equippedPowerIds[slotId].bImg.setEnabled(false);
        game->playerData.equipment[slotId] = 0;
        game->savePlayerData();
    }
}
void GuiPowers::onMouseMove(sf::Vector2f vec)
{
    Gui::onMouseMove(vec);
}

