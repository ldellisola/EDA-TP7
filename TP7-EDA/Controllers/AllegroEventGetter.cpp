#include "AllegroEventGetter.h"

#define WORMID 1

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

	ALLEGRO_EVENT alEv;

	int * size = (int *)data;
	*size = 2;

	if (al_get_next_event(eq, &alEv)) {

		switch (alEv.type) {
		case ALLEGRO_EVENT_KEY_DOWN:

			if (alEv.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
				ev[1].Event = QUIT_EV;
			else
				if (!ev[0].active && validKey(alEv.keyboard.keycode) && !ev[0].timerExist())
					ev[0] = setEvent(trasformAllegroEvents(alEv.keyboard.keycode), WORMID);
			break;
		case ALLEGRO_EVENT_KEY_UP:
			if (ev[0].timerExist() && ev[0].Event == trasformAllegroEvents(alEv.keyboard.keycode)) {
				if (!ev[0].timerGreaterThan(100))
				{
					if (!ev[0].active && ev[0].Event == LEFT_EV) {
						ev[0].Event = FLIP_LEFT_EV;
						ev[0].activate();
					}
					else if (!ev[0].active && ev[0].Event == RIGHT_EV) {
						ev[0].Event = FLIP_RIGHT_EV;
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
				float time_ = ev[0].time->getTime();
				if (time_ >= 100)
				{
					ev[0].activate();
					ev[0].time->start();
				}
			}



			break;
		case ALLEGRO_EVENT_DISPLAY_CLOSE:
			ev[1].Event = QUIT_EV;
			break;
			}
		}
	}

	return ev;
}

bool AllegroEventGetter::isThereEvent()
{
	return (ev[0].active || ev[1].active);
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