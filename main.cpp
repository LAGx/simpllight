#include <SFML/Graphics.hpp>
#include <iostream>

#include "window.h"
#include "log.h"
#include "phisic.h"
#include "game_objects.h"
#include "state.h"
#include "world.h"

using namespace std;

int main() {

	Log::clear();
	Log::log("____________LOG SESSION START__________", true);

	spl::Window window;

#ifdef DEV_MODE
	ScreenLog screenLog;
	screenLog.setNewLog("FPS", 0);
	screenLog.setNewLog("collision h1", 1);
	screenLog.setNewLog("collision h2", 2);
	screenLog.setNewLog("Mouse", 3);
#endif

	World world(&window);
	world.loadLocation("world/testLoc"); //!!!!

#ifdef DEV_MODE
	int i = 0;
	sf::Clock clock;
#endif


	/////////////////////////////////////
	// ---------- MAIN LOOP ---------- //
	/////////////////////////////////////

	while (window.canvas.isOpen()) {
		// --- FPS --- //
	#ifdef DEV_MODE
		if (i > 15) {
			ScreenLog::setValue(0, to_string((int)(1.f / clock.getElapsedTime().asSeconds())));
			i = 0;
		}
		else {
			i++;
		}
		clock.restart();
	#endif

		world.blit();
		window.updateState();
#ifdef DEV_MODE
		screenLog.blit();
#endif

		window.drawAll();
	}

	Log::log("____________LOG SESSION END____________", true);
	return 0;
}
