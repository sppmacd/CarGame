#include "GuiMapSelect.h"
#include "Game.h"
#include "GameDisplay.h"
#include <iostream>
#include "maptype.h"

using namespace std;

Button GuiMapSelect::bMap1;
Button GuiMapSelect::bMap2;
Button GuiMapSelect::bMap3;
Button GuiMapSelect::bMap4;
Button GuiMapSelect::bMap5;
Button GuiMapSelect::bMapB1;
Button GuiMapSelect::bMapB2;
Button GuiMapSelect::bMapB3;
Button GuiMapSelect::bMapB4;
Button GuiMapSelect::bMapB5;
Button GuiMapSelect::bReturn;
Button GuiMapSelect::bPowers;

void GuiMapSelect::onLoad()
{
    GameDisplay* game = GameDisplay::instance;

    addButton(bMap1 = Button(sf::Vector2f(400.f, 40.f), sf::Vector2f(game->getRenderWnd()->getSize().x / 2 - 200, game->getRenderWnd()->getSize().y / 2 - 180), "Countryside", 1));
    addButton(bMap2 = Button(sf::Vector2f(400.f, 40.f), sf::Vector2f(game->getRenderWnd()->getSize().x / 2 - 200, game->getRenderWnd()->getSize().y / 2 - 120), "Desert", 2));
    addButton(bMap3 = Button(sf::Vector2f(400.f, 40.f), sf::Vector2f(game->getRenderWnd()->getSize().x / 2 - 200, game->getRenderWnd()->getSize().y / 2 - 60), "Forest", 3));
    addButton(bMap4 = Button(sf::Vector2f(400.f, 40.f), sf::Vector2f(game->getRenderWnd()->getSize().x / 2 - 200, game->getRenderWnd()->getSize().y / 2), "Ice", 4));
    addButton(bMap5 = Button(sf::Vector2f(400.f, 40.f), sf::Vector2f(game->getRenderWnd()->getSize().x / 2 - 200, game->getRenderWnd()->getSize().y / 2 + 60), "Mountains", 5));

    addButton(bMapB1 = Button(sf::Vector2f(100.f, 40.f), sf::Vector2f(game->getRenderWnd()->getSize().x / 2 + 420, game->getRenderWnd()->getSize().y / 2 - 180), "$0", 101));
    addButton(bMapB2 = Button(sf::Vector2f(100.f, 40.f), sf::Vector2f(game->getRenderWnd()->getSize().x / 2 + 420, game->getRenderWnd()->getSize().y / 2 - 120), "$100", 102));
    addButton(bMapB3 = Button(sf::Vector2f(100.f, 40.f), sf::Vector2f(game->getRenderWnd()->getSize().x / 2 + 420, game->getRenderWnd()->getSize().y / 2 - 60), "$400", 103));
    addButton(bMapB4 = Button(sf::Vector2f(100.f, 40.f), sf::Vector2f(game->getRenderWnd()->getSize().x / 2 + 420, game->getRenderWnd()->getSize().y / 2), "$1000", 104));
    addButton(bMapB5 = Button(sf::Vector2f(100.f, 40.f), sf::Vector2f(game->getRenderWnd()->getSize().x / 2 + 420, game->getRenderWnd()->getSize().y / 2 + 60), "$4000", 105));

    addButton(bReturn = Button(sf::Vector2f(400.f, 40.f), sf::Vector2f(game->getRenderWnd()->getSize().x / 2 - 200, game->getRenderWnd()->getSize().y / 2 + 190), "Return to main menu", 0));

    addButton(bPowers = Button(sf::Vector2f(400.f, 40.f), sf::Vector2f(game->getRenderWnd()->getSize().x / 2 - 200, game->getRenderWnd()->getSize().y / 2 + 120), "Powers...", 200));

    bReturn.setColor(sf::Color::Red);
    bPowers.setColor(sf::Color::Cyan);
}

void GuiMapSelect::draw(sf::RenderWindow* wnd)
{
    bool u1, u2, u3, u4, u5;
    u1 = Game::instance->isLevelUnlocked(LevelData::COUNTRYSIDE);
    u2 = Game::instance->isLevelUnlocked(LevelData::DESERT);
    u3 = Game::instance->isLevelUnlocked(LevelData::FOREST);
    u4 = Game::instance->isLevelUnlocked(LevelData::ICE);
    u5 = Game::instance->isLevelUnlocked(LevelData::MOUNTAINS);

    bMap1.enabled = u1;
    bMapB1.enabled = !u1;
    bMap2.enabled = u2;
    bMapB2.enabled = !u2;
    bMap3.enabled = u3;
    bMapB3.enabled = !u3;
    bMap4.enabled = u4;
    bMapB4.enabled = !u4;
    bMap5.enabled = u5;
    bMapB5.enabled = !u5;

    if(Game::instance->getCoins() < 0)
    {
        bMapB1.enabled = false;
    }
    if(Game::instance->getCoins() < 100)
    {
        bMapB2.enabled = false;
    }
    if(Game::instance->getCoins() < 400)
    {
        bMapB3.enabled = false;
    }
    if(Game::instance->getCoins() < 1000)
    {
        bMapB4.enabled = false;
    }
    if(Game::instance->getCoins() < 4000)
    {
        bMapB5.enabled = false;
    }

    bMap1.draw(wnd);
    bMap2.draw(wnd);
    bMap3.draw(wnd);
    bMap4.draw(wnd);
    bMap5.draw(wnd);

    bMapB1.draw(wnd);
    bMapB2.draw(wnd);
    bMapB3.draw(wnd);
    bMapB4.draw(wnd);
    bMapB5.draw(wnd);

    bReturn.draw(wnd);
    bPowers.draw(wnd);

    wnd->draw(GameDisplay::instance->drawCenteredString("Map & Powers", 30, sf::Vector2f(wnd->getSize().x / 2, 200)));
}

void GuiMapSelect::onButtonClicked(long button)
{
    Game* game = Game::instance;

    if(button == 0)
    {
        Game::instance->displayGui(2); //main menu
    }
    else if(button == 1)
    {
        if(Game::instance->isNewPlayer && Game::instance->tutorialStep == 4)
        {
            Game::instance->tutorialStep = 5;
        }
        game->loadGame(Maps::countryside);
    }

    else if(button == 2)
    {
        game->loadGame(Maps::desert);
    }

    else if(button == 3)
    {
        game->loadGame(Maps::forest);
    }

    else if(button == 4)
    {
        game->loadGame(Maps::ice);
    }

    else if(button == 5)
    {
        game->loadGame(Maps::mountains);
    }

    else if(button == 101)
    {
        Game::instance->unlockedLevels |= 0b00001;
        Game::instance->removeCoins(0);

        if(Game::instance->isNewPlayer && Game::instance->tutorialStep == 3)
        {
            Game::instance->tutorialStep = 4;
        }
    }
    else if(button == 102)
    {
        Game::instance->unlockedLevels |= 0b00010;
        Game::instance->removeCoins(100);
    }
    else if(button == 103)
    {
        Game::instance->unlockedLevels |= 0b00100;
        Game::instance->removeCoins(400);
    }
    else if(button == 104)
    {
        Game::instance->unlockedLevels |= 0b01000;
        Game::instance->removeCoins(1000);
    }
    else if(button == 105)
    {
        Game::instance->unlockedLevels |= 0b10000;
        Game::instance->removeCoins(3000);
    }
    else if(button == 200)
    {
        game->displayGui(5);
    }
}
