#ifndef _FDU_RECOGNIZER_H_
#define _FDU_RECOGNIZER_H_

#include <pocketsphinx.h>
#include <sphinxbase/ad.h>
#include <sphinxbase/err.h>
#include <mutex>
#include <string>

class Recognizer {

public:
	std::mutex mu;
	ps_decoder_t *ps;                  // create pocketsphinx decoder structure
	cmd_ln_t *config;                  // create configuration structure
	ad_rec_t *ad;                      // create audio recording structure - for use with ALSA functions

	int16 adbuf[8192];                 // buffer array to hold audio data
	uint8 utt_started, in_speech;      // flags for tracking active speech - has speech started? - is speech currently happening?
	int32 k;                           // holds the number of frames in the audio buffer
	char const *hyp;                   // pointer to "hypothesis" (best guess at the decoded result)

	bool isRecognizerActive;
	int shouldStopRecording;
	int listenerCount;

	// All functions below are run in a separate thread and are non-blocking **except** closeRecognizer()
	// Why? If we're shutting down things async, X-Plane will quit before we can clean up.


	void initRecognizer(const std::string &device = "sysdefault");
	void closeRecognizer();


	void recognizeAsync();

	void startListening(bool processCommands = true);
	void stopListening();
};

extern char dirSep[2];
extern Recognizer defaultRecognizer;

#endif