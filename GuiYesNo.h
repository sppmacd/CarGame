#pragma once
#include "Gui.h"

using namespace std;
using namespace sf;

class GuiYesNo :
	public Gui
{
public:
	static void draw(RenderWindow* wnd);
	static void onButtonClicked(long button);
	static void onLoad();
	static int returnValue;
	static string vstr;

private:

	static Button bYes;
	static Button bNo;
};

