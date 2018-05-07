#include "Allegro/AllegroClass.h"
#include "Events\EventHandler.h"
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
using namespace std;

/*
PROBLEMAS:

	Hay que hacer que el worm siga caminando cuando mantenes apretado

	Tira mucho cancer cuando enviamos el evento de vuelta cada vez que hay timeout


*/


int main(int argc ,char * argv[]) {

	Parser parser;
	parser.Read(argc, argv);

	// Agarro las ips del archivo y asigno los valores que viene por comando.
	IPData ips;
	ips.ip = parser.myIP;
	ips.imServer = parser.initialMachine;
	ips.init(IPFILE);
	
	NetworkEvents networkEvents(initialWormX);
	
	bool run = false;
	FSM fsm(notREADY, waitEVENT, waitACK, (void *)networkEvents.getFSMData());
	networkEvents.loadFSM(&fsm);
	uint32_t id1;
	uint32_t id2;

	//Dependiendo si la maquina es server o client, va a inicializar todo acorde

	if (ips.imServer) {
		Server * server = new Server(PORT);
		networkEvents.loadServer(server);
		server->connect();
		id1 = WORM_S;
		id2 = WORM_C;
		run = networkEvents.initServer();		
	}
	else {
		Client * client= new Client(ips.getOtherIP(), PORT);
		networkEvents.loadClient(client);
		client->link();
		id1 = WORM_C;
		id2 = WORM_S;
		run = networkEvents.initClient();
	}

	bool * stopprogram = (bool*)&(networkEvents.getFSMData()->exitProgram);

	if (run)	
	{
		AllegroClass allegro(1920, 696, 50);
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


	cout << "Program ended. Press 'enter' to continue" << endl;
	cin.get();
	return 0;
}

