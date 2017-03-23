#include "window.h"
#include "human.h"
#include "log.h"
#include <SFML/Graphics.hpp>
#include "phisic.h"
#include <iostream>
using namespace std;

int main() {

	b2Vec2 Gravity(0.f, 0.f);
	b2World World(Gravity);

	CollideListener collideListener;
	World.SetContactListener(&collideListener);


	Log::clear();
	Log::log("____________LOG SESSION START__________", true);

	CollideListener collideListener;
	World.SetContactListener(&collideListener);

	ScreenLog screenLog;
	screenLog.setNewLog("FPS", 1, 0);
	screenLog.setNewLog("collision h1", 2, 1);
	screenLog.setNewLog("collision h2", 3, 2);
	spl::Window window;


	Human h(sf::Vector2f(250, 250), "image/human.png","human1",  &World);
	Human h2(sf::Vector2f(200, 100), "image/human.png","human2",  &World);
	Human player(sf::Vector2f(150, 150), "image/player.png","player", &World);
	player.depthRender = -10;

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
			//window.view.setCenter(sf::Vector2f(player.body_ph->getPosition().x, player.body_ph->getPosition().x));
			player.body_ph->ApplyForceToCenter(b2Vec2(0, -speed), true);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
			//window.view.move(sf::Vector2f(0, 1));
			player.body_ph->ApplyForceToCenter(b2Vec2(0, speed), true);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
			//window.view.move(sf::Vector2f(1, 0));
			player.body_ph->ApplyForceToCenter(b2Vec2(speed, 0), true);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
			//window.view.move(sf::Vector2f(-1, 0));
			player.body_ph->ApplyForceToCenter(b2Vec2(-speed, 0), true);

		}
		window.view.setCenter(sf::Vector2f(player.body_ph->GetPosition().x*SCALE_BOX, player.body_ph->GetPosition().y*SCALE_BOX));
		/////////////// FPS //////////////////
		if (i > 10) {
			ScreenLog::setValue(1, to_string(1.f / clock.getElapsedTime().asSeconds()));
			i = 0;
		}else { i++; }
		clock.restart();
		///////////////////////////////

		World.Step(1 / 60.f, 8, 3);
		window.updateState();

		h.blit();
		h2.blit();
		player.blit();
		screenLog.blit();
		window.drawAll();

	}
	Log::log("____________LOG SESSION END____________", true);
	return 0;
}
