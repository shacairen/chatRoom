#pragma once
#include"Msg.h"
#include"ThreadPool.h"

static ThreadPool tp;
static string MyName;


void ChatAll(SOCKET s);
void ShowList(SOCKET s);

static int Enter(SOCKET s,Msg &m) {
	m.WriteWork(2);//�˴�ѡ��Ҫ���Ĺ���
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

	send(s, (char *)&m, sizeof(Msg), 0);//ע��

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
		cout << "��ѡ����Ҫ���еĹ���-------------" << endl;
		cout << "����1������ע��==============" << endl;
		cout << "����2�����˵�¼==============" << endl;
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
	cout << "������������:" << endl;
	send(s, (char *)&m, 1024, 0);
}