#include "ButtonToggle.h"

ButtonToggle::ButtonToggle(sf::Vector2f size, sf::Vector2f pos, string text, int id, bool defaultState)
	: Button(size,pos,text,id)
{
	this->setState(defaultState);
}

void ButtonToggle::draw(RenderWindow * wnd)
{
	sf::RectangleShape rect(this->getSize());
	rect.setPosition(this->getPos());
	rect.setFillColor(state ? Color::Green : Color::Red);
	rect.setOutlineThickness(2.2f);
	rect.setOutlineColor(this->bColor);

	if (this->isMouseOver && this->enabled)
		rect.setFillColor(sf::Color(240, 206, 100));
	if (!this->enabled)
		rect.setFillColor(sf::Color(60, 51, 25));

	wnd->draw(rect);

	sf::Text tx = this->drawString(this->getText() + (state ? ": On" : ": Off"), 28, this->getPos() + sf::Vector2f(5.f, 5.f), sf::Text::Bold);
	tx.setOrigin(tx.getLocalBounds().width / 2, tx.getLocalBounds().height / 2);
	tx.setPosition(this->getPos() + this->getSize() / 2.f - sf::Vector2f(0.f, 6.f));
	wnd->draw(tx);
}

void ButtonToggle::onClick()
{
	state = !state;
}

bool ButtonToggle::getState()
{
	return state;
}

void ButtonToggle::setState(bool b)
{
	state = b;
}
