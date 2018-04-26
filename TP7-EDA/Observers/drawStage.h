#pragma once
#include <vector>
#include <string>
#include "Observer.h"
#include <allegro5\allegro.h>

using namespace std;

class DrawStage : public Observer
{
public:
	DrawStage(const char * jumpPath,int jumpSize ,const char * walkPath, int walkSize, const char * background);
	~DrawStage();
	void update(void * data);
private:
	void drawWorm(float x_, float y_, vector<ALLEGRO_BITMAP*>& sprite, int tick);

	ALLEGRO_BITMAP * background;
	vector<ALLEGRO_BITMAP *> jump;
	int jumpSize;
	int walkSize;
	vector<ALLEGRO_BITMAP *> walk;
	
};

