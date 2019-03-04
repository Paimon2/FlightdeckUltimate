#include "Recognizer.h"
#include "GlobalVars.h"
#include "GUI.h"
#include "Checklist.h"
#include <thread>
#include <future>
#include "Datarefs.h"
#include <fstream>

#define MODELDIR "tests"


Recognizer defaultRecognizer;


void processSpeechText(std::string speech, Recognizer &recog);

void initRecognizerFunc(std::string device, Recognizer &recognizer) {

	recognizer.mu.lock();
	std::string hmmDir;
	hmmDir.clear();
	hmmDir.append("Resources").append(dirSep).append("plugins").append(dirSep).append("Flightdeck Ultimate").append(dirSep).append("model").append(dirSep).append("en-us").append(dirSep).append("en-us");
	std::string lmDir;
	lmDir.clear();
	lmDir.append("Resources").append(dirSep).append("plugins").append(dirSep).append("Flightdeck Ultimate").append(dirSep).append("model").append(dirSep).append("en-us").append(dirSep).append("flightdeckultimate_R1.lm.bin");
	std::string dictDir;
	dictDir.clear();
	dictDir.append("Resources").append(dirSep).append("plugins").append(dirSep).append("Flightdeck Ultimate").append(dirSep).append("model").append(dirSep).append("en-us").append(dirSep).append("flightdeckultimate_R1.dict");




	recognizer.config = cmd_ln_init(NULL, ps_args(), TRUE,
		"-hmm", hmmDir.c_str(),
		"-lm", lmDir.c_str(),
		"-dict", dictDir.c_str(),
		NULL);

	if (recognizer.config == NULL) {
		recognizer.isRecognizerActive = false;
		pluginLogger.logError("Recognizer configuration was empty. Speech-to-text will be inoperative. Is the configuration directory correct?");
		return;
	}

	recognizer.ps = ps_init(recognizer.config);                                                        // initialize the pocketsphinx decoder
	recognizer.ad = ad_open_dev(device.c_str(), (int)cmd_ln_float32_r(recognizer.config, "-samprate")); // open default microphone at default samplerate

	if (recognizer.ps == NULL) {
		recognizer.isRecognizerActive = false;
		pluginLogger.logError("Recognizer audio decoder was NULL. Speech-to-text will be inoperative.");
		return;
	}

	if (recognizer.ad == NULL) {
		recognizer.isRecognizerActive = false;
		pluginLogger.logError("Recognizer audio device was NULL. Speech-to-text will be inoperative. Is your microphone plugged in?");
		return;
	}


	recognizer.mu.unlock();
	pluginLogger << "Speech recognizer initialized successfully.";
}

void recognizeSpeechAsync(Recognizer &recognizer, bool processCommands) {

	while (1) {
		ad_start_rec(recognizer.ad);                                // start recording
		ps_start_utt(recognizer.ps);                                // mark the start of the utterance
		recognizer.utt_started = FALSE;                             // clear the utt_started flag

		while (1) {
			recognizer.k = ad_read(recognizer.ad, recognizer.adbuf, 8192);                // capture the number of frames in the audio buffer
			ps_process_raw(recognizer.ps, recognizer.adbuf, recognizer.k, FALSE, FALSE);  // send the audio buffer to the pocketsphinx decoder

			recognizer.in_speech = ps_get_in_speech(recognizer.ps);            // test to see if speech is being detected

			if (recognizer.in_speech && !recognizer.utt_started) {             // if speech has started and utt_started flag is false                            
				recognizer.utt_started = TRUE;                      // then set the flag
			}

			if (!recognizer.in_speech && recognizer.utt_started) {             // if speech has ended and the utt_started flag is true 
				ps_end_utt(recognizer.ps);                          // then mark the end of the utterance
				ad_stop_rec(recognizer.ad);                         // stop recording
				recognizer.hyp = ps_get_hyp(recognizer.ps, NULL);             // query pocketsphinx for "hypothesis" of decoded statement
				processSpeechText(std::string(recognizer.hyp), recognizer);

				break;                                   // exit the while loop and return to main
			}
		}
	}


}

void recognizeSpeech(Recognizer &recognizer, bool processCommands) {

	recognizer.mu.lock();

	recognizer.listenerCount++;

	ad_start_rec(recognizer.ad);                                // start recording
	ps_start_utt(recognizer.ps);                                // mark the start of the utterance


	while (1) {


		recognizer.k = ad_read(recognizer.ad, recognizer.adbuf, 4096);                // capture the number of frames in the audio buffer
		ps_process_raw(recognizer.ps, recognizer.adbuf, recognizer.k, FALSE, FALSE);  // send the audio buffer to the pocketsphinx decoder

																					  //	recognizer.in_speech = ps_get_in_speech(recognizer.ps);            // test to see if speech is being detected


		if (recognizer.shouldStopRecording == 1 /*!recognizer.in_speech && recognizer.utt_started*/) {             // if speech has ended and the utt_started flag is true 
			ps_end_utt(recognizer.ps);                          // then mark the end of the utterance
			ad_stop_rec(recognizer.ad);                         // stop recording
			recognizer.hyp = ps_get_hyp(recognizer.ps, NULL);             // query pocketsphinx for "hypothesis" of decoded statement

			if (processCommands) {
				// TODO: Process the commands
				//	XPLMSpeakString(recognizer.hyp);

				if (!recognizer.hyp)
					break;

				std::ostringstream oss;
				oss << recognizer.hyp;
				std::string hypString = oss.str();
				processSpeechText(hypString, recognizer);


				recognizer.listenerCount--;
				break;
			}
			recognizer.listenerCount--;
			break;
		}



	}

	recognizer.mu.unlock();
}

void Recognizer::initRecognizer(const std::string &device)
{
	listenerCount = 0;
	std::thread initThread(initRecognizerFunc, device, std::ref(*this));
	initThread.detach();

}

void Recognizer::closeRecognizer()
{
	ad_close(ad);
}

void Recognizer::recognizeAsync() {
	std::thread recognizerThread(recognizeSpeechAsync, std::ref(*this), true);
	recognizerThread.detach();
}

