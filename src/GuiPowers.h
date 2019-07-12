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
        //unsigned int level; // 0.2
        Power* power;
        Button bBuyPower;
        //Button bUpgradePower; // 0.2
    };

    virtual ~GuiPowers();
    vector<PowerData*> powerData;
    Button bReturn;
    int cooldown;
};


