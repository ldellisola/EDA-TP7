#include "CursesClass.h"
#include "Allegro/AllegroClass.h"
#include "Events\EventHandler.h"
#include "UserHandler.h"
#include "Game/Stage.h"
#include "Controllers\AllegroEventGetter.h"
#include "Observers\drawStage.h"
#include "Network\NetworkEvents.h"


#define WORM_S (12345)
#define WORM_C (67890)

#define initialWormX 1000

#define PORT "15667"


int main(int argc ,char * argv[]) {
	// Pregunto si soy server o CLient

	CursesClass *curses = new CursesClass;
	uint32_t id1;
	uint32_t id2;
	
	NetworkEvents networkEvents(initialWormX);
	Client client;
	networkEvents.loadClient(&client);
	Server server(PORT);
	networkEvents.loadServer(&server);

	fsmData fsmdata = networkEvents.getFSMData();
	bool run;
	void * fsmPointer = NULL;
	AllegroClass allegro(1920, 696, 50);
	
	switch (selectmode(allegro.getEventQueue())) {
	case SERVER:


		id1 = WORM_S;
		id2 = WORM_C;
		fsmPointer= (void *)new fsmS(notREADY_s, waitEVENT_s, waitACK_s,(void *)&fsmdata);
		networkEvents.loadFSMServer((fsmS *)fsmPointer);
		networkEvents.initServer();
		run = true;
		break;
	case CLIENT:
		id2 = WORM_S;
		id1 = WORM_C;
		fsmPointer = (void *) new fsmC(notREADY_s, waitEVENT_s, waitACK_s, (void *)&fsmdata);
		networkEvents.loadFSMClient((fsmC *)fsmPointer);
		networkEvents.initClient();
		run = true;
		break;
	case LEAVE:
		run = false;
		break;
	}
	delete curses;

	if (run)	//Agrego condicional en caso de se salga de la pantalla de inicio, no se haga nada
	{
		EventHandler eventHandler;
		Stage stage(id1, id2);



		// Controllers
		AllegroEventGetter allegroEvents(allegro.getEventQueue());
		eventHandler.loadController(&allegroEvents);
		// Falta network controller
		// Hay que cargar el controller de network

		// Observers
		DrawStage drawStage(JUMPFILE, JUMPPICS, WALKFILE, WALKPICS, BACKGROUNDFILE, STAGEFILE);
		stage.addObserver(&drawStage);
		// Falta el observer de network y cargarlo 

		// Worms
		WormData wormData;
		Worm worm1(&wormData, id1);
		stage.createWorms(&worm1);
		//Worm worm2(&wormData);
		//stage.createWorms(&worm2,id2); Lo descomento cuadno tenga networking

		bool quit = false;

		while (!stage.isOver()) {
			eventHandler.getEvent();
			if (eventHandler.areThereActiveEvents())
				eventHandler.HandleEventDispatch(stage);
		}


	}
	return 0;
}

