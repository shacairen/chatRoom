#include"Msg.h"

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

int Msg::GetFlag() {
	return flag;
}

string Msg::GetTime() {
	string str = time;
	return str;
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