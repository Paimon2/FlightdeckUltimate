#include "Widgets.h"
#include "GlobalVars.h"
#include "XPLMGraphics.h"
#include "XPLMDisplay.h"
/**
*	XP11.10+/MCP-Passengers/Flightdeck Ultimate
*   OpenGL and XPLM Widget Library
*   Widgets.cpp
*   @version 1.4 9/12/18 © Transonic Simulations
*/

int textBoxBlinker;



void drawButtonHandler(int state, int mouse_x, int mouse_y, int button_x_pos, int button_y_pos, int width, int height)
{
	if (CoordGUIInRect(mouse_x, mouse_y, button_x_pos, button_y_pos, width, height)
		&& state != 2)
	{
		drawTexture(button_x_pos, button_y_pos, width, height, button_med_hover);
	}
	else if (CoordGUIInRect(mouse_x, mouse_y, button_x_pos, button_y_pos, width, height)
		&& state == 2)
	{
		drawTexture(button_x_pos, button_y_pos, width, height, button_med_clicked);
	}
	else
	{
		drawTexture(button_x_pos, button_y_pos, width, height, button_med);
	}
}


Button::Button(fixed_string<32> texti, int xi, int yi, int widthi, int heighti)
{
	snprintf(text, 32, "%s", texti);
	buttonState = 0;
	x = xi;
	y = yi;
	width = widthi;
	height = heighti;

}


Button::~Button() {
	memset(text, '\0', sizeof(char)*strlen(text));
	x = 0;
	y = 0;
	width = 0;
	height = 0;
}


void Button::draw(int mouse_x, int mouse_y, int window_x, int window_y)
{
	
	drawButtonHandler(buttonState, mouse_x, mouse_y, window_x + x, window_y + y, width, height);
	XPLMDrawString(color, window_x + x, window_y + y + 10, text, NULL, xplmFont_Proportional);
}


bool Button::onMouseEvent(int mouse_x, int mouse_y, XPLMMouseStatus is_down, int window_x, int window_y)
{
	if (CoordGUIInRect(mouse_x, mouse_y, window_x + x, window_y + y, width, height) && is_down == xplm_MouseDown) { buttonState = 2; }
	else if (CoordGUIInRect(mouse_x, mouse_y, window_x + x, window_y + y, width, height) && is_down == xplm_MouseUp) { buttonState = 0; return 1; }
	if (is_down == xplm_MouseUp) { buttonState = 0; }
	return 0;
}


TextBox::TextBox(int xi, int yi, int widthi, int heighti, unsigned maxleni, int modei, bool protecttexti, int textboxtype_)
{
	protectedText = protecttexti;
	mode = modei;
	maxlen = maxleni;
	x = xi;
	y = yi;
	width = widthi;
	height = heighti;
	keyBugFixCounter = 0;
	sprintf(text, "");
	textBoxType = textboxtype_;
}


TextBox::~TextBox()
{
	memset(text, '\0', sizeof(char)*strlen(text));
	// Do nothing else for now
}


