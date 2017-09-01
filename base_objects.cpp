#include "game_objects.h"

#include <SFML/System/Vector2.hpp>
#include <Box2D/Box2D.h>
#include <string>

#include "phisic.h"
#include "log.h"
#include "window.h"
#include "state.h"

using Vector2f = sf::Vector2f;


//////////////////////////////////////////////////////////////////////////////////////////////
////////////        BASE OBJECT       ///////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////


BaseObject::BaseObject(sf::Vector2f initCord, std::string textr) {
	if (!texture.loadFromFile(textr)) {
		throw Log::Exception("Texture load in BaseObject from " + textr, true);
	}
	texture.setSmooth(true);
	texturePath = textr;
	g_body.setTexture(texture);
	g_body.setOrigin(Vector2f(texture.getSize().x / 2, texture.getSize().y / 2));
	g_body.setPosition(initCord);
}

const sf::Vector2f BaseObject::getCoordinates() const
{
	return Vector2f(g_body.getPosition().x*SCALE_BOX, g_body.getPosition().y*SCALE_BOX);
}

const float BaseObject::getRotation() const
{
	return g_body.getRotation();
}

const std::string &BaseObject::getTexturePath() const
{
	return texturePath;
}

void BaseObject::freezeObject()
{
	isVisible = false;
}

void BaseObject::unFreezeObject()
{
	isVisible = true;
}

void BaseObject::updateTextrPosition(sf::Vector2f newCord, float newAngle) {
	g_body.setPosition(newCord);
	g_body.setRotation(newAngle);
}

void BaseObject::blit() {

	if (isVisible) {
		spl::ToDraw draw = { &g_body, depthRender };
		spl::Window::allDrawable.push_back(draw);
	}
}

BaseObject::~BaseObject() {

}

//////////////////////////////////////////////////////////////////////////////////////////////
////////////        DYNAMIC OBJECT       ////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////

