#include "Stage.h"




unsigned int Stage::trasnformIDintoPos(unsigned int wormID) {
	if (wormID == myID)
		return 0;
	if (wormID == otherID)
		return 1;
}

Stage::Stage(uint32_t myID_, uint32_t otherID_)
{
	myID = myID_;
	otherID = otherID_;
}


Stage::~Stage()
{
	for (int i = 0; i < worms.size(); i++)
		worms.pop_back();
}



void Stage::createWorms(Worm * worm)
{
	this->worms.push_back(*worm);
}

vector<Worm>* Stage::getWorms()
{
	return &worms;
}

void Stage::destroyWorm(unsigned int wormID)
{
	worms.erase(worms.begin() + trasnformIDintoPos(wormID));
}

void Stage::addObserver(Observer * obs)
{
	observers.push_back(obs);
}

void Stage::addEvent(Ev_t  ev)
{
	this->event = ev;
}

Ev_t * Stage::getEvetn()
{
	return &this->event;
}

uint32_t Stage::getMyWormID()
{
	return this->myID;
}

void Stage::wormMoveLeft(uint32_t wormID)
{
	worms[trasnformIDintoPos(wormID)].move(LEFT_DR);
	lastAction = MOVE_LEFT_AT;
}

void Stage::wormMoveRight(uint32_t wormID)
{
	worms[trasnformIDintoPos(wormID)].move(RIGHT_DR);
	lastAction = MOVE_RIGHT_AT;
}

void Stage::wormJump(uint32_t wormID)
{
	worms[trasnformIDintoPos(wormID)].jump();
	lastAction = JUMP_AT;
}

void Stage::wormFlipLeft(uint32_t wormID)
{
	worms[trasnformIDintoPos(wormID)].flipLeft();
	lastAction = FLIP_LEFT_AT;
}

void Stage::wormFlipRight(uint32_t wormID)
{
	worms[trasnformIDintoPos(wormID)].flipRight();
	lastAction = FLIP_RIGHT_AT;
}

void Stage::wormFlip(uint32_t wormID)
{
	worms[trasnformIDintoPos(wormID)].flip();

}

void Stage::refresh()
{
	for (Worm& worm : worms) 
		worm.update();

	lastAction = REFRESH_AT;
	update();

	this->event.deactivate();
	
}

void Stage::quit()
{
	leave = true;
}

bool Stage::isOver()
{
	return leave;
}

void Stage::update()
{
	for (Observer * obs : observers)
		obs->update(this);
}

