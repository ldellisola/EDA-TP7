#include <ctype.h>
#include <vector>
#include "EventHandler.h"
#include <iostream>

using namespace std;
Evnt trasformAllegroEvents(int key);



// Transforma eventos de allegro en eventos de mi programa
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

EventHandler::EventHandler()
{
	for (Ev_t& ev : events)
	{
		ev.deactivate();
		ev.time = NULL;
	}
}

// Explicacion del 'getEvent' :

/*
Esta funcion permite tomar eventos de allegro de forma simultanea, ya que al presionar una tecla, se setea el evento (pero no se activa, 
entonces no se ejecuta) y se crea un timer que verifica que pase el tiempo de confirmacion. Cuando llega un evento de timer de allegro,
si no hay eventos activos, pero hay un timer activo, verifica que pase el tiempo necesario para que se confirme que el usuario no apreto
la tecla sin querer (100ms en este caso) y se activa el evento correspondiente.

Por ultimo, cuando se suelta la tecla, se verifica si el tiempo del timer es menor a 100ms y se desruye el timer. Si el tiempo es menor, 
entonces va a flipear hacia donde se le indico. Sino, no hace nada.

*/



bool EventHandler::getEvent(ALLEGRO_EVENT_QUEUE * eq)
{
	ALLEGRO_EVENT ev;
	bool quit = false;

	al_get_next_event(eq, &ev);


	switch (ev.type)
	{
	case ALLEGRO_EVENT_KEY_DOWN:

		if (ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
			quit = true;
		else
			for (int i = 0; i < 2; ++i)			
				if (!this->events[i].active && moveWorm(ev.keyboard.keycode, i) && !this->events[i].timerExist())
					setEvent(trasformAllegroEvents(ev.keyboard.keycode), i);

		break;
	case ALLEGRO_EVENT_KEY_UP:

		for (int i = 0; i < 2; ++i)
			if (this->events[i].timerExist() && this->events[i].Event == trasformAllegroEvents(ev.keyboard.keycode)) {
				if (!this->events[i].timerGreaterThan(100))
				{
					if (!this->events[i].active && this->events[i].Event == LEFT_EV) {
						this->events[i].Event = FLIP_LEFT_EV;
						this->events[i].activate();
					}
					else if(!this->events[i].active && this->events[i].Event == RIGHT_EV) {
						this->events[i].Event = FLIP_RIGHT_EV;
						this->events[i].activate();
					}
				}
			}
					
		break;
	case ALLEGRO_EVENT_TIMER:
		this->setEvent(TIMER_EV, 2);
		this->events[2].activate();


		for (int i = 0; i < 2; ++i)
			if (!this->events[i].active && this->events[i].timerExist())
			{
				this->events[i].time->stop();
				float time_ = this->events[i].time->getTime();
				if ( time_ >= 300)//100)//Debug
				{
					this->events[i].activate();
					this->events[i].time->start();
				}
			}


		
		break;
	case ALLEGRO_EVENT_DISPLAY_CLOSE:
		quit = true;
		break;
	}

	if (quit)
		for (int i = 0; i < 2; i++)
			if (this->events[i].timerExist())
				this->events[i].killTimer();
	
	return !quit;

}

bool EventHandler::isThereEvent()
{
	return this->events[0].active || this->events[1].active || this->events[2].active;
}


// Esta funcion es bastante simple. Yo se que tengo un array de eventos, y que el evento del timer de allegro va ultimo, por lo que paso por todos
// y, si estan activos, los ejecuto y los desactivo
void EventHandler::handleEventDispatcher(Stage& stage)
{
		for (int i =0 ; i <this->events.size() ; i++)
		{
			if (this->events[i].active)
			{
				dispatchEvent(this->events[i].Event, stage,i);
				this->events[i].deactivate();
			}
		}
}

// Este es el dispatcher, y viene con un wormID integrado, que basicamente es el lugar del worm en el vector

void EventHandler::dispatchEvent(Evnt ev, Stage& stage, int wormID = 0)
{
	switch (ev)
	{
	case LEFT_EV: if (stage.worms[wormID].state == STILL) stage.worms[wormID].move(LEFT); break;
	case RIGHT_EV:if (stage.worms[wormID].state == STILL)stage.worms[wormID].move(RIGHT); break;
	case JUMP_EV:if (stage.worms[wormID].state == STILL)stage.worms[wormID].jump(); break;
	case FLIP_LEFT_EV: if (stage.worms[wormID].state == STILL) stage.worms[wormID].flipLeft(); break;
	case FLIP_RIGHT_EV: if (stage.worms[wormID].state == STILL) stage.worms[wormID].flipRight(); break;
	case TIMER_EV:
		stage.draw();
		for (Worm& worm : stage.worms)
		{
			worm.update();
			worm.draw();
		}
	}

}

void EventHandler::setEvent(Evnt ev, int worm)
{

	this->events[worm].Event = ev;

	if (ev != TIMER_EV)
		this->events[worm].newTimer();
}

bool EventHandler::moveWorm(int ev, int worm)
{
	bool retValue = false;

	switch (worm)
	{
	case 0: retValue = moveWorm1(ev);
		break;
	case 1: retValue = moveWorm2(ev);
		break;
	}
	return retValue;
}

bool EventHandler::moveWorm1(int ev)
{
	return ((ev == ALLEGRO_KEY_LEFT) || (ev == ALLEGRO_KEY_RIGHT) || (ev == ALLEGRO_KEY_UP));
}

bool EventHandler::moveWorm2(int ev)
{
	return ((ev == ALLEGRO_KEY_W) || (ev == ALLEGRO_KEY_A) || (ev == ALLEGRO_KEY_D));
}