void TextBox::draw(int window_x, int window_y)
{
	if (protectedText == 1) {
		char protectedText[32] = "";
		for (int i = 0; i < strlen(text); i++) {
			snprintf(protectedText, 32, "%s%s", protectedText, "*");
		}
		if (hasFocus) {
			textBoxType == 1 ? drawTexture(window_x + x, window_y + y, width, height, text_box_med_highlighted) : drawTexture(window_x + x, window_y + y, width, height, text_box_small_highlighted);
		}
		else {
			textBoxType == 1 ? drawTexture(window_x + x, window_y + y, width, height, text_box_med) : drawTexture(window_x + x, window_y + y, width, height, text_box_small);
		}
		XPLMSetGraphicsState(0, 0, 0, 0, 0, 1, 0); /*disable alpha blending*/
		int ms = XPLMMeasureString(xplmFont_Proportional, protectedText, strlen(protectedText));
		//XPLMDrawString(black, window_x + x + 7, y + window_y + y + 7, protectedText, nullptr, xplmFont_Proportional);
		XPLMDrawString(black, window_x + x + 7, window_y + y + 13, protectedText, nullptr, xplmFont_Proportional);
		if (ms == 2) { memset(text, '\0', sizeof(char)*strlen(text)); }
		XPLMSetGraphicsState(0, 0, 0, 0, 0, 1, 0); /*disable alpha blending*/
		if (textBoxBlinker == 1 && hasFocus) {
			int mxpos = XPLMMeasureString(xplmFont_Proportional, protectedText, strlen(protectedText));;
			if (ms != 0) { mxpos = (window_x + x) + 8 + ms; }
			else { mxpos = (window_x + x) + 7; }
			glLineWidth(1);	glColor3f(0.0f, 0.0f, 0.0f);	glBegin(GL_LINES);
			glVertex2i(mxpos + 1, window_y + y + 5);  glVertex2i(mxpos + 1, window_y + y + height);
			glEnd();
		}
	}
	else {
		if (hasFocus) {
			textBoxType == 1 ? drawTexture(window_x + x, window_y + y, width, height, text_box_med_highlighted) : drawTexture(window_x + x, window_y + y, width, height, text_box_small_highlighted);
		}
		else {
			textBoxType == 1 ? drawTexture(window_x + x, window_y + y, width, height, text_box_med) : drawTexture(window_x + x, window_y + y, width, height, text_box_small);
		}
		XPLMSetGraphicsState(0, 0, 0, 0, 0, 1, 0); /*disable alpha blending*/
		int ms = XPLMMeasureString(xplmFont_Proportional, text, strlen(text));
		//XPLMDrawString(black, window_x + x + 7, y + window_y + y + 7, text, nullptr, xplmFont_Proportional);
		XPLMDrawString(black, window_x + x + 7, window_y + y + 13, text, nullptr, xplmFont_Proportional);
		if (ms == 2) { memset(text, '\0', sizeof(char)*strlen(text)); }
		XPLMSetGraphicsState(0, 0, 0, 0, 0, 1, 0); /*disable alpha blending*/
		if (textBoxBlinker == 1 && hasFocus) {
			int mxpos = XPLMMeasureString(xplmFont_Proportional, text, strlen(text));;
			if (ms != 0) { mxpos = (window_x + x) + 8 + ms; }
			else { mxpos = (window_x + x) + 7; }
			glLineWidth(1);	glColor3f(0.0f, 0.0f, 0.0f);	glBegin(GL_LINES);
			glVertex2i(mxpos + 1, window_y + y + 5);  glVertex2i(mxpos + 1, window_y + y + height);
			glEnd();
		}
	}
}


bool TextBox::onMouseEvent(int mouse_x, int mouse_y, int window_x, int window_y)
{
	if (CoordGUIInRect(mouse_x, mouse_y, window_x + x, window_y + y, width, height)) {
		hasFocus = 1;
		return 1;
	}
	else {
		hasFocus = 0;
		XPLMTakeKeyboardFocus(0);
		return 0;
	}

}


char* TextBox::getText()
{
	return text;
}

bool TextBox::hasMKFocus()
{
	return hasFocus;
}


void TextBox::setText(char texti[32])
{
	snprintf(text, maxlen, "%s", texti);
}


void TextBox::sendKey(char key, char virtual_key)
{
	if (hasFocus) {
		int ms = XPLMMeasureString(xplmFont_Proportional, text, strlen(text));
		if (virtual_key == XPLM_VK_BACK && strlen(text) != 0) {
			keyBugFixCounter++;
			if (keyBugFixCounter == 2) { text[strlen(text) - 1] = '\0'; keyBugFixCounter = 0; }
		}

		
		if (mode == 1) { /*Mode 1 - integers only*/
			if (virtual_key != XPLM_VK_BACK && isdigit(key) && key != 0 && strlen(text) < maxlen) {
				keyBugFixCounter++;
				if (keyBugFixCounter == 2) {
					sprintf(text, "%s%c", text, key); keyBugFixCounter = 0;
				}
			}
		}

		else if (mode == 2) { /*Mode 2 - characters only*/
			if (virtual_key != XPLM_VK_BACK && !isdigit(key) && key != 0 && strlen(text) < maxlen) {
				keyBugFixCounter++;
				if (keyBugFixCounter == 2) {
					sprintf(text, "%s%c", text, key); keyBugFixCounter = 0;
				}
			}
		}

		else if (mode == 0) { /*Mode 0 - default mode - characters and numbers*/
			if (virtual_key != XPLM_VK_BACK && key != 0 && strlen(text) < maxlen) {
				keyBugFixCounter++;
				if (keyBugFixCounter == 2) {
					sprintf(text, "%s%c", text, key); keyBugFixCounter = 0;
				}
			}
		}
	}
}

Checkbox::Checkbox(int xi, int yi, int widthi, int heighti, short statei)
{
	x = xi;
	y = yi;
	width = widthi;
	height = heighti;
	state = statei;
}

Checkbox::~Checkbox()
{
}

void Checkbox::draw(int window_x, int window_y)
{
	state == 0 ? drawTexture(window_x + x, window_y + y - 10, width, height, checkbox) : drawTexture(window_x + x, window_y + y - 10, width, height, checkbox_checked);
}

