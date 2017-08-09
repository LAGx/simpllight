#ifndef _GAME_OBJECTS_
#define _GAME_OBJECTS_

#pragma once
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <string>

#include "control.h"
#include "state.h"

enum class figureType {
	circle_T = 1,
	tringle_T,
	rect_T,
	pentagon_T,
	intnugon_T
};

class BaseObject {
protected:
	sf::Texture texture;
	std::string texturePath;

	sf::Sprite g_body;

	void updateTextrPosition(sf::Vector2f newCord, float newAngle);
	bool isVisible = true;

public:
	int depthRender = 0;

	BaseObject(sf::Vector2f initCord, std::string texture);

	const sf::Vector2f getCoordinates() const;

	const float getRotation() const;

	const std::string &getTexturePath() const;

	virtual void freezeObject();

	virtual void unFreezeObject();

	virtual void blit();

	virtual ~BaseObject();
};


class DynamicObject :public BaseObject {
private:
	void update();

protected:

	void setDrag(float linear, float angular);

public:

	b2Body *body_ph;

	//can be load only square texture. for rect сan be not square
	//figureSize - radius of figure. for rect it`s shadow size
	DynamicObject(b2World* World, sf::Vector2f initCord, std::string texture, figureType type, float figureSize = 0, bool isSensor = false);

	const sf::Vector2f getCoordinates() const;

	void freezeObject();

	void unFreezeObject();

	void blit();

	~DynamicObject();
};


class StaticObject :public BaseObject {
protected:
	b2Body *body_ph;
public:

	//can be load only square texture. for rect сan be not square
	//figureSize - radius of figure. for rect it`s shadow size
	//angle in degree. 
	StaticObject(b2World* World, sf::Vector2f initCord, float angle, std::string texture, figureType type, float figureSize = 0, bool isSensor = false);

	const sf::Vector2f getCoordinates() const;

	void freezeObject();

	void unFreezeObject();

	~StaticObject();
};


class House :public StaticObject {
public:
	StaticObject *door;

	//shadow size 10
	//door shadow 4
	House(b2World* World, sf::Vector2f initCord, float angle, std::string textureHouse, std::string textureDoor);

	void blit();

	void freezeObject();

	void unFreezeObject();

	~House();
};

class Fir_tree :public StaticObject {
private:
	int health;
public:

	//texture 40x40. size - 17
	Fir_tree(b2World* World, sf::Vector2f initCord, std::string textr, int health);

	void setHealth(int health);
	bool decreaseHealth(int delta = 0); //1 - if health >= 0
	const int &getHealth() const;

	~Fir_tree();
};


class Shrub :public StaticObject {
private:
	int health;
public:

	//sahdow size 15, rect trtigger
	Shrub(b2World* World, sf::Vector2f initCord, std::string textr, int health);

	void setHealth(int health);
	bool decreaseHealth(int delta = 0); //1 - if health >= 0
	const int &getHealth() const;

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

	void update();
protected:
	float radiusSpeed = 1;
	float speed = 1;
	int health;

public:

	Alive(b2World* World, sf::Vector2f initCord, std::string textr, figureType type, float figureSize, int health);

	void setHealth(int health);
	bool decreaseHealth(int delta = 0); //1 - if health >= 0
	const int &getHealth() const;

	void freezeObject();

	void unFreezeObject();


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

	//Human texture have to be a single color with shadow(10 px) 32x32. 
	Human(b2World* World, sf::Vector2f initCord, std::string textr, int health);

	~Human();
};


class Person : public Human {
private:
	void PAIForce();//pseudo AI
	void update();

	b2Vec2 PAIvec;
	int PAIdelta = 0;
	int PAIiter = 0;
public:
	Person(b2World* World, sf::Vector2f initCord, std::string textr, int health);

	void blit();

	~Person();
};

class Cursor : public DynamicObject, public spl::EventInterface {
private:
	void positionMouse(int x, int y);
public:
	Cursor(b2World* World, std::string textr);
};


class Player : public Human, public spl::EventInterface {
private:
	b2Vec2 currForceVec;
protected:
	void moveTop();
	void moveBottom();
	void moveLeft();
	void moveRight();
	void haste();

	void update();
public:
	Cursor *cursor = nullptr;

	Player(b2World* World, sf::Vector2f initCord, int health, std::string textr, std::string textrCur = "None");

	void freezeObject();

	void blit();

	~Player();
};
#endif