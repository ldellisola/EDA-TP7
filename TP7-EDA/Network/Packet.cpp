#include "Packet.h"
#include "Network\Packet.h"



Packet::Packet()
{
}


Packet::~Packet()
{
}

void Packet::clear()
{
	wormID = NOTLOADED;
	wormX = NOTLOADED;
	header = NOTLOADED;
	action = NOTLOADED;

}

void Packet::setPacket(int8_t type, int8_t action_ , int32_t wormID_ , int16_t wormX_ ){
	clear();

	switch (type) {
	case ACKQ_HD:
		header = ACKQ_HD;
		wormID = 0;
		break;
	case ACK_HD:
		header = ACK_HD;
		wormID = wormID_;
		break;
	case IAM_HD:
		header = IAM_HD;
		wormX = wormX_;
		break;
	case MOVE_HD:
		header = MOVE_HD;
		action = action_;
		wormID = wormID_;
		break;
	case QUIT_HD:
		header = QUIT_HD;
		break;
	case ERROR_HD:
		header = ERROR_HD;
		break;
	}
}

void Packet::setPacket(string packet)
{
	header = packet[0];
	packet.erase(packet.begin());
	switch (header) {
	case ACK_HD:
		getBEStringToNum(packet, &wormID);
		break;
	case IAM_HD:
		getBEStringToNum(packet, &wormX);
		break;
	case MOVE_HD:
		action = packet[0];
		packet.erase(packet.begin());
		getBEStringToNum(packet, &wormID);
		break;
	}	// No hago de los demas headers por que no tienen mas info.
}

bool Packet::isPacketClear()
{
	return wormID != NOTLOADED && wormX != NOTLOADED && header != NOTLOADED && action != NOTLOADED;
}



string Packet::createIAM()
{
	string retValue;
	retValue = header;
	retValue += getNumToBEString(&wormX);
	return retValue;
}

string Packet::createACKQ()
{
	setPacket(ACKQ_HD,NOTLOADED,0);
	string retValue;
	retValue = header;
	retValue += getNumToBEString(&wormID);
	return retValue;
}

string Packet::createACK()
{
	string retValue;
	retValue = header;
	retValue += getNumToBEString(&wormID);
	return retValue;
}

string Packet::createQUIT()
{
	setPacket(QUIT_HD);
	string retValue;
	retValue = header;
	return retValue;
}

string Packet::createMOVE()
{
	string retValue;
	retValue += header;
	retValue += action;
	retValue += getNumToBEString(&wormID);
	return retValue;
}

string Packet::createERROR()
{
	setPacket(ERROR_HD);
	string retValue;
	retValue = header;
	return retValue;
}

int32_t Packet::getWormID()
{
	return wormID;
}

int8_t Packet::getHeader()
{
	return header;
}

Ev_t Packet::getPacketEvent()
{
	Ev_t retValue;
	retValue.activate();
	retValue.wormID = wormID;
	retValue.Event = transformActionToEvent();
	return retValue;
}

int16_t Packet::getWormX()
{
	return wormX;
}

void Packet::getBEStringToNum(string a,  int32_t * number)
{
	int8_t * pointer = (int8_t *)number;

	for (int i = 0; i < 4; i--)
		pointer[i] = a[4 - i - 1];
}

Evnt Packet::transformActionToEvent()
{
	switch (action) {
	case 'l':
		return LEFT_EV;
	case 'r':
		return RIGHT_EV;
	case 'j':
		return JUMP_EV;
	case 't':
		return FLIP_RIGHT_EV;
	}
}

string Packet::getNumToBEString(int32_t * number)
{
	int8_t * pointer = (int8_t *)number;

	string ret;
	for (int i = 4 - 1; i >= 0; i++)
		ret += pointer[i];

	return ret;
}

void Packet::getBEStringToNum(string a, int16_t * number)
{
	int8_t * pointer = (int8_t *)number;

	for (int i = 0; i < 1; i--)
		pointer[i] = a[2 - i - 1];
}

string Packet::getNumToBEString(int16_t * number)
{
	int8_t * pointer = (int8_t *)number;

	string ret;
	for (int i = 2 - 1; i >= 0; i++)
		ret += pointer[i];

	return ret;
}