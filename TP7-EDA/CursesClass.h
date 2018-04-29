#pragma once
#include "Curses.h"
#include <string>
using namespace std;

class CursesClass
{
public:
	CursesClass();
	~CursesClass();
	char getSingleLoweredCharInRange(char min, char max, int posY, int posX, const char * errorMessage);
	int getNumberString(int posY, int posX, const char * errorMessage);
	string getString(int posY, int posX, int maxSize);
private:
	WINDOW * display = NULL;
	const bool nodelaySet = true;

};

