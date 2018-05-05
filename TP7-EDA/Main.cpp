#include "CursesClass.h"
#include "Allegro/AllegroClass.h"
#include "Events\EventHandler.h"
#include "UserHandler.h"
#include "Game/Stage.h"
#include "Controllers\AllegroEventGetter.h"
#include "Observers\drawStage.h"
#include "Network\NetworkEvents.h"
#include "Network\IPData.h"
#include "Parser\Parser.h"

#define WORM_S (12345)
#define WORM_C (67890)

#define IPFILE "direcciones.txt"

#define initialWormX 1000

#define PORT "15667"


int main(int argc ,char * argv[]) {
	// Pregunto si soy server o CLient

	Parser parser;
	parser.Read(argc, argv);

	
	uint32_t id1;
	uint32_t id2;

	IPData ips;
	ips.ip = parser.myIP;
	ips.imServer = parser.initialMachine;
	ips.init(IPFILE);
	
	NetworkEvents networkEvents(initialWormX);
	


	//fsmData fsmdata = networkEvents.getFSMData();
	bool run = false;
	void * fsmPointer = NULL;

	if (ips.imServer) {
		Server *server = new Server(PORT);
		networkEvents.loadServer(server);
		server->connect();
		id1 = WORM_S;
		id2 = WORM_C;
		fsmPointer = (void *)new fsmS(notREADY_s, waitEVENT_s, waitACK_s, (void *)networkEvents.getFSMData());
		networkEvents.loadFSMServer((fsmS *)fsmPointer);
		run = networkEvents.initServer();

	}
	else {
		Client *client = new Client(ips.getOtherIP(), PORT);
		networkEvents.loadClient(client);
		client->link();
		id1 = WORM_C;
		id2 = WORM_S;
		fsmPointer = (void *) new fsmC(notREADY_c, waitEVENT_c, waitACK_c, (void *)networkEvents.getFSMData());
		networkEvents.loadFSMClient((fsmC *)fsmPointer);
		run = networkEvents.initClient();
		
	}



	
	AllegroClass allegro(1920, 696, 50);
	//switch (selectmode(allegro.getEventQueue())) {
	//case SERVER:


	//	id1 = WORM_S;
	//	id2 = WORM_C;
	//	fsmPointer= (void *)new fsmS(notREADY_s, waitEVENT_s, waitACK_s,(void *)&fsmdata);
	//	networkEvents.loadFSMServer((fsmS *)fsmPointer);
	//	networkEvents.initServer();
	//	run = true;
	//	break;
	//case CLIENT:
	//	id2 = WORM_S;
	//	id1 = WORM_C;
	//	fsmPointer = (void *) new fsmC(notREADY_s, waitEVENT_s, waitACK_s, (void *)&fsmdata);
	//	networkEvents.loadFSMClient((fsmC *)fsmPointer);
	//	networkEvents.initClient();
	//	run = true;
	//	break;
	//case LEAVE:
	//	run = false;
	//	break;
	//}


	if (run)	//Agrego condicional en caso de se salga de la pantalla de inicio, no se haga nada
	{
		
		EventHandler eventHandler;
		Stage stage(id1, id2);



		// Controllers
		AllegroEventGetter allegroEvents(allegro.getEventQueue());
		eventHandler.loadController(&allegroEvents);
		eventHandler.loadController(&networkEvents);
		

		// Observers
		DrawStage drawStage(JUMPFILE, JUMPPICS, WALKFILE, WALKPICS, BACKGROUNDFILE, STAGEFILE);
		stage.addObserver(&drawStage);
		stage.addObserver(&networkEvents);
		

		// Worms
		WormData wormData;
		Worm worm1(&wormData, id1);
		stage.createWorms(&worm1);
		Worm worm2(&wormData,id2);
		stage.createWorms(&worm2); 

		bool quit = false;

		while (!stage.isOver()) {
			eventHandler.getEvent();
			if (eventHandler.areThereActiveEvents())
				eventHandler.HandleEventDispatch(stage);
		}


	}
	delete fsmPointer;
	return 0;
}

