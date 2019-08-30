#include "GuiMapSelect.h"
#include "Game.h"
#include "GameDisplay.h"
#include "GuiMainMenu.h"
#include <iostream>
#include "GuiPowers.h"
#include "maptype.h"

using namespace std;

void GuiMapSelect::onLoad()
{
    GameDisplay* game = GameDisplay::instance;

    addWidget(&(bReturn = Button(this, sf::Vector2f(250.f, 40.f), sf::Vector2f(game->getSize().x / 2 - 300.f, game->getSize().y / 2 + 320.f), Game::instance->translation.get("gui.return"), 0)));
    addWidget(&(bPowers = Button(this, sf::Vector2f(250.f, 40.f), sf::Vector2f(game->getSize().x / 2 + 50.f, game->getSize().y / 2 + 320.f), Game::instance->translation.get("gui.selectmap.powers"), 1)));
    addWidget(&(bNext = Button(this, sf::Vector2f(40.f, 600.f), sf::Vector2f(game->getSize().x / 2 + 310.f, game->getSize().y / 2 - 300.f), ">", 2)));
    addWidget(&(bPrev = Button(this, sf::Vector2f(40.f, 600.f), sf::Vector2f(game->getSize().x / 2 - 350.f, game->getSize().y / 2 - 300.f), "<", 3)));

	id = 0;

	int i = 0;
	for(auto ld : Game::instance->levelRegistry)
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
		Game::instance->unlockedLevels |= (0b1 << id);
		Game::instance->removeCoins(bMd[id].cost);
		Game::instance->sound.playSound("upgrade", 100.f);

		if(Game::instance->isNewPlayer && Game::instance->tutorialStep == 3)
		{
			Game::instance->tutorialStep = 4;
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
    float bSize = min(game->getSize().y / 2, game->getSize().x / 2);

    wnd.draw(GameDisplay::instance->drawCenteredString(Game::instance->translation.get("gui.selectmap.title"), 30, sf::Vector2f(GameDisplay::instance->getSize().x / 2, game->getSize().y / 12)));
	String mapstr = Game::instance->translation.get("map." + bMd[id].name); //map name
	String mapstr2; //map info

	if (!game->isLevelUnlocked((LevelData::MapType)id))
	{
		if (!(game->getCoins() >= bMd[id].cost))
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
        game->displayGui(new GuiPowers);
        return;
    }
    else if (button == 2)
    {
        removeWidget(bMd[id].bImg);
        id++;
        if (id >= int(game->levelRegistry.size()))
            id = 0;
        addWidget(&bMd[id].bImg);
    }
    else if (button == 3)
    {
        removeWidget(bMd[id].bImg);
        id--;
        if (id < 0)
            id = game->levelRegistry.size() - 1;
        addWidget(&bMd[id].bImg);
    }
    else
    {
        if (button == 100)
        {
            if (game->isLevelUnlocked((LevelData::MapType)id))
            {
                game->loadGame(game->findLevel((LevelData::MapType)id));

                if (Game::instance->isNewPlayer && Game::instance->tutorialStep == 4)
                {
                    Game::instance->tutorialStep = 5;
                }
                return;
            }
            else
            {
                if (game->getCoins() >= bMd[id].cost)
                {
                    runDialog(new GuiYesNo(Game::instance->translation.get("gui.selectmap.buyconfirm", {Game::instance->translation.get("map." + bMd[id].name)})), 0);
                }
                else
                    bMd[id].bImg.setEnabled(false);
            }
        }
    }

	if (!(game->getCoins() >= bMd[id].cost) && !game->isLevelUnlocked((LevelData::MapType)id))
		bMd[id].bImg.setEnabled(false);
	else
		bMd[id].bImg.setEnabled(true);
}