DynamicObject::DynamicObject(b2World* World, sf::Vector2f initCord, std::string textr, figureType type, float figureSize, bool isSensor) :BaseObject(initCord, textr) {

	if ((texture.getSize().x - texture.getSize().y) && type != figureType::rect_T)
		throw Log::Exception("Texture " + textr + " have to be square.");

	b2BodyDef b_bdef;
	b_bdef.type = b2_dynamicBody;
	b_bdef.linearDamping = 1;
	b_bdef.angularDamping = 1;
	b_bdef.position.Set(initCord.x / SCALE_BOX, initCord.y / SCALE_BOX);
	body_ph = World->CreateBody(&b_bdef);
	world = World;

	b2FixtureDef b_fixture;
	b2PolygonShape b_shape;
	b2CircleShape c_shape;
	b2Vec2 vec[6];
	float x = texture.getSize().x;
	float y = texture.getSize().y;
	switch (type) {
	case figureType::circle_T: {
		c_shape.m_radius = figureSize / SCALE_BOX;
		b_fixture.shape = &c_shape;
		break;
	}
	case figureType::tringle_T: {
		vec[0].Set(0, -figureSize);
		vec[1].Set(figureSize / sqrt(1 + tan(PI / 6)*tan(PI / 6)), (tan(PI / 6)*figureSize) / sqrt(1 + tan(PI / 6)*tan(PI / 6)));
		vec[2].Set(-figureSize / sqrt(1 + tan(PI / 6)*tan(PI / 6)), (tan(PI / 6)*figureSize) / sqrt(1 + tan(PI / 6)*tan(PI / 6)));
		for (int i = 0; i < 3; i++) {
			vec[i].x = vec[i].x / SCALE_BOX;
			vec[i].y = vec[i].y / SCALE_BOX;
		}
		b_shape.Set(vec, 3);
		b_fixture.shape = &b_shape;
		break;
	}
	case figureType::rect_T: {
		vec[0].Set(-x / 2 + figureSize, y / 2 - figureSize);
		vec[1].Set(-x / 2 + figureSize, -y / 2 + figureSize);
		vec[2].Set(x / 2 - figureSize, -y / 2 + figureSize);
		vec[3].Set(x / 2 - figureSize, y / 2 - figureSize);
		for (int i = 0; i < 4; i++) {
			vec[i].x = vec[i].x / SCALE_BOX;
			vec[i].y = vec[i].y / SCALE_BOX;
		}
		b_shape.Set(vec, 4);
		b_fixture.shape = &b_shape;
		break;
	}
	case figureType::pentagon_T: {
		vec[0].Set(0, figureSize);
		vec[1].Set(-figureSize / sqrt(1 + tan(18.f / DEG_BOX)*tan(18.f / DEG_BOX)), (tan(18.f / DEG_BOX)*figureSize) / sqrt(1 + tan(18.f / DEG_BOX)*tan(18.f / DEG_BOX)));
		vec[2].Set(-figureSize / sqrt(1 + tan(54.f / DEG_BOX)*tan(54.f / DEG_BOX)), (-tan(54.f / DEG_BOX)*figureSize) / sqrt(1 + tan(54.f / DEG_BOX)*tan(54.f / DEG_BOX)));
		vec[3].Set(figureSize / sqrt(1 + tan(54.f / DEG_BOX)*tan(54.f / DEG_BOX)), (-tan(54.f / DEG_BOX)*figureSize) / sqrt(1 + tan(54.f / DEG_BOX)*tan(54.f / DEG_BOX)));
		vec[4].Set(figureSize / sqrt(1 + tan(18.f / DEG_BOX)*tan(18.f / DEG_BOX)), (tan(18.f / DEG_BOX)*figureSize) / sqrt(1 + tan(18.f / DEG_BOX)*tan(18.f / DEG_BOX)));
		for (int i = 0; i < 5; i++) {
			vec[i].x = vec[i].x / SCALE_BOX;
			vec[i].y = vec[i].y / SCALE_BOX;
		}
		b_shape.Set(vec, 5);
		b_fixture.shape = &b_shape;
		break;
	}
	case figureType::intnugon_T: {
		vec[0].Set(0, figureSize);
		vec[1].Set(-figureSize / sqrt(1 + tan(PI / 6)*tan(PI / 6)), (tan(PI / 6)*figureSize) / sqrt(1 + tan(PI / 6)*tan(PI / 6)));
		vec[2].Set(-figureSize / sqrt(1 + tan(PI / 6)*tan(PI / 6)), (-tan(PI / 6)*figureSize) / sqrt(1 + tan(PI / 6)*tan(PI / 6)));
		vec[3].Set(0, -figureSize);
		vec[4].Set(figureSize / sqrt(1 + tan(PI / 6)*tan(PI / 6)), (tan(PI / 6)*figureSize) / sqrt(1 + tan(PI / 6)*tan(PI / 6)));
		vec[5].Set(figureSize / sqrt(1 + tan(PI / 6)*tan(PI / 6)), (-tan(PI / 6)*figureSize) / sqrt(1 + tan(PI / 6)*tan(PI / 6)));
		for (int i = 0; i < 6; i++) {
			vec[i].x = vec[i].x / SCALE_BOX;
			vec[i].y = vec[i].y / SCALE_BOX;
		}
		b_shape.Set(vec, 6);
		b_fixture.shape = &b_shape;
		break;
	}
	default:
		throw Log::Exception("No such type (figure type) in DynamicObject");
	}

	b_fixture.isSensor = isSensor;
	b_fixture.density = 1;
	body_ph->CreateFixture(&b_fixture);
	body_ph->SetUserData(this);
}

const sf::Vector2f DynamicObject::getCoordinates() const
{
	return Vector2f(body_ph->GetPosition().x*SCALE_BOX, body_ph->GetPosition().y*SCALE_BOX);
}

void DynamicObject::freezeObject()
{
	BaseObject::freezeObject();
	body_ph->SetActive(false);
}

void DynamicObject::unFreezeObject()
{
	BaseObject::unFreezeObject();
	body_ph->SetActive(true);
}

