#pragma once
#include <allegro5\allegro.h>


#include <array>

// Comment the libraries you are not goiing to use

#define FONT_C
//#define AUDIO_C
#define IMAGE_C
//#define PRIMITIVES_C
#define COLOR_C

#define DISPLAY_C
#define EVENTS_C
#define TIMER_C
#define KEYBOARD_C
//#define MOUSE_C


// Para usar libreria necesito estos dos addons
#ifdef COLOR_C
#ifdef FONT_C
#include "WrittenBox.h"
#endif
#endif

#ifdef IMAGE_C
#include <allegro5\allegro_image.h>
#endif

#ifdef PRIMITIVES_C
#include <allegro5\allegro_primitives.h>
#endif

#ifdef COLOR_C
#include <allegro5\allegro_color.h>
#endif

#ifdef AUDIO_C
#include <allegro5\allegro_acodec.h>
#include <allegro5\allegro_audio.h>
#endif

#ifdef FONT_C
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_ttf.h>
#endif

using namespace std;

enum resources
{
	ALLEGRO, DISPLAY , EVENTQUEUE,PRIMITIVES, TIMER, TTFADDON, FONT, AUDIO, CODEC, IMAGE, LAST
};



class AllegroClass
{
public:
	AllegroClass(float displayW_, float displayH_, float fps_);
	~AllegroClass();
#ifdef DISPLAY_C
	void updateDisplay();
#endif
#ifdef EVENTS_C
	ALLEGRO_EVENT_QUEUE * getEventQueue();
#endif
#ifdef AUDIO_C
	ALLEGRO_SAMPLE * loadSong(char * file);
	void playSong(ALLEGRO_SAMPLE * song, float volume, float songSpeed, ALLEGRO_PLAYMODE playMode);
	void unloadSong(ALLEGRO_SAMPLE * song);
#endif
#ifdef FONT_C
	ALLEGRO_FONT * loadFont(const char * file_, float size);
	void unloadFont(ALLEGRO_FONT * font);
#endif
#ifdef TIMER_C
	ALLEGRO_TIMER * createTimer(float t);
	void destroyTimer(ALLEGRO_TIMER * timer);
#ifdef EVENTS_C
	void setTimerEventSource(ALLEGRO_TIMER * timer);
#endif
#endif
#ifdef IMAGE_C
	ALLEGRO_BITMAP * loadImage(const char * file);
	void unloadImage(ALLEGRO_BITMAP * image);
#endif

private:
	array<bool, LAST> initResources;
#ifdef DISPLAY_C
	ALLEGRO_DISPLAY * display = NULL;
#endif
#ifdef TIMER_C
	ALLEGRO_TIMER * timer = NULL;
#endif
#ifdef EVENTS_C
	ALLEGRO_EVENT_QUEUE * eventQueue = NULL;
#endif

};

