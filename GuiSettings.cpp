#include "GuiSettings.h"
#include "Game.h"
#include "GuiMainMenu.h"
#include "GameDisplay.h"
#include "GuiYesNo.h"
#include <iostream>

using namespace sf;

void GuiSettings::onLoad()
{
    GameDisplay* game = GameDisplay::instance;

    addButton(bDone = Button(Vector2f(400.f, 40.f), Vector2f(game->getSize().x / 2 - 200, game->getSize().y / 2 + 120), "Done", 0));
    addButton(bResetHS = Button(Vector2f(400.f, 40.f), Vector2f(game->getSize().x / 2 - 200, game->getSize().y / 2 + 30), "Reset Game", 1));
    addButton(bRefreshGD = Button(Vector2f(400.f, 40.f), Vector2f(game->getSize().x / 2 - 200, game->getSize().y / 2 - 90), "Refresh Resources", 2));
    addButton(bVerticalSync = ButtonToggle(Vector2f(400.f, 40.f),::Vector2f(game->getSize().x / 2 - 200, game->getSize().y / 2 - 30), "Vertical Sync", 3, game->getVSync()));
    //addButton(bTFM = ButtonToggle(sf::Vector2f(400.f, 40.f), sf::Vector2f(game->getRenderWnd()->getSize().x / 2 - 200, game->getRenderWnd()->getSize().y / 2 + 90), "Toggle Fullscreen Mode", 4));

    bDone.setColor(sf::Color::Green);
}

void GuiSettings::onDraw(sf::RenderWindow& wnd)
{
    //Gui::drawGui(wnd);

    bDone.draw(wnd);
    bResetHS.draw(wnd);
    bRefreshGD.draw(wnd);
    bVerticalSync.draw(wnd);
	//bTFM.draw(wnd);

    wnd.draw(GameDisplay::instance->drawCenteredString("Settings", 30, sf::Vector2f(GameDisplay::instance->getSize().x / 2, 200)));

    Gui::onDraw(wnd);
}

void GuiSettings::onDialogFinished(Gui* dialog, int callId)
{
	if(callId == 0 && dialogReturnValue == 1)
	{
		remove("data.txt");
		remove("highscore.txt");
		Game::instance->loadPlayerData();
	}
}

void GuiSettings::onClick(long button)
{
    Game* game = Game::instance;

    if (button == 0)
    {
        game->displayGui(new GuiMainMenu); //main menu
    }
    else if (button == 1)
    {
        runDialog(new GuiYesNo("Are you sure to delete all your player data?\n\nThis can't be undone!"), 0); //yes/no
    }
    else if (button == 2)
    {
        GameDisplay::instance->reload();
    }
    else if (button == 3)
    {
        GameDisplay::instance->setVSync(bVerticalSync.getState());
    }
    else if (button == 4)
    {
        GameDisplay::instance->nextFullscreenMode();
    }
}

