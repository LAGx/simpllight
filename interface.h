#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include "game_objects.h"
#include "service.h"

class BaseObject;

class CellInterface{
private:
	void update();

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
public:

	struct StyleCell {
		sf::Color baseColor = {0,0,0,255};
		sf::Color frameColor = {0,0,0};

		int borderSize = 1;
		int shadow = 0;

		char deltaTransperActive = 0;//how more transperent base, then 100%
		char deltaTransperQuiet = 0;
		float speedChangeTransper = 0;
	};

	enum typeCell {
		rect = 0,
		round
	};

	CellInterface(sf::Vector2f initRatio, sf::Vector2f sizeRatio, StyleCell& style, typeCell type, std::string id);

	std::string id = "None";

	bool isActive = false;

	void setDepthRender(int depth);

	void blit();

	~CellInterface();
};


class UserInterfaceBox {
protected:
	CellInterface::StyleCell *style;

	std::vector<CellInterface*> allCell;

public:

	void createNewCell();
	void createCostumNewCell();
	void deleteCell();
	
};