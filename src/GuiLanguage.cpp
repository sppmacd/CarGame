#include "GuiLanguage.hpp"
#include "GuiSettings.h"
#include "GameDisplay.h"
#include "Game.h"

void GuiLanguage::onLoad()
{
    addWidget(&(bCancel = Button(this, Vector2f(400.f, 40.f), Vector2f(GameDisplay::instance->getSize().x / 2 -  200.f, GameDisplay::instance->getSize().y / 2 + 200.f),
                               Game::instance->translation.get("gui.cancel"), 0)));
    addWidget(&(bDone = Button(this, Vector2f(400.f, 40.f), Vector2f(GameDisplay::instance->getSize().x / 2 -  200.f, GameDisplay::instance->getSize().y / 2 + 150.f),
                               Game::instance->translation.get("gui.done"), 1)));
    addWidget(&(bUp = Button(this, Vector2f(400.f, 40.f), Vector2f(GameDisplay::instance->getSize().x / 2 -  200.f, GameDisplay::instance->getSize().y / 2 + 100.f),
                               Game::instance->translation.get("gui.up"), 2)));
    addWidget(&(bDown = Button(this, Vector2f(400.f, 40.f), Vector2f(GameDisplay::instance->getSize().x / 2 -  200.f, GameDisplay::instance->getSize().y / 2 + 50.f),
                               Game::instance->translation.get("gui.down"), 3)));

    // TODO

    current = 0;
}
void GuiLanguage::onDraw(RenderWindow& wnd)
{
    // TODO
    bCancel.draw(wnd);
    bDone.draw(wnd);
    bUp.draw(wnd);
    bDown.draw(wnd);

    wnd.draw(GameDisplay::instance->drawCenteredString(Game::instance->translation.get("gui.language.title"), 30, Vector2f(GameDisplay::instance->getSize().x / 2, 200.f)));
    wnd.draw(GameDisplay::instance->drawCenteredString("(Currently only reloading language settings)", 30, Vector2f(GameDisplay::instance->getSize().x / 2, 250.f)));
}
void GuiLanguage::onClick(int buttonId)
{
    // TODO
    switch(buttonId)
    {
    case 0:
        Game::instance->displayGui(new GuiSettings);
        break;
    case 1:
        Game::instance->loadLanguages();
        break;
    }
}
