#pragma once

#include <CG/CG.h>
#include "AbilityBase.hpp"

using namespace cg;

class GuiShop : public Gui
{
    struct AbilityData
    {
        AbilityBase& base;
        Button bUpgrade;
        int cost;
        double value;
        int id;
        int level;
    };
    vector<AbilityData*> abilityData;
    Button bReturn;
    Button bPowers;
public:
    virtual ~GuiShop();
    void onLoad();
    void onResize();
    void onClick(int buttonId);
    void onDraw(RenderWindow& wnd);
};
