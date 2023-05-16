#include"UserLink.h"

UserLink::UserLink() {
	_head = New<User>();
	_head->next = nullptr;
}

bool UserLink::InsertUser(User *u) {
	if (_head == nullptr)
		return false;
	User *p = _head->next;
	_head->next = u;
	u->next = p;
	return true;
}


void UserLink::DeleteUser(User *u) {
	User *p = _head->next;
	User *q = _head;
	while (p) {
		if (p->_sk == u->_sk)
			break;
		p = p->next;
		q = q->next;
	}
	if (p == nullptr)
		return;
	q->next = p->next;
	delete p;
}

User* UserLink::GetHead() {
	return _head;
}

void UserLink::ShowAllUser() {
	User *p = _head->next;
	while (p!=nullptr) {
		cout << "SOCKET:" << p->_sk << endl;
		cout << "NAME:" << p->name << endl;
		p = p->next;
	}
}

Msg::Msg() {

}

Msg::~Msg() {

}

string Msg::GetAccount() {
	string str = account;
	return str;
}

string Msg::GetPassword() {
	string str = password;
	return str;
}


int Msg::GetWork() {
	return work;
}

string Msg::GetMess() {
	string str = mess;
	return str;
}

string Msg::GetFromName() {
	string str = from;
	return str;
}

string Msg::GetName() {
	string str = name;
	return str;
}

string Msg::GetToName() {
	string str = to;
	return str;
}

string Msg::GetTime() {
	string str = time;
	return str;
}

int Msg::GetFlag() {
	return flag;
}

void Msg::WriteWork(int w) {
	work = w;
}

void Msg::WriteFromName(string _from) {
	strcpy(from, _from.c_str());
}

void Msg::WriteToName(string _to) {
	strcpy(to, _to.c_str());
}

void Msg::WriteMess(string _mess) {
	strcpy(mess, _mess.c_str());
}

void Msg::WriteName(string n) {
	strcpy(name, n.c_str());
}

void Msg::WriteTime(string _time) {
	strcpy(time, _time.c_str());
}

void Msg::WriteFlag(int f) {
	flag = f;
}

void Msg::WriteAccount(string a) {
	strcpy(account, a.c_str());
}
void Msg::WritePassword(string p) {
	strcpy(password, p.c_str());
}
