#include "drawStage.h"



DrawStage::DrawStage(const char * jumpPath, int jumpSize, const char * walkPath, int walkSize, const char * background, const char * stageBitmap)
{
	this->jumpSize = jumpSize;

	for (int i = 0; i < (LANDING); i++)
		if (i < WARMUP) {

			jump.push_back(al_load_bitmap((jumpPath + to_string(i + 1) + EXT).c_str()));
		}
		else if (i < AIRTIME)
			jump.push_back(jump[2]);
		else if (i < LANDING) {
			jump.push_back(al_load_bitmap((jumpPath + to_string(i + 1 - 33) + EXT).c_str()));
		}


	this->walkSize = walkSize;
	for (int i = 0; i < walkSize; i++)
		walk.push_back(al_load_bitmap((walkPath + to_string(i + 1) + EXT).c_str()));
	
	for (int i = 0; i < 3; i++) 
		for (int a = 0; a < walkSize; a++) 
			walk.push_back(walk[(a > 11 ? a - 1 : a)]);


	this->background = al_load_bitmap(background);
	this->stageBitmap = al_load_bitmap( stageBitmap);
}


DrawStage::~DrawStage()
{
	for (int i = 0; i < jump.size(); i++) {
		if (i < WARMUP) {
			if (jump[i])
				al_destroy_bitmap(jump[i]);
		}
		else if (i < AIRTIME);
		else if (i < LANDING)
			if (jump[i])
				al_destroy_bitmap(jump[i]);

	}
		

	for (int i = 0; i < walkSize; i++)
		if (walk[i])
			al_destroy_bitmap(walk[i]);

	if (background)
		al_destroy_bitmap(background);
}

void DrawStage::update(void * data)
{
	Stage * stage = (Stage *)data;
	al_draw_bitmap(background,0,0,0);
	al_draw_bitmap(stageBitmap, 0, 0, 0);


	vector<Worm> * worms = stage->getWorms();


	for (Worm& worm : worms[0]) 
		drawWorm(worm.getX(), worm.getY(), (worm.getState() == MOVE_ST ? walk:jump),worm.getTicks(), (worm.getDirection() == RIGHT_DR ? ALLEGRO_FLIP_HORIZONTAL : 0));

	al_flip_display();
	

}

void DrawStage::drawWorm(float x_, float y_, vector<ALLEGRO_BITMAP*>& sprite, int tick,int flag)
{
	al_draw_bitmap(sprite[(tick >=1 ? tick -1 : 1)], x_, y_, flag);
}
