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

		virtual void haste() {} //for player

		virtual void apply() {}

		virtual void primaryMouseAction() {} //mouse

		virtual void secondaryMouseAction() {} //mouse

		virtual void wheelMouse(float delta) {} //mouse

		virtual void positionMouse(int x, int y) {} //mouse
	};



	class ControlBox {
	private:

		/*class Console {
		public:

			#define cendl '\n'

			~Console() {
				history.clear();
			}

			void hide();

			void show();

			void renderAll();

			template<class T>
			void render(T &in);

			template<class T> 
			Console &operator<<(T &in);
		private:
			void clear();
			void saveHistory(); //Saving available console history to file "consoleLog.txt"
		//	void listenCommand();

			template <class T> 
			Console &operator>>(std::string &in);

			std::string history;
		};*/

		struct KeyBindings {
			sf::Keyboard::Key moveTop = sf::Keyboard::W;
			sf::Keyboard::Key moveBottom = sf::Keyboard::S;
			sf::Keyboard::Key moveLeft = sf::Keyboard::A;
			sf::Keyboard::Key moveRight = sf::Keyboard::D;
			sf::Keyboard::Key haste = sf::Keyboard::LShift;
			sf::Keyboard::Key apply = sf::Keyboard::F;
			sf::Keyboard::Key console = sf::Keyboard::Tilde;

			sf::Mouse::Button primaryMouseAction = sf::Mouse::Button::Left;
			sf::Mouse::Button secondaryMouseAction = sf::Mouse::Button::Right;
		} keyBindings;

		std::vector<spl::EventInterface*> controlObjects;

		sf::Event event;

	public:

		//static Console *console;

		ControlBox();

		void updateKeyBindings();

		void saveKeyBindings();

		bool deleteControlObject(spl::EventInterface*);

		void setControlObject(spl::EventInterface*);

		void resulveControl(spl::Window &window);
	};
}
#endif