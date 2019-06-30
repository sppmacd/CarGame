#include "ButtonToggle.h"
#include "Game.h"

ButtonToggle::ButtonToggle(sf::Vector2f size, sf::Vector2f pos, String text, int id, bool defaultState)
	: Button(size,pos,text,id)
{
	this->setState(defaultState);
	this->animStat = 0;
}

void ButtonToggle::draw(RenderWindow& wnd)
{
	if (this->animStat > 0)
		this->animStat--;

	int ap = state ? this->animStat : 30 - this->animStat;

	sf::RectangleShape rect(this->getSize());
	rect.setPosition(this->getPos());
	rect.setFillColor(sf::Color(119, 103, 50));
	rect.setOutlineThickness(2.2f);
	rect.setOutlineColor(this->bColor);

	sf::RectangleShape rect2(Vector2f(30.f, this->getSize().y));
	rect2.setPosition(this->getPos().x + (this->getSize().x - 30.f) * (ap / 30.f), this->getPos().y);

	int rcp = this->state ? this->animStat : 30 - this->animStat;
	int gcp = this->state ? 30 - this->animStat : this->animStat;

	Color color1 = Color(0, 255 * (gcp / 30.f), 0);
	Color color2 = Color(255 * (rcp / 30.f), 0, 0);

	rect2.setFillColor(color1 + color2);

	if (this->isMouseOver && this->enabled)
		rect.setFillColor(sf::Color(240, 206, 100));
	if (!this->enabled)
		rect.setFillColor(sf::Color(60, 51, 25));

	wnd.draw(rect);
	wnd.draw(rect2);

	String on = Game::instance->translation.get("gui.toggle.on");
	String off = Game::instance->translation.get("gui.toggle.off");

	sf::Text tx = this->drawString(this->getText() + (state ? ": "+on : ": "+off), 28, this->getPos() + sf::Vector2f(5.f, 5.f), sf::Text::Bold);
	tx.setOrigin(tx.getLocalBounds().width / 2, tx.getLocalBounds().height / 2);
	tx.setPosition(this->getPos() + this->getSize() / 2.f - sf::Vector2f(0.f, 6.f));
	wnd.draw(tx);
}

void ButtonToggle::onClick()
{
	state = !state;
	this->animStat = 30-this->animStat; //0.5 seconds
}

bool ButtonToggle::getState()
{
	return state;
}

void ButtonToggle::setState(bool b)
{
	state = b;
}
