#ifndef _STATE_
#define _STATE_

#pragma once
#include <string>
#include <SFML/Graphics.hpp>


#define DEV_MODE //GAME_MODE, DEV_MODE
#define OS_WIN //OS_WIN, OS_LINUX //DON`T WORK


class State {
	private:
	 
	public:

		static std::string current_state; //states of game like lvls/menus/ect

		static sf::Color mainColorMajor;

		static sf::Color mainColorMinor;
};
#endif