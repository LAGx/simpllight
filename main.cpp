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


	UserInterfaceBox user_interface;
	///example to create file (need to use once)

	//user_interface.createLayer("text_test");
	//user_interface.getCurrLayer()->createNewCell(sf::Vector2f(30, 40), sf::Vector2f(20, 15), CellInterface::rect, "main");
	//user_interface.getCurrLayer()->textControl("main", "new", 1, "some_text", sf::Vector2f(0, 0), 20);

	user_interface.downloadLayerById("text_test");
	user_interface.getCurrLayer()->getCellById("main")->getTextPtr(1)->setPositionRatioCell(sf::Vector2f(-user_interface.getCurrLayer()->getCellById("main")->getTextPtr(1)->getRatioCellSize().x/2,0));
	

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
		user_interface.blit();///< have to be after updateWindowStateBox
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