#include "game_objects.h"
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>

using namespace std;
using namespace sf;

Human::Human(b2World* World, Vector2f initCord, string textr, string name) :Alive(World, initCord, textr, name, rect_T, 10) {
	body_ph->SetLinearDamping(3);
	body_ph->SetAngularDamping(1);
} 