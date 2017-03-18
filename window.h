#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

namespace spl {

struct ToDraw {
	sf::Drawable *image;
	signed int depth = 0;
};

class Window {
	private:

	public:
		sf::RenderWindow canvas;
		sf::View view;

		static std::vector<ToDraw> allDrawable;

		Window();

		void drawAll();
		void updateState();

		static sf::Vector2f currGlobalViewCord;
		static sf::Vector2f currScreenSize;

		~Window();
	};

}