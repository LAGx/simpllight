#include "window.h"
#include "human.h"
#include "log.h"
#include <SFML/Graphics.hpp>
#include "phisic.h"
#include <iostream>
#include "game_objects.h"
using namespace std;

int main() {

	b2Vec2 Gravity(0.f, 0.f);
	b2World World(Gravity);

	CollideListener collideListener;
	World.SetContactListener(&collideListener);


	Log::clear();
	Log::log("____________LOG SESSION START__________", true);

	ScreenLog screenLog;
	screenLog.setNewLog("FPS", 1, 0);
	screenLog.setNewLog("collision h1", 2, 1);
	screenLog.setNewLog("collision h2", 3, 2);
	screenLog.setNewLog("Mouse: ", 4, 3);
	spl::Window window;


	Human h(sf::Vector2f(50, 70), "image/human.png","human1",  &World);
	Human h2(sf::Vector2f(200, 100), "image/human.png","human2",  &World);
	Human player(sf::Vector2f(100, 100), "image/player.png","player", &World);
	player.depthRender = -10;

	DynamicObject obj(&World, sf::Vector2f(200, 200), "image/human2.png", "obj", intnugon_T,16);
	
	int i = 0;
	sf::Clock clock;

	float speed = 3;
	while (window.canvas.isOpen()){
		sf::Event event;
		

		while (window.canvas.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.canvas.close();
		}
		
		
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
			player.body_ph->ApplyForceToCenter(b2Vec2(0, -speed), true);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
			player.body_ph->ApplyForceToCenter(b2Vec2(0, speed), true);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
			player.body_ph->ApplyForceToCenter(b2Vec2(speed, 0), true);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
			player.body_ph->ApplyForceToCenter(b2Vec2(-speed, 0), true);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
			player.moveRadius(1);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
			player.moveRadius(-1);
		}

		//window.view.setCenter(sf::Vector2f(player.body_ph->GetPosition().x*SCALE_BOX, player.body_ph->GetPosition().y*SCALE_BOX));
		/////////////// FPS //////////////////
		if (i > 10) {
			ScreenLog::setValue(1, to_string(int(1.f / clock.getElapsedTime().asSeconds())));
			i = 0;
		}
		else
			i++;
		clock.restart();
		///////////////////////////////

		ScreenLog::setValue(4, to_string(sf::Mouse::getPosition(window.canvas).x) + " | " + to_string(sf::Mouse::getPosition(window.canvas).y));

		World.Step(1 / 60.f, 8, 3);
		window.updateState();
		obj.blit();
		h.blit();
		h2.blit();
		player.blit();
		screenLog.blit();
		window.drawAll();
}
	Log::log("____________LOG SESSION END____________", true);
	return 0;
}
