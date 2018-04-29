#pragma once

#define PI (3.1415926535897932384626433832795028841971693993751058209749445923078164062862089986280348253421170679)


#define WALKTICKS 45 
#define JUMPTICKS 43


typedef struct
{
	const float maxX = 1212.0;
	const float minX = 701.0;
	const float minY = 616.0;
	const float maxY = (616 - 33.0);

	const float gravity = 0.24;
	const float degrees60 = PI / 3.0;
	const float speed = 4.5;
	const float pixelsPerCycle = 9;
	const int RepeatedFramesPerCycle = 14;

}WormData;


typedef struct {		
	float x, y; 
}Point;
 
typedef enum Direction{
	LEFT_DR = -1, NONE = 0, RIGHT_DR = 1
};

typedef enum State{
	STILL_ST, MOVE_ST, JUMP_ST
};

class Worm
{
public:
	Worm(WormData * data, float xoffset_ = 200, Direction dir = RIGHT_DR);
	~Worm();
	// Esta funcion setea el estado de 'MOVE' y la direccion hacia la cual se mueve
	void move(Direction a); 
	// Esta funcion setea el estado 'Jump'
	void jump();
	// Esta funcion forza al worm a mirar a la izquierda
	void flipLeft();
	// Esta funcion forza al worm a mirar a la derecha
	void flipRight();
	
	State getState();
	Direction getDirection();
	float getX();
	float getY();
	int getTicks();

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
	State state;
	int ticks;
	WormData * data;

	
};

