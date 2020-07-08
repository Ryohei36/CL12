#include "stdafx.h"
#include "Setting.h"
#include <Windows.h>

int readInt(const char* section, const char* keyword, int defaultValue, const char* filePath) {
	//maximun 255
	//return GetPrivateProfileInt(section, keyword, defaultValue, filePath);
	char valueString[CHARBUFF];
	if (readChar(section, keyword, "", valueString, filePath)) {
		return atof(valueString);
	}
	else {
		return defaultValue;
	}
}

double readDouble(const char* section, const char* keyword, double defaultValue, const char* filePath) {
	char valueString[CHARBUFF];
	char* endptr;
	if (readChar(section, keyword, "", valueString, filePath)) {
		return strtod(valueString, &endptr);
	}
	else {
		return defaultValue;
	}
}

bool  readChar(const char* section, const char* keyword, const char* defaultValue, char* returnValue, const char* filePath) {
	if (GetPrivateProfileString(section, keyword, defaultValue, returnValue, CHARBUFF, filePath) != 0) {
		return true;
	}
	else {
		return false;
	}
}

void getCurrentDirectory(char* currentDirectory) {
	GetCurrentDirectory(CHARBUFF, currentDirectory);
}


bool writeChar(const char* section, const char* keyword, const char* returnValue, const char* filePath) {
	return WritePrivateProfileString(section, keyword, returnValue, filePath);
}

bool writeInt(const char* section, const char* keyword, int returnValue, const char* filePath) {
	char valueChar[CHARBUFF];
	sprintf_s(valueChar, "%d", returnValue);
	return writeChar(section, keyword, valueChar, filePath);
}
bool writeDouble(const char* section, const char* keyword, double returnValue, const char* filePath) {
	char valueChar[CHARBUFF];
	sprintf_s(valueChar, "%lf", returnValue);
	return writeChar(section, keyword, valueChar, filePath);
}
void getGurrentDirectory(char* currentDirectory) {
	GetCurrentDirectory(CHARBUFF, currentDirectory);
}
