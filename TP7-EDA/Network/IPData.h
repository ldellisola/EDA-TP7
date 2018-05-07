#pragma once
#include <string>
#include <fstream>
#include <vector>

using namespace std;

typedef struct IPData
{
	string ip;
	bool imServer;
	int ipPosition;
	vector<string> ipList;
	int ammountOfIPs;

	void init(const char * ipFile)
	{
		char buff[200];
		bool matchNotFound = true;
		ifstream source;
		source.open(ipFile, ifstream::in);

		for (int i = 0; source.good(); i++) {
			source.getline(buff, 200, '\n');
			ipList.push_back(buff);
		}
		source.close();


		ammountOfIPs = ipList.size();
		int i = 1;
		for (string ip_ : ipList) {

			if (!ip.compare(ip_.c_str()))
				ipPosition = i;
			i++;
		}
	}
	string getOtherIP() {
		if (ipPosition == 1)
		return ipList[1];
		if (ipPosition == 2)
			return ipList[0];
	}


};