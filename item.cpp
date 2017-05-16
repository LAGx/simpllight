#include "item.h"
#include "log.h"


sf::Texture* BaseItem::itemTexture = nullptr;

BaseItem::BaseItem(string name, sf::Vector2i startPos, sf::Vector2i size, string file = "None") {
	name = name;
	if(file != "None")
		setLocalImage(file);
	void setSpriteParameters(sf::Vector2i startPos, sf::Vector2i size);
}

void BaseItem::setGlobalImage(string file) {
	static bool flag = false;
	itemTexture = new sf::Texture;
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
	g_body.setTexture(texture);
	isLocalTexture = true;
}

void BaseItem::setSpriteParameters(sf::Vector2i startPos, sf::Vector2i size) {
	if (!(size.y%size.x))
		Log::warning("BaseItem::setSpriteParameterst, sprite can`t divide by square", true);
	if (isLocalTexture) {
		g_body.setTexture(texture);
	}
	else {
		texture.loadFromImage(itemTexture->copyToImage(), sf::IntRect(startPos, size));
		g_body.setTexture(texture);
	}
}

void BaseItem::setVisible(bool isVisible) {
	this->isVisible = isVisible;
}

void BaseItem::set_isPlaying(bool isPlaying) {
	this->isPlaying = isPlaying;
	if (!isPlaying) {
		g_body.setTextureRect(sf::IntRect(0,0, texture.getSize().x, texture.getSize().x));
	}
}

void BaseItem::setSpeedAnimations(bool speed) {
	speedAnimations = speed;
}

void BaseItem::animation(){
	if (isPlaying) {
		if (lastTime - spl::Window::clock.getElapsedTime().asMilliseconds() > speedAnimations) {
			if (cursor < (texture.getSize().y / texture.getSize().x)) {
				g_body.setTextureRect(sf::IntRect(cursor*texture.getSize().x, 0, texture.getSize().x, texture.getSize().x));
				cursor++;
			}
			else
				cursor = 0;
			lastTime = spl::Window::clock.getElapsedTime().asMilliseconds();
		}
	}
}

void BaseItem::deleteGloabImage() {
	delete itemTexture;
}

///////////////////////////////////////////////////////////

InterfaceItem::InterfaceItem(string name):BaseItem(name, ){//make standart size and frames

}