#include "..\Events\EventHandler.h"



EventHandler::EventHandler()
{
}


EventHandler::~EventHandler()
{
}

void EventHandler::loadController(Controller * cont)
{
	controllers.push_back(cont);
}

void EventHandler::getEvent()		// El networking tambien tiene que tener un controller que le de eventos. hay que usar herencia con controller
{									//  y que el puntero a void sea un int que te da la cantidad de elementos que hay;
	int size = 0;
	for (Controller * controller : controllers) {
		Ev_t * evs = (Ev_t *) controller->getEvent(&size);
		for (int i = 0; i < size; i++)
				events.push_back(evs[i]);
	}
}

bool EventHandler::areThereActiveEvents()
{
	
	return (events.size() == 0 ? false: true);
}

Ev_t * EventHandler::returnEvent(int * size)
{
	Ev_t retValue[5];
	for (int i = 0; i < 5; i++)
		retValue->deactivate();
	

	for (int i = 0 ;i < 5 && !events.empty() ; i++){
		retValue[i] = events.front();
		events.pop_front();
		*size = i+1;
	}
	return retValue;
}

void EventHandler::displatchEvent(Ev_t & ev, Stage& stage)
{
	switch (ev.Event) {
	case LEFT_EV: stage.wormMoveLeft(ev.wormID); break;
	case RIGHT_EV: stage.wormMoveRight(ev.wormID); break;
	case JUMP_EV: stage.wormJump(ev.wormID); break;
	case FLIP_RIGHT_EV: stage.wormFlipRight(ev.wormID); break;
	case FLIP_LEFT_EV: stage.wormFlipLeft(ev.wormID); break;
	case QUIT_EV: stage.quit(); break;
	case TIMER_EV: stage.refresh();	break;
	}
}


void EventHandler::HandleEventDispatch(Stage& stage)
{
	while (events.size()) {
		if (events.begin()->Event != TIMER_EV && events.begin()->Event != NOEVENT) {
			stage.addEvent(*events.begin());
		}
		displatchEvent(*events.begin(), stage);
		events.pop_front();
	}
}

