#pragma once
#include "Define.h"
#include <map>
#include <vector>
#include "SingleTon.h"
#include "../../com/Packet.h"
#define SERVERPORT 9000
#define BUFSIZE 512
using namespace std;
class Room;
class UserManager:public SingleTon<UserManager>
{
private:
	map<SOCKET, USERINFO*>m_mapUser;
	vector<Room*>m_vecRoom;
	bool m_bGameStart;
	int m_iUserNum,m_iPlayerNum,m_iGameOverCount,retval;
public:
	UserManager();
	void Init();
	void AddUser(SOCKET sock);
	USERINFO*  GetUser(SOCKET sock);
	void Attack(SOCKET sock, int line);
	void RoomIn(SOCKET sock, int roomnum);
	void RoomOut(SOCKET sock);
	void LobbyIn(SOCKET sock);
	void SendInput(SOCKET sock, WORD index);
	void LogOut(SOCKET sock);
	void LogIn(SOCKET sock);
	void EchoChat(SOCKET sock);
	void Ready(SOCKET sock,BOOL ready);
	void GameOver(SOCKET sock);
	void Remove(SOCKET sock);
	void Release();
	~UserManager();
};

