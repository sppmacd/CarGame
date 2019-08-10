#include "GuiCredits.hpp"
#include "GuiMainMenu.h"
#include "Game.h"
#include "version.hpp"

void GuiCredits::onClick(int id)
{
    if(id == 0)
        guiHandler->displayGui(new GuiMainMenu);
}
void GuiCredits::onResize()
{
    bOk.setPosition(Vector2f(guiHandler->getCenter().x - 200.f, guiHandler->getSize().y - 50.f));
}
void GuiCredits::onLoad()
{
    bOk = Button(this, Vector2f(400.f, 40.f), Vector2f(guiHandler->getCenter().x - 200.f, 0.f), Game::instance->translation.get("gui.done"), 0);
    addWidget(&bOk);
}
void GuiCredits::onDraw(RenderWindow& wnd)
{
    wnd.clear(colors::bgColor);
    bOk.draw(wnd);
    static const string credits
    = string("CG ") + CG_VERSION + "\n"
    + string("Programming: sppmacd\n")
    + string("Graphics: sppmacd + some internet resources\n")
    + string("Sounds: not made yet\n")
    + string("Dependencies: \n")
    + string("- SFML - Simple & Fast Multimedia Library [www.sfml-dev.org]\n")
    + string("- hmutil - Hackerman Utility (datafiles) [included in dependency package]\n")
    ;
    wnd.draw(cg::Gui::drawCenteredString(credits, 15, guiHandler->getCenter()));
}
