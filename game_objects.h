#pragma once
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <string>

enum figureType {
	circle_T = 1,
	tringle_T,
	rect_T,
	pentagon_T,
	intnugon_T
};

class BaseObject {
protected:
	sf::Texture texture;
	sf::Sprite g_body;

	void updateTextrPosition(sf::Vector2f newCord, float newAngle);
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

	b2Fixture *zoneFixt;
public:

	std::string name = "None";

	b2Body *body_ph;

	//can be load only square texture. for rect сan be not square
	//figureSize - radius of figure. for rect it`s shadow size
	DynamicObject(b2World* World, sf::Vector2f initCord, std::string texture, std::string name, figureType type, float figureSize = 0, bool isSensor = false);//circle, tringle
	
	void update();

	void blit();

	~DynamicObject();
};


class StaticObject :public BaseObject {
private:

protected:

public:

	b2Body *body_ph;

	StaticObject(sf::Vector2f initCord, std::string textr, std::string name, std::string type, sf::Vector2f size, bool isSensor = false);

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