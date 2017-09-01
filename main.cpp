#include <SFML/System/Clock.hpp>
#include <string>

#include "window.h"
#include "log.h"
#include "state.h"
#include "service.h"
#include "interface.h"
#include "item.h"
#include "world.h"

using namespace std;

int main() {

	Log::startSession();

	spl::Window window;

	//Editor edit(&World, window.view, "image/cursor.png");

#ifdef DEV_MODE
	ScreenLog screenLog;
	screenLog.setNewLog("FPS", 0);
	screenLog.setNewLog("Mouse", 1);
	screenLog.setNewLog("Location", 2);
#endif

#ifdef DEV_MODE
	unsigned short i = 0;
	sf::Clock clock;
#endif

	World world(&window);
	world.loadWorld();

	/////////////////////////////////////
	// ---------- MAIN LOOP ---------- //
	/////////////////////////////////////

	while (window.canvas.isOpen()) {
		// --- FPS --- //
#ifdef DEV_MODE
		if (i > 15) {
			ScreenLog::setValue(0, std::to_string(static_cast<int>(1.f / clock.getElapsedTime().asSeconds())));
			i = 0;
		}
		else {
			i++;
		}
		clock.restart();
#endif

		world.blit();
		window.updateWindowStateBox();
#ifdef DEV_MODE
		screenLog.blit();
#endif

		window.drawAll();
	}

	world.saveWorld("testSave");
	world.closeWorld();

	Log::finishSession();
	return 0;
}