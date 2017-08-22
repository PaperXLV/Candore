#include "audio.h"

Audio::Audio() {
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
	this->loadSounds();
}

Audio::~Audio() {
	//Mix_FreeChunk(this->_backgroundSound);
	Mix_CloseAudio();
}

void Audio::loadSounds() {
	//this->_backgroundSound = Mix_LoadWAV("content/audio/background.wav");
}

void Audio::playSoundEffect(std::string effect) {
	//effect is passed in like a _hitEffect1
}

void Audio::playBackgroundSound() {
	//Mix_PlayChannel(-1, this->_backgroundSound, -1);
}

