#pragma once
#include "../FSMData.h"
#include "../Network/Packet.h"





STATE notREADY_s[];
STATE waitEVENT_s[];
STATE waitACK_s[];


void waitEvent_GetEvent_s(void *data);
void notReady_ReadyRecieved_s(void *data);
void waitEvent_SendEvent_s(void *data);
//
void waitEvent_QuitRecieved_s(void *data);

void waitAck_AckRecieved_s(void *data);

void notReady_AnswerIAM_s(void * data);



//manejo del evento error. 
void errorfun_s(void *data);
//error de comunicacion, dato incorrecto.
void failedcom_s(void *data);
void reset_s(void * data);
