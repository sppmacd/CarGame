#pragma once

#include "Gui.h"
#include <vector>
#include "Power.h"

class GuiPowers : public Gui
{
public:
    void onDraw(sf::RenderWindow& wnd);
    void onClick(long button);
    void onLoad();

protected:

private:
    struct PowerData
    {
        unsigned int cost;
        unsigned int count;
        Power* power;
        Button bBuyPower;
    };

    virtual ~GuiPowers();
    vector<PowerData*> powerData;
    Button bReturn;
    int cooldown;
};