bool Checkbox::onMouseEvent(int mouse_x, int mouse_y, XPLMMouseStatus is_down, int window_x, int window_y)
{
	if (CoordGUIInRect(mouse_x, mouse_y, window_x + x - 5, window_y + y - 5, width + 5, height + 5) && is_down == xplm_MouseUp) {
		state == 0 ? state = 1 : state = 0;
		return true;
	}
	return false;
}

Slider::Slider(int x, int y, int width, int height, float value)
{
}

void Slider::draw(int mouse_x, int mouse_y, int window_x, int window_y)
{
	XPLMSetGraphicsState(0, 0, 0, 0, 0, 1, 0);
	glColor3ub(255, 0, 0);
	//drawRectangle(window_x + x, y + window_y, x + window_x + width, window_y + y + height);
	drawRectangle(window_x + x, window_y + y, window_x + width + x, y + window_y + height);

	//glColor3ub(192, 192, 192);
	//drawRectangle(window_x + x + ((value / 1.0f) * width), window_y + y, window_y + y + height, window_x + x + ((value / 1.0f) * width) + 20);
}


bool Slider::onMouseEvent(int mouse_x, int mouse_y, XPLMMouseStatus is_down, int window_x, int window_y)
{
	if (CoordGUIInRect(mouse_x, mouse_y, window_x + x, window_y + y, width, height) && is_down == xplm_MouseUp) {

	}

	if (CoordGUIInRect(mouse_x, mouse_y, window_x + x, window_y + y, width, height) && is_down == xplm_MouseUp) {

	}

	return false;
}

DropdownMenu::DropdownMenu(int x_, int y_, int width_, int height_)
{
	x = x_;
	y = y_;
	width = width_;
	height = height_;
	
}

bool DropdownMenu::onMouseEvent(int mouse_x, int mouse_y, XPLMMouseStatus is_down, int window_x, int window_y)
{
	if (CoordGUIInRect(mouse_x, mouse_y, window_x + x, window_y + y, width, height)) {
		isExtended = true;
	
	}

	if(isExtended && is_down == xplm_MouseUp) {

		int iterations = 0;
		for (auto &item : items) {
			iterations++;
		
			if (CoordGUIInRect(mouse_x, mouse_y, window_x + x, window_y + y + 7 - (iterations * 18), width, ((iterations * 2) + 7))) {
				setItem(item);
				isExtended = false;
				return true;
			}
		}
		}

	
	return false;
}

void DropdownMenu::draw(int mouse_x, int mouse_y, int window_x, int window_y)
{
	try {
		XPLMSetGraphicsState(0, 0, 0, 0, 0, 1, 0);
		glColor3ub(0, 0, 0);
		drawRectangle(window_x + x, y + window_y, x + window_x + width, y + window_y + height);
		XPLMDrawString(white, window_x + x + 7, window_y + y + 7, items.at(0).name, nullptr, xplmFont_Proportional);

		if (isExtended) {
			int iterations = 0;
			for (auto &item : items) {
				iterations++;
				//XPLMSetGraphicsState(0, 0, 0, 0, 0, 1, 0);
				
				//todo fix coords testing
				if (CoordGUIInRect(mouse_x, mouse_y, window_x + x, window_y + y + 7 - (iterations * 18), width, ((iterations * 2) + 7))) {
			
				glColor4ub(0, 0, 0, 130);
				drawRectangle(window_x + x, y + window_y - (iterations * 18), x + window_x + width, y + window_y + height - ((iterations * 18) * 1.0f));
				glColor4ub(0, 238, 238, 140); 
				drawRectangle(window_x + x, y + window_y - (iterations / 18), x + window_x + width, y + window_y - ((iterations / 18) * 1.0f));
			
				}
				else {
					glColor4ub(0, 0, 0, 130);
					drawRectangle(window_x + x, y + window_y - (iterations * 18), x + window_x + width, y + window_y + height - ((iterations * 18) * 1.0f));
				}
				drawRectangle(window_x + x, y + window_y - (iterations * 18), x + window_x + width, y + window_y + height - ((iterations * 18) * 1.0f));
			

				XPLMDrawString(white, window_x + x + 7, window_y + y + 7 - (iterations * 18), item.name, nullptr, xplmFont_Proportional);


			}
		}

	
	}
		
	
	catch (std::out_of_range) {
		pluginLogger << "Vector element out of range.";
	}

}

void DropdownMenu::addItem(fixed_string<128> name)
{
	DropdownMenuItem item;
	item.name = name;
	item.isMouseOver = false;

	items.push_back(item);
}


void DropdownMenu::setItem(DropdownMenuItem item)
{
	for (int i = 0; i < items.size(); i++) {
		if (item.name == items.at(i).name) {
			std::swap(items[i], items[0]);
		}
	}

	//items.push_back(item);

}

void DropdownMenu::removeItem(int index)
{
}
