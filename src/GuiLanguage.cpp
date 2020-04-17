#include "GuiLanguage.hpp"
#include "GuiSettings.h"
#include "GameDisplay.h"
#include "Game.h"
#include "FileUtil.hpp"
#include "DebugLogger.hpp"

void GuiLanguage::onLoad()
{
    addWidget(&(bCancel = Button(this, Vector2f(400.f, 40.f), Vector2f(guiHandler->getSize().x / 2 -  200.f, guiHandler->getSize().y / 2 + 200.f),
                               Game::instance->translation.get("gui.cancel"), 0)));
    addWidget(&(bDone = Button(this, Vector2f(400.f, 40.f), Vector2f(guiHandler->getSize().x / 2 -  200.f, guiHandler->getSize().y / 2 + 150.f),
                               Game::instance->translation.get("gui.done"), 1)));
    addWidget(&(bUp = Button(this, Vector2f(400.f, 40.f), Vector2f(guiHandler->getSize().x / 2 -  200.f, guiHandler->getSize().y / 2 + 100.f),
    Game::instance->translation.get("gui.up"), 2))); bUp.setEnabled(false);

    addWidget(&(bDown = Button(this, Vector2f(400.f, 40.f), Vector2f(guiHandler->getSize().x / 2 -  200.f, guiHandler->getSize().y / 2 + 50.f),
    Game::instance->translation.get("gui.down"), 3))); bDown.setEnabled(false);

    // load file with names of all languages
    HMDataMap langNameMap;
    langNameMap.loadFromFile("res/langnames.hmd");

    // generate widgets for all languages
    // "res/lang" length: 8
    std::vector<std::string> langFiles = FileUtil::listFiles("res/lang");

    // convert file names to lang codes, add widgets
    std::string currentLangCode = Game::instance->hmLangCfg.getKey("current", "lang", "en_US");
    int langCount = 0;
    for(int i = 0; i < langFiles.size(); i++)
    {
        std::string cf = langFiles[i];
        int dPos = cf.find_last_of('.');
        std::string fn = cf.substr(9, dPos - 9); //Use only file name (without extension)
        std::string ext = cf.substr(dPos + 1);

        // Exclude all files that are not language files.
        if(ext == "lang")
        {
            if(langCount % GUILANGUAGE_LANGS_PER_PAGE == 0)
            {
                // add new page
                GlLangPage pg;
                bLangButtons.push_back(pg);
            }
            // retrieve country and lang name
            std::string langName = fn.substr(0,2); //first 2 chars
            std::string langCountry = fn.substr(3,2); //2 chars starting from char #2
            DebugLogger::logDbg("Adding language to GuiLanguage: fn=" + fn + ", name=" + langName + ", country=" + langCountry, "GuiLanguage");
            sf::String langNameLoc = langNameMap.getKey(langName, "l", "???");
            sf::String langCountryLoc = langNameMap.getKey(langCountry, "c", "???");
            sf::String btLabel = langNameLoc + " (" + langCountryLoc + ")";

            // add button to page
            GlLangPage& pg2 = bLangButtons[langCount / GUILANGUAGE_LANGS_PER_PAGE];
            GlLangData gld;
            gld.bSetLang = new cg::Button(this, sf::Vector2f(400.f, 40.f), sf::Vector2f(10.f, (langCount % GUILANGUAGE_LANGS_PER_PAGE) * 50.f + 200.f), btLabel, 100 + langCount);
            addWidget(gld.bSetLang);
            gld.langCode = fn;
            pg2.bBts[langCount % GUILANGUAGE_LANGS_PER_PAGE] = gld;

            // Find current language.
            if(fn == currentLangCode)
            {
                currentLang = langCount;
                gld.bSetLang->setEnabled(false);
            }

            langCount++;
        }
    }

    bUp.setEnabled(false);

    if(bLangButtons.size() <= 1)
        bDown.setEnabled(false);

    current = 0;
}
GuiLanguage::~GuiLanguage()
{
    for(int i = 0; i < bLangButtons.size(); i++)
    {
        for(int j = 0; j < GUILANGUAGE_LANGS_PER_PAGE; j++)
        {
            if(!bLangButtons[i].bBts[j].langCode.empty())
                delete bLangButtons[i].bBts[j].bSetLang;
        }
    }
}
void GuiLanguage::onResize()
{
    bCancel.setPosition(Vector2f(guiHandler->getSize().x / 2 -  200.f, guiHandler->getSize().y / 2 + 200.f));
    bDone.setPosition(Vector2f(guiHandler->getSize().x / 2 -  200.f, guiHandler->getSize().y / 2 + 150.f));
    bUp.setPosition(Vector2f(guiHandler->getSize().x / 2 -  200.f, guiHandler->getSize().y / 2 - 260.f));
    bDown.setPosition(Vector2f(guiHandler->getSize().x / 2 -  200.f, guiHandler->getSize().y / 2 + 60.f));

    GuiLanguage::GlLangPage& pg = bLangButtons[current];
    for(int i = 0; i < pg.bBts.size(); i++)
    {
        if(!pg.bBts[i].langCode.empty())
            pg.bBts[i].bSetLang->setPosition(sf::Vector2f(guiHandler->getSize().x / 2 -  200.f, guiHandler->getSize().y / 2 + i * 50.f - 200.f));
    }
}

