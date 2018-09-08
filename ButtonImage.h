#pragma once

#include <string>
#include "Button.h"
#include "SFML/Graphics.hpp"

using namespace sf;
using namespace std;

class ButtonImage : public Button
{
public:
	ButtonImage(string imgName, sf::Vector2f size, sf::Vector2f pos, string text, int id);
	ButtonImage();

	void draw(RenderWindow* wnd);
private:
	Texture* img;
};