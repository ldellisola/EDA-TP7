
#include "AllegroClass.h"
#include "Events\EventHandler.h"
#include "Controllers\AllegroEventGetter.h"
#include "Observers\drawStage.h"


#define WALKFILE ""
#define JUMPFILE ""
#define WALKSIZE (2)
#define JUMPSIZE (2)
#define BACKGROUNDFILE ""
int main(int argc ,char * argv[]) {

	AllegroClass allegro(2000, 1000, 50);
	AllegroEventGetter allegroEvents(allegro.getEventQueue());
	EventHandler eventHandler;
	// Falta network controller
	DrawStage drawStage(JUMPFILE, JUMPSIZE, WALKFILE, WALKSIZE, BACKGROUNDFILE);

	eventHandler.loadController(&allegroEvents);
	// Hay que cargar el controller de network

	do {
		
		eventHandler.getEvent();
		if (eventHandler.areThereActiveEvents()){
			int size = 0;
			Ev_t * events = eventHandler.returnEvent(&size);
		}
	}
	
}