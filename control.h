#ifndef _CONTROL_
#define _CONTROL_

#pragma once
#include <vector>
#include <string>
#include <SFML/Window.hpp>

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

		virtual void haste() {} //for player

		virtual void apply() {}

		virtual void primaryMouseAction() {} //mouse

		virtual void secondaryMouseAction() {} //mouse

		virtual void wheelMouse(float delta) {} //mouse

		virtual void positionMouse(int x, int y) {} //mouse

		virtual void l_ctrl() {} 

		virtual void l_shift() {}
	};



	class ControlBox {
	private:

		class Console {
		public:

			~Console() {
				history.clear();
			}

			void hide();

			void show();

			template<class T> 
			Console &operator<<(T &in);

		private:
			/// clear history
			void clear();

			/// saving available console history to file "consoleLog.txt"
			void saveHistory();

			template <class T> 
			Console &operator>>(std::string &in);

			std::string history;
		};

		struct KeyBindings {
			sf::Keyboard::Key moveTop = sf::Keyboard::W;
			sf::Keyboard::Key moveBottom = sf::Keyboard::S;
			sf::Keyboard::Key moveLeft = sf::Keyboard::A;
			sf::Keyboard::Key moveRight = sf::Keyboard::D;
			sf::Keyboard::Key haste = sf::Keyboard::LShift;
			sf::Keyboard::Key apply = sf::Keyboard::F;
			sf::Keyboard::Key console = sf::Keyboard::Tilde;
			sf::Keyboard::Key l_ctrl = sf::Keyboard::LControl; //Rename
			sf::Keyboard::Key l_shift = sf::Keyboard::LShift; //Rename

			sf::Mouse::Button primaryMouseAction = sf::Mouse::Button::Left;
			sf::Mouse::Button secondaryMouseAction = sf::Mouse::Button::Right;
		} keyBindings;

		std::vector<spl::EventInterface*> controlObjects;

		sf::Event event;

	public:

		Console console;

		ControlBox();

		void updateKeyBindings();

		void saveKeyBindings();

		bool deleteControlObject(spl::EventInterface*);

		void setControlObject(spl::EventInterface*);

		void resulveControl(spl::Window &window);
	};
}
#endif