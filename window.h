#ifndef _WINDOW_
#define _WINDOW_

#pragma once
#include <SFML/Graphics.hpp>
#include <SFML\System\Vector2.hpp>
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

		static void updateWindowStateBox();

		static sf::RenderWindow canvas;
		static sf::View view;

		static std::vector<ToDraw> allDrawable;
		static sf::Clock clock;

		Window();

		void drawAll();

		~Window();
	};

	class WindowStateBox {
	private:

		
	public:
		static sf::Vector2f currScreenSize;
		static sf::Vector2f currGlobalViewCord;
		static sf::Vector2f currViewSize;
		static float absoluteScale;		//increase of gameObject on screen in absoluteScale times

		static sf::Vector2f inGameZeroCordRelativeWindow;
		static sf::Vector2i mouseCurrPositionRelativeWindow;//need updateing
	};
}
#endif

