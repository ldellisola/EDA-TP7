#pragma once
#include "../Controllers\Controller.h"
#include "Ev_t.h"
#include <vector>
#include <list>

using namespace std;

class EventHandler
{
public:
	EventHandler();
	~EventHandler();
	void loadController(Controller * cont);
	void getEvent();
	bool areThereActiveEvents();
	Ev_t * returnEvent(int * size);
	void HandleEventDispatch();
private:
	void displatchEvent(Ev_t& ev);
	vector<Controller *> controllers;
	list<Ev_t> events;
};

