#ifndef BUTTON_H
#define BUTTON_H

#include <SFML/Graphics.hpp>
#include <string>

using namespace std;
using namespace sf;

class Button
{
public:
    Button(sf::Vector2f size, sf::Vector2f pos, String text, int id);
	Button() : Button(Vector2f(0, 0), Vector2f(100, 30), "Button", -1) {}

    sf::Vector2f getPos()
    {
        return this->bPos;
    }
    sf::Vector2f getSize()
    {
        return this->bSize;
    }
    String getText()
    {
        return this->bText;
    }

    void setColor(sf::Color color);

    sf::Text drawString(String, int, sf::Vector2f, sf::Text::Style);

    virtual void draw(sf::RenderWindow& wnd);
	virtual void onClick();
	virtual bool isClicked(Vector2f pos);

    bool isMouseOver;
    bool enabled;
    long id;

    bool operator==(Button& r)
    {
        return r.id == this->id;
    }

protected:
	sf::Vector2f bSize;
	sf::Vector2f bPos;
	String bText;
	sf::Color bColor;
private:
};

#endif // BUTTON_H