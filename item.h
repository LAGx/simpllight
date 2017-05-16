#ifndef _ITEM_
#define _ITEM_

#pragma once
#include "game_objects.h"

using namespace std;

class BaseItem :public BaseObject {
private:
	static sf::Texture* itemTexture;
	int speedAnimations = 0; //frame per sec.
	bool isPlaying = false;
	string name = "None";
	bool isLocalTexture = false;

public:

	BaseItem(string name);

	void setVisible(bool isVisible);
	void set_isPlaying(bool isPlaying);
	void setSpeedAnimations(bool speed);

	static void setGlobalImage(string file);
	void setLocalImage(string file);
	void setSpriteParameters(sf::Vector2i startPos, sf::Vector2i size);//square sprites with side size.x//

	void animation();
	virtual void blit() = 0;
};

class WorldItem;

class InterfaceItem :public BaseItem, protected spl::EventInterface {
private:

public:

	InterfaceItem();

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