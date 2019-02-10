#ifndef GUIMAPSELECT_H
#define GUIMAPSELECT_H

#include "Gui.h"
#include "ButtonImage.h"

class GuiMapSelect : public Gui
{
public:
    void onDraw(sf::RenderWindow& wnd);
    void onClick(long button);
    void onLoad();
	void onClose();
	void onDialogFinished(Gui* dialog);

private:
	int id;

	struct MapData
	{
		string name;
		ButtonImage bImg;
		int cost;
	};

	vector<MapData> bMd;
    Button bReturn;
	Button bPowers;

	Button bNext;
	Button bPrev;
};

#endif // GUIMAPSELECT_H

