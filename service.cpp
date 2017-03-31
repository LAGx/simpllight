#include "service.h"
#include <iostream>
#include <random>

unsigned int Rand::intRand(int min, int max) {
	std::mt19937 rng;
	rng.seed(std::random_device()());
	std::uniform_int_distribution<std::mt19937::result_type> dist6(min, max);
	return (int)dist6(rng);
}