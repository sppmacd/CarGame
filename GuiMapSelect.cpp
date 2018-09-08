#include "GuiMapSelect.h"
#include "Game.h"
#include "GameDisplay.h"
#include "GuiYesNo.h"
#include <iostream>
#include "maptype.h"

using namespace std;

vector<GuiMapSelect::MapData> GuiMapSelect::bMd;
Button GuiMapSelect::bReturn;
Button GuiMapSelect::bPowers;

Button GuiMapSelect::bNext;
Button GuiMapSelect::bPrev;

int GuiMapSelect::id;

void GuiMapSelect::onLoad()
{
	static int costs[] = { 0,100,400,1000,4000 };

    GameDisplay* game = GameDisplay::instance;

    addButton(bReturn = Button(sf::Vector2f(250.f, 40.f), sf::Vector2f(game->getRenderWnd()->getSize().x / 2 - 300, game->getRenderWnd()->getSize().y / 2 + 320), "Return...", 0));
    addButton(bPowers = Button(sf::Vector2f(250.f, 40.f), sf::Vector2f(game->getRenderWnd()->getSize().x / 2 + 50, game->getRenderWnd()->getSize().y / 2 + 320), "Powers...", 1));
    addButton(bNext = Button(sf::Vector2f(40.f, 300.f), sf::Vector2f(game->getRenderWnd()->getSize().x / 2 + 310, game->getRenderWnd()->getSize().y / 2 - 300), ">", 2));
    addButton(bPrev = Button(sf::Vector2f(40.f, 300.f), sf::Vector2f(game->getRenderWnd()->getSize().x / 2 - 350, game->getRenderWnd()->getSize().y / 2 - 300), "<", 3));
	
	id = 0;
	
	int i = 0;
	for (auto ld : Game::instance->levelRegistry)
	{
		LevelData* lvld = ld.second;
		ButtonImage bimg("stat/coin", Vector2f(600.f, 600.f), Vector2f(game->getRenderWnd()->getSize().x / 2 - 300.f, game->getRenderWnd()->getSize().y / 2 - 300.f), ld.first, 100);
		bimg.setColor(lvld->getColor());
		MapData md{ld.first, bimg, costs[i]};
		bMd.push_back(md);
		i++;
	}

	addButton(bMd[id].bImg);

    bReturn.setColor(sf::Color::Red);
    bPowers.setColor(sf::Color::Cyan);
}

void GuiMapSelect::onClose()
{
	bMd.clear();
}

void GuiMapSelect::onDialogFinished(int did, int rv)
{
	if (did == 100 && rv == 1)
	{
		Game::instance->unlockedLevels |= (0b1 << id);
		Game::instance->removeCoins(bMd[id].cost);

		if (Game::instance->isNewPlayer && Game::instance->tutorialStep == 3)
		{
			Game::instance->tutorialStep = 4;
		}
	}
}

void GuiMapSelect::draw(sf::RenderWindow* wnd)
{
	// draw all maps
	bMd[id].bImg.draw(wnd);

    bReturn.draw(wnd);
    bPowers.draw(wnd);
	bNext.draw(wnd);
	bPrev.draw(wnd);

    wnd->draw(GameDisplay::instance->drawCenteredString("Map & Powers", 30, sf::Vector2f(wnd->getSize().x / 2, 100)));
    wnd->draw(GameDisplay::instance->drawCenteredString(bMd[id].name + ": " + to_string(bMd[id].cost) + "$ " + "(ID "+to_string(id)+")", 30, sf::Vector2f(wnd->getSize().x / 2, 150)));
}

void GuiMapSelect::onButtonClicked(long button)
{
    Game* game = Game::instance;

	if (!isDialogRunning())
	{
		if (button == 0)
			game->displayGui(2);
		else if (button == 1)
			game->displayGui(5);
		else if (button == 2)
		{
			removeButton(bMd[id].bImg);
			id++;
			if (id >= game->levelRegistry.size())
				id = 0;
			addButton(bMd[id].bImg);
		}
		else if (button == 3)
		{
			removeButton(bMd[id].bImg);
			id--;
			if (id < 0)
				id = game->levelRegistry.size() - 1;
			addButton(bMd[id].bImg);
		}
		else
		{
			if (button == 100)
			{
				if (game->isLevelUnlocked((LevelData::MapType)id))
					game->loadGame(*game->levelRegistry[bMd[id].name]);
				else
				{
					if (game->getCoins() >= bMd[id].cost)
					{
						GuiYesNo::vstr = "Do you want to buy " + bMd[id].name + "?";
						Gui::runDialog(100);
					}
					else
						bMd[id].bImg.enabled = false;
				}
			}
		}
	}
	else
		GuiYesNo::onButtonClicked(button);

	if (!(game->getCoins() >= bMd[id].cost) && !game->isLevelUnlocked((LevelData::MapType)id))
		bMd[id].bImg.enabled = false;
	else
		bMd[id].bImg.enabled = true;
}
