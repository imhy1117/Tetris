#pragma once
#include "Define.h"
#include <map>
#include "../../com/Packet.h"
using namespace std;
class Room
{
private:
	map<SOCKET, USERINFO*>m_mapPlayer;
	int m_iPlayerNum,m_iGameOverCount;
	bool m_bOpen,m_bGameStart;
public:
	void Init();
	void AddPlayer(SOCKET sock);
	bool FindPlayer(SOCKET sock);
	void StartGame();
	inline int GetRoomStat()
	{
		if (m_bGameStart == true)
			return ROOM_PLAYING;
		else
			return ROOM_WAIT;
	}
	inline int GetRoomPlayerNum()
	{
		return m_iPlayerNum;
	}
	void Ready(SOCKET sock, BOOL ready);
	void SendInput(SOCKET sock, WORD index);
	void Attack(SOCKET sock, int line);
	void GameOver(SOCKET sock);
	void RemovePlayer(SOCKET sock);
	void Release();
	Room();
	~Room();
};

