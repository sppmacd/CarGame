#include "GuiSettings.h"
#include "Game.h"
#include "GuiMainMenu.h"
#include "GameDisplay.h"
#include "GuiLanguage.hpp"
#include <iostream>

using namespace sf;

GuiSettings::GuiSettings()
{
    scSettings = new ScrollableCompound(this, Vector2f(0.f, Game::instance->getSize().y / 8.f), Vector2f(Game::instance->getSize().x, Game::instance->getSize().y * 6 / 8), -1);
    addWidget(scSettings);
}
GuiSettings::~GuiSettings()
{
    for(Widget* widget: widgets)
        if(widget->getID() != -2)
            delete widget;
}
void GuiSettings::onLoad()
{
    GameDisplay* game = GameDisplay::instance;

    addWidget(&(bDone = Button(this, Vector2f(400.f, 40.f), Vector2f(0.f, 0.f), Game::instance->translation.get("gui.done"), -2)));
    /*addWidget(&(bResetHS = Button(this, Vector2f(400.f, 40.f), Vector2f(game->getSize().x / 2 - 200, game->getSize().y / 2 - 30), Game::instance->translation.get("gui.settings.resetgame"), 1)));
    addWidget(&(bRefreshGD = Button(this, Vector2f(400.f, 40.f), Vector2f(game->getSize().x / 2 - 200, game->getSize().y / 2 - 150), Game::instance->translation.get("gui.settings.refreshres"), 2)));
    addWidget(&(bVerticalSync = ButtonToggle(this, Vector2f(400.f, 40.f),Vector2f(game->getSize().x / 2 - 200, game->getSize().y / 2 - 90), Game::instance->translation.get("gui.settings.verticalsync"), 3, game->getVSync())));
    addWidget(&(bLanguage = Button(this, Vector2f(400.f, 40.f), Vector2f(game->getSize().x / 2 - 200, game->getSize().y / 2 + 30), Game::instance->translation.get("gui.settings.language"), 4)));
    addWidget(&(sVolume = Slider(this, Vector2f(game->getSize().x / 2 - 200, game->getSize().y / 2 + 120), 400.f, 100.f, 5)));*/

    bDone.setColor(sf::Color::Green);
}

void GuiSettings::onResize()
{
    GameDisplay* game = GameDisplay::instance;
    bDone.setPosition(Vector2f(game->getSize().x / 2 - 200, game->getSize().y * 7 / 8 + 10.f));
    /*bResetHS.setPosition(Vector2f(game->getSize().x / 2 - 200, game->getSize().y / 2 - 30));
    bRefreshGD.setPosition(Vector2f(game->getSize().x / 2 - 200, game->getSize().y / 2 - 150));
    bVerticalSync.setPosition(Vector2f(game->getSize().x / 2 - 200, game->getSize().y / 2 - 90));
    bLanguage.setPosition(Vector2f(game->getSize().x / 2 - 200, game->getSize().y / 2 + 30));
    sVolume.setPosition(Vector2f(game->getSize().x / 2 - 200, game->getSize().y / 2 + 90));*/

    scSettings->setSize(Vector2f(game->getSize().x, game->getSize().y * 6 / 8));
    scSettings->setPosition(Vector2f(0.f, game->getSize().y / 8.f));
}

void GuiSettings::onDraw(sf::RenderWindow& wnd)
{
    wnd.clear(colors::bgColor);
    bDone.draw(wnd);
    /*bResetHS.draw(wnd);
    bRefreshGD.draw(wnd);
    bVerticalSync.draw(wnd);
    bLanguage.draw(wnd);
    sVolume.draw(wnd);*/

    scSettings->draw(wnd);

    wnd.draw(GameDisplay::instance->drawCenteredString(Game::instance->translation.get("gui.settings.title"), 30, sf::Vector2f(GameDisplay::instance->getSize().x / 2, GameDisplay::instance->getSize().y / 16)));

    Gui::onDraw(wnd);
}

void GuiSettings::onDialogFinished(Gui*, int callId)
{
    pair<string,string>& p = callIdToSetting[callId];
    if(dialogReturnValue == 1)
        manager->triggerSetting(p.second, p.first);
	/*if(callId == 0 && dialogReturnValue == 1)
	{
		remove("data.txt");
		remove("highscore.txt");
		remove("profile_1.txt");
		Game::instance->loadPlayerData();
		Game::instance->displayGui(new GuiMainMenu);
	}*/
}

void GuiSettings::onClick(int button)
{
    if(button == -2)
    {
        close(0);
        return;
    }

    Game* game = Game::instance;
    auto it = idToSetting.find(button);
    SettingsManager::SettingType type = manager->getSettingType(it->second.second, it->second.first);

    if(type == SettingsManager::TRIGGER)
        manager->triggerSetting(it->second.second, it->second.first);
    else if(type == SettingsManager::CONFIRM_TRIGGER)
        runDialog(new GuiYesNo(Game::instance->translation.get("settings." + it->second.first + "." + it->second.second + ".msg")), triggerToCallId[it->second]);


    /*else if(button == 1)
    {
        runDialog(new GuiYesNo(game->translation.get("gui.settings.resetgame.delete")+"\n\n"+game->translation.get("gui.settings.resetgame.warning")), 0); //yes/no
    }
    else if(button == 2)
    {

    }
    else if(button == 3)
    {

    }
    else if(button == 4)
    {

    }*/
}

#define instanceof(x,type) (dynamic_cast<type*>(&x))
#define instanceof_ptr(x,type) (dynamic_cast<type*>(x))
void GuiSettings::onClose()
{
    for(Widget* widget: widgets)
    {
        auto p = idToSetting[widget->getID()];
        if(instanceof_ptr(widget, ButtonToggle))
        {
            ButtonToggle* bt = (ButtonToggle*)widget;
            manager->setSetting(p.second, to_string(bt->getState()), p.first);
        }
        else if(instanceof_ptr(widget, Slider))
        {
            Slider* bt = (Slider*)widget;
            manager->setSetting(p.second, to_string(bt->getSliderPosition()), p.first);
        }
        else if(instanceof_ptr(widget, TextBox))
        {
            TextBox* bt = (TextBox*)widget;
            manager->setSetting(p.second, bt->getText().toAnsiString(), p.first);
        }
    }
    manager->triggerAllClose();
    manager->saveSettings("settings.txt");
}
void GuiSettings::onMouseMove(Vector2f pos)
{
    Gui::onMouseMove(pos);
    scSettings->onMouseMove(pos);
}
Widget* GuiSettings::onMouseClick(Vector2f pos, bool rel, Mouse::Button but)
{
    scSettings->onMouseClickW(pos, rel, but);
    return Gui::onMouseClick(pos, rel, but);
}

