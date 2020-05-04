#include "GuiMapSelect.h"

#include "Game.h"

#include "GameDisplay.h"
#include "GuiMainMenu.h"
#include "GuiPowers.h"
#include "GuiShop.hpp"
#include "maptype.h"

#include <iostream>

using namespace std;

void GuiMapSelect::onLoad()
{
    GameDisplay* game = GameDisplay::instance;

    addWidget(&(bReturn = Button(this, sf::Vector2f(250.f, 40.f), sf::Vector2f(game->getSize().x / 2 - 300.f, game->getSize().y / 2 + 320.f), Game::instance->translation.get("gui.return"), 0)));
    addWidget(&(bPowers = Button(this, sf::Vector2f(250.f, 40.f), sf::Vector2f(game->getSize().x / 2 + 50.f, game->getSize().y / 2 + 320.f), Game::instance->translation.get("gui.selectmap.shop"), 1)));
    addWidget(&(bNext = Button(this, sf::Vector2f(40.f, 600.f), sf::Vector2f(game->getSize().x / 2 + 310.f, game->getSize().y / 2 - 300.f), ">", 2)));
    addWidget(&(bPrev = Button(this, sf::Vector2f(40.f, 600.f), sf::Vector2f(game->getSize().x / 2 - 350.f, game->getSize().y / 2 - 300.f), "<", 3)));

	id = 0;

	int i = 0;
	for(auto ld : Game::instance->gpo.levels.arr())
	{
		LevelData* lvld = ld.second;
		ButtonImage bimg(this, "map/" + lvld->getTextureName(), Vector2f(600.f, 600.f), Vector2f(game->getSize().x / 2 - 300.f, game->getSize().y / 2 - 300.f), ld.first, 100);
		bimg.setColor(lvld->getColor());
		MapData md{ld.first, bimg, lvld->getCost()};
		bMd.push_back(md);

		if(Game::instance->isLevelUnlocked(lvld->getMapType()))
            id = lvld->getMapType();

        i++;
	}

	addWidget(&bMd[id].bImg);

    bReturn.setColor(sf::Color::Red);
    bPowers.setColor(sf::Color::Cyan);

    if(Game::instance->canPowerBuyOrEquip())
        bPowers.setBlinking(true);
}

void GuiMapSelect::onResize()
{
    GameDisplay* game = GameDisplay::instance;

    float bSize = min(game->getSize().y / 2, game->getSize().x / 2);
    bReturn.setPosition(Vector2f(game->getSize().x / 2 - bSize / 2, game->getSize().y / 2 + bSize / 2 + 20));
    bReturn.setSize(Vector2f(bSize / 2 - 20.f, 40.f));
    bPowers.setPosition(Vector2f(game->getSize().x / 2 + 20.f, game->getSize().y / 2 + bSize / 2 + 20));
    bPowers.setSize(Vector2f(bSize / 2 - 20.f, 40.f));
    bNext.setPosition(Vector2f(game->getSize().x / 2 + bSize / 2 + 10.f, game->getSize().y / 2 - bSize / 2));
    bNext.setSize(Vector2f(40.f, bSize));
    bPrev.setPosition(Vector2f(game->getSize().x / 2 - bSize / 2 - 50.f, game->getSize().y / 2 - bSize / 2));
    bPrev.setSize(Vector2f(40.f, bSize));

    for(GuiMapSelect::MapData& b: bMd)
    {
        b.bImg.setPosition(Vector2f(game->getSize().x / 2 - bSize / 2, game->getSize().y / 2 - bSize / 2));
        b.bImg.setSize(Vector2f(bSize, bSize));
    }
}

void GuiMapSelect::onClose()
{
	bMd.clear();
}

void GuiMapSelect::onDialogFinished(Gui*, int callId)
{
	if(callId == 0 && dialogReturnValue == 1)
	{
		Game::instance->playerData.unlockedLevels |= (0b1 << id);
		Game::instance->removeCoins(bMd[id].cost);
		Game::instance->sound.playSound("upgrade", 100.f);

		if(Game::instance->playerData.isNewPlayer && Game::instance->playerData.tutorialStep == TUT_BUYMAP)
		{
			Game::instance->playerData.tutorialStep = TUT_SELECTMAP;
		}
	}
}

