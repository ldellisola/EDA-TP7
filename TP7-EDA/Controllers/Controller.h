#pragma once

class Controller
{
public:
	Controller();
	virtual void * getEvent(void * data) = 0;
	~Controller();
};

