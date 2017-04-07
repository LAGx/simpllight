#include <SFML/Graphics.hpp>
#include <fstream>

#include "control.h"
#include "log.h"
#include "state.h"
#include "INIReader.h"
#include "window.h"

using namespace spl;

ControlBox::ControlBox() {
	updateKeyBindings();
}

void ControlBox::updateKeyBindings() {
	INIReader settings("settings.ini");

	if (settings.ParseError() < 0) {
		Log::log("Can't load 'settings.ini', creating and using default bindings");
		saveKeyBindings();
	}
	else {
		keyBindings.moveTop = (sf::Keyboard::Key) settings.GetInteger("keyBindings", "moveTop", 22);
		keyBindings.moveBottom = (sf::Keyboard::Key) settings.GetInteger("keyBindings", "moveBottom", 18);
		keyBindings.moveLeft = (sf::Keyboard::Key) settings.GetInteger("keyBindings", "moveLeft", 0);
		keyBindings.moveRight = (sf::Keyboard::Key) settings.GetInteger("keyBindings", "moveRight", 3);

		keyBindings.primaryMouseAction = (sf::Mouse::Button) settings.GetInteger("keyBindings", "primaryMouseAction", 0);
		keyBindings.secondaryMouseAction = (sf::Mouse::Button) settings.GetInteger("keyBindings", "secondaryMouseAction", 1);
		keyBindings.verticalMouseWheel = (sf::Mouse::Wheel) settings.GetInteger("keyBindings", "verticalMouseWheel", 0);
		keyBindings.horizontalMouseWheel = (sf::Mouse::Wheel) settings.GetInteger("keyBindings", "horizontalMouseWheel", 1);
	}
}

void ControlBox::saveKeyBindings() {
	ofstream settingsFile("settings.ini", ios_base::app);

	settingsFile << "[keyBindings]" << endl
		<< "moveTop = " << keyBindings.moveTop << endl
		<< "moveBottom = " << keyBindings.moveBottom << endl
		<< "moveLeft = " << keyBindings.moveLeft << endl
		<< "moveRight = " << keyBindings.moveRight << endl
		<< "primaryMouseAction = " << keyBindings.primaryMouseAction << endl
		<< "secondaryMouseAction = " << keyBindings.secondaryMouseAction << endl
		<< "verticalMouseWheel = " << keyBindings.verticalMouseWheel << endl
		<< "horizontalMouseWheel = " << keyBindings.horizontalMouseWheel << endl;

	Log::log("Saved 'settings.ini'");
	settingsFile.close();
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
		if (sf::Mouse::isButtonPressed(keyBindings.primaryMouseAction))
			controlObjects[i]->primaryMouseAction();
		if (sf::Mouse::isButtonPressed(keyBindings.secondaryMouseAction))
			controlObjects[i]->secondaryMouseAction();

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
			controlObjects.erase(controlObjects.begin() + i);
			return true;
		}
	}

	throw Log::Exception("No such object in Control to remove.", true);
	return false;
}