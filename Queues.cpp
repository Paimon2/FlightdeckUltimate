#include <queue>
#include <deque>
#include <vector>
#include <XPLMDataAccess.h>
#include <XPLMUtilities.h>
#include <functional>
#include "Datarefs.h"


std::queue<DatarefActionFloat> floatRefsToSet;
std::queue<DatarefActionInt> intRefsToSet;
std::vector<XPLMCommandRef> commandsToExecute;
std::vector<std::string> soundsToPlay;
