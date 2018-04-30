#pragma once

#include <boost/asio.hpp>
#include <string>
#include <cstdio>
#include "../Events/Timer.h"
#include <iostream>

#define SERVER_TIMEOUT "timeout"

class Server {
public:
	Server(std::string port);
	~Server();
	void connect();
	std::string getInfo();
	std::string getInfoTimed(int ms);

private:
	boost::asio::io_service* IO_handler;
	boost::asio::ip::tcp::socket* serverSocket;
	boost::asio::ip::tcp::acceptor* serverAcceptor;
};