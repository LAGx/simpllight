#include <SFML/System/Clock.hpp>
#include <string>

#include "window.h"
#include "log.h"
#include "state.h"
#include "world.h"

int main() {

	Log::startSession();

	spl::Window window;

#ifdef DEV_MODE
	ScreenLog screenLog;
	screenLog.setNewLog("FPS", 0);
	screenLog.setNewLog("collision h1", 1);
	screenLog.setNewLog("collision h2", 2);
	screenLog.setNewLog("Mouse", 3);
#endif

#ifdef DEV_MODE
	unsigned short i = 0;
	sf::Clock clock;
#endif


	/////////////////////////////////////
	// ---------- MAIN LOOP ---------- //
	/////////////////////////////////////

	while (window.canvas.isOpen()) {
		// --- FPS --- //
	#ifdef DEV_MODE
		if (i > 15) {
			ScreenLog::setValue(0, std::to_string((int)(1.f / clock.getElapsedTime().asSeconds())));
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


	Log::finishSession();
	return 0;
}
