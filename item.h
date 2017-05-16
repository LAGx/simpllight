#ifndef _ITEM_
#define _ITEM_

#pragma once
#include "game_objects.h"

using namespace std;

class BaseItem :public BaseObject {
protected:
	static sf::Texture* itemTexture;
	int speedAnimations = 300; //delay
	bool isPlaying = false;
	string name = "None";
	bool isLocalTexture = false;
	char cursor = 0;
	float lastTime;
public:

	BaseItem(string name, sf::Vector2i startPos, sf::Vector2i size, string file="None");

	void setVisible(bool isVisible);
	void set_isPlaying(bool isPlaying);
	void setSpeedAnimations(bool speed);

	static void setGlobalImage(string file);
	static void deleteGloabImage();
	void setLocalImage(string file);
	void setSpriteParameters(sf::Vector2i startPos, sf::Vector2i size);//square sprites with side size.x//

	void animation();
	virtual void blit() = 0;
};


class WorldItem;

class InterfaceItem :public BaseItem, protected spl::EventInterface {
private:

public:

	InterfaceItem(string name);

	WorldItem* convertToWorldItem();

	void blit();
	void blitInCell(float scale_Rel_Y);
};


class WorldItem : public BaseItem, public StaticObject {
private:


public:
	InterfaceItem* convertToInterfaceItem();
};

#endif