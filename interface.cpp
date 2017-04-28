#include "interface.h"
#include "window.h"
#include "log.h"
#include "service.h"
#include <cstdio>  
#include <string>
#include <iostream>
#include <windows.h>
#include <conio.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////     CELL INTERFACE     /////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////

CellInterface::CellInterface(sf::Vector2f initRatio, sf::Vector2f sizeRatio, StyleCell& style, typeCell type, std::string id) {
	this->type = type;
	this->id = id;
	this->speedChangeTransper = style.speedChangeTransper;
	deltaTransp = style.deltaTransperActive - style.deltaTransperQuiet;
	this->positionRelWindow = sf::Vector2f(spl::WindowStateBox::currScreenSize.x * initRatio.x/100, spl::WindowStateBox::currScreenSize.y * initRatio.y / 100);
	
	{
		TextureGenerator textureGenerator;
		if ((fopen(("image/tempInterface/" + id + "_base.png").c_str(), "r") == NULL) &&
			(fopen(("image/tempInterface/" + id + "_frame.png").c_str(), "r") == NULL)) {
			switch (this->type) {
			case typeCell::rect:
				sizeCell = sf::Vector2f(spl::WindowStateBox::currScreenSize.x * sizeRatio.x / 100, spl::WindowStateBox::currScreenSize.y * sizeRatio.y / 100);
				textureGenerator.fillShadowRect(style.shadow, sf::Vector2i(sizeCell), style.baseColor - sf::Color(0, 0, 0, style.deltaTransperActive), "image/tempInterface/" + id + "_base.png");
				textureGenerator.conturRect(sf::Vector2i(sizeCell), style.borderSize, style.frameColor, "image/tempInterface/" + id + "_frame.png");
				break;
			case typeCell::round:
				sizeCell.x = spl::WindowStateBox::currScreenSize.x * sizeRatio.x / 100;
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




///////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////    USER INTERFACE BOX    ///////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////

UserInterfaceBox::UserInterfaceBox() {
	CellInterface::StyleCell style = { sf::Color(10,60,50), sf::Color(0,255,255), 2, 2, 50, 200, 0.05};
	createStyle("default",style);
}



UserInterfaceBox::~UserInterfaceBox() {
	if (currInterfaceLayer != nullptr)
		delete currInterfaceLayer;

	if (!DeleteDirectory("tempInterface"))
		Log::warning("no directory tempInterface", true);
	CreateDirectory("tempInterface", NULL);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////    INTERFACE CREATOR    ////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////

