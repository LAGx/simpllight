#include "game_objects.h"
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include "log.h"
#include "window.h"
using namespace std;
using namespace sf;
//////////////////////////////////////////////////////////////////////////////////////////////
////////////        BASE OBJECT       ///////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////


BaseObject::BaseObject(Vector2f initCord, string textr) {
	if (!texture.loadFromFile(textr)) {
		Log::error("Texture load in BaseObjec from " + textr, true);
	}
	texture.setSmooth(true);
	g_body.setTexture(texture);
	g_body.setOrigin(Vector2f(texture.getSize().x/2, texture.getSize().y/2));
	g_body.setPosition(initCord);
}


void BaseObject::updatePosition(sf::Vector2f newCord, float newAngle) {
	g_body.setPosition(newCord);
	g_body.setRotation(newAngle);
}


void BaseObject::blit() {

	if (isVisible) {
		spl::ToDraw draw = { &g_body, depthRender + 1 };
		spl::Window::allDrawable.push_back(draw);
	}
}

BaseObject::~BaseObject() {

}