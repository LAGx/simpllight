#include "window.h"
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
	screenLog.setNewLog("FPS", 0);
	screenLog.setNewLog("collision h1", 1);
	screenLog.setNewLog("collision h2", 2);
	screenLog.setNewLog("Mouse: ", 3);
	spl::Window window;

 
    Person h(&World, sf::Vector2f(605, 190), "image/human/human1.png","human1");
	Person h2(&World, sf::Vector2f(520, 354), "image/human/human2.png","human2");
	Player player(&World, sf::Vector2f(370, 235), "image/human/player.png","player");

	House house(&World, sf::Vector2f(150, 100), -120,"image/house/house2.png", "image/house/door2.png", "house");
	House house1(&World, sf::Vector2f(300, 400), 30, "image/house/house.png", "image/house/door.png", "house1");
	House house2(&World, sf::Vector2f(500, 100), 120, "image/house/house.png", "image/house/door.png", "house2");

	Fir_tree y1(&World, sf::Vector2f(350, 175), "image/veget/tree.png", "y1");
	Fir_tree y2(&World, sf::Vector2f(127, 419), "image/veget/tree.png", "y2");
	Fir_tree y3(&World, sf::Vector2f(107, 316), "image/veget/tree.png", "y3");

	Shrub s1(&World, sf::Vector2f(207, 250), "image/veget/shrub.png", "s1");
	Shrub s2(&World, sf::Vector2f(397, 324), "image/veget/shrub2.png", "s2");
	Shrub s3(&World, sf::Vector2f(492, 290), "image/veget/shrub2.png", "s3");

	int i = 0;
	sf::Clock clock;
	float speed = 2;
	while (window.canvas.isOpen()){
		sf::Event event;
		

		while (window.canvas.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.canvas.close();
		}
		
		int w = sf::Keyboard::W;
		if (sf::Keyboard::isKeyPressed(w)) {
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
		

		window.view.setCenter(sf::Vector2f(player.body_ph->GetPosition().x*SCALE_BOX, player.body_ph->GetPosition().y*SCALE_BOX));
		/////////////// FPS //////////////////
		if (i > 10) {
			ScreenLog::setValue(0, to_string(int(1.f / clock.getElapsedTime().asSeconds())));
			i = 0;
		}
		else
			i++;
		clock.restart();
		///////////////////////////////

		ScreenLog::setValue(3, to_string(sf::Mouse::getPosition(window.canvas).x) + " | " + to_string(sf::Mouse::getPosition(window.canvas).y));

		World.Step(1 / 60.f, 8, 3);
		window.updateState();
		house.blit();
		house1.blit();
		house2.blit();
		y1.blit();
		y2.blit();
		y3.blit();
		h.blit();
		h2.blit();
		s1.blit();
		s2.blit();
		s3.blit();
		player.blit();
		screenLog.blit();
		window.drawAll();
}
	Log::log("____________LOG SESSION END____________", true);
	return 0;
}
