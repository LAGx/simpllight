#include "world.h"

#include <json.hpp>
#include <string>
#include <fstream>

#include "log.h"
#include "INIReader.h"

using json = nlohmann::json;


World::World(spl::Window *window)
{
	gravity.Set(0.f, 0.f);
	world = new b2World(gravity);

	this->window = window;

	world->SetContactListener(&collideListener);

	control = new spl::ControlBox;
}

void World::loadLocation(std::string locationName, bool isLoadPlayer)
{
	INIReader locationSettings(locationName + "/locationSettings.ini");

	// Checking for bad location type
	short *locationType = new short;
	*locationType = locationSettings.GetInteger("settings", "type", -2);
	if (*locationType <= -2 || *locationType >= 2)
		throw Log::Exception("unknown location type in " + locationName + "/locationSettings.ini", true);

	LocationType type = (LocationType)(*locationType);
	delete locationType;

	if (type == LocationType::big && currLocType == LocationType::notPlayable) {
		//TODO: Interface -> "big location" loading proccess

		closeLocation(World::LocationType::mainLoc); // for future background world in menu
		loadFromFile(locationName, isLoadPlayer);
	}
	else if (type == LocationType::small && currLocType == LocationType::notPlayable) {
		//TODO: Interface -> "big location" loading proccess

		closeLocation(World::LocationType::mainLoc); // for future background world in menu
		loadFromFile(locationName, isLoadPlayer);
	}
	else if (type == LocationType::notPlayable && currLocType == LocationType::notPlayable) {
		//TODO: Interface -> ???

		closeLocation(World::LocationType::mainLoc); // for future background world in menu
		loadFromFile(locationName, isLoadPlayer);
	}
	else if (type == LocationType::big && currLocType == LocationType::big) {
		//TODO: Interface -> "big location" loading proccess

		saveLocation(mainLocName);
		closeLocation(World::LocationType::mainLoc);
		loadFromFile(locationName, isLoadPlayer);
	}
	else if (type == LocationType::small && currLocType == LocationType::big) {
		//TODO: Interface -> "small location" loading proccess

		additionalLocation = mainLocation;
		for (auto &i : additionalLocation) {
			for (auto &j : i.second) {
				j.second->freezeObject();
			}
		}

		closeLocation(World::LocationType::mainLoc);
		loadFromFile(locationName, isLoadPlayer);
	}
	else if (type == LocationType::notPlayable && currLocType == LocationType::big) {
		//TODO: Interface -> ???

		saveLocation(mainLocName);
		closeLocation(World::LocationType::mainLoc);
		loadFromFile(locationName, isLoadPlayer);
	}
	else if (type == LocationType::small && currLocType == LocationType::small) {
		//TODO: Interface -> "small location" loading proccess

		saveLocation(mainLocName);
		closeLocation(World::LocationType::mainLoc);
		loadFromFile(locationName, isLoadPlayer);
	}
	else if (type == LocationType::big && currLocType == LocationType::small) {
		//TODO: Interface -> "small location" loading proccess

		saveLocation(mainLocName);
		closeLocation(World::LocationType::mainLoc);

		for (auto &i : additionalLocation) {
			for (auto &j : i.second) {
				j.second->unFreezeObject();
			}
		}
		mainLocation = additionalLocation;

		closeLocation(World::LocationType::additionalLoc);
	}
	else if (type == LocationType::notPlayable && currLocType == LocationType::small) {
		//TODO: Interface -> ???

		saveLocation(mainLocName);
		closeLocation(World::LocationType::mainLoc);
		loadFromFile(locationName, isLoadPlayer);
	}
	else {
		throw Log::Exception("unexpected situation", true);
	}

	currLocType = type;
	mainLocName = locationName;
	//TODO: Interface -> stop loading proccess
}

