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

		virtual void selectMouse() {} //mouse

		virtual void useMouse() {} //mouse

		virtual void wheelMouse(float delta) {} //mouse

		virtual void positionMouse(int x, int y) {} //mouse
	};



	class ControlBox {
	private:

		struct KeyBindings {
			sf::Keyboard::Key moveTop = sf::Keyboard::Unknown;
			sf::Keyboard::Key moveBottom = sf::Keyboard::Unknown;
			sf::Keyboard::Key moveLeft = sf::Keyboard::Unknown;
			sf::Keyboard::Key moveRight = sf::Keyboard::Unknown;

			sf::Mouse::Button primaryMouseAction = sf::Mouse::Button::Left;
			sf::Mouse::Button secondaryMouseAction = sf::Mouse::Button::Right;
		};

		std::vector<EventInterface*> controlObjects;

		KeyBindings keyBindings;

		sf::Event event;

	public:

		ControlBox();

		void updateKeyBindings();

		bool deleteControlObject(EventInterface*);

		void setControlObject(EventInterface*);

		void resulveControl(spl::Window &window);
	};
}
#endif