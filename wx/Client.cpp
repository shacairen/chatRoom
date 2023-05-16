#include "Client.h"


Client::Client() {

	WORD w = MAKEWORD(2, 2);
	WSADATA wsa;
	WSAStartup(w, &wsa);

	SOCKET socket_ = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_ == SOCKET_ERROR)
		cout << "socket����ʧ��" << endl;

	sockaddr_in addr = {};
	addr.sin_family = AF_INET;//Э����
	addr.sin_port = htons(1111);//�˿ں�ת����
	addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

	int ret = connect(socket_, (struct sockaddr *)&addr, sizeof(addr));
	if (ret == -1)
		cout << "Client����ʧ��" << endl;

	_clientSocket = socket_;

	//Start();
}


SOCKET Client::GetSocket() {
	return _clientSocket;
}