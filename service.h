#ifndef _SERVICE_
#define _SERVICE_

#pragma once
#include <string>

namespace spl {

	class Rand {
	public:
		static unsigned int intRand(int by, int to); // [by, to]
	};

	class Time {
	public:

		enum TimeMode
		{
			day_HourMinSec = 0
		};

		static std::string getTime(TimeMode mode); //day|hour:min:sec
	};

}
#endif