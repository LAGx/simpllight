#include "interface.h"

#include <cstdio>  
#include <string>
#include <iostream>
#include <fstream>
#include <windows.h>
#include <vector>
#include <conio.h>
#include <json.hpp>

#include "service.h"
#include "window.h"
#include "log.h"
#include "service.h"

#pragma warning(disable : 4996)

using json = nlohmann::json;
using namespace std;


///////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////     CELL INTERFACE     /////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////

CellInterface::CellInterface(sf::Vector2f initRatio, sf::Vector2f sizeRatio, StyleCell& style, typeCell type, std::string id) {
	this->type = type;
	this->id = id;
	this->speedChangeTransper = style.speedChangeTransper;
	deltaTransp = style.deltaTransperActive - style.deltaTransperQuiet;
	this->positionRelWindow = sf::Vector2f(spl::WindowStateBox::currScreenSize.x * initRatio.x / 100, spl::WindowStateBox::currScreenSize.y * initRatio.y / 100);
	sizeCell = sf::Vector2f(spl::WindowStateBox::currScreenSize.x * sizeRatio.x / 100, spl::WindowStateBox::currScreenSize.y * sizeRatio.y / 100);

	{
		spl::TextureGenerator textureGenerator;
		if ((fopen((spl::Folders::getGameFolderPath(spl::Folders::GameFolders::tempTextures) + "\\interface\\" + id + "_base.png").c_str(), "r") == NULL) &&
			(fopen((spl::Folders::getGameFolderPath(spl::Folders::GameFolders::tempTextures) + "\\interface\\" + id + "_frame.png").c_str(), "r") == NULL)) {
			switch (this->type) {
			case typeCell::rect:
				textureGenerator.fillShadowRect(style.shadow, sf::Vector2i(sizeCell), style.baseColor - sf::Color(0, 0, 0, style.deltaTransperActive), spl::Folders::getGameFolderPath(spl::Folders::GameFolders::tempTextures) + "\\interface\\" + id + "_base.png");
				textureGenerator.conturRect(sf::Vector2i(sizeCell), style.borderSize, style.frameColor, spl::Folders::getGameFolderPath(spl::Folders::GameFolders::tempTextures) + "\\interface\\" + id + "_frame.png");
				break;
			case typeCell::round:
				textureGenerator.fillShadowCircle(style.shadow, int(sizeCell.x), style.baseColor - sf::Color(0, 0, 0, style.deltaTransperActive), spl::Folders::getGameFolderPath(spl::Folders::GameFolders::tempTextures) + "\\interface\\" + id + "_base.png");
				textureGenerator.conturCircle(sizeCell.x, style.borderSize, style.frameColor, spl::Folders::getGameFolderPath(spl::Folders::GameFolders::tempTextures) + "\\interface\\" + id + "_frame.png");
				break;
			default:
				throw Log::Exception("Error type in CellInterface");
			}
		}
	}

	this->baseTextr = new CellTexture(sf::Vector2f(0, 0), spl::Folders::getGameFolderPath(spl::Folders::GameFolders::tempTextures) + "\\interface\\" + id + "_base.png");
	this->frameTextr = new CellTexture(sf::Vector2f(0, 0), spl::Folders::getGameFolderPath(spl::Folders::GameFolders::tempTextures) + "\\interface\\" + id + "_frame.png");

	baseTextr->depthRender = -1000;
	depthRender = -1000;
	frameTextr->depthRender = -1000 - 10;

	if (!font.loadFromFile(style.textFont)) {
		throw Log::Exception("load interface font");
	}
	textColor = style.textColor;
}

void CellInterface::changeDepthRender(int deltaDepth) {
	baseTextr->depthRender += deltaDepth;
	frameTextr->depthRender += deltaDepth;
	depthRender += deltaDepth;
}

void CellInterface::blit() {
	if (isVisible) {
		update();
		baseTextr->blit();
		frameTextr->blit();
		for (auto text : allText)
			text->blit();
		if (item != nullptr) {
			if (type == typeCell::rect)
				item->blit(positionRelWindow, sizeCell.y);
			else
				item->blit(positionRelWindow, sizeCell.x * 2);
		}
	}
	else {
		isActive = false;
	}
}

