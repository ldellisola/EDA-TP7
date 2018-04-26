#pragma once
#include "bitinit.h"



typedef struct {		
	float x, y; 
}Point;
 
typedef enum Direction{
	LEFT = -1, NONE = 0, RIGHT = 1
};

typedef enum State{
	STILL, MOVE, JUMP
};

class Worm
{
public:
	Worm(info * data);
	~Worm();
	// Esta funcion setea el estado de 'MOVE' y la direccion hacia la cual se mueve
	void move(Direction a); 
	// Esta funcion setea el estado 'Jump'
	void jump();
	// Esta funcion forza al worm a mirar a la izquierda
	void flipLeft();
	// Esta funcion forza al worm a mirar a la derecha
	void flipRight();
	// Es el estado actual del worm
	State state;
	// Dibuja al worm
	void draw();
	// Actualiza al worm. Esta funcion va a llamar a doMove y doJump que son las funciones que se encargan
	// de mover al worm.
	void update();
private:
	void doMove();		//This functions should not be accesible from outside the object as they are only called from update.
	void doJump();		//To use them ypu should set the state using 'move' and 'jump'
	bool isXValid();	

	Direction direction;
	Point coord;
	State prev_state;
	
	int ticks;
	info * data;

	const float gravity = 0.24;
	const float degrees60 = ALLEGRO_PI / 3.0;
	const float speed = 4.5;
	const float pixelsPerCycle = 9;
	const int RepeatedFramesPerCycle = 14;
};

