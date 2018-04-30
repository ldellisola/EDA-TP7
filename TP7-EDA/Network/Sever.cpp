#include "Server.h"

Server::Server(std::string port) {

	this->IO_handler = new boost::asio::io_service();
	this->serverSocket = new boost::asio::ip::tcp::socket(*(this->IO_handler));
	this->serverAcceptor = new boost::asio::ip::tcp::acceptor(*(this->IO_handler), 
	boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), std::stoi(port)));

}

Server::~Server() {

	std::cout << "Disconnecting.." << std::endl;
	this->serverAcceptor->close();
	this->serverSocket->close();
	delete this->serverAcceptor;
	delete this->serverSocket;
	delete this->IO_handler;

}

void Server::connect() {

	std::cout << "Waiting for somebody to connect.. :( " << std::endl;
	serverSocket->non_blocking(true);
	this->serverAcceptor->accept(*(this->serverSocket));

}

std::string Server::getInfo() {

	char buffer[1 + 255 + 1];
	size_t lenght = 0;
	boost::system::error_code error;

	do {
		lenght = this->serverSocket->read_some(boost::asio::buffer(buffer), error);
	} while (error);

	buffer[lenght] = 0;
	std::string retValue = buffer;
	std::cout << "Recieved a message" << std::endl;

	return retValue;
}

std::string Server::getInfoTimed(int ms)
{
	Timer timer;

	

	char buffer[1 + 255 + 1];
	size_t lenght = 0;
	boost::system::error_code error;

	timer.start();

	bool timeout = false;

	do {
		lenght = this->serverSocket->read_some(boost::asio::buffer(buffer), error);
		timer.stop();
		if (timer.getTime() > ms && lenght == 0) { // Pido que lenght == 0 asi no lo paro mientras esta mandando
			timeout = true;
		}

	} while (error && !timeout);
	std::string retValue;

	if (!timeout) {
		buffer[lenght] = 0;
		retValue = buffer;
		std::cout << "Recieved a message" << std::endl;
	}
	else
		retValue = SERVER_TIMEOUT;

	return retValue;
}
