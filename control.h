#ifndef _CONTROL_
#define _CONTROL_

#pragma once
#include <vector>
#include <SFML/Graphics.hpp>

#include "state.h"
#include "window.h"


namespace spl {

	class EventInterface {

	public:

		virtual void moveTop() {} //for player

		virtual void moveBottom() {} //for player

		virtual void moveLeft() {} //for player

		virtual void moveRight() {} //for player

		virtual void primaryMouseAction() {} //mouse

		virtual void secondaryMouseAction() {} //mouse

		virtual void wheelMouse(float delta) {} //mouse

		virtual void positionMouse(int x, int y) {} //mouse
	};



	class ControlBox {
	private:

		struct KeyBindings {
			sf::Keyboard::Key moveTop = sf::Keyboard::W;
			sf::Keyboard::Key moveBottom = sf::Keyboard::S;
			sf::Keyboard::Key moveLeft = sf::Keyboard::A;
			sf::Keyboard::Key moveRight = sf::Keyboard::D;

			sf::Mouse::Button primaryMouseAction = sf::Mouse::Button::Left;
			sf::Mouse::Button secondaryMouseAction = sf::Mouse::Button::Right;
			sf::Mouse::Wheel verticalMouseWheel = sf::Mouse::Wheel::VerticalWheel;
			sf::Mouse::Wheel horizontalMouseWheel = sf::Mouse::Wheel::HorizontalWheel;
		} keyBindings;

		std::vector<EventInterface*> controlObjects;

		sf::Event event;

	public:

		ControlBox();

		void updateKeyBindings();

		void saveKeyBindings();

		bool deleteControlObject(EventInterface*);

		void setControlObject(EventInterface*);

		void resulveControl(spl::Window &window);
	};
}
#endif