
#include "Network\NetworkEvents.h"

bool getInfoWithTimeout(string msgSend,string& msg, fsmData * fsminfo, bool server)
{

	//server->connect();
	bool error = false;
	bool keep = true;
	Timer countTime;

	while (keep && !error) {
		if (server)
			msg = fsminfo->server->getInfoTimed(TIMEOUT_TIME);
		else
			msg = fsminfo->client->getInfoTimed(TIMEOUT_TIME);
		if (!msg.compare(SERVER_TIMEOUT)) {
			fsminfo->timeouts += 1;
			cout << "Timing out. Number" << fsminfo->timeouts << endl;
			countTime.stop();
			cout <<countTime.getTime() <<"Time passed" << endl;
			countTime.start();

			if (server)
				fsminfo->server->sendMessageTimed(TIMEOUT_TIME, msgSend);
			else
				fsminfo->client->sendMessageTimed(msgSend,TIMEOUT_TIME);
		}
		else
			keep = false;
		if (fsminfo->timeouts == MAXTIMEOUT)
			error = true;
	}
	//server->stop();
	return error;
}

bool getInfoOneTry(string&msg, fsmData * fsminfo, bool server) {

	bool error;
	if (server)
		error = fsminfo->server->getInfoSigle(msg);
	else
		error = fsminfo->client->getInfoSigle(msg);

	return error;
}



NetworkEvents::NetworkEvents(uint16_t wormX)
{
	Ev_t extEv;
	infoForFsm = { extEv,0,false,false,wormX,0 ,NULL,NULL };

}


NetworkEvents::~NetworkEvents()
{
	if (server)
		delete server;
	else if (client)
		delete client;
}

bool NetworkEvents::initClient() {
	bool success = false;
	fsmData * fsminfo;

	fsminfo = (fsmData *)this->fsmCL->getData();
	//Espero a que me llegue un IAM del server
	Packet packet;
	fsminfo->timeouts = 0;
	string msg;

	msg = client->getInfo();
	
	
		packet.setPacket(msg);
		cout << packet << endl;
		if (IAM_HD == packet.getHeader()) {
			fsminfo->wormXOther = packet.getWormX();
			fsmCL->setEvent(IAM_FSM);
		}
		else
			fsmCL->setEvent(ERROR_FSM);
	
	// Me llego perfecto el IAM Entro en la fsm
	do {
		this->fsmCL->run();
		// Aca ya le mande mi IAM y estoy esperando a que me llegue un ACK.
		if (!fsminfo->leave)
		{
			Packet packet;
			fsminfo->timeouts = 0;
			string msg2;

			if (getInfoWithTimeout(packet.createIAM(), msg2, fsminfo, false))
				this->fsmCL->setEvent(ERROR_FSM);
			else {
				packet.setPacket(msg2);
				cout << packet << endl;
				if (ACKQ_HD == packet.getHeader()) {
					fsmCL->setEvent(ACK_FSM);
					success = true;
				}
				else
					fsmCL->setEvent(ERROR_FSM);
			}
			// Me llega el ACK hermoso
		}
	} while (!fsminfo->leave);

	return success;
}

bool NetworkEvents::initServer() {
	bool success = false;
	fsmData * fsminfo = &this->infoForFsm; // ESTO ME TIRA EXCEPTION PERO SI NO LO INICALIZO NO COMPILA

	// PASOS:

	// Mando un IAM al cliente
	Packet packet;
	packet.setPacket(IAM_HD, NOTLOADED, NOTLOADED, fsminfo->wormXMine);
	server->sendMessageTimed(TIMEOUT_TIME,packet.createIAM());

	//cin.get();

	// Espero a que venga un IAM del cliente
	string msg;
	if (getInfoWithTimeout(packet.createIAM(), msg, fsminfo,true))
		this->fsmSE->setEvent(ERROR_FSM);
	else {
		packet.setPacket(msg);
		cout << packet << endl;
		if (IAM_HD == packet.getHeader()) {
			fsminfo->wormXOther = packet.getWormX();
			fsmSE->setEvent(ANS_IAM_FSM);
			success = true;
		}
		else
			fsmSE->setEvent(ERROR_FSM);

		do {
			cout << "FSM Event:" << fsmSE->actualEvent << endl;
			this->fsmSE->run();
			// Dentro de la FSM se manda el ultimo ACK y somos todos felices
		} while (!fsminfo->leave);


	}
	return success;
}






void NetworkEvents::loadServer(Server * server)
{
	this->server = server;
	infoForFsm.server = server;

}

void NetworkEvents::loadClient(Client * client)
{
	this->client = client;
	infoForFsm.client = client;
}

