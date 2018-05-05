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
#include "FSMData.h"

#define WORM_S (12345)
#define WORM_C (67890)

#define IPFILE "direcciones.txt"

#define initialWormX 1000

#define PORT "15667"

/*
PROBLEMAS:

	Si tengo el controller de Network, allegro no funciona. (Pasa cada mucho tiempo por la funcion (creo)
	/// Cambie la forma en la que obtenemos informacion. Ahora no hay timeout cuando uso la funcion getEvent




	No asigno wormID al evento.
	// Ya lo implemente para Networking y Allegro.



	Cuando el server no tiene controller de Network, no tira cancer a la otra computadora.
	

	Creo que resolvi lo de salir, es medio un quilombo de punteros
	//Hay que implementar que se cierre le programa cuando hay un error de networking.





*/


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
	bool * stopprogram = NULL;
	if (ips.imServer) {
		Server * server = new Server(PORT);
		networkEvents.loadServer(server);
		server->connect();
		id1 = WORM_S;
		id2 = WORM_C;
		fsmPointer = (void *)new fsmS(notREADY_s, waitEVENT_s, waitACK_s, (void *)networkEvents.getFSMData());
		networkEvents.loadFSMServer((fsmS *)fsmPointer);
		run = networkEvents.initServer();
		//stopprogram = &(((fsmData*)(((fsmS *)fsmPointer)->getData()))->exitProgram);	//creo que este quilombo de casteos me deja la direccion del bool (es tarde y no se me ocurrio otra cosa)
		
	}
	else {
		Client client(ips.getOtherIP(), PORT);
		networkEvents.loadClient(&client);
		client.link();
		id1 = WORM_C;
		id2 = WORM_S;
		fsmPointer = (void *) new fsmC(notREADY_c, waitEVENT_c, waitACK_c, (void *)networkEvents.getFSMData());
		networkEvents.loadFSMClient((fsmC *)fsmPointer);
		run = networkEvents.initClient();
		//stopprogram = &(((fsmData*)(((fsmC *)fsmPointer)->getData()))->exitProgram);
	}

	stopprogram = (bool*)&(networkEvents.getFSMData()->exitProgram);	//creo que terminan apuntando a lo mismo y con menos quilombo de punteros mas bonito

	
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
		allegroEvents.loadWormID(id1);
		eventHandler.loadController(&allegroEvents);
		networkEvents.loadWormID(id2);
		eventHandler.loadController(&networkEvents);
		

		// Observers
		DrawStage drawStage(JUMPFILE, JUMPPICS, WALKFILE, WALKPICS, BACKGROUNDFILE, STAGEFILE);
		stage.addObserver(&drawStage);
		stage.addObserver(&networkEvents);
		

		// Worms
		WormData wormData;
		Worm worm1(&wormData, id1, networkEvents.getFSMData()->wormXMine);
		stage.createWorms(&worm1);
		Worm worm2(&wormData,id2, networkEvents.getFSMData()->wormXOther);
		stage.createWorms(&worm2); 

		bool quit = false;
		
		while ((!stage.isOver())&&!(*stopprogram)) {		//usa el puntero a exitprogram a ver si tiene que salir
			eventHandler.getEvent();
			if (eventHandler.areThereActiveEvents())
				eventHandler.HandleEventDispatch(stage);
		}


	}

	delete fsmPointer;
	getchar();
	return 0;
}

