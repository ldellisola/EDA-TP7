#include "eventsFSM.h"

//intial state.



STATE notREADY[] = {
	{ ANS_IAM_FSM , waitEVENT,notReady_AnswerIAM },
{ SEND_FSM,notREADY,failedcom },
{ MOVE_FSM,notREADY,failedcom },
{ ACK_FSM,notREADY,failedcom },
{ QUIT_FSM,notREADY,failedcom },
{ ERROR_FSM,notREADY,errorfun },
{ IAM_FSM,waitEVENT,notReady_ReadyRecieved },
{ NOEVENT_FSM,notREADY,failedcom },
{ END,notREADY,reset },
};

STATE waitEVENT[] = {
{ ANS_IAM_FSM , notREADY,failedcom },
{ SEND_FSM,waitACK,waitEvent_SendEvent },
{ MOVE_FSM,waitEVENT,waitEvent_GetEvent },
{ ACK_FSM,waitEVENT,waitEvent_NoEvent },
{ QUIT_FSM,notREADY,waitEvent_QuitRecieved },
{ ERROR_FSM,notREADY,errorfun },
{ NOEVENT_FSM,waitEVENT,waitEvent_NoEvent },
{ IAM_FSM, notREADY,failedcom },
{ END,notREADY,reset },
};

STATE waitACK[] = {
{ ANS_IAM_FSM, notREADY,failedcom },
{ MOVE_FSM,notREADY,waitEvent_NoEvent },
{ SEND_FSM,notREADY,waitEvent_NoEvent },
{ ACK_FSM,waitEVENT,waitAck_AckRecieved },
{ QUIT_FSM,notREADY, failedcom },
{ ERROR_FSM,notREADY,errorfun },
{ IAM_FSM,notREADY,failedcom },
{ NOEVENT_FSM,notREADY,failedcom },
{ END,notREADY,reset },
};



int8_t TransformEvent(Evnt ev) {
	switch (ev) {
	case LEFT_EV:
		return 'l';
	case RIGHT_EV:
		return 'r';
	case JUMP_EV:
		return 'j';
	case TOGGLE_EV:
		return 't';
	}

}

void waitEvent_NoEvent(void*data) {
	//cout << "No Event was Recieved" << endl;
}


void notReady_AnswerIAM(void * data) {
	cout << "IAM recieved. Sending ACK" << endl;
	fsmData * pointer = (fsmData *)data;
	Packet packet;
	packet.setPacket(ACKQ_HD, NOTLOADED, 0, NOTLOADED);
	if (pointer->server)
		pointer->server->sendMessageTimed(TIMEOUT_TIME, packet.createACKQ());
	if (pointer->client)
		pointer->client->sendMessageTimed(packet.createACKQ(), TIMEOUT_TIME);
	pointer->leave = true;
	//	cout << "Leaving FSM" << endl;
}

void notReady_ReadyRecieved(void * data)
{
	cout << "IAM recieved. Answering Handshake" << endl;
	fsmData * pointer = (fsmData *)data;
	Packet packet;
	packet.setPacket(IAM_HD, NOTLOADED, NOTLOADED, pointer->wormXMine);
	
	if (pointer->server)
		pointer->server->sendMessageTimed(TIMEOUT_TIME,packet.createIAM());
	if (pointer->client)
		pointer->client->sendMessageTimed(packet.createIAM(), TIMEOUT_TIME);

}
///OK
void waitEvent_SendEvent(void * data)
{// Tengo que mandar el evento que me llego. Lo tengo en el puntero data.
	fsmData * pointer = (fsmData *)data;
	Packet packet;

	packet.setPacket(MOVE_HD, TransformEvent((pointer->ev.Event)), pointer->ev.wormID);

	pointer->oldPacket = packet.createMOVE();
	
	if (pointer->server)
		pointer->server->sendMessageTimed(TIMEOUT_TIME, pointer->oldPacket);
	if (pointer->client)
		pointer->client->sendMessageTimed(pointer->oldPacket, TIMEOUT_TIME);

	cout << "Local event sent. Waiting for ACK" << endl;
	pointer->leave = false;
}
///OK
void waitEvent_GetEvent(void * data)
{
	fsmData * pointer = (fsmData *)data;
	Packet packet;

	cout << "Event received. Sending ACK" << endl;
	packet.setPacket(ACK_HD, NOTLOADED, pointer->ev.wormID);
	if (pointer->server)
		pointer->server->sendMessageTimed(TIMEOUT_TIME, packet.createACK());
	if (pointer->client)
		pointer->client->sendMessageTimed(packet.createACK(), TIMEOUT_TIME);

	pointer->leave = true;
	pointer->exitProgram = false;

	// Llega un evento y le tengo que contestar al porgrama con un ID igual al que me mando
}
///OK
void waitEvent_QuitRecieved(void * data)
{
	fsmData * pointer = (fsmData *)data;
	Packet packet;
	cout << "Quit Recieved. Answering ACK and leaving the program." << endl;
	if (pointer->server)
		pointer->server->sendMessageTimed(TIMEOUT_TIME, packet.createACKQ());
	if (pointer->client)
		pointer->client->sendMessageTimed(packet.createACKQ(), TIMEOUT_TIME);
	pointer->leave = true;
	pointer->exitProgram = true;
}
///OK
void waitAck_AckRecieved(void * data)
{
	cout << "ACK recieved and matched the ID. Leaving FSM" << endl;
	fsmData * pointer = (fsmData *)data;
	pointer->leave = true;
}



void errorfun(void * data)
{
	cout << "There was an error. Shutting down" << endl;
	fsmData * pointer = (fsmData *)data;
	Packet packet;
	if (pointer->server)
		pointer->server->sendMessageTimed(TIMEOUT_TIME,packet.createERROR());
	else if (pointer->client)
		pointer->client->sendMessageTimed(packet.createERROR(),TIMEOUT_TIME);
	pointer->leave = true;
	pointer->error = true;
	pointer->exitProgram = true;
}

void failedcom(void * data)
{
	cout << "Network events are out of sync. Shutting down" << endl;
	fsmData * pointer = (fsmData *)data;
	Packet packet;
	if (pointer->server)
		pointer->server->sendMessageTimed(TIMEOUT_TIME, packet.createERROR());
	else if (pointer->client)
		pointer->client->sendMessageTimed(packet.createERROR(), TIMEOUT_TIME);
	pointer->leave = true;
	pointer->error = true;
	pointer->exitProgram = true;
}

void reset(void * data)
{
	cout << "Closing" << endl;
	fsmData * pointer = (fsmData *)data;
	Packet packet;
	if (pointer->server)
		pointer->server->sendMessageTimed(TIMEOUT_TIME, packet.createERROR());
	else if (pointer->client)
		pointer->client->sendMessageTimed(packet.createERROR(), TIMEOUT_TIME);
	pointer->leave = true;
	pointer->exitProgram = true;
}

