#include "UserHandler.h"


MODE selectmode(ALLEGRO_EVENT_QUEUE * ev_que)
{
	MODE respuesta;
	ALLEGRO_BITMAP* fondo = al_load_bitmap("background.png");
	al_draw_bitmap(fondo, 0, 0, 0);
	WrittenBox bienvenida(700, 50, 400, 80, 40, "MODE SELECTION", "Montserrat-Black.otf", "black");
	bienvenida.draw();
	WrittenBox instrucciones(400, 120, 1000, 40, 20, "Select your role with your keyboard. Press Q to exit", "Montserrat-Black.otf","white");
	instrucciones.draw();
	WrittenBox opcionS(550, 250, 400, 40, 20, "S = SERVER", "Montserrat-Black.otf", "red");
	WrittenBox opcionC(750, 250, 400, 40, 20, "C = CLIENT", "Montserrat-Black.otf", "yellow");
	opcionS.draw();
	opcionC.draw();
	WrittenBox warning(550, 350, 600, 40, 20, "Invalid imput, only S, C or Q are valid options", "Montserrat-Black.otf", "green");
	al_flip_display();
	ALLEGRO_EVENT ev;
	bool looping = true;
	while (looping)
	{
		if (al_get_next_event(ev_que, &ev))
		{
			if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
			{
				respuesta = LEAVE;
				looping = false;
			}
			else if (ev.type == ALLEGRO_EVENT_KEY_UP)
			{
				switch (ev.keyboard.keycode)
				{
				case ALLEGRO_KEY_S:
					respuesta = SERVER;
					looping = false;
					break;
				case ALLEGRO_KEY_C:
					respuesta = CLIENT;
					looping = false;
					break;
				case ALLEGRO_KEY_Q:
					looping = false;
					respuesta = LEAVE;
					break;

				default:
					warning.draw();
					al_flip_display();
					break;
				}
			}
		}
	}
	return respuesta;

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

void initNetworkControlScreen(CursesClass& curses)
{
	mvprintw(8, 0, "HOLA MUNDO2!");
}

