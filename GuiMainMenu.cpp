#include "GuiMainMenu.h"
#include "Game.h"
#include "GameDisplay.h"
#include "GuiSettings.h"
#include "GuiMapSelect.h"
#include <iostream>

using namespace std;

void GuiMainMenu::onLoad()
{
    GameDisplay* game = GameDisplay::instance;

    addButton(bStart = ButtonCircle(180.f, Vector2f(game->getSize().x / 2, game->getSize().y / 2), "gui/start", 0));
    addButton(bSettings = ButtonCircle(100.f, Vector2f(game->getSize().x / 2 - 380, game->getSize().y / 2), "gui/settings", 2));
    addButton(bQuit = ButtonCircle(100.f, Vector2f(game->getSize().x / 2 + 380, game->getSize().y / 2), "gui/quit", 1));

    bStart.setColor(Color::Green);
    bQuit.setColor(Color::Red);
}

void GuiMainMenu::onDraw(RenderWindow& wnd)
{
    bStart.draw(wnd);
    bQuit.draw(wnd);
    bSettings.draw(wnd);

    Text text = GameDisplay::instance->drawCenteredString("Car Destroyer", 150, Vector2f(GameDisplay::instance->getSize().x / 2, 125), Text::Italic);
    text.setFillColor(Color(100, 0, 0));
    wnd.draw(text);

    Text credits = drawString("v0.1 - " + Game::instance->translation.get("gui.mainmenu.credit"), 20, Vector2f(10.f, GameDisplay::instance->getSize().y - 30.f));

    wnd.draw(credits);

    Gui::onDraw(wnd);
}

void GuiMainMenu::onClick(long button)
{
    Game* game = Game::instance;

    if(button == 0)
    {
        game->displayGui(new GuiMapSelect); //level selection
        if(game->isNewPlayer && game->tutorialStep == 2)
        {
            game->tutorialStep = 3;
        }
    }

    if(button == 1)
    {
        game->exit(0);
    }

    if(button == 2)
    {
        game->displayGui(new GuiSettings); //settings
    }
}

