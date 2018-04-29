#pragma once
#include <vector>
#include <string>
#include "Observer.h"
#include "../Game/Stage.h"
#include <allegro5\allegro.h>


#define WARMUP 3
#define AIRTIME 36
#define LANDING 43
#define WALKPICS 15
#define WALKFILE "Observers/Images/Walk/wwalk-F"
#define JUMPPICS 10
#define JUMPFILE "Observers/Images/Jump/wjump-F"
#define EXT ".png"
#define BACKGROUNDFILE "Observers/Images/background.png"
#define STAGEFILE "Observers/Images/Scenario.png"

using namespace std;

class DrawStage : public Observer
{
public:
	DrawStage(const char * jumpPath,int jumpSize ,const char * walkPath, int walkSize, const char * background, const char * stageBitmap);
	~DrawStage();
	void update(void * data);
private:
	void drawWorm(float x_, float y_, vector<ALLEGRO_BITMAP*>& sprite, int tick, int flag);

	ALLEGRO_BITMAP * background  = NULL;
	ALLEGRO_BITMAP * stageBitmap = NULL;
	vector<ALLEGRO_BITMAP *> jump;
	int jumpSize;
	int walkSize;
	vector<ALLEGRO_BITMAP *> walk;
	
};

