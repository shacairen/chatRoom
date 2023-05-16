#pragma once

#include<stdio.h>
#include"ThreadPool.h"
#include"Action.h"



class Server {
private:
	SOCKET _s;
	ThreadPool tp;
public:
	Server();
	SOCKET GetSocket();
	SOCKET Accept_(SOCKET socket_);
	void Start();
};


