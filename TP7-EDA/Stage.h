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
	void draw();
	void createWorms(info * data);
	void loadImages(const char * stage_, const char * backg_);
	vector<Worm> worms;
private:
	ALLEGRO_BITMAP * stageBitmap;
	ALLEGRO_BITMAP * backgroundBitmap;
};

