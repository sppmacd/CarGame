#pragma once

#include <CG/CG.h>
#include "SettingsManager.hpp"
using namespace cg;

class GuiSettings : public Gui
{
public:
    ScrollableCompound* scSettings;
    map<int, pair<string,string>> idToSetting;
    map<pair<string,string>, int> triggerToCallId;
    map<int, pair<string,string>> callIdToSetting;
    SettingsManager* manager;
    Gui* returnGui;

    GuiSettings();
    ~GuiSettings();
    void onDraw(sf::RenderWindow& wnd);
    void onClick(int button);
    void onLoad();
	void onDialogFinished(Gui* dialog, int callId);
	void onResize();
	void onMouseMove(Vector2f pos);
	void onClose();
	Widget* onMouseClick(Vector2f pos, bool rel, Mouse::Button but);
	
	/// Removes all widgets, resets settings maps.
	void reset();

private:
    Button bDone;
};

