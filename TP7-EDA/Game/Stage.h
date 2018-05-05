#pragma once
#include "Worm.h"
#include "../Observers/Observer.h"
#include "../Events/Ev_t.h"
#include <vector>

using namespace std;

typedef enum ActionType
{
	REFRESH_AT,MOVE_LEFT_AT,MOVE_RIGHT_AT, JUMP_AT,FLIP_RIGHT_AT,FLIP_LEFT_AT 
};

class Stage
{
public:
	Stage(uint32_t myID_, uint32_t otherID_);
	~Stage();
	void createWorms(Worm * worm);
	vector<Worm> * getWorms();
	void destroyWorm(unsigned int wormID);

	void addObserver(Observer * obs);

	void addEvent(Ev_t  ev);
	Ev_t * getEvetn();
	uint32_t getMyWormID();

	void wormMoveLeft(unsigned int wormID);
	void wormMoveRight(unsigned int wormID);
	void wormJump(unsigned int wormID);
	void wormFlipLeft(unsigned int wormID);
	void wormFlipRight(unsigned int wormID);
	void refresh();
	void quit();
	bool isOver();

	
private:
	unsigned int trasnformIDintoPos(unsigned int wormID);

	uint32_t myID;
	uint32_t otherID;
	vector<Worm> worms;
	vector<Observer *> observers;
	ActionType lastAction;
	Ev_t event;
	void update();
	bool leave = false;
};

