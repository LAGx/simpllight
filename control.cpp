#include <SFML/Graphics.hpp>

#include "control.h"
#include "log.h"
#include "state.h"
#include "INIReader.h"


using namespace spl;

ControlBox::ControlBox(){
	updateKeyBindings();
}

void ControlBox::updateKeyBindings() {
	INIReader settings("settings.ini");

	if (settings.ParseError() < 0)
		Log::warning("Can't load 'settings.ini', using default bindings\n");

	keyBindings.moveTop = (sf::Keyboard::Key) settings.GetInteger("keyBindings", "moveTop", 22);
	keyBindings.moveBottom = (sf::Keyboard::Key) settings.GetInteger("keyBindings", "moveBottom", 18);
	keyBindings.moveLeft = (sf::Keyboard::Key) settings.GetInteger("keyBindings", "moveLeft", 0);
	keyBindings.moveRight = (sf::Keyboard::Key) settings.GetInteger("keyBindings", "moveRight", 3);
#ifdef DEV_MODE
	keyBindings.moveRadiusPlus = (sf::Keyboard::Key) settings.GetInteger("keyBindings", "moveRadiusPlus", 4);
	keyBindings.moveRadiusMinus = (sf::Keyboard::Key) settings.GetInteger("keyBindings", "moveRadiusMinus", 16);
#endif
}

void ControlBox::setControlObject(EventInterface* obj) {
	controlObjects.push_back(obj);
}

void ControlBox::resulveControl() {
	for (size_t i = 0; i < controlObjects.size(); i++) {

		if (sf::Keyboard::isKeyPressed(keyBindings.moveTop))
			controlObjects[i]->moveTop();
		if (sf::Keyboard::isKeyPressed(keyBindings.moveBottom))
			controlObjects[i]->moveBottom();
		if (sf::Keyboard::isKeyPressed(keyBindings.moveLeft))
			controlObjects[i]->moveLeft();
		if (sf::Keyboard::isKeyPressed(keyBindings.moveRight))
			controlObjects[i]->moveRight();
#ifdef DEV_MODE
		if (sf::Keyboard::isKeyPressed(keyBindings.moveRadiusPlus))
			controlObjects[i]->moveRadiusPlus();
		if (sf::Keyboard::isKeyPressed(keyBindings.moveRadiusMinus))
			controlObjects[i]->moveRadiusMinus();
#endif
	}
}


bool ControlBox::deleteControlObject(EventInterface* obj) {
	for (size_t i = 0; i < controlObjects.size(); i++) {

		if (controlObjects[i] == obj) {
			controlObjects.erase(controlObjects.begin()+i);
			return true;
		}
	}

	Log::error("No such object in Control to remove.", true);
	return false;
}