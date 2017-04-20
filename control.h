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

		virtual void moveRadiusMinus() {} //for player

		virtual void moveRadiusPlus() {} //for player
		
		virtual void selectMouse() {} //mouse

		virtual void useMouse() {} //mouse

		virtual void wheelMouse(float delta) {} //mouse

		virtual void positionMouse(int x, int y) {} //mouse

		virtual void l_ctrl() {} 

		virtual void l_shift() {}
	};



	class ControlBox {
	private:

		struct KeyBindings {
			sf::Keyboard::Key moveTop = sf::Keyboard::Unknown;
			sf::Keyboard::Key moveBottom = sf::Keyboard::Unknown;
			sf::Keyboard::Key moveLeft = sf::Keyboard::Unknown;
			sf::Keyboard::Key moveRight = sf::Keyboard::Unknown;

			sf::Keyboard::Key l_ctrl = sf::Keyboard::Unknown;
			sf::Keyboard::Key l_shift = sf::Keyboard::Unknown;

			sf::Keyboard::Key moveRadiusMinus = sf::Keyboard::Unknown;
			sf::Keyboard::Key moveRadiusPlus = sf::Keyboard::Unknown;

			sf::Mouse::Button selectMouse = sf::Mouse::Button::Left;
			sf::Mouse::Button useMouse = sf::Mouse::Button::Right;
		};

		std::vector<EventInterface*> controlObjects;

		KeyBindings keyBindings;

		sf::Event event;

	public:

		ControlBox();
		
		void updateKeyBindings();//for xml setting key
		
		bool deleteControlObject(EventInterface*);

		void setControlObject(EventInterface*);

		void resulveControl(spl::Window &window);
	};



}
#endif