#include "settings.h"
#include <sstream>
#include <string.h>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <SDL.h>


Settings::Settings() {
	this->Load();
}

Settings::~Settings() {
	this->Save();
}

void Settings::Load() {
	std::ifstream ifs("Assets/data/settings.txt", std::ofstream::binary);
	ifs.seekg(0, ifs.end);
	long size = ifs.tellg();
	ifs.seekg(0);

	char* screen_height = new char[256];
	char* screen_width = new char[256];
	char* left_button = new char[256];
	char* right_button = new char[256];
	char* jump_button = new char[256];
	char* attack_button = new char[256];

	try {
		//ORDER
		//HEIGHT -> WIDTH
		ifs.getline(screen_width, 256);
		ifs.getline(screen_height, 256);
		//get data saved

		std::string screen_height_str = screen_height;
		std::string screen_width_str = screen_width;

		std::string screen_height_num = screen_height_str.substr(screen_height_str.find("=") + 1);
		std::string screen_width_num = screen_width_str.substr(screen_width_str.find("=") + 1);

		int newHeight = atoi(screen_height_num.c_str());
		int newWidth = atoi(screen_width_num.c_str());

		this->SCREEN_HEIGHT = atoi(screen_height_num.c_str());
		this->SCREEN_WIDTH = atoi(screen_width_num.c_str());

		if (this->SCREEN_HEIGHT == 0) {
			this->SCREEN_HEIGHT = 600;
		}
		if (this->SCREEN_WIDTH == 0) {
			this->SCREEN_WIDTH = 800;
		}

		//Keys
		ifs.getline(left_button, 256);
		ifs.getline(right_button, 256);
		ifs.getline(jump_button, 256);
		ifs.getline(attack_button, 256);

		std::string left_button_str = left_button;
		std::string right_button_str = right_button;
		std::string jump_button_str = jump_button;
		std::string attack_button_str = attack_button;

		std::string left_buttons = left_button_str.substr(left_button_str.find("=") + 1);
		std::string right_buttons = right_button_str.substr(right_button_str.find("=") + 1);
		std::string jump_buttons = jump_button_str.substr(jump_button_str.find("=") + 1);
		std::string attack_buttons = attack_button_str.substr(attack_button_str.find("=") + 1);

		std::string nextbutton = left_buttons;
		for (int i = 0; i < std::count(left_buttons.begin(), left_buttons.end(), ','); i++) {
			std::string currentbutton = nextbutton.substr(0, nextbutton.find(","));
			left.push_back(SDL_GetScancodeFromName(currentbutton.c_str()));
			nextbutton = nextbutton.substr(nextbutton.find(",") + 1);
		}
		nextbutton = right_buttons;
		for (int i = 0; i < std::count(right_buttons.begin(), right_buttons.end(), ','); i++) {
			std::string currentbutton = nextbutton.substr(0, nextbutton.find(","));
			right.push_back(SDL_GetScancodeFromName(currentbutton.c_str()));
			nextbutton = nextbutton.substr(nextbutton.find(",") + 1);
		}
		nextbutton = jump_buttons;
		for (int i = 0; i < std::count(jump_buttons.begin(), jump_buttons.end(), ','); i++) {
			std::string currentbutton = nextbutton.substr(0, nextbutton.find(","));
			jump.push_back(SDL_GetScancodeFromName(currentbutton.c_str()));
			nextbutton = nextbutton.substr(nextbutton.find(",") + 1);
		}
		nextbutton = attack_buttons;
		for (int i = 0; i < std::count(jump_buttons.begin(), jump_buttons.end(), ','); i++) {
			std::string currentbutton = nextbutton.substr(0, nextbutton.find(","));
			attack.push_back(SDL_GetScancodeFromName(currentbutton.c_str()));
			nextbutton = nextbutton.substr(nextbutton.find(",") + 1);
		}
	}
	catch (...) {
		printf("fix");
		this->SCREEN_HEIGHT = 600;
		this->SCREEN_WIDTH = 800;
		this->left.push_back(SDL_SCANCODE_A);
		this->right.push_back(SDL_SCANCODE_D);
		this->jump.push_back(SDL_SCANCODE_Z);
		this->attack.push_back(SDL_SCANCODE_X);
	}

	delete[] screen_height;
	delete[] screen_width;
	delete[] left_button;
	delete[] right_button;
	delete[] jump_button;
	ifs.close();
}

void Settings::Save() {
	std::ofstream ofs("Assets/data/settings.txt", std::ofstream::binary);

	std::stringstream ss;
	//add more to ss for more stuff to save
	ss << "SCREEN_WIDTH =" << this->SCREEN_WIDTH << "\r\n" << "SCREEN_HEIGHT =" << this->SCREEN_HEIGHT << "\r\n";
	ss << "LEFT =";
	for (int i = 0; i < this->left.size(); i++) {
		ss << SDL_GetScancodeName(this->left[i]) << ",";
	}
	ss << "\r\n" << "RIGHT =";
	for (int i = 0; i < this->right.size(); i++) {
		ss << SDL_GetScancodeName(this->right[i]) << ",";
	}
	ss << "\r\n" << "JUMP =";
	for (int i = 0; i < this->jump.size(); i++) {
		ss << SDL_GetScancodeName(this->jump[i]) << ",";
	}
	ss << "\r\n" << "ATTACK =";
	for (int i = 0; i < this->attack.size(); i++) {
		ss << SDL_GetScancodeName(this->attack[i]) << ",";
	}

	std::string temp = ss.str();
	const char* buffer = temp.c_str();

	ofs.write(buffer, strlen(buffer));


	ofs.close();
}


bool Settings::is_left(SDL_Scancode key) {
	for (int i = 0; i < this->left.size(); i++) {
		if (key == this->left[i]) {
			return true;
		}
	}
	return false;
}

bool Settings::is_right(SDL_Scancode key) {
	for (int i = 0; i < this->right.size(); i++) {
		if (key == this->right[i]) {
			return true;
		}
	}
	return false;
}

bool Settings::is_jump(SDL_Scancode key) {
	for (int i = 0; i < this->jump.size(); i++) {
		if (key == this->jump[i]) {
			return true;
		}
	}
	return false;
}
