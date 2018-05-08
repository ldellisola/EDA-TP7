
#include "Network\NetworkEvents.h"

bool getInfoWithTimeout(string msgSend,string& msg, fsmData * fsminfo, bool server)
{

	bool error = false;
	bool keep = true;
	Timer countTime;

	while (keep && !error) {
		if (fsminfo->server)
			msg = fsminfo->server->getInfoTimed(TIMEOUT_TIME);
		else if (fsminfo->client)
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
	return error;
}

bool getInfoOneTry(string&msg, fsmData * fsminfo) {

	bool error;
	if (fsminfo->server)
		error = fsminfo->server->getInfoSingleTry(msg);
	else if (fsminfo->client)
		error = fsminfo->client->getInfoSingleTry(msg);

	return error;
}



NetworkEvents::NetworkEvents(uint16_t wormX)
{
	Ev_t extEv;
	extEv.deactivate();
	infoForFsm = { extEv,extEv,0,false,false,wormX,0 ,NULL,NULL };

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

	fsminfo = (fsmData *)this->fsm->getData();
	//Espero a que me llegue un IAM del server
	Packet packet;
	fsminfo->timeouts = 0;
	string msg;

	msg = client->getInfo();
	
	
		packet.setPacket(msg);
		cout << packet << endl;
		if (IAM_HD == packet.getHeader()) {
			fsminfo->wormXOther = packet.getWormX();
			fsm->setEvent(IAM_FSM);
		}
		else
			fsm->setEvent(ERROR_FSM);
	
	// Me llego perfecto el IAM Entro en la fsm
	do {
		this->fsm->run();
		// Aca ya le mande mi IAM y estoy esperando a que me llegue un ACK.
		if (!fsminfo->leave)
		{
			Packet packet;
			fsminfo->timeouts = 0;
			string msg2;

			if (getInfoWithTimeout(packet.createIAM(), msg2, fsminfo, false))
				this->fsm->setEvent(ERROR_FSM);
			else {
				packet.setPacket(msg2);
				cout << packet << endl;
				if (ACKQ_HD == packet.getHeader()) {
					fsm->setEvent(ACK_FSM);
					success = true;
					fsminfo->leave = true;
				}
				else
					fsm->setEvent(ERROR_FSM);
			}
			// Me llega el ACK hermoso
		}
	} while (!fsminfo->leave);

	return success;
}

bool NetworkEvents::initServer() {
	bool success = false;
	fsmData * fsminfo = &this->infoForFsm; 

	// PASOS:

	// Mando un IAM al cliente
	Packet packet;
	packet.setPacket(IAM_HD, NOTLOADED, NOTLOADED, fsminfo->wormXMine);
	server->sendMessageTimed(TIMEOUT_TIME,packet.createIAM());

	//cin.get();

	// Espero a que venga un IAM del cliente
	string msg;
	if (getInfoWithTimeout(packet.createIAM(), msg, fsminfo,true))
		this->fsm->setEvent(ERROR_FSM);
	else {
		packet.setPacket(msg);
		cout << packet << endl;
		if (IAM_HD == packet.getHeader()) {
			fsminfo->wormXOther = packet.getWormX();
			fsm->setEvent(ANS_IAM_FSM);
			success = true;
		}
		else
			fsm->setEvent(ERROR_FSM);

		do {
			cout << "FSM Event:" << fsm->actualEvent << endl;
			this->fsm->run();
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

void NetworkEvents::loadFSM(FSM * fsm)
{
	this->fsm = fsm;
	fsmLoaded = true;
}



void NetworkEvents::update(void * data)
{
	Ev_t extEv;
	Stage * st = NULL;
	bool activeEvent = false;
	if (data != NULL) {
		st = (Stage *)data;
		if (st->getEvetn() != NULL) {
			extEv = *(Ev_t *)st->getEvetn();
			if (extEv.active)
				activeEvent = true;
		}
	}


	if (activeEvent) {
		fsmData * fsminfo = (fsmData *)this->fsm->getData();
		fsminfo->ev = extEv;
		this->fsm->setEvent(SEND_FSM);
		bool getACK = true;

		do {
			this->fsm->run();

			if (getACK) {

				Packet packet;
				fsminfo->timeouts = 0;
				string msg;

				if (getInfoWithTimeout(fsminfo->oldPacket, msg, fsminfo, true)) {
					this->fsm->setEvent(ERROR_FSM);
					getACK = false;
				}
				else {

					packet.setPacket(msg);
					cout << packet << endl;
					fsminfo->ev.wormID = packet.getWormID();
					if (packet.getHeader() == ACK_HD) {
						this->fsm->setEvent(ACK_FSM);
						fsminfo->leave = false;
						getACK = false;
						fsminfo->oldPacket.clear();
					}
					else {
						fsminfo->backup = packet.getPacketEvent();
						fsminfo->backup.activate();
						this->fsm->setEvent(MOVE_FSM);
					}
				}
				
				
			}
		} while (!fsminfo->leave);
	}
}

void * NetworkEvents::getEvent(void * data)
{

	int * size = (int*)data;
	retEv.deactivate();

	// El primer evento que tiene que recibir la FSM es de Mandar un ACK, es decir, le llego un evento de MOVE.
	// Despues de eso sale de la FSM.

	fsmData * fsminfo = (fsmData *)this->fsm->getData();

	Packet packet;
	fsminfo->timeouts = 0;
	string msg;

	if (getInfoOneTry(msg, fsminfo)) {
		if (fsminfo->backup.active) {
			this->fsm->setEvent(MOVE_FSM);
			this->retEv = fsminfo->backup;
			fsminfo->backup.deactivate();
		}else
			this->fsm->setEvent(NOEVENT_FSM);
	}
	else {
		
		packet.setPacket(msg);
		if (packet.getHeader() == MOVE_HD) {
			this->fsm->setEvent(MOVE_FSM);
			this->retEv = packet.getPacketEvent();
			*size = 1;
			fsminfo->ev = packet.getPacketEvent();
			retEv.activate();
			retEv.wormID = this->wormID;
		}
		else if (packet.getHeader() == ACK_HD)
		{
			this->fsm->setEvent(ACK_FSM);
		}
		else
			this->fsm->setEvent(ERROR_FSM);
	}

	do {
		this->fsm->run();
	} while (!fsminfo->leave);


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

