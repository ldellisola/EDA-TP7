#include "drawStage.h"



DrawStage::DrawStage(const char * jumpPath, int jumpSize, const char * walkPath, int walkSize, const char * background)
{
	this->jumpSize = jumpSize;
	for (int i = 0; i < jumpSize; i++)
		jump.push_back(al_load_bitmap((jumpPath + to_string(i + 1)).c_str()));

	// Hay que organizar bien esto asi hacemos como con Guido

	this->walkSize = walkSize;
	for (int i = 0; i < walkSize; i++)
		jump.push_back(al_load_bitmap((walkPath + to_string(i + 1)).c_str()));
	// Hay que organizar bien esto asi hacemos como con Guido

	this->background = al_load_bitmap(background);
}


DrawStage::~DrawStage()
{
	for (int i = 0; i < jumpSize; i++)
		if (jump[i])
			al_destroy_bitmap(jump[i]);

	for (int i = 0; i < walkSize; i++)
		if (walk[i])
			al_destroy_bitmap(walk[i]);

	if (background)
		al_destroy_bitmap(background);s
}

void DrawStage::update(void * data)
{
	Stage * stage = (Stage *)data;
	al_draw_bitmap(background);

	vector<Worm> * worms = stage->getWorms();


	for (Worm worm : worms) 
		drawWorm(worm.getX(), worm.getY(), (worm.getState() == WALK ? walk:jump));

	al_flip_display();
	

}

void DrawStage::drawWorm(float x_, float y_, vector<ALLEGRO_BITMAP*>& sprite, int tick)
{
	al_draw_bitmap(sprite[tick], x_, y_, 0);
}
