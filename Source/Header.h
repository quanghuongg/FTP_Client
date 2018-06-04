#include<stdio.h>
#include<conio.h>
#include<iostream>
#include<Winsock2.h>
#include<string>
#include<vector>
#include<direct.h>
#pragma comment(lib,"ws2_32.lib") //Winsock Library
using namespace std;
void receive(SOCKET s, char *buff);
int getNumber(char *buff);
int Login(SOCKET s);
SOCKET sendPort(SOCKET s, int port);
void sendLs(SOCKET s, bool isActive);
void sendDir(SOCKET s, bool isActive);
void sendPut(SOCKET s, char*name, bool isActive);
void sendGet(SOCKET s, char *name, bool isActive);
void sendMput(SOCKET s, vector<char*> name, bool isActive);
void sendMget(SOCKET s, vector<char*> name, bool isActive);
void sendCd(SOCKET s, char *name);
void sendLcd(SOCKET s, char *name);
void sendDelete(SOCKET s, char *name);
void sendMdelete(SOCKET s, vector<char*> name);
void sendMkdir(SOCKET s, char *name);
SOCKET sendPasv(SOCKET s);
void sendRmdir(SOCKET s, char *name);
void sendPwd(SOCKET s);
void sendQuit(SOCKET s);
void exitSocket(SOCKET s);


