#ifndef _STATE_
#define _STATE_

#pragma once
#include <string>
#include <SFML/Graphics.hpp>

class State {
	private:
	 
	public:

		#define DEV_MODE //GAME_MODE, DEV_MODE, EDITOR_MODE

		static std::string current_state; //states of game like lvls/menus/ect

		static sf::Color mainColorMajor;

		static sf::Color mainColorMinor;
};
#endif