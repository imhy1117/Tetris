#pragma once
#pragma comment(lib,"ws2_32")
#include <WinSock2.h>
#include <stdlib.h>
#include <stdio.h>
#define SERVERPORT 9000
#define BUFSIZE 512
class Room;
struct USERINFO
{
	OVERLAPPED overlapped;
	SOCKET sock;
	DWORD bytes;
	bool ready;
	int len;
	char ch_username[16];
	char ch_chat[BUFSIZE + 1];
	int i_chatlen;
	char buf[BUFSIZE + 1];
	char tmpbuf[BUFSIZE + 1];
	WSABUF wsabuf;
	Room* pCurRoom;
};