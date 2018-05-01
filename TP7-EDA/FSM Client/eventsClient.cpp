#include "eventsClient.h"


//intial state.


STATE notREADY_c[] = {
{ ANS_IAM_FSM , waitEVENT_c,notReady_AnswerIAM_c },
{ MOVE_FSM,notREADY_c,failedcom_c },
{ SEND_FSM,notREADY_c,failedcom_c },
{ ACK_FSM,notREADY_c,failedcom_c },
{ QUIT_FSM,notREADY_c,failedcom_c },
{ ERROR_FSM,notREADY_c,errorfun_c },
{ IAM_FSM,waitACK_c,notReady_ReadyRecieved_c },
{ END,notREADY_c,reset_c },
};
// Tendria que ahcer que cuando llega el ACK o cuando Mando un ACK, que salga del loop. Lo mismo si hay un error
STATE waitEVENT_c[] = {
{ ANS_IAM_FSM , notREADY_c,failedcom_c },
{ SEND_FSM,waitACK_c,waitEvent_SendEvent_c },
{ MOVE_FSM,waitEVENT_c,waitEvent_GetEvent_c},
{ ACK_FSM,notREADY_c,failedcom_c },
{ QUIT_FSM,notREADY_c,waitEvent_QuitRecieved_c },
{ ERROR_FSM,notREADY_c,errorfun_c },
{ IAM_FSM, notREADY_c,failedcom_c },
{ END,notREADY_c,reset_c },
};

STATE waitACK_c[] = {
{ ANS_IAM_FSM, notREADY_c,failedcom_c },
{ MOVE_FSM,notREADY_c,failedcom_c },
{ SEND_FSM,notREADY_c,failedcom_c },
{ ACK_FSM,waitEVENT_c,waitAck_AckRecieved_c },
{ QUIT_FSM,notREADY_c, failedcom_c },
{ ERROR_FSM,notREADY_c,errorfun_c },
{IAM_FSM,notREADY_c,failedcom_c },
{ END,notREADY_c,reset_c },
};




int8_t TransformEvent_c(Evnt ev) {
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

void notReady_AnswerIAM_c(void * data) {
	cout << "IAM recieved. Sending ACK" << endl;
	fsmData * pointer = (fsmData *)data;
	Packet packet;
	packet.setPacket(ACKQ_HD, NOTLOADED, 0, NOTLOADED);
	pointer->client->sendMessage(packet.createACKQ());
	cout << "ACK sent" << endl;
}

///OK
void notReady_ReadyRecieved_c(void * data)
{
	cout << "IAM recieved. Answering Handshake" << endl;
	fsmData * pointer = (fsmData *)data;
	Packet packet;
	packet.setPacket(IAM_HD, NOTLOADED, NOTLOADED, pointer->wormXMine);
	pointer->client->sendMessage(packet.createIAM());
	cout << "Waiting for ACK" << endl;
}
///OK
void waitEvent_SendEvent_c(void * data)
{// Tengo que mandar el evento que me llego. Lo tengo en el puntero data.
	fsmData * pointer = (fsmData *)data;
	Packet packet;
	cout << "Sending local event to network. Waiting for ACK" << endl;
	packet.setPacket(MOVE_FSM, TransformEvent_c((pointer->ev.Event)), pointer->ev.wormID);
	string msg = packet.createMOVE();
	// Asumo que  el paquete siempre se envia bien y tarda en recibirlo // NO ES BLOQUEANTE
	pointer->client->sendMessage(msg);
	cout << "Local event sent" << endl;
}
///OK
void waitEvent_GetEvent_c(void * data)
{
	fsmData * pointer = (fsmData *)data;
	Packet packet;
	cout << "Event received. Sending ACK" << endl;
	packet.setPacket(ACK_HD, NOTLOADED, pointer->ev.wormID);
	pointer->client->sendMessage(packet.createACK());
	cout << "ACK sent. Leaving FSM" << endl;
	pointer->leave = true;
	pointer->exitProgram = false;

	// Llega un evento y le tengo que contestar al porgrama con un ID igual al que me mando
}
///OK
void waitEvent_QuitRecieved_c(void * data)
{
	fsmData * pointer = (fsmData *)data;
	Packet packet;
	cout << "Quit Recieved. Answering ACK and leaving the program." << endl;
	pointer->client->sendMessage(packet.createACKQ());
	pointer->leave = true;
	pointer->exitProgram = true;
}
///OK
void waitAck_AckRecieved_c(void * data)
{
	cout << "ACK recieved and matched the ID. Leaving FSM" << endl;
	fsmData * pointer = (fsmData *)data;
	pointer->leave = true;
}



void errorfun_c(void * data)
{
	cout << "There was an error. Shutting down" << endl;
	fsmData * pointer = (fsmData *)data;
	pointer->leave = true;
	pointer->error = true;
	pointer->exitProgram = true;
}

void failedcom_c(void * data)
{
	cout << "Network events are out of sync. Shutting down" << endl;
	fsmData * pointer = (fsmData *)data;
	pointer->leave = true;
	pointer->error = true;
	pointer->exitProgram = true;
}

void reset_c(void * data)
{
	cout << "Closing" << endl;
	fsmData * pointer = (fsmData *)data;
	pointer->leave = true;
	pointer->exitProgram = true;
}
