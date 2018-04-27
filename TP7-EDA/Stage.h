#pragma once
#include <allegro5\allegro.h>
#include "Worm.h"
#include "bitinit.h"
#include <vector>

using namespace std;

class Stage
{
public:
	Stage();
	~Stage();
	void createWorms(Worm * worm);
	void wormMoveLeft(unsigned int wormID);
	void wormMoveRight(unsigned int wormID);
	void wormJump(unsigned int wormID);
	void wormFlipLeft(unsigned int wormID);
	void wormFlipRight(unsigned int wormID);
	void worm
	
private:
	vector<Worm> worms;
};

