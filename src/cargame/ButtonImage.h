#pragma once

#include <string>

#include <CG/CG.h>
using namespace cg;

using namespace sf;
using namespace std;

// Class to create new Button with image instead of text.
class ButtonImage : public Button
{
public:
	// %gui - The parent GUI.
	// %imgName - The texture name, loaded from "/res".
	// %size - THe size of button.
	// %pos - The position of left upper corner of Button.
	// %id - The button ID used by onClick().
	ButtonImage(Gui* gui, String imgName, sf::Vector2f size, sf::Vector2f pos, string unused, int id);
	
	ButtonImage();

	// Draw button to %wnd.
	void draw(RenderWindow& wnd);
protected:

	// The image texture pointer (loaded from GameDisplay)
	Texture* img;
};
