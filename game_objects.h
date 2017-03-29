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
protected:
	float drug_air = 1;
	float drug_angle = 1;
	float mass = 1;

	void update();
public:

	std::string name = "None";

	b2Body *body_ph;

	//can be load only square texture. for rect сan be not square
	//figureSize - radius of figure. for rect it`s shadow size
	DynamicObject(b2World* World, sf::Vector2f initCord, std::string texture, std::string name, figureType type, float figureSize = 0, bool isSensor = false);//circle, tringle
	


	void blit();

	~DynamicObject();
};


class StaticObject :public BaseObject {
protected:
	b2Body *body_ph;
public:
	
	std::string name = "None";

	//can be load only square texture. for rect сan be not square
	//figureSize - radius of figure. for rect it`s shadow size
	//angle in degree. 
	StaticObject(b2World* World, sf::Vector2f initCord,float angle, std::string texture, std::string name, figureType type, float figureSize = 0, bool isSensor = false);

	~StaticObject();
};


class House :public StaticObject {
public:
	StaticObject *door;

	House(b2World* World, sf::Vector2f initCord, float angle, std::string textureHouse, std::string textureDoor, std::string name);

	void blit();

	~House();
};

class Fir_tree :public StaticObject {
private:
	int helth = 100;
public:

	//texture 40x40. size - 17
	Fir_tree(b2World* World, sf::Vector2f initCord, std::string textr, std::string name);

	void setHelth(int helth);
	bool decreaseHelth(int delta = 0); //1 - if health >= 0

	~Fir_tree();
};


class Shrub :public StaticObject {
private:
	int helth = 100;
public:

	//sahdow size 15, rect trtigger
	Shrub(b2World* World, sf::Vector2f initCord, std::string textr, std::string name);

	void setHelth(int helth);
	bool decreaseHelth(int delta = 0); //1 - if health >= 0

	~Shrub();
};


class Alive :public DynamicObject {
private:

	b2Fixture *zoneFixt;
	sf::Texture texture_zone;
	sf::CircleShape g_zone;
	void updateZonePos(sf::Vector2f cord);

	float radiusZone = 30.f;
	bool isVisibleZone = true;

	void updateRadiusZone();

protected:

	void update();
	int helth = 100;

public:

	Alive(b2World* World, sf::Vector2f initCord, std::string textr, std::string name, figureType type, float figureSize);

	void setHelth(int helth);
	bool decreaseHelth(int delta = 0); //1 - if health >= 0

	void setRadius(float radius = 0);
	void moveRadius(float radius_delta = 0);
	void setZoneVisible(bool isVisible = true);

	void blit();
	
	~Alive();

};


class Human :public Alive {
private:

protected:

public:

	Human();

	~Human();
};

class Person : public Human {
};
class Player : public Human {
};

class Lut;
