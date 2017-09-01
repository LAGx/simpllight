#ifndef _PHISIC_
#define _PHISIC_

#pragma once
#include <Box2D/Box2D.h>

#define SCALE_BOX 30.0F //divide by this to get from pixel -> meter
#define DEG_BOX 57.29577F //divide by this to get from degree -> radian
#define PI 3.141592F

class CollideListener : public b2ContactListener {
public:

	int isLoad = 0;

	void BeginContact(b2Contact* contact);

	void EndContact(b2Contact* contact);
};
#endif