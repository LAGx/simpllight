#ifndef _ITEM_
#define _ITEM_

#pragma once
#include "game_objects.h"

using namespace std;

class BaseItem :public BaseObject {
private:
	bool isLocalTexture = false;
	char cursor = 0;
	float lastTime;
protected:
	static sf::Texture* itemTexture;
	float speedAnimations = 300; //delay
	bool isPlaying = true;
	string name = "None";
public:

	BaseItem(string name, sf::Vector2i startPos = { 0,0 }, sf::Vector2i size = {1,1}, string file = "None");

	void setVisible(bool isVisible);
	void set_isPlaying(bool isPlaying);
	void setSpeedAnimations(float speed);
	void setDepthRender(int depth);

	static void setGlobalImage(string file);
	static void deleteGloabImage();
	void setLocalImage(string file);
	void setSpriteParameters(sf::Vector2i startPos, sf::Vector2i size);//square sprites with side size.x//

	void animation();
	//virtual void blit() = 0;
};


class WorldItem;

class InterfaceItem :public BaseItem{//icons have to be 64x64, 4 lines * x frames 
private:

public:

	InterfaceItem(string name, sf::Vector2i startPos);

	WorldItem* convertToWorldItem(sf::Vector2f posToPut);

	void blit(sf::Vector2f pos = {0,0}, float scale_Rel_Y = 1);
};


class WorldItem : public BaseItem, public StaticObject {
private:


public:


	InterfaceItem* convertToInterfaceItem();

	void blit();
};

#endif