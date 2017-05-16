#pragma once

#include "game_objects.h"





class BaseItem :public BaseObject {
private:
	static sf::Image* itemImage;

	int speedAnimations = 0;//frame per sec.
	bool isPlaying = false;
	string name = "None";
public:

	BaseItem(string id);

	void setVisible(bool isVisible);
	void set_isPlaying(bool isPlaying);
	void setSpeedAnimations(bool speed);

	virtual void blit() = 0;
};

class InterfaceItem :public BaseItem {
private:


public:

	WorldItem* convertToWorldItem();

	//void blit();
};

class WorldItem : public BaseItem, public StaticObject {
private:


public:
	InterfaceItem* convertToInterfaceItem();
};


