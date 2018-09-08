#ifndef GUI_H
#define GUI_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <map>
#include "Button.h"
#include <cmath>
#include <string>
#include <iostream>

using namespace std;

// Many typedefs to handlers
typedef void(*GUIDrawFunc)(sf::RenderWindow*);
typedef Button(*GUIMouseClickHandler)(sf::Vector2f);
typedef void(*GUIMouseMoveHandler)(sf::Vector2f);
typedef void(*GUIButtonHandler)(long);
typedef void(*GUILoadHandler)();
typedef void(*GUICloseHandler)();
typedef void(*GUIKeyboardHandler)(char);
typedef void(*GUIDialogFinishHandler)(int, int);

// Struct to store handlers
struct GuiData
{
	GUIDrawFunc draw;
	GUIMouseClickHandler onMouseClick;
	GUIMouseMoveHandler onMouseMove;
	GUIButtonHandler onButtonClick;
	GUILoadHandler load;
	GUICloseHandler close;
	GUIDialogFinishHandler onDialogFinished;

	GuiData(
		GUIDrawFunc drawFunc1,
		GUIMouseClickHandler mouseClickHandler1,
		GUIMouseMoveHandler mouseMoveHandler1,
		GUIButtonHandler buttonHandler1,
		GUILoadHandler loadHandler1,
		GUICloseHandler closeHandler1,
		GUIDialogFinishHandler dialogFinishHandler1
		);

	GuiData() : GuiData(nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr) {}
};

class Gui
{
public:
	/// Add gui to registry
	static void registerGuiHandlers(int guiId, GuiData gui);

	/// Clear registry
	static void clearRegistry();

	/// Find handler by ID
	static const GuiData findHandlerByID(int id);

	/// Called in render phase.
	static void drawGui(sf::RenderWindow* wnd);

	/// Called if the user presses left mouse button. \return Clicked button
	static Button onMouseClick(sf::Vector2f vec);

	/// Called if the user moves mouse (always in GUI ticking phase)
	static void onMouseMove(sf::Vector2f vec);

	/// Called if the user clicks button with specified ID.
	static void onButtonClicked(long button);

	/// Adds specified button pointer to GUI.
	static void addButton(Button& button);

	/// Removes specified Button from GUI.
	static void removeButton(Button& button);

	/// Adds button to dialog
	static void addButtonToDialog(Button& button);

	/// Runs dialog with specified ID
	static void runDialog(int id);

	/// Get dialog return value
	static int getDialogReturnValue();

	/// Draw dialog
	static void drawDialog(sf::RenderWindow* wnd);

	/// Called on load.
	static void onLoad();

	/// Called if the gui is closed.
	static void onClose();

	/// Called if the dialog GUI is finished. Params: dialog ID, return value
	static void onDialogFinished(int d, int rv);

	/// Checks if the dialog is running.
	static bool isDialogRunning() { return runningDialog; }

protected:
	/// Internal GameDisplay drawString() hook.
	static sf::Text drawString(string tx, int height, sf::Vector2f pos, sf::Text::Style style = sf::Text::Regular);

	/// Set dialog return value
	static void setDialogReturnValue(int rv);

private:
	/// buttons :)
	static vector<Button*> buttons;
	static vector<Button*> dialogButtons;

	/// GuiData handlers registry
	static map<int, GuiData> registry;

	static void drawNFG(sf::RenderWindow* wnd);
	static Button mouseNFG(sf::Vector2f pos);

	static bool runningDialog;
	static int dialogId;
	static int dialogRv;
};

#endif // GUI_H