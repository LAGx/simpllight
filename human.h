#pragma once
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>

class Human {//to do trigger zone//test phisic
protected:
	sf::Sprite body;
	sf::CircleShape zone;

	sf::Texture texture_body;
	sf::Texture texture_zone;

	float radiusZone = 30.f;
	float drug_air = 0.7;
	float drug_angle = 1;

	void update();
	void setTexturePosition(sf::Vector2f cord, float angle);

public:
	signed int depthRender = 0;
	b2Body *body_ph;

	Human(sf::Vector2f initCord, std::string texture); //Human texture have to be a single color with shadow 32x32. 
	
	void blit();
	
	~Human();
};