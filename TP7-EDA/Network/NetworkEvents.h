#pragma once
#include "Client.h"
#include "Server.h"
#include "../FSM Client/fsmClient.h"
#include "../FSM Server/fsmServer.h"
#include"../Observers/Observer.h"
#include"../Controllers/Controller.h"
#include "../Game/Stage.h"

/*
	No me gusta la implementacion de la FSM de networking y aca me suena a que hay algo raro, ya que paso como maximo dos veces por la FSM cuando quiero
	enviar o recibir un paquete. El problema es que no se me ocurre otra forma de hacerlo.
*/

#define TIMEOUT_TIME (20) // en ms


class NetworkEvents : public Observer, public Controller
{
public:
	NetworkEvents(uint16_t wormX);
	~NetworkEvents();

	void loadServer(Server * server);
	void loadClient(Client * client);

	void loadFSMClient(fsmC * client);
	void loadFSMServer(fsmS * server);

	bool initServer();
	bool initClient();

	void update(void *data);
	void * getEvent(void * data);

	void loadWormID(uint32_t wormID);

	fsmData * getFSMData();

private:
	Client * client = NULL;
	Server * server = NULL;

	bool fsmClient = false;
	fsmC * fsmCL = NULL;	//CAMBIE EL NOMBRE A fsmCL por que sino no compilaba

	bool fsmServer = false;
	fsmS * fsmSE = NULL;	//CAMBIE EL NOMBRE A fsmSE por que sino no compilaba

	Ev_t retEv;
	fsmData infoForFsm;

	uint32_t wormID;


};

