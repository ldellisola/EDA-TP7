#pragma once
#include <string>
#include <allegro5\allegro.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_color.h>
using namespace std;


typedef struct textData
{
	float x, y, width, height;
	int fontSize;
	string text;
	const char * path;
	const char * color;
};


class WrittenBox
{
public:
	WrittenBox(float x_, float y_, float width_, float height_, int fontSize_, string text_, const char * fontPath, const char * fontColor);
	~WrittenBox();
	void draw();
protected:
	string text;
	ALLEGRO_BITMAP * bitmap;
	ALLEGRO_COLOR fontColor;
	float x;
	float y;
	float width;
	float height;
	ALLEGRO_FONT * font;
	
};

