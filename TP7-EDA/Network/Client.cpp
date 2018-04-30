#include "Client.h"


using namespace boost::asio;
Client::Client()
{
	this->IOHandler = new io_service();
	this->clientSocket = new ip::tcp::socket(*IOHandler);
	this->clientResolver = new ip::tcp::resolver(*IOHandler);
}

//using namespace boost::asio;
void Client::link(const char * host, const char * port)
{
	auto q = ip::tcp::resolver::query(host, port);
	this->endpoint = clientResolver->resolve(q);
	clientSocket->non_blocking(true);
	connect(*clientSocket, endpoint);
	

	cout << "Client trying to connect to " << host << endl;
}

void Client::sendMessage(string msg)
{
	std::cout << "Trying to send message" << std::endl;
	size_t lenght = 0;
	boost::system::error_code error;

	do {
		lenght = this->clientSocket->write_some(boost::asio::buffer(msg, msg.size()), error);
	} while (error);

	std::cout << "Message sent" << std::endl;


}

bool Client::sendMessageTimed(string msg, int ms)
{
	std::cout << "Trying to send message" << std::endl;
	Timer timer;
	timer.start();
	bool timeout = false;

	size_t lenght = 0;
	boost::system::error_code error;

	do {
		lenght = this->clientSocket->write_some(boost::asio::buffer(msg, msg.size()), error);
		timer.stop();
		if (timer.getTime() > ms && lenght == 0)
			timeout = true;
	} while (error && !timeout);

	return !timeout;
}

Client::~Client()
{
	std::cout << "Closing connection" << std::endl;
	this->clientSocket->close();
	delete this->clientResolver;
	delete this->clientSocket;
	delete this->IOHandler;
}
