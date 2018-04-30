#pragma once
#include "eventsClient.h"

#include <ctype.h>



class fsmC
{
public:
	fsmC(STATE * state1_, STATE * state2_, STATE * state3_, void * data_);
	~fsmC();
	void run();
	int setEvent(int ev_);
	void *getData();
private:
	int actualEvent;
	STATE * actualState;
	STATE * state1;
	STATE * state2;
	STATE * state3;

	void * data;
};
