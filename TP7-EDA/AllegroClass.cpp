#include "AllegroClass.h"

bool resourcesLoaded(array<bool, LAST>& arr);

AllegroClass::AllegroClass(float displayW_, float displayH_,float fps_)
{

	if (al_init())
	{
		initResources[ALLEGRO] = true;
#ifdef KEYBOARD_C
		al_install_keyboard();
#endif
#ifdef MOUSE_C
		al_install_mouse();
#endif
#ifdef IMAGE_C
		if (al_init_image_addon())
			initResources[IMAGE] = true;
#else
		initResources[IMAGE] = true;
#endif
#ifdef DISPLAY_C
		if (display = al_create_display(displayW_, displayH_))
			initResources[DISPLAY] = true;
#else 
		initResources[DISPLAY] = true;
#endif
#ifdef EVENTS_C
		if (eventQueue = al_create_event_queue())
			initResources[EVENTQUEUE] = true;
#else 
		initResources[EVENTQUEUE] = true;
#endif
#ifdef TIMER_C
		if (timer = al_create_timer(1 / fps_))
			initResources[TIMER] = true;
#else
		initResources[TIMER] = true;
#endif
#ifdef PRIMITIVES_C
		al_init_primitives_addon();
		initResources[PRIMITIVES] = true;
#else
		initResources[PRIMITIVES] = true;
#endif


#ifdef FONT_C
		al_init_font_addon();
		if (al_init_ttf_addon())
		{
			initResources[TTFADDON] = true;
			initResources[FONT] = true;
		}
#else
		initResources[TTFADDON] = true;
		initResources[FONT] = true;
#endif

#ifdef AUDIO_C
		if (al_install_audio())
			initResources[AUDIO] = true;
		if (al_init_acodec_addon())
			initResources[CODEC] = true;
#else
		initResources[AUDIO] = true;
		initResources[CODEC] = true;
#endif
	}

	if (resourcesLoaded(initResources))
	{
#ifdef EVENTS_C
#ifdef DISPLAY_C
		al_register_event_source(eventQueue, al_get_display_event_source(display));
#endif
#ifdef TIMER_C
		al_register_event_source(eventQueue, al_get_timer_event_source(timer));
		al_start_timer(timer);
#endif
#ifdef MOUSE_C
		al_register_event_source(eventQueue, al_get_mouse_event_source());
#endif
#ifdef KEYBOARD_C
		al_register_event_source(eventQueue, al_get_keyboard_event_source());
#endif
#endif

	}
	else
	{
		AllegroClass::~AllegroClass();
	}
}


AllegroClass::~AllegroClass()
{
#ifdef AUDIO_C
	al_stop_samples();
#endif
#ifdef EVENTS_C
	if (initResources[EVENTQUEUE])
		al_destroy_event_queue(eventQueue);
#endif
#ifdef TIMER_C
	if (initResources[TIMER])
		al_destroy_timer(timer);
#endif
#ifdef DISPLAY_C
	if (initResources[DISPLAY])
		al_destroy_display(display);
#endif

}

#ifdef DISPLAY_C
void AllegroClass::updateDisplay()
{
	al_flip_display();
}
ALLEGRO_EVENT_QUEUE * AllegroClass::getEventQueue()
{
	return this->eventQueue;
}
#endif

#ifdef AUDIO_C
ALLEGRO_SAMPLE * AllegroClass::loadSong(char * file)
{
	ALLEGRO_SAMPLE * song = NULL;

	if (al_reserve_samples(1))
		song = al_load_sample(file);

	return song;
}

void AllegroClass::playSong(ALLEGRO_SAMPLE * song, float volume, float songSpeed, ALLEGRO_PLAYMODE playMode)
{
	al_play_sample(song, volume, 0, songSpeed, playMode, NULL);
}

void AllegroClass::unloadSong(ALLEGRO_SAMPLE * song)
{
	al_destroy_sample(song);
}
#endif

#ifdef FONT_C

ALLEGRO_FONT * AllegroClass::loadFont (const char * file_, float size)
{
	return al_load_ttf_font(file_,-size,0);
}
void AllegroClass::unloadFont(ALLEGRO_FONT * font)
{
	al_destroy_font(font);
}
#endif

#ifdef TIMER_C
ALLEGRO_TIMER * AllegroClass::createTimer(float t)
{
	return al_create_timer(t);
}
void AllegroClass::destroyTimer(ALLEGRO_TIMER * timer)
{
	al_destroy_timer(timer);
}
#ifdef EVENTS_C
void AllegroClass::setTimerEventSource(ALLEGRO_TIMER * timer)
{
	al_register_event_source(eventQueue, al_get_timer_event_source(timer));
}

#endif
#endif

#ifdef IMAGE_C
ALLEGRO_BITMAP * AllegroClass::loadImage(const char * file)
{
	return al_load_bitmap(file);
}
void AllegroClass::unloadImage(ALLEGRO_BITMAP * image)
{
	al_destroy_bitmap(image);
}
#endif



bool resourcesLoaded(array<bool, LAST>& arr) {
	bool retValue = true;

	for (bool b : arr)
		if (!b)
			retValue = false;

	return retValue;
}


