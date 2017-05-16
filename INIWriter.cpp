#include "INIWriter.h"
   
#include <fstream>
#include <string>

using namespace std;


///////////////////////////////
/////////  INIWriter  /////////
///////////////////////////////

INIWriter::INIbooleanType INIWriter::boolType = INIWriter::INI_true_false;

INIWriter::INIWriter() {
	boolType = INI_true_false;
}

void INIWriter::saveToFile(const string &fileName, int iosMode)
{
	file.open(fileName, iosMode);

	for (auto &i : INImap) {
		file << '[' << i.first << ']' << endl;
		for (auto &j : i.second.sectionMap) {
			file << j.first << " = " << j.second.str << endl;
		}
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

INIWriter::INIsectionMap &INIWriter::operator[](const string &section)
{
	return INImap[section];
}

INIWriter::INIsectionMap &INIWriter::operator[](const char *section)
{
	return INImap[section];
}

INIWriter::INIsectionMap &INIWriter::operator[](long section)
{
	return INImap[to_string(section)];
}

INIWriter::INIsectionMap &INIWriter::operator[](int section)
{
	return INImap[to_string(section)];
}



///////////////////////////////
///////  INIsectionMap  ///////
///////////////////////////////

INIWriter::INIsectionMap::INIstring &INIWriter::INIsectionMap::operator[](const string &name)
{
	return sectionMap[name];
}

INIWriter::INIsectionMap::INIstring &INIWriter::INIsectionMap::operator[](const char *name)
{
	return sectionMap[name];
}

INIWriter::INIsectionMap::INIstring &INIWriter::INIsectionMap::operator[](long name)
{
	return sectionMap[to_string(name)];
}

INIWriter::INIsectionMap::INIstring &INIWriter::INIsectionMap::operator[](int name)
{
	return sectionMap[to_string(name)];
}



///////////////////////////////
/////////  INIstring  /////////
///////////////////////////////

INIWriter::INIsectionMap::INIstring INIWriter::INIsectionMap::INIstring::operator=(const string &val)
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
	str = to_string(val);
	return *this;
}

INIWriter::INIsectionMap::INIstring INIWriter::INIsectionMap::INIstring::operator=(int val)
{
	str = to_string(val);
	return *this;
}

INIWriter::INIsectionMap::INIstring INIWriter::INIsectionMap::INIstring::operator=(double val)
{
	str = to_string(val);
	return *this;
}

INIWriter::INIsectionMap::INIstring INIWriter::INIsectionMap::INIstring::operator=(float val)
{
	str = to_string(val);
	return *this;
}

INIWriter::INIsectionMap::INIstring INIWriter::INIsectionMap::INIstring::operator=(bool val)
{
	if (val)
		switch (boolType)
		{
		default:
		case INIWriter::INI_true_false:
			str = "true";
			break;
		case INIWriter::INI_True_False:
			str = "True";
			break;
		case INIWriter::INI_TRUE_FALSE:
			str = "TRUE";
			break;
		case INIWriter::INI_yes_no:
			str = "yes";
			break;
		case INIWriter::INI_Yes_No:
			str = "Yes";
			break;
		case INIWriter::INI_YES_NO:
			str = "YES";
			break;
		case INIWriter::INI_on_off:
			str = "on";
			break;
		case INIWriter::INI_On_Off:
			str = "On";
			break;
		case INIWriter::INI_ON_OFF:
			str = "ON";
			break;
		case INIWriter::INI_1_0:
			str = "1";
			break;
		}
	else
		switch (boolType)
		{
		default:
		case INIWriter::INI_true_false:
			str = "false";
			break;
		case INIWriter::INI_True_False:
			str = "False";
			break;
		case INIWriter::INI_TRUE_FALSE:
			str = "FALSE";
			break;
		case INIWriter::INI_yes_no:
			str = "no";
			break;
		case INIWriter::INI_Yes_No:
			str = "No";
			break;
		case INIWriter::INI_YES_NO:
			str = "NO";
			break;
		case INIWriter::INI_on_off:
			str = "off";
			break;
		case INIWriter::INI_On_Off:
			str = "Off";
			break;
		case INIWriter::INI_ON_OFF:
			str = "OFF";
			break;
		case INIWriter::INI_1_0:
			str = "0";
			break;
		}

	return *this;
}