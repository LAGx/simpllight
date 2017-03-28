#include "service.h"
#include <iostream>

unsigned int Rand::intRand(int minimum, int width) {
	return minimum + std::rand() % width;
}