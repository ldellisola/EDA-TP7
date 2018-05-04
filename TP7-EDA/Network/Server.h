#pragma once

#include <boost/asio.hpp>
#include <string>
#include <cstdio>
#include "../Events/Timer.h"
#include <iostream>

#define SERVER_TIMEOUT "timeout"
using namespace std;

class Server {
public:
	Server(std::string port);
	~Server();
	void connect();
	string getInfo();
	bool getInfoSigle(string& msg);
	string getInfoTimed(int ms);
	void sendMessageTimed(int ms, string msg);


private:
	boost::asio::io_service* IO_handler;
	boost::asio::ip::tcp::socket* serverSocket;
	boost::asio::ip::tcp::acceptor* serverAcceptor;
};