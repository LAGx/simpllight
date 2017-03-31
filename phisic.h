#ifndef _PHISIC_
#define _PHISIC_

#pragma once
#include <Box2D/Box2D.h>

static const float SCALE_BOX = 30.0f; //divide by this to get from pixel -> meter
static const float DEG_BOX = 57.29577f; //divide by this to get from degree -> radian
static const float PI = 3.141592;

class CollideListener : public b2ContactListener {
public:

	void BeginContact(b2Contact* contact);

	void EndContact(b2Contact* contact);
};
#endif