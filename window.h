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

	class Window {
	public:
		sf::RenderWindow canvas;
		sf::View view;

		static std::vector<ToDraw> allDrawable;

		Window();

		void drawAll();
		void updateState();

		static sf::Vector2f currGlobalViewCord;
		static sf::Vector2f screenSize;

		~Window();
	};
}
#endif