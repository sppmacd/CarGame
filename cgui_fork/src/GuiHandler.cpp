#include <CG/GuiHandler.h>
#include <CG/Widget.h>

namespace colors
{
    Color buttonColor(119, 103, 50);
    Color buttonOutline(60, 51, 25);
    Color buttonDisabledColor(60, 51, 25);
    Color buttonMouseOverColor(240, 206, 100);
    int textSize = 28;

    Color guiYesNoColor(110, 100, 65);
    Color guiYesNoOutline(55, 50, 33);

    Color stringColor(255, 255, 255);
    Color bgColor(60,60,60);

    Color textBoxColor(105, 105, 105);
    Color textBoxOutline(128, 128, 128);
    Color textBoxFocusOutline(170, 170, 170);
    Color textBoxDisabledColor(60, 51, 25);
    Color textBoxTooltipColor(250, 250, 250);
}

namespace cg
{
bool ev(Event) {return false;}

GuiHandler::GuiHandler() : GuiHandler(NULL,NULL)
{

}
Vector2f GuiHandler::getSize()
{
    return guiView.getSize();
}

RenderWindow* GuiHandler::getWindow()
{
    return window;
}

GuiHandler::GuiHandler(RenderWindow* wnd, Font* font)
{
    window = NULL;
    guiFont = NULL;
    returnValue = 0;
    externalWindow = true;
    externalFont = true;
    displayedGui = NULL;
    guiToDisplay = NULL;
    Gui::setGUIHandler(this);
    setEventHandler(ev);

    if(wnd != NULL)
    {
        window = wnd;
    }

    if(font != NULL)
    {
        guiFont = font;
    }

    guiView = wnd->getDefaultView();
}

bool GuiHandler::loadFont(String fileName)
{
    if(!externalFont)
        delete guiFont;

    externalFont = false;
    guiFont = new Font;
    if(!guiFont->loadFromFile(fileName))
    {
        delete guiFont;
        guiFont = NULL;
        externalFont = true;
        return false;
    }
    return true;
}

void GuiHandler::setFont(Font* font)
{
    if(font != NULL)
        externalFont = true;
    else
        externalFont = false;

    guiFont = font;
}

int GuiHandler::runTick()
{
    returnValue = -1;
    if(window->isOpen())
    {
        Event event;

        initializeGui(); // display new GUI if set

        while(window->pollEvent(event))
        {
            handleEvent(event);
        }

        if(isGuiLoaded())
            displayedGui->onUpdate();
    }
    return returnValue;
}

void GuiHandler::setEventHandler(EventHandler handler)
{
    eventHandler = handler;
}

void GuiHandler::close(int ret)
{
    returnValue = ret;
    running = false;
    // onClose(ret); //Called by close(). Note that the current GUI will be displayed once so you cannot delete here e.g textures.
}

void GuiHandler::handleEvent(Event& event)
{
    if(event.type == Event::MouseMoved && !eventHandler(event))
    {
        Vector2i mousepos(event.mouseMove.x, event.mouseMove.y);
        tickEventMouseMove(window->mapPixelToCoords(mousepos, guiView));
    }
    else if(event.type == Event::MouseButtonPressed && !eventHandler(event))
    {
        Vector2i mousepos(event.mouseButton.x, event.mouseButton.y);
        tickEventMouseClick(window->mapPixelToCoords(mousepos, guiView), false, event.mouseButton.button);
    }
    else if(event.type == Event::MouseButtonReleased && !eventHandler(event))
    {
        Vector2i mousepos(event.mouseButton.x, event.mouseButton.y);
        tickEventMouseClick(window->mapPixelToCoords(mousepos, guiView), true, event.mouseButton.button);
    }
    else if(event.type == Event::KeyPressed && !eventHandler(event) && isGuiLoaded())
    {
        displayedGui->onKeyboardBase(event.key.code, false, event.key.alt, event.key.shift, event.key.control, event.key.system);
    }
    else if(event.type == Event::KeyReleased && !eventHandler(event) && isGuiLoaded())
    {
        displayedGui->onKeyboardBase(event.key.code, true, event.key.alt, event.key.shift, event.key.control, event.key.system);
    }
    else if(event.type == Event::TextEntered && !eventHandler(event) && isGuiLoaded())
    {
        displayedGui->onTextBase(event.text.unicode);
    }
    else if(event.type == Event::Resized && !eventHandler(event))
    {
        FloatRect visibleArea(0.f, 0.f, event.size.width, event.size.height);
        guiView = View(visibleArea);
        window->setView(guiView);

        if(isGuiLoaded())
        {
            displayedGui->reloadOnResize();
        }
    }
    else if(event.type == Event::Closed && !eventHandler(event))
    {
        close(0);
    }
    else
    {
        eventHandler(event);
    }
}
bool GuiHandler::isGuiLoaded()
{
    return displayedGui != NULL;////////////////////////////////////////
}

void GuiHandler::drawGui(bool fullRender, View gameView)
{
    if(fullRender)
    {
        window->clear(colors::bgColor);
    }
    if(isGuiLoaded() && guiFont != NULL)
    {
        window->setView(guiView);

        this->displayedGui->drawAll(*window);

        if(gameView.getSize() != Vector2f(0.f,0.f))
            window->setView(gameView);
		else
			window->setView(guiView);
    }
    if(fullRender)
    {
        window->display();
    }
}

Vector2f GuiHandler::getCenter()
{
    return guiView.getSize() / 2.f;
}

Gui* GuiHandler::getCurrentGUI()
{
    return displayedGui;
}

Font* GuiHandler::getFont()
{
    return guiFont;
}

RenderWindow* GuiHandler::createWindow(int xSize, int ySize, String title, bool fullscreen)
{
    if(!externalWindow)
        delete window;

    externalWindow = false;
    window = new RenderWindow(fullscreen ? VideoMode::getFullscreenModes()[0] : VideoMode(xSize, ySize, 32), title, fullscreen ? Style::Fullscreen : Style::Default);

    FloatRect visibleArea(0.f, 0.f, window->getSize().x, window->getSize().y);
    guiView = View(visibleArea);

    return window;
}

GuiHandler::~GuiHandler()
{
    if(!externalFont)
        delete guiFont;
    if(!externalWindow)
        delete window;
}

void GuiHandler::tickEventMouseMove(Vector2f pos)
{
    if(this->isGuiLoaded())
    {
		this->displayedGui->onMouseMove(pos);
    }
}

void GuiHandler::tickEventMouseClick(Vector2f pos, bool release, Mouse::Button button)
{
    if(this->isGuiLoaded())
    {
		Widget* b = this->displayedGui->onMouseClick(pos, release, button);
		if(b != nullptr && release && button == Mouse::Left)
        {
            this->displayedGui->onButton(b->getID());
        }
    }
}

void GuiHandler::displayGui(Gui* gui, bool init)
{
    guiToDisplay = gui;
    newGuiSet = true;
    if(init)
        initializeGui();
}

void GuiHandler::closeGui()
{
	if(isGuiLoaded())
	{
	    displayedGui->onClose();
	    delete displayedGui;
		displayedGui = nullptr;
	}
}

void GuiHandler::initializeGui()
{
	if(guiToDisplay != nullptr && newGuiSet)
	{
	    this->closeGui(); //Close previous GUI
	    displayedGui = guiToDisplay;
	    displayedGui->onLoad();
	    displayedGui->onResize();
	    newGuiSet = false;
	}
}

int GuiHandler::run()
{
    running = true;
    // onLoad();
    while(running)
    {
        int rv = runTick();

        if(rv != -1)
            getWindow()->close();

        drawGui();
    }
    // onFinish(); // Called at the end of the program.
    return returnValue;
}
}
