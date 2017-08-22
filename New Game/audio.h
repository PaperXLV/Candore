#pragma once
#ifndef AUDIO_H
#define AUDIO_H

#include <string>
#include <SDL_audio.h>
#include <SDL.h>
#include <SDL_mixer.h>

class Audio {
public:
	Audio();
	~Audio();

	void loadSounds();
	void playSoundEffect(std::string sound);
	void playBackgroundSound();

private:
	//Mix_Chunk *(soundeffects)
};





#endif AUDIO_H