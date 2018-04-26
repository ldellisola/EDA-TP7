
#include "AllegroClass.h"
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
	// Falta network controller
	DrawStage drawStage(JUMPFILE, JUMPSIZE, WALKFILE, WALKSIZE, BACKGROUNDFILE);

	while (getEvent()) {
		if (isThereEvent()) {
			dispatchEvent();
		}
	}
	
}