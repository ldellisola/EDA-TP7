#include "AllegroEventGetter.h"



Evnt trasformAllegroEvents(int key)
{
	Evnt ev = NOEVENT;

	switch (key)
	{
	case ALLEGRO_KEY_LEFT:
	case ALLEGRO_KEY_A:
		ev = LEFT_EV;
		break;
	case ALLEGRO_KEY_RIGHT:
	case ALLEGRO_KEY_D:
		ev = RIGHT_EV;
		break;
	case ALLEGRO_KEY_UP:
	case ALLEGRO_KEY_W:
		ev = JUMP_EV;
		break;
	case ALLEGRO_KEY_ESCAPE:
		ev = QUIT_EV;
		break;
	}
	return ev;
}

AllegroEventGetter::AllegroEventGetter( ALLEGRO_EVENT_QUEUE * eq_)
{
	eq = eq_;
}

void * AllegroEventGetter::getEvent(void * data)
{
	ev[0].deactivate();
	ev[1].deactivate();
	ev[2].deactivate();

	ALLEGRO_EVENT alEv;

	int * size = (int *)data;
	

	if (al_get_next_event(eq, &alEv)) {

		switch (alEv.type) {
		case ALLEGRO_EVENT_KEY_DOWN:

			if (alEv.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
				ev[2].Event = QUIT_EV;
				ev[2].activate();
			}
			else
				if (!ev[0].active && validKey(alEv.keyboard.keycode) && !ev[0].timerExist())
					setEvent(trasformAllegroEvents(alEv.keyboard.keycode), WORMID);
			break;
		case ALLEGRO_EVENT_KEY_UP:
			if (ev[0].timerExist() && ev[0].Event == trasformAllegroEvents(alEv.keyboard.keycode)) {
				if (!ev[0].timerGreaterThan(TIMETHRESHOLD))
				{
					if (!ev[0].active && ev[0].Event == LEFT_EV) {
						ev[0].Event = FLIP_LEFT_EV;
						ev[0].wormID = this->wormID;
						ev[0].activate();
					}
					else if (!ev[0].active && ev[0].Event == RIGHT_EV) {
						ev[0].Event = FLIP_RIGHT_EV;
						ev[0].wormID = this->wormID;
						ev[0].activate();
					}
				}
				break;
		case ALLEGRO_EVENT_TIMER:
			this->setEvent(TIMER_EV);
			ev[1].activate();

			if (!ev[0].active && ev[0].timerExist())
			{
				ev[0].time->stop();
				if (ev[0].time->getTime() >= TIMETHRESHOLD)
				{
					ev[0].activate();
					ev[0].wormID = this->wormID;
					ev[0].killTimer();
					ev[0].newTimer();
				}
			}
			break;
		case ALLEGRO_EVENT_DISPLAY_CLOSE:
			ev[2].Event = QUIT_EV;
			ev[2].activate();
			break;
			}
		}
	}

	
	int i = 0;

	if (ev[0].active)
		retValue[i++] = ev[0];
	if (ev[1].active)
		retValue[i++] = ev[1];
	if (ev[2].active)
		retValue[i++] = ev[2];

	*size = i;

	return retValue;
}

bool AllegroEventGetter::isThereEvent()
{
	return (ev[0].active || ev[1].active || ev[2].active);
}

void AllegroEventGetter::loadWormID(uint32_t wormID)
{
	this->wormID = wormID;
}




AllegroEventGetter::~AllegroEventGetter()
{
}


void AllegroEventGetter::setEvent(Evnt evento, unsigned int wormID ) {

	
	if (evento != TIMER_EV) {
		ev[0].Event = evento;
		ev[0].wormID = wormID;
		ev[0].newTimer();
	}
	else
		ev[1].Event = evento;


}

bool AllegroEventGetter::validKey(int key) {
	
	return (key == ALLEGRO_KEY_LEFT || key == ALLEGRO_KEY_RIGHT || key == ALLEGRO_KEY_UP);
}