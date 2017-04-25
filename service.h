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
	void fillShadowRect(int shadowSize, sf::Vector2i size, sf::Color color, std::string name);

	void fillShadowCircle(int shadowSize, int radiusSize, sf::Color color, std::string name);

	void conturRect(sf::Vector2i size,int lineWidth, sf::Color color, std::string name);

	void conturCircle(int radius, int lineWidth, sf::Color color, std::string name);

};

#endif

