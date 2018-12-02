#pragma once
#include "Define.h"
#include <process.h>
#include "SingleTon.h"

#define SERVERPORT 9000
#define BUFSIZE 512
using namespace std;
class PacketManager:public SingleTon<PacketManager>
{
private:
	int addrlen;
	SOCKET client_sock;
	SOCKADDR_IN clientaddr;
	SOCKET listen_sock;
	HANDLE hcp;
	int retval;
	WSADATA wsa; 
	CRITICAL_SECTION mSc;
	DWORD recvbytes, flags;
public:
	PacketManager();
	int Init();
	void Accept();
	void ProcessPacket(SOCKET sock,int retval);
	static UINT WINAPI WorkerThread(LPVOID arg);
	void RecvSet(SOCKET sock);
	void err_quit(char *msg);
	void err_display(char *msg);
	~PacketManager();
};

