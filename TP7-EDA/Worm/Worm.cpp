#include "Worm.h"
#include <math.h>



using namespace std;
Worm::Worm(info * data_)
{
	this->data = data_;
	this->coord.x = this->data->minX + (rand() % (int)(this->data->maxX - this->data->minX - al_get_bitmap_width(this->data->walk[0]))) ; 
	this->coord.y = this->data->minY; 
	this->direction = RIGHT;
	this->state = STILL;
	this->prev_state = STILL;
	this->ticks = 0;
}


Worm::~Worm()
{
}

void Worm::move(Direction a)
{
	if (this->prev_state != JUMP)
	{
	this->direction = a;
	this->prev_state = state;
	this->state = MOVE;
	}
}

void Worm::jump()
{
	if (this->prev_state != MOVE )
	{
		this->prev_state = state;
		this->state = JUMP;
	}
}

void Worm::flipLeft()
{
	this->direction = (this->direction == LEFT ? LEFT : LEFT) ;
}

void Worm::flipRight() {
	this->direction = (this->direction == RIGHT ? RIGHT : RIGHT);
}

void Worm::doMove()
{
	this->ticks++;
	if (!(abs(this->ticks - 3) % this->RepeatedFramesPerCycle))
		if (this->isXValid())
			this->coord.x += this->direction * this->pixelsPerCycle;
}

void Worm::doJump()
{
	this->ticks++;
	if (this->ticks >= 0 && this->ticks < JUMPTICKS)
	{
		if (isXValid())
		{	
			this->coord.x += this->direction * cos(this->degrees60)*this->speed;
		}
		if ((this->coord.y <= this->data->minY))
		{
			this->coord.y =( data->minY - this->speed * ticks *sin(this->degrees60) + this->gravity * ticks*ticks /2.0);
		}
		if (this->ticks == JUMPTICKS - 1)
			this->coord.y = this->data->minY;
	
	}

}

bool Worm::isXValid()
{
	bool retValue = false;

	if ((this->direction == LEFT) && ((this->coord.x + 9) > data->minX))
		retValue = true;
	else if ((this->direction == RIGHT) && (this->coord.x + 49< data->maxX))
		retValue = true;

	return retValue;
}

void Worm::draw()
{
	int flag = (this->direction == RIGHT ? ALLEGRO_FLIP_HORIZONTAL : 0);
	switch (this->state)
	{
	case MOVE:
			al_draw_bitmap(this->data->walk[this->ticks -1], this->coord.x, this->coord.y,flag );
		break;
	case JUMP:
			al_draw_bitmap(this->data->jump[this->ticks - 1], this->coord.x, this->coord.y, flag);
		break;
	case STILL:
		al_draw_bitmap(this->data->walk[3], (this->coord).x, (this->coord).y, flag);
		break;
	}


}

void Worm::update()
{

	switch (this->state)
	{
		
	case MOVE: 
		if (this->prev_state != JUMP)
		{
			if (this->ticks < WALKTICKS)
			{
				this->doMove();
				this->prev_state = MOVE;
			}
			else
			{
				this->state = STILL;
				this->prev_state = STILL;
				this->ticks = 0;
			}

		}
		
		break;
	case JUMP:
		if (this->prev_state != MOVE)
		{
			if (this->ticks < JUMPTICKS)
			{
				this->doJump();
				this->prev_state = JUMP;
			}
			else
			{
				this->state = STILL;
				this->prev_state = STILL;
				this->ticks = 0;
			}
		}
		break;
	case STILL:
		this->ticks = 0;
		break;
	}
}
