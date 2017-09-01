#pragma once
#ifndef _SERVICE_
#define _SERVICE_

#include <SFML/Graphics.hpp>
#include <string>

namespace spl {

	class TextureGenerator {
	public:
		void fillShadowRect(int shadowSize, sf::Vector2i size, sf::Color color, std::string name);

		void fillShadowCircle(int shadowSize, int radiusSize, sf::Color color, std::string name);

		void conturRect(sf::Vector2i size, int lineWidth, sf::Color color, std::string name);

		void conturCircle(int radius, int lineWidth, sf::Color color, std::string name);
	};

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

	class Folders
	{
	public:

		/*!
		@brief CSIDL of needed shell folders that defined in <ShlObj.h>
		*/
		enum foldersCSIDL {
			desktop = 0x0000, // <desktop>
			startMenu_programs = 0x0002, // Start Menu\Programs
			myDocuments = 0x0005, // My Documents
			startMenu_programs_startup = 0x0007, // Start Menu\Programs\Startup
			userName_startMenu = 0x000b, // <user name>\Start Menu
			userName_desktop = 0x0010, // <user name>\Desktop
			myComputer = 0x0011, // My Computer
			windows_fonts = 0x0014, // windows\fonts
			allUsers_startMenu = 0x0016, // All Users\Start Menu
			allUsers_startMenu_programs = 0X0017, // All Users\Start Menu\Programs
			allUsers_Startup = 0x0018, // All Users\Startup
			allUsers_Desktop = 0x0019, // All Users\Desktop
			userName_applicationData = 0x001a, // <user name>\Application Data
			userName_localSettings_applicaitonData = 0x001c, // <user name>\Local Settings\Applicaiton Data (non roaming)
			allUsers_applicationData = 0x0023, // All Users\Application Data
			getWindowsDirectory = 0x0024, // GetWindowsDirectory()
			c_programFiles = 0x0026, // C:\Program Files
			userprofile = 0x0028, // USERPROFILE
			c_programFilesRISC = 0x002a, // x86 C:\Program Files on RISC
			allUsers_documents = 0x002e, // All Users\Documents
		};

		/*!
		@brief return path to special folder,
		for example "Documents" or "Startup Directory"

		@param[in] csidl  identificator for folder that you want to find

		@throw Log::Exception if can't get folder path

		@return path to folder
		*/
		static std::string getSpecialFolderPath(foldersCSIDL csidl);

		/*!
		@brief creating folder in the specific path,
		can create all needed folders

		@param[in] path  full path to folder that you want to create
		*/
		static void createFolder(std::string path);

		static void copyFolder(std::string from, std::string to);

		static void deleteFolder(std::string path);
	};

	template<class T>
	T getLength(sf::Vector2<T> vec1, sf::Vector2<T> vec2 = { 0 , 0 });
}
#endif