void DynamicObject::setDrag(float linear, float angular) {
	body_ph->SetLinearDamping(linear);
	body_ph->SetAngularDamping(angular);
}

void DynamicObject::update() {
	updateTextrPosition(Vector2f(body_ph->GetPosition().x*SCALE_BOX, body_ph->GetPosition().y*SCALE_BOX), body_ph->GetAngle()*DEG_BOX);
}

void DynamicObject::blit() {
	update();
	if (isVisible) {
		spl::ToDraw draw = { &g_body, depthRender };
		spl::Window::allDrawable.push_back(draw);
	}
}

DynamicObject::~DynamicObject() {
	world->DestroyBody(body_ph);
}


//////////////////////////////////////////////////////////////////////////////////////////////
////////////        STATIC OBJECT        ////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////

StaticObject::StaticObject(b2World* World, sf::Vector2f initCord, float angle, std::string textr, figureType type, float figureSize, bool isSensor) :BaseObject(initCord, textr) {

	if ((texture.getSize().x - texture.getSize().y) && type != figureType::rect_T)
		throw Log::Exception("Texture " + textr + " have to be square.");

	b2BodyDef b_bdef;
	b_bdef.type = b2_staticBody;
	b_bdef.position.Set(initCord.x / SCALE_BOX, initCord.y / SCALE_BOX);
	body_ph = World->CreateBody(&b_bdef);
	world = World;

	b2FixtureDef b_fixture;
	b2PolygonShape b_shape;
	b2CircleShape c_shape;
	b2Vec2 vec[6];
	float x = texture.getSize().x;
	float y = texture.getSize().y;
	switch (type) {
	case figureType::circle_T: {
		c_shape.m_radius = figureSize / SCALE_BOX;
		b_fixture.shape = &c_shape;
		break;
	}
	case figureType::tringle_T: {
		vec[0].Set(0, -figureSize);
		vec[1].Set(figureSize / sqrt(1 + tan(PI / 6)*tan(PI / 6)), (tan(PI / 6)*figureSize) / sqrt(1 + tan(PI / 6)*tan(PI / 6)));
		vec[2].Set(-figureSize / sqrt(1 + tan(PI / 6)*tan(PI / 6)), (tan(PI / 6)*figureSize) / sqrt(1 + tan(PI / 6)*tan(PI / 6)));
		for (int i = 0; i < 3; i++) {
			vec[i].x = vec[i].x / SCALE_BOX;
			vec[i].y = vec[i].y / SCALE_BOX;
		}
		b_shape.Set(vec, 3);
		b_fixture.shape = &b_shape;
		break;
	}
	case figureType::rect_T: {
		vec[0].Set(-x / 2 + figureSize, y / 2 - figureSize);
		vec[1].Set(-x / 2 + figureSize, -y / 2 + figureSize);
		vec[2].Set(x / 2 - figureSize, -y / 2 + figureSize);
		vec[3].Set(x / 2 - figureSize, y / 2 - figureSize);
		for (int i = 0; i < 4; i++) {
			vec[i].x = vec[i].x / SCALE_BOX;
			vec[i].y = vec[i].y / SCALE_BOX;
		}
		b_shape.Set(vec, 4);
		b_fixture.shape = &b_shape;
		break;
	}
	case figureType::pentagon_T: {
		vec[0].Set(0, figureSize);
		vec[1].Set(-figureSize / sqrt(1 + tan(18.f / DEG_BOX)*tan(18.f / DEG_BOX)), (tan(18.f / DEG_BOX)*figureSize) / sqrt(1 + tan(18.f / DEG_BOX)*tan(18.f / DEG_BOX)));
		vec[2].Set(-figureSize / sqrt(1 + tan(54.f / DEG_BOX)*tan(54.f / DEG_BOX)), (-tan(54.f / DEG_BOX)*figureSize) / sqrt(1 + tan(54.f / DEG_BOX)*tan(54.f / DEG_BOX)));
		vec[3].Set(figureSize / sqrt(1 + tan(54.f / DEG_BOX)*tan(54.f / DEG_BOX)), (-tan(54.f / DEG_BOX)*figureSize) / sqrt(1 + tan(54.f / DEG_BOX)*tan(54.f / DEG_BOX)));
		vec[4].Set(figureSize / sqrt(1 + tan(18.f / DEG_BOX)*tan(18.f / DEG_BOX)), (tan(18.f / DEG_BOX)*figureSize) / sqrt(1 + tan(18.f / DEG_BOX)*tan(18.f / DEG_BOX)));
		for (int i = 0; i < 5; i++) {
			vec[i].x = vec[i].x / SCALE_BOX;
			vec[i].y = vec[i].y / SCALE_BOX;
		}
		b_shape.Set(vec, 5);
		b_fixture.shape = &b_shape;
		break;
	}
	case figureType::intnugon_T: {
		vec[0].Set(0, figureSize);
		vec[1].Set(-figureSize / sqrt(1 + tan(PI / 6)*tan(PI / 6)), (tan(PI / 6)*figureSize) / sqrt(1 + tan(PI / 6)*tan(PI / 6)));
		vec[2].Set(-figureSize / sqrt(1 + tan(PI / 6)*tan(PI / 6)), (-tan(PI / 6)*figureSize) / sqrt(1 + tan(PI / 6)*tan(PI / 6)));
		vec[3].Set(0, -figureSize);
		vec[4].Set(figureSize / sqrt(1 + tan(PI / 6)*tan(PI / 6)), (tan(PI / 6)*figureSize) / sqrt(1 + tan(PI / 6)*tan(PI / 6)));
		vec[5].Set(figureSize / sqrt(1 + tan(PI / 6)*tan(PI / 6)), (-tan(PI / 6)*figureSize) / sqrt(1 + tan(PI / 6)*tan(PI / 6)));
		for (int i = 0; i < 6; i++) {
			vec[i].x = vec[i].x / SCALE_BOX;
			vec[i].y = vec[i].y / SCALE_BOX;
		}
		b_shape.Set(vec, 6);
		b_fixture.shape = &b_shape;
		break;
	}
	default:
		throw Log::Exception("No such type (figure type) in StaticObject");
	}

	b_fixture.isSensor = isSensor;
	body_ph->CreateFixture(&b_fixture);
	body_ph->SetTransform(body_ph->GetPosition(), angle / DEG_BOX);
	body_ph->SetUserData(this);
	updateTextrPosition(Vector2f(body_ph->GetPosition().x*SCALE_BOX, body_ph->GetPosition().y*SCALE_BOX), body_ph->GetAngle()*DEG_BOX);
}

