#pragma comment(lib,"ws2_32")
#include <WinSock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <map>
#include "UserManager.h"
#include "PacketManager.h"
#define SERVERPORT 9000
#define BUFSIZE 512

using namespace std;

int main(int argc, char *argv[])
{
	if (PacketManager::GetInstance()->Init() == TRUE)
		return 0;
	PacketManager::GetInstance()->Accept();
	UserManager::GetInstance()->Release();
	PacketManager::GetInstance()->DestroyInstance();
	WSACleanup();
	return 0;
}






