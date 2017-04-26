#include "interface.h"
#include "window.h"
#include "log.h"
#include "service.h"
#include <cstdio>  

CellInterface::CellInterface(sf::Vector2f initRatio, sf::Vector2f sizeRatio, StyleCell& style, typeCell type, std::string id) {
	this->type = type;
	this->id = id;
	this->speedChangeTransper = style.speedChangeTransper;
	deltaTransp = style.deltaTransperActive - style.deltaTransperQuiet;
	this->positionRelWindow = sf::Vector2f(spl::WindowStateBox::currScreenSize.x * initRatio.x/100, spl::WindowStateBox::currScreenSize.y * initRatio.y / 100);
	
	TextureGenerator textureGenerator;

	switch (this->type) {
	case typeCell::rect:
		sizeCell = sf::Vector2f(spl::WindowStateBox::currScreenSize.x * sizeRatio.x / 100, spl::WindowStateBox::currScreenSize.y * sizeRatio.y / 100);
		textureGenerator.fillShadowRect(style.shadow, sf::Vector2i(sizeCell), style.baseColor - sf::Color(0,0,0, style.deltaTransperActive), "image/tempInterface/" + id + "_base.png");
		textureGenerator.conturRect(sf::Vector2i(sizeCell), style.borderSize,style.frameColor, "image/tempInterface/" + id + "_frame.png");
		break;
	case typeCell::round:
		sizeCell.x = spl::WindowStateBox::currScreenSize.x * sizeRatio.x / 100;
		textureGenerator.fillShadowCircle(style.shadow, int(sizeCell.x), style.baseColor - sf::Color(0, 0, 0, style.deltaTransperActive), "image/tempInterface/" + id + "_base.png");
		textureGenerator.conturCircle(sizeCell.x, style.borderSize, style.frameColor, "image/tempInterface/" + id + "_frame.png");
		break;
	default:
		throw Log::Exception("Error type in CellInterface");
	}
	
	this->baseTextr  = new BaseObject(sf::Vector2f(0,0), "image/tempInterface/" + id + "_base.png");
	this->frameTextr = new BaseObject(sf::Vector2f(0,0), "image/tempInterface/" + id + "_frame.png");

	setDepthRender(-500);
}

void CellInterface::setDepthRender(int depth) {
	this->depthRender = depth;
	baseTextr->depthRender = depth;
	frameTextr->depthRender = depth-1;
}

void CellInterface::blit() {
	update();
	baseTextr->blit();
	frameTextr->blit();
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

void CellInterface::update() {
	phisicCollideDetecting();
	spl::Window::updateWindowStateBox();//don`t need there //too much calls 
	baseTextr->updateTextrPosition(spl::WindowStateBox::inGameZeroCordRelativeWindow + positionRelWindow/spl::WindowStateBox::absoluteScale, 0);
	baseTextr->g_body.setScale(sf::Vector2f(1 / spl::WindowStateBox::absoluteScale, 1 / spl::WindowStateBox::absoluteScale));
	frameTextr->updateTextrPosition(spl::WindowStateBox::inGameZeroCordRelativeWindow + positionRelWindow/spl::WindowStateBox::absoluteScale, 0);
	frameTextr->g_body.setScale(sf::Vector2f(1 / spl::WindowStateBox::absoluteScale, 1 / spl::WindowStateBox::absoluteScale));

	////////////////////TODO ANIMATION
	static float i = 0;
	if (isActive) {	
		if (i <= 1) {
			baseTextr->g_body.setColor(sf::Color(255, 255, 255, 255 * i));
			i +=  speedChangeTransper;
		}
	}
	else {
		if (i <= 1) {
			baseTextr->g_body.setColor(sf::Color(255, 255, 255, deltaTransp* i));
			i +=  speedChangeTransper;
		}
	}
	///////////////////////////////////
}

CellInterface::~CellInterface() {
	if (baseTextr != nullptr)
		delete baseTextr;
	if (frameTextr != nullptr)
		delete frameTextr;

	if (remove(("image/tempInterface/" + id + "_base.png").c_str()))
		Log::warning("no such image "+ id +"_base interface to delete ", true);
	if (remove(("image/tempInterface/" + id+ "_frame.png").c_str()))
		Log::warning("no such image " + id + "_frame interface to delete ", true);
}