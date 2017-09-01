#include "phisic.h"

#include <Box2D\BOX2D.H>

#include "game_objects.h"
#include "state.h"
#include "world.h"

void CollideListener::BeginContact(b2Contact* contact) {
	if (isLoad == 0) {
		void *dataA = contact->GetFixtureA()->GetBody()->GetUserData();
		void *dataB = contact->GetFixtureB()->GetBody()->GetUserData();
#ifdef DEV_MODE
		if ((static_cast<Human*>(dataA)->getTexturePath() == "image/human/human1.png") && (static_cast<Human*>(dataB)->getTexturePath() == "image/human/player.png")) {
			isLoad = 1;
		}

		if ((static_cast<Human*>(dataB)->getTexturePath() == "image/human/human1.png") && (static_cast<Human*>(dataA)->getTexturePath() == "image/human/player.png")) {
			isLoad = 1;
		}

		if ((static_cast<Human*>(dataA)->getTexturePath() == "image/human/human2.png") && (static_cast<Human*>(dataB)->getTexturePath() == "image/human/player.png")) {
			isLoad = 2;
		}

		if ((static_cast<Human*>(dataB)->getTexturePath() == "image/human/human2.png") && (static_cast<Human*>(dataA)->getTexturePath() == "image/human/player.png")) {
			isLoad = 2;
		}
#endif
	}
}

void CollideListener::EndContact(b2Contact* contact) {
/*	void *dataA = contact->GetFixtureA()->GetBody()->GetUserData();
	void *dataB = contact->GetFixtureB()->GetBody()->GetUserData();
#ifdef DEV_MODE
	if ((static_cast<Human*>(dataA)->getTexturePath() == "image/human/human1.png") && (static_cast<Human*>(dataB)->getTexturePath() == "image/human/player.png")) {
		isLoad = 0;
	}

	if ((static_cast<Human*>(dataB)->getTexturePath() == "image/human/human1.png") && (static_cast<Human*>(dataA)->getTexturePath() == "image/human/player.png")) {
		isLoad = 0;
	}

	if ((static_cast<Human*>(dataA)->getTexturePath() == "image/human/human2.png") && (static_cast<Human*>(dataB)->getTexturePath() == "image/human/player.png")) {
		isLoad = 0;
	}

	if ((static_cast<Human*>(dataB)->getTexturePath() == "image/human/human2.png") && (static_cast<Human*>(dataA)->getTexturePath() == "image/human/player.png")) {
		isLoad = 0;
	}
#endif*/
}