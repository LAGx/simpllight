#include "game_objects.h"

#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>

#include "service.h"


//////////////////////////////////////////////////////////////////////////////////////////////
////////////        HUMAN                ////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////

Human::Human(b2World* World, sf::Vector2f initCord, std::string textr, int health) :Alive(World, initCord, textr, figureType::rect_T, 10, health) {
	setDrag(3, 2);
}

Human::~Human() {
}

//////////////////////////////////////////////////////////////////////////////////////////////
////////////        PERSON               ////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////

Person::Person(b2World* World, sf::Vector2f initCord, std::string textr, int health) :Human(World, initCord, textr, health) {
	body_ph->SetActive(true);
}


void Person::blit() {
	this->update();
	Human::blit();
}


void Person::update() {
	PAIForce();
}


void Person::PAIForce() {

	if (PAIdelta < PAIiter) {
		PAIdelta++;
		body_ph->ApplyForceToCenter(PAIvec, true);
	}
	else {
		PAIiter = spl::Rand::intRand(60, 240);
		PAIdelta = 0;
		b2Vec2 temp(0, 0);
		float x = spl::Rand::intRand(0, 10), y = spl::Rand::intRand(0, 10);
		temp.Set(x, y);
		temp.Normalize();
		if (spl::Rand::intRand(0, 1))
			PAIvec = -speed*temp;
		else
			PAIvec = speed*temp;
	}
}


Person::~Person() {

}


//////////////////////////////////////////////////////////////////////////////////////////////
////////////        PLAYER               ////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////

Player::Player(b2World* World, sf::Vector2f initCord, int health, std::string textr, std::string textrCur) :Human(World, initCord, textr, health) {
	if (textrCur != "None")
		cursor = new Cursor(World, textrCur);
	speed = 2;
}

void Player::freezeObject()
{
	DynamicObject::freezeObject();
	speed = 0;
}

void Player::blit() {
	if (cursor != nullptr)
		cursor->blit();
	update();
	Human::blit();
}

void Player::update() {
	currForceVec.Normalize();
	currForceVec *= speed;
	body_ph->ApplyForceToCenter(currForceVec, true);
	currForceVec.SetZero();

	if (speed > 2)
		speed -= 0.05;
}


inline void Player::moveTop() {
	b2Vec2 curr(0, -1);
	currForceVec += curr;
}
inline void Player::moveBottom() {
	b2Vec2 curr(0, 1);
	currForceVec += curr;
}
inline void Player::moveLeft() {
	b2Vec2 curr(-1, 0);
	currForceVec += curr;
}
inline void Player::moveRight() {
	b2Vec2 curr(1, 0);
	currForceVec += curr;
}

inline void Player::haste() {
	if (speed < 5)
		speed += 0.1;
}

Player::~Player() {
	if (cursor != nullptr) {
		delete cursor;
		cursor = nullptr;
	}
}

