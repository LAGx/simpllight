#include "game_objects.h"
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include "phisic.h"
#include "log.h"
#include "window.h"
#include "state.h"

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

bool Fir_tree::decreaseHealth(int delta){
	health -= delta;
	if (health <= 0)
		return 0;
	return 1;
}

void Fir_tree::setHealth(int health) {
	this->health = health;
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

bool Shrub::decreaseHealth(int delta) {
	health -= delta;
	if (health <= 0)
		return 0;
	return 1;
}

void Shrub::setHealth(int health) {
	this->health = health;
}

Shrub::~Shrub() {
}

//////////////////////////////////////////////////////////////////////////////////////////////
////////////        ALIVE        ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////

Alive::Alive(b2World* World, sf::Vector2f initCord, std::string textr, std::string name, figureType type, float figureSize): DynamicObject(World, initCord, textr, name, type, figureSize, false) {

	if (!texture_zone.loadFromFile(textr, sf::IntRect(texture.getSize().x/2, texture.getSize().x / 2, 2, 2))) {
		throw Log::Exception("Texture load in Alive. ");
	}
	texture_zone.setSmooth(true);
	g_zone.setRadius(radiusZone);
	g_zone.setFillColor(Color(230, 255, 255, 30));
	g_zone.setTexture(&texture_zone);

	b2CircleShape z_shape;
	z_shape.m_radius = radiusZone / SCALE_BOX;
	b2FixtureDef z_fixt;
	z_fixt.isSensor = true;
	z_fixt.shape = &z_shape;
	zoneFixt = body_ph->CreateFixture(&z_fixt);
}

void Alive::setRadius(float radius) {
	if (radius > 0) {
		radiusZone = radius;
	}
	updateRadiusZone();
}

#ifdef DEV_MODE
void Alive::moveRadius(float radiusDelta) {
	if (radiusZone + radiusDelta >= 1)
		radiusZone += radiusDelta;
	else
		radiusZone = 1;
	updateRadiusZone();
}
#endif

void Alive::setZoneVisible(bool isVisible) {
	isVisibleZone = isVisible;
}

void Alive::updateRadiusZone() {
	g_zone.setRadius(radiusZone);

	body_ph->DestroyFixture(zoneFixt);

	b2CircleShape z_shape;
	z_shape.m_radius = radiusZone / SCALE_BOX;
	b2FixtureDef z_fixture;
	z_fixture.isSensor = true;
	z_fixture.shape = &z_shape;
	zoneFixt = body_ph->CreateFixture(&z_fixture);
}

void Alive::update() {
	updateZonePos(Vector2f(body_ph->GetPosition().x*SCALE_BOX, body_ph->GetPosition().y*SCALE_BOX));
}

void Alive::updateZonePos(Vector2f cord) {
	g_zone.setPosition(Vector2f(cord.x - radiusZone, cord.y - radiusZone));
}

void Alive::blit() {
	update();
	DynamicObject::blit();

	if (isVisibleZone) {
		spl::ToDraw set_z = { &g_zone, depthRender + 1 };
		spl::Window::allDrawable.push_back(set_z);
	}
}

bool Alive::decreaseHealth(int delta) {
	health -= delta;
	if (health <= 0)
		return 0;
	return 1;
}

void Alive::setHealth(int health) {
	this->health = health;
}

Alive::~Alive() {
}

//////////////////////////////////////////////////////////////////////////////////////////////
////////////       EDITOR        ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////

Editor::Editor(b2World* World, sf::View &view, std::string textrCur):view(view){
	speed = 4;
	cursor = new Cursor(World, textrCur);
	currPosition = view.getCenter();
	currSize = view.getSize();
}

void Editor::blit() {
#include <string>
	try {
		ScreenLog::setValue(4, to_string(speed));
	}
	catch (Log::Exception) {}

	view.setCenter(currPosition);
	view.setSize(currSize);
	cursor->blit();

}

inline void Editor::moveTop() {
	currPosition.y -= speed + 1 / spl::WindowStateBox::absoluteScale*2;
}
inline void Editor::moveBottom() {
	currPosition.y += speed + 1 / spl::WindowStateBox::absoluteScale*2;
}
inline void Editor::moveLeft() {
	currPosition.x -= speed + 1/ spl::WindowStateBox::absoluteScale*2;
}
inline void Editor::moveRight() {
	currPosition.x += speed + 1/ spl::WindowStateBox::absoluteScale*2;
}
inline void Editor::l_ctrl() {
	isPressedContrl = true;
}

inline void Editor::selectMouse() {}
inline void Editor::useMouse() {}

inline void Editor::wheelMouse(float delta) {
	if (isPressedContrl)
		speed += delta/5;
	else {
		currSize *= (-(delta*speed) / (20*(spl::WindowStateBox::absoluteScale*5+1)) + 1);
	}
	isPressedContrl = false;
}

Editor::~Editor() {
	delete cursor;
}