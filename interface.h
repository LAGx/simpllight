
#ifndef _INTERFACE_
#define _INTERFACE_

#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include "item.h"

class BaseClass;

class CellInterface {
private:
	class Text {

	private:
		int id;
		sf::Text text;

		sf::Vector2f posRatio;
		int depthRender = 0;

	public:
		Text(int id, std::string text, sf::Vector2f posRatio, float scaleRatio, sf::Color textColor, sf::Font &font, CellInterface& cell);

		void setPosition(sf::Vector2f position);
		void setScale(float scale);

		void setNewText(std::string text);
		void setColor(sf::Color color);
		void setSize(float size);

		int getId();
		void changeDepthRender(int delta);

		void blit();

	};

	class BaseObject {//gvnocod becouse of circular include
	public:
		sf::Texture texture;
		sf::Sprite g_body;

		void updateTextrPosition(sf::Vector2f newCord, float newAngle = 0);
		bool isVisible = true;

		int depthRender = 0;

		BaseObject(sf::Vector2f initCord, std::string texture);

		BaseObject();

		void blit();

	};

	sf::Color textColor;
	sf::Font font;
	vector<Text* > allText;
	
	void update();
	float animationCoef = 0;
	void animation();

	InterfaceItem* item = nullptr; //have to be created outside

	void phisicCollideDetecting();

	CellInterface::BaseObject *baseTextr;
	CellInterface::BaseObject *frameTextr;

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
		sf::Color baseColor = { 0,0,0,255 };
		sf::Color frameColor = { 0,0,0 };

		int borderSize = 1;
		int shadow = 0;

		unsigned char deltaTransperActive = 0;//how more transperent base, then 100%
		unsigned char deltaTransperQuiet = 0;
		float speedChangeTransper = 0;

		std::string textFont = "None";
		sf::Color textColor = {255,255,255};
	};

	enum typeCell {
		rect = 0,
		round
	};

//	position and scale reletive cell (in %)
	void textControl(std::string mod, int id, std::string text = "None", sf::Vector2f posRatio = { 0,0 }, float scaleRatio = 1);//mod "new"-new text, "del"-delete text
	Text* getTextPtr(int id);

	void setItem(InterfaceItem* item);
	void removeItem();

	CellInterface(sf::Vector2f initRatio, sf::Vector2f sizeRatio, StyleCell& style, typeCell type, std::string id);

	std::string id = "None";

	bool isActive = false;

	void changeDepthRender(int depth);

	void blit();

	void setVisible(bool isVisible);

	~CellInterface();
};


class AssemblyLayerInterface {

	CellInterface::StyleCell getStyle(std::string styleId);
	void updateAllCellFromFile();

	std::vector<CellInterface*> allCell;
	std::string styleId = "None";//can be changed only once in constructor

public:

	AssemblyLayerInterface(std::string id, std::string mode, std::string styleId = "default"); //mod "new", "old"(can`t change style)

	CellInterface* getCellById(std::string id = "None");
	void createNewCell(sf::Vector2f initRatio, sf::Vector2f sizeRatio, CellInterface::typeCell type, std::string id, std::string styleId = "default");//for creating//can be using for update 
	void deleteCell(std::string id); //for creating
	void textControl(std::string cellId, std::string mod, int idText, std::string text = "None", sf::Vector2f posRatio = { 0,0 }, float scaleRatio = 1);


	std::string id = "None";

	std::string getActiveCell();

	void blit();

	~AssemblyLayerInterface();
};


class UserInterfaceBox {
protected:
	std::vector<AssemblyLayerInterface*> fastAccessLayer;

	AssemblyLayerInterface* currInterfaceLayer = nullptr;
	
public:

	UserInterfaceBox();

	static std::string activeLayer;

	static std::string activeCell;

	void createLayer(std::string id, std::string styleId = "default"); //for creating

	void createStyle(std::string id, CellInterface::StyleCell& style); //for creating

	void downloadLayerById(std::string id);

	AssemblyLayerInterface* getCurrLayer();

	void blit();

	~UserInterfaceBox();
};
#endif