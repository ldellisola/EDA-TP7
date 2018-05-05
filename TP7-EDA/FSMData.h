#pragma once
#include "Events\Ev_t.h"
#include "Network\Server.h"
#include "Network\Client.h"

enum event_types { MOVE_FSM, ACK_FSM, QUIT_FSM, IAM_FSM,ANS_IAM_FSM, SEND_FSM, ERROR_FSM , NOEVENT_FSM};

#define MAXTIMEOUT 5

#define END 99

typedef struct state_t STATE;

struct state_t {
	char ev;
	STATE * nextState;
	void(*func)(void * data);
};


typedef struct fsmData {
	Ev_t ev;
	unsigned int timeouts = 0;
	bool leave = false;
	bool error = false;
	uint16_t wormXMine;
	uint16_t wormXOther;
	Client *client;
	Server * server;
	bool exitProgram = false;

	string oldPacket;

};