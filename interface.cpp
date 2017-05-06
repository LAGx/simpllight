#include "interface.h"
#include "window.h"
#include "log.h"
#include "service.h"
#include <cstdio>  
#include <string>
#include <iostream>
#include <windows.h>
#include <conio.h>
#include "json.hpp"
using json = nlohmann::json;
#pragma warning(disable : 4996)


///////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////     CELL INTERFACE     /////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////

CellInterface::CellInterface(sf::Vector2f initRatio, sf::Vector2f sizeRatio, StyleCell& style, typeCell type, std::string id) {
	this->type = type;
	this->id = id;
	this->speedChangeTransper = style.speedChangeTransper;
	deltaTransp = style.deltaTransperActive - style.deltaTransperQuiet;
	this->positionRelWindow = sf::Vector2f(spl::WindowStateBox::currScreenSize.x * initRatio.x/100, spl::WindowStateBox::currScreenSize.y * initRatio.y / 100);
	sizeCell = sf::Vector2f(spl::WindowStateBox::currScreenSize.x * sizeRatio.x / 100, spl::WindowStateBox::currScreenSize.y * sizeRatio.y / 100);

	{
		TextureGenerator textureGenerator;
		if ((fopen(("image/tempInterface/" + id + "_base.png").c_str(), "r") == NULL) &&
			(fopen(("image/tempInterface/" + id + "_frame.png").c_str(), "r") == NULL)) {
			switch (this->type) {
			case typeCell::rect:
				textureGenerator.fillShadowRect(style.shadow, sf::Vector2i(sizeCell), style.baseColor - sf::Color(0, 0, 0, style.deltaTransperActive), "image/tempInterface/" + id + "_base.png");
				textureGenerator.conturRect(sf::Vector2i(sizeCell), style.borderSize, style.frameColor, "image/tempInterface/" + id + "_frame.png");
				break;
			case typeCell::round:
				textureGenerator.fillShadowCircle(style.shadow, int(sizeCell.x), style.baseColor - sf::Color(0, 0, 0, style.deltaTransperActive), "image/tempInterface/" + id + "_base.png");
				textureGenerator.conturCircle(sizeCell.x, style.borderSize, style.frameColor, "image/tempInterface/" + id + "_frame.png");
				break;
			default:
				throw Log::Exception("Error type in CellInterface");
			}
		}
	}

	this->baseTextr  = new BaseObject(sf::Vector2f(0,0), "image/tempInterface/" + id + "_base.png");
	this->frameTextr = new BaseObject(sf::Vector2f(0,0), "image/tempInterface/" + id + "_frame.png");

	baseTextr->depthRender = -500;
	frameTextr->depthRender = -500 - 1;
}

void CellInterface::changeDepthRender(int depth) {
	baseTextr->depthRender += depth;
	frameTextr->depthRender += depth;
}

void CellInterface::blit() {
	if (isVisible) {
		update();
		baseTextr->blit();
		frameTextr->blit();
	}
	else {
		isActive = false;
	}
}

void CellInterface::phisicCollideDetecting() {
	if (type == typeCell::rect) {
		if (((positionRelWindow.x - sizeCell.x / 2 <= spl::WindowStateBox::mouseCurrPositionRelativeWindow.x) && (positionRelWindow.x+ sizeCell.x/2  >= spl::WindowStateBox::mouseCurrPositionRelativeWindow.x)) &&
			((positionRelWindow.y - sizeCell.y / 2 <= spl::WindowStateBox::mouseCurrPositionRelativeWindow.y) && (positionRelWindow.y + sizeCell.y / 2 >= spl::WindowStateBox::mouseCurrPositionRelativeWindow.y)))
			isActive = true;
		else
			isActive = false;
	}
	else if (type == typeCell::round) {
		if (sqrt((positionRelWindow.x - spl::WindowStateBox::mouseCurrPositionRelativeWindow.x)*(positionRelWindow.x - spl::WindowStateBox::mouseCurrPositionRelativeWindow.x) + (positionRelWindow.y - spl::WindowStateBox::mouseCurrPositionRelativeWindow.y)*(positionRelWindow.y - spl::WindowStateBox::mouseCurrPositionRelativeWindow.y)) <= sizeCell.x)
			isActive = true;
		else
			isActive =  false;
	}
}

