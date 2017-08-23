#include "INIWriter.h"
   
#include <fstream>
#include <string>


///////////////////////////////
/////////  INIWriter  /////////
///////////////////////////////

INIWriter::INIWriter() {
	boolType = new INIbooleanType;
	*boolType = INIbooleanType::INI_true_false;
}

INIWriter::INIWriter(INIbooleanType boolType)
{
	*this->boolType = boolType;
}                                                                                                                                                                                                                                                                                                                                                                             

void INIWriter::saveToFile(const std::string &fileName, int iosMode)
{
	std::ofstream file(fileName, iosMode);

	for (auto &i : INImap) {
		file << '[' << i.first << ']' << std::endl;

		for (auto &j : i.second->sectionMap) {
			file << j.first << " = " << j.second->str << std::endl;
		}

		file << std::endl;
	}

	file.close();
}

void INIWriter::setBooleanType(INIbooleanType type)
{
	*boolType = type;
}

void INIWriter::clear()
{
	INImap.clear();
}

std::ofstream &operator<<(std::ofstream &ofstr, const INIWriter &ini)
{
	for (auto &i : ini.INImap) {
		ofstr << '[' << i.first << ']' << std::endl;

		for (auto &j : i.second->sectionMap) {
			ofstr << j.first << " = " << j.second->str << std::endl;
		}

		ofstr << std::endl;
	}

	return ofstr;
}

std::ofstream &operator>>(const INIWriter &ini, std::ofstream &ofstr)
{
	return ofstr << ini;
}

INIsectionMap &INIWriter::operator[](const std::string &section)
{
	INImap[section] = new INIsectionMap(boolType);
	return *INImap[section];
}

INIsectionMap &INIWriter::operator[](const char *section)
{
	INImap[section] = new INIsectionMap(boolType);
	return *INImap[section];
}

INIsectionMap &INIWriter::operator[](long section)
{
	INImap[std::to_string(section)] = new INIsectionMap(boolType);
	return *INImap[std::to_string(section)];
}

INIsectionMap &INIWriter::operator[](int section)
{
	INImap[std::to_string(section)] = new INIsectionMap(boolType);
	return *INImap[std::to_string(section)];
}

INIWriter::~INIWriter()
{
	for (auto &i : INImap)
		delete i.second;
	delete boolType;
}



///////////////////////////////
///////  INIsectionMap  ///////
///////////////////////////////

INIsectionMap::INIsectionMap(INIWriter::INIbooleanType *type)
{
	boolType = type;
}

INIstring &INIsectionMap::operator[](const std::string &name)
{
	sectionMap[name] = new INIstring(boolType);
	return *sectionMap[name];
}

INIstring &INIsectionMap::operator[](const char *name)
{
	sectionMap[name] = new INIstring(boolType);
	return *sectionMap[name];
}

INIstring &INIsectionMap::operator[](long name)
{
	sectionMap[std::to_string(name)] = new INIstring(boolType);
	return *sectionMap[std::to_string(name)];
}

INIstring &INIsectionMap::operator[](int name)
{
	sectionMap[std::to_string(name)] = new INIstring(boolType);
	return *sectionMap[std::to_string(name)];
}

INIsectionMap::~INIsectionMap()
{
	for (auto &i : sectionMap)
		delete i.second;
}



///////////////////////////////
/////////  INIstring  /////////
///////////////////////////////

INIstring::INIstring(INIWriter::INIbooleanType *type)
{
	boolType = type;
}

INIstring INIstring::operator=(const std::string &val)
{
	str = val;
	return *this;
}

INIstring INIstring::operator=(const char *val)
{
	str = val;
	return *this;
}

INIstring INIstring::operator=(long val)
{
	str = std::to_string(val);
	return *this;
}

INIstring INIstring::operator=(int val)
{
	str = std::to_string(val);
	return *this;
}

INIstring INIstring::operator=(double val)
{
	str = std::to_string(val);
	return *this;
}

INIstring INIstring::operator=(float val)
{
	str = std::to_string(val);
	return *this;
}

INIstring INIstring::operator=(bool val)
{
	if (val)
		switch (*boolType)
		{
		default:
		case INIWriter::INIbooleanType::INI_true_false:
			str = "true";
			break;
		case INIWriter::INIbooleanType::INI_True_False:
			str = "True";
			break;
		case INIWriter::INIbooleanType::INI_TRUE_FALSE:
			str = "TRUE";
			break;
		case INIWriter::INIbooleanType::INI_yes_no:
			str = "yes";
			break;
		case INIWriter::INIbooleanType::INI_Yes_No:
			str = "Yes";
			break;
		case INIWriter::INIbooleanType::INI_YES_NO:
			str = "YES";
			break;
		case INIWriter::INIbooleanType::INI_on_off:
			str = "on";
			break;
		case INIWriter::INIbooleanType::INI_On_Off:
			str = "On";
			break;
		case INIWriter::INIbooleanType::INI_ON_OFF:
			str = "ON";
			break;
		case INIWriter::INIbooleanType::INI_1_0:
			str = "1";
			break;
		}
	else
		switch (*boolType)
		{
		default:
		case INIWriter::INIbooleanType::INI_true_false:
			str = "false";
			break;
		case INIWriter::INIbooleanType::INI_True_False:
			str = "False";
			break;
		case INIWriter::INIbooleanType::INI_TRUE_FALSE:
			str = "FALSE";
			break;
		case INIWriter::INIbooleanType::INI_yes_no:
			str = "no";
			break;
		case INIWriter::INIbooleanType::INI_Yes_No:
			str = "No";
			break;
		case INIWriter::INIbooleanType::INI_YES_NO:
			str = "NO";
			break;
		case INIWriter::INIbooleanType::INI_on_off:
			str = "off";
			break;
		case INIWriter::INIbooleanType::INI_On_Off:
			str = "Off";
			break;
		case INIWriter::INIbooleanType::INI_ON_OFF:
			str = "OFF";
			break;
		case INIWriter::INIbooleanType::INI_1_0:
			str = "0";
			break;
		}

	return *this;
}