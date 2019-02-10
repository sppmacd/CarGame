#pragma once
#include "Gui.h"

using namespace std;
using namespace sf;

class GuiYesNo : public Gui
{
public:
    GuiYesNo(string text);
	void onDraw(RenderWindow& wnd);
	void onClick(long button);
	void onLoad();
private:
    string message;
	Button bYes;
	Button bNo;
};