void GuiLanguage::onDraw(RenderWindow& wnd)
{
    wnd.clear(colors::bgColor);
    bCancel.draw(wnd);
    bDone.draw(wnd);
    bUp.draw(wnd);
    bDown.draw(wnd);

    // draw some bg under langs

    // draw only current page
    GuiLanguage::GlLangPage& pg = bLangButtons[current];
    for(int i = 0; i < pg.bBts.size(); i++)
    {
        if(!pg.bBts[i].langCode.empty())
            pg.bBts[i].bSetLang->draw(wnd);
    }

    wnd.draw(GameDisplay::instance->drawCenteredString(Game::instance->translation.get("gui.language.title"), 30, Vector2f(guiHandler->getSize().x / 2, 200.f)));
}
void GuiLanguage::onClick(int buttonId)
{
    // TODO
    switch(buttonId)
    {
    case 0:
        close(0);
        break;
    case 1:
        Game::instance->loadLanguages();
        close(0);
        break;
    case 2:
        if(current - 1 <= 0)
            bUp.setEnabled(false);
        else
            bUp.setEnabled(true);
        current--;
        break;
    case 3:
        if(current + 1 >= bLangButtons.size())
            bDown.setEnabled(false);
        else
            bDown.setEnabled(true);
        current++;
        break;
    default:
        {
            int relLangId = (buttonId - 100);
            int btPosInPage = relLangId % GUILANGUAGE_LANGS_PER_PAGE;
            bool isInCurrentPage = (relLangId / GUILANGUAGE_LANGS_PER_PAGE) == current;
            if(isInCurrentPage)
            {
                GuiLanguage::GlLangData& gld = bLangButtons[current].bBts[btPosInPage];

                //enable button for previous lang.
                GuiLanguage::GlLangData& gldPrev = bLangButtons[currentLang / GUILANGUAGE_LANGS_PER_PAGE].bBts[currentLang % GUILANGUAGE_LANGS_PER_PAGE];
                gldPrev.bSetLang->setEnabled(true);

                //save language to config file
                Game::instance->hmLangCfg.setKey("current",gld.langCode,"lang");
                Game::instance->hmLangCfg.saveToFile("lang.hmd");

                //disable button for current lang.
                gld.bSetLang->setEnabled(false);
                currentLang = relLangId;
            }
        }
    }
}
void GuiLanguage::onKeyboardEvent(Keyboard::Key key, bool released, bool, bool, bool, bool)
{
    // We don't need implement changing pages the next one, we
    // have done it in onClick() - so only simulate button clicks
    if(!released)
    {
        switch(key)
        {
            case sf::Keyboard::Up: if(bUp.isEnabled()) onClick(2); break;
            case sf::Keyboard::Down: if(bUp.isEnabled()) onClick(3); break;
            case sf::Keyboard::Return: onClick(1); break;
            case sf::Keyboard::Escape: onClick(0); break;
            default: break;
        }
    }
}
