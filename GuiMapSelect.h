#ifndef GUIMAPSELECT_H
#define GUIMAPSELECT_H

#include "Gui.h"
#include "ButtonImage.h"

class GuiMapSelect : public Gui
{
public:
    static void draw(sf::RenderWindow* wnd);
    static void onButtonClicked(long button);
    static void onLoad();
	static void onClose();
	static void onDialogFinished(int did, int rv);

protected:

private:
	static int id;

	struct MapData
	{
		string name;
		ButtonImage bImg;
		int cost;
	};

	static vector<MapData> bMd;
    static Button bReturn;
	static Button bPowers;

	static Button bNext;
	static Button bPrev;
};

#endif // GUIMAPSELECT_H

