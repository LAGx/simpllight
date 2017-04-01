#pragma once
#include "log.h"
#include <fstream>
#include <string>
#include <ctime>
#include <SFML/Graphics.hpp>
#include "window.h"
#include "state.h"

using namespace std;

string Log::logName = "log.txt";

void Log::clear() {
	ofstream file(logName, ios_base::trunc);
	file.close();
}

string Log::getTime() { //day|hour:min:sec
	time_t rawtime = time(0);
	struct tm timeinfo;
	localtime_s(&timeinfo, &rawtime);

	string date = to_string(timeinfo.tm_mday) + "|" + to_string(timeinfo.tm_hour) + ":" + to_string(timeinfo.tm_min) + ":" + to_string(timeinfo.tm_sec);
	return date;
}

void Log::log(string log, bool time) {
	ofstream file(logName, ios_base::app);
	if (!time) {
		file << "LOG: " << log << endl;
	}
	else {
		file << "LOG|" << getTime() << "|: " << log << endl;
	}
	file.close();
}

void Log::warning(string warn, bool time) {
	ofstream file(logName, ios_base::app);
	if (!time) {
		file << "WARNING: " << warn << endl;
	}
	else {
		file << "WARNING|" << getTime() << "|: " << warn << endl;
	}
	file.close();
}

void Log::error(string err, bool time) {
	ofstream file(logName, ios_base::app);
	if (!time) {
		file << "ERROR: " << err << endl;
	}
	else {
		file << "ERROR|"  << getTime() << "|: " << err << endl;
	}
	log("_______EMERGENCY CLOSING PROGRAM_______", true);
	log("____________LOG SESSION END____________", true);
	file.close();
	exit(EXIT_FAILURE);
}



////////////////////////////////////////////////////////////////////////
////
////               SCREEN LOG
//////////////////////////////////////////////////////////////////////
std::vector<ScreenLog::lgT> ScreenLog::logText;
sf::Font ScreenLog::font;

ScreenLog::ScreenLog() {
	if (!font.loadFromFile("font/arial.ttf"))
	{
		Log::error("load screenLog font");
	}
}

void ScreenLog::setNewLog(string name, int id) {

	lgT st;
	st.name = name;
	st.id = id;
	st.text.setFont(font);
	st.text.setString(name + ": " + "None");
	st.text.setCharacterSize(15);
	st.text.setFillColor(sf::Color::White);
	st.text.setOutlineColor(sf::Color::Black);
	logText.push_back(st);
}

void ScreenLog::setValue(int id, string value) {
	for (int i = 0; i < logText.size(); i++) {
		if (logText[i].id == id) {
			logText[i].text.setString(logText[i].name + ": " + value);
			return;
		}
	}
	Log::error("id screen log.");
}

void ScreenLog::blit() {
	for (int i = 0; i < logText.size(); i++) {
		logText[i].text.setPosition(spl::Window::currGlobalViewCord.x + 5- spl::Window::currScreenSize.x/2, 15 * logText[i].id + spl::Window::currGlobalViewCord.y- spl::Window::currScreenSize.y / 2);
		spl::ToDraw td = { &logText[i].text, -1000 };
		spl::Window::allDrawable.push_back(td);
	}
}

