#ifndef _SERVICE_
#define _SERVICE_

#include <SFML/Graphics.hpp>
#pragma once

class Rand {
public:
	static unsigned int intRand(int by, int to); // [by, to]
};


class TextureGenerator {

public:
	static void fillShadowRect(int shadowSize, sf::Vector2i size, sf::Color color, std::string name);

	static void fillShadowCircle(int shadowSize, int radiusSize, sf::Color color, std::string name);

};

#endif

