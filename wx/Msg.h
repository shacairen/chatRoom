#pragma once

#include<windows.h>
#include<WinSock2.h>
#include<iostream>
#include<cstdio>
#include<string>
#include<fstream>

#pragma comment(lib,"ws2_32.lib")

using std::endl;
using std::cout;
using std::cin;
using std::string;

class Msg {
private:
	int work;
	int flag;
	char mess[1024];//��Ϣ
	char name[30];//����
	char from[30];//������
	char to[30];//������
	char time[30];//ʱ��
	char account[30];//�˺�
	char password[30];//����
public:
	Msg();
	~Msg();
	int GetWork();
	int GetFlag();
	string GetAccount();
	string GetPassword();
	string GetMess();
	string GetFromName();
	string GetToName();
	string GetTime();
	string GetName();
	void WriteMess(string m);
	void WriteFromName(string f);
	void WriteToName(string t);
	void WriteTime(string t);
	void WriteWork(int w);
	void WriteFlag(int f);
	//void Clear();
	void WriteName(string n);
	void WriteAccount(string a);
	void WritePassword(string p);
};
