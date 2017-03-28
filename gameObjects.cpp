#include "game_objects.h"
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include "phisic.h"
#include "log.h"
#include "window.h"

using namespace std;
using namespace sf;

//////////////////////////////////////////////////////////////////////////////////////////////
////////////        HOUSE          //////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////

House::House(b2World* World, Vector2f initCord, float angle, string textureHouse, string textureDoor, string name) :StaticObject(World, initCord, angle, textureHouse, name, rect_T, 10, false) {
	depthRender = -100;
	Vector2f doorCord(initCord);
	doorCord.x = doorCord.x + ((texture.getSize().y)/2-10) * sin(angle / DEG_BOX);
	doorCord.y = doorCord.y - ((texture.getSize().y)/2-10) * cos(angle / DEG_BOX);
	door = new StaticObject(World, doorCord, angle,textureDoor,name+"_d",rect_T,4,true);
	door->depthRender = depthRender - 1;
}

void House::blit() {
	door->blit();
	BaseObject::blit();
}

House::~House() {
	delete door;
}

//////////////////////////////////////////////////////////////////////////////////////////////
////////////        FIR_TREE     ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////


Fir_tree::Fir_tree(b2World* World, sf::Vector2f initCord, std::string textr, std::string name) :StaticObject(World, initCord, 0, textr, name, tringle_T, 17, false) {
}

bool Fir_tree::decreaseHelth(int delta){
	helth -= delta;
	if (helth <= 0)
		return 0;
	return 1;
}

void Fir_tree::setHelth(int helth) {
	this->helth = helth;
}

Fir_tree::~Fir_tree() {
}

//////////////////////////////////////////////////////////////////////////////////////////////
////////////        SHRUB        ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////


Shrub::Shrub(b2World* World, sf::Vector2f initCord, std::string textr, std::string name) :StaticObject(World, initCord, 0, textr, name, rect_T, 15, true) {
	g_body.setColor(sf::Color(255, 255, 255, 175));
	depthRender = -200;
}

bool Shrub::decreaseHelth(int delta) {
	helth -= delta;
	if (helth <= 0)
		return 0;
	return 1;
}

void Shrub::setHelth(int helth) {
	this->helth = helth;
}

Shrub::~Shrub() {
}
