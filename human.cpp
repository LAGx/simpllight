#include "human.h"
#include "log.h"
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include "phisic.h"
#include <iostream>
#include "window.h"

using namespace std;
using namespace sf;

Human::Human(Vector2f initCord, string textr){
	////////////////graphic part
	if (!texture_body.loadFromFile(textr)) {
		Log::error("Texture load in Human. ", true);
	}
	texture_body.setSmooth(true);
	body.setTexture(texture_body);
	body.setOrigin(Vector2f(16,16));
	body.setPosition(initCord);

	if (!texture_zone.loadFromFile(textr, sf::IntRect(13,13, 2, 2))) {
		Log::error("Texture load in Human. ", true);
	}
	texture_zone.setSmooth(true);
	zone.setRadius(radiusZone);
	zone.setFillColor(Color(230, 255, 255, 30));
	zone.setTexture(&texture_zone);
	zone.setOrigin(Vector2f(radiusZone, radiusZone));
	zone.setPosition(initCord);

	/////////////////phisic part
	b2BodyDef bdef;
	b2PolygonShape b_shape;
	b_shape.SetAsBox(6 / SCALE_BOX, 6 / SCALE_BOX);
	bdef.type = b2_dynamicBody;
	bdef.position.Set(initCord.x/ SCALE_BOX, initCord.y / SCALE_BOX);
	body_ph = World.CreateBody(&bdef);
	body_ph->CreateFixture(&b_shape, 1);
}

void Human::update() {
	b2Vec2 air = -body_ph->GetLinearVelocity();
	air.x = air.x * drug_air;
	air.y = air.y * drug_air;
	body_ph->ApplyForceToCenter(air, true);
	body_ph->SetAngularDamping(drug_angle);

	setTexturePosition(Vector2f(body_ph->GetPosition().x*SCALE_BOX, body_ph->GetPosition().y*SCALE_BOX), body_ph->GetAngle()*DEG_BOX);
	World.Step(1 / 60.f, 8, 3);
}

void Human::setTexturePosition(Vector2f cord, float angle) {
	body.setPosition(cord);
	zone.setPosition(cord);
	body.setRotation(angle);
}

void Human::blit() {
	update();
	spl::ToDraw set_b = {&body, depthRender};
	spl::ToDraw set_z = {&zone, depthRender-1};
	spl::Window::allDrawable.push_back(set_z);
	spl::Window::allDrawable.push_back(set_b);
}

Human::~Human() {

}