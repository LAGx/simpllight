#include "control.h"

#include <SFML/Window.hpp>

#include "log.h"
#include "INIReader.h"
#include "INIWriter.h"
#include "service.h"
#include "state.h"

using ControlBox = spl::ControlBox;
using string = std::string;
using Folders = spl::Folders;


///////////////////////////////////////
// ---------  CONTROL_BOX  --------- //
///////////////////////////////////////

ControlBox::ControlBox() {
	updateKeyBindings();
}

void ControlBox::updateKeyBindings() {
	INIReader settings(Folders::getSpecialFolderPath(Folders::myDocuments) + "\\simpllight\\settings.ini");

	if (settings.ParseError() < 0) {
		Log::warning("Can't load 'settings.ini', creating and using default bindings");
		saveKeyBindings();
	}
	else {
		keyBindings.moveTop = static_cast<sf::Keyboard::Key>(settings.GetInteger("keyBindings", "moveTop", -1));
		keyBindings.moveBottom = static_cast<sf::Keyboard::Key>(settings.GetInteger("keyBindings", "moveBottom", -1));
		keyBindings.moveLeft = static_cast<sf::Keyboard::Key>(settings.GetInteger("keyBindings", "moveLeft", -1));
		keyBindings.moveRight = static_cast<sf::Keyboard::Key>(settings.GetInteger("keyBindings", "moveRight", -1));
		keyBindings.haste = static_cast<sf::Keyboard::Key>(settings.GetInteger("keyBindings", "haste", -1));
		keyBindings.apply = static_cast<sf::Keyboard::Key>(settings.GetInteger("keyBindings", "apply", -1));
		keyBindings.console = static_cast<sf::Keyboard::Key>(settings.GetInteger("keyBindings", "console", -1));
		keyBindings.l_ctrl = static_cast<sf::Keyboard::Key>(settings.GetInteger("keyBindings", "l_ctrl", -1));
		keyBindings.l_shift = static_cast<sf::Keyboard::Key>(settings.GetInteger("keyBindings", "l_shift", -1));
		keyBindings.primaryMouseAction = static_cast<sf::Mouse::Button>(settings.GetInteger("keyBindings", "primaryMouseAction", -1));
		keyBindings.secondaryMouseAction = static_cast<sf::Mouse::Button>(settings.GetInteger("keyBindings", "secondaryMouseAction", -1));
/*#ifdef DEV_MODE
		keyBindings.moveRadiusPlus = (sf::Keyboard::Key) settings.GetInteger("keyBindings", "moveRadiusPlus", -1);
		keyBindings.moveRadiusMinus = (sf::Keyboard::Key) settings.GetInteger("keyBindings", "moveRadiusMinus", -1);
#endif*/

		if (keyBindings.moveTop == -1 || keyBindings.moveBottom == -1 || keyBindings.moveLeft == -1 || keyBindings.moveRight == -1
			|| keyBindings.haste == -1 || keyBindings.apply == -1 || keyBindings.console == -1 || keyBindings.l_ctrl == -1 
			|| keyBindings.l_shift == -1 || keyBindings.primaryMouseAction == -1 || keyBindings.secondaryMouseAction == -1) 
		{
			Log::warning("Bad bindings, creating and using default bindings");
			saveKeyBindings();
		}
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
	settings["keyBindings"]["haste"] = keyBindings.haste;
	settings["keyBindings"]["l_ctrl"] = keyBindings.l_ctrl;
	settings["keyBindings"]["l_shift"] = keyBindings.l_shift;

	string path = Folders::getSpecialFolderPath(Folders::myDocuments);
	Folders::createFolder(path + "\\simpllight");
	settings.saveToFile(path + "\\simpllight\\settings.ini");

	Log::log("Saved 'settings.ini'");
}

void ControlBox::setControlObject(spl::EventInterface* obj) {
	controlObjects.push_back(obj);
}

void ControlBox::resulveControl(spl::Window &window) {
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
		//if (sf::Keyboard::isKeyPressed(keyBindings.console))
			//console->show();
		if (sf::Keyboard::isKeyPressed(keyBindings.haste))
			controlObjects[i]->haste();
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

bool ControlBox::deleteControlObject(spl::EventInterface* obj) {
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