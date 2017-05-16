#include "item.h"
#include "log.h"


sf::Texture* BaseItem::itemTexture = nullptr;

BaseItem::BaseItem(string name) {
	name = name;
}

void BaseItem::setGlobalImage(string file) {
	static bool flag = false;
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
	isLocalTexture = true;
}

void BaseItem::setSpriteParameters(sf::Vector2i startPos, sf::Vector2i size) {
	
}

void BaseItem::setVisible(bool isVisible) {

}

void BaseItem::set_isPlaying(bool isPlaying) {

}

void BaseItem::setSpeedAnimations(bool speed) {

}

void BaseItem::animation(){

}
