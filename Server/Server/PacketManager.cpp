#include "PacketManager.h"
#include "UserManager.h"


PacketManager::PacketManager()
{
}
int PacketManager::Init()
{
	InitializeCriticalSection(&mSc);
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;
	hcp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	if (hcp == NULL)
		return 1;

	SYSTEM_INFO si;
	GetSystemInfo(&si);
	HANDLE hThread;
	for (int i = 0; i < (int)si.dwNumberOfProcessors * 2; i++)
	{
		hThread=(HANDLE)_beginthreadex(NULL,0,WorkerThread,(LPVOID)hcp,0,NULL);
		if (hThread == NULL)
			return 1;
		CloseHandle(hThread);
	}
	listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET)
		err_quit("socket()");

	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = bind(listen_sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR)
		err_quit("bind()");

	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR)
		err_quit("listen()");
	UserManager::GetInstance()->Init();
	return 0;
}
void PacketManager::Accept()
{
	while (1)
	{
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (SOCKADDR*)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET)
		{
			err_display("accept()");
			break;
		}
		printf("[TCP 서버] 클라이언트 접속: IP 주소=%s, 포트 번호=%d\n",
			inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));


		CreateIoCompletionPort((HANDLE)client_sock, hcp, client_sock, 0);

		UserManager::GetInstance()->AddUser(client_sock);

		USERINFO* ptr = UserManager::GetInstance()->GetUser(client_sock);
		ZeroMemory(&ptr->overlapped, sizeof(ptr->overlapped));
		ptr->wsabuf.buf = ptr->tmpbuf;
		ptr->wsabuf.len = BUFSIZE;
		flags = 0;
		retval = WSARecv(client_sock, &ptr->wsabuf, 1, &ptr->bytes,
			&flags, &ptr->overlapped, NULL);
		if (retval == SOCKET_ERROR) 
		{
			if (WSAGetLastError() != ERROR_IO_PENDING) 
			{
				err_display("WSARecv()");
			}
			continue;
		}
	}
}
void PacketManager::ProcessPacket(SOCKET sock,int retval)
{
	HEADER packet;
	EnterCriticalSection(&mSc);
	USERINFO* ptr = UserManager::GetInstance()->GetUser(sock);
	memcpy(&ptr->buf[ptr->len], ptr->tmpbuf, retval);
	memcpy(&packet, ptr->buf, sizeof(packet));
	ptr->len += retval;
	if (retval > packet.len)
	{
		ptr->len = retval;
	}
	if (ptr->len < packet.len)
	{
		memcpy(ptr->buf, ptr->tmpbuf, ptr->len);
		return;
	}
	else
	{
		while (1)
		{
			switch (packet.name)
			{
			case PACKET_HEADER_READY:
			{
				PACKET_READY ready;
				memcpy(&ready, ptr->buf, sizeof(ready));
				UserManager::GetInstance()->Ready(ptr->sock, ready.ready_info);
				break;
			}
			case PACKET_HEADER_INPUT:
			{
				PACKET_INPUT input;
				memcpy(&input, ptr->buf, sizeof(input));
				printf("소켓 %d :입력 %d \n", sock, input.input);
				UserManager::GetInstance()->SendInput(ptr->sock, input.input);
				break;
			}
			case PACKET_HEADER_LOGIN:
			{
				PACKET_LOGIN login;
				memcpy(&login, ptr->buf, sizeof(login));
				memcpy(ptr->ch_username, &ptr->buf[sizeof(login)], login.name_len);
				UserManager::GetInstance()->LogIn(ptr->sock);
				break;
			}
			case PACKET_HEADER_REQUEST_INFO:
			{
				PACKET_REQUEST_INFO info;
				memcpy(&info, ptr->buf, sizeof(info));
				UserManager::GetInstance()->LobbyIn(ptr->sock);
				break;
			}
			case PACKET_HEADER_LOBBY_CHAT:
			{
				PACKET_LOBBY_CHAT chat;
				memcpy(&chat, ptr->buf,sizeof(chat));
				memcpy(ptr->ch_chat, &ptr->buf[sizeof(chat)], chat.chat_len);
				ptr->i_chatlen = chat.chat_len;
				printf("소켓 %d: %s\n", ptr->sock, ptr->ch_chat);
				UserManager::GetInstance()->EchoChat(ptr->sock);
			}
			break;
			case PACKET_HEADER_ROOM_IN:
			{
				PACKET_ROOM_IN roomin;
				memcpy(&roomin, ptr->buf, sizeof(roomin));
				UserManager::GetInstance()->RoomIn(ptr->sock, roomin.room_num);
			}
			break;
			case PACKET_HEADER_ROOM_OUT:
			{
				PACKET_ROOM_OUT roomout;
				memcpy(&roomout, ptr->buf, sizeof(roomout));
				UserManager::GetInstance()->RoomOut(ptr->sock);
				break;
			}
			case PACKET_HEADER_GAMEOVER:
			{
				PACKET_GAMEOVER over;
				memcpy(&over, ptr->buf, sizeof(over));
				UserManager::GetInstance()->GameOver(ptr->sock);
				break;
			}

			case PACKET_HEADER_ATTACK:
			{
				PACKET_ATTACK attack;
				memcpy(&attack, ptr->buf, sizeof(attack));
				UserManager::GetInstance()->Attack(ptr->sock, attack.line);
				break;
			}
			}
			if (ptr->len > packet.len)
			{
				memcpy(ptr->buf, &ptr->buf[packet.len], retval);
				ptr->len -= packet.len;
				memcpy(&packet, ptr->buf, sizeof(packet));
				continue;
			}
				ptr->len = 0;
				break;
		}
	}
	LeaveCriticalSection(&mSc);
}
void PacketManager::RecvSet(SOCKET sock)
{
	USERINFO* ptr = UserManager::GetInstance()->GetUser(sock);
	ZeroMemory(&ptr->overlapped, sizeof(ptr->overlapped));
	ptr->wsabuf.buf = ptr->tmpbuf;
	ptr->wsabuf.len = BUFSIZE;
	flags = 0;
	retval = WSARecv(sock, &ptr->wsabuf, 1, &ptr->bytes,
		&flags, &ptr->overlapped, NULL);
	if (retval == SOCKET_ERROR)
	{
		if (WSAGetLastError() != ERROR_IO_PENDING)
		{
			err_display("WSARecv()");
		}
	}
}
UINT WINAPI PacketManager::WorkerThread(LPVOID arg)
{
	int retval;
	HANDLE hcp = (HANDLE)arg;
	DWORD cbTransferred;
	SOCKET client_sock;
	USERINFO* ptr;
	SOCKADDR_IN clientaddr;
	while (1)
	{
		retval = GetQueuedCompletionStatus(hcp, &cbTransferred,
			(LPDWORD)&client_sock, (LPOVERLAPPED *)&ptr, INFINITE);
		ptr = UserManager::GetInstance()->GetUser(client_sock);
		if (ptr == NULL)
			continue;
		int addrlen = sizeof(clientaddr);
		getpeername(ptr->sock, (SOCKADDR *)&clientaddr, &addrlen);
		
		if (retval == 0 || cbTransferred == 0)
		{
			if (retval == 0) {
				DWORD temp1, temp2;
				WSAGetOverlappedResult(ptr->sock, &ptr->overlapped,
					&temp1, FALSE, &temp2);
				PacketManager::GetInstance()->err_display("WSAGetOverlappedResult()");
			}
			closesocket(ptr->sock);
			printf("[TCP 서버] 클라이언트 종료: IP 주소=%s, 포트 번호=%d\n",
				inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
			UserManager::GetInstance()->Remove(ptr->sock);
			continue;
		}
		
		PacketManager::GetInstance()->ProcessPacket(client_sock, cbTransferred);
		PacketManager::GetInstance()->RecvSet(client_sock);
		
	}
	return 0;
}
void PacketManager::err_quit(char *msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
}


void PacketManager::err_display(char *msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	printf("[%s] %s", msg, (char *)lpMsgBuf);
	LocalFree(lpMsgBuf);
}

PacketManager::~PacketManager()
{
	DeleteCriticalSection(&mSc);
}
