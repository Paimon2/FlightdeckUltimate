#include "glfunctions.h"
#include <vector>
#include "fixed_string.hpp"

#pragma once
/**
*	XP11/MCP-Passengers OpenGL and XPLM Widget Library
*   Widgets.h
*   @version 1.3 25/11/18 onwards © Transonic Simulations
*	ORIGINAL COPYRIGHT VOID
*/

#define FOME(window) XPLMTakeKeyboardFocus(window); return 1;

extern int textBoxBlinker;

/*Standard X-Plane style light blue button*/
class Button {
private:
	char text[32];
	short buttonState;
	int x;
	int y;
	int width;
	int height;
public:
	/**
	* Constructs a button.
	* @param texti The text which is to be drawn on top of this button
	* @param xi The position of this object (in boxels) from the left edge of the window
	* @param yi The position of this object (in boxels) from the bottom edge of the window
	* @param widthi The width of this object (optional, default value = 150)
	* @param heighti The height of this object (optional, default value = 25)
	* @return Nothing
	*/
	Button(fixed_string<32> texti, int xi, int yi, int widthi = 150, int heighti = 25);
	/**
	* Button destructor
	*/
	~Button();
	/**
	* Draws a button
	* Pass in the arguments as you get them from XPLMGetMouseLocationGlobal() and XPLMGetWindowGeometry()
	*/
	void draw(int mouse_x, int mouse_y, int window_x, int window_y);
	/**
	* Sends a mouse down or up event to the button.
	@return true if button has been clicked, else false
	*/
	bool onMouseEvent(int mouse_x, int mouse_y, XPLMMouseStatus is_down, int window_x, int window_y);
};

/*Standard generic/X-Plane style text box*/
class TextBox {
private:
	char text[32];
	int keyBugFixCounter;
	unsigned maxlen;
	bool protectedText;
	bool hasFocus;
	int mode;
	int textBoxType;
	int x;
	int y;
	int width;
	int height;
public:
	/**
	* Constructs a text box.
	* @param xi The position of this object (in boxels) from the left edge of the window
	* @param yi The position of this object (in boxels) from the bottom edge of the window
	* @param widthi The width of this object (optional, default value = 90)
	* @param heighti The height of this object (optional, default value = 25)
	* @param maxleni The maximum amount of characters allowed in the text box (optional, default value = 8)
	* @param modei The text box mode. 0 = allow characters and numbers, 1 = numbers only, 2 = characters only. (optional, default value = 0)
	* @param protecttexti Whether to protect text with the * (asterisk) special character (optional, default value = 0)
	* @param textboxtype_ The type of the text box - 0 = small, 1 = exceptionally ridiculously long (optional, default value = 0)
	* @return Nothing
	*/
	TextBox(int xi, int yi, int widthi = 90, int heighti = 25, unsigned maxleni = 8, int modei = 0, bool protecttexti = 0, int textboxtype_ = 0);
	/**
	* Text box destructor
	*/
	~TextBox();
	/**
	* Draws a text box, including the caret (blinker) if it has focus.
	* Pass in the arguments as you get them from XPLMGetWindowGeometry()
	*/
	void draw(int window_x, int window_y);
	/**
	* Sends a mouse down or up event to the text box.
	* IMPORTANT: IF THIS IS TRUE, YOU MUST DO XPLMTakeKeyboardFocus(Window_ID);
	* Otherwise it will not work!
	* Automatically returns keyboard focus to XP if mouse location outside bounds
	* @return true if button has been clicked, else false
	*/
	bool onMouseEvent(int mouse_x, int mouse_y, int window_x, int window_y);
	/**
	* Sends a keystroke to the text box.
	* Pass in the arguments as you get them from the XPLM keystroke callback.
	*/
	void sendKey(char key, char virtual_key);
	/**
	* Gets text of the text box.
	* @return A char array containing the text (alternatively use fixed_string<size>)
	*/
	char* getText();
	/**
	* Tells us if the text box has keyboard and mouse focus.
	* @return A boolean indicating whether or not the text box in question has focus
	*/
	bool hasMKFocus();
	/**
	* Sets text of the text box.
	* @param text A char array of the text to set (alternatively use fixed_string<size>)
	*/
	void setText(char texti[32]);
};



class Checkbox
{
private:
	int x;
	int y;
	int width;
	int height;
public:
	/**
	* The state of the text box
	* Values: (0 = unchecked), (1 = mouse held down), (>=2 = checked)
	*/
	short state;
	
	/**
	* Constructs a checkbox.
	* @param xi The position of this object (in boxels) from the left edge of the window
	* @param yi The position of this object (in boxels) from the bottom edge of the window
	* @param widthi The width of this object (optional, default value = 30)
	* @param heighti The height of this object (optional, default value = 25)
	* @param maxleni The maximum amount of characters allowed in the text box (optional, default value = 8)
	* @param state Whether to be checked or unchecked by default (optional, default value = 0)
	* @return Nothing
	*/
	Checkbox(int xi, int yi, int widthi = 30, int heighti = 25, short statei = 0);
	/**
	* Checkbox destructor
	*/
	~Checkbox();
	/**
	* Draws a checkbox
	* Pass in the arguments as you get them from XPLMGetMouseLocationGlobal() and XPLMGetWindowGeometry()
	*/
	void draw(int window_x, int window_y);
	/**
	* Returns whether or not a mouse up or down event has been triggered.
	@return true if button has been clicked, else false
	*/
	bool onMouseEvent(int mouse_x, int mouse_y, XPLMMouseStatus is_down, int window_x, int window_y);

};

struct DropdownMenuItem {
	fixed_string<128> name;
	bool isMouseOver;
};

class DropdownMenu {
private:
	int x;
	int y;
	int width;
	int height;
public:
	bool isExtended;
	unsigned long selectedIndex;
	std::vector<DropdownMenuItem> items;
	DropdownMenu(int x_, int y_, int width_ = 100, int height_ = 25);
	/**
	* Draws the slider
	* Pass in the arguments as you get them from XPLMGetMouseLocationGlobal() and XPLMGetWindowGeometry()
	*/
	bool onMouseEvent(int mouse_x, int mouse_y, XPLMMouseStatus is_down, int window_x, int window_y);
	void setItem(DropdownMenuItem item);
	void draw(int mouse_x, int mouse_y, int window_x, int window_y);
	void addItem(fixed_string<128> name);
	void removeItem(int index);
};




class Slider {
private:
	int x;
	int y;
	int width;
	int height;
public:
	float value;
	int sliderState;
	Slider(int x, int y, int width, int height, float value = 0.50);
	/**
	* Draws the slider
	* Pass in the arguments as you get them from XPLMGetMouseLocationGlobal() and XPLMGetWindowGeometry()
	*/
	void draw(int mouse_x, int mouse_y, int window_x, int window_y);
	/**
	* Returns whether or not a mouse up or down event has been triggered.
	@return true if button has been clicked, else false
	*/
	bool onMouseEvent(int mouse_x, int mouse_y, XPLMMouseStatus is_down, int window_x, int window_y);
};