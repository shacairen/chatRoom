#include"Server.h"


int main() {
	
	Server s;
	SOCKET sk=s.GetSocket();
	s.Start();
}