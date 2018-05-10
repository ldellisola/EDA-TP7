#pragma once
#include "Controller.h"
#include "../Events/Ev_t.h"
#include <allegro5\allegro.h>

#define WORMID 1
#define TIMETHRESHOLD (100) // ms


class AllegroEventGetter :
	public Controller
{
public:
	AllegroEventGetter(ALLEGRO_EVENT_QUEUE * eq_);
	~AllegroEventGetter();

	void * getEvent(void * data);
	bool isThereEvent();
	void loadWormID(uint32_t wormID);

private:
	ALLEGRO_EVENT_QUEUE * eq = NULL;
	bool validKey(int key);
	void setEvent(Evnt evento, unsigned int wormID = 0);
	uint32_t wormID;
	Ev_t ev[3];	// 0:Worm , 1:Timer , 2:Quit
	Ev_t retValue[3];
};
