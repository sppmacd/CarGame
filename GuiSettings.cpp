#include "GuiSettings.h"
#include "Game.h"
#include "GameDisplay.h"
#include "GuiYesNo.h"
#include <iostream>

Button GuiSettings::bDone;
Button GuiSettings::bResetHS;
Button GuiSettings::bRefreshGD;
ButtonToggle GuiSettings::bVerticalSync;
Button GuiSettings::bTFM;

void GuiSettings::onLoad()
{
    GameDisplay* game = GameDisplay::instance;

    addButton(bDone = Button(sf::Vector2f(400.f, 40.f), sf::Vector2f(game->getRenderWnd()->getSize().x / 2 - 200, game->getRenderWnd()->getSize().y / 2 + 120), "Done", 0));
    addButton(bResetHS = Button(sf::Vector2f(400.f, 40.f), sf::Vector2f(game->getRenderWnd()->getSize().x / 2 - 200, game->getRenderWnd()->getSize().y / 2 + 30), "Reset Game", 1));
    addButton(bRefreshGD = Button(sf::Vector2f(400.f, 40.f), sf::Vector2f(game->getRenderWnd()->getSize().x / 2 - 200, game->getRenderWnd()->getSize().y / 2 - 90), "Refresh Resources", 2));
    addButton(bVerticalSync = ButtonToggle(sf::Vector2f(400.f, 40.f), sf::Vector2f(game->getRenderWnd()->getSize().x / 2 - 200, game->getRenderWnd()->getSize().y / 2 - 30), "Vertical Sync", 3, game->getVSync()));
    //addButton(bTFM = ButtonToggle(sf::Vector2f(400.f, 40.f), sf::Vector2f(game->getRenderWnd()->getSize().x / 2 - 200, game->getRenderWnd()->getSize().y / 2 + 90), "Toggle Fullscreen Mode", 4));

    bDone.setColor(sf::Color::Green);
}

void GuiSettings::draw(sf::RenderWindow* wnd)
{
    Gui::drawGui(wnd);

    bDone.draw(wnd);
    bResetHS.draw(wnd);
    bRefreshGD.draw(wnd);
    bVerticalSync.draw(wnd);
	//bTFM.draw(wnd);

    wnd->draw(GameDisplay::instance->drawCenteredString("Settings", 30, sf::Vector2f(wnd->getSize().x / 2, 200)));
}

void GuiSettings::onButtonClicked(long button)
{
	if (!Gui::isDialogRunning())
	{
		Game* game = Game::instance;

		if (button == 0)
		{
			game->displayGui(2); //main menu
		}
		else if (button == 1)
		{
			GuiYesNo::vstr = "Are you sure to delete all your player data?\n\nThis can't be undone!";
			Gui::runDialog(100); //yes/no
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

		if (Gui::getDialogReturnValue() == 1)
		{
			remove("data.txt");
			remove("highscore.txt");
			game->loadPlayerData();
		}
	}
	else //not yet finished
	{
		GuiYesNo::onButtonClicked(button);
	}
}

