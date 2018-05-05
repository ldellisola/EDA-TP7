#pragma once
#include "../FSMData.h"
#include "eventsServer.h"
#include <ctype.h>



class fsmS
{
public:
	fsmS(STATE * state1_, STATE * state2_, STATE * state3_,  void* data_);
	~fsmS();
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

