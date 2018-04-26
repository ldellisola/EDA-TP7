#pragma once
class Observer
{
public:
	Observer();
	~Observer();

	virtual void update(void * data) = 0;
};

