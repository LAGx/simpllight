#pragma once
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <string>

class BaseObject {
protected:
	sf::Texture texture;
	sf::Sprite g_body;

	void updatePosition(sf::Vector2f newCord, float newAngle);
	bool isVisible = true;

public:
	int depthRender = 0;

	BaseObject(sf::Vector2f initCord, std::string texture);

	void blit();

	~BaseObject();
};


class DynamicObject :public BaseObject {
private:

protected:
	float drug_air = 1;
	float drug_angle = 1;
	float mass = 1;

public:

	b2Body *body_ph;

	DynamicObject(sf::Vector2f initCord, std::string texture, std::string name, std::string type,sf::Vector2f size, bool isSensor = false); //type circle, tringle(3), rect(4), pentagon(5), intnugon(6)//size 1 - lenth of side(pentagon, intnugon, circle)

	void update();

	~DynamicObject();
};


class StaticObject :public BaseObject {
private:

protected:

public:

	b2Body *body_ph;

	StaticObject(sf::Vector2f initCord, std::string texture, std::string name, std::string type, sf::Vector2f size, bool isSensor = false);

	void update();

	~StaticObject();
};


class House;
class Tree;
class Alive;
	class Human;
		class Person;
		class Player;

class Lut;