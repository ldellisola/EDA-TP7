#pragma once
#include "CursesClass.h"
#include"Allegro\AllegroClass.h"
#include"Allegro\WrittenBox.h"
#include <fstream>
#include <string>

using namespace std;

typedef enum MODE
{
	SERVER, CLIENT, LEAVE
};



string askForFile(CursesClass& curses);

bool fileExists(const char * fileName);

void fileDoesntExist(CursesClass& curses, string fileName);

bool askToOverwrite(CursesClass& curses);


MODE selectmode(ALLEGRO_EVENT_QUEUE*);