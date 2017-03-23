#include "human.h"
#include "log.h"
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include "phisic.h"
#include <iostream>
#include "window.h"

using namespace std;
using namespace sf;


Human::Human(Vector2f initCord, string textr, string l_name, b2World* World){
	name = l_name;

	////////////////graphic part
	///body
	if (!texture_body.loadFromFile(textr)) {
		Log::error("Texture load in Human. ", true);
	}
	texture_body.setSmooth(true);
	body.setTexture(texture_body);
	body.setOrigin(Vector2f(16,16));
	body.setPosition(initCord);
	///circle
	if (!texture_zone.loadFromFile(textr, sf::IntRect(13,13, 2, 2))) {
		Log::error("Texture load in Human. ", true);
	}
	texture_zone.setSmooth(true);
	zone.setRadius(radiusZone);
	zone.setFillColor(Color(230, 255, 255, 30));
	zone.setTexture(&texture_zone);


	/////////////////phisic part
	///body

	b2BodyDef b_bdef;
	b_bdef.type = b2_dynamicBody;
	b_bdef.linearDamping = drug_air;
	b_bdef.angularDamping = drug_angle;

	b_bdef.position.Set(initCord.x / SCALE_BOX, initCord.y / SCALE_BOX);

	body_ph = World->CreateBody(&b_bdef);
	b2PolygonShape b_shape;
	b_shape.SetAsBox(6 / SCALE_BOX, 6 / SCALE_BOX);
	b2FixtureDef b_fixture;
	b_fixture.isSensor = false;
	b_fixture.shape = &b_shape;
	b_fixture.density = 1;
	body_ph->CreateFixture(&b_fixture);

	///zone
	b2CircleShape z_shape;
	z_shape.m_radius = radiusZone / SCALE_BOX;

	b2FixtureDef z_fixt;
	z_fixt.isSensor = true;
	z_fixt.shape = &z_shape;
	zoneFixt = body_ph->CreateFixture(&z_fixt);
	body_ph->SetUserData(this);
}

void Human::update() {
	updateTexturePosition(Vector2f(body_ph->GetPosition().x*SCALE_BOX-6, body_ph->GetPosition().y*SCALE_BOX-6), body_ph->GetAngle()*DEG_BOX);
}


void Human::updateTexturePosition(Vector2f cord, float angle) {
	body.setPosition(cord);

	zone.setRadius(radiusZone);
	zone.setPosition(Vector2f(cord.x-radiusZone,cord.y- radiusZone));

	body.setRotation(angle);
}

void Human::setRadius(float radius) {
	if (radius > 0) {
			radiusZone = radius;
	}
	updateRadiusZone();
}

void Human::updateRadiusZone() {

	body_ph->DestroyFixture(zoneFixt);

	b2CircleShape z_shape;
	z_shape.m_radius = radiusZone / SCALE_BOX;
	b2FixtureDef z_fixture;
	z_fixture.isSensor = true;
	z_fixture.shape = &z_shape;
	zoneFixt = body_ph->CreateFixture(&z_fixture);
}

void Human::moveRadius(float radiusDelta) {
	if(radiusZone + radiusDelta >= 1)
		radiusZone += radiusDelta;
	else
		radiusZone = 1;
	updateRadiusZone();
}

void Human::setZoneVisible(bool isVisible) {
	isVisibleZone = isVisible;
}

void Human::blit() {
	update();

	spl::ToDraw set_b = { &body, depthRender };

	if (isVisibleZone) {
		spl::ToDraw set_z = { &zone, depthRender + 1 };
		spl::Window::allDrawable.push_back(set_z);
	}
	spl::Window::allDrawable.push_back(set_b);
}

Human::~Human() {
	//World.DestroyBody(zone_ph);
}