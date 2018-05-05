#include "Worm.h"
#include <math.h>



using namespace std;
Worm::Worm(WormData * data, uint32_t wormID_, float xoffset_, Direction dir)
{
	this->data = data;
	this->coord.x = this->data->minX + xoffset_ ; 
	this->coord.y = this->data->minY; 
	this->direction = dir;
	this->state = STILL_ST;
	this->prev_state = STILL_ST;
	this->ticks = 0;
	wormID = wormID_;
}


Worm::~Worm()
{
}

void Worm::move(Direction a)
{
	if (this->prev_state != JUMP_ST)
	{
	this->direction = a;
	this->prev_state = state;
	this->state = MOVE_ST;
	}
}

void Worm::jump()
{
	if (this->prev_state != MOVE_ST )
	{
		this->prev_state = state;
		this->state = JUMP_ST;
	}
}

void Worm::flipLeft()
{
	if (state == STILL_ST)
		this->direction = (this->direction == LEFT_DR ? LEFT_DR : LEFT_DR) ;
}

void Worm::flipRight() {
	if (state == STILL_ST)
		this->direction = (this->direction == RIGHT_DR ? RIGHT_DR : RIGHT_DR);
}

State Worm::getState()
{
	return state;
}

Direction Worm::getDirection()
{
	return direction;
}

float Worm::getX()
{
	return coord.x;
}

float Worm::getY()
{
	return coord.y;
}

int Worm::getTicks()
{
	return ticks;
}

void Worm::doMove()
{
	this->ticks++;
	if (!(abs(this->ticks - 3) % this->data->RepeatedFramesPerCycle))
		if (this->isXValid())
			this->coord.x += this->direction * this->data->pixelsPerCycle;
}

void Worm::doJump()
{
	this->ticks++;
	if (this->ticks >= 0 && this->ticks < JUMPTICKS)
	{
		if (isXValid())
		{	
			this->coord.x += this->direction * cos(this->data->degrees60)*this->data->speed;
		}
		if ((this->coord.y <= this->data->minY))
		{
			this->coord.y =( data->minY - this->data->speed * ticks *sin(this->data->degrees60) + this->data->gravity * ticks*ticks /2.0);
		}
		if (this->ticks == JUMPTICKS - 1)
			this->coord.y = this->data->minY;
	
	}

}

bool Worm::isXValid()
{
	bool retValue = false;

	if ((this->direction == LEFT_DR) && ((this->coord.x + 9) > data->minX))
		retValue = true;
	else if ((this->direction == RIGHT_DR) && (this->coord.x + 49< data->maxX))
		retValue = true;

	return retValue;
}

void Worm::update()
{

	switch (this->state)
	{
		
	case MOVE_ST: 
		if (this->prev_state != JUMP_ST)
		{
			if (this->ticks < WALKTICKS)
			{
				this->doMove();
				this->prev_state = MOVE_ST;
			}
			else
			{
				this->state = STILL_ST;
				this->prev_state = STILL_ST;
				this->ticks = 0;
			}

		}
		
		break;
	case JUMP_ST:
		if (this->prev_state != MOVE_ST)
		{
			if (this->ticks < JUMPTICKS)
			{
				this->doJump();
				this->prev_state = JUMP_ST;
			}
			else
			{
				this->state = STILL_ST;
				this->prev_state = STILL_ST;
				this->ticks = 0;
			}
		}
		break;
	case STILL_ST:
		this->ticks = 0;
		break;
	}
}
