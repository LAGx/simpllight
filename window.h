#ifndef _WINDOW_
#define _WINDOW_

#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

namespace spl {

	struct ToDraw {
		sf::Drawable *image;
		int depth = 0;
	};

	class WindowStateBox;

	class Window {
	private:

		friend class WindowStateBox;
	
	public:


		sf::RenderWindow canvas;
		sf::View view;

		static std::vector<ToDraw> allDrawable;

		Window();

		void drawAll();

		~Window();
	};


	class WindowStateBox {
	private:
		static void update();
		
	public:

		static void updateWindowStateBox(Window& window);

		static sf::Vector2f currScreenSize;
		static sf::Vector2f currGlobalViewCord;
		static sf::Vector2f currViewSize;
		static float absoluteScale;		//increase of gameObject on screen in absoluteScale times

		static sf::Vector2f inGameZeroCordRelativeWindow;
		
	};
}
#endif