void GuiMapSelect::onDraw(sf::RenderWindow& wnd)
{
	// draw all maps
	bMd[id].bImg.draw(wnd);

    bReturn.draw(wnd);
    bPowers.draw(wnd);
	bNext.draw(wnd);
	bPrev.draw(wnd);
	Game* game = Game::instance;

	if(game->playerData.isNewPlayer)
    {
        if(game->playerData.tutorialStep == TUT_BUYMAP)
        {
            GameDisplay::instance->drawTutorial(bMd[id].bImg.getPosition(), bMd[id].bImg.getSize(), game->translation.get("tutorial.buymap"));
        }
        else if(game->playerData.tutorialStep == TUT_SELECTMAP)
        {
            GameDisplay::instance->drawTutorial(bMd[id].bImg.getPosition(), bMd[id].bImg.getSize(), game->translation.get("tutorial.selectmap"));
        }
        else if(game->playerData.tutorialStep == TUT_SHOP)
        {
            GameDisplay::instance->drawTutorial(bPowers.getPosition(), bPowers.getSize(), game->translation.get("tutorial.shop"));
        }
    }

    float bSize = min(game->getSize().y / 2, game->getSize().x / 2);

    wnd.draw(GameDisplay::instance->drawCenteredString(Game::instance->translation.get("gui.selectmap.title"), 30, sf::Vector2f(GameDisplay::instance->getSize().x / 2, game->getSize().y / 12)));
	String mapstr = Game::instance->translation.get("map." + bMd[id].name); //map name
	String mapstr2; //map info

	if (!game->isLevelUnlocked((LevelData::MapType)id))
	{
		if (!(game->playerData.playerCoins >= bMd[id].cost))
			mapstr2 += (Game::instance->translation.get("gui.selectmap.notenoughcoins"));

		mapstr += (" - " + Game::instance->translation.get("gui.selectmap.buyfor", {to_string(bMd[id].cost)}));
	}

    wnd.draw(GameDisplay::instance->drawCenteredString(mapstr, 25, sf::Vector2f(GameDisplay::instance->getSize().x / 2, game->getSize().y * 1 / 8 + 10.f)));
    wnd.draw(GameDisplay::instance->drawCenteredString(mapstr2, 25, sf::Vector2f(GameDisplay::instance->getSize().x / 2, game->getSize().y * 1 / 6 + 10.f)));

    Gui::onDraw(wnd);
}

void GuiMapSelect::onClick(int button)
{
    Game* game = Game::instance;

    if (button == 0)
    {
        game->displayGui(new GuiMainMenu);
        return;
    }
    else if (button == 1)
    {
        game->displayGui(new GuiShop);
        return;
    }
    else if (button == 2)
    {
        removeWidget(bMd[id].bImg);
        id++;
        if (id >= int(game->gpo.levels.count()))
            id = 0;
        addWidget(&bMd[id].bImg);
    }
    else if (button == 3)
    {
        removeWidget(bMd[id].bImg);
        id--;
        if (id < 0)
            id = game->gpo.levels.count() - 1;
        addWidget(&bMd[id].bImg);
    }
    else
    {
        if (button == 100)
        {
            if(Game::instance->playerData.isNewPlayer && Game::instance->playerData.tutorialStep == TUT_SELECTMAP)
            {
                Game::instance->playerData.tutorialStep = TUT_DESTROYCAR;
            }

            if (game->isLevelUnlocked((LevelData::MapType)id))
            {
                game->loadGame(*game->gpo.levels.arr()[id].second);

                if (Game::instance->playerData.isNewPlayer && Game::instance->playerData.tutorialStep == 4)
                {
                    Game::instance->playerData.tutorialStep = 5;
                }
                return;
            }
            else
            {
                if (game->playerData.playerCoins >= bMd[id].cost)
                {
                    runDialog(new GuiYesNo(Game::instance->translation.get("gui.selectmap.buyconfirm", {Game::instance->translation.get("map." + bMd[id].name)})), 0);
                }
                else
                    bMd[id].bImg.setEnabled(false);
            }
        }
    }

	if (!(game->playerData.playerCoins >= bMd[id].cost) && !game->isLevelUnlocked((LevelData::MapType)id))
		bMd[id].bImg.setEnabled(false);
	else
		bMd[id].bImg.setEnabled(true);
}
