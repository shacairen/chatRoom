#pragma once
#include"UserLink.h"

static UserLink ul;

void ChatAll(SOCKET s,Msg *m);

bool CheckPwd(Msg *m,string &str);

int CheckUser(Msg *m);

void NewUser(SOCKET s,Msg *m);

void ChatPic(SOCKET s, Msg *m);

static void enterAccount(SOCKET _client, Msg *msg)//µ«»Î
{
	string str;
	if (CheckPwd(msg,str)) {
		User *u = New<User>();
		u->name = str;
		u->_sk = _client;
		//u->IsInChat = 1;
		ul.InsertUser(u);
		ul.ShowAllUser();
		msg->WriteName(str);
		msg->WriteFlag(1);
		send(_client, (char *)msg, sizeof(Msg), 0);
	}
	else {
		msg->WriteFlag(3);
		send(_client, (char *)msg, sizeof(Msg), 0);
	}
}

static void GetOnlineUser(SOCKET s,Msg *m) {
	User *u = ul.GetHead()->next;
	while (u != nullptr) {
		if (u->IsInChat == 1) {
			m->WriteName(u->name);
			m->WriteWork(6);
			send(s, (char*)m,sizeof(Msg), 0);
		}
		u = u->next;
	}
}

static void ServerAction(SOCKET c) {
	cout << "Action client is:" << c << endl;
	while (1)
	{
		Msg m;
		if (recv(c,(char *)&m,sizeof(Msg), 0) <= 0)
		{
			cout << "Clientœ¬œﬂ" << endl;
			break;
		}
		switch (m.GetWork()) {
		case 1: NewUser(c,&m); break;
		case 2: enterAccount(c,&m); break;
		case 3: ChatAll(c,&m); break;
		case 4: ChatPic(c, &m); break;
		case 6: GetOnlineUser(c,&m); break;
		}
	}
}

static void ChatAll(SOCKET s,Msg *m) {
	User *u = ul.GetHead()->next;
	while (u != nullptr) {
		if (u->_sk == s) {
			u->IsInChat = 1;
			break;
		}
		u = u->next;
	}
	u = ul.GetHead()->next;
	while (u) {
		if (u->IsInChat == 1) {
			send(u->_sk,(char *)m, sizeof(Msg), 0);
		}
		u = u->next;
	}
}

static void NewUser(SOCKET s,Msg *m) {
	int res = CheckUser(m);
	m->WriteFlag(res);
	send(s, (char *)m, sizeof(Msg), 0);
}

static void ChatPic(SOCKET s, Msg *m) {
	User *u = ul.GetHead()->next;
	while (u != nullptr) {
		if (u->_sk == s) {
			u->IsInChat = 1;
			break;
		}
		u = u->next;
	}
	u = ul.GetHead()->next;
	while (u) {
		if (u->IsInChat == 1) {
			send(u->_sk, (char *)m, sizeof(Msg), 0);
		}
		u = u->next;
	}
}

static int CheckUser(Msg *m) {
	MYSQL mysql;
	MYSQL_RES *res;
	MYSQL_ROW row;

	int num = 1;

	mysql_init(&mysql);

	mysql_options(&mysql, MYSQL_SET_CHARSET_NAME, "gbk");

	if (mysql_real_connect(&mysql, "127.0.0.1", "root", "Wyy846159", "login", 3306, NULL, 0) == NULL) {
		cout << "fail!" << endl;
		exit(-1);
	}

	int ret = mysql_query(&mysql, "select * from user;");

	res = mysql_store_result(&mysql);

	while (row = mysql_fetch_row(res)) {
		if (strcmp(row[0], m->GetAccount().c_str()) == 0) {
			num = 2;
			break;
		}
		if (strcmp(row[2], m->GetName().c_str()) == 0) {
			num = 3;
			break;
		}
	}
	mysql_free_result(res);
	if (num == 1) {
		char Buff[128];

		sprintf_s(Buff, "INSERT INTO user VALUES('%s','%s','%s')", m->GetAccount().c_str(), m->GetPassword().c_str(), m->GetName().c_str());

		mysql_query(&mysql, Buff);
	}
	mysql_close(&mysql);

	return num;

}

static bool CheckPwd(Msg *m,string &str) {

	MYSQL mysql;
	MYSQL_RES *res;
	MYSQL_ROW row;

	bool IsReal = false;

	mysql_init(&mysql);

	mysql_options(&mysql, MYSQL_SET_CHARSET_NAME, "gbk");

	if (mysql_real_connect(&mysql, "127.0.0.1", "root", "Wyy846159", "login", 3306, NULL, 0) == NULL) {
		cout << "fail!" << endl;
		exit(-1);
	}

	int ret = mysql_query(&mysql, "select * from user;");

	res = mysql_store_result(&mysql);

	while (row = mysql_fetch_row(res)) {
		if (strcmp(row[0], m->GetAccount().c_str()) == 0 && strcmp(row[1],m->GetPassword().c_str()) == 0) {
			IsReal = true;
			str = row[2];
			break;
		}
	}
	mysql_free_result(res);
	mysql_close(&mysql);

	return IsReal;
}