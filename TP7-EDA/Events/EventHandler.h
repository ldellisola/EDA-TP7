#include <allegro5\allegro.h>
#include <vector>
#include "Timer.h"
#include "Stage.h"

using namespace std;


// Este enum tiene todos los eventos posibles que puede recibir el programa. Si se quiere usar este EventHandler en otro programa solo habraiq eu cambiar 
// esta parte y un poco del codigo en el .cpp



class EventHandler {
public:
	EventHandler();
	
	bool getEvent(ALLEGRO_EVENT_QUEUE *  eq);		// Gets the event from the event queue and/or other mediums not yet implemented
	bool isThereEvent();							// Checks if there is an active event
	void handleEventDispatcher(Stage& stage);		// Dispatches multiple events
private:

	void setEvent(Evnt ev, int worm);				// Sets the event but it does not activate it.
	void dispatchEvent(Evnt ev, Stage& stage, int wormID);		// The actual dispatcher for every single event
	bool moveWorm(int ev, int worm);				// It decides if the key pressed correspond to a given worm
	bool moveWorm1(int ev);
	bool moveWorm2(int ev);

	array<Ev_t, 3>events;
};