void CellInterface::animation() {
	if (isActive) {
		if (animationCoef + speedChangeTransper <= 1)
			animationCoef += speedChangeTransper;
		else
			animationCoef = 1;

		baseTextr->g_body.setColor(sf::Color(255, 255, 255, deltaTransp+(255-deltaTransp)*animationCoef));
	}
	else {
		if (animationCoef - speedChangeTransper >= 0)
			animationCoef -= speedChangeTransper;
		else
			animationCoef = 0;
		
		baseTextr->g_body.setColor(sf::Color(255, 255, 255, deltaTransp + (255 - deltaTransp)*animationCoef));
	}
}

void CellInterface::setVisible(bool isVisible) {
	this->isVisible = isVisible;
}

void CellInterface::update() {
		phisicCollideDetecting();
		spl::Window::updateWindowStateBox();//don`t need there //too much calls 
		baseTextr->updateTextrPosition(spl::WindowStateBox::inGameZeroCordRelativeWindow + positionRelWindow / spl::WindowStateBox::absoluteScale, 0);
		baseTextr->g_body.setScale(sf::Vector2f(1 / spl::WindowStateBox::absoluteScale, 1 / spl::WindowStateBox::absoluteScale));
		frameTextr->updateTextrPosition(spl::WindowStateBox::inGameZeroCordRelativeWindow + positionRelWindow / spl::WindowStateBox::absoluteScale, 0);
		frameTextr->g_body.setScale(sf::Vector2f(1 / spl::WindowStateBox::absoluteScale, 1 / spl::WindowStateBox::absoluteScale));
		animation();
}

