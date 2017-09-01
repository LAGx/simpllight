#pragma once
#ifndef _INI_WRITER_
#define _INI_WRITER_

#include <string>
#include <fstream>
#include <map>

// force declaration
class INIstring; 
class INIsectionMap;

class INIWriter
{
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
	std::map<std::string, INIsectionMap*> INImap;
	INIbooleanType *boolType;
};

class INIsectionMap
{
public:

	INIsectionMap(INIWriter::INIbooleanType *type);

	INIstring &operator[](const std::string &name);
	INIstring &operator[](const char *name);

	INIstring &operator[](long name);
	INIstring &operator[](int name);

	~INIsectionMap();

private:
	friend std::ofstream &operator<<(std::ofstream& ofstr, const INIWriter& ini);
	friend void INIWriter::saveToFile(const std::string &fileName, int iosMode);

	INIWriter::INIbooleanType *boolType;
	std::map<std::string, INIstring*> sectionMap;
};

class INIstring
{
public:

	INIstring(INIWriter::INIbooleanType *type);

	INIstring operator=(const std::string &val);
	INIstring operator=(const char *val);

	INIstring operator=(long val);
	INIstring operator=(int val);

	INIstring operator=(double val);
	INIstring operator=(float val);

	INIstring operator=(bool val);

private:
	std::string str;
	INIWriter::INIbooleanType *boolType;

	friend std::ofstream &operator<<(std::ofstream& ofstr, const INIWriter& ini);
	friend void INIWriter::saveToFile(const std::string &fileName, int iosMode);
};
#endif