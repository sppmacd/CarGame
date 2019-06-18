#include "GuiLanguage.hpp"

void GuiLanguage::onLoad()
{
    addButton(bCancel = Button(Vector2f(400.f, 40.f), Vector2f(GameDisplay::instance->getSize().x / 2 -  200.f, GameDisplay::instance->getSize().y / 2 + 200.f),
                               Game::instance->translation.get("gui.cancel"), 0));
    addButton(bDone = Button(Vector2f(400.f, 40.f), Vector2f(GameDisplay::instance->getSize().x / 2 -  200.f, GameDisplay::instance->getSize().y / 2 + 200.f),
                               Game::instance->translation.get("gui.done"), 1));
    addButton(bUp = Button(Vector2f(400.f, 40.f), Vector2f(GameDisplay::instance->getSize().x / 2 -  200.f, GameDisplay::instance->getSize().y / 2 + 200.f),
                               Game::instance->translation.get("gui.up"), 2));
    addButton(bDown = Button(Vector2f(400.f, 40.f), Vector2f(GameDisplay::instance->getSize().x / 2 -  200.f, GameDisplay::instance->getSize().y / 2 + 200.f),
                               Game::instance->translation.get("gui.down"), 3));

    // TODO

    current = 0;
}
void GuiLanguage::onDraw(RenderWindow& wnd)
{

}
void GuiLanguage::onClick(long buttonId)
{
    switch()
}
