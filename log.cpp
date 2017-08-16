#include "log.h"

#include <fstream>
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>

#include "window.h"
#include "service.h"

using string = std::string;
using Folders = spl::Folders;

string Log::logName;

void Log::startSession()
{
	string path = Folders::getSpecialFolderPath(Folders::myDocuments);
	Folders::createFolder(path + "\\simpllight");
	logName = path + "\\simpllight\\log.txt";
	clear();
	Log::log("____________LOG SESSION START__________", true);
}

void Log::clear() {
	std::ofstream file(logName, std::ios_base::trunc);
	file.close();
}

void Log::log(std::string log, bool time) {
	std::ofstream file(logName, std::ios_base::app);
	if (!time) {
		file << "LOG: " << log << std::endl;
	}
	else {
		file << "LOG|" << spl::Time::getTime(spl::Time::TimeMode::Day_HourMinSec) << "|: " << log << std::endl;
	}
	file.close();
}

void Log::warning(std::string warn, bool time) {
	std::ofstream file(logName, std::ios_base::app);
	if (!time) {
		file << "WARNING: " << warn << std::endl;
	}
	else {
		file << "WARNING|" << spl::Time::getTime(spl::Time::TimeMode::Day_HourMinSec) << "|: " << warn << std::endl;
	}
	file.close();
}

void Log::finishSession()
{
	Log::log("____________LOG SESSION END____________", true);
}

void Log::error(std::string err, bool time) {
	std::ofstream file(logName, std::ios_base::app);
	if (!time) {
		file << "ERROR: " << err << std::endl;
	}
	else {
		file << "ERROR|" << spl::Time::getTime(spl::Time::TimeMode::Day_HourMinSec) << "|: " << err << std::endl;
	}
	file.close();
}

Log::Exception::Exception(std::string error, bool isTime) {
	Log::error(error, isTime);
}


///////////////////////////////////////
// ----------  ScreenLog  ---------- //
///////////////////////////////////////

std::vector<ScreenLog::lgT> ScreenLog::logText;
sf::Font ScreenLog::font;

ScreenLog::ScreenLog() {
	if (!font.loadFromFile("font/arial.ttf"))
	{
		throw Log::Exception("load screenLog font");
	}
}

void ScreenLog::setNewLog(std::string name, int id) {

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

void ScreenLog::setValue(int id, std::string value) {
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

