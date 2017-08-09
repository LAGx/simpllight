#include <iostream>
#include <random>
#include <string>
#include <ctime>
#include <cmath>
#include <ShlObj.h>

#include "service.h"
#include "log.h"

///////////////////////////////////////
// -------------  Rand  ------------ //
///////////////////////////////////////

unsigned int spl::Rand::intRand(int min, int max) {
	std::mt19937 rng;
	rng.seed(std::random_device()());
	std::uniform_int_distribution<std::mt19937::result_type> dist6(min, max);
	return (int)dist6(rng);
}


///////////////////////////////////////
// -------------  Time  ------------ //
///////////////////////////////////////

std::string spl::Time::getTime(TimeMode mode)
{
	time_t rawtime;
	rawtime = time(0);
	tm timeinfo;
	localtime_s(&timeinfo, &rawtime);

	switch (mode)
	{
	case spl::Time::TimeMode::Day_HourMinSec:
		return std::to_string(timeinfo.tm_mday) + "|" + std::to_string(timeinfo.tm_hour) + ":" + std::to_string(timeinfo.tm_min) + ":" + std::to_string(timeinfo.tm_sec);
	
	case spl::Time::TimeMode::Day_HourMin:
		return std::to_string(timeinfo.tm_mday) + "|" + std::to_string(timeinfo.tm_hour) + ":" + std::to_string(timeinfo.tm_min);
	
	case spl::Time::TimeMode::HourMinSec:
		return std::to_string(timeinfo.tm_hour) + ":" + std::to_string(timeinfo.tm_min) + ":" + std::to_string(timeinfo.tm_sec);
	
	case spl::Time::TimeMode::HourMin:
		return std::to_string(timeinfo.tm_hour) + ":" + std::to_string(timeinfo.tm_min);
	}
}

///////////////////////////////////////
// -----------  Vectors  ----------- //
///////////////////////////////////////

template<class T>
T spl::getLength(sf::Vector2<T> vec1, sf::Vector2<T> vec2)
{
	return sqrt((vec2.x - vec1.x)*(vec2.x - vec1.x) + (vec2.y - vec1.y)*(vec2.y - vec1.y));
}

///////////////////////////////////////
// ------------  Paths  ------------ //
///////////////////////////////////////

std::string spl::getSpecialFolderPath(int csidl)
{
	LPITEMIDLIST pidl;
	LPMALLOC pShellMalloc;
	char szDir[MAX_PATH];
	std::string result("None");

	if (SUCCEEDED(SHGetMalloc(&pShellMalloc))) {
		if (SUCCEEDED(SHGetSpecialFolderLocation(NULL, csidl, &pidl))) {
			if (SHGetPathFromIDList(pidl, szDir)) {
				result = szDir;
			}
			pShellMalloc->Free(pidl);
		}
		pShellMalloc->Release();
	}

	if (result == "None")
		throw Log::Exception("getting special folder");
	else
		return result;
}

void spl::createFolder(std::string path)
{
	SHCreateDirectoryEx(NULL, path.c_str(), NULL);
}
