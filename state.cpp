#include "state.h"
#include <iostream>
#include <SFML/Graphics.hpp>

std::string State::current_state = "None";

std::string State::mode = "game";

sf::Color State::mainColorMajor(5,20,20);

sf::Color State::mainColorMinor(255, 0, 153);
