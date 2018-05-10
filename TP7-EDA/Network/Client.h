#pragma once

#include <iostream>
#include <cstdio>
#include <boost/asio.hpp>
#include <boost/chrono.hpp>
#include <boost/timer/timer.hpp>
#include <string>
#include "../Events/Timer.h"
using namespace std;
using namespace boost::asio;

#define CLIENT_TIMEOUT "timeout"

class Client
{
public:
	Client(string ip, const char * port_);
	void link();
	//string recieveMessage();
	void sendMessage(string msg);
	bool sendMessageTimed(string msg, int ms);
	std::string getInfo();
	string getInfoTimed(int ms);

	bool getInfoSingleTry(string & msg);

	~Client();
private:
	string ipToConect;
	const char * port;
	io_service* IOHandler;
	ip::tcp::socket* clientSocket;
	ip::tcp::resolver* clientResolver;
	ip::tcp::resolver::iterator endpoint;
};

