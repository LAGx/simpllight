
#include <string>
#include <vector>
#include <fstream>
#include <SFML/Graphics.hpp>
using namespace std;

class Log {
private:
	static string getTime(); //day|hour:min:sec

	static std::string logName;

public:
 
	static void clear();

	static void log(string log, bool time = false);

	static void error(string err, bool time = false);

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
