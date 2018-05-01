#pragma once
#include "../FSMData.h"
#include "../Network/Packet.h"

//escribimos todos los eventos explicitamente para claridad, por mas que el enum ya cubre el orden.




STATE notREADY_c[];
STATE waitEVENT_c[];
STATE waitACK_c[];


// Estado actual _  Evento recibido
void notReady_AnswerIAM_c(void * data);

void notReady_ReadyRecieved_c(void *data);

void waitEvent_SendEvent_c(void *data);

void waitEvent_GetEvent_c(void *data);



void waitEvent_QuitRecieved_c(void *data);

void waitAck_AckRecieved_c(void *data);



void errorfun_c(void *data);
void failedcom_c(void *data);
void reset_c(void * data);
