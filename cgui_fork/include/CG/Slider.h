#pragma once

#include <CG/Widget.h>
#include <CG/Gui.h>

namespace cg
{
class Slider : public Widget
{
    float bMaxPos;
    float bCurrentPos;
    float bDisplaySize;
    Color bColor;
    bool mouseClicked;
public:
    Slider();
    Slider(Gui* gui, Vector2f pos, float dispSize, float max, int bId);
    virtual void setColor(Color color);
    virtual void setSliderPosition(float pos);
    virtual void setMaxSliderPosition(float pos);
    virtual float getSliderPosition();
    virtual float getMaxSliderPosition();
    virtual void draw(sf::RenderWindow& wnd);
    virtual bool isClicked(Vector2f pos);
    virtual void onMouseClick(Vector2f pos, bool release, Mouse::Button button);
private:
    void updateSlider(Vector2f clickPos);
};
}
