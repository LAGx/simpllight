#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include "game_objects.h"
#include "service.h"
#include "control.h"

using namespace std;

class BaseObject;

class CellInterface{//TODO text and sprite
private:
	void update();
	float animationCoef = 0;
	void animation();


	void phisicCollideDetecting();

	BaseObject *baseTextr;
	BaseObject *frameTextr;

	enum typeCell;
	typeCell type;

	sf::Vector2f sizeCell;           //in pixel relative window//if it`s circle, first parameter is radius 
	sf::Vector2f positionRelWindow; //in pixel //no update
	float speedChangeTransper = 1;  //0 - newer change//1 - in moment change
	char deltaTransp = 0;          //in pixel//how more transperent it need to be in Quiet

	int depthRender = 0;

	bool isVisible = true;
public:

	struct StyleCell {
		sf::Color baseColor = {0,0,0,255};
		sf::Color frameColor = {0,0,0};

		int borderSize = 1;
		int shadow = 0;

		unsigned char deltaTransperActive = 0;//how more transperent base, then 100%
		unsigned char deltaTransperQuiet = 0;
		float speedChangeTransper = 0;
	};

	enum typeCell {
		rect = 0,
		round
	};

	CellInterface(sf::Vector2f initRatio, sf::Vector2f sizeRatio, StyleCell& style, typeCell type, std::string id);

	std::string id = "None";

	bool isActive = false;

	void changeDepthRender(int depth);

	void blit();

	void setVisible(bool isVisible);

	~CellInterface();
};


class AssemblyLayerInterface {

	CellInterface::StyleCell getStyle(string styleId);
	void updateAllCellFromFile();

	std::vector<CellInterface*> allCell;
	string styleId = "None";//can be changed only once in constructor

public:

	AssemblyLayerInterface(string id, string mode, string styleId = "default"); //mod "new", "old"(can`t change style)

	void createNewCell(sf::Vector2f initRatio, sf::Vector2f sizeRatio, CellInterface::typeCell type, std::string id, string styleId = "default");//for creating//can be using for update
	void deleteCell(std::string id); //for creating

	string id = "None";

	string getActiveCell();

	void blit();

	~AssemblyLayerInterface();
};


class UserInterfaceBox {
protected:

	AssemblyLayerInterface* currInterfaceLayer = nullptr;
	
public:

	UserInterfaceBox();

	static string activeLayer;

	static string activeCell;

	void createLayer(string id, string styleId = "default"); //for creating

	void createStyle(string id, CellInterface::StyleCell& style); //for creating

	void downloadLayerById(string id);

	AssemblyLayerInterface* getCurrLayer();

	void blit();

	~UserInterfaceBox();
};

class InterfaceCreator:protected spl::EventInterface {
public:
	
};


class IconAnimated {
	sf::Texture texture;
	sf::Sprite sprite;
public:
	float speed = 1;
	bool isPlay = 1;

	//IconAnimated(sf::Vector2f initRatio, string file);

	void blit();
};