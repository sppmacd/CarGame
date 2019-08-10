#include "GuiMainMenu.h"
#include "Game.h"
#include "GameDisplay.h"
#include "GuiSettings.h"
#include "GuiMapSelect.h"
#include <iostream>
#include "GuiCredits.hpp"

using namespace std;

void GuiMainMenu::onLoad()
{
    GameDisplay* game = GameDisplay::instance;

    addWidget(&(bStart = ButtonCircle(this, 180.f, Vector2f(game->getSize().x / 2, game->getSize().y / 2), "gui/start", 0)));
    addWidget(&(bSettings = ButtonCircle(this, 100.f, Vector2f(game->getSize().x / 2 - 380, game->getSize().y / 2), "gui/settings", 2)));
    addWidget(&(bQuit = ButtonCircle(this, 100.f, Vector2f(game->getSize().x / 2 + 380, game->getSize().y / 2), "gui/quit", 1)));
    addWidget(&(sTest = Slider(this, Vector2f(10.f, 10.f), 100.f, 5.f, 100)));
    addWidget(&(bCredits = Button(this, Vector2f(), Vector2f(), "", 3)));

    bStart.setColor(Color::Green);
    bQuit.setColor(Color::Red);
}

void GuiMainMenu::onResize()
{
    GameDisplay* game = GameDisplay::instance;
    bStart.setPosition(Vector2f(game->getSize().x / 2, game->getSize().y / 2));
    bSettings.setPosition(Vector2f(game->getSize().x / 2 - 380, game->getSize().y / 2));
    bQuit.setPosition(Vector2f(game->getSize().x / 2 + 380, game->getSize().y / 2));
    bCredits.setSize(Vector2f(game->getSize().x, 30.f));
    bCredits.setPosition(Vector2f(0.f, game->getSize().y - 30.f));
}

void GuiMainMenu::onDraw(RenderWindow& wnd)
{
    bStart.draw(wnd);
    bQuit.draw(wnd);
    bSettings.draw(wnd);
    //sTest.draw(wnd);

    Sprite sprite(GameDisplay::instance->logoTexture);
    sprite.setOrigin(Vector2f(GameDisplay::instance->logoTexture.getSize() / 2U));
    sprite.setPosition(guiHandler->getSize().x / 2, guiHandler->getSize().y / 7);
    wnd.draw(sprite);

    GameDisplay::loadingStr = "Loading game engine...";
    Text credits = drawString(string(CG_VERSION) + " - " + Game::instance->translation.get("gui.mainmenu.credit"), 20, Vector2f(10.f, GameDisplay::instance->getSize().y - 30.f));

    wnd.draw(credits);

    Gui::onDraw(wnd);
}

void GuiMainMenu::onClick(int button)
{
    Game* game = Game::instance;

    if(button == 0)
    {
        game->displayGui(new GuiMapSelect);
        if(game->isNewPlayer && game->tutorialStep == 2)
        {
            game->tutorialStep = 3;
        }
    }
    else if(button == 1)
    {
        game->exit(0);
    }
    else if(button == 2)
    {
        runDialog(new GuiSettings, -1);
    }
    else if(button == 3)
    {
        game->displayGui(new GuiCredits);
    }
}
