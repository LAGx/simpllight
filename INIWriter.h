#pragma once
#ifndef _INI_WRITER_
#define _INI_WRITER_

#include <string>
#include <fstream>
#include <map>
#include <vector>
#include <list>

using namespace std;


class INIWriter
{
private: // force declaration
	class INIsectionMap;
public:

	/*!
	@brief  enum class that contain boolean types which uses for
	changing how bool variables type will be looks in file
	*/
	enum class INIbooleanType
	{
		INI_true_false,
		INI_True_False,
		INI_TRUE_FALSE,
		INI_yes_no,
		INI_Yes_No,
		INI_YES_NO,
		INI_on_off,
		INI_On_Off,
		INI_ON_OFF,
		INI_1_0
	};

	INIWriter();

	INIWriter(INIbooleanType boolType);

	void clear();

	void saveToFile(const string &fileName, int iosMode = std::ios::trunc);

	void setBooleanType(INIbooleanType type);

	INIsectionMap &operator[](const string &section);
	INIsectionMap &operator[](const char *section);

	INIsectionMap &operator[](long section);
	INIsectionMap &operator[](int section);

private:
	class INIsectionMap
	{
	private: // force declaration
		class INIstring;
	public:

		INIWriter::INIsectionMap::INIstring &operator[](const string &name);
		INIWriter::INIsectionMap::INIstring &operator[](const char *name);

		INIWriter::INIsectionMap::INIstring &operator[](long name);
		INIWriter::INIsectionMap::INIstring &operator[](int name);

	private:
		class INIstring
		{
		public:

			INIstring operator=(const string &val);
			INIstring operator=(const char *val);

			INIstring operator=(long val);
			INIstring operator=(int val);

			INIstring operator=(double val);
			INIstring operator=(float val);

			INIstring operator=(bool val);

		private:
			string str;
			friend void INIWriter::saveToFile(const string &fileName, int iosMode);
		};

		friend void INIWriter::saveToFile(const string &fileName, int iosMode);
		map<string, INIstring> sectionMap;

	};

	map<string, INIsectionMap> INImap;
	ofstream file;
	static INIbooleanType boolType;
};
#endif
