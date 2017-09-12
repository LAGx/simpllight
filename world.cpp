#include "world.h"

#include <json.hpp>
#include <Box2D/BOX2D.H>
#include <string>
#include <fstream>
#include <SFML/System/Vector2.hpp>

#include "log.h"
#include "state.h"
#include "INIReader.h"
#include "INIWriter.h"
#include "service.h"
#include "window.h"
#include "control.h"
#include "game_objects.h"

#define jsonTmp_ptr (*jsonTmp)  // Getting rid of intrusive (), when using ptr with overloaded []
#define jsonTmp2_ptr (*jsonTmp2)  // Don't throw tomatos in me for that

using json = nlohmann::json;
using Folders = spl::Folders;
using string = std::string;

World::World(spl::Window *window)
{
	const b2Vec2 gravity(0.f, 0.f);
	world = new b2World(gravity);

	this->window = window;

	world->SetContactListener(&collideListener);

	control = new spl::ControlBox;
}

void World::loadWorld(const std::string saveName)
{
	const string path = Folders::getGameFolderPath(Folders::GameFolders::tempWorld);
	Folders::createFolder(path);

	if (saveName == "None")
		Folders::copyFolder(Folders::getGameFolderPath(Folders::GameFolders::nativeWorld), path);
	else
		Folders::copyFolder(Folders::getGameFolderPath(Folders::GameFolders::savesWorlds) + '\\' + saveName, path);

	json jsonTmp_ptr = new json;

	std::ifstream mainPlayerFile(path + "\\mainPlayer.json");
	if (!mainPlayerFile.is_open())
		throw Log::Exception("can`t load world file: mainPlayer.json");

	jsonTmp_ptr << mainPlayerFile;
	mainPlayerFile.close();

	loadLocation(jsonTmp_ptr[0]["lastLocation"]);

	delete jsonTmp;
}

void World::loadLocation(const std::string locationName)
{
	const string path = Folders::getGameFolderPath(Folders::GameFolders::tempWorld) + '\\' + locationName;

	INIReader locationSettings(path + "\\locationSettings.ini");

	// Checking for bad location type
	short *locationType = new short;
	*locationType = locationSettings.GetInteger("settings", "type", -2);
	if (*locationType <= -2 || *locationType >= 2)
		throw Log::Exception("unknown location type in " + path + "/locationSettings.ini", true);

	const LocationType type = static_cast<LocationType>(*locationType);
	delete locationType;

	if (type == LocationType::big && currLocType == LocationType::notPlayable) {
		//TODO: Interface -> "big location" loading proccess
		if (parentLocName != "None") {
			closeLocation(World::LocationType::mainLoc, true, false);
			loadPlayer(path, LocationType::mainLoc);
			parentLocName = "None";

			for (auto &i : additionalLocation) {
				for (auto &j : i.second) {
					j.second->unFreezeObject();
				}
			}
			mainLocation = additionalLocation;

			closeLocation(World::LocationType::additionalLoc, false, false);
		}
		else {
			closeLocation(World::LocationType::mainLoc, true); // for future background world in menu
			loadFromFile(path, type);
		}
	}
	else if (type == LocationType::small && currLocType == LocationType::notPlayable) {
		//TODO: Interface -> "big location" loading proccess

		closeLocation(World::LocationType::mainLoc, true); // for future background world in menu
		
		if (parentLocName != "None") {
			parentLocName = locationSettings.Get("settings", "parent", "err");
			if (parentLocName == "err")
				throw Log::Exception("unknown parent name in " + path + "/locationSettings.ini", true);

			loadLocation(parentLocName);
		}

		loadFromFile(path, type);
	}
	else if (type == LocationType::notPlayable && currLocType == LocationType::notPlayable) {
		//TODO: Interface -> ???

		closeLocation(World::LocationType::mainLoc, true); // for future background world in menu
		loadFromFile(path, type);
	}
	else if (type == LocationType::big && currLocType == LocationType::big) {
		//TODO: Interface -> "big location" loading proccess

		saveLocation(currLocName);
		closeLocation(World::LocationType::mainLoc, true);
		loadFromFile(path, type);
	}
	else if (type == LocationType::small && currLocType == LocationType::big) {
		//TODO: Interface -> "small location" loading proccess
		additionalLocation = mainLocation;
		parentLocName = currLocName;
		savePlayer(Folders::getGameFolderPath(Folders::GameFolders::tempWorld) + '\\' + parentLocName);

		for (auto &i : additionalLocation) {
			for (auto &j : i.second) {
				j.second->freezeObject();
			}
		}

		closeLocation(World::LocationType::mainLoc, false);
		loadFromFile(path, type);
	}
	else if (type == LocationType::notPlayable && currLocType == LocationType::big) {
		//TODO: Interface -> ???

		saveLocation(currLocName);
		closeLocation(World::LocationType::mainLoc, true);
		loadFromFile(path, type);
	}
	else if (type == LocationType::small && currLocType == LocationType::small) {
		//TODO: Interface -> "small location" loading proccess

		saveLocation(currLocName);
		closeLocation(World::LocationType::mainLoc, true);
		loadFromFile(path, type);
	}
	else if (type == LocationType::big && currLocType == LocationType::small) {
		//TODO: Interface -> "small location" loading proccess

		saveLocation(currLocName);
		closeLocation(World::LocationType::mainLoc, true);
		loadPlayer(path, LocationType::mainLoc);
		parentLocName = "None";

		for (auto &i : additionalLocation) {
			for (auto &j : i.second) {
				j.second->unFreezeObject();
			}
		}
		mainLocation = additionalLocation;

		closeLocation(World::LocationType::additionalLoc, false, false);
	}
	else if (type == LocationType::notPlayable && currLocType == LocationType::small) {
		//TODO: Interface -> ???

		saveLocation(currLocName);
		closeLocation(World::LocationType::mainLoc, true);
		loadFromFile(path, type);
	}
	else {
		throw Log::Exception("unexpected situation", true);
	}

	currLocType = type;
	currLocName = locationName;
	State::current_state = locationName;
	//TODO: Interface -> stop loading proccess
#ifdef DEV_MODE
	ScreenLog::setValue(2, locationName);
#endif
}

