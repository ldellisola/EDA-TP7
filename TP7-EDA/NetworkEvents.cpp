
#include "Network\NetworkEvents.h"

bool getInfoWithTimeout(Server * server,string& msg, fsmData * fsminfo)
{
	bool error = false;
	bool keep = true;
	while (keep && !error) {
		msg = server->getInfoTimed(20);
		if (!msg.compare(SERVER_TIMEOUT))
			fsminfo->timeouts += 1;
		else
			keep = false;
		if (fsminfo->timeouts == MAXTIMEOUT)
			error = true;
	}
	return error;
}

NetworkEvents::NetworkEvents(int16_t wormX)
{
	Ev_t extEv;
	infoForFsm = { extEv,0,false,false,wormX,0 ,NULL,NULL };

}


NetworkEvents::~NetworkEvents()
{
}

bool NetworkEvents::initClient() {
	bool success = false;
	fsmData * fsminfo;

	fsminfo = (fsmData *)this->fsmCL->getData();
	//Espero a que me llegue un IAM del server
	Packet packet;
	fsminfo->timeouts = 0;
	string msg;

	if (getInfoWithTimeout(server, msg, fsminfo))
		this->fsmCL->setEvent(ERROR_FSM);
	else {
		packet.setPacket(msg);
		if (IAM_HD == packet.getHeader()) {
			fsminfo->wormXOther = packet.getWormX();
			fsmCL->setEvent(IAM_FSM);
		}
		else
			fsmCL->setEvent(ERROR_FSM);
	}
	// Me llego perfecto el IAM Entro en la fsm
	do {
		this->fsmCL->run();
		// Aca ya le mande mi IAM y estoy esperando a que me llegue un ACK.
		Packet packet;
		fsminfo->timeouts = 0;
		string msg;

		if (getInfoWithTimeout(server, msg, fsminfo))
			this->fsmCL->setEvent(ERROR_FSM);
		else {
			packet.setPacket(msg);
			if (ACKQ_HD == packet.getHeader()) {
				fsmCL->setEvent(ACK_FSM);
				success = true;
			}
			else
				fsmCL->setEvent(ERROR_FSM);
			// Me llega el ACK hermoso

		}
	} while (!fsminfo->leave);

	return success;
}

bool NetworkEvents::initServer() {
	bool success = false;
	fsmData * fsminfo = NULL; // ESTO ME TIRA EXCEPTION PERO SI NO LO INICALIZO NO COMPILA

	// PASOS:

	// Mando un IAM al cliente
	Packet packet;
	packet.setPacket(IAM_HD, NOTLOADED, NOTLOADED, fsminfo->wormXMine);
	client->sendMessage(packet.createIAM());

	// Espero a que venga un IAM del cliente
	string msg;
	if (getInfoWithTimeout(server, msg, fsminfo))
		this->fsmSE->setEvent(ERROR_FSM);
	else {
		packet.setPacket(msg);
		if (IAM_HD == packet.getHeader()) {
			fsminfo->wormXOther = packet.getWormX();
			fsmSE->setEvent(ANS_IAM_FSM);
			success = true;
		}
		else
			fsmSE->setEvent(ERROR_FSM);

		do {
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
	fsmData * fsminfo;

	if (fsmClient && data!= NULL) {
		extEv = *(Ev_t *)data;

		fsminfo = (fsmData *)this->fsmCL->getData();
		fsminfo->ev = extEv;
		this->fsmCL->setEvent(SEND_FSM);

		do {
			this->fsmCL->run();
			
			Packet packet;
			fsminfo->timeouts = 0;
			string msg;


			if (getInfoWithTimeout(server, msg, fsminfo))
				this->fsmCL->setEvent(ERROR_FSM);
			else {
				packet.setPacket(msg);
				if(!(fsminfo->ev.wormID - packet.getWormID()))
					this->fsmCL->setEvent(ACK_FSM);
				else
					this->fsmCL->setEvent(ERROR_FSM);

			}


		} while (!fsminfo->leave);


	}
	else if (fsmServer && data != NULL) {
		extEv = *(Ev_t *)data;
		fsminfo = (fsmData *)this->fsmSE->getData();
		fsminfo->ev = extEv;
		this->fsmCL->setEvent(SEND_FSM);

		do {
			this->fsmCL->run();

			Packet packet;
			fsminfo->timeouts = 0;
			string msg;

			if (getInfoWithTimeout(server, msg, fsminfo))
				this->fsmSE->setEvent(ERROR_FSM);
			else {
				this->fsmSE->setEvent(ACK_FSM);
				packet.setPacket(msg);
				fsminfo->ev.wormID = packet.getWormID();
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


		if (getInfoWithTimeout(server, msg, fsminfo))
			this->fsmCL->setEvent(ERROR_FSM);
		else {
			packet.setPacket(msg);
			this->fsmCL->setEvent(MOVE_FSM);
			
			this->retEv = packet.getPacketEvent();
			*size = 1;
			fsminfo->ev = packet.getPacketEvent();
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
	

		if (getInfoWithTimeout(server, msg, fsminfo))
			this->fsmSE->setEvent(ERROR_FSM);
		else {
			this->fsmSE->setEvent(MOVE_FSM);
			packet.setPacket(msg);
			this->retEv = packet.getPacketEvent();
			*size = 1;
			fsminfo->ev = packet.getPacketEvent();
		}

		do {

			this->fsmSE->run();
		} while (!fsminfo->leave);

	}

	return &retEv;

}

fsmData NetworkEvents::getFSMData()
{
	return infoForFsm;
}

