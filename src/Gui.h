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
using namespace sf;

class Gui
{
public:
    Gui();
    virtual ~Gui();

    /// current dialog id, unique for each GUI
	int dialogCallId;

	/// current dialog return value
	int dialogReturnValue;

	/// Called in render phase.
	virtual void onDraw(RenderWindow& wnd);

	/// Called if the user presses left mouse button. \return Clicked button
	virtual Button onMouseClick(Vector2f pos);

	/// Called if the user moves mouse (always in GUI ticking phase)
	virtual void onMouseMove(Vector2f pos);

	/// Called if the user clicks button with specified ID.
	virtual void onClick(long buttonId);

	/// Adds specified button pointer to GUI.
	virtual void addButton(Button& widget);

	/// Removes specified Button from GUI.
	virtual void removeButton(Button& widget);

	/// Runs dialog with specified ID
	virtual void runDialog(Gui* dialog, int callId);

	/// Called on load.
	virtual void onLoad();

	/// Called if the gui is closed.
	virtual void onClose();

	/// Called when the keyboard key is pressed.
	virtual void onKeyboard(Keyboard::Key key);

	/// Called if the dialog GUI is finished. Params: dialog ID, return value
	virtual void onDialogFinished(Gui* dialog, int callId);

	/// Called by dialog function, calls onDialogFinished (child closes)
	virtual void close(int returnValue);

	/// Closes dialog by GUI (parent closes)
	virtual void closeDialog(int returnValue);

	/// Called by Game when the button is clicked.
	void onButton(int buttonId);

	/// Checks if the dialog is running.
	bool isDialogRunning();

protected:
	/// Internal GameDisplay drawString() hook.
	sf::Text drawString(String tx, int height, sf::Vector2f pos, sf::Text::Style style = sf::Text::Regular);
	Gui* parent;

private:
	/// buttons :)
	vector<Button*> buttons;
	Gui* currentDialog;
};

#endif // GUI_H
