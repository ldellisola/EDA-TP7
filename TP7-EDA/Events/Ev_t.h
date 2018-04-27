#pragma once
#include <vector>
#include "../Events/Timer.h"

using namespace std;

typedef enum Evnt {
	NOEVENT, LEFT_EV, RIGHT_EV, JUMP_EV, TIMER_EV, QUIT_EV, FLIP_LEFT_EV, FLIP_RIGHT_EV
};

//Esta estructura es el evento que obtenes. Tiene lugar para almacenar el evento actual y el evento anterior, un puntero a timer y tambien te dice si esta
// activo o no el evento. Tiene funciones para manejar tanto los eventos como el timer

typedef struct {
	Evnt Event;
	Evnt lastEv;
	Timer * time;
	unsigned int wormID;
	bool active;
	int keycode;
	bool keyPressed = true;

	void activate() {
		if (Event == NOEVENT)
			Event == lastEv;
		active = true;
	}
	void deactivate() {
		active = false;
		lastEv = Event;
		

	}
	void newTimer() {
		time = new Timer();
	}
	bool timerExist() {
		return time != NULL;
	}

	void killTimer() {
		delete time;
		time = NULL;
	}
	// Esta funcion mata al time despues de ser utilizada. CUIDADO
	bool timerGreaterThan(float ms)
	{
		bool retValue = false;
		time->stop();
		if (time->getTime() >= ms)
			retValue = true;
		killTimer();

		return retValue;
	}
}Ev_t;