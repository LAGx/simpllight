#ifndef _WORLD_
#define _WORLD_

#pragma once
#include <Box2D/Box2D.h>
#include <string>
#include <map>

#include "game_objects.h"
#include "phisic.h"
#include "window.h"
#include "control.h"

class World {
public:

	/// enum class that contain types of location
	enum class LocationType
	{
		// for others situation, e.g. menu background or cutscene
		notPlayable = -1,

		big, mainLoc = 0,
		small, additionalLoc = 1
	};
	
	/*!
	@brief constructor that set gravity, window and contact listener,
	also it create b2World and control

	@param [in] window  Window where world will blit
	*/
	World(spl::Window *window);
	
	/*!
	@brief create work folder, load world with specific 
	save name or by default from native world and load last location

	@param [in] saveName  name of save which will load into the world

	@throw Log::Exception if can`t load world file mainPlayer.json
	*/
	void loadWorld(const std::string saveName = "None");
	
	/*!
	@brief loads location with specific location name

	@param [in] locationName  name of location which should be load

	@throw Log::Exception if location type will be unknown in 
	locationPath/locationSettings.ini
	*/
	void loadLocation(const std::string locationName);

	/*!
	@brief save world to specific save name

	@param [in] saveName  name of save where world will be saved
	*/
	void saveWorld(const std::string saveName);

	/*!
	@brief save location with specific location name and type

	@param [in] locationName  name of location which should be saved
	@param [in] type  type of location which should be saved (mailLoc or addtionalLoc

	@throw Log::Exception if location type will be different from mainLoc or addtionalLoc
	*/
	void saveLocation(const std::string locationName, const LocationType type = LocationType::mainLoc);

	/// close all locations and delete world folder
	void closeWorld();

	/*!
	@brief close location with specific location type and delete player

	@param [in] type  type of container which should be closed (mailLoc or addtionalLoc)
	@param [in] isDeleteObjects  should this function delete objects in container or not
	@param [in] isDeletePlayer  should this function delete player or not

	@throw Log::Exception if location type will be different from mainLoc or addtionalLoc
	*/
	void closeLocation(const LocationType type, const bool isDeleteObjects, const bool isDeletePlayer = true);

	/*!
	@brief set step parameters for world.Step(...) function

	@param timeStep  the amount of time to simulate, this should not vary.
	@param velocityIterations  for the velocity constraint solver.
	@param positionIterations  for the position constraint solver.
	*/
	void setStep(const float32 timeStep, const int32 velocityIterations, const int32 positionIterations);

	/// blit world to windows
	void blit();

	/// destructor that closing all location, delete player and b2World
	~World();

private:
	/*!
	@brief load objects to specific location container from specific location folder

	@param [in] locationFolder  path to folder from where objects will load
	@param [in] type  type of container where objects will loads (mailLoc or addtionalLoc)

	@throw Log::Exception if can`t load location file locationFolder\main.json
	@throw Log::Exception if object type is wrong in locationFolder\main.json
	@throw Log::Exception if can`t load location file locationFolder\player.json
	@throw Log::Exception if can`t load world file mainPlayer.json
	*/
	void loadFromFile(const std::string locationFolder, const LocationType type);

	/*!
	@brief save player to specific location

	@param [in] path  path to location where player should save
	*/
	void savePlayer(const std::string path);

	/*!
	@brief load player from specific location data to player ptr

	@param [in] locationFolder  location from where player should load
	@param [in] type  type of location from where player should load
	*/
	void loadPlayer(const std::string locationFolder, const LocationType type);

	spl::Window *window;

	b2World *world;

	/// World step param
	float32 timeStep = 1 / 60.f;
	int32 velocityIterations = 8;
	int32 positionIterations = 3;

	CollideListener collideListener;

	spl::ControlBox *control;
	Player *player = nullptr;

	LocationType currLocType = LocationType::notPlayable;

	std::string currLocName = "None";
	std::string parentLocName = "None";

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