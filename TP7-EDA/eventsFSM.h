#pragma once
#include "FSMData.h"
#include "Network/Packet.h"



//#define TIMEOUT_TIME_ 500

STATE notREADY[];
STATE waitEVENT[];
STATE waitACK[];


void waitEvent_GetEvent(void *data);
void notReady_ReadyRecieved(void *data);
void waitEvent_SendEvent(void *data);
//
void waitEvent_QuitRecieved(void *data);

void waitAck_AckRecieved(void *data);

void notReady_AnswerIAM(void * data);

void waitEvent_NoEvent(void * data);



//manejo del evento error. 
void errorfun(void *data);
//error de comunicacion, dato incorrecto.
void failedcom(void *data);
void reset(void * data);
