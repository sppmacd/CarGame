#pragma once

#include <CG/CG.h>
#include <string>
#include <vector>
#include <array>

using namespace cg;
using namespace std;

#define GUILANGUAGE_LANGS_PER_PAGE 5

class GuiLanguage : public Gui
{
    Button bCancel; //0
    Button bDone; //1
    Button bUp; //2
    Button bDown; //3
    int current;

    struct GlLangData
    {
        string langCode;
        Button* bSetLang;
    };
    struct GlLangPage
    {
        // Language buttons IDs starts from 100.
        std::array<GlLangData, GUILANGUAGE_LANGS_PER_PAGE> bBts;
    };
public:
    virtual ~GuiLanguage();

    int currentLang;

    vector<GlLangPage> bLangButtons;

    virtual void onLoad();
    virtual void onDraw(RenderWindow& wnd);
    virtual void onClick(int buttonId);
    virtual void onResize();

    // todo: change pages by kb arrows
    virtual void onKeyboardEvent(Keyboard::Key key, bool released, bool, bool, bool, bool);
};