void World::saveWorld(const std::string saveName)
{
	if (currLocName != "None")
		saveLocation(currLocName, LocationType::mainLoc);
	if (parentLocName != "None")
		saveLocation(parentLocName, LocationType::additionalLoc);

	const string path = Folders::getGameFolderPath(Folders::GameFolders::savesWorlds) + '\\' + saveName;
	Folders::createFolder(path);

	Folders::copyFolder(Folders::getGameFolderPath(Folders::GameFolders::tempWorld), path);
}

void World::saveLocation(const std::string locationName, const LocationType type)
{
	// Path to location folder
	const string path = Folders::getGameFolderPath(Folders::GameFolders::tempWorld) + '\\' + locationName;

	// Saving location settings
	INIWriter locSettings;

	if (type == LocationType::mainLoc)
		locSettings["settings"]["type"] = static_cast<int>(currLocType);
	else if (type == LocationType::additionalLoc)
		locSettings["settings"]["type"] = static_cast<int>(LocationType::mainLoc);
	else
		throw Log::Exception("error location type");

	if (player != nullptr) {
		if (player->cursor != nullptr)
			locSettings["settings"]["cursor"] = true;
		else
			locSettings["settings"]["cursor"] = false;
	}
	else {
		locSettings["settings"]["cursor"] = false;
	}

	locSettings.saveToFile(path + "\\locationSettings.ini");

	// Saving main
	json jsonTmp_ptr = new json;
	std::ofstream *file = new std::ofstream;

	size_t jsonI = 0;

	std::map<string, std::map<string, BaseObject*>> tmpLoc;
	if (type == LocationType::mainLoc)
		tmpLoc = mainLocation;
	else if (type == LocationType::additionalLoc)
		tmpLoc = additionalLocation;
	else
		throw Log::Exception("error location type");

	// House
	for (auto &i : tmpLoc["House"]) {
		House *tmp = static_cast<House*>(i.second);

		jsonTmp_ptr[jsonI] =
		{
			{ "name" , i.first },
			{ "type" , "House" },
			{ "initCord" , { tmp->getCoordinates().x, tmp->getCoordinates().y } },
			{ "angle" , tmp->getRotation() },
			{ "textureHouse" , tmp->getTexturePath() },
			{ "textureDoor" , tmp->door->getTexturePath() }
		};
		jsonI++;
	}

	// Fir_tree
	for (auto &i : tmpLoc["Fir_tree"]) {
		Fir_tree *tmp = static_cast<Fir_tree*>(i.second);

		jsonTmp_ptr[jsonI] =
		{
			{ "name" , i.first },
			{ "type" , "Fir_tree" },
			{ "initCord" ,{ tmp->getCoordinates().x, tmp->getCoordinates().y } },
			{ "texture" , tmp->getTexturePath() },
			{ "health" , tmp->getHealth() }
		};
		jsonI++;
	}

	// Shrub
	for (auto &i : tmpLoc["Shrub"]) {
		Shrub *tmp = static_cast<Shrub*>(i.second);

		jsonTmp_ptr[jsonI] =
		{
			{ "name" , i.first },
			{ "type" , "Shrub" },
			{ "initCord" ,{ tmp->getCoordinates().x, tmp->getCoordinates().y } },
			{ "texture" , tmp->getTexturePath() },
			{ "health" , tmp->getHealth() }
		};
		jsonI++;
	}

	// Person
	for (auto &i : tmpLoc["Person"]) {
		Person *tmp = static_cast<Person*>(i.second);

		jsonTmp_ptr[jsonI] =
		{
			{ "name" , i.first },
			{ "type" , "Person" },
			{ "initCord" ,{ tmp->getCoordinates().x, tmp->getCoordinates().y } },
			{ "texture" , tmp->getTexturePath() },
			{ "health" , tmp->getHealth() }
		};
		jsonI++;
	}

	file->open(path + "\\main.json", std::ios::trunc);
	*file << std::setw(4) << jsonTmp_ptr << std::endl;
	file->close();
	jsonTmp->clear();

	savePlayer(path);

	delete file;
	delete jsonTmp;
}

