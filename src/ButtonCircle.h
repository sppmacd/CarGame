#pragma once

#include "ButtonImage.h"
#include <SFML/Graphics.hpp>

using namespace sf;

class ButtonCircle : public ButtonImage
{
    float bRadius;
public:
    ButtonCircle() {}
    ButtonCircle(float radius, Vector2f pos, string imageName, int id);
    void setRadius(float radius);
    void draw(RenderWindow& wnd);
    bool isClicked(Vector2f pos);
};
