#include "CursesClass.h"
#include "Allegro/AllegroClass.h"
#include "Events\EventHandler.h"
#include "UserHandler.h"
#include "Game/Stage.h"
#include "Controllers\AllegroEventGetter.h"
#include "Observers\drawStage.h"





int main(int argc ,char * argv[]) {
	// Pregunto si soy server o CLient

	CursesClass *curses = new CursesClass;

	switch (selectMode(*curses)) {
	case SERVER:
		break;
	case CLIENT:
		break;
	case LEAVE:
		break;
	}
	delete curses;

	AllegroClass allegro(1920, 696, 50);
	EventHandler eventHandler;
	Stage stage;



	// Controllers
	AllegroEventGetter allegroEvents(allegro.getEventQueue());
	eventHandler.loadController(&allegroEvents);
	// Falta network controller
	// Hay que cargar el controller de network

	// Observers
	DrawStage drawStage(JUMPFILE, JUMPPICS, WALKFILE, WALKPICS, BACKGROUNDFILE,STAGEFILE);
	stage.addObserver(&drawStage);
	// Falta el observer de network y cargarlo 

	// Worms
	WormData wormData;
	Worm worm1(&wormData);
	stage.createWorms(&worm1);
	//Worm worm2(&wormData);
	//stage.createWorms(&worm2); Lo descomento cuadno tenga networking

	bool quit = false;

	while (!stage.isOver()) {
		eventHandler.getEvent();
		if (eventHandler.areThereActiveEvents()) 
			eventHandler.HandleEventDispatch(stage);
	}



	return 0;
}

