#pragma once
#include <cstdint>
#include <string>
using namespace std;

#define ACKQ_HD (0x00)
#define ACK_HD (0x01)
#define IAM_HD (0x02)
#define MOVE_HD (0x03)
#define QUIT_HD (0xFF)
#define ERROR_HD (0xE0)

#define IDSIZE (4)
#define NOTLOADED (-1)


class Packet
{
public:
	Packet();
	~Packet();
	void clear();
	void setPacket(int8_t type, int8_t action_ = 0, int32_t wormID_ = 0, int16_t wormX_ = 0, int16_t wormY_ = 0);
	void setPacket(string packet);
	bool isPacketClear();
	string createIAM();
	string createACK();
	string createACKQ();
	string createQUIT();
	string createMOVE();
	string createERROR();

private:
	
	string getNumToBEString(int32_t * number);
	string getNumToBEString(int16_t * number);
	void getBEStringToNum(string a, int32_t * number);
	void getBEStringToNum(string a, int16_t * number);
	int32_t wormID = NOTLOADED;
	int16_t wormX = NOTLOADED;
	int8_t header = NOTLOADED;
	int8_t action = NOTLOADED;
	

	
	
};

