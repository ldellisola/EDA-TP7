#include "Client.h"


using namespace boost::asio;
Client::Client(string ip, const char * port_)
{
	this->IOHandler = new io_service();
	this->clientSocket = new ip::tcp::socket(*IOHandler);
	this->clientResolver = new ip::tcp::resolver(*IOHandler);

	ipToConect = ip;
	port = port_;
}

//using namespace boost::asio;
void Client::link()
{

	auto q = ip::tcp::resolver::query(ipToConect.c_str(), this->port);
	this->endpoint = clientResolver->resolve(q);
	cout << "Waiting for server to connect" << endl;
	connect(*clientSocket, endpoint);
	clientSocket->non_blocking(true);
	

	cout << "Client trying to connect to " << ipToConect.c_str() << endl;
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

std::string Client::getInfo() {

	char buffer[1 + 255 + 1];
	size_t lenght = 0;
	boost::system::error_code error;

	do {
		lenght = this->clientSocket->read_some(boost::asio::buffer(buffer), error);
	} while (error);

	buffer[lenght] = 0;
	std::string retValue = buffer;
	std::cout << "Recieved a message" << std::endl;

	return retValue;
}

string Client::getInfoTimed(int ms)
{
	Timer timer;



	char buffer[1 + 255 + 1];
	size_t lenght = 0;
	boost::system::error_code error;

	timer.start();

	bool timeout = false;

	do {
		lenght = this->clientSocket->read_some(boost::asio::buffer(buffer), error);
		timer.stop();
		if (timer.getTime() > ms && lenght == 0) { // Pido que lenght == 0 asi no lo paro mientras esta mandando
			timeout = true;
		}

	} while (error && !timeout);
	std::string retValue;

	if (!timeout) {
		buffer[lenght] = 0;

		for (int i = 0; i < lenght; i++) {
			retValue.push_back(buffer[i]);
		}

		//retValue = buffer;
		std::cout << "Recieved a message" << std::endl;
	}
	else
		retValue = CLIENT_TIMEOUT;

	return retValue;
}

bool Client::getInfoSigle(string & msg)
{
	bool returnError = true;

	char buffer[1 + 255 + 1];
	size_t lenght = 0;
	boost::system::error_code error;

	lenght = this->clientSocket->read_some(boost::asio::buffer(buffer), error);

	if (!error) {
		buffer[lenght] = 0;
		
		for (int i = 0; i < lenght; i++) {
			msg.push_back(buffer[i]);
		}
		returnError = false;
		cout << "Message Recieved" << endl;
	}


	return returnError;
}

Client::~Client()
{
	std::cout << "Closing connection" << std::endl;
	this->clientSocket->close();
	delete this->clientResolver;
	delete this->clientSocket;
	delete this->IOHandler;
}
