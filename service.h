#ifndef _SERVICE_
#define _SERVICE_

#pragma once
#include <string>
#include <SFML\Graphics.hpp>

namespace spl {

	class Rand {
	public:
		static unsigned int intRand(int by, int to); // [by, to]
	};

	class Time {
	public:

		enum class TimeMode
		{
			Day_HourMinSec = 0, //day|hour:min:sec
			Day_HourMin, //day|hour:min
			HourMinSec, // hour:min:sec
			HourMin //hour:min
		};

		static std::string getTime(TimeMode mode);
	};

	template<class T>
	T getLength(sf::Vector2<T> vec1, sf::Vector2<T> vec2 = { 0 , 0 });
}
#endif