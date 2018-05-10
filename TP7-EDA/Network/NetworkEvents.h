#pragma once
#include "Client.h"
#include "Server.h"
#include "../FSM.h"
#include"../Observers/Observer.h"
#include"../Controllers/Controller.h"
#include "../Game/Stage.h"
#include "../FSMData.h"

/*
	No me gusta la implementacion de la FSM de networking y aca me suena a que hay algo raro, ya que paso como maximo dos veces por la FSM cuando quiero
	enviar o recibir un paquete. El problema es que no se me ocurre otra forma de hacerlo.

*/




class NetworkEvents : public Observer, public Controller
{
public:
	NetworkEvents(uint16_t wormX);
	~NetworkEvents();

	void loadServer(Server * server);
	void loadClient(Client * client);

	void loadFSM(FSM * fsm);


	bool initServer();
	bool initClient();

	void update(void *data);
	void * getEvent(void * data);

	void loadWormID(uint32_t wormID);

	fsmData * getFSMData();

private:
	Client * client = NULL;
	Server * server = NULL;

	bool fsmLoaded = false;
	FSM * fsm = NULL;
	

	Ev_t retEv;
	fsmData infoForFsm;

	uint32_t wormID;


};