void Recognizer::startListening(bool processCommands)
{
	if (listenerCount == 0) {
		shouldStopRecording = 0;
		std::thread recognizerThread(recognizeSpeech, std::ref(*this), processCommands);
		recognizerThread.detach();

	}

}

void Recognizer::stopListening()
{

	shouldStopRecording = 1;
}


enum NumberUnit {
	Auto = -1,
	Units = 0,
	Tens = 1,
	Hundreds = 2,
	Thousands = 3,
};

void findAndReplaceAll(std::string & data, std::string toSearch, std::string replaceStr)
{
	// Get the first occurrence
	size_t pos = data.find(toSearch);

	// Repeat till end is reached
	while (pos != std::string::npos)
	{
		// Replace this occurrence of Sub String
		data.replace(pos, toSearch.size(), replaceStr);
		// Get the next occurrence from the current position
		pos = data.find(toSearch, pos + toSearch.size());
	}
}

namespace CStringUtils {

	constexpr int npos = std::numeric_limits<int>::max();

	int wordToInt(const char* integer) {

		if (integer == NULL) return npos;

		if (strstr(integer, "ZERO")) return 0;
		if (strstr(integer, "YOU") /*sometimes confused with zero*/) return 0;



		if (strstr(integer, "ELEVEN")) return 11;
		if (strstr(integer, "TWELVE")) return 12;
		if (strstr(integer, "THIRTEEN")) return 13;
		if (strstr(integer, "FOURTEEN")) return 14;
		if (strstr(integer, "FIFTEEN")) return 15;
		if (strstr(integer, "SIXTEEN")) return 16;
		if (strstr(integer, "SEVENTEEN")) return 17;
		if (strstr(integer, "EIGHTEEN")) return 18;
		if (strstr(integer, "NINETEEN")) return 19;


		if (strstr(integer, "TWENTY")) return 20;
		if (strstr(integer, "THIRTY")) return 30;
		if (strstr(integer, "FORTY")) return 40;
		if (strstr(integer, "FIFTY")) return 50;
		if (strstr(integer, "SIXTY")) return 60;
		if (strstr(integer, "SEVENTY")) return 70;
		if (strstr(integer, "EIGHTY")) return 80;
		if (strstr(integer, "NINETY")) return 90;

		if (strstr(integer, "HUNDRED")) return 100;
		if (strstr(integer, "THOUSAND")) return 1000;

		if (strstr(integer, "ONE")) return 1;

		if (strstr(integer, "TWO")) return 2;
		if (strstr(integer, "TO")) return 2;
		if (strstr(integer, "TOO")) return 2;
		if (strstr(integer, "COUPLE")) return 2;

		if (strstr(integer, "THREE")) return 3;
		if (strstr(integer, "FOUR")) return 4;
		if (strstr(integer, "FOR")) return 4;
		if (strstr(integer, "FIVE")) return 5;
		if (strstr(integer, "SIX")) return 6;
		if (strstr(integer, "SEVEN")) return 7;
		if (strstr(integer, "EIGHT")) return 8;
		if (strstr(integer, "NINE")) return 9;
		if (strstr(integer, "NINER")) return 9;
		if (strstr(integer, "TEN")) return 10;




		return npos;
	}




	enum NumberToWordMode {
		IndividualDigits = 0, /*Read the digits one by one. E.g three-six-zero*/
		Altitude = 1, /*Read the digits in altitude format. E.g thirty-six thousand feet*/
		Radios = 2 /*Read the digits in radio format. E.g one-three-five-point-eight-one*/
	};

	void intToWords(int inInteger, NumberToWordMode mode, std::vector<std::string> &outVec, bool addToQueue = false) {


		std::vector<int> digits;

		while (inInteger)
		{
			digits.push_back(inInteger % 10);
			inInteger /= 10;
		}

		std::reverse(digits.begin(), digits.end());




		if (mode == NumberToWordMode::IndividualDigits) {
			for (int i = 0; i < digits.size(); i++) {
				outVec.push_back(std::to_string(digits.at(i)));

			}



			if (addToQueue) {
				for (int i = 0; i < outVec.size(); i++) {
					soundsToPlay.push_back(wordToFile("numbers" + std::string(dirSep) + outVec.at(i)));
					//pluginLogger << outVec.at(i);
				}

			}
		}


	}

	size_t split(const std::string &txt, std::vector<std::string> &strs, char ch)
	{
		size_t pos = txt.find(ch);
		size_t initialPos = 0;
		strs.clear();

		// Decompose statement
		while (pos != std::string::npos) {
			strs.push_back(txt.substr(initialPos, pos - initialPos));
			initialPos = pos + 1;

			pos = txt.find(ch, initialPos);
		}

		// Add the last one
		strs.push_back(txt.substr(initialPos, std::min(pos, txt.size()) - initialPos + 1));

		return strs.size();
	}




