#pragma once
#include "Client.h"
#include "Server.h"
#include "../FSM Client/fsmClient.h"
#include "../FSM Server/fsmServer.h"
#include"../Observers/Observer.h"
#include"../Controllers/Controller.h"


class NetworkEvents : public Observer, public Controller
{
public:
	NetworkEvents(int16_t wormX);
	~NetworkEvents();

	bool init();

	void loadServer(Server * server);
	void loadClient(Client * client);

	void update(void *data);
	void * getEvent(void * data);

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

