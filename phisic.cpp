#include "phisic.h"
#include <iostream>
#include "log.h"

using namespace std;

void CollideListener::BeginContact(b2Contact* contact) {
//	if ((contact->GetFixtureA()->GetBody()->GetUserData() == "player") &&
//		(contact->GetFixtureB()->GetBody()->GetUserData() == "human1"))
	//	ScreenLog::setValue(2, "START");
}

void CollideListener::EndContact(b2Contact* contact) {
//	if ((contact->GetFixtureA()->GetBody()->GetUserData() == "player") &&
	//	(contact->GetFixtureB()->GetBody()->GetUserData() == "human1"))
	///	ScreenLog::setValue(2, "END");
}