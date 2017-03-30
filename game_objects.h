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

	virtual void blit();

	virtual ~BaseObject();
};


class DynamicObject :public BaseObject {
private:
	void update();
	
protected:

	//TODO maybe...
	void setPhisicParameters(float mass, int linear_drug, int angular_drug);//better to use only in constructors

	void setDrug(float linear, float angular);

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


	//shadow size 10
	//door shadow 4
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

	void update();
protected:


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
	float speed = 0.5;
public:

	//Human texture have to be a single color with shadow(10 px) 32x32. 
	Human(b2World* World, sf::Vector2f initCord, std::string textr, std::string name);

	~Human();
};


class Person : public Human {
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


class Player : public Human {
private:

protected:

public:

	Player(b2World* World, sf::Vector2f initCord, std::string textr, std::string name);

	void blit();

	~Player();
};