void World::saveLocation(std::string locationName)
{
	json *jsonTmp = new json;
	std::ofstream *file = new std::ofstream;

	size_t jsonI = 0;

	// House
	for (auto &i : mainLocation["House"]) {
		House *tmp = static_cast<House*>(i.second);

		(*jsonTmp)[jsonI] =
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
	for (auto &i : mainLocation["Fir_tree"]) {
		Fir_tree *tmp = static_cast<Fir_tree*>(i.second);

		(*jsonTmp)[jsonI] =
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
	for (auto &i : mainLocation["Shrub"]) {
		Shrub *tmp = static_cast<Shrub*>(i.second);

		(*jsonTmp)[jsonI] =
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
	for (auto &i : mainLocation["Person"]) {
		Person *tmp = static_cast<Person*>(i.second);

		(*jsonTmp)[jsonI] =
		{
			{ "name" , i.first },
			{ "type" , "Person" },
			{ "initCord" ,{ tmp->getCoordinates().x, tmp->getCoordinates().y } },
			{ "texture" , tmp->getTexturePath() },
			{ "health" , tmp->getHealth() }
		};
		jsonI++;
	}

	file->open(locationName + "/main.json", std::ios::trunc);
	*file << std::setw(4) << *jsonTmp << std::endl;
	file->close();
	jsonTmp->clear();

	// Player
	if (player != nullptr) {
		jsonI = 0;

		if (player->cursor != nullptr) {
			(*jsonTmp)[jsonI] =
			{
				{ "type" , "Cursor" },
				{ "textureCursor" , player->cursor->getTexturePath() },
			};
			jsonI++;
		}

		(*jsonTmp)[jsonI] =
		{
			{ "type" , "Player" },
			{ "initCord" , { player->getCoordinates().x, player->getCoordinates().y } },
			{ "texture" , player->getTexturePath() },
			{ "health" , player->getHealth() }
		};

		file->open(locationName + "/player.json", std::ios::trunc);
		*file << std::setw(4) << *jsonTmp << std::endl;
		file->close();
	}

	delete file;
	delete jsonTmp;
}

void World::closeLocation(LocationType type)
{
	if (type == LocationType::additionalLoc || type == LocationType::small) {
		additionalLocation.clear();
	}
	else if (type == LocationType::mainLoc || type == LocationType::big) {
		mainLocation.clear();
		mainLocName.clear();
	}
	else {
		throw Log::Exception("unknown location type");
	}
}

void World::blit()
{
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

	// add here new object type

	// Player
	if (player != nullptr) {
		player->blit();
		window->view.setCenter(sf::Vector2f(player->body_ph->GetPosition().x*SCALE_BOX, player->body_ph->GetPosition().y*SCALE_BOX));
	}

	control->resulveControl(*window);
	world->Step(timeStep, velocityIterations, positionIterations);
}

void World::setStep(float32 timeStep, int32 velocityIterations, int32 positionIterations)
{
	this->timeStep = timeStep;
	this->velocityIterations = velocityIterations;
	this->positionIterations = positionIterations;
}

void World::loadFromFile(std::string locationFolder, bool isLoadPlayer)
{
	json *jsonTmp = new json;

	std::ifstream mainFile(locationFolder + "/main.json");
	if (!mainFile.is_open())
		throw Log::Exception("can`t load location file: " + locationFolder + "/main.json");

	*jsonTmp << mainFile;
	mainFile.close();

	for (auto obj : *jsonTmp) {
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
			throw Log::Exception("wrong type in " + locationFolder + "/main.json");
		}
	}

	// Processing objects that needs control
	if (isLoadPlayer) {
		std::ifstream locationPlayerFile(locationFolder + "/player.json");
		if (!locationPlayerFile.is_open())
			throw Log::Exception("can`t load location file: " + locationFolder + "/player.json");

		*jsonTmp << locationPlayerFile;
		locationPlayerFile.close();

		std::ifstream mainPlayerFile(locationFolder + "/player.json");
		if (!mainPlayerFile.is_open())
			throw Log::Exception("can`t load location file: " + locationFolder + "/player.json");

		*jsonTmp << mainPlayerFile;
		mainPlayerFile.close();

		INIReader locationSettings(locationFolder + "/locationSettings.ini");
		bool *cursor = new bool;
		*cursor = locationSettings.GetBoolean("settings", "cursor", false);

		if (*cursor) {
			player = new Player(world, sf::Vector2f((*jsonTmp)[1]["initCord"][0], (*jsonTmp)[1]["initCord"][1]), (*jsonTmp)[1]["health"], (*jsonTmp)[1]["texture"], (*jsonTmp)[0]["textureCursor"]);
			control->setControlObject(player);
			control->setControlObject(player->cursor);
		}
		else {
			player = new Player(world, sf::Vector2f((*jsonTmp)[0]["initCord"][0], (*jsonTmp)[0]["initCord"][1]), (*jsonTmp)[1]["health"], (*jsonTmp)[0]["texture"]);
			control->setControlObject(player);
		}

		delete cursor;
	}

	delete jsonTmp;
}

World::~World()
{
	closeLocation(LocationType::additionalLoc);
	closeLocation(LocationType::mainLoc);

	if (player != nullptr) {
		control->deleteControlObject(player);
		control->deleteControlObject(player->cursor);

		delete player;
	}

	delete control;
	delete world;
}