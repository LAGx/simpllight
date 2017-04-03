#include "control.h"
#include <SFML/Graphics.hpp>
#include "log.h"
#include "state.h"

using namespace spl;

ControlBox::ControlBox(){
	updateKeyBindings();
}

void ControlBox::updateKeyBindings() {
	keyBindings.moveTop = sf::Keyboard::W;
	keyBindings.moveBottom = sf::Keyboard::S;
	keyBindings.moveLeft = sf::Keyboard::A;
	keyBindings.moveRight = sf::Keyboard::D;

	keyBindings.moveRadiusPlus = sf::Keyboard::E;
	keyBindings.moveRadiusMinus = sf::Keyboard::Q;
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