void CellInterface::phisicCollideDetecting() {
	if (type == typeCell::rect) {
		if (((positionRelWindow.x - sizeCell.x / 2 <= spl::WindowStateBox::mouseCurrPositionRelativeWindow.x) && (positionRelWindow.x + sizeCell.x / 2 >= spl::WindowStateBox::mouseCurrPositionRelativeWindow.x)) &&
			((positionRelWindow.y - sizeCell.y / 2 <= spl::WindowStateBox::mouseCurrPositionRelativeWindow.y) && (positionRelWindow.y + sizeCell.y / 2 >= spl::WindowStateBox::mouseCurrPositionRelativeWindow.y)))
			isActive = true;
		else
			isActive = false;
	}
	else if (type == typeCell::round) {
		if (sqrt((positionRelWindow.x - spl::WindowStateBox::mouseCurrPositionRelativeWindow.x)*(positionRelWindow.x - spl::WindowStateBox::mouseCurrPositionRelativeWindow.x) + (positionRelWindow.y - spl::WindowStateBox::mouseCurrPositionRelativeWindow.y)*(positionRelWindow.y - spl::WindowStateBox::mouseCurrPositionRelativeWindow.y)) <= sizeCell.x)
			isActive = true;
		else
			isActive = false;
	}
}

void CellInterface::animation() {
	if (isActive) {
		if (animationCoef + speedChangeTransper <= 1)
			animationCoef += speedChangeTransper;
		else
			animationCoef = 1;

		baseTextr->g_body.setColor(sf::Color(255, 255, 255, deltaTransp + (255 - deltaTransp)*animationCoef));
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
	//spl::Window::updateWindowStateBox();//don`t need there //too much calls 
	baseTextr->updateTextrPosition(spl::WindowStateBox::inGameZeroCordRelativeWindow + positionRelWindow / spl::WindowStateBox::absoluteScale, 0);
	baseTextr->g_body.setScale(sf::Vector2f(1 / spl::WindowStateBox::absoluteScale, 1 / spl::WindowStateBox::absoluteScale));
	frameTextr->updateTextrPosition(spl::WindowStateBox::inGameZeroCordRelativeWindow + positionRelWindow / spl::WindowStateBox::absoluteScale, 0);
	frameTextr->g_body.setScale(sf::Vector2f(1 / spl::WindowStateBox::absoluteScale, 1 / spl::WindowStateBox::absoluteScale));
	animation();
	for (auto text : allText) {
		text->setPosition(spl::WindowStateBox::inGameZeroCordRelativeWindow + positionRelWindow / spl::WindowStateBox::absoluteScale);
		text->setScale(1 / spl::WindowStateBox::absoluteScale);
	}
}

CellInterface::~CellInterface() {
	if (baseTextr != nullptr)
		delete baseTextr;
	if (frameTextr != nullptr)
		delete frameTextr;
	for (auto i : allText)
		delete i;
}

void CellInterface::textControl(std::string mod, int id, std::string text, sf::Vector2f posRatio, float scaleRatio) {
	if (mod == "new") {
		for (std::vector<Text*>::iterator i = allText.begin(); i != allText.end(); i++) {
			if ((*i)->getId() == id) {
				delete (*i);
				allText.erase(i);
				break;
			}
		}
		allText.push_back(new Text(id, text, posRatio, scaleRatio, textColor, font, *this));
		allText.back()->changeDepthRender(-1005);
		allText.back()->setColor(textColor);
	}
	else if (mod == "del") {
		for (std::vector<Text*>::iterator i = allText.begin(); i != allText.end(); i++) {
			if ((*i)->getId() == id) {
				delete *i;
				allText.erase(i);
				goto end;
			}
		}
		Log::warning("No such text in " + this->id + " cell to delete", true);
	}
	else
		throw Log::Exception("no such mode in CellInterface::textControl", true);
end:;



}

CellInterface::Text* CellInterface::getTextPtr(int id) {
	for (auto text : allText)
		if (text->getId() == id)
			return text;
}

void CellInterface::setItem(InterfaceItem *item) {
	this->item = nullptr;
	this->item = item;
	this->item->setDepthRender(depthRender - 3);
}

void CellInterface::removeItem() {
	item = nullptr;
}

CellInterface::CellTexture::CellTexture(sf::Vector2f initCord, string textr) {
	if (!texture.loadFromFile(textr)) {
		throw Log::Exception("Texture load in CellInterface::CellTexture from " + textr, true);
	}
	texture.setSmooth(true);
	g_body.setTexture(texture);
	g_body.setOrigin(sf::Vector2f(texture.getSize().x / 2, texture.getSize().y / 2));
	g_body.setPosition(initCord);
}

CellInterface::CellTexture::CellTexture() {	//empty//for inheritance
	isVisible = false;
}

void CellInterface::CellTexture::updateTextrPosition(sf::Vector2f newCord, float newAngle) {
	g_body.setPosition(newCord);
	g_body.setRotation(newAngle);
}

void CellInterface::CellTexture::blit() {
	if (isVisible) {
		spl::ToDraw draw = { &g_body, depthRender };
		spl::Window::allDrawable.push_back(draw);
	}
}

/////////////// TEXT ////////////

CellInterface::Text::Text(int id, std::string text, sf::Vector2f posRatio, float scaleRatio, sf::Color textColor, sf::Font &font, CellInterface& cell) {
	this->id = id;
	this->text.setFont(font);
	this->text.setString(text);
	this->text.setCharacterSize(scaleRatio* (spl::WindowStateBox::currScreenSize.x / 1000));
	this->text.setFillColor(textColor);
	this->text.setOutlineColor(textColor - sf::Color(10, 10, 10));

	this->text.setOrigin(this->text.getCharacterSize() * text.size() / 4, this->text.getCharacterSize() / 2);

	if (cell.type == typeCell::rect)
		this->posRatio = sf::Vector2f((posRatio.x / 100)*cell.sizeCell.x, (posRatio.y / 100)*cell.sizeCell.y);
	else
		this->posRatio = sf::Vector2f((posRatio.x / 100)*cell.sizeCell.x, (posRatio.y / 100)*cell.sizeCell.x);
	this->depthRender = cell.depthRender - 1;
}

void CellInterface::Text::setPosition(sf::Vector2f position) {
	text.setPosition(position + posRatio / spl::WindowStateBox::absoluteScale);
}

void CellInterface::Text::setScale(float scale) {
	text.setScale(scale, scale);
}

void CellInterface::Text::setNewText(string text) {
	this->text.setString(text);
}

void CellInterface::Text::setColor(sf::Color color) {
	this->text.setColor(color);
}

void CellInterface::Text::setSize(float size) {
	this->text.setCharacterSize(size* (spl::WindowStateBox::currScreenSize.x / 1000));
}

int CellInterface::Text::getId() {
	return id;
}

void CellInterface::Text::changeDepthRender(int delta) {
	depthRender += delta;
}

void CellInterface::Text::blit() {
	spl::ToDraw draw = { &text, depthRender };
	spl::Window::allDrawable.push_back(draw);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////// ASSEMBLY LAYER INTERFACE ///////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////

AssemblyLayerInterface::AssemblyLayerInterface(string id, string mode, string styleId) {
	if (mode == "new") {
		std::ofstream file(spl::Folders::getGameFolderPath(spl::Folders::GameFolders::interfaceFiles) + "\\" + id + ".json", ios::trunc);

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
	ifstream file(spl::Folders::getGameFolderPath(spl::Folders::GameFolders::interfaceFiles) + "\\" + id + ".json");
	if (!file.is_open())
		throw Log::Exception("no such id file", true);

	json j;
	j << file;
	string t = j["styleId"];
	this->styleId = t;

	for (auto element : j) {
		try {
			if (element["typeObject"] == "CellInterface") {
				CellInterface* toSaveCell = new CellInterface(
					sf::Vector2f(element["initRatio"][0], element["initRatio"][1]),
					sf::Vector2f(element["sizeRatio"][0], element["sizeRatio"][1]),
					getStyle(element["style"]),
					element["type"],
					element["id"]);

				if (element["text"].is_object())
					for (auto exs : element["text"]) {
						toSaveCell->textControl(
							"new",
							exs["id"],
							exs["content"],
							sf::Vector2f(exs["posRatio"][0], exs["posRatio"][1]),
							exs["scale"]);
					}
				allCell.push_back(toSaveCell);


			}
		}
		catch (domain_error) {}
	}
	file.close();
}

void AssemblyLayerInterface::textControl(string cellId, string mod, int idText, string text, sf::Vector2f posRatio, float scaleRatio) {

	ifstream file(spl::Folders::getGameFolderPath(spl::Folders::GameFolders::interfaceFiles) + "\\" + this->id + ".json");
	if (!file.is_open())
		throw Log::Exception("no such id file", true);
	json j;
	j << file;

	if (mod == "new") {
		j[cellId]["text"][to_string(idText)]["id"] = idText;
		j[cellId]["text"][to_string(idText)]["content"] = text;
		j[cellId]["text"][to_string(idText)]["posRatio"] = { posRatio.x , posRatio.y };
		j[cellId]["text"][to_string(idText)]["scale"] = scaleRatio;
	}
	else if (mod == "del") {
		j[cellId]["text"].erase(to_string(idText));
	}
	file.close();
	ofstream file_o(spl::Folders::getGameFolderPath(spl::Folders::GameFolders::interfaceFiles) + "\\" + this->id + ".json");
	if (!file_o.is_open())
		throw Log::Exception("no such id file", true);

	file_o << j;
	file_o.close();
	updateAllCellFromFile();
}

void AssemblyLayerInterface::createNewCell(sf::Vector2f initRatio, sf::Vector2f sizeRatio, CellInterface::typeCell type, std::string id, string styleId) {
	json jStyle;

	ifstream inStreamFile(spl::Folders::getGameFolderPath(spl::Folders::GameFolders::interfaceFiles) + "\\" + this->id + ".json");
	if (!inStreamFile.is_open())
		throw Log::Exception(spl::Folders::getGameFolderPath(spl::Folders::GameFolders::interfaceFiles) + "\\" + this->id + ".json", true);


	jStyle << inStreamFile;
	jStyle[id]["initRatio"] = { initRatio.x, initRatio.y };
	jStyle[id]["sizeRatio"] = { sizeRatio.x, sizeRatio.y };
	jStyle[id]["type"] = int(type);
	if (styleId == "default")
		jStyle[id]["style"] = this->styleId;
	else
		jStyle[id]["style"] = styleId;
	jStyle[id]["typeObject"] = "CellInterface";
	jStyle[id]["id"] = id;

	ofstream outStreamFile(spl::Folders::getGameFolderPath(spl::Folders::GameFolders::interfaceFiles) + "\\" + this->id + ".json");
	if (!outStreamFile.is_open())
		throw Log::Exception(spl::Folders::getGameFolderPath(spl::Folders::GameFolders::interfaceFiles) + "\\" + this->id + ".json", true);

	outStreamFile << jStyle;
	outStreamFile.close();
	inStreamFile.close();

	updateAllCellFromFile();
}

CellInterface::StyleCell AssemblyLayerInterface::getStyle(string styleId) {
	ifstream file(spl::Folders::getGameFolderPath(spl::Folders::GameFolders::interfaceFiles) + "\\styles.json");
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
			j[styleId]["speedChangeTransper"],
			j[styleId]["font"],
			sf::Color(j[styleId]["textColor"][0],j[styleId]["textColor"][1], j[styleId]["textColor"][2])
		};

		return style;
	}
	else
		throw Log::Exception("no such style id");
	file.close();
}

string AssemblyLayerInterface::getActiveCell() {
	for (int i = 0; i < allCell.size(); i++)
		if (allCell[i]->isActive)
			return allCell[i]->id;
	return "None";
}

CellInterface* AssemblyLayerInterface::getCellById(string id) {
	for (int i = 0; i < allCell.size(); i++)
		if (allCell[i]->id == id)
			return allCell[i];
	throw Log::Exception("no such cell id", true);
}

void AssemblyLayerInterface::deleteCell(std::string id) {
	ifstream file(spl::Folders::getGameFolderPath(spl::Folders::GameFolders::interfaceFiles) + "\\" + this->id + ".json");
	if (!file.is_open())
		throw Log::Exception("no such id file", true);

	json j;
	j << file;
	j.erase("cell_1");

	file.close();
	ofstream outStreamFile(spl::Folders::getGameFolderPath(spl::Folders::GameFolders::interfaceFiles) + "\\" + this->id + ".json");
	if (!outStreamFile.is_open())
		throw Log::Exception(spl::Folders::getGameFolderPath(spl::Folders::GameFolders::interfaceFiles) + "\\" + this->id + ".json", true);

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
std::string UserInterfaceBox::getActiveLayer() { return activeLayer; }
std::string UserInterfaceBox::getActiveCell() { return activeCell; }

UserInterfaceBox::UserInterfaceBox() {
	spl::Folders::deleteFolder(spl::Folders::getGameFolderPath(spl::Folders::GameFolders::tempTextures) + "\\interface");
	spl::Folders::createFolder(spl::Folders::getGameFolderPath(spl::Folders::GameFolders::tempTextures) + "\\interface");

	CellInterface::StyleCell style = { sf::Color(10,60,50), sf::Color(0,255,255), 2, 2, 50, 200, 0.05, "font/arial.ttf", sf::Color(255,255,255) };
	createStyle("default", style);
}

void UserInterfaceBox::createStyle(string id, CellInterface::StyleCell& style) {
	json jStyle;

	ifstream inStreamFile(spl::Folders::getGameFolderPath(spl::Folders::GameFolders::interfaceFiles) + "\\styles.json");
	if (!inStreamFile.is_open())
		throw Log::Exception(spl::Folders::getGameFolderPath(spl::Folders::GameFolders::interfaceFiles) + "\\styles.json", true);

	jStyle << inStreamFile;
	jStyle[id]["baseColor"] = { style.baseColor.r,style.baseColor.g, style.baseColor.b,style.baseColor.a };
	jStyle[id]["frameColor"] = { style.frameColor.r,style.frameColor.g, style.frameColor.b,style.frameColor.a };
	jStyle[id]["borderSize"] = style.borderSize;
	jStyle[id]["shadow"] = style.shadow;
	jStyle[id]["deltaTransperActive"] = style.deltaTransperActive;
	jStyle[id]["deltaTransperQuiet"] = style.deltaTransperQuiet;
	jStyle[id]["speedChangeTransper"] = style.speedChangeTransper;
	jStyle[id]["font"] = style.textFont;
	jStyle[id]["textColor"] = { style.frameColor.r,style.frameColor.g, style.frameColor.b };

	ofstream outStreamFile(spl::Folders::getGameFolderPath(spl::Folders::GameFolders::interfaceFiles) + "\\styles.json");
	if (!outStreamFile.is_open())
		throw Log::Exception("Can`t open " + spl::Folders::getGameFolderPath(spl::Folders::GameFolders::interfaceFiles) + "\\styles.json", true);

	outStreamFile << jStyle;
	outStreamFile.close();
}

void UserInterfaceBox::createLayer(string id, string styleId) {
	this->fastAccessLayer.push_back(new AssemblyLayerInterface(id, "new", styleId));
	currInterfaceLayer = fastAccessLayer.back();
	activeLayer = id;
}

void UserInterfaceBox::downloadLayerById(string id) {
	for (std::vector<AssemblyLayerInterface*>::iterator i = fastAccessLayer.begin(); i != fastAccessLayer.end(); i++) {
		if ((*i)->id == id) {
			currInterfaceLayer = (*i);
			activeLayer = id;
			return;
		}
	}
	this->fastAccessLayer.push_back(new AssemblyLayerInterface(id, "old"));
	currInterfaceLayer = fastAccessLayer.back();
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
	for (auto i : fastAccessLayer)
		delete i;
	fastAccessLayer.clear();
	spl::Folders::deleteFolder(spl::Folders::getGameFolderPath(spl::Folders::GameFolders::tempTextures) + "\\interface");
}


///////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////    INTERFACE CREATOR    ////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////




///////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////      ICON ANIMATED      ////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////