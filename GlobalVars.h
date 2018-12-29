#pragma once
#include <queue>
#include <deque>
#include <vector>
#include <SFML/Audio.hpp>
#include "Datarefs.h"
#include "Logger.h"
#define VERSION_S "1.0.0"
#define CURRENT_VERSION_N 100

extern char dirSep[2];
extern Logger pluginLogger;


extern std::queue<DatarefActionFloat> floatRefsToSet;
extern std::queue<DatarefActionInt> intRefsToSet;
extern std::vector<XPLMCommandRef> commandsToExecute;
extern std::vector<std::string> soundsToPlay;
extern std::string wordToFile(std::string word);
extern void playAllSounds();
extern int currentSoundIndex;
extern sf::Music *currentSound;

static  float	color[] = { 1.0, 1.0, 1.0 }; // RGB white
static  float	white[] = { 1.0, 1.0, 1.0 }; // RGB white
static  float	grey[] = { 169.0f, 169.0f, 169.0f }; // RGB grey
static  float	tpr[] = { 0.0f, 191.0f, 255.0f }; // RGB deep sky blue + important text color
static  float	yellow[] = { 255.0f, 255.0f, 0.0f }; 
static float	black[] = { 0.0, 0.0, 0.0 }; // RGB black
static float	red[] = { 255.0, 0.0, 0.0 }; // RGB red

inline int	CoordGUIInRect(float mouse_x, float mouse_y, float x, float y, float w, float h)
{
	return ((mouse_x >= x && mouse_x < (x + w)) && mouse_y >= y && mouse_y < (y + h));
}

inline int	CoordGUIInRectFlippedAll(float mouse_x, float mouse_y, float x, float y, float w, float h)
{
	return ((mouse_x <= x && mouse_x >(x - w)) && mouse_y <= y && mouse_y >(y - h));
}

inline int	CoordGUIInRectFlippedX(float mouse_x, float mouse_y, float x, float y, float w, float h)
{
	return ((mouse_x <= x && mouse_x > (x - w)) && mouse_y >= y && mouse_y < (y + h));
}

inline int	CoordGUIInRectFlippedY(float mouse_x, float mouse_y, float x, float y, float w, float h)
{
	return ((mouse_x >= x && mouse_x < (x + w)) && mouse_y <= y && mouse_y >(y - h));
}

inline int checkCollide(int rect1_x, int rect1_y, int rect2_x, int rect2_y, int rect1_w, int rect1_h, int rect2_w, int rect2_h)
{


	return (
		CoordGUIInRect(rect1_x, rect1_y, rect2_x, rect2_y, rect2_w, rect2_h) ||
		CoordGUIInRectFlippedAll(rect1_x + rect1_w, rect1_y + rect1_h, rect2_x + rect2_w, rect2_y + rect2_h, rect2_w, rect2_h) ||
		CoordGUIInRectFlippedX(rect1_x + rect1_w, rect1_y + rect1_h, rect2_x + rect2_w, rect2_y + rect2_h, rect2_w, rect1_h) ||
		CoordGUIInRectFlippedY(rect1_x + rect1_w, rect1_y + rect1_h, rect2_x + rect2_w, rect2_y + rect2_h, rect1_w, rect2_h));

}
