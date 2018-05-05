#include "eventsServer.h"
#include "..\FSM Client\eventsClient.h"

//intial state.



STATE notREADY_s[] = {
{ ANS_IAM_FSM , waitEVENT_s,notReady_AnswerIAM_s },
{ SEND_FSM,notREADY_s,failedcom_s },
{ MOVE_FSM,notREADY_s,failedcom_s },
{ ACK_FSM,notREADY_s,failedcom_s },
{ QUIT_FSM,notREADY_s,failedcom_s },
{ ERROR_FSM,notREADY_s,errorfun_s },
{ IAM_FSM,waitEVENT_s,notReady_ReadyRecieved_s },
{ NOEVENT_FSM,notREADY_s,failedcom_s},
{ END,notREADY_s,reset_s },
};

STATE waitEVENT_s[] = {
{ ANS_IAM_FSM , notREADY_s,failedcom_s },
{ SEND_FSM,waitACK_s,waitEvent_SendEvent_s },
{ MOVE_FSM,waitEVENT_s,waitEvent_GetEvent_s },
{ ACK_FSM,notREADY_s,failedcom_s },
{ QUIT_FSM,notREADY_s,waitEvent_QuitRecieved_s},
{ ERROR_FSM,notREADY_s,errorfun_s },
{ NOEVENT_FSM,waitEVENT_s,waitEvent_NoEvent_s},
{ IAM_FSM, notREADY_s,failedcom_s },
{ END,notREADY_s,reset_s },
};

STATE waitACK_s[] = {
{ ANS_IAM_FSM, notREADY_s,failedcom_s },
{ MOVE_FSM,notREADY_s,failedcom_s },
{ SEND_FSM,notREADY_s,failedcom_s },
{ ACK_FSM,waitEVENT_s,waitAck_AckRecieved_s },
{ QUIT_FSM,notREADY_s, failedcom_s },
{ ERROR_FSM,notREADY_s,errorfun_s },
{ IAM_FSM,notREADY_s,failedcom_s },
{ NOEVENT_FSM,notREADY_s,failedcom_s },
{ END,notREADY_s,reset_s },
};



int8_t TransformEvent_s(Evnt ev) {
	switch (ev) {
	case LEFT_EV:
		return 'l';
	case RIGHT_EV:
		return 'r';
	case JUMP_EV:
		return 'j';
	case FLIP_LEFT_EV: case FLIP_RIGHT_EV:
		return 't';
	}

}

void waitEvent_NoEvent_s(void*data) {
	cout << "No Event was Recieved" << endl;
}


void notReady_AnswerIAM_s(void * data) {
	cout << "IAM recieved. Sending ACK" << endl;
	fsmData * pointer = (fsmData *)data;
	Packet packet;
	packet.setPacket(ACKQ_HD, NOTLOADED, 0,NOTLOADED);
	cout << "Composing ACK:" << packet << endl;
	pointer->server->sendMessageTimed(TIMEOUT_TIME_,packet.createACKQ());
	cout << "ACK sent" << endl;
	pointer->leave = true;
	cout << "Leaving FSM" << endl;
}

void notReady_ReadyRecieved_s(void * data)
{
	cout << "IAM recieved. Answering Handshake" << endl;
	fsmData * pointer = (fsmData *)data;
	Packet packet;
	packet.setPacket(IAM_HD, NOTLOADED, NOTLOADED, pointer->wormXMine);
	cout << "Composing ACK:" << packet << endl;
	pointer->server->sendMessageTimed(TIMEOUT_TIME_,packet.createIAM());	//Borre un TIME que no estaba definido y creo que estaba de mas
	cout << "Waiting for ACK" << endl;
}
///OK
void waitEvent_SendEvent_s(void * data)
{// Tengo que mandar el evento que me llego. Lo tengo en el puntero data.
	fsmData * pointer = (fsmData *)data;
	Packet packet;
	cout << "Sending local event to network" << endl;
	packet.setPacket(MOVE_HD, TransformEvent_s((pointer->ev.Event)), pointer->ev.wormID);
	cout << "Composing MOVE Packet:" << packet << endl;
	string msg = packet.createMOVE();
	// Asumo que  el paquete siempre se envia bien y tarda en recibirlo // NO ES BLOQUEANTE
	pointer->server->sendMessageTimed(TIMEOUT_TIME_,msg);
	cout << "Local event sent. Waiting for ACK" << endl;
	pointer->leave = false;
}
///OK
void waitEvent_GetEvent_s(void * data)
{
	fsmData * pointer = (fsmData *)data;
	Packet packet;

	cout << "Event received. Sending ACK" << endl;
	packet.setPacket(ACK_HD, NOTLOADED, pointer->ev.wormID);
	cout << "Composing ACK:" << packet << endl;
	pointer->server->sendMessageTimed(TIMEOUT_TIME_, packet.createACK());
	cout << "ACK sent. Leaving FSM" << endl;

	pointer->leave = true;
	pointer->exitProgram = false;

	// Llega un evento y le tengo que contestar al porgrama con un ID igual al que me mando
}
///OK
void waitEvent_QuitRecieved_s(void * data)
{
	fsmData * pointer = (fsmData *)data;
	Packet packet;
	cout << "Quit Recieved. Answering ACK and leaving the program." << endl;
	pointer->server->sendMessageTimed(TIMEOUT_TIME_, packet.createACKQ());
	pointer->leave = true;
	pointer->exitProgram = true;
}
///OK
void waitAck_AckRecieved_s(void * data)
{
	cout << "ACK recieved and matched the ID. Leaving FSM" << endl;
	fsmData * pointer = (fsmData *)data;
	pointer->leave = true;
}



void errorfun_s(void * data)
{
	cout << "There was an error. Shutting down" << endl;
	fsmData * pointer = (fsmData *)data;
	pointer->leave = true;
	pointer->error = true;
	pointer->exitProgram = true;
}

void failedcom_s(void * data)
{
	cout << "Network events are out of sync. Shutting down" << endl;
	fsmData * pointer = (fsmData *)data;
	pointer->leave = true;
	pointer->error = true;
	pointer->exitProgram = true;
}

void reset_s(void * data)
{
	cout << "Closing" << endl;
	fsmData * pointer = (fsmData *)data;
	pointer->leave = true;
	pointer->exitProgram = true;
}