void World::closeWorld()
{
	currLocName = "None";
	parentLocName = "None";

	closeLocation(LocationType::additionalLoc, true);
	closeLocation(LocationType::mainLoc, true);
	Folders::deleteFolder(Folders::getGameFolderPath(Folders::GameFolders::tempWorld));
}

void World::closeLocation(const LocationType type, const bool isDeleteObjects, const bool isDeletePlayer)
{
	if (type == LocationType::additionalLoc || type == LocationType::small) {
		if (isDeleteObjects)
			for (auto &i : additionalLocation)
				for (auto &j : i.second)
					delete j.second;
				
		additionalLocation.clear();
	}
	else if (type == LocationType::mainLoc || type == LocationType::big) {
		if (isDeleteObjects)
			for (auto &i : mainLocation)
				for (auto &j : i.second)
					delete j.second;
				
		mainLocation.clear();
		currLocName = "None";
	}
	else {
		throw Log::Exception("unknown location type");
	}

	if (isDeletePlayer && player != nullptr) {
		if (player->cursor != nullptr)
			control->deleteControlObject(player->cursor);

		control->deleteControlObject(player);
		delete player;
		player = nullptr;
	}
}

void World::blit()
{
	// ------ DEBUG --------
	if (collideListener.isLoad == 1) {
		loadLocation("testLoc");
		collideListener.isLoad = 0;
	}
	else if (collideListener.isLoad == 2) {
		loadLocation("testLoc2");
		collideListener.isLoad = 0;
	}
	else {}


	// House
	for (auto &i : mainLocation["House"]) {
		House *tmp = static_cast<House*>(i.second);

		tmp->blit();
	}

	// Fir_tree
	for (auto &i : mainLocation["Fir_tree"]) {
		Fir_tree *tmp = static_cast<Fir_tree*>(i.second);

		tmp->blit();
	}

	// Shrub
	for (auto &i : mainLocation["Shrub"]) {
		Shrub *tmp = static_cast<Shrub*>(i.second);

		tmp->blit();
	}

	// Person
	for (auto &i : mainLocation["Person"]) {
		Person *tmp = static_cast<Person*>(i.second);

		tmp->blit();
	}

	/////
	// add here new object type
	/////

	// Player
	if (player != nullptr) {
		player->blit();
		window->view.setCenter(sf::Vector2f(player->body_ph->GetPosition().x*SCALE_BOX, player->body_ph->GetPosition().y*SCALE_BOX));
	}

	control->resulveControl(*window);
	world->Step(timeStep, velocityIterations, positionIterations);
}

void World::setStep(const float32 timeStep, const int32 velocityIterations, const int32 positionIterations)
{
	this->timeStep = timeStep;
	this->velocityIterations = velocityIterations;
	this->positionIterations = positionIterations;
}