const sf::Vector2f StaticObject::getCoordinates() const
{
	return Vector2f(body_ph->GetPosition().x*SCALE_BOX, body_ph->GetPosition().y*SCALE_BOX);
}

void StaticObject::freezeObject()
{
	BaseObject::freezeObject();
	body_ph->SetActive(false);
}

void StaticObject::unFreezeObject()
{
	BaseObject::unFreezeObject();
	body_ph->SetActive(true);
}

StaticObject::~StaticObject() {
	world->DestroyBody(body_ph);
}


//////////////////////////////////////////////////////////////////////////////////////////////
////////////        CURSOR               ////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////

Cursor::Cursor(b2World* World, std::string textr) :DynamicObject(World, sf::Vector2f(0, 0), textr, figureType::circle_T, 1, true) {
	depthRender = -500;
}

void Cursor::positionMouse(int x, int y) {
	body_ph->SetTransform(b2Vec2((x + spl::Window::currGlobalViewCord.x - spl::Window::screenSize.x / 2) / SCALE_BOX, (y + spl::Window::currGlobalViewCord.y - spl::Window::screenSize.y / 2) / SCALE_BOX), 0);
#ifdef DEV_MODE
	ScreenLog::setValue(1, std::to_string(x) + " | " + std::to_string(y));
#endif
}