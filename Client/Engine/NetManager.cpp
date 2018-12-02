#include "NetManager.h"
#include "SceneManager.h"

NetManager::NetManager()
{
}
void NetManager::Init(HWND hWnd)
{
	m_hWnd = hWnd;
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return;
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
		err_quit("socket()");
}
bool NetManager::Connect(string serverip,int serverport)
{
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(serverip.c_str());
	serveraddr.sin_port = htons(serverport);
	retval = connect(sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR)
	{
		err_display("서버에 접속할 수 없습니다.");
		return false;
	}
	retval = WSAAsyncSelect(sock, m_hWnd, WM_SOCKET, FD_READ | FD_CLOSE);
	return true;
}
void NetManager::ProcessSocketMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (WSAGETSELECTERROR(lParam))
	{
		err_display(WSAGETSELECTERROR(lParam));
		return;
	}
	switch (WSAGETSELECTEVENT(lParam))
	{
	case FD_READ:
		if (SceneManager::GetInstance()->ProcessPacket(wParam))
			break;
		else
			SendMessage(hWnd, uMsg, wParam, lParam);
		break;

	case FD_CLOSE:
		closesocket(sock);
		break;
	}
}
void NetManager::err_quit(char *msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
}
void NetManager::err_display(char *msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
}
void NetManager::err_display(int errcode)
{
	TCHAR msg[256];
	wsprintf(msg, "%d", errcode);
	LPVOID lpMsgBuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, errcode,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);
	MessageBox(NULL, (LPCTSTR)lpMsgBuf,msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
}
void NetManager::Release()
{
	closesocket(sock);
	WSACleanup();
	DestroyInstance();
}

NetManager::~NetManager()
{
}
