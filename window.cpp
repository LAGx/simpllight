#include "window.h"

#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <algorithm>
#include <vector>
#include <Windows.h>

#include "state.h"

using Window = spl::Window;
using Vector2f = sf::Vector2f;

Window::Window() {
#ifdef GAME_MODE
	FreeConsole();
	canvas.create(sf::VideoMode().getDesktopMode(), "simpllight", sf::Style::Fullscreen); // Trying to fullscreen
	view.setSize(sf::VideoMode().getDesktopMode().width, sf::VideoMode().getDesktopMode().height);
	view.setCenter(sf::VideoMode().getDesktopMode().width / 2, sf::VideoMode().getDesktopMode().height / 2);
#else
	canvas.create(sf::VideoMode(900, 500), "simpllight", sf::Style::Default);
	view.setSize(900, 500);
	view.setCenter(450, 250);
#endif
	updateWindowStateBox();
	canvas.setMouseCursorVisible(false);
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

sf::Clock Window::clock;
sf::RenderWindow Window::canvas;
sf::View Window::view;

std::vector<spl::ToDraw> Window::allDrawable;

sf::Vector2f spl::WindowStateBox::currScreenSize;
sf::Vector2f spl::WindowStateBox::currGlobalViewCord;
sf::Vector2f spl::WindowStateBox::currViewSize;
float spl::WindowStateBox::absoluteScale;

sf::Vector2f spl::WindowStateBox::inGameZeroCordRelativeWindow;
sf::Vector2i spl::WindowStateBox::mouseCurrPositionRelativeWindow;

void Window::updateWindowStateBox(){
	spl::WindowStateBox::mouseCurrPositionRelativeWindow = sf::Mouse::getPosition(canvas);
	spl::WindowStateBox::currScreenSize = sf::Vector2f(canvas.getSize());
	spl::WindowStateBox::currGlobalViewCord = view.getCenter();
	spl::WindowStateBox::currViewSize = view.getSize();
	spl::WindowStateBox::absoluteScale = spl::WindowStateBox::currScreenSize.x / spl::WindowStateBox::currViewSize.x;

	spl::WindowStateBox::inGameZeroCordRelativeWindow = spl::WindowStateBox::currGlobalViewCord - (spl::WindowStateBox::currViewSize / (float)2);
}