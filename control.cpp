#include "control.h"
#include <SFML/Graphics.hpp>
#include "log.h"
#include "window.h"

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

	keyBindings.selectMouse = sf::Mouse::Button::Left;
	keyBindings.useMouse = sf::Mouse::Button::Right;
}

void ControlBox::setControlObject(EventInterface* obj) {
	controlObjects.push_back(obj);
}

void ControlBox::resulveControl(Window &window) {
while (window.canvas.pollEvent(event)) {
	if (event.type == sf::Event::Closed)
		window.canvas.close();
}

for (int i = 0; i < controlObjects.size(); i++) {


	if (sf::Keyboard::isKeyPressed(keyBindings.moveTop))
		controlObjects[i]->moveTop();
	if (sf::Keyboard::isKeyPressed(keyBindings.moveBottom))
		controlObjects[i]->moveBottom();
	if (sf::Keyboard::isKeyPressed(keyBindings.moveLeft))
		controlObjects[i]->moveLeft();
	if (sf::Keyboard::isKeyPressed(keyBindings.moveRight))
		controlObjects[i]->moveRight();
	if (sf::Keyboard::isKeyPressed(keyBindings.moveRadiusPlus))
		controlObjects[i]->moveRadiusPlus();
	if (sf::Keyboard::isKeyPressed(keyBindings.moveRadiusMinus))
		controlObjects[i]->moveRadiusMinus();
	if (sf::Mouse::isButtonPressed(keyBindings.selectMouse))
		controlObjects[i]->selectMouse();
	if (sf::Mouse::isButtonPressed(keyBindings.useMouse))
		controlObjects[i]->useMouse();

	controlObjects[i]->positionMouse(sf::Mouse::getPosition(window.canvas).x, sf::Mouse::getPosition(window.canvas).y);
	if (event.type == sf::Event::MouseWheelScrolled) {
		controlObjects[i]->wheelMouse(event.mouseWheelScroll.delta);
		event.mouseWheelScroll.delta = 0;
	}
}
}


bool ControlBox::deleteControlObject(EventInterface* obj) {
	for (int i = 0; i < controlObjects.size(); i++) {

		if (controlObjects[i] == obj) {
			controlObjects.erase(controlObjects.begin()+i);
			return true;
		}
	}

	throw Log::Exception("No such object in Control to remove.", true);
	return false;
}