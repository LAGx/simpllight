#ifndef _CONTROL_
#define _CONTROL_

#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "state.h"

namespace spl {

	class EventInterface {

	public:

		virtual void moveTop() {} //for player

		virtual void moveBottom() {} //for player

		virtual void moveLeft() {} //for player

		virtual void moveRight() {} //for player

#ifdef DEV_MODE
		virtual void moveRadiusMinus() {} //for player

		virtual void moveRadiusPlus() {} //for player

		virtual void herna() {} //TEST
#endif
	};



	class ControlBox {
	private:

		struct KeyBindings {
			sf::Keyboard::Key moveTop = sf::Keyboard::Unknown;
			sf::Keyboard::Key moveBottom = sf::Keyboard::Unknown;
			sf::Keyboard::Key moveLeft = sf::Keyboard::Unknown;
			sf::Keyboard::Key moveRight = sf::Keyboard::Unknown;

			sf::Keyboard::Key moveRadiusMinus = sf::Keyboard::Unknown;
			sf::Keyboard::Key moveRadiusPlus = sf::Keyboard::Unknown;
		};

		std::vector<EventInterface*> controlObjects;

		KeyBindings keyBindings;

	public:

		ControlBox();
		
		void updateKeyBindings();
		
		bool deleteControlObject(EventInterface*);

		void setControlObject(EventInterface*);

		void resulveControl();
	};



}
#endif