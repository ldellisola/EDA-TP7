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

class Client
{
public:
	Client();
	void link(const char * host, const char * port);
	//string recieveMessage();
	void sendMessage(string msg);
	bool sendMessageTimed(string msg, int ms);
	~Client();
private:
	
	io_service* IOHandler;
	ip::tcp::socket* clientSocket;
	ip::tcp::resolver* clientResolver;
	ip::tcp::resolver::iterator endpoint;
};

