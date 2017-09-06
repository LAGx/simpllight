#include "service.h"

#include <random>
#include <string>
#include <windows.h>
#include <conio.h>
#include "log.h"
#include <SFML/Graphics.hpp>
#include <ctime>
#include <cmath>
#include <ShlObj.h>
#include "state.h"

using string = std::string;


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
// -------  TextureGenerator  ------ //
///////////////////////////////////////

void spl::TextureGenerator::fillShadowRect(int shadowSize, sf::Vector2i size, sf::Color color, std::string name) {
	sf::Image image;
	image.create(shadowSize * 2 + size.x, shadowSize * 2 + size.y, sf::Color(255,0,0,0)); //DEBUG change 155 to 0

	//fill base rect
	for (int i = shadowSize; i < size.x + shadowSize; i++) {//by x move
		for (int j = shadowSize; j < size.y + shadowSize; j++) {//by y move
			image.setPixel(i, j, color);
		}
	}

	int level = 0;int x = 0, y = 0;
	for (; level < shadowSize; level++) {
		sf::Color color(0, 0, 0, 200 * ((float(level - shadowSize) / shadowSize)));

		y = level;
		for (x = level; x < image.getSize().x - level; x++) {
			image.setPixel(x, y, color);
		}
		for (y = level; y < image.getSize().y - level; y++) {
			image.setPixel(x - 1, y, color);
		}

		for (x = image.getSize().x - level; x > level; x--) {
			image.setPixel(x - 1, y - 1, color);
		}
		for (y = image.getSize().y - level; y > level; y--) {
			image.setPixel(x , y-1, color);
		}
	}

	if (!image.saveToFile(name))
		throw Log::Exception("invalid way to save image rect " + name);
}

void spl::TextureGenerator::fillShadowCircle(int shadowSize, int radiusSize, sf::Color color, std::string name) {
	sf::Image image;
	image.create((shadowSize+ radiusSize)*2, (shadowSize + radiusSize) * 2, sf::Color(255, 0, 0, 0)); //DEBAG change 155 to 0

	int x = 0, y = 0; 
	float length= 0;

	for (x = 0; x < image.getSize().x; x++) {//by y move
		for (y = 0; y < image.getSize().y; y++) {//by x move
			length = getLength(sf::Vector2f(image.getSize().x/2, image.getSize().y / 2), sf::Vector2f(x,y));

			//draw
			if (length < radiusSize) {
				image.setPixel(x, y, color);
			}
			//shadow
			if ((length >= radiusSize) && (length < (image.getSize().y/2))) {
				image.setPixel(x, y, sf::Color(0, 0, 0, 220 * (1-(length-radiusSize)/(shadowSize))));

			}
			//blur
			if (length < radiusSize && length >= radiusSize -1) {
				image.setPixel(x, y, color - sf::Color(200, 200,200, 30));
			}
		}
	}


	if (!image.saveToFile(name))
		throw Log::Exception("invalid way to save image circle" + name);
}

void spl::TextureGenerator::conturRect(sf::Vector2i size, int lineWidth, sf::Color color, std::string name) {
	sf::Image image;
	image.create(size.x, size.y, sf::Color(255, 0, 0, 0)); //DEBAG change 155 to 0

	int level = 0; int x = 0, y = 0;
	for (; level < lineWidth; level++) {

		y = level;
		for (x = level; x < image.getSize().x - level; x++) {
			image.setPixel(x, y, color);
		}
		for (y = level; y < image.getSize().y - level; y++) {
			image.setPixel(x - 1, y, color);
		}

		for (x = image.getSize().x - level; x > level; x--) {
			image.setPixel(x - 1, y - 1, color);
		}
		for (y = image.getSize().y- level; y > level; y--) {
			image.setPixel(x, y-1 , color);
		}
	}

	if (!image.saveToFile(name))
		throw Log::Exception("invalid way to save image contur rect " + name);
}

void spl::TextureGenerator::conturCircle(int radius, int lineWidth, sf::Color color, std::string name) {
	sf::Image image;

	image.create(radius *2, radius *2, sf::Color(255, 0, 0, 0)); //DEBAG change 155 to 0

	int x = 0, y = 0;
	float length = 0;

	for (x = 0; x < image.getSize().x; x++) {//by y move
		for (y = 0; y < image.getSize().y; y++) {//by x move
			length = getLength(sf::Vector2f(image.getSize().x / 2, image.getSize().y / 2), sf::Vector2f(x, y));

			//blur
			if (length-1 < radius && length+1 >= radius  - lineWidth) {
				image.setPixel(x, y,color- sf::Color(0,0,0, 155));
			}
			//draw
			if (length < radius  && length >= radius -lineWidth) {
				image.setPixel(x, y, color);
			}
		}
	}


	if (!image.saveToFile(name))
		throw Log::Exception("invalid way to save image contur circle" + name);
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
// -----------  Folders  ----------- //
///////////////////////////////////////

std::string spl::Folders::getGameFolderPath(GameFolders folder)
{
	switch (folder)
	{
	case GameFolders::nativeWorld:
		return "world";
	case GameFolders::workWorld: 
		return getSpecialFolderPath(userName_applicationData) + "\\simpllight\\temp\\world";
	case GameFolders::savesWorlds: 
		return getSpecialFolderPath(myDocuments) + "\\simpllight\\saves";
	case GameFolders::nativeTextures:
		return "textures";
	case GameFolders::workTextures:
		return getSpecialFolderPath(userName_applicationData) + "\\simpllight\\temp\\textures";
	case GameFolders::sounds:
		return "sounds";
	}
}

std::string spl::Folders::getSpecialFolderPath(FoldersCSIDL csidl)
{
	LPITEMIDLIST pidl;
	LPMALLOC pShellMalloc;
	char szDir[MAX_PATH];
	string result("None");

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

void spl::Folders::createFolder(std::string path)
{
	SHCreateDirectoryEx(NULL, path.c_str(), NULL);
}

void spl::Folders::copyFolder(std::string from, std::string to)
{
#ifdef GAME_MODE
	string command = "\"xcopy " + from + ' ' + to + " /E /Y /Q\"";
	system(command.c_str());
#else
	string command = "\"xcopy " + from + ' ' + to + " /E /Y /F\"";
	system(command.c_str());
#endif
}

void spl::Folders::deleteFolder(std::string path)
{
	system(("\"rmdir /S /Q " + path + " \"").c_str());
}
