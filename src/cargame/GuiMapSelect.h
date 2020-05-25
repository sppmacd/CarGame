#ifndef GUIMAPSELECT_H
#define GUIMAPSELECT_H

#include <CG/CG.h>
using namespace cg;

#include "ButtonImage.h"

class GuiMapSelect : public Gui
{
public:
    void onDraw(sf::RenderWindow& wnd);
    void onClick(int button);
    void onLoad();
	void onClose();
	void onDialogFinished(Gui* dialog, int callId);
	void onResize();

private:
	int id;

	struct MapData
	{
		string id;
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

