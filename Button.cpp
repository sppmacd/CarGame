#include "Button.h"
#include "GameDisplay.h"

Button::Button(sf::Vector2f size, sf::Vector2f pos, string text, int id)
{
    this->isMouseOver = false;
    this->bColor = sf::Color(60, 51, 25);
    this->bSize = size;
    this->bPos = pos;
    this->bText = text;
    this->id = id;
    this->enabled = true;
}

void Button::draw(sf::RenderWindow& wnd)
{
    sf::RectangleShape rect(this->getSize());
    rect.setPosition(this->getPos());
    rect.setFillColor(sf::Color(119, 103, 50));
    rect.setOutlineThickness(2.2f);
    rect.setOutlineColor(this->bColor);

    if(this->isMouseOver && this->enabled)
        rect.setFillColor(sf::Color(240, 206, 100));
    if(!this->enabled)
        rect.setFillColor(sf::Color(60, 51, 25));

    wnd.draw(rect);

    sf::Text tx = this->drawString(this->getText(), 28, this->getPos() + sf::Vector2f(5.f, 5.f), sf::Text::Bold);
	tx.setOrigin(tx.getLocalBounds().width / 2, tx.getLocalBounds().height / 2);
	tx.setPosition(this->getPos() + this->getSize() / 2.f - sf::Vector2f(0.f, 6.f));
	wnd.draw(tx);
}

bool Button::isClicked(Vector2f pos)
{
    FloatRect buttonRect(getPos(), getSize());
    return enabled && buttonRect.contains(pos);
}

void Button::onClick()
{
	//nothing to do
}

void Button::setColor(sf::Color color)
{
    this->bColor = color;
}

sf::Text Button::drawString(string tx, int height, sf::Vector2f pos, sf::Text::Style style)
{
    return GameDisplay::instance->drawString(tx, height, pos, style);
}
