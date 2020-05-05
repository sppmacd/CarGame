#pragma once

#include <CG/CG.h>
using namespace cg;

#include <vector>
#include <array>
#include "Power.h"
#include "ButtonImage.h"

class GuiPowers : public Gui
{
public:
    void onDraw(sf::RenderWindow& wnd);
    void onClick(int button);
    void onLoad();
    void onResize();
    void onClose();
    void onMouseMove(sf::Vector2f vec);

protected:

private:
    struct PowerData
    {
        unsigned int cost;
        unsigned int count;
        Power* power;
        Button bBuyPower;
        ButtonImage bEquipPower;
    };
    struct EquippedPowerData
    {
        int powerId;
        ButtonImage bImg;
    };

    virtual ~GuiPowers();
    vector<PowerData*> powerData;
    array<EquippedPowerData, 2> equippedPowerIds;
    Button bReturn;
    int cooldown;
};


