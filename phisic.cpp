#include "phisic.h"
#include "log.h"
#include "game_objects.h"
#include "state.h"

void CollideListener::BeginContact(b2Contact* contact) {
	void *dataA = contact->GetFixtureA()->GetBody()->GetUserData();
	void *dataB = contact->GetFixtureB()->GetBody()->GetUserData();
#ifdef DEV_MODE
	if (((static_cast<Human*>(dataA)->name == "player") && (static_cast<Human*>(dataB)->name == "human1")) || ((static_cast<Human*>(dataB)->name == "player") && (static_cast<Human*>(dataA)->name == "human1")))
		ScreenLog::setValue(1, "YES");

	if (((static_cast<Human*>(dataA)->name == "player") && (static_cast<Human*>(dataB)->name == "human2")) || ((static_cast<Human*>(dataB)->name == "player") && (static_cast<Human*>(dataA)->name == "human2")))
		ScreenLog::setValue(2, "YES");
#endif
}

void CollideListener::EndContact(b2Contact* contact) {
	void *dataA = contact->GetFixtureA()->GetBody()->GetUserData();
	void *dataB = contact->GetFixtureB()->GetBody()->GetUserData();
#ifdef DEV_MODE
	if (((static_cast<Human*>(dataA)->name == "player") && (static_cast<Human*>(dataB)->name == "human1")) || ((static_cast<Human*>(dataB)->name == "player") && (static_cast<Human*>(dataA)->name == "human1")))
		ScreenLog::setValue(1, "NO");

	if (((static_cast<Human*>(dataA)->name == "player") && (static_cast<Human*>(dataB)->name == "human2")) || ((static_cast<Human*>(dataB)->name == "player") && (static_cast<Human*>(dataA)->name == "human2")))
		ScreenLog::setValue(2, "NO");
#endif
}