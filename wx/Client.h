#pragma once

#include"Action.h"

class Client {
private:
	SOCKET _clientSocket;
public:
	Client();
	SOCKET GetSocket();
};