	/* 1) Iterate through the speech backwards by spaces
	2) Convert only numbers as (English word) strings to explicit integers
	3) Reverse the integer (e.g 063 becomes 360)
	@param wantedIndividualNumberCount How many individual numbers do we want (e.g 3 in headings as hdgs have 3 numbers - 360 for example)
	*/
	void reverse_number_search(char* speechtext, int &outInteger) {

		outInteger = 0; // extremely important
		std::string nrmlString(speechtext);
		std::vector<std::string> words;
		std::vector<int> numbers;

		split(nrmlString, words, ' ');


		std::string numberString;



		/*First traverse over the list of words and try to convert each **English** word to a native C/C++ integer*/


		for (int i = 0; i < words.size(); i++) {
			std::string word = words.at(i);

			int res = wordToInt(word.c_str());

			if (res != npos) {
				numbers.push_back(res);
			}


		}

		if (numbers.size() == 0) { outInteger = npos; return; }

		/*We now have our numbers in a vector.*/

		


		if (numbers.size() == 1) {
			outInteger = numbers.at(0);
			return;
		}
		for (int i = 0; i < numbers.size(); i++) {

			try {

				if (numbers.at(i) >= 10 && (numbers.at(i + 1) <= 10 || i + 1 > numbers.size())) {
					std::string bothNumberString;
					bothNumberString.clear();
					bothNumberString.append(std::to_string(numbers.at(i)));
					findAndReplaceAll(bothNumberString, "0", "");
					bothNumberString.append(std::to_string(numbers.at(i + 1)));
					numbers.erase(numbers.begin() + i + 1);
					outInteger += stoi(bothNumberString);
				}

				if (numbers.at(i) <= 10 && numbers.at(i + 1) >= 100) {
					numbers.at(i) *= numbers.at(i + 1);
					numbers.erase(numbers.begin() + i + 1);
					outInteger += numbers.at(i);
				}


				if (numbers.at(i) >= 100 && numbers.at(i + 1) <= 99) {
					numbers.at(i) += numbers.at(i + 1) - numbers.at(i); // hacky fix
					numbers.erase(numbers.begin() + i + 1);
					outInteger += numbers.at(i);
				}


				if (numbers.at(i + 1) >= 100) {
					outInteger *= numbers.at(i + 1);
					numbers.erase(numbers.begin() + i + 1);
					//	outInteger += numbers.at(i);
				}

				else {

					std::string bothNumberString;
					bothNumberString.clear();
					bothNumberString.append(std::to_string(numbers.at(i)));
					//	findAndReplaceAll(bothNumberString, "0", "");
					for (int j = 1; j < numbers.size(); j++) { bothNumberString.append(std::to_string(numbers.at(i + j))); }

					numbers.erase(numbers.begin() + i + 1);
					outInteger += stoi(bothNumberString);

				}


			}
			catch (std::out_of_range) {
				break;
			}
		}


	}

	bool word_exists(const char* speechtext, const char* word) {
		// Detects spaces on either side, and return true if exists

		char spaceBefore[48]; // word is hopefully not bigger than 47 chars + null terminator
		char spaceAfter[48]; // word is hopefully not bigger than 47 chars + null terminator

		snprintf(spaceBefore, 48, " %s", word);
		snprintf(spaceAfter, 48, "%s ", word);

		return strstr(speechtext, spaceBefore) || strstr(speechtext, spaceAfter);
	}

}

bool negative_trigger(const char* speechc) {
	return strstr(speechc, "OFF") ||
		strstr(speechc, "DISENGAGE") || 
		strstr(speechc, "DISABLE") || 
		strstr(speechc, "CLOSE");
}

bool positive_trigger(const char* speechc) {
	return strstr(speechc, "ON") ||
		strstr(speechc, "ENGAGE") ||
		strstr(speechc, "ENABLE") || 
		strstr(speechc, "OPEN");
}



/* The one big megafunction.
Started on 3-10-18.

TODO: REBUILD the entire sentence corpus using lmtool:
http://www.speech.cs.cmu.edu/tools/lmtool-new.html

1) Remove the negative_trigger section for speed/altitude bug (line 1278 onwards).
2) Redo the ENTIRE JSON system. It does not meet quality assurance standards.
3) Redo MOST OF the GUI system. It does not meet quality assurance standards.
4) List out all commands in a format that can be easily copied over to our
website. Then compare everything to PlaneCommand's list of commands, and make
changes accordingly.
*/

