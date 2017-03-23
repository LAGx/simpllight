#include <SFML/Graphics.hpp>
#include "state.h"
#include "window.h"
#include "log.h"
#include <iostream>
#include <algorithm>
#include <vector>
#include <Windows.h>

using namespace spl;

Window::Window() {
	if (State::mode == "dev") {
		canvas.create(sf::VideoMode(900, 500), "simpllight", sf::Style::Default);
		view.setSize(900, 500);
		view.setCenter(450, 250);
	}
	else {
		FreeConsole();
		canvas.create(sf::VideoMode().getDesktopMode(), "simpllight", sf::Style::None);
		view.setSize(sf::VideoMode().getDesktopMode().width, sf::VideoMode().getDesktopMode().height);
		view.setCenter(sf::VideoMode().getDesktopMode().width/2, sf::VideoMode().getDesktopMode().height/2);
	}
	canvas.setFramerateLimit(60);
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

std::vector<ToDraw> Window::allDrawable;

void Window::updateState() {
	currGlobalViewCord = view.getCenter();
	currScreenSize = sf::Vector2f(canvas.getSize());
}

sf::Vector2f Window::currGlobalViewCord;
sf::Vector2f Window::currScreenSize;