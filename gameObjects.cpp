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
#include <string>
	Log::log(name +": " + to_string(texture.getSize().y) + " | " + to_string(texture.getSize().y));
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
////////////        YOLKA        ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////


Yolka::Yolka(b2World* World, sf::Vector2f initCord, std::string textr, std::string name) :StaticObject(World, initCord,0, textr, name, tringle_T, 17, false){

}

Yolka::~Yolka() {
}