#pragma once

#include "ButtonImage.h"
#include <SFML/Graphics.hpp>

using namespace sf;

// A class to create circular button with image in GUI.
class ButtonCircle : public ButtonImage
{
    // Circle radius.
    float bRadius;
public:
    ButtonCircle() {}
    
    // %gui - The parent GUI
    // %radius - Button radius
    // %pos - Button position (left up corner)
    // %imageName - Image texture name (loaded from "res")
    // %id - ID used in onClick()
    ButtonCircle(Gui* gui, float radius, Vector2f pos, string imageName, int id);
    
    // Sets the new radius for button.
    // %radius - The new radius
    void setRadius(float radius);
    
    // Draw button to window.
    // %wnd - The SFML RenderWindow.
    void draw(RenderWindow& wnd);
    
    // \returns <true> if %pos belongs to widget area, <false> otherwise.
    // %pos - The checked position.
    bool isClicked(Vector2f pos);
};
