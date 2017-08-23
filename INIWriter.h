#pragma once
#ifndef _INI_WRITER_
#define _INI_WRITER_

#include <string>
#include <fstream>
#include <map>


class INIWriter
{
private: // force declaration
	class INIsectionMap;
public:

	/*!
	@brief  enum class that contain boolean types which uses for
	changing how bool variables will be looks in file
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

	void saveToFile(const std::string &fileName, int iosMode = std::ios::trunc);

	void setBooleanType(INIbooleanType type);

	friend std::ofstream &operator<<(std::ofstream& ofstr, const INIWriter& ini);
	friend std::ofstream &operator>>(const INIWriter& ini, std::ofstream& ofstr);

	INIsectionMap &operator[](const std::string &section);
	INIsectionMap &operator[](const char *section);

	INIsectionMap &operator[](long section);
	INIsectionMap &operator[](int section);

	~INIWriter();

private:
	class INIsectionMap
	{
	private: // force declaration
		class INIstring;
	public:
		INIsectionMap(INIbooleanType *type);

		INIWriter::INIsectionMap::INIstring &operator[](const std::string &name);
		INIWriter::INIsectionMap::INIstring &operator[](const char *name);

		INIWriter::INIsectionMap::INIstring &operator[](long name);
		INIWriter::INIsectionMap::INIstring &operator[](int name);

		~INIsectionMap();

	private:
		friend std::ofstream &operator<<(std::ofstream& ofstr, const INIWriter& ini);

		class INIstring
		{
		public:

			INIstring(INIbooleanType *type);

			INIstring operator=(const std::string &val);
			INIstring operator=(const char *val);

			INIstring operator=(long val);
			INIstring operator=(int val);

			INIstring operator=(double val);
			INIstring operator=(float val);

			INIstring operator=(bool val);

		private:
			std::string str; 
			INIbooleanType *boolType;

			friend std::ofstream &operator<<(std::ofstream& ofstr, const INIWriter& ini);
			friend void INIWriter::saveToFile(const std::string &fileName, int iosMode);
		};

		friend void INIWriter::saveToFile(const std::string &fileName, int iosMode);

		INIbooleanType *boolType;
		std::map<std::string, INIstring*> sectionMap;
	};

	std::map<std::string, INIsectionMap*> INImap;
	INIbooleanType *boolType;
};
#endif