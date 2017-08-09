#pragma once
#include <fstream>
#include <string>
#include <SFML/Graphics.hpp>
#include <ShlObj.h>

#include "log.h"
#include "window.h"
#include "service.h"

using namespace std;

string Log::logName;

void Log::startSession()
{
	string path = spl::getSpecialFolderPath(CSIDL_PERSONAL);
	spl::createFolder(path + "\\simpllight");
	logName = path + "\\simpllight\\log.txt";
	clear();
}

void Log::clear() {
	ofstream file(logName, ios_base::trunc);
	file.close();
}

void Log::log(string log, bool time) {
	ofstream file(logName, ios_base::app);
	if (!time) {
		file << "LOG: " << log << endl;
	}
	else {
		file << "LOG|" << spl::Time::getTime(spl::Time::TimeMode::Day_HourMinSec) << "|: " << log << endl;
	}
	file.close();
}

void Log::warning(string warn, bool time) {
	ofstream file(logName, ios_base::app);
	if (!time) {
		file << "WARNING: " << warn << endl;
	}
	else {
		file << "WARNING|" << spl::Time::getTime(spl::Time::TimeMode::Day_HourMinSec) << "|: " << warn << endl;
	}
	file.close();
}

void Log::error(string err, bool time) {
	ofstream file(logName, ios_base::app);
	if (!time) {
		file << "ERROR: " << err << endl;
	}
	else {
		file << "ERROR|" << spl::Time::getTime(spl::Time::TimeMode::Day_HourMinSec) << "|: " << err << endl;
	}
	file.close();
}

Log::Exception::Exception(string error, bool isTime) {
	Log::error(error, isTime);
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
		throw Log::Exception("load screenLog font");
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
	for (size_t i = 0; i < logText.size(); i++) {
		if (logText[i].id == id) {
			logText[i].text.setString(logText[i].name + ": " + value);
			return;
		}
	}
	throw Log::Exception("id screen log.");
}

void ScreenLog::blit() {
	for (size_t i = 0; i < logText.size(); i++) {
		logText[i].text.setPosition(spl::Window::currGlobalViewCord.x + 5 - spl::Window::screenSize.x / 2, 15 * logText[i].id + spl::Window::currGlobalViewCord.y - spl::Window::screenSize.y / 2);
		spl::ToDraw td = { &logText[i].text, -1000 };
		spl::Window::allDrawable.push_back(td);
	}
}

