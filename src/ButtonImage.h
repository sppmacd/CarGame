#pragma once

#include <string>

#include <CG/CG.h>
using namespace cg;

using namespace sf;
using namespace std;

class ButtonImage : public Button
{
public:
	ButtonImage(Gui* gui, String imgName, sf::Vector2f size, sf::Vector2f pos, string unused, int id);
	ButtonImage();

	void draw(RenderWindow& wnd);
protected:
	Texture* img;
};
