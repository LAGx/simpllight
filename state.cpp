#include "state.h"
#include <iostream>
#include <SFML/Graphics.hpp>

std::string State::current_state = "None";

std::string State::mode = "dev";

sf::Color State::mainColorMajor(5,20,20);
