#include "CursesClass.h"



CursesClass::CursesClass()
{
	display = initscr();
	start_color();

	init_pair(1, COLOR_GREEN, COLOR_BLACK);
	init_pair(2, COLOR_RED, COLOR_BLACK);
	color_set(1, NULL);

	nodelay(display, nodelaySet);
	noecho();
}


CursesClass::~CursesClass()
{
	endwin();
}

char CursesClass::getSingleLoweredCharInRange(char min, char max, int posY, int posX, const char * errorMessage) {

	char retValue;
	bool leave = false;
	bool err = false;
	do {
		move(posY, posX);
		retValue = getch();
		if (retValue != ERR) {
			retValue = tolower(retValue);
			if ((retValue >= min && retValue <= max))
			{
				leave = true;
				if (err) {
					mvdeleteln(3, 0);
					err = false;
				}

			}
			else
			{
				if (!err)
				{
					color_set(2, NULL);
					mvdeleteln(posY - 1, 0);
					mvprintw(posY - 1, 0, errorMessage);
					err = true;
					color_set(1, NULL);
				}

			}
		}

	} while (!leave);

	return retValue;
}

int CursesClass::getNumberString(int posY, int posX, const char * errorMessage) {
	bool leave = false;
	bool isNum = true;
	bool numEnded = false;

	char str[4] = { 0,0,0,0 };


	do {
		isNum = true;
		move(posY, posX);
		if (getnstr(str, 3) != ERR) {

			for (int i = 0; i < 4 && isNum && !numEnded; i++) {

				if (!isdigit(str[i]) && str[i] != 0 && str[i] != '\n') {
					isNum = false;
					color_set(2, NULL);
					mvprintw(posY - 1, posX, errorMessage);
					mvdeleteln(posY, posX);
					color_set(1, NULL);
				}
				else if (str[i] == '\n')
					str[i] = 0;
				if (str[i] == 0)
				{
					numEnded = true;
					leave = true;
				}


			}
		}

	} while (!leave);

	mvdeleteln(posY - 1, 0);
	mvdeleteln(posY, 0);

	return atoi(str);
}

string CursesClass::getString(int posY, int posX, int maxSize)
{
	bool leave = false;
	bool isNum = true;
	bool numEnded = false;
	echo();
	char * str = new char[maxSize +1];


	do {
		isNum = true;
		move(posY, posX);
		if (getnstr(str, maxSize) != ERR) {

			leave = true;
		}

	} while (!leave);

	mvdeleteln(posY - 1, 0);
	mvdeleteln(posY, 0);

	string retValue = str;

	delete[] str;

	return retValue;
}
