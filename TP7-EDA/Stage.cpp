#include "Stage.h"



Stage::Stage()
{
	this->backgroundBitmap = NULL;
	this->stageBitmap = NULL;
}


Stage::~Stage()
{
	if (this->backgroundBitmap)
		al_destroy_bitmap(this->backgroundBitmap);
	if (this->stageBitmap)
		al_destroy_bitmap(this->stageBitmap);

	for (int i = 0; i < worms.size(); i++)
		worms.pop_back();
}

void Stage::draw()
{
	al_draw_bitmap(this->backgroundBitmap, 0, 0, 0);
	al_draw_bitmap(this->stageBitmap, 0, 0, 0);
}

void Stage::createWorms(info * data)
{
	this->worms.push_back(Worm(data)); //Implemented with vectors
	this->worms.push_back(Worm(data));
}

void Stage::loadImages(const char * stage_, const char * backg_)
{
	this->backgroundBitmap = al_load_bitmap(backg_);
	this->stageBitmap = al_load_bitmap(stage_);
}
