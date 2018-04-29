#pragma once
#include "../Controllers\Controller.h"
#include "Ev_t.h"
#include "../Game/Stage.h"
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
	void HandleEventDispatch(Stage& stage);
	void removeEvent(list<Ev_t>::iterator it);
	list<Ev_t> events;
private:
	void displatchEvent(Ev_t& ev,Stage& stage);
	vector<Controller *> controllers;
	
};

