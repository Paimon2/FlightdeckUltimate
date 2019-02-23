#include <queue>
#include <thread>
#include <algorithm>
#include <string>
#include "GlobalVars.h"
#include "Settings.h"
#include <SFML/Audio.hpp>



int currentSoundIndex;
sf::Music *currentSound;

void playQueuedSoundsInternal() {
	
	for (auto &v : soundsToPlay) {
		sf::Music thisSound;
		thisSound.openFromFile(v);
		sf::Int32 duration = 0;

		try {
			
			duration = thisSound.getDuration().asMilliseconds();
		}
		catch (...) {
			duration = 1500;
		}

		thisSound.setVolume(100); // todo custom volume with json settings 
		thisSound.setPosition(sf::Vector3f(0.66f, 1.59f, -19.64f));
		thisSound.play();
		std::this_thread::sleep_for(std::chrono::milliseconds(duration + rand() % 100 + 70));
	}
	
		
	//soundsToPlay.clear();
}




void playAllSounds() {
	std::thread soundProcessorThread(playQueuedSoundsInternal);
	soundProcessorThread.detach();
	currentSoundIndex = 0;
}

std::string wordToFile(std::string word) {
	std::string voicePackPathCopy = voicePackPath;
	std::transform(word.begin(), word.end(), word.begin(), ::tolower);
	voicePackPathCopy.append(word);
	voicePackPathCopy.append(".wav");

	return voicePackPathCopy;
}