void NetworkEvents::loadFSMClient(fsmC * client)
{
	this->fsmCL = client;
	fsmClient = true;
}

void NetworkEvents::loadFSMServer(fsmS * server)
{
	this->fsmSE = server;
	fsmServer = true;
}

void NetworkEvents::update(void * data)
{
	Ev_t extEv;
	Stage * st = NULL;
	bool great = false;
	if (data != NULL) {
		st = (Stage *)data;
		if (st->getEvetn() != NULL ) {
			extEv = *(Ev_t *)st->getEvetn();
			if (extEv.active)
				great = true;
		}
	}
	fsmData * fsminfo;

	if (fsmClient && data!= NULL &&great) {

		fsminfo = (fsmData *)this->fsmCL->getData();
		fsminfo->ev = extEv;
		this->fsmCL->setEvent(SEND_FSM);
		bool getACK = true;

		do {
			this->fsmCL->run();

			if (getACK)
			{

				Packet packet;
				fsminfo->timeouts = 0;
				string msg;


				if (getInfoWithTimeout(fsminfo->oldPacket, msg, fsminfo, false))
					this->fsmCL->setEvent(ERROR_FSM);
				else {
					packet.setPacket(msg);
					cout << packet << endl;
					if (! (fsminfo->ev.wormID - packet.getWormID()))
						this->fsmCL->setEvent(ACK_FSM);
					else 
						this->fsmCL->setEvent(ERROR_FSM);
				}
				getACK = false;
			}

			fsminfo->oldPacket.clear();

		} while (!fsminfo->leave);


	}
	else if (fsmServer && data != NULL && great) {
		fsminfo = (fsmData *)this->fsmSE->getData();
		fsminfo->ev = extEv;
		this->fsmSE->setEvent(SEND_FSM);
		bool getACK = true;

		do {
			this->fsmSE->run();

			if (getACK) {

				Packet packet;
				fsminfo->timeouts = 0;
				string msg;

				if (getInfoWithTimeout(fsminfo->oldPacket, msg, fsminfo, true))
					this->fsmSE->setEvent(ERROR_FSM);
				else {
					
					packet.setPacket(msg);
					cout << packet << endl;
					if (!(fsminfo->ev.wormID - packet.getWormID()))
						this->fsmSE->setEvent(ACK_FSM);
					else
						this->fsmSE->setEvent(ERROR_FSM);
					
				}
				getACK = false;
				fsminfo->oldPacket.clear();
			}


		} while (!fsminfo->leave);

	}

}

void * NetworkEvents::getEvent(void * data)
{

	int * size = (int*)data;
	retEv.deactivate();
	fsmData * fsminfo;

	if (fsmClient) {

		// El primer evento que tiene que recibir la FSM es de Mandar un ACK, es decir, le llego un evento de MOVE.
		// Despues de eso sale de la FSM.

		fsminfo = (fsmData *)this->fsmCL->getData();

		Packet packet;
		fsminfo->timeouts = 0;
		string msg;


		if (getInfoOneTry( msg, fsminfo,false))
			this->fsmCL->setEvent(NOEVENT_FSM);
		else {
			packet.setPacket(msg);
			cout << packet << endl;
			this->fsmCL->setEvent(MOVE_FSM);
			
			this->retEv = packet.getPacketEvent();
			*size = 1;
			fsminfo->ev = packet.getPacketEvent();
			retEv.activate();
			retEv.wormID = this->wormID;
		}
		do {

			this->fsmCL->run();
		} while (!fsminfo->leave);


	}
	else if (fsmServer) {

		// El primer evento que tiene que recibir la FSM es de Mandar un ACK, es decir, le llego un evento de MOVE.
		// Despues de eso sale de la FSM.

		fsminfo = (fsmData *)this->fsmSE->getData();

		Packet packet;
		fsminfo->timeouts = 0;
		string msg;
	

		if (getInfoOneTry(msg, fsminfo, true))
			this->fsmSE->setEvent(NOEVENT_FSM);
		else {
			this->fsmSE->setEvent(MOVE_FSM);
			packet.setPacket(msg);
			this->retEv = packet.getPacketEvent();
			*size = 1;
			fsminfo->ev = packet.getPacketEvent();
			retEv.activate();
			retEv.wormID = this->wormID;
		}

		do {

			this->fsmSE->run();
		} while (!fsminfo->leave);

	}
	if (!retEv.active) {
		*size = 0;
	}

	return &retEv;

}

void NetworkEvents::loadWormID(uint32_t wormID)
{
	this->wormID = wormID;
}

fsmData * NetworkEvents::getFSMData()
{
	return &infoForFsm;
}

