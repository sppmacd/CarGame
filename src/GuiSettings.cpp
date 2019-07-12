#include "GuiSettings.h"
#include "Game.h"
#include "GuiMainMenu.h"
#include "GameDisplay.h"
#include "GuiLanguage.hpp"
#include "GuiYesNo.h"
#include <iostream>

using namespace sf;

void GuiSettings::onLoad()
{
    GameDisplay* game = GameDisplay::instance;

    addButton(bDone = Button(Vector2f(400.f, 40.f), Vector2f(game->getSize().x / 2 - 200, game->getSize().y / 2 + 120), Game::instance->translation.get("gui.done"), 0));
    addButton(bResetHS = Button(Vector2f(400.f, 40.f), Vector2f(game->getSize().x / 2 - 200, game->getSize().y / 2 - 30), Game::instance->translation.get("gui.settings.resetgame"), 1));
    addButton(bRefreshGD = Button(Vector2f(400.f, 40.f), Vector2f(game->getSize().x / 2 - 200, game->getSize().y / 2 - 150), Game::instance->translation.get("gui.settings.refreshres"), 2));
    addButton(bVerticalSync = ButtonToggle(Vector2f(400.f, 40.f),::Vector2f(game->getSize().x / 2 - 200, game->getSize().y / 2 - 90), Game::instance->translation.get("gui.settings.verticalsync"), 3, game->getVSync()));
    addButton(bLanguage = Button(Vector2f(400.f, 40.f), Vector2f(game->getSize().x / 2 - 200, game->getSize().y / 2 + 30), Game::instance->translation.get("gui.settings.language"), 4));
    bDone.setColor(sf::Color::Green);
}

void GuiSettings::onDraw(sf::RenderWindow& wnd)
{
    //Gui::drawGui(wnd);

    bDone.draw(wnd);
    bResetHS.draw(wnd);
    bRefreshGD.draw(wnd);
    bVerticalSync.draw(wnd);
    bLanguage.draw(wnd);

    wnd.draw(GameDisplay::instance->drawCenteredString(Game::instance->translation.get("gui.settings.title"), 30, sf::Vector2f(GameDisplay::instance->getSize().x / 2, 200)));

    Gui::onDraw(wnd);
}

void GuiSettings::onDialogFinished(Gui*, int callId)
{
	if(callId == 0 && dialogReturnValue == 1)
	{
		remove("data.txt");
		remove("highscore.txt");
		remove("profile_1.txt");
		Game::instance->loadPlayerData();
		Game::instance->displayGui(new GuiMainMenu);
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
        runDialog(new GuiYesNo(game->translation.get("gui.settings.resetgame.delete")+"\n\n"+game->translation.get("gui.settings.resetgame.warning")), 0); //yes/no
    }
    else if (button == 2)
    {
        GameDisplay::instance->reload();
    }
    else if (button == 3)
    {
        GameDisplay::instance->setVSync(bVerticalSync.getState());
    }
    else if(button == 4)
    {
        game->displayGui(new GuiLanguage);
    }
}

