#pragma once
#include <string>
#include <SFML/Graphics.hpp>

class State {
	private:
	 
	public:

		static std::string current_state; //states of game like lvls/menus/ect

		static std::string mode; //dev, game

		static sf::Color mainColorMajor;

		State();

		~State();
	};
