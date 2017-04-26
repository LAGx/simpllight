#include <SFML/Graphics.hpp>

#include "control.h"
#include "log.h"
#include "state.h"
#include "INIReader.h"
#include "window.h"


using namespace spl;

ControlBox::ControlBox(){
	updateKeyBindings();
}

void ControlBox::updateKeyBindings() {
	INIReader settings("settings.ini");


	if (settings.ParseError() < 0)
		Log::warning("Can't load 'settings.ini', using default bindings");

  
	keyBindings.moveTop = (sf::Keyboard::Key) settings.GetInteger("keyBindings", "moveTop", 22);
	keyBindings.moveBottom = (sf::Keyboard::Key) settings.GetInteger("keyBindings", "moveBottom", 18);
	keyBindings.moveLeft = (sf::Keyboard::Key) settings.GetInteger("keyBindings", "moveLeft", 0);
	keyBindings.moveRight = (sf::Keyboard::Key) settings.GetInteger("keyBindings", "moveRight", 3);
	keyBindings.l_ctrl = (sf::Keyboard::Key) settings.GetInteger("keyBindings", "l_ctrl", 37);
	keyBindings.l_shift = (sf::Keyboard::Key) settings.GetInteger("keyBindings", "l_shift", 38);
#ifdef DEV_MODE
	keyBindings.moveRadiusPlus = (sf::Keyboard::Key) settings.GetInteger("keyBindings", "moveRadiusPlus", 4);
	keyBindings.moveRadiusMinus = (sf::Keyboard::Key) settings.GetInteger("keyBindings", "moveRadiusMinus", 16);
#endif
	
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

for (size_t i = 0; i < controlObjects.size(); i++) {
	if (sf::Keyboard::isKeyPressed(keyBindings.moveTop))
		controlObjects[i]->moveTop();
	if (sf::Keyboard::isKeyPressed(keyBindings.moveBottom))
		controlObjects[i]->moveBottom();
	if (sf::Keyboard::isKeyPressed(keyBindings.moveLeft))
		controlObjects[i]->moveLeft();
	if (sf::Keyboard::isKeyPressed(keyBindings.moveRight))
		controlObjects[i]->moveRight();

	if (sf::Keyboard::isKeyPressed(keyBindings.l_ctrl))
		controlObjects[i]->l_ctrl();
	if (sf::Keyboard::isKeyPressed(keyBindings.l_shift))
		controlObjects[i]->l_shift();

  #ifdef DEV_MODE
	if (sf::Keyboard::isKeyPressed(keyBindings.moveRadiusPlus))
		controlObjects[i]->moveRadiusPlus();
	if (sf::Keyboard::isKeyPressed(keyBindings.moveRadiusMinus))
		controlObjects[i]->moveRadiusMinus();
  #endif
  
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
	for (size_t i = 0; i < controlObjects.size(); i++) {

		if (controlObjects[i] == obj) {
			controlObjects.erase(controlObjects.begin()+i);
			return true;
		}
	}

	throw Log::Exception("No such object in Control to remove.", true);
	return false;
}