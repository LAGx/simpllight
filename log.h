#ifndef _LOG_
#define _LOG_

#include <string>
#include <vector>
#include <fstream>
#include <SFML/Graphics.hpp>
using namespace std;

class Log {
private:
	static string getTime(); //day|hour:min:sec

	static std::string logName;

	static void error(string err, bool time = false);

public:

	class Exception {
	public:
		Exception(string error, bool isTime = false);
	};
 
	static void clear();

	static void log(string log, bool time = false);

	static void warning(string warn, bool time = false);



};

class ScreenLog {
private:
	struct lgT {
		int id = 0;
		string name;
		sf::Text text;
	};

	static sf::Font font;
	static vector<lgT> logText;
	
public:
	
	ScreenLog();
	void setNewLog(string name,int id = 0);

	static void setValue(int id, string value = "None");
	
	void blit();
};
#endif