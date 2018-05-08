#include "Packet.h"
#include "..\Network\Packet.h"

using namespace std;



uint8_t TransformEvent(Evnt ev) {
	switch (ev) {
	case LEFT_EV:
		return 'l';
	case RIGHT_EV:
		return 'r';
	case JUMP_EV:
		return 'j';
	case TOGGLE_EV:
		return 't';
	}

}

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

void Packet::setPacket(uint8_t type, uint8_t action_ , uint32_t wormID_ , uint16_t wormX_ ){
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

void Packet::setPacket(Ev_t event_)
{
	header = MOVE_HD;
	wormID = event_.wormID;
	action = TransformEvent(event_.Event);
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

uint32_t Packet::getWormID()
{
	return wormID;
}

uint8_t Packet::getHeader()
{
	return header;
}

Ev_t Packet::getPacketEvent()
{
	Ev_t retValue;
	retValue.activate();
	retValue.wormID = wormID;
	retValue.Event = transformActionToEvent();
	if (retValue.Event == NOEVENT ) {
		if (this->header == QUIT_HD) {
			cout << "QUIT Event recieved. Shutting down" << endl;
			retValue.Event = QUIT_EV;
		}
		else if (this->header == ERROR_HD) {
			cout << "Error Event recieved. Shutting down" << endl;
			retValue.Event = QUIT_EV;
		}

	}
		
	
	return retValue;
}

uint16_t Packet::getWormX()
{
	return wormX;
}

void Packet::getBEStringToNum(string a,  uint32_t * number)
{
	uint8_t * pointer = (uint8_t *)number;

	for (int i = 0; i < 4; i++)
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
		return TOGGLE_EV;
	default: return NOEVENT;
	}
}

string Packet::getNumToBEString(uint32_t * number)
{
	uint8_t * pointer = (uint8_t *)number;

	string ret;
	for (int i = 4 - 1; i >= 0; i--)
		ret += pointer[i];

	return ret;
}

void Packet::getBEStringToNum(string a, uint16_t * number)
{
	uint8_t * pointer = (uint8_t *)number;

	for (int i = 0; i <= 1; i++)
		pointer[i] = a[2 - i - 1];
}

string Packet::getNumToBEString(uint16_t * number)
{
	uint8_t * pointer = (uint8_t *)number;

	string ret;
	for (int i = 2 - 1; i >= 0; i--)
		ret += pointer[i];

	return ret;
}

ostream& operator<<(ostream& o, Packet& c)
{
	
	string head;
	string action;
	switch (c.getHeader())
	{
	case ACKQ_HD:

		head = "ACKQ";
		o << "header = "<< head <<"Worn ID = "<< c.getWormID() << endl;
		break;
	case ACK_HD:
		head = "ACK";
		o << "header = " << head << "Worn ID = " << c.getWormID() << endl;
		break;
	case IAM_HD:
		head = "IAM";
		o << "header = " << head << "Worn X = " << c.getWormX() << endl;
		break;
	case MOVE_HD:
		head = "MOVE";
		switch (c.transformActionToEvent())
		{
		case LEFT_EV:
			action = "LEFT";
			break;
		case RIGHT_EV:
			action = "RIGHT";
			break;
		case JUMP_EV:
			action = "JUMP";
			break;
		case FLIP_RIGHT_EV:
			action = "FLIP RIGHT";
		}
		o << "header = " << head << "action= " << action << endl;

		break;
	case QUIT_HD:
		head = "QUIT";
		o << head << endl;
		break;
	case ERROR_HD:
		head = "ERROR";
		o << head << endl;
		break;
	}

	return o;
}