CellInterface::~CellInterface() {
	if (baseTextr != nullptr)
		delete baseTextr;
	if (frameTextr != nullptr)
		delete frameTextr;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////// ASSEMBLY LAYER INTERFACE ///////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////

AssemblyLayerInterface::AssemblyLayerInterface(string id, string mode, string styleId) {
	if (mode == "new") {
		ofstream file("game_data/interface/" + id + ".json", ios::trunc);

		json j;
		j["id"] = id;
		this->id = id;
		j["styleId"] = styleId;
		this->styleId = styleId;

		file << j;
		file.close();
	}
	else if (mode == "old") {
		this->id = id;
		updateAllCellFromFile();
	}
	else
		throw Log::Exception("invalid AssemblyLayerInterface mode", true);
}

void AssemblyLayerInterface::updateAllCellFromFile() {
	for (int i = 0; i < allCell.size(); i++)
		delete allCell[i];

	allCell.clear();
	ifstream file("game_data/interface/" + id + ".json");
	if (!file.is_open())
		throw Log::Exception("no such id file", true);

	json j;
	j << file;
	string t = j["styleId"];
	this->styleId = t;

	for (auto element : j) {

		try {
			if (element["typeObject"] == "CellInterface") {
				CellInterface* toSaveCell = new CellInterface(sf::Vector2f(element["initRatio"][0], element["initRatio"][1]),
					sf::Vector2f(element["sizeRatio"][0], element["sizeRatio"][1]),
					getStyle(element["style"]),
					element["type"],
					element["id"]);
				allCell.push_back(toSaveCell);
			}
		}catch (domain_error) {}
	}
	file.close();
}

void AssemblyLayerInterface::createNewCell(sf::Vector2f initRatio, sf::Vector2f sizeRatio, CellInterface::typeCell type, std::string id, string styleId) {
	json jStyle;

	ifstream inStreamFile("game_data/interface/" + this->id + ".json");
	if (!inStreamFile.is_open())
		throw Log::Exception("game_data/interface/" + this->id + ".json", true);


	jStyle << inStreamFile;
	jStyle[id]["initRatio"] = { initRatio.x, initRatio.y};
	jStyle[id]["sizeRatio"] = { sizeRatio.x, sizeRatio.y};
	jStyle[id]["type"] = int(type);
	if(styleId == "default")
		jStyle[id]["style"] = this->styleId;
	else
		jStyle[id]["style"] = styleId;
	jStyle[id]["typeObject"] = "CellInterface";
	jStyle[id]["id"] = id;

	ofstream outStreamFile("game_data/interface/" + this->id + ".json");
	if (!outStreamFile.is_open())
		throw Log::Exception("game_data/interface/" + this->id + ".json", true);

	outStreamFile << jStyle;
	outStreamFile.close();
	inStreamFile.close();

	updateAllCellFromFile();
}


CellInterface::StyleCell AssemblyLayerInterface::getStyle(string styleId) {
	ifstream file("game_data/interface/styles.json");
	if (!file.is_open())
		throw Log::Exception("no such styles file", true);

	json j;
	j << file;
	if (j.find(styleId) != j.end()) {

		CellInterface::StyleCell style = {
			sf::Color(j[styleId]["baseColor"][0],j[styleId]["baseColor"][1], j[styleId]["baseColor"][2], j[styleId]["baseColor"][3]) ,
			sf::Color(j[styleId]["frameColor"][0],j[styleId]["frameColor"][1], j[styleId]["frameColor"][2], j[styleId]["frameColor"][3]),
			j[styleId]["borderSize"],
			j[styleId]["shadow"],
			j[styleId]["deltaTransperActive"],
			j[styleId]["deltaTransperQuiet"],
			j[styleId]["speedChangeTransper"]
		};

		return style;
	}
	else
		throw Log::Exception("no such style id");
}

string AssemblyLayerInterface::getActiveCell() {
	for (int i = 0; i < allCell.size(); i++)
		if (allCell[i]->isActive)
			return allCell[i]->id;
	return "None";
}

void AssemblyLayerInterface::deleteCell(std::string id) {
	ifstream file("game_data/interface/" + this->id + ".json");
	if (!file.is_open())
		throw Log::Exception("no such id file", true);

	json j;
	j << file;
	j.erase("cell_1");

	file.close();
	ofstream outStreamFile("game_data/interface/" + this->id + ".json");
	if (!outStreamFile.is_open())
		throw Log::Exception("game_data/interface/" + this->id + ".json", true);

	outStreamFile << j;
	outStreamFile.close();

	updateAllCellFromFile();
}

void AssemblyLayerInterface::blit() {
	for (int i = 0; i < allCell.size(); i++)
		allCell[i]->blit();
}

AssemblyLayerInterface::~AssemblyLayerInterface() {
	for (int i = 0; i < allCell.size(); i++)
		delete allCell[i];	
}


///////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////    USER INTERFACE BOX    ///////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////

string UserInterfaceBox::activeLayer;
string UserInterfaceBox::activeCell;

UserInterfaceBox::UserInterfaceBox() {
	DeleteDirectory("image\\tempInterface", false);
	if (!CreateDirectory("image\\tempInterface", NULL))
		Log::warning("no directory tempInterface 2", true);

	CellInterface::StyleCell style = { sf::Color(10,60,50), sf::Color(0,255,255), 2, 2, 50, 200, 0.05};
	createStyle("default",style);
}


void UserInterfaceBox::createStyle(string id, CellInterface::StyleCell& style) {
	json jStyle;

	ifstream inStreamFile("game_data/interface/styles.json");
	if (!inStreamFile.is_open())
		throw Log::Exception("Can`t open game_data/interface/styles.json", true);

	jStyle << inStreamFile;
	jStyle[id]["baseColor"] = { style.baseColor.r,style.baseColor.g, style.baseColor.b,style.baseColor.a };
	jStyle[id]["frameColor"] = { style.frameColor.r,style.frameColor.g, style.frameColor.b,style.frameColor.a };
	jStyle[id]["borderSize"] = style.borderSize;
	jStyle[id]["shadow"] = style.shadow;
	jStyle[id]["deltaTransperActive"] = style.deltaTransperActive;
	jStyle[id]["deltaTransperQuiet"] = style.deltaTransperQuiet;
	jStyle[id]["speedChangeTransper"] = style.speedChangeTransper;

	ofstream outStreamFile("game_data/interface/styles.json");
	if (!outStreamFile.is_open())
		throw Log::Exception("Can`t open game_data/interface/styles.json", true);

	outStreamFile << jStyle;
	outStreamFile.close();
}

void UserInterfaceBox::createLayer(string id, string styleId) {
	if (currInterfaceLayer != nullptr)
		delete currInterfaceLayer;
	currInterfaceLayer = new AssemblyLayerInterface(id, "new", styleId);
	activeLayer = id;
}

void UserInterfaceBox::downloadLayerById(string id) {
	if (currInterfaceLayer != nullptr)
		delete currInterfaceLayer;

	currInterfaceLayer = new AssemblyLayerInterface(id, "old");
	activeLayer = id;
}

void UserInterfaceBox::blit() {
	if (currInterfaceLayer != nullptr) {
		currInterfaceLayer->blit();
		activeCell = currInterfaceLayer->getActiveCell();
	}	
}

AssemblyLayerInterface* UserInterfaceBox::getCurrLayer() {
	return currInterfaceLayer;
}

UserInterfaceBox::~UserInterfaceBox() {
	if (currInterfaceLayer != nullptr)
		delete currInterfaceLayer;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////    INTERFACE CREATOR    ////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////




///////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////    INTERFACE CREATOR    ////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////