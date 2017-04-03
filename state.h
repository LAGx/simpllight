#ifndef _STATE_
#define _STATE_

#pragma once
#include <string>
#include <SFML/Graphics.hpp>

#define GAME_MODE //GAME_MODE, DEV_MODE, EDITOR_MODE

class State {
	private:
	 
	public:

		static std::string current_state; //states of game like lvls/menus/ect

		static sf::Color mainColorMajor;

		static sf::Color mainColorMinor;
};
#endif