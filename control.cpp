#include "control.h"
#include <SFML/Graphics.hpp>
#include "log.h"

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
	controlObgects.push_back(obj);
}

void ControlBox::resulveControl() {
for (int i = 0; i < controlObgects.size(); i++) {


	if (sf::Keyboard::isKeyPressed(keyBindings.moveTop))
		controlObgects[i]->moveTop();
	if (sf::Keyboard::isKeyPressed(keyBindings.moveBottom))
		controlObgects[i]->moveBottom();
	if (sf::Keyboard::isKeyPressed(keyBindings.moveLeft))
		controlObgects[i]->moveLeft();
	if (sf::Keyboard::isKeyPressed(keyBindings.moveRight))
		controlObgects[i]->moveRight();
	if (sf::Keyboard::isKeyPressed(keyBindings.moveRadiusPlus))
		controlObgects[i]->moveRadiusPlus();
	if (sf::Keyboard::isKeyPressed(keyBindings.moveRadiusMinus))
		controlObgects[i]->moveRadiusMinus();

}
}


void ControlBox::deleteControlObject(EventInterface* obj) {
	for (int i = 0; i < controlObgects.size(); i++) {

		if (controlObgects[i] == obj) {
			controlObgects.erase(controlObgects.begin()+i);
			goto end;
		}
	}

	Log::log("No such object in Control to remove.", true);
end:;
}



