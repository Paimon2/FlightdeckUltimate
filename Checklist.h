#pragma once
#include "json.hpp"
#include "fixed_string.hpp"
#include "GlobalVars.h"
#include <vector>
#include <XPLMDataAccess.h>
#include <sstream>


#ifdef max
#undef max
#endif


enum ChecklistType {
	BeforeStart = -2,
	BeforeTaxi = -1,
	BeforeTO = 0,
	AfterTO = 1,
	Descent = 2,
	Approach = 3,
	Landing = 4,
	Shutdown = 5,
};

enum ComparisonType {
	EqualTo = 0,
	LessThan = 1,
	GreaterThan = 2,
	NotEqualTo = 3,
};

enum DatarefType {
	UnableToDetermine = -1,
	Int = 0,
	Float = 1,
	Double = 2,
};

class DataRefValue {
public:
	XPLMDataRef handle;
	std::string name;
	double value;
	DatarefType type;
	ComparisonType comparisonType;
};

class ChecklistItem {
public:
	std::string itemName;
	std::string itemResponse;
	DataRefValue refVal;
};


class Checklist {
public:
	std::string checklistName;
	ChecklistType type;
	int currentIndex;
	std::vector<ChecklistItem> items;

	// Not thread safe
	inline void findDatarefs() {

	}

	inline void parseChecklistFromJSON(const nlohmann::json &j, const std::string &checklistName_) {

		currentIndex = -1;
		checklistName = checklistName_;
		nlohmann::json data = j[checklistName];

		int p = 0;
		for (nlohmann::json::iterator it = data.begin(); it != data.end(); ++it)
		{
			p++;
			std::ostringstream oss;
			ChecklistItem item;

		   item.itemName = it.key();

		   oss.str("");
			oss.clear();
			oss << it.key();
			std::string val = oss.str();

			oss.str("");
			oss.clear();
			oss << j[checklistName][val]["readback"];
			item.itemResponse = oss.str();
			item.itemResponse.erase(0, 1);
			item.itemResponse.pop_back();
	

			oss.str("");
			oss.clear();
			oss << j[checklistName][val]["dataref"];
			item.refVal.name = oss.str();
			item.refVal.name.pop_back();
			item.refVal.name.erase(0, 1);
			

			oss.str("");
			oss.clear();
			oss << j[checklistName][val]["condition"];
			std::string unparsedCondition = oss.str();
			unparsedCondition.erase(0, 1);
			unparsedCondition.pop_back();

			if (unparsedCondition[0] == '=') item.refVal.comparisonType == ComparisonType::EqualTo;
			else if (unparsedCondition[0] == '>') item.refVal.comparisonType == ComparisonType::GreaterThan;
			else if (unparsedCondition[0] == '<') item.refVal.comparisonType == ComparisonType::LessThan;
			else if (unparsedCondition[0] == '!') item.refVal.comparisonType == ComparisonType::NotEqualTo;
			else {
				pluginLogger << "Error parsing dataref condition; no comparison operator found in " + it.key();
				pluginLogger << "Some or all elements of this checklist will not be available.";
				return;
			}

			unparsedCondition.erase(0, 1);
			std::string p = unparsedCondition;
			item.refVal.value = atof(unparsedCondition.c_str());
			items.push_back(item);

		}
		
		/*Convert all underscores to spaces and make first letter uppercase*/
		std::replace(checklistName.begin(), checklistName.end(), '_', ' ');
		checklistName[0] = toupper(checklistName[0]);
	}

};

extern bool inChecklistMode;
extern Checklist currentChecklist;
extern Checklist currentChecklist;
extern Checklist beforeTaxiChecklist;
extern Checklist beforeTakeoffChecklist;
extern Checklist beforeStartChecklist;
extern Checklist approachChecklist;
extern Checklist descentChecklist;
extern Checklist landingChecklist;
extern ChecklistItem currentItem;
extern void nextChecklistItem();
