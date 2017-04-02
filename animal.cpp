#include "game_objects.h"
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include "service.h"

using namespace std;
using namespace sf;

//////////////////////////////////////////////////////////////////////////////////////////////
////////////        HUMAN                ////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////

Human::Human(b2World* World, Vector2f initCord, string textr, string name) :Alive(World, initCord, textr, name, rect_T, 10) {
	setDrug(3, 2);
} 

Human::~Human() {
}

//////////////////////////////////////////////////////////////////////////////////////////////
////////////        PERSON               ////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////

Person::Person(b2World* World, Vector2f initCord, string textr, string name) :Human(World, initCord, textr, name) {

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
		PAIiter = Rand::intRand(60,240);
		PAIdelta = 0;
		b2Vec2 temp(0, 0);
		float x = Rand::intRand(0, 10), y = Rand::intRand(0, 10);
		temp.Set(x,y);
		temp.Normalize();
		if (Rand::intRand(0, 1))
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

Player::Player(b2World* World, Vector2f initCord, string textr, string name) :Human(World, initCord, textr, name) {
	speed = 2;
}


void Player::blit(){
	update();
	Human::blit();
}

void Player::update() {
	currForceVec.Normalize();
	currForceVec *= speed;
	body_ph->ApplyForceToCenter(currForceVec, true);
	currForceVec.SetZero();
}


void Player::moveTop() {
	b2Vec2 curr(0, -1);
	currForceVec += curr;
}
void Player::moveBottom() {
	b2Vec2 curr(0, 1);
	currForceVec += curr;
}
void Player::moveLeft() {
	b2Vec2 curr(-1, 0);
	currForceVec += curr;
}
void Player::moveRight() {
	b2Vec2 curr(1, 0);
	currForceVec += curr;
}
void Player::moveRadiusPlus() {
	moveRadius(radiusSpeed);
}
void Player::moveRadiusMinus() {
	moveRadius(-radiusSpeed);
}


Player::~Player() {
}
