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

//////////////////////////////////////////////////////////////////////////////////////////////
////////////        ALIVE        ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////

Alive::Alive(b2World* World, sf::Vector2f initCord, std::string textr, std::string name, figureType type, float figureSize): DynamicObject(World, initCord, textr, name, type, figureSize, false) {

	if (!texture_zone.loadFromFile(textr, sf::IntRect(texture.getSize().x/2, texture.getSize().x / 2, 2, 2))) {
		Log::error("Texture load in Alive. ");
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

void Alive::moveRadius(float radiusDelta) {
	if (radiusZone + radiusDelta >= 1)
		radiusZone += radiusDelta;
	else
		radiusZone = 1;
	updateRadiusZone();
}

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

bool Alive::decreaseHelth(int delta) {
	helth -= delta;
	if (helth <= 0)
		return 0;
	return 1;
}

void Alive::setHelth(int helth) {
	this->helth = helth;
}

Alive::~Alive() {
}

//////////////////////////////////////////////////////////////////////////////////////////////
////////////        ALIVE        ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////

