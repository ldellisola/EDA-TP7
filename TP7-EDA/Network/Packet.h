#pragma once
#include <cstdint>
#include <string>
#include "../Events/Ev_t.h"
using namespace std;

#define ACKQ_HD (0x00)
#define ACK_HD (0x01)
#define IAM_HD (0x02)
#define MOVE_HD (0x03)
#define QUIT_HD (0xFF)
#define ERROR_HD (0xE0)

#define IDSIZE (4)
#define NOTLOADED (-1)	// Ya se quu va a ir a una variable positiva, pero esto me va a dar un valor muy alto que me sirve como base


class Packet
{
public:
	Packet();
	~Packet();
	void clear();
	void setPacket(int8_t type, int8_t action_ = NOTLOADED, uint32_t wormID_ = NOTLOADED, uint16_t wormX_ = NOTLOADED);
	void setPacket(string packet);
	bool isPacketClear();
	string createIAM();
	string createACK();
	string createACKQ();
	string createQUIT();
	string createMOVE();
	string createERROR();
	uint32_t getWormID();
	int8_t getHeader();
	Ev_t getPacketEvent();
	uint16_t getWormX();

private:
	Evnt transformActionToEvent();
	string getNumToBEString(uint32_t * number);
	string getNumToBEString(uint16_t * number);
	void getBEStringToNum(string a, uint32_t * number);
	void getBEStringToNum(string a, uint16_t * number);
	uint32_t wormID = NOTLOADED;
	uint16_t wormX = NOTLOADED;
	int8_t header = NOTLOADED;
	int8_t action = NOTLOADED;
	

	
	
};

