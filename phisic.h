#pragma once
#include <Box2D/Box2D.h>


static const float SCALE_BOX = 30.0f; //divide by this to get from pixel -> meter
static const float DEG_BOX = 57.29577f; //divide by this to get from degree -> radian

class CollideListener : public b2ContactListener {
public:

	void BeginContact(b2Contact* contact);

	void EndContact(b2Contact* contact);
};