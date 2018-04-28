
#include "AllegroClass.h"
#include "Events\EventHandler.h"
#include "Stage.h"
#include "Controllers\AllegroEventGetter.h"
#include "Observers\drawStage.h"





int main(int argc ,char * argv[]) {

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