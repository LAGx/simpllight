#include "item.h"
#include "log.h"
#include <cmath>

sf::Texture* BaseItem::itemTexture = nullptr;

BaseItem::BaseItem(string name, sf::Vector2i startPos, sf::Vector2i size, string file) {
	this->name = name;
    if(file != "None")
		setLocalImage(file);
	setVisible(true);
	setSpriteParameters(startPos, size);
}

void BaseItem::setGlobalImage(string file) {
	static bool flag = false;
	itemTexture = new sf::Texture;
	itemTexture->setSmooth(true);
	if (!itemTexture->loadFromFile(file)) {
		throw Log::Exception("can`t load texture itemImage from " + file, true);
		flag = true;
	}
	if (flag) 
		throw Log::Exception("unallawed redefinition of itemImage", true);
}

void BaseItem::setLocalImage(string file) {
	if (!texture.loadFromFile(file))
		throw Log::Exception("can`t load texture itemImage from " + file, true);
	texture.setSmooth(true);
	g_body.setTexture(texture);
	isLocalTexture = true;
}

void BaseItem::setSpriteParameters(sf::Vector2i startPos, sf::Vector2i size) {
	if (isLocalTexture) {
		texture.loadFromImage(texture.copyToImage(), sf::IntRect(startPos, size));
		g_body.setTexture(texture);
	}
	else {
		texture.loadFromImage(itemTexture->copyToImage(), sf::IntRect(startPos, size));
		g_body.setTexture(texture);
	}
	g_body.setTextureRect(sf::IntRect(0, 0, texture.getSize().y, texture.getSize().y));
}

void BaseItem::setVisible(bool isVisible) {
	this->isVisible = isVisible;
}

void BaseItem::set_isPlaying(bool isPlaying) {
	this->isPlaying = isPlaying;
	if (!isPlaying) {
		g_body.setTextureRect(sf::IntRect(0,0, texture.getSize().y, texture.getSize().y));
	}
}

void BaseItem::setSpeedAnimations(float speed) {
	speedAnimations = speed;
}

void BaseItem::animation(){
	if (isPlaying) {
		if (std::fabs(spl::Window::clock.getElapsedTime().asMilliseconds()-lastTime) > speedAnimations) {
			if (cursor < (texture.getSize().x / texture.getSize().y)) {
				g_body.setTextureRect(sf::IntRect(cursor*texture.getSize().y, 0, texture.getSize().y, texture.getSize().y));
				cursor++;
			}
			else {
				cursor = 0;
				return;
			}
			lastTime = spl::Window::clock.getElapsedTime().asMilliseconds();
		}
	}
}

void BaseItem::setDepthRender(int depth) {
	depthRender = depth;
}

void BaseItem::deleteGloabImage() {
	delete itemTexture;
}

///////////////////////////////////////////////////////////


InterfaceItem::InterfaceItem(string name, sf::Vector2i startPos):BaseItem(name, startPos, sf::Vector2i(64*16,64)){//make standart size and frames
}


void InterfaceItem::blit(sf::Vector2f pos, float size_to_spread_Y){
	if (isVisible) {
		animation();
	    g_body.setScale(sf::Vector2f((size_to_spread_Y / (g_body.getTextureRect().height+0)) / spl::WindowStateBox::absoluteScale, (size_to_spread_Y/ (g_body.getTextureRect().height +0 )) / spl::WindowStateBox::absoluteScale));
		updateTextrPosition(spl::WindowStateBox::inGameZeroCordRelativeWindow + sf::Vector2f(pos.x- g_body.getTextureRect().height / 2 *(size_to_spread_Y /( g_body.getTextureRect().height + 6)), pos.y- g_body.getTextureRect().height /2 * (size_to_spread_Y / (g_body.getTextureRect().height + 6))) / spl::WindowStateBox::absoluteScale);
		BaseObject::blit();
	}
}


