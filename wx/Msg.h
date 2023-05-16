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
	char mess[1024];//信息
	char name[30];//姓名
	char from[30];//发送者
	char to[30];//接收者
	char time[30];//时间
	char account[30];//账号
	char password[30];//密码
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
