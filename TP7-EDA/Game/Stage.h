#pragma once
#include "Worm.h"
#include "../Observers/Observer.h"
#include "../Events/Ev_t.h"
#include <vector>

using namespace std;

typedef enum ActionType
{
	REFRESH_AT,MOVE_LEFT_AT,MOVE_RIGHT_AT, JUMP_AT,FLIP_RIGHT_AT,FLIP_LEFT_AT 
};

class Stage
{
public:
	// Le paso al constructor los ID de los worms, en el orden que los elegi
	Stage(uint32_t myID_, uint32_t otherID_);
	~Stage();

	// Agrego un worm existente al stage
	void createWorms(Worm * worm);
	// Devuelvo el vector de worms
	vector<Worm> * getWorms();
	// Destruye a un worm en particular, dado su ID
	void destroyWorm(unsigned int wormID);

	// Agrega un observer al Stage
	void addObserver(Observer * obs);

	// Agrega un evento que luego se va a pasar al observer de networking
	void addEvent(Ev_t  ev);
	// Devuelve el evento guardado
	Ev_t * getEvetn();

	// Devuelve el ID de mi worm
	uint32_t getMyWormID();

	// Mueven a un worm dado su ID
	void wormMoveLeft(unsigned int wormID);
	void wormMoveRight(unsigned int wormID);
	void wormJump(unsigned int wormID);
	void wormFlipLeft(unsigned int wormID);
	void wormFlipRight(unsigned int wormID);

	// actualiza el 
	void refresh();
	void quit();
	bool isOver();

	
private:
	unsigned int trasnformIDintoPos(unsigned int wormID);

	uint32_t myID;
	uint32_t otherID;
	vector<Worm> worms;
	vector<Observer *> observers;
	ActionType lastAction;
	Ev_t event;
	void update();
	bool leave = false;
};