void World::loadFromFile(const std::string locationFolder, const LocationType type)
{
	json jsonTmp_ptr = new json;

	std::ifstream mainFile(locationFolder + "\\main.json");
	if (!mainFile.is_open())
		throw Log::Exception("can`t load location file: " + locationFolder + "\\main.json");

	jsonTmp_ptr << mainFile;
	mainFile.close();

	for (auto obj : jsonTmp_ptr) {
		if (obj["type"] == "House") {
			mainLocation["House"][obj["name"]] =
				new House(world, sf::Vector2f(obj["initCord"][0], obj["initCord"][1]), obj["angle"], obj["textureHouse"], obj["textureDoor"]);
		}
		else if (obj["type"] == "Fir_tree") {
			mainLocation["Fir_tree"][obj["name"]] =
				new Fir_tree(world, sf::Vector2f(obj["initCord"][0], obj["initCord"][1]), obj["texture"], obj["health"]);
		}
		else if (obj["type"] == "Shrub") {
			mainLocation["Shrub"][obj["name"]] =
				new Shrub(world, sf::Vector2f(obj["initCord"][0], obj["initCord"][1]), obj["texture"], obj["health"]);
		}
		else if (obj["type"] == "Person") {
			mainLocation["Person"][obj["name"]] =
				new Person(world, sf::Vector2f(obj["initCord"][0], obj["initCord"][1]), obj["texture"], obj["health"]);
		}
		// add here new object type
		else {
			throw Log::Exception("wrong object type in " + locationFolder + "/main.json");
		}
	}
	
	// Processing objects that needs control
	loadPlayer(locationFolder, type);

	delete jsonTmp;
}

void World::savePlayer(const std::string path)
{
	json jsonTmp_ptr = new json;
	std::ofstream *file = new std::ofstream;

	if (player != nullptr) {
		size_t jsonI = 0;

		// Cursor
		if (player->cursor != nullptr) {
			jsonTmp_ptr[jsonI] =
			{
				{ "type" , "Cursor" },
				{ "textureCursor" , player->cursor->getTexturePath() }
			};
			jsonI++;
		}

		// Player
		jsonTmp_ptr[jsonI] =
		{
			{ "type" , "Player" },
			{ "initCord" ,{ player->getCoordinates().x, player->getCoordinates().y } },
			{ "texture" , player->getTexturePath() }
		};

		file->open(path + "\\player.json", std::ios::trunc);
		*file << std::setw(4) << jsonTmp_ptr << std::endl;
		file->close();
		jsonTmp->clear();

		jsonTmp_ptr[0] =
		{
			{ "lastLocation" , currLocName },
			{ "health" , player->getHealth() }
		};

		file->open(Folders::getGameFolderPath(Folders::GameFolders::tempWorld) + "\\mainPlayer.json", std::ios::trunc);
		*file << std::setw(4) << jsonTmp_ptr << std::endl;
		file->close();
	}

	delete file;
	delete jsonTmp;
}

void World::loadPlayer(const std::string locationFolder, const LocationType type)
{
	json jsonTmp_ptr = new json;

	if (type != LocationType::notPlayable) {
		std::ifstream playerFile(locationFolder + "\\player.json");

		if (!playerFile.is_open())
			throw Log::Exception("can`t load location file: " + locationFolder + "\\player.json");

		jsonTmp_ptr << playerFile;
		playerFile.close();

		json jsonTmp2_ptr = new json;

		playerFile.open(Folders::getGameFolderPath(Folders::GameFolders::tempWorld) + "\\mainPlayer.json");
		if (!playerFile.is_open())
			throw Log::Exception("can`t load world file: mainPlayer.json");

		jsonTmp2_ptr << playerFile;
		playerFile.close();

		INIReader locationSettings(locationFolder + "\\locationSettings.ini");
		bool *cursor = new bool;
		*cursor = locationSettings.GetBoolean("settings", "cursor", false);

		if (*cursor) {
			player = new Player(world, sf::Vector2f(jsonTmp_ptr[1]["initCord"][0], jsonTmp_ptr[1]["initCord"][1]), jsonTmp2_ptr[0]["health"], jsonTmp_ptr[1]["texture"], jsonTmp_ptr[0]["textureCursor"]);
			control->setControlObject(player);
			control->setControlObject(player->cursor);
		}
		else {
			player = new Player(world, sf::Vector2f(jsonTmp_ptr[0]["initCord"][0], jsonTmp_ptr[0]["initCord"][1]), jsonTmp2_ptr[0]["health"], jsonTmp_ptr[0]["texture"]);
			control->setControlObject(player);
		}

		delete jsonTmp2;
		delete cursor;
	}
	else {
		Log::Exception("uncorrect location type");
	}
	delete jsonTmp;
}

World::~World()
{
	closeWorld();

	delete control;
	delete world;
}