#include "fsmClient.h"


fsmC::fsmC(STATE * state1_, STATE * state2_, STATE * state3_,void * data_)
{
	this->state1 = state1_;			// Cargo los estados con sus posibles eventos en la clase
	this->state2 = state2_;
	this->state3 = state3_;
	this->data = data_;

	this->actualState = this->state1;  //Elijo al estado 1 como el estado inicial
}

fsmC::~fsmC()
{
}

void fsmC::run()
{
	int i;
	// Busco en el arreglo de los posibles eventos del estado actual cual corresponde al evento que me mandan
	for (i = 0; this->actualState[i].ev != this->actualEvent && this->actualState[i].ev != END; i++);

	this->actualState[i].func(this->data);							// Ejecuto la funcion del evento
	this->actualState = actualState[i].nextState;					// Cambio el estado

}
int fsmC::setEvent(int ev_)
{
	this->actualEvent = ev_;

	if (isalpha(ev_))
		this->actualEvent = tolower(ev_);

	return this->actualEvent;
}

void *fsmC::getData()
{
	return data;
}

