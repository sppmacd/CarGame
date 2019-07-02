#pragma once
#include "Gui.h"

using namespace std;
using namespace sf;

class GuiYesNo : public Gui
{
public:
    GuiYesNo(String text);
	void onDraw(RenderWindow& wnd);
	void onClick(long button);
	void onLoad();
private:
    String message;
	Button bYes;
	Button bNo;
};

