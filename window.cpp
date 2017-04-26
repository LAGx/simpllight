#include <SFML/Graphics.hpp>
#include "state.h"
#include "window.h"
#include <iostream>
#include <algorithm>
#include <vector>
#include <Windows.h>

using namespace spl;

Window::Window() {
#ifdef GAME_MODE
	FreeConsole();
	canvas.create(sf::VideoMode().getDesktopMode(), "simpllight", sf::Style::None);
	view.setSize(sf::VideoMode().getDesktopMode().width, sf::VideoMode().getDesktopMode().height);
	view.setCenter(sf::VideoMode().getDesktopMode().width / 2, sf::VideoMode().getDesktopMode().height / 2);
#else
	canvas.create(sf::VideoMode(900, 500), "simpllight", sf::Style::Default);
	view.setSize(900, 500);
	view.setCenter(450, 250);
#endif
	updateWindowStateBox();
	canvas.setMouseCursorVisible(true);
	canvas.setFramerateLimit(120);
	canvas.setView(view);
}



void Window::drawAll() {
	canvas.setView(view);
	canvas.clear(State::mainColorMajor);

	sort(allDrawable.begin(), allDrawable.end(), [](const ToDraw& a, const ToDraw& b) {
		return a.depth > b.depth;
	});

	for (int i = 0; i < allDrawable.size(); i++) {
		canvas.draw(*allDrawable[i].image);
	}

	canvas.display();
	allDrawable.clear();
}

Window::~Window() {

}


sf::RenderWindow Window::canvas;
sf::View Window::view;


std::vector<ToDraw> Window::allDrawable;

sf::Vector2f WindowStateBox::currScreenSize;
sf::Vector2f WindowStateBox::currGlobalViewCord;
sf::Vector2f WindowStateBox::currViewSize;
float WindowStateBox::absoluteScale;

sf::Vector2f WindowStateBox::inGameZeroCordRelativeWindow;
sf::Vector2i WindowStateBox::mouseCurrPositionRelativeWindow;

void Window::updateWindowStateBox(){
	WindowStateBox::mouseCurrPositionRelativeWindow = sf::Mouse::getPosition(canvas);
	WindowStateBox::currScreenSize = sf::Vector2f(canvas.getSize());
	WindowStateBox::currGlobalViewCord = view.getCenter();
	WindowStateBox::currViewSize = view.getSize();
	WindowStateBox::absoluteScale = WindowStateBox::currScreenSize.x / WindowStateBox::currViewSize.x;

	WindowStateBox::inGameZeroCordRelativeWindow = WindowStateBox::currGlobalViewCord - (WindowStateBox::currViewSize / (float)2);
}
