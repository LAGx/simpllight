#include "service.h"
#include <iostream>
#include <random>
#include "log.h"
#include <SFML/Graphics.hpp>

unsigned int Rand::intRand(int min, int max) {
	std::mt19937 rng;
	rng.seed(std::random_device()());
	std::uniform_int_distribution<std::mt19937::result_type> dist6(min, max);
	return (int)dist6(rng);
}


float getLength(sf::Vector2f vec1, sf::Vector2f vec2) {
	return sqrt((vec1.x - vec2.x)*(vec1.x - vec2.x) + (vec1.y - vec2.y)*(vec1.y - vec2.y));
}


void TextureGenerator::fillShadowRect(int shadowSize, sf::Vector2i size, sf::Color color, std::string name) {
	sf::Image image;
	image.create(shadowSize * 2 + size.x, shadowSize * 2 + size.y, sf::Color(255,0,0,0)); //DEBAG change 155 to 0

	//fill base rect
	for (int i = shadowSize; i < size.x + shadowSize; i++) {//by x move
		for (int j = shadowSize; j < size.y + shadowSize; j++) {//by y move
			image.setPixel(i, j, color);
		}
	}

	int level = 0;int x = 0, y = 0;
	for (; level < shadowSize; level++) {
		sf::Color color(0, 0, 0, 200 * ((float(level - shadowSize) / shadowSize)));

		y = level;
		for (x = level; x < image.getSize().x - level; x++) {
			image.setPixel(x, y, color);
		}
		for (y = level; y < image.getSize().y - level; y++) {
			image.setPixel(x - 1, y, color);
		}

		for (x = image.getSize().x - level; x > level; x--) {
			image.setPixel(x - 1, y - 1, color);
		}
		for (y = image.getSize().y - level; y > level; y--) {
			image.setPixel(x , y-1, color);
		}
	}

	if (!image.saveToFile(name))
		throw Log::Exception("invalid way to save image rect " + name);
}


void TextureGenerator::fillShadowCircle(int shadowSize, int radiusSize, sf::Color color, std::string name) {
	sf::Image image;
	image.create((shadowSize+ radiusSize)*2, (shadowSize + radiusSize) * 2, sf::Color(255, 0, 0, 0)); //DEBAG change 155 to 0

	int x = 0, y = 0; 
	float length= 0;

	for (x = 0; x < image.getSize().x; x++) {//by y move
		for (y = 0; y < image.getSize().y; y++) {//by x move
			length = getLength(sf::Vector2f(image.getSize().x/2, image.getSize().y / 2), sf::Vector2f(x,y));

			//draw
			if (length < radiusSize) {
				image.setPixel(x, y, color);
			}
			//shadow
			if ((length >= radiusSize) && (length < (image.getSize().y/2))) {
				image.setPixel(x, y, sf::Color(0, 0, 0, 220 * (1-(length-radiusSize)/(shadowSize))));

			}
			//blur
			if (length < radiusSize && length >= radiusSize -1) {
				image.setPixel(x, y, color - sf::Color(200, 200,200, 30));
			}
		}
	}


	if (!image.saveToFile(name))
		throw Log::Exception("invalid way to save image circle" + name);
}


void TextureGenerator::conturRect(sf::Vector2i size, int lineWidth, sf::Color color, std::string name) {
	sf::Image image;
	image.create(size.x, size.y, sf::Color(255, 0, 0, 0)); //DEBAG change 155 to 0

	int level = 0; int x = 0, y = 0;
	for (; level < lineWidth; level++) {

		y = level;
		for (x = level; x < image.getSize().x - level; x++) {
			image.setPixel(x, y, color);
		}
		for (y = level; y < image.getSize().y - level; y++) {
			image.setPixel(x - 1, y, color);
		}

		for (x = image.getSize().x - level; x > level; x--) {
			image.setPixel(x - 1, y - 1, color);
		}
		for (y = image.getSize().y- level; y > level; y--) {
			image.setPixel(x, y-1 , color);
		}
	}

	if (!image.saveToFile(name))
		throw Log::Exception("invalid way to save image contur rect " + name);
}

void TextureGenerator::conturCircle(int radius, int lineWidth, sf::Color color, std::string name) {
	sf::Image image;

	image.create(radius *2, radius *2, sf::Color(255, 0, 0, 0)); //DEBAG change 155 to 0

	int x = 0, y = 0;
	float length = 0;

	for (x = 0; x < image.getSize().x; x++) {//by y move
		for (y = 0; y < image.getSize().y; y++) {//by x move
			length = getLength(sf::Vector2f(image.getSize().x / 2, image.getSize().y / 2), sf::Vector2f(x, y));

			//blur
			if (length-1 < radius && length+1 >= radius  - lineWidth) {
				image.setPixel(x, y,color- sf::Color(0,0,0, 155));
			}
			//draw
			if (length < radius  && length >= radius -lineWidth) {
				image.setPixel(x, y, color);
			}
		}
	}


	if (!image.saveToFile(name))
		throw Log::Exception("invalid way to save image contur circle" + name);
}
