#include "UserHandler.h"

MODE selectMode(CursesClass& curses)
{
	mvprintw(0, 0, "Press 'S' to be Server or press 'C' to be Client. If you rather leave, pres 'Q'");
	char value;
	bool valid = false;
	while (!valid) {
		value = curses.getSingleLoweredCharInRange('a', 'z', 2, 0, "Invalid character, please try again");
		if (value == 's' || value == 'c' ||value == 'q')
			valid = true;
		else
		{
			mvdeleteln(1, 0);
			mvprintw(1, 0, "ERROR, try again");
		}
	}

	return (value == 'q' ? LEAVE : (value == 's' ? SERVER : CLIENT));
	
}

string askForFile(CursesClass & curses)
{
	const int maxC = 200;
	mvprintw(0, 0, "Please type the name of the file. It must include the extension. The max ammount of characters is %d ",maxC);
	string fileName = curses.getString(2, 0, maxC);
	return fileName;
}

bool fileExists(const char * fileName)
{
	bool retValue = false;
	ifstream src;
	src.open(fileName);

	if (src.good())
		retValue = true;
	src.close();
	return retValue;
}

void fileDoesntExist(CursesClass & curses, string fileName)
{
	clear();
	mvprintw(0, 0, "ERROR: File %s doesn't exist \nPress any key to continue", fileName.c_str());
	bool leave = false;
	do {
		if (getch() != ERR)
			leave = true;
	} while (!leave);
}

bool askToOverwrite(CursesClass & curses)
{
	bool retValue = false;
	clear();

	mvprintw(0, 0, "The file you are trying to create already exists. Do you want to overwrite it?");
	mvprintw(1, 0, "Please type 'Y' if you agree or type 'N' if you don't");

	bool leave = false;
	do {
		switch (curses.getSingleLoweredCharInRange('a', 'z', 3, 0, "Error: invalid key pressed")) {
		case 'y':
			leave = true;
			retValue = true;
			break;
		case 'n':
			leave = true;
			retValue = false;
			break;
		}

	} while (!leave);
	
	

	return retValue;
}
