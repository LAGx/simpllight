#ifndef _GAME_OBJECTS_
#define _GAME_OBJECTS_

#pragma once
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <string>
#include "control.h"
#include "state.h"
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

	void updateTextrPosition(sf::Vector2f newCord, float newAngle = 0);
	bool isVisible = true;

public:
	int depthRender = 0;

	BaseObject(sf::Vector2f initCord, std::string texture);

	virtual void blit();

	virtual ~BaseObject();
};


class PhantomObject:public BaseObject {
public:
	
	PhantomObject(sf::Vector2f initCord, std::string texture);

	void setPosition(int x, int y, float angle = 0);

	sf::Vector3f getPosition();

};


class DynamicObject :public BaseObject {
private:
	void update();
	
protected:

	void setDrug(float linear, float angular);

public:

	std::string name = "None";

	b2Body *body_ph;

	//can be load only square texture. for rect сan be not square
	//figureSize - radius of figure. for rect it`s shadow size
	DynamicObject(b2World* World, sf::Vector2f initCord, std::string texture, std::string name, figureType type, float figureSize = 0, bool isSensor = false);

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


	//shadow size 10
	//door shadow 4
	House(b2World* World, sf::Vector2f initCord, float angle, std::string textureHouse, std::string textureDoor, std::string name);

	void blit();

	~House();
};

class Fir_tree :public StaticObject {
private:
	int health = 100;
public:

	//texture 40x40. size - 17
	Fir_tree(b2World* World, sf::Vector2f initCord, std::string textr, std::string name);

	void setHealth(int health);
	bool decreaseHealth(int delta = 0); //1 - if health >= 0

	~Fir_tree();
};


class Shrub :public StaticObject {
private:
	int health = 100;
public:

	//sahdow size 15, rect trtigger
	Shrub(b2World* World, sf::Vector2f initCord, std::string textr, std::string name);

	void setHealth(int health);
	bool decreaseHealth(int delta = 0); //1 - if health >= 0

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
	int health = 100;

public:

	Alive(b2World* World, sf::Vector2f initCord, std::string textr, std::string name, figureType type, float figureSize);

	void setHealth(int health);
	bool decreaseHealth(int delta = 0); //1 - if health >= 0

	void setRadius(float radius = 0);
#ifdef DEV_MODE
	void moveRadius(float radius_delta = 0);
#endif
	void setZoneVisible(bool isVisible = true);

	void blit();
	
	~Alive();

};


class Human :public Alive {
private:

protected:

public:
	
	//Human texture have to be a single color with shadow(10 px) 32x32. 
	Human(b2World* World, sf::Vector2f initCord, std::string textr, std::string name);

	~Human();
};


class Person : public Human{
private:
	void PAIForce();//pseudo AI
	void update();

	b2Vec2 PAIvec;
	int PAIdelta = 0;
	int PAIiter = 0;

protected:
	
public:
	Person(b2World* World, sf::Vector2f initCord, std::string textr, std::string name);

	void blit();

	~Person();
};

class Cursor : public DynamicObject, public spl::EventInterface {
private:
	void positionMouse(int x, int y);
public:
	sf::Vector2f getPosition();

	Cursor(b2World* World, std::string textr, std::string name = "cursor");
};


class Editor: public spl::EventInterface {
private:
	bool isPressedContrl = false;
	float speed = 1;
	float delta = 0;
	sf::Vector2f currSize;
	sf::Vector2f currPosition;

	sf::View &view;

	PhantomObject *phantom = nullptr;
protected:
	void moveTop();
	void moveBottom();
	void moveLeft();
	void moveRight();
	void l_ctrl();

	void selectMouse();
	void useMouse();
	void wheelMouse(float delta);

public:

	Cursor *cursor;

	Editor(b2World* World, sf::View &view, std::string textrCur = "None");


	void blit();

	~Editor();
};


class Player : public Human, public spl::EventInterface{
private:
	b2Vec2 currForceVec;

protected:
	void moveTop();
	void moveBottom();
	void moveLeft();
	void moveRight();	

	void update();
public:
	Cursor *cursor;

	Player(b2World* World, sf::Vector2f initCord, std::string textr, std::string name, std::string textrCur = "None");

	void blit();

	~Player();
};
#endif