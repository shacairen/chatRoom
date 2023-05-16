// ChatRoom.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include"Server.h"

Server::Server():tp(){

}

SOCKET Server::GetSocket() {
	WORD w = MAKEWORD(2, 2);
	WSADATA wsa;
	WSAStartup(w, &wsa);

	SOCKET socket_ = socket(AF_INET,SOCK_STREAM,0);
	if (socket_ == SOCKET_ERROR)
		cout << "socket申请失败" << endl;

	sockaddr_in addr = {};
	addr.sin_family = AF_INET;//协议族
	addr.sin_port = htons(1111);//端口号转换。
	addr.sin_addr.S_un.S_addr = INADDR_ANY;

	if (bind(socket_, (sockaddr *)&addr, sizeof(addr)) == SOCKET_ERROR)
		cout << "绑定失败" << endl;

	if (listen(socket_, 100) == SOCKET_ERROR)
		cout << "监听失败" << endl;

	cout << "等待客户端的连接中=========" << endl;
	_s = socket_;
	return socket_;	
}

SOCKET Server::Accept_(SOCKET socket_) {
	sockaddr_in client_ = {};
	int len = sizeof(client_);
	SOCKET clientSocket = accept(socket_, (sockaddr *)&client_,&len);
	if (clientSocket == INVALID_SOCKET)
		cout << "接收client失败" << endl;
	cout << "成功接收一个客户端:" << inet_ntoa(client_.sin_addr) << endl;

	return clientSocket;
}



void Server::Start() {
	while (true) {
		SOCKET _client = Accept_(_s);
		tp.PushFront(&ServerAction, _client);
	}
}




// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
