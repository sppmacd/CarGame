#pragma once

#include "Button.h"
#include "SFML/Graphics.hpp"

using namespace sf;

class ButtonToggle : public Button
{
	bool state;

public:
	ButtonToggle(sf::Vector2f size, sf::Vector2f pos, string text, int id, bool defaultState = false);
	ButtonToggle() : ButtonToggle(Vector2f(0, 0), Vector2f(0, 0), "Button", -1) {}

	void draw(RenderWindow* wnd);
	void onClick();
	bool getState();
	void setState(bool b);
};