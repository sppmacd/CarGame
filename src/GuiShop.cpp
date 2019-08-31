#include "GuiShop.hpp"
#include "GuiMapSelect.h"
#include "GuiPowers.h"
#include "Game.h"

void GuiShop::onLoad()
{
    bReturn = cg::Button(this, Vector2f(400.f, 40.f), Vector2f(), Game::instance->translation.get("gui.done"), 0);
    bPowers = cg::Button(this, Vector2f(400.f, 40.f), Vector2f(), Game::instance->translation.get("gui.shop.labels.power"), 1);
    addWidget(&bReturn);
    addWidget(&bPowers);

    int id = 0;
    for(pair<const int, AbilityBase*>& base: Game::instance->abilities.abilities)
    {
        GuiShop::AbilityData* data = new GuiShop::AbilityData{*base.second};
        data->id = base.first;
        data->cost = Game::instance->abilities.calculateCost(base.first);
        data->value = Game::instance->abilities.calculateValue(base.first);
        data->bUpgrade = cg::Button(this, Vector2f(450.f, 40.f), Vector2f(),
                                    Game::instance->translation.get("gui.shop.upgrade_ability",
                                    {
                                        Game::instance->translation.get("ability." + data->base.getName()),
                                        to_string(data->cost)
                                    }
                                ), id + 100);
        data->level = Game::instance->abilities.getAbilityLevel(data->id);
        abilityData.push_back(data);
        addWidget(&data->bUpgrade);
        id++;
    }
}
GuiShop::~GuiShop()
{
    for(AbilityData* data: abilityData)
    {
        delete data;
    }
}
void GuiShop::onResize()
{
    for(AbilityData* data: abilityData)
    {
        data->bUpgrade.setPosition(Vector2f(guiHandler->getCenter().x - 225.f, (data->bUpgrade.getID() - 101) * 50.f + guiHandler->getCenter().y / 2.f));
    }
    bPowers.setPosition(Vector2f(guiHandler->getCenter().x - 200.f, abilityData.size() * 50.f + guiHandler->getCenter().y / 2.f - 30.f));
    bReturn.setPosition(Vector2f(guiHandler->getCenter().x - 200.f, abilityData.size() * 50.f + guiHandler->getCenter().y / 2.f + 20.f));
}
void GuiShop::onClick(int buttonId)
{
    if(buttonId == 0)
        guiHandler->displayGui(new GuiMapSelect);
    else if(buttonId == 1)
        guiHandler->displayGui(new GuiPowers);
    else if(buttonId >= 100)
    {
        int index = buttonId - 100;

        AbilityData* data = abilityData[index];
        do
        {
            bool b = Game::instance->abilities.upgradeAbility(Game::instance, data->id);
            if(!b)
                break;
            data->cost = Game::instance->abilities.calculateCost(data->id);
            data->value = Game::instance->abilities.calculateValue(data->id);
            data->level++;
            data->bUpgrade.setText(Game::instance->translation.get("gui.shop.upgrade_ability",
                                        {
                                            Game::instance->translation.get("ability." + data->base.getName()),
                                            to_string(data->cost)
                                        }));
        } while(0);
    }
}
void GuiShop::onDraw(RenderWindow& wnd)
{
    wnd.draw(drawCenteredString(Game::instance->translation.get("gui.shop.title"), 30, Vector2f(guiHandler->getCenter().x, guiHandler->getCenter().y / 2.f - 150.f)));
    wnd.draw(drawCenteredString(Game::instance->translation.get("gui.shop.labels.ability"), 20, Vector2f(guiHandler->getCenter().x, guiHandler->getCenter().y / 2.f - 90.f)));
    bPowers.draw(wnd);
    bReturn.draw(wnd);
    for(AbilityData* data: abilityData)
    {
        if(data->cost <= Game::instance->getCoins())
        {
            data->bUpgrade.setEnabled(true);
        }
        else
        {
            data->bUpgrade.setEnabled(false);
        }
        data->bUpgrade.draw(wnd);
        wnd.draw(drawString(Game::instance->translation.get("gui.shop.ability", {to_string(data->level), to_string(data->value)}), 15, data->bUpgrade.getPosition() + Vector2f(465.f, 0.f)));
    }
}
