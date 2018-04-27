#include "Stage.h"



Stage::Stage()
{

}


Stage::~Stage()
{
	for (int i = 0; i < worms.size(); i++)
		worms.pop_back();
}



void Stage::createWorms(Worm * worm)
{
	this->worms.push_back(Worm(data)); //Implemented with vectors
	this->worms.push_back(Worm(data));
}