void processSpeechText(std::string speech, Recognizer& recog) {

	char speechc[512]; // hopefully spoken words will not be longer than 512 chars
	if (speechc == NULL)
		return; 
	if (speechc == nullptr)
		return; 
	if (strlen(speechc) == 0) 
		return; 
	strcpy(speechc, speech.c_str());
	int numberResult = NULL;

	//XPLMDebugString(speechc);
	//return;


	if (CStringUtils::word_exists(speechc, "SORRY?")
		|| CStringUtils::word_exists(speechc, "WHAT?")
		|| CStringUtils::word_exists(speechc, "WUT?")
		|| CStringUtils::word_exists(speechc, "WAT?")
		|| CStringUtils::word_exists(speechc, "HAM")
		|| CStringUtils::word_exists(speechc, "HEM")
		|| strstr(speechc, "REPEAT THAT")
		|| strstr(speechc, "SAY THAT AGAIN")
		|| strstr(speechc, "REPEAT LAST")
		|| strstr(speechc, "HUM?")
		|| strstr(speechc, "SAY LAST")
		|| strstr(speechc, "COULDN'T HEAR YOU")
		|| strstr(speechc, "SAY AGAIN"))
	
	{
		playAllSounds();
		return;
	}

	soundsToPlay.clear(); // clear if the user *did* get what we said

	if (inChecklistMode) {
		if (!strcmp(speechc, fixed_string<512>(currentItem.itemResponse))) {
			nextChecklistItem();
		}
	}

	if (strstr(speechc, "BEFORE TAKEOFF CHECKLIST")) {
		currentChecklist = beforeTakeoffChecklist;
		createCPWWindow();
	}
	
	else if (strstr(speechc, "BEFORE START CHECKLIST")) {
		currentChecklist = beforeStartChecklist;
		createCPWWindow();
	}

	else if (strstr(speechc, "BEFORE TAXI CHECKLIST")) {
		currentChecklist = beforeTaxiChecklist;
		createCPWWindow();
	}

	else if (strstr(speechc, "APPROACH CHECKLIST")) {
		currentChecklist = approachChecklist;
		createCPWWindow();
	}


	else if (strstr(speechc, "DESCENT CHECKLIST")) {
		currentChecklist = descentChecklist;
		createCPWWindow();
	}


	else if (strstr(speechc, "LANDING CHECKLIST")) {
		currentChecklist = landingChecklist;
		createCPWWindow();
	}

	if (CStringUtils::word_exists(speechc, "PUSHBACK")) {
		if (negative_trigger(speechc) || CStringUtils::word_exists(speechc, "STOP") ||
			CStringUtils::word_exists(speechc, "ABORT")) {
			commandsToExecute.push_back(commands::stop_pushback); // not working w/o better pushback
			soundsToPlay.push_back(wordToFile("pushback_stopped_1"));
			playAllSounds();
		}


		else {
			commandsToExecute.push_back(commands::start_pushback);
			soundsToPlay.push_back(wordToFile("starting_pushback_1")); // not working w/o better pushback
			playAllSounds();
		}
	}


	if (CStringUtils::word_exists(speechc, "GPU") ||
		(CStringUtils::word_exists(speechc, "EXTERNAL") 
		&& CStringUtils::word_exists(speechc, "POWER"))) {
		// very inflexible recognition; see pronounciation of GPU in dictionary file
		if (negative_trigger(speechc)) {
			DatarefActionInt apSetting;
			apSetting.handle = datarefs::gpu;
			apSetting.value = 0;
			intRefsToSet.push(apSetting);
			soundsToPlay.push_back(wordToFile("gpu_disc"));
			playAllSounds();
		}


		else {
			DatarefActionInt apSetting;
			apSetting.handle = datarefs::gpu;
			apSetting.value = 1;
			intRefsToSet.push(apSetting);
			soundsToPlay.push_back(wordToFile("gpu_on"));
			playAllSounds();
		}

	}

	if (strstr(speechc, "REVERSE")) {

		if (CStringUtils::word_exists(speechc, "MAX") ||
			CStringUtils::word_exists(speechc, "MAXIMUM")) {
			commandsToExecute.push_back(commands::thrust_rev_max_hold);
			soundsToPlay.push_back(wordToFile("maxrevthr"));
			playAllSounds();
			return;
		}



		if (negative_trigger(speechc)) {
			DatarefActionInt apSetting;
			apSetting.handle = datarefs::thrust_lever;
			apSetting.value = 0;
			intRefsToSet.push(apSetting);
			soundsToPlay.push_back(wordToFile("rev_thr_on")); // no snd
			playAllSounds();
		}


		else {
			DatarefActionInt apSetting;
			apSetting.handle = datarefs::thrust_lever;
			apSetting.value = 0;
			intRefsToSet.push(apSetting);
			soundsToPlay.push_back(wordToFile("rev_thr_off")); // no snd
			playAllSounds();
		}
	}


	if (CStringUtils::word_exists(speechc, "AUTOPILOT")) {

		if (negative_trigger(speechc)) {
			DatarefActionInt apSetting;
			apSetting.handle = datarefs::autopilot;
			apSetting.value = 0;
			intRefsToSet.push(apSetting);
			soundsToPlay.push_back(wordToFile("ap_disc"));
			playAllSounds();
		}


		else {
			DatarefActionInt apSetting;
			apSetting.handle = datarefs::autopilot;
			apSetting.value = 2; // this is intentional; leave it at 2
			intRefsToSet.push(apSetting);
			soundsToPlay.push_back(wordToFile("ap_eng"));
			playAllSounds();
		}

	}

	if (CStringUtils::word_exists(speechc, "DOOR") ||
		CStringUtils::word_exists(speechc, "DOORS")) {

		if (CStringUtils::word_exists(speechc, "CARGO")) {
			if (negative_trigger(speechc)) {
				DatarefActionInt apSetting;
				apSetting.handle = datarefs::cargo_door;
				apSetting.value = 0;
				intRefsToSet.push(apSetting);
				soundsToPlay.push_back(wordToFile("cargo_door_close")); // no snd
				playAllSounds();
			}


			else {
				DatarefActionInt apSetting;
				apSetting.handle = datarefs::cargo_door;
				apSetting.value = 1;
				intRefsToSet.push(apSetting);
				soundsToPlay.push_back(wordToFile("cargo_door_open"));
				playAllSounds();
			}
		}

		if (CStringUtils::word_exists(speechc, "LEFT")) {

			if (CStringUtils::word_exists(speechc, "FORWARD") ||
				CStringUtils::word_exists(speechc, "FRONT")) {

				if (negative_trigger(speechc)) {
					DatarefActionInt apSetting;
					apSetting.handle = datarefs::fl_door;
					apSetting.value = 0;
					intRefsToSet.push(apSetting);
					soundsToPlay.push_back(wordToFile("fl_door_close")); // no snd
					playAllSounds();
				}


				else {
					DatarefActionInt apSetting;
					apSetting.handle = datarefs::fl_door;
					apSetting.value = 1;
					intRefsToSet.push(apSetting);
					soundsToPlay.push_back(wordToFile("fl_door_open"));
					playAllSounds();
				}
			}

			if (CStringUtils::word_exists(speechc, "REAR") ||
				CStringUtils::word_exists(speechc, "BACK") ||
				CStringUtils::word_exists(speechc, "BACKWARD")) {


				if (negative_trigger(speechc)) {
					DatarefActionInt apSetting;
					apSetting.handle = datarefs::rl_door;
					apSetting.value = 0;
					intRefsToSet.push(apSetting);
					soundsToPlay.push_back(wordToFile("rl_door_close")); // no snd
					playAllSounds();
				}


				else {
					DatarefActionInt apSetting;
					apSetting.handle = datarefs::rl_door;
					apSetting.value = 1;
					intRefsToSet.push(apSetting);
					soundsToPlay.push_back(wordToFile("rl_door_open"));
					playAllSounds();
				}
			}


		}


		if (CStringUtils::word_exists(speechc, "RIGHT")) {

			if (CStringUtils::word_exists(speechc, "FORWARD") 
				|| CStringUtils::word_exists(speechc, "FRONT")) {

				if (negative_trigger(speechc)) {
					DatarefActionInt apSetting;
					apSetting.handle = datarefs::fr_door;
					apSetting.value = 0;
					intRefsToSet.push(apSetting);
					soundsToPlay.push_back(wordToFile("fr_door_close")); // no snd
					playAllSounds();
				}


				else {
					DatarefActionInt apSetting;
					apSetting.handle = datarefs::fr_door;
					apSetting.value = 1;
					intRefsToSet.push(apSetting);
					soundsToPlay.push_back(wordToFile("fr_door_open"));
					playAllSounds();
				}
			}

			if (CStringUtils::word_exists(speechc, "REAR") 
				|| CStringUtils::word_exists(speechc, "BACK") ||
				CStringUtils::word_exists(speechc, "BACKWARD")) {


				if (negative_trigger(speechc)) {
					DatarefActionInt apSetting;
					apSetting.handle = datarefs::rr_door;
					apSetting.value = 0;
					intRefsToSet.push(apSetting);
					soundsToPlay.push_back(wordToFile("rr_door_close")); // no snd
					playAllSounds();
				}


				else {
					DatarefActionInt apSetting;
					apSetting.handle = datarefs::rr_door;
					apSetting.value = 1;
					intRefsToSet.push(apSetting);
					soundsToPlay.push_back(wordToFile("rr_door_open"));
					playAllSounds();
				}
			}


		}


	}
	if (CStringUtils::word_exists(speechc, "GEAR")) {

		if (CStringUtils::word_exists(speechc, "DOWN") || 
			CStringUtils::word_exists(speechc, "EXTEND") ||
			positive_trigger(speechc)) {
			commandsToExecute.push_back(commands::gear_dn);
			throw std::logic_error("gear down.wav not implemented");
			soundsToPlay.push_back(wordToFile("navlights_off"));
			playAllSounds();
		}

		if (CStringUtils::word_exists(speechc, "UP") ||
			CStringUtils::word_exists(speechc, "RETRACT") ||
			negative_trigger(speechc)) {
			commandsToExecute.push_back(commands::gear_up);
			soundsToPlay.push_back(wordToFile("gearup"));
			playAllSounds();
		}

	}

	if (CStringUtils::word_exists(speechc, "COM1")) {

		CStringUtils::reverse_number_search(speechc, numberResult);
		if (numberResult >= 100000) { numberResult /= 10; }
		if (numberResult >= 1000 && numberResult <= 10000) { numberResult *= 10; }
		if (numberResult >= 100 && numberResult <= 1000) { numberResult *= 100; }
		if (std::to_string(numberResult)[0] == '2') {

			numberResult -= 20000;
			//if (numberResult <= 9999) {}
		}
		if (numberResult >= 100000) { numberResult /= 10; }
		if (numberResult >= 1000 && numberResult <= 10000) { numberResult *= 10; }
		if (numberResult >= 100 && numberResult <= 1000) { numberResult *= 100; }

		DatarefActionInt commsRefChange;
		commsRefChange.handle = datarefs::com1;
		commsRefChange.value = numberResult;
		intRefsToSet.push(commsRefChange);
		soundsToPlay.push_back(wordToFile("com1_set_to")); // no snd; corrupted file
		std::vector<std::string> tempVec;
		CStringUtils::intToWords(numberResult, CStringUtils::NumberToWordMode::IndividualDigits, tempVec, true);
		playAllSounds();

	}

	if (CStringUtils::word_exists(speechc, "COM2")) {
		CStringUtils::reverse_number_search(speechc, numberResult);
		if (numberResult >= 100000) { numberResult /= 10; }
		if (numberResult >= 1000 && numberResult <= 10000) { numberResult *= 10; }
		if (numberResult >= 100 && numberResult <= 1000) { numberResult *= 100; }
		if (std::to_string(numberResult)[0] == '2') {

			numberResult -= 20000;
			//if (numberResult <= 9999) {}
		}
		if (numberResult >= 100000) { numberResult /= 10; }
		if (numberResult >= 1000 && numberResult <= 10000) { numberResult *= 10; }
		if (numberResult >= 100 && numberResult <= 1000) { numberResult *= 100; }

		DatarefActionInt commsRefChange;
		commsRefChange.handle = datarefs::com2;
		commsRefChange.value = numberResult;
		intRefsToSet.push(commsRefChange);
		soundsToPlay.push_back(wordToFile("com2_set_to"));
		std::vector<std::string> tempVec;
		CStringUtils::intToWords(numberResult, CStringUtils::NumberToWordMode::IndividualDigits, tempVec, true);
		playAllSounds();

	}


	if (CStringUtils::word_exists(speechc, "FLAPS")) {

		if (CStringUtils::word_exists(speechc, "UP")) {
			CStringUtils::reverse_number_search(speechc, numberResult);
			if (numberResult != CStringUtils::npos) {
				/*todo implement*/
			}

		}


		if (CStringUtils::word_exists(speechc, "DOWN")) {
			CStringUtils::reverse_number_search(speechc, numberResult);
			if (numberResult != CStringUtils::npos) {
				/*todo implement*/
			}
		}


		if (CStringUtils::word_exists(speechc, "DEGREES")) {
			CStringUtils::reverse_number_search(speechc, numberResult);

			float requestedFlapsAmount = ((float)numberResult) / 40.0f;

			DatarefActionFloat flapsSetting;
			flapsSetting.handle = datarefs::flaps;
			flapsSetting.value = requestedFlapsAmount;
			floatRefsToSet.push(flapsSetting);


			soundsToPlay.push_back(wordToFile("flaps"));
			std::vector<std::string> tempVec;
			CStringUtils::intToWords(numberResult, CStringUtils::NumberToWordMode::IndividualDigits, tempVec, true);
			soundsToPlay.push_back(wordToFile("degrees"));
			playAllSounds();
		}


	}

	if (CStringUtils::word_exists(speechc, "APU")) {
		// check pronunciation
		if (CStringUtils::word_exists(speechc, "START")) {
			//			commandsToExecute.push(commands::apu_start);
			soundsToPlay.push_back(wordToFile("starting_apu"));
			playAllSounds();
		}

		if (positive_trigger(speechc)) {
			//	commandsToExecute.push(commands::apu_on);
			soundsToPlay.push_back(wordToFile("apu_on"));
			playAllSounds();
		}

		if (negative_trigger(speechc)) {
			//commandsToExecute.push(commands::apu_off);
			soundsToPlay.push_back(wordToFile("apu_off"));
			playAllSounds();
		}

	}

	if (CStringUtils::word_exists(speechc, "BELT") ||
		CStringUtils::word_exists(speechc, "SEATBELT")) {

		if (positive_trigger(speechc)) {
			DatarefActionInt seatbeltSetter;
			seatbeltSetter.handle = datarefs::seatbelt;
			seatbeltSetter.value = 1;
			intRefsToSet.push(seatbeltSetter);
			(rand() % 5 + 0) == 2 ? soundsToPlay.push_back(wordToFile("belt_on_2")) : soundsToPlay.push_back(wordToFile("belt_on_1"));
			playAllSounds();
		}

		if (negative_trigger(speechc)) {
			DatarefActionInt seatbeltSetter;
			seatbeltSetter.handle = datarefs::seatbelt;
			seatbeltSetter.value = 0;
			intRefsToSet.push(seatbeltSetter);
			soundsToPlay.push_back(wordToFile("belt_off"));
			playAllSounds();
		}

	}

	if (CStringUtils::word_exists(speechc, "LNAV")) {
		if (negative_trigger(speechc)) {
			// inflexible recognition
			DatarefActionInt lnavRefChange;
			lnavRefChange.handle = datarefs::autopilot_lnav;
			lnavRefChange.value = 0;
			intRefsToSet.push(lnavRefChange);
			soundsToPlay.push_back(wordToFile("lnav_disc"));
			playAllSounds();
		}

		else {
			// inflexible recognition
			DatarefActionInt lnavRefChange;
			lnavRefChange.handle = datarefs::autopilot_lnav;
			lnavRefChange.value = 1;
			intRefsToSet.push(lnavRefChange);
			soundsToPlay.push_back(wordToFile("lnav"));
			playAllSounds();
		}
	}

	if (strstr(speechc, "NAV") || 
		CStringUtils::word_exists(speechc, "NAVIGATION")) {

		if (strstr(speechc, "LIGHT") || strstr(speechc, "LIGHTS")) {
			if (negative_trigger(speechc)) {
				DatarefActionInt navLightAction;
				navLightAction.handle = datarefs::nav_lights;
				navLightAction.value = 0;
				intRefsToSet.push(navLightAction);
				soundsToPlay.push_back(wordToFile("navlights_off"));
				playAllSounds();
			}


			else {
				DatarefActionInt navLightAction;
				navLightAction.handle = datarefs::nav_lights;
				navLightAction.value = 1;
				intRefsToSet.push(navLightAction);
				soundsToPlay.push_back(wordToFile("navlights_on"));
				playAllSounds();
			}
		}
	}

	if (CStringUtils::word_exists(speechc, "STROBE")) {
		// not working - command
		// inflexible recognition
		if (strstr(speechc, "LIGHT") || strstr(speechc, "LIGHTS")) {
			if (negative_trigger(speechc)) {
				DatarefActionInt lightAction;
				lightAction.handle = datarefs::strobe_lights;
				lightAction.value = 0;
				intRefsToSet.push(lightAction);
				soundsToPlay.push_back(wordToFile("strobelights_off"));
				playAllSounds();
			}


			else {
				DatarefActionInt lightAction;
				lightAction.handle = datarefs::strobe_lights;
				lightAction.value = 1;
				intRefsToSet.push(lightAction);
				soundsToPlay.push_back(wordToFile("strobelights_on"));
				playAllSounds();
			}
		}
	}

	if (CStringUtils::word_exists(speechc, "BEACON")) {
		// inflexible recognition
		if (strstr(speechc, "LIGHT") || strstr(speechc, "LIGHTS")) {
			if (negative_trigger(speechc)) {
				DatarefActionInt lightAction;
				lightAction.handle = datarefs::beacon_lights;
				lightAction.value = 0;
				intRefsToSet.push(lightAction);
				soundsToPlay.push_back(wordToFile("beaconlights_off"));
				playAllSounds();
			}



			else {
			
				DatarefActionInt lightAction;
				lightAction.handle = datarefs::strobe_lights;
				lightAction.value = 1;
				intRefsToSet.push(lightAction);
				soundsToPlay.push_back(wordToFile("beaconlights_on"));
				playAllSounds();
			}
		}
	}


	if (CStringUtils::word_exists(speechc, "TAXI")) {
		if (strstr(speechc, "LIGHT") || strstr(speechc, "LIGHTS")) {
			if (negative_trigger(speechc)) {
				DatarefActionInt lightAction;
				lightAction.handle = datarefs::taxi_lights;
				lightAction.value = 0;
				intRefsToSet.push(lightAction);
				soundsToPlay.push_back(wordToFile("taxilights_off")); // no snd
				playAllSounds();
			}


			else {
				DatarefActionInt lightAction;
				lightAction.handle = datarefs::taxi_lights;
				lightAction.value = 1;
				intRefsToSet.push(lightAction);
				soundsToPlay.push_back(wordToFile("taxilights_on")); // no snd
				playAllSounds();
			}
		}
	}

	if (strstr(speechc, "TRANSPONDER") ||
		strstr(speechc, "SQUAWK")) {


		CStringUtils::reverse_number_search(speechc, numberResult);
		if (numberResult != CStringUtils::npos) {
			if (std::to_string(numberResult)[0] == '2' && numberResult >= 10000) { numberResult -= 2000; }

			DatarefActionInt transponderSetting;
			transponderSetting.handle = datarefs::transponder_code;
			transponderSetting.value = numberResult;
			intRefsToSet.push(transponderSetting);
			soundsToPlay.push_back(wordToFile("squawk_set_to"));
			std::vector<std::string> tempVec;
			CStringUtils::intToWords(numberResult, CStringUtils::NumberToWordMode::IndividualDigits, tempVec, true);
			playAllSounds();
			return;
		}

		if (CStringUtils::word_exists(speechc, "MODE")) {

			if (CStringUtils::word_exists(speechc, "C") || CStringUtils::word_exists(speechc, "CHARLIE")) {
				DatarefActionInt transponderSetting;
				transponderSetting.handle = datarefs::transponder_mode;
				transponderSetting.value = 2;
				intRefsToSet.push(transponderSetting);
				soundsToPlay.push_back(wordToFile("transponder_c_1"));
				playAllSounds();
				return;
			}


			if (CStringUtils::word_exists(speechc, "STANDBY")) {
				// no sentence for this in dictionary file...?
				DatarefActionInt transponderSetting;
				transponderSetting.handle = datarefs::transponder_mode;
				transponderSetting.value = 1;
				intRefsToSet.push(transponderSetting);
				soundsToPlay.push_back(wordToFile("transponder_standby"));
				playAllSounds();
			}


			if (CStringUtils::word_exists(speechc, "TEST")) {
				DatarefActionInt transponderSetting;
				transponderSetting.handle = datarefs::transponder_mode;
				transponderSetting.value = 4;
				intRefsToSet.push(transponderSetting);
				soundsToPlay.push_back(wordToFile("transponder_test"));
				playAllSounds();
			}




		}


		if (negative_trigger(speechc)) {
			DatarefActionInt transponderSetting;
			transponderSetting.handle = datarefs::transponder_mode;
			transponderSetting.value = 0;
			intRefsToSet.push(transponderSetting);
			soundsToPlay.push_back(wordToFile("transponder_off"));
			playAllSounds();
		}


		if (positive_trigger(speechc)) {
			DatarefActionInt transponderSetting;
			transponderSetting.handle = datarefs::transponder_mode;
			transponderSetting.value = 2;
			intRefsToSet.push(transponderSetting);
			soundsToPlay.push_back(wordToFile("transponder_on"));
			playAllSounds();
		}

		


	}
	

	if (CStringUtils::word_exists(speechc, "AUTOBRAKE")) { // 0 = rejected take-off, 1 = off, 2>5 = levels

		CStringUtils::reverse_number_search(speechc, numberResult);

		if (numberResult != CStringUtils::npos) {
			DatarefActionInt transponderSetting;
			transponderSetting.handle = datarefs::autobrake_level;
			transponderSetting.value = numberResult;
			intRefsToSet.push(transponderSetting);
			soundsToPlay.push_back(wordToFile("autobrake_set_to"));
			numberResult -= 20;
			std::vector<std::string> tempVec;
			pluginLogger << std::to_string(numberResult);
			CStringUtils::intToWords(numberResult, CStringUtils::NumberToWordMode::IndividualDigits, tempVec, true);
			playAllSounds();
			return;
		}


		if (negative_trigger(speechc)) {
			DatarefActionInt athrAction;
			athrAction.handle = datarefs::autobrake_level;
			athrAction.value = 1;
			intRefsToSet.push(athrAction);
			soundsToPlay.push_back(wordToFile("autobrakes_off"));
			playAllSounds();
		}

		if (positive_trigger(speechc)) {
			DatarefActionInt athrAction;
			athrAction.handle = datarefs::autobrake_level;
			athrAction.value = 2;
			intRefsToSet.push(athrAction);
			soundsToPlay.push_back(wordToFile("autobrakes_on"));
			playAllSounds();
		}


		

	}

	if (CStringUtils::word_exists(speechc, "AVIONICS")) {

		if (negative_trigger(speechc)) {
//			commandsToExecute.push(commands::avionics_off);
			soundsToPlay.push_back(wordToFile("avionics_off"));
			playAllSounds();
		}

		else {
			//commandsToExecute.push(commands::avionics_on);
			soundsToPlay.push_back(wordToFile("avionics_on"));
			playAllSounds();
		}
	}


	if (CStringUtils::word_exists(speechc, "AUTOTHROTTLE")) {

		if (negative_trigger(speechc)) {
			DatarefActionInt athrAction;
			athrAction.handle = datarefs::autopilot_autothrottle;
			athrAction.value = 0;
			intRefsToSet.push(athrAction);
			soundsToPlay.push_back(wordToFile("autothrottle_disarmed"));
			playAllSounds();
		}

		else {
			DatarefActionInt athrAction;
			athrAction.handle = datarefs::autopilot_autothrottle;
			athrAction.value = 1;
			intRefsToSet.push(athrAction);
			soundsToPlay.push_back(wordToFile("autothrottle"));
			playAllSounds();
		}
	}


	if (CStringUtils::word_exists(speechc, "ALTIMETER")) {

		if (CStringUtils::word_exists(speechc, "STANDARD"))
		{
			CStringUtils::reverse_number_search(speechc, numberResult);
			DatarefActionInt altimeterChange;
			altimeterChange.handle = datarefs::altimeter;
			altimeterChange.value = numberResult;
			intRefsToSet.push(altimeterChange);
			soundsToPlay.push_back(wordToFile("altimeter_standard"));
			playAllSounds();
		}

		else {
			CStringUtils::reverse_number_search(speechc, numberResult);
			DatarefActionInt altimeterChange;
			altimeterChange.handle = datarefs::altimeter;
			altimeterChange.value = numberResult;
			intRefsToSet.push(altimeterChange);
			soundsToPlay.push_back(wordToFile("altimeter_set_to")); // no snd
			std::vector<std::string> tempVec;
			CStringUtils::intToWords(numberResult, CStringUtils::NumberToWordMode::IndividualDigits, tempVec, true);
			playAllSounds();
		}

	}

	if (CStringUtils::word_exists(speechc, "DIRECTOR") || 
		CStringUtils::word_exists(speechc, "DIRECTORS")) {

		if (negative_trigger(speechc)) {
			DatarefActionInt fdAction;
			fdAction.handle = datarefs::autopilot_fd;
			fdAction.value = 0;
			intRefsToSet.push(fdAction);
			soundsToPlay.push_back(wordToFile("fd_disc"));
			playAllSounds();
		}

		else {
			DatarefActionInt athrAction;
			athrAction.handle = datarefs::autopilot_autothrottle;
			athrAction.value = 1;
			intRefsToSet.push(athrAction);
			soundsToPlay.push_back(wordToFile("fd_on_2"));
			playAllSounds();
		}


	}


	if (CStringUtils::word_exists(speechc, "SPEED")){

		if (CStringUtils::word_exists(speechc, "BUG")) {
			// not working; vector iterator incompatible exception
			if (negative_trigger(speechc)) {
				DatarefActionInt apAction;
				apAction.handle = datarefs::autopilot_spd;
				apAction.value = 0;
				intRefsToSet.push(apAction);
				soundsToPlay.push_back(wordToFile("autopilot_spd_disc"));
				playAllSounds();
			}

			else {
				CStringUtils::reverse_number_search(speechc, numberResult);
				DatarefActionInt apAction;
				apAction.handle = datarefs::autopilot_spd;
				apAction.value = numberResult;
				intRefsToSet.push(apAction);
				soundsToPlay.push_back(wordToFile("autopilot_spd"));
				playAllSounds();
			}


		}


		if (CStringUtils::word_exists(speechc, "INTERVENTION")) {
			if (negative_trigger(speechc)) {
				DatarefActionInt apAction;
				apAction.handle = datarefs::spd_intv;
				apAction.value = 0;
				intRefsToSet.push(apAction);
				soundsToPlay.push_back(wordToFile("spd_intv_desel"));
				playAllSounds();
			}

			else {
				DatarefActionInt apAction;
				apAction.handle = datarefs::spd_intv;
				apAction.value = 1;
				intRefsToSet.push(apAction);
				soundsToPlay.push_back(wordToFile("spd_intv"));
				playAllSounds();
			}

		}


		else {
			// no sequences for this in dictionary file...? IN PROGRESS
			CStringUtils::reverse_number_search(speechc, numberResult);
			DatarefActionInt apAction;
			apAction.handle = datarefs::ap_speed;
			apAction.value = numberResult;
			if (std::to_string(numberResult)[0] == '2' && numberResult >= 1000) { numberResult -= 2000; }
			intRefsToSet.push(apAction);
			soundsToPlay.push_back(wordToFile("speed_set_to")); // no snd
			std::vector<std::string> tempVec;
			CStringUtils::intToWords(numberResult, CStringUtils::NumberToWordMode::IndividualDigits, tempVec, true);
			playAllSounds();
		}
	}

	/*"Heading Hold" is installed mostly for ATC purposes. Generally to insure horizontal separation between two aircraft. The pilot will receive that kind of message : "XXXXX...maintain present heading". It has not been designed to intercept the displayed heading.*/

	if (CStringUtils::word_exists(speechc, "HEADING")) {

		if (CStringUtils::word_exists(speechc, "HOLD")) {
			if (negative_trigger(speechc)) {
				DatarefActionInt apAction;
				apAction.handle = datarefs::autopilot_hdg_hold;
				apAction.value = 0;
				intRefsToSet.push(apAction);
				soundsToPlay.push_back(wordToFile("hdg_hold_disc")); // no impl
				playAllSounds();
			}

			else {
				DatarefActionInt apAction;
				apAction.handle = datarefs::autopilot_hdg_hold;
				apAction.value = 1;
				intRefsToSet.push(apAction);
				soundsToPlay.push_back(wordToFile("hdg_hold")); 
				playAllSounds();
			}
			
	


		}

		if (CStringUtils::word_exists(speechc, "SELECT")) {
			if (negative_trigger(speechc)) {
				DatarefActionInt apAction;
				apAction.handle = datarefs::autopilot_hdg_sel;
				apAction.value = 0;
				intRefsToSet.push(apAction);
				soundsToPlay.push_back(wordToFile("hdg_sel_disc")); // no impl
				playAllSounds();
			}

			else {
				DatarefActionInt apAction;
				apAction.handle = datarefs::autopilot_hdg_sel;
				apAction.value = 1;
				intRefsToSet.push(apAction);
				soundsToPlay.push_back(wordToFile("hdg_sel")); // no snd??
				playAllSounds();
			}
		}

		if (CStringUtils::word_exists(speechc, "DUE")
			&& CStringUtils::word_exists(speechc, "SOUTH")) {
			DatarefActionInt hdgAction;
			hdgAction.handle = datarefs::autopilot_hdg;
			hdgAction.value = 180;
			intRefsToSet.push(hdgAction);
			soundsToPlay.push_back(wordToFile("hdg_set_to"));
			std::vector<std::string> tempVec;
			CStringUtils::intToWords(180, CStringUtils::NumberToWordMode::IndividualDigits, tempVec, true);
			playAllSounds();
		}

		if (CStringUtils::word_exists(speechc, "DUE")
			&& CStringUtils::word_exists(speechc, "WEST")) {
			DatarefActionInt hdgAction;
			hdgAction.handle = datarefs::autopilot_hdg;
			hdgAction.value = 270;
			intRefsToSet.push(hdgAction);
			soundsToPlay.push_back(wordToFile("hdg_set_to"));
			std::vector<std::string> tempVec;
			CStringUtils::intToWords(270, CStringUtils::NumberToWordMode::IndividualDigits, tempVec, true);
			playAllSounds();
		}

		if (CStringUtils::word_exists(speechc, "DUE")
			&& CStringUtils::word_exists(speechc, "EAST")) {
			DatarefActionInt hdgAction;
			hdgAction.handle = datarefs::autopilot_hdg;
			hdgAction.value = 90;
			intRefsToSet.push(hdgAction);
			soundsToPlay.push_back(wordToFile("hdg_set_to"));
			std::vector<std::string> tempVec;
			CStringUtils::intToWords(90, CStringUtils::NumberToWordMode::IndividualDigits, tempVec, true);
			playAllSounds();
		}

		if (CStringUtils::word_exists(speechc, "DUE")
			&& CStringUtils::word_exists(speechc, "NORTH")) {
			DatarefActionInt hdgAction;
			hdgAction.handle = datarefs::autopilot_hdg;
			hdgAction.value = 0;
			intRefsToSet.push(hdgAction);
			soundsToPlay.push_back(wordToFile("hdg_set_to"));
			std::vector<std::string> tempVec;
			CStringUtils::intToWords(0, CStringUtils::NumberToWordMode::IndividualDigits, tempVec, true);
			playAllSounds();
		}

		else {
		
			CStringUtils::reverse_number_search(speechc, numberResult);
			DatarefActionInt hdgAction;
			hdgAction.handle = datarefs::autopilot_hdg;
			hdgAction.value = numberResult;
			intRefsToSet.push(hdgAction);
			soundsToPlay.push_back(wordToFile("hdg_set_to"));
			std::vector<std::string> tempVec;
			CStringUtils::intToWords(numberResult, CStringUtils::NumberToWordMode::IndividualDigits, tempVec, true);
			playAllSounds();
		}

	}


	if (CStringUtils::word_exists(speechc, "PITOT")) {
				if (negative_trigger(speechc)) {
					DatarefActionInt fdAction;
					fdAction.handle = datarefs::pitot_heat;
					fdAction.value = 0;
					intRefsToSet.push(fdAction);
					soundsToPlay.push_back(wordToFile("pitot_heat_off"));
					playAllSounds();
				}

				else {
					DatarefActionInt athrAction;
					athrAction.handle = datarefs::pitot_heat;
					athrAction.value = 1;
					intRefsToSet.push(athrAction);
					soundsToPlay.push_back(wordToFile("pitot_heat_on"));
					playAllSounds();
				}


			}

		}

	