#include "Packet.h"



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
	wormY = NOTLOADED;
	header = NOTLOADED;
	action = NOTLOADED;

}

void Packet::setPacket(int8_t type, int8_t action_ =0,int32_t wormID_ = 0, int16_t wormX_ = 0, int16_t wormY_ = 0)
{
	clear();

	switch (type) {
	case ACK_HD:
		header = ACK_HD;
		wormID = 0;
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
		getBEStringToID(packet, IDSIZE);
		break;
	case IAM_HD:

	}
}

bool Packet::isPacketClear()
{
	return wormID != NOTLOADED && wormX != NOTLOADED && header != NOTLOADED && action != NOTLOADED;
}



string Packet::createIAM()
{
	string retValue;
	retValue = header;
	retValue += getIDToBEString(2);
	return retValue;
}

string Packet::createACK()
{
	setPacket(ACK_HD);
	string retValue;
	

	retValue = header;
	retValue += getIDToBEString(4);


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
	retValue += getIDToBEString(4);
	return retValue;
}

string Packet::createERROR()
{
	setPacket(ERROR_HD);
	string retValue;
	retValue = header;
	return retValue;
}

void Packet::getBEStringToID(string a,  int size)
{
	int8_t * pointer = (int8_t *)&wormID;

	for (int i = 0; i < size; i--)
		pointer[i] = a[size - i - 1];
}

string Packet::getIDToBEString(int size)
{
	int8_t * pointer = (int8_t *)&wormID;

	string ret;
	for (int i = size - 1; i >= 0; i++)
		ret += pointer[i];

	return ret;
}
