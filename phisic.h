#pragma once
#include <Box2D/Box2D.h>

static b2Vec2 Gravity(0.f, 0.f);//DEBAG (2)
static b2World World(Gravity);

static const float SCALE_BOX = 30.0f; //divide by this to get from pixel -> meter
static const float DEG_BOX = 57.29577f; //divide by this to get from degree -> radian