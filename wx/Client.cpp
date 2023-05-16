#include "Client.h"


Client::Client() {

	WORD w = MAKEWORD(2, 2);
	WSADATA wsa;
	WSAStartup(w, &wsa);

	SOCKET socket_ = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_ == SOCKET_ERROR)
		cout << "socket申请失败" << endl;

	sockaddr_in addr = {};
	addr.sin_family = AF_INET;//协议族
	addr.sin_port = htons(1111);//端口号转换。
	addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

	int ret = connect(socket_, (struct sockaddr *)&addr, sizeof(addr));
	if (ret == -1)
		cout << "Client连接失败" << endl;

	_clientSocket = socket_;

	//Start();
}


SOCKET Client::GetSocket() {
	return _clientSocket;
}