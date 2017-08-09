#ifndef _LOG_
#define _LOG_

#include <string>
#include <vector>
#include <fstream>
#include <SFML/Graphics.hpp>

class Log {
private:
	static std::string logName;

	static void error(std::string err, bool time = false);

public:

	class Exception {
	public:
		Exception(std::string error, bool isTime = false);
	};

	static void startSession();

	static void clear();

	static void log(std::string log, bool time = false);

	static void warning(std::string warn, bool time = false);

};

class ScreenLog {
private:
	struct lgT {
		int id = 0;
		std::string name;
		sf::Text text;
	};

	static sf::Font font;
	static std::vector<lgT> logText;

public:

	ScreenLog();
	void setNewLog(std::string name, int id = 0);

	static void setValue(int id, std::string value = "None");

	void blit();
};
#endif