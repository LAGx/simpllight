#include <iostream>
#include <random>
#include <string>
#include <ctime>

#include "service.h"
#include "log.h"

///////////////////////////////////////
// -------------  RAND  ------------ //
///////////////////////////////////////

unsigned int spl::Rand::intRand(int min, int max) {
	std::mt19937 rng;
	rng.seed(std::random_device()());
	std::uniform_int_distribution<std::mt19937::result_type> dist6(min, max);
	return (int)dist6(rng);
}


///////////////////////////////////////
// -------------  TIME  ------------ //
///////////////////////////////////////

std::string spl::Time::getTime(TimeMode mode)
{
	switch (mode)
	{
	case spl::Time::day_HourMinSec:
		time_t rawtime;
		rawtime = time(0);
		struct tm timeinfo;
		localtime_s(&timeinfo, &rawtime);

		return std::to_string(timeinfo.tm_mday) + "|" + std::to_string(timeinfo.tm_hour) + ":" + std::to_string(timeinfo.tm_min) + ":" + std::to_string(timeinfo.tm_sec);
	default:
		throw Log::Exception("choise time time");
		break;
	}
}