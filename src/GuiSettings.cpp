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
    bDone.setColor(sf::Color::Green);
}

void GuiSettings::onResize()
{
    GameDisplay* game = GameDisplay::instance;
    bDone.setPosition(Vector2f(game->getSize().x / 2 - 200, game->getSize().y * 7 / 8 + 10.f));

    scSettings->setSize(Vector2f(game->getSize().x, game->getSize().y * 6 / 8));
    scSettings->setPosition(Vector2f(0.f, game->getSize().y / 8.f));

    for(size_t s = 0; s < widgets.size(); s++)
    {
        if(widgets[s]->getID() > 0)
        {   //                                                    [omit first widget: scSettings]
            Vector2f pos = Vector2f(scSettings->getSize().x / 2.f - 200.f, (s - 1) * 60.f + 20.f);
            widgets[s]->setPosition(pos);
        }
    }
}

void GuiSettings::onDraw(sf::RenderWindow& wnd)
{
    wnd.clear(colors::bgColor);
    bDone.draw(wnd);
    scSettings->draw(wnd);

    wnd.draw(GameDisplay::instance->drawCenteredString(Game::instance->translation.get("gui.settings.title"), 30, sf::Vector2f(GameDisplay::instance->getSize().x / 2, GameDisplay::instance->getSize().y / 16)));

    Gui::onDraw(wnd);
}

void GuiSettings::onDialogFinished(Gui*, int callId)
{
    if(callId != -1)
    {
        pair<string,string>& p = callIdToSetting[callId];
        if(dialogReturnValue == 1)
            manager->triggerSetting(p.second, p.first);
    }
}

void GuiSettings::onClick(int button)
{
    Game* game = Game::instance;

    if(button == -2)
    {
        game->displayGui(returnGui);
        return;
    }

    auto it = idToSetting.find(button);
    SettingsManager::SettingType type = manager->getSettingType(it->second.second, it->second.first);

    if(type == SettingsManager::TRIGGER)
        manager->triggerSetting(it->second.second, it->second.first);
    else if(type == SettingsManager::CONFIRM_TRIGGER)
        runDialog(new GuiYesNo(Game::instance->translation.get("settings." + it->second.first + "." + it->second.second + ".msg")), triggerToCallId[it->second]);
}

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

void GuiSettings::reset()
{
	for(cg::Widget* widget: widgets)
        if(widget != scSettings && widget->getID() != -2)
            delete widget;
    delete scSettings;

	widgets.clear();
    idToSetting.clear();
    triggerToCallId.clear();
    callIdToSetting.clear();

	// GuiSettings()
	scSettings = new ScrollableCompound(this, Vector2f(0.f, Game::instance->getSize().y / 8.f), Vector2f(Game::instance->getSize().x, Game::instance->getSize().y * 6 / 8), -1);
    addWidget(scSettings);

    // add done button
    addWidget(&bDone);
}
