#pragma once
#include "Client.h"
#include "Server.h"
#include "../FSM Client/fsmClient.h"
#include "../FSM Server/fsmServer.h"
#include"../Observers/Observer.h"
#include"../Controllers/Controller.h"

/*
	No me gusta la implementacion de la FSM de networking y aca me suena a que hay algo raro, ya que paso como maximo dos veces por la FSM cuando quiero
	enviar o recibir un paquete. El problema es que no se me ocurre otra forma de hacerlo.



*/


class NetworkEvents : public Observer, public Controller
{
public:
	NetworkEvents(int16_t wormX);
	~NetworkEvents();

	void loadServer(Server * server);
	void loadClient(Client * client);

	void loadFSMClient(fsmC * client);
	void loadFSMServer(fsmS * server);

	bool initServer();
	bool initClient();

	void update(void *data);
	void * getEvent(void * data);

	fsmData getFSMData();

private:
	Client * client = NULL;
	Server * server = NULL;

	bool fsmClient = false;
	fsmC * fsmC = NULL;

	bool fsmServer = false;
	fsmS * fsmS = NULL;

	Ev_t retEv;
	fsmData infoForFsm;


};

