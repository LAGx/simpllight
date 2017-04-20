#include <SFML/Graphics.hpp>
#include <iostream>

#include "window.h"
#include "log.h"
#include "phisic.h"
#include "game_objects.h"
#include "state.h"
using namespace std;

int main() {

	Log::clear();
	Log::log("____________LOG SESSION START__________", true);

	spl::Window window;

	b2Vec2 Gravity(0.f, 0.f);
	b2World World(Gravity);

	CollideListener collideListener;
	World.SetContactListener(&collideListener);

	spl::ControlBox controlBox;

#ifdef DEV_MODE
	ScreenLog screenLog;
	screenLog.setNewLog("FPS", 0);
	screenLog.setNewLog("collision h1", 1);
	screenLog.setNewLog("collision h2", 2);
	screenLog.setNewLog("Mouse", 3);
	screenLog.setNewLog("Tests", 4);
#endif

 
    Person h(&World, sf::Vector2f(605, 190), "image/human/human1.png","human1");
	Person h2(&World, sf::Vector2f(520, 354), "image/human/human2.png","human2");
	//Player player(&World, sf::Vector2f(370, 235), "image/human/player.png","player", "image/cursor.png");
	//controlBox.setControlObject(&player);
	//controlBox.setControlObject(player.cursor);
	Editor edit(&World,window.view ,"image/cursor.png");
	controlBox.setControlObject(&edit);
	controlBox.setControlObject(edit.cursor);

	House house(&World, sf::Vector2f(150, 100), -120,"image/house/house2.png", "image/house/door2.png", "house");
	House house1(&World, sf::Vector2f(300, 400), 30, "image/house/house.png", "image/house/door.png", "house1");
	House house2(&World, sf::Vector2f(500, 100), 120, "image/house/house.png", "image/house/door.png", "house2");
	Fir_tree y1(&World, sf::Vector2f(350, 175), "image/veget/tree.png", "y1");
	Fir_tree y2(&World, sf::Vector2f(127, 419), "image/veget/tree.png", "y2");
	Fir_tree y3(&World, sf::Vector2f(107, 316), "image/veget/tree.png", "y3");

	Shrub s1(&World, sf::Vector2f(207, 250), "image/veget/shrub.png", "sh1");
	Shrub s2(&World, sf::Vector2f(397, 324), "image/veget/shrub2.png", "sh2");
	Shrub s3(&World, sf::Vector2f(492, 290), "image/veget/shrub2.png", "sh3");

#ifdef DEV_MODE
	int i = 0;
	sf::Clock clock;
#endif
while (window.canvas.isOpen()){

		//window.view.setCenter(sf::Vector2f(player.body_ph->GetPosition().x*SCALE_BOX, player.body_ph->GetPosition().y*SCALE_BOX));

#ifdef DEV_MODE
		/////////////// FPS //////////////////
		if (i > 15) {
			ScreenLog::setValue(0, to_string((int) (1.f / clock.getElapsedTime().asSeconds() )));
			i = 0;
		}
		else 
			i++;
		clock.restart();
		///////////////////////////////
	//	ScreenLog::setValue(3, to_string(sf::Mouse::getPosition().x) + " | " + to_string(sf::Mouse::getPosition().y));
#endif


		controlBox.resulveControl(window);	
		World.Step(1 / 60.f, 8, 3); 
	
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
		edit.blit();
		//player.blit();	
#ifdef DEV_MODE
		screenLog.blit();
#endif	
		window.drawAll();

}

	Log::log("____________LOG SESSION END____________", true);
	return 0;
}