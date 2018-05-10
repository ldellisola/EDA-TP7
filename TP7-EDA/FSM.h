#pragma once
#include "FSMData.h"
#include "eventsFSM.h"
#include <ctype.h>



class FSM
{
public:
	FSM(STATE * state1_, STATE * state2_, STATE * state3_, void* data_);
	~FSM();
	void run();
	int setEvent(int ev_);
	void * getData();
	int actualEvent;

private:
	STATE * actualState;

	STATE * state1;
	STATE * state2;
	STATE * state3;

	void * data;
};

