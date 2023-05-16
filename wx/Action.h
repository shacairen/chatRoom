#pragma once
#include"Msg.h"
#include"ThreadPool.h"

static ThreadPool tp;
static string MyName;


void ChatAll(SOCKET s);
void ShowList(SOCKET s);

static int Enter(SOCKET s,Msg &m) {
	m.WriteWork(2);//此处选择要做的工作
	//m.WriteFromName(MyName);
	send(s, (char *)&m, sizeof(Msg), 0);
	if (recv(s, (char *)&m, sizeof(Msg), 0) <= 0)
	{
		return -1;
	}
	if (m.GetFlag() == 1) {
		MyName = m.GetName();
		return 1;
	}
	else{
		return 3;
	}
	return 1;
}

static int NewUser(SOCKET s,Msg &m) {
	m.WriteWork(1);

	send(s, (char *)&m, sizeof(Msg), 0);//注册

	if (recv(s, (char *)&m, sizeof(Msg), 0) <= 0)
	{
		return -1;
	}
	if (m.GetFlag() == 1) {
		return 1;
	}
	else if (m.GetFlag() == 2) {
		return 2;
	}
	else {
		return 3;
	}
	return 1;
}

/*static void ClientAction(SOCKET s) {
	Msg m;
	while (true) {
		int action = 0;
		cout << "请选择你要进行的工作-------------" << endl;
		cout << "输入1代表了注册==============" << endl;
		cout << "输入2代表了登录==============" << endl;
		cin >> action;
		switch (action)
		{
		case 1: NewUser(s);
			break;
		case 2:
			if (Enter(s,m)) {
				tp.PushFront(&RecvClient, s);
				ChatAll(s);
			}
			break;
		}
	}
}*/


static void ChatAll(SOCKET s) {
	Msg m;
	while (1) {

		m.WriteFlag(0);

	
	}
}

static void ShowList(SOCKET s) {
	Msg m;
	m.WriteWork(6);
	cout << "在线名单如下:" << endl;
	send(s, (char *)&m, 1024, 0);
}