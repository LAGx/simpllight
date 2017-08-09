#ifndef _WORLD_
#define _WORLD_

#pragma once
#include <Box2D/Box2D.h>
#include <string>
#include <map>

#include "game_objects.h"
#include "phisic.h"

class World {
public:

	enum class LocationType
	{
		// for others situation, e.g. menu background or cutscene
		notPlayable = -1,

		big, mainLoc = 0,
		small, additionalLoc = 1
	};

	World(spl::Window *window);

	void loadLocation(std::string locationName, bool isLoadPlayer = "true");

	void saveLocation(std::string locationName);

	void closeLocation(LocationType type);

	void setStep(float32 timeStep, int32 velocityIterations, int32 positionIterations);

	void blit();

	~World();

private:
	void loadFromFile(std::string locationFolder, bool isLoadPlayer);

	spl::Window *window;

	b2Vec2 gravity;
	b2World *world;

	// World step param
	float32 timeStep = 1 / 60.f;
	int32 velocityIterations = 8;
	int32 positionIterations = 3;

	CollideListener collideListener;

	spl::ControlBox *control;
	Player *player = nullptr;

	LocationType currLocType = LocationType::notPlayable;

	std::string mainLocName;

	/*!
	@brief container for all main-location objects

	@using <"objects type", map<"object name", obj*>>
	*/
	std::map<std::string, std::map<std::string, BaseObject*>> mainLocation;

	/*! 
	@brief container for all additional-location objects

	@using <"objects type", map<"object name", obj*>>
	*/
	std::map<std::string, std::map<std::string, BaseObject*>> additionalLocation;
};
#endif