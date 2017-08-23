#include "INIWriter.h"
   
#include <fstream>
#include <string>


///////////////////////////////
/////////  INIWriter  /////////
///////////////////////////////

INIWriter::INIbooleanType INIWriter::boolType = INIWriter::INIbooleanType::INI_true_false;

INIWriter::INIWriter() {
	boolType = INIbooleanType::INI_true_false;
}

INIWriter::INIWriter(INIbooleanType boolType)
{
	this->boolType = boolType;
}                                                                                                                                                                                                                                                                                                                                                                             

void INIWriter::saveToFile(const std::string &fileName, int iosMode)
{
	std::ofstream file(fileName, iosMode);

	for (auto &i : INImap) {
		file << '[' << i.first << ']' << std::endl;

		for (auto &j : i.second.sectionMap) {
			file << j.first << " = " << j.second.str << std::endl;
		}

		file << std::endl;
	}

	file.close();
}

void INIWriter::setBooleanType(INIbooleanType type)
{
	boolType = type;
}

void INIWriter::clear()
{
	INImap.clear();
}

std::ofstream &operator<<(std::ofstream &ofstr, const INIWriter &ini)
{
	for (auto &i : ini.INImap) {
		ofstr << '[' << i.first << ']' << std::endl;

		for (auto &j : i.second.sectionMap) {
			ofstr << j.first << " = " << j.second.str << std::endl;
		}

		ofstr << std::endl;
	}

	return ofstr;
}

std::ofstream &operator>>(const INIWriter &ini, std::ofstream &ofstr)
{
	return ofstr << ini;
}

INIWriter::INIsectionMap &INIWriter::operator[](const std::string &section)
{
	return INImap[section];
}

INIWriter::INIsectionMap &INIWriter::operator[](const char *section)
{
	return INImap[section];
}

INIWriter::INIsectionMap &INIWriter::operator[](long section)
{
	return INImap[std::to_string(section)];
}

INIWriter::INIsectionMap &INIWriter::operator[](int section)
{
	return INImap[std::to_string(section)];
}



///////////////////////////////
///////  INIsectionMap  ///////
///////////////////////////////

INIWriter::INIsectionMap::INIstring &INIWriter::INIsectionMap::operator[](const std::string &name)
{
	return sectionMap[name];
}

INIWriter::INIsectionMap::INIstring &INIWriter::INIsectionMap::operator[](const char *name)
{
	return sectionMap[name];
}

INIWriter::INIsectionMap::INIstring &INIWriter::INIsectionMap::operator[](long name)
{
	return sectionMap[std::to_string(name)];
}

INIWriter::INIsectionMap::INIstring &INIWriter::INIsectionMap::operator[](int name)
{
	return sectionMap[std::to_string(name)];
}



///////////////////////////////
/////////  INIstring  /////////
///////////////////////////////

INIWriter::INIsectionMap::INIstring INIWriter::INIsectionMap::INIstring::operator=(const std::string &val)
{
	str = val;
	return *this;
}

INIWriter::INIsectionMap::INIstring INIWriter::INIsectionMap::INIstring::operator=(const char *val)
{
	str = val;
	return *this;
}

INIWriter::INIsectionMap::INIstring INIWriter::INIsectionMap::INIstring::operator=(long val)
{
	str = std::to_string(val);
	return *this;
}

INIWriter::INIsectionMap::INIstring INIWriter::INIsectionMap::INIstring::operator=(int val)
{
	str = std::to_string(val);
	return *this;
}

INIWriter::INIsectionMap::INIstring INIWriter::INIsectionMap::INIstring::operator=(double val)
{
	str = std::to_string(val);
	return *this;
}

INIWriter::INIsectionMap::INIstring INIWriter::INIsectionMap::INIstring::operator=(float val)
{
	str = std::to_string(val);
	return *this;
}

INIWriter::INIsectionMap::INIstring INIWriter::INIsectionMap::INIstring::operator=(bool val)
{
	if (val)
		switch (boolType)
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
		switch (boolType)
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