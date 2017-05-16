#include <SFML/Graphics.hpp>
#include <fstream>
#include <string>
#include <iostream>

#include "control.h"
#include "log.h"
#include "state.h"
#include "INIReader.h"
#include "INIWriter.h"
#include "window.h"
#include "service.h"

using namespace spl;


///////////////////////////////////////
// ---------  CONTROL_BOX  --------- //
///////////////////////////////////////

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
		keyBindings.apply = (sf::Keyboard::Key) settings.GetInteger("keyBindings", "apply", 5);
		keyBindings.console = (sf::Keyboard::Key) settings.GetInteger("keyBindings", "console", 54);

		keyBindings.primaryMouseAction = (sf::Mouse::Button) settings.GetInteger("keyBindings", "primaryMouseAction", 0);
		keyBindings.secondaryMouseAction = (sf::Mouse::Button) settings.GetInteger("keyBindings", "secondaryMouseAction", 1);
	}
}

void ControlBox::saveKeyBindings() {
	INIWriter settings;

	settings["keyBindings"]["moveTop"] = keyBindings.moveTop;
	settings["keyBindings"]["moveBottom"] = keyBindings.moveBottom;
	settings["keyBindings"]["moveLeft"] = keyBindings.moveLeft;
	settings["keyBindings"]["moveRight"] = keyBindings.moveRight;
	settings["keyBindings"]["primaryMouseAction"] = keyBindings.primaryMouseAction;
	settings["keyBindings"]["secondaryMouseAction"] = keyBindings.secondaryMouseAction;
	settings["keyBindings"]["apply"] = keyBindings.apply;
	settings["keyBindings"]["console"] = keyBindings.console;

	settings.saveToFile("settings.ini");

	Log::log("Saved 'settings.ini'");
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
		if (sf::Keyboard::isKeyPressed(keyBindings.apply))
			controlObjects[i]->apply();
		if (sf::Keyboard::isKeyPressed(keyBindings.console))
			//console->show();

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


/* TODO: remake with interface i/o
///////////////////////////////////////
// -----------  CONSOLE  ----------- //
///////////////////////////////////////

/*void ControlBox::Console::listenCommand()
{
	std::string command;
	do {
		console >> command;
		if (command == "create") {
			string object;
			cin >> object;

			if (object == "house") {
				string name;
				cin >> name;
				string houseTexture;
				cin >> houseTexture;
				string doorTexture;
				cin >> doorTexture;

				//ObjectsFactory::createObject(ObjectsFactory::objectType::House, name, houseTexture, doorTexture);
			}
			else if (object == "fir_tree") {
				string name;
				cin >> name;
				string texture;
				cin >> texture;

				//ObjectsFactory::createObject(ObjectsFactory::objectType::Fir_tree, name, texture);
			}
			else if (object == "shrub") {
				string name;
				cin >> name;
				string texture;
				cin >> texture;

				//ObjectsFactory::createObject(ObjectsFactory::objectType::Shrub, name, texture);
			}
			else if (object == "person") {
				string name;
				cin >> name;
				string texture;
				cin >> texture;

				//ObjectsFactory::createObject(ObjectsFactory::objectType::Person, name, texture);
			}
			else if (object == "player") {
				string name;
				cin >> name;
				string texture;
				cin >> texture;

				//ObjectsFactory::createObject(ObjectsFactory::objectType::Player, name, texture);
			}
			else {
				command = "UNKNOWN";
				cout << "Unknown object type, use 'usage [command]' to see available args" << endl;
			}
		}
		else if (command == "delete") {
			// delete object
		}
		else if (command == "camera") {
			// turn on camera mode
		}
		else if (command == "dim") {
			string dimension;
			cin >> dimension;

			// TP to dim
		}
		else if (command == "editor") {
			//Editor::launchEditor();
		}
		else if (command == "cls") {
			clear();
			command = "UNKNOWN";
		}
		else if (command == "save_history") {
			saveConsole();
			this << "Saved!";
			command = "UNKNOWN";
		}
		else if (command == "usage") {
			string usageCommand;
			cin >> usageCommand;

			if (usageCommand == "create")
				cout << "Using 'create [objectType] [objectName] [argument1]...'" << endl;
			else if (usageCommand == "delete")
				cout << "Using 'delete [objectName]'" << endl;
			else if (usageCommand == "camera")
				cout << "Using 'camera' without args" << endl;
			else if (usageCommand == "dim")
				cout << "Using 'dim' without args" << endl;
			else if (usageCommand == "editor")
				cout << "Using 'editor' without args" << endl;
			else if (usageCommand == "cls")
				cout << "Using 'cls' without args" << endl;
		}
		else if (command == "help") {
			cout << "Available commands: create delete camera dim editor cls usage help";
		}
		else {
			command = "UNKNOWN";
			cout << "Unknown command, use 'help' to see available commands" << endl;
		}
	} while (command == "UNKNOWN");
}

template<class T>
ControlBox::Console &ControlBox::Console::operator>>(std::string &in)
{
	char tmp;
	do {
		if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Return))
		render(tmp);
	} while ((event.type =! sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Return));
	return *this;
}

template<class T>
void spl::ControlBox::Console::render(T &in)
{
}

template<class T>
ControlBox::Console &ControlBox::Console::operator<<(T &in)
{
	history += in;
	if (State::current_state == "console") {
		render(in);
	}
	return *this;
}

void ControlBox::Console::hide()
{
	//TODO: Game continue && hide all interface and icon
}

void ControlBox::Console::show()
{
	renderAll();
	//TODO: Game pause && pause icon (in right top corner)
	//listenCommand();
}

void spl::ControlBox::Console::renderAll()
{
	//TODO: render console interface in left top corner with death fields && render all history, last symbol is '_'
}

void ControlBox::Console::saveHistory()
{
	std::ofstream file("consoleLog.txt", std::ios_base::ate);

	file << spl::Time::getTime(spl::Time::day_HourMinSec) << history << std::endl;

	file.close();
}

void ControlBox::Console::clear()
{
	history.clear();
}*/