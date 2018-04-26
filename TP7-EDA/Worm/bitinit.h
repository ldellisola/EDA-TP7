#pragma once
#include <allegro5\allegro.h>
#include<allegro5\allegro_image.h>
#include <array>

using namespace std;


#define WARMUP 3
#define AIRTIME 36
#define LANDING 43
#define WALKTICKS 45 
#define JUMPTICKS 43
#define WALKPICS 15
#define JUMPPICS 10

// Elegimos cargar los bitmaps de la forma en la que se iran mostrando para evitar complicar la funcion draw().
//
// Creamos estas funciones de forma que no se cargan mas imagenes de las necesarias, solo se cargan las imagenes y se copian los
// punteros a otras posiciones. Al terminar se debe llamar a unload() para eliminar correctamente los bitmaps y evitar memory leaks
//
typedef struct
{

	const float maxX = 1212.0;
	const float minX = 701.0;
	const float minY = 616.0;
	const float maxY = (616 - 33.0);
	array<ALLEGRO_BITMAP *, WALKTICKS> walk;	// Decidimos usar array ya que no hay que pasar el tama;o del arreglo como un parametro extra
	array<ALLEGRO_BITMAP *, JUMPTICKS> jump;	// y por que te avisa cuando te estas pasando de la cantidad maxima de elementos.

	void load( array< char [14], WALKPICS>& walkAnimation, array< char[14], 10>& jumpAnimation)  //Loading the animations
	{
		int sizeWalk = walkAnimation.size();
		//Moving 
		for (int i = 0; i < sizeWalk; i++)
		{
			this->walk[i] = al_load_bitmap(walkAnimation[i]); //Loading first walk
		}
		for (int i = 1; i < 3; i++)
		{								
			for (int a = 0; a < (sizeWalk ); a++)
			{
				walk[sizeWalk*i + a ] = walk[(a > 11 ? a - 1 : a)]; //Repeating for full movement
			}
		}
		//Jumping
		for (int i = 0; i < (LANDING); i++)
		{
			if (i < WARMUP)
			{
				jump[i] = al_load_bitmap(jumpAnimation[i]);
			}
			else if (i < AIRTIME)
			{
				jump[i] = jump[2];
			}
			else if (i < LANDING)
			{
				jump[i] = al_load_bitmap(jumpAnimation[i - 33]);
			}
		}
	}
	void unload(int sizeWalk, int sizeJump)
	{
		for (int i = 0; i < sizeJump; i++)
		{
			al_destroy_bitmap(walk[i]);
		}
		for (int i = 0; i < WARMUP; i++)
		{
			al_destroy_bitmap(jump[i]);
		}
		for (int i = AIRTIME; i < LANDING; i++)
		{
			al_destroy_bitmap(jump[i]);
		}
	}
}info;