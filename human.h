#pragma once
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>

class Human {//to do trigger zone//test phisic
protected:
	sf::Sprite body;
	sf::CircleShape zone;

	b2Fixture *zoneFixt;

	sf::Texture texture_body;
	sf::Texture texture_zone;

	float radiusZone = 30.f;
	bool isVisibleZone = true;

	float drug_air = 1;
	float drug_angle = 1;


	void update();
	void setTexturePosition(sf::Vector2f cord, float angle);
	void updateRadiusZone();

public:
	int depthRender = 0;

	std::string name = "None";

	void setRadius(float radius = 0);
	void moveRadius(float radius_delta = 0);
	void setZoneVisible(bool isVisible = true);

	b2Body *body_ph;
    Human(sf::Vector2f initCord, std::string texture,std::string name, b2World* World); //Human texture have to be a single color with shadow 32x32. 


	void blit();
	
	~Human();
};