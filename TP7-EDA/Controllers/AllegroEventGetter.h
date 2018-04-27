#pragma once
#include "Controller.h"
#include "../Events/Ev_t.h"
#include <allegro5\allegro.h>


class AllegroEventGetter :
	public Controller
{
public:
	AllegroEventGetter(ALLEGRO_EVENT_QUEUE * eq_);
	~AllegroEventGetter();

	void * getEvent(void * data);
	bool isThereEvent();
	

private:
	ALLEGRO_EVENT_QUEUE * eq = NULL;;
	bool validKey(int key);
	void setEvent(Evnt evento, unsigned int wormID = 0);
	Ev_t ev[2];